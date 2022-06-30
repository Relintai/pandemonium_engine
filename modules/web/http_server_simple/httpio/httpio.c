/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 by Sergey Fetisov <fsenok@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "httpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MEM_ALIGNMENT
#define MEM_ALIGNMENT 4
#endif

#define CODE_DONE      0
#define CODE_IN_EOF    1
#define CODE_OUT_EOF   2
#define CODE_CNT_LIM   3
#define CODE_ERROR     4
#define CODE_MEM_ERROR 5

#define STATE0_ERROR     -1
#define STATE0_RQLINE     0
#define STATE0_RQHDR      1
#define STATE0_URLENC     2
#define STATE0_MPART_DATA 3
#define STATE0_MPART_INIT 4
#define STATE0_MPART_HDR  5
#define STATE0_DONE       6
#define STATE0_RESET      7

#define STATE_RQLINE_METHOD  0
#define STATE_RQLINE_URI     1
#define STATE_RQLINE_KEY     2
#define STATE_RQLINE_VAL     3
#define STATE_RQLINE_VER     4
#define STATE_RQLINE_SKIPN   5

#define STATE_HDRRD_INIT     0
#define STATE_HDRRD_KEYRD    1
#define STATE_HDRRD_KEY      2
#define STATE_HDRRD_SKIPWS   3
#define STATE_HDRRD_VALRD    4
#define STATE_HDRRD_VAL      5
#define STATE_HDRRD_CSKIPWS  6
#define STATE_HDRRD_CVALRD   7
#define STATE_HDRRD_CVAL     8
#define STATE_HDRRD_SKIPN    9
#define STATE_HDRRD_LASTN    10
#define STATE_HDRRD_FINISHED 11

#define STATE_MPART_DATARD    0
#define STATE_MPART_NOTBNDRY  1
#define STATE_MPART_ENDCHECK  2
#define STATE_MPART_SKIPN     3
#define STATE_MPART_SKIPMINUS 4
#define STATE_MPART_SKIPR     5
#define STATE_MPART_LASTN     6
#define STATE_MPART_BNDRY     7
#define STATE_MPART_FINISHED  8

#define STATE_URLENC_INIT     0
#define STATE_URLENC_KEY      1
#define STATE_URLENC_CHUNK    2
#define STATE_URLENC_END      3
#define STATE_URLENC_FINISHED 4
#define STATE_URLENC_KEYRD    5
#define STATE_URLENC_VALRD    6

typedef struct field_info fld_t;

struct http_input
{
    struct
    {
        char *data;
        int   pos;
        int   size;
        int   tot;
    } in;
    struct
    {
        char *data;
        int pos;
        int size;
    } out;
    struct
    {
        char *data;
        int size;
        int used;
    } pool;
    struct
    {
        char stop;
    } lexrd;
    struct
    {
        char state;
    } rqline;
    struct
    {
        int state;
    } hdrrd;
    struct
    {
        int state;
        const fld_t *field;
    } rqhdr;
    struct
    {
        int state;
    } setter;
    struct
    {
        int state;
        int bndry_cntr;
        int bndry_pos;
        const char *bndry;
        int bndry_size;
    } mpart;
    struct
    {
        int state;
        char extra[4];
    } urlenc;
    http_req_t req;
    http_post_t post;
    int used;
    int state;
};

static const char *METHODS_STR[] =
{
    "",
    "GET",
    "POST",
    "HEAD",
    "PUT",
    "CONNECT",
    "OPTIONS",
    "DELETE",
    "TRACE",
    "PATCH"
};

static http_mt_t str_to_method(const char *str)
{
    int res;
    for (res = METHOD_GET; res <= METHOD_PATCH; res++)
        if (strcmp(METHODS_STR[res], str) == 0)
            return (http_mt_t)res;
    return METHOD_NONE;
}

bool str2hex(const char *str, uint8_t *hex)
{
    int d;

    d = str[0];
    if (d >= 'a') d -= 32; /* to upper case */
    if (d >= '0' && d <= '9')
        d -= '0'; else
    if (d >= 'A' && d <= 'F')
        d += 10 - 'A'; else
        return false;
    *hex = d << 4;

    d = str[1];
    if (d >= 'a') d -= 32; /* to upper case */
    if (d >= '0' && d <= '9')
        d -= '0'; else
    if (d >= 'A' && d <= 'F')
        d += 10 - 'A'; else
        return false;
    *hex |= d;

    return true;
}

int http_unescape(char *str, int len, int null)
{
    int res = 0;
    char *c = str;
    (void)len;
    while (*c != 0 && len > 0)
    {
        if (*c == '%' && len > 2)
        {
            uint8_t hex;
            if (str2hex(c + 1, &hex))
            {
                *str = hex;
                str++;
                res++;
                c += 3;
                len -= 3;
                continue;
            }
        }
        *str = *c == '+' ? ' ' : *c;
        str++;
        res++;
        c++;
        len--;
    }
    if (null) *str = 0;
    return res;
}

static http_ct_t str_to_ct(const char *str)
{
    if (strcmp(str, "close") == 0) return CT_CLOSE;
    if (strcmp(str, "keep-alive") == 0) return CT_KEEP_ALIVE;
    return CT_NONE;
}

static void httpi_acc_field_setter(httpi_t *httpi, int arg);
static void httpi_str_field_setter(httpi_t *httpi, int arg);
static void httpi_ct_field_setter(httpi_t *httpi, int arg);
static void httpi_cntt_field_setter(httpi_t *httpi, int arg);
static void httpi_int_field_setter(httpi_t *httpi, int arg);
static void httpi_post_cdh_setter(httpi_t *httpi, int arg);

#define OFFSET_REQ            offsetof(httpi_t, req)
#define OFFSET_FLD(name)      offsetof(http_req_t, name)
#define OFFSET_CONT_FLD(name) offsetof(http_cont_t, name)
#define OFFSET_POST           offsetof(httpi_t, post)
#define OFFSET_POST_FLD(name) offsetof(http_post_t, name)

struct field_info
{
    const char *name;
    int offset;
    void (*setter)(httpi_t *httpi, int arg);
    int composite;
};

struct field_info hdr_fields[] =
{
    /* hdr field name */
    { "Accept",              OFFSET_REQ + OFFSET_FLD(accept),                             httpi_acc_field_setter,  1 },
    { "Accept-Charset",      OFFSET_REQ + OFFSET_FLD(accept_charset),                      httpi_str_field_setter,  0 },
    { "Accept-Encoding",     OFFSET_REQ + OFFSET_FLD(accept_encoding),                     httpi_str_field_setter,  0 },
    { "Accept-Language",     OFFSET_REQ + OFFSET_FLD(accept_language),                     httpi_str_field_setter,  0 },
    { "Authorization",       OFFSET_REQ + OFFSET_FLD(authorization),                       httpi_str_field_setter,  0 },
    { "Connection",          OFFSET_REQ + OFFSET_FLD(connection),                          httpi_ct_field_setter,   0 },
    { "Expect",              OFFSET_REQ + OFFSET_FLD(expect),                              httpi_str_field_setter,  0 },
    { "From",                OFFSET_REQ + OFFSET_FLD(from),                                httpi_str_field_setter,  0 },
    { "Host",                OFFSET_REQ + OFFSET_FLD(host),                                httpi_str_field_setter,  0 },
    { "If-Match",            OFFSET_REQ + OFFSET_FLD(if_match),                            httpi_str_field_setter,  0 },
    { "If-Modified-Since",   OFFSET_REQ + OFFSET_FLD(if_modified_since),                   httpi_str_field_setter,  0 },
    { "If-None-Match",       OFFSET_REQ + OFFSET_FLD(if_none_match),                       httpi_str_field_setter,  0 },
    { "If-Range",            OFFSET_REQ + OFFSET_FLD(if_range),                            httpi_str_field_setter,  0 },
    { "If-Unmodified-Since", OFFSET_REQ + OFFSET_FLD(if_unmodified_since),                 httpi_str_field_setter,  0 },
    { "Max-Forwards",        OFFSET_REQ + OFFSET_FLD(max_forwards),                        httpi_int_field_setter,  0 },
    { "Proxy-Authorization", OFFSET_REQ + OFFSET_FLD(proxy_authorization),                 httpi_str_field_setter,  0 },
    { "Referer",             OFFSET_REQ + OFFSET_FLD(referer),                             httpi_str_field_setter,  0 },
    { "TE",                  OFFSET_REQ + OFFSET_FLD(te),                                  httpi_str_field_setter,  0 },
    { "User-Agent",          OFFSET_REQ + OFFSET_FLD(user_agent),                          httpi_str_field_setter,  0 },
    { "Cookie",              OFFSET_REQ + OFFSET_FLD(cookie),                              httpi_str_field_setter,  0 },
    { "Keep-Alive",          OFFSET_REQ + OFFSET_FLD(keep_alive),                          httpi_int_field_setter,  0 },
    { "Content-Type",        OFFSET_REQ + OFFSET_FLD(content),                             httpi_cntt_field_setter, 1 },
    { "Content-Encoding",    OFFSET_REQ + OFFSET_FLD(content) + OFFSET_CONT_FLD(encoding), httpi_str_field_setter,  0 },
    { "Content-Language",    OFFSET_REQ + OFFSET_FLD(content) + OFFSET_CONT_FLD(language), httpi_str_field_setter,  0 },
    { "Content-Length",      OFFSET_REQ + OFFSET_FLD(content) + OFFSET_CONT_FLD(length),   httpi_int_field_setter,  0 },
    { "Content-Location",    OFFSET_REQ + OFFSET_FLD(content) + OFFSET_CONT_FLD(location), httpi_str_field_setter,  0 },
    { "Content-MD5",         OFFSET_REQ + OFFSET_FLD(content) + OFFSET_CONT_FLD(md5),      httpi_str_field_setter,  0 },
    { "Content-Version",     OFFSET_REQ + OFFSET_FLD(content) + OFFSET_CONT_FLD(ver),      httpi_str_field_setter,  0 },
    { "Expires",             OFFSET_REQ + OFFSET_FLD(content) + OFFSET_CONT_FLD(expires),  httpi_str_field_setter,  0 },
    { "Last-Modified",       OFFSET_REQ + OFFSET_FLD(content) + OFFSET_CONT_FLD(modified), httpi_str_field_setter,  0 },
    { "Link",                OFFSET_REQ + OFFSET_FLD(content) + OFFSET_CONT_FLD(link),     httpi_str_field_setter,  0 },
    { "Title",               OFFSET_REQ + OFFSET_FLD(content) + OFFSET_CONT_FLD(title),    httpi_str_field_setter,  0 },
    { NULL }
};

struct field_info mpart_fields[] =
{
    { "Content-Type",        OFFSET_POST + OFFSET_POST_FLD(content_type),                  httpi_str_field_setter, 1 },
    { "Content-Disposition", OFFSET_POST,                                                  httpi_post_cdh_setter,  1 },
    { NULL }
};

int httpi_readlex(httpi_t *httpi, int *stack, const char *stop, int skipstop, int cntlim)
{
    while (true)
    {
        char c;
        if (httpi->in.pos == httpi->in.size) return CODE_IN_EOF;
        c = httpi->in.data[httpi->in.pos];
        if (httpi->out.pos >= httpi->out.size - 1) return CODE_OUT_EOF;
        if (c == stop[0] || c == stop[1] || c == stop[2] || c == stop[3])
        {
            if (skipstop) httpi->in.pos++;
            httpi->out.data[httpi->out.pos] = 0;
            stack[1] = c;
            return CODE_DONE;
        }
        httpi->out.data[httpi->out.pos] = c;
        httpi->out.pos++;
        httpi->in.pos++;
        httpi->in.tot++;
        *stack += 1;
        if (*stack == cntlim) return CODE_CNT_LIM;
    }
}

int httpi_skip_char(httpi_t *httpi, int char1, int char2)
{
    char c;
    if (httpi->in.pos == httpi->in.size) return CODE_IN_EOF;
    c = httpi->in.data[httpi->in.pos];
    if (c != char1 && c != char2) return CODE_ERROR;
    httpi->in.pos++;
    httpi->in.tot++;
    return CODE_DONE;
}

static inline void httpi_inc_used(httpi_t *httpi, int size)
{
    httpi->pool.used += size;
    httpi->out.data = httpi->pool.data + httpi->pool.used;
    httpi->out.size = httpi->pool.size - httpi->pool.used;
    httpi->out.pos = 0;
}

static inline void httpi_set_used(httpi_t *httpi, int used)
{
    httpi->pool.used = used;
    httpi->out.data = httpi->pool.data + httpi->pool.used;
    httpi->out.size = httpi->pool.size - httpi->pool.used;
    httpi->out.pos = 0;
}

#define FIELD ((char *)httpi + httpi->rqhdr.field->offset)

static void httpi_str_field_setter(httpi_t *httpi, int arg)
{
    *(const char **)FIELD = httpi->out.data;
    httpi_inc_used(httpi, http_unescape(httpi->out.data, httpi->out.pos, 1) + 1);
}

static void httpi_int_field_setter(httpi_t *httpi, int arg)
{
    *(int *)FIELD = strtol(httpi->out.data, NULL, 10);
    httpi->out.pos = 0;
}

static void httpi_ct_field_setter(httpi_t *httpi, int arg)
{
    *(http_ct_t *)FIELD = str_to_ct(httpi->out.data);
    httpi->out.pos = 0;
}

static void httpi_acc_field_setter(httpi_t *httpi, int arg)
{
    httpaccept_t *acc = (httpaccept_t *)FIELD;
    /* text/html,application/xhtml+xml,application/xml;q=0.9,*;q=0.8 */
    if (acc->count >= HTTP_REQ_MAX_ACCEPT) return;
    switch (httpi->setter.state)
    {
    case 0: /* wait type name */
        acc->type[acc->count] = httpi->out.data;
        acc->q[acc->count] = 1;
        acc->count++;
        httpi_inc_used(httpi, http_unescape(httpi->out.data, httpi->out.pos, 1) + 1);
        if (arg == ';') httpi->setter.state = 1;
        break;
    case 1: /* wait param name */
        httpi->out.pos = 0;
        if (arg != '=')
        {
            httpi->setter.state = 0;
            break;
        }
        if (strcmp(httpi->out.data, "level") == 0)
            httpi->setter.state = 2;
        else
        if (strcmp(httpi->out.data, "q") == 0)
            httpi->setter.state = 3;
        break;
    case 2: /* wait level value */
        acc->level[acc->count - 1] = strtol(httpi->out.data, NULL, 10);
        if (arg == ',')
            httpi->setter.state = 1; else
            httpi->setter.state = 0;
        httpi->out.pos = 0;
        break;
    case 3: /* wait q value */
        acc->q[acc->count - 1] = strtof(httpi->out.data, NULL);
        if (arg == ';')
            httpi->setter.state = 1; else
            httpi->setter.state = 0;
        httpi->out.pos = 0;
        break;
    }
}

static void httpi_cntt_field_setter(httpi_t *httpi, int arg)
{
    http_cont_t *cont = (http_cont_t *)FIELD;
    /* multipart/form-data; boundary=Asrf456BGe4h */
    switch (httpi->setter.state)
    {
    case 0: /* wait type name */
        cont->type = httpi->out.data;
        httpi_inc_used(httpi, http_unescape(httpi->out.data, httpi->out.pos, 1) + 1);
        if (arg == ';')
            httpi->setter.state = 1; else
            httpi->setter.state = 10;
        break;
    case 1: /* wait param name */
        httpi->out.pos = 0;
        if (arg != '=')
        {
            httpi->setter.state = 10;
            break;
        }
        if (strcmp(httpi->out.data, "boundary") == 0)
        {
            httpi->out.data[0] = '\r';
            httpi->out.data[1] = '\n';
            httpi->out.data[2] = '-';
            httpi->out.data[3] = '-';
            httpi->mpart.bndry = httpi->out.data + 2;
            httpi->mpart.bndry_size = 2;
            httpi_inc_used(httpi, 4);
            httpi->setter.state = 2;
        }
        else
            httpi->setter.state = 10;
        break;
    case 2: /* boundary */
        httpi->mpart.bndry_size += httpi->out.pos;
        cont->boundary = httpi->out.data;
        httpi_inc_used(httpi, httpi->out.pos + 1);
        httpi->setter.state = 10;
        break;
    case 10:
        break;
    }
}

static void httpi_post_cdh_setter(httpi_t *httpi, int arg)
{
    http_post_t *post = (http_post_t *)FIELD;
    /* form-data; name="AttachedFile1"; filename="photo-1.jpg" */
    switch (httpi->setter.state)
    {
    case 0: /* wait type name */
        post->disp_type = httpi->out.data;
        httpi_inc_used(httpi, http_unescape(httpi->out.data, httpi->out.pos, 1) + 1);
        if (arg == ';')
            httpi->setter.state = 1; else
            httpi->setter.state = 10;
        break;
    case 1: /* wait param name */
        httpi->out.pos = 0;
        httpi->setter.state = 10;
        if (arg != '=') break;
        if (strcmp(httpi->out.data, "name") == 0)
            httpi->setter.state = 2; else
        if (strcmp(httpi->out.data, "filename") == 0)
            httpi->setter.state = 3;
        break;
    case 2: /* wait name */
        if (arg == ',')
            httpi->setter.state = 0; else
            httpi->setter.state = 1;
        post->name = httpi->out.data;
        httpi_inc_used(httpi, http_unescape(httpi->out.data, httpi->out.pos, 1) + 1);
        break;
    case 3: /* wait filename */
        if (arg == ',')
            httpi->setter.state = 0; else
            httpi->setter.state = 1;
        post->file = httpi->out.data;
        httpi_inc_used(httpi, http_unescape(httpi->out.data, httpi->out.pos, 1) + 1);
        break;
    case 10:
        break;
    }
}

#undef FIELD

int httpi_lex_reader(httpi_t *httpi, const char *stop, int skipstop, int cntlim)
{
    while (true)
    {
        char c;
        if (httpi->out.pos >= httpi->out.size - 1) return CODE_OUT_EOF;
        if (httpi->in.tot == cntlim && cntlim >= 0)
        {
            httpi->out.data[httpi->out.pos] = 0;
            return CODE_CNT_LIM;
        }
        if (httpi->in.pos == httpi->in.size) return CODE_IN_EOF;
        c = httpi->in.data[httpi->in.pos];
        if (c == stop[0] || c == stop[1] || c == stop[2] || c == stop[3])
        {
            if (skipstop)
            {
                httpi->in.pos++;
                httpi->in.tot++;
            }
            httpi->out.data[httpi->out.pos] = 0;
            httpi->lexrd.stop = c;
            return CODE_DONE;
        }
        httpi->out.data[httpi->out.pos] = c;
        httpi->out.pos++;
        httpi->in.pos++;
        httpi->in.tot++;
    }
}

int httpi_rqline(httpi_t *httpi)
{
    int code;
start:
    switch (httpi->rqline.state)
    {
    case STATE_RQLINE_METHOD:
        code = httpi_lex_reader(httpi, "\r \0\0", 1, -1);
        if (code != CODE_DONE) return code;
        if (httpi->lexrd.stop == '\r') return CODE_ERROR;
        httpi->req.method = str_to_method(httpi->out.data);
        httpi->out.pos = 0;
        httpi->rqline.state = STATE_RQLINE_URI;
        goto start;
    case STATE_RQLINE_URI:
        code = httpi_lex_reader(httpi, "\r ?\0", 1, -1);
        if (code != CODE_DONE) return code;
        if (httpi->lexrd.stop == '\r') return CODE_ERROR;
        if (httpi->lexrd.stop == '?')
            httpi->rqline.state = STATE_RQLINE_KEY; else
            httpi->rqline.state = STATE_RQLINE_VER;
        httpi->req.uri = httpi->out.data;
        httpi_inc_used(httpi, http_unescape(httpi->out.data, httpi->out.pos, 1) + 1);
        goto start;
    case STATE_RQLINE_KEY:
    case STATE_RQLINE_VAL:
        code = httpi_lex_reader(httpi, "\r &=", 1, -1);
        if (code != CODE_DONE) return code;
        if (httpi->req.nparams >= HTTP_REQ_MAX_PARAMS) return CODE_MEM_ERROR;
        if (httpi->rqline.state == STATE_RQLINE_KEY)
        {
            httpi->req.params[httpi->req.nparams] = httpi->out.data;
            httpi->req.values[httpi->req.nparams] = NULL;
            httpi->req.nparams++;
        }
        else
        {
            httpi->req.values[httpi->req.nparams - 1] = httpi->out.data;
        }
        httpi_inc_used(httpi, http_unescape(httpi->out.data, httpi->out.pos, 1) + 1);
        switch (httpi->lexrd.stop)
        {
        case ' ':  httpi->rqline.state = STATE_RQLINE_VER; break;
        case '&':  httpi->rqline.state = STATE_RQLINE_KEY; break;
        case '=':  httpi->rqline.state = STATE_RQLINE_VAL; break;
        case '\r': return CODE_ERROR;
        }
        goto start;
    case STATE_RQLINE_VER:
        code = httpi_lex_reader(httpi, "\r\0\0\0", 1, -1);
        if (code != CODE_DONE) return code;
        httpi->req.ver = httpi->out.data;
        httpi_inc_used(httpi, http_unescape(httpi->out.data, httpi->out.pos, 1) + 1);
        httpi->rqline.state = STATE_RQLINE_SKIPN;
        goto start;
    case STATE_RQLINE_SKIPN:
        code = httpi_skip_char(httpi, '\n', '\0');
        return code;
    default:
        return CODE_ERROR;
    }
}

const fld_t *find_field(const fld_t *fields, const char *name)
{
    int i;
    char c = name[0];
    for (i = 0; fields[i].name != NULL; i++)
        if (c == fields[i].name[0])
            if (strcmp(fields[i].name, name) == 0)
                return fields + i;
    return NULL;
}

int httpi_hdr_reader(httpi_t *httpi, int composite_val)
{
    int code;
start:
    switch (httpi->hdrrd.state)
    {
    case STATE_HDRRD_INIT:
        httpi->hdrrd.state = STATE_HDRRD_KEYRD;
        httpi->out.pos = 0;
        goto start;
    case STATE_HDRRD_KEYRD:
        code = httpi_lex_reader(httpi, "\r:\0\0", 1, -1);
        if (code != CODE_DONE) return code;
        if (httpi->lexrd.stop == '\r')
        {
            if (httpi->out.pos != 0) return CODE_ERROR;
            httpi->hdrrd.state = STATE_HDRRD_LASTN;
            goto start;
        }
        httpi->hdrrd.state = STATE_HDRRD_KEY;
        return CODE_DONE;
    case STATE_HDRRD_KEY:
        httpi->hdrrd.state = composite_val ? STATE_HDRRD_CSKIPWS : STATE_HDRRD_SKIPWS;
        goto start;
    case STATE_HDRRD_FINISHED:
        return CODE_DONE;
    case STATE_HDRRD_SKIPWS:
    case STATE_HDRRD_CSKIPWS:
        if (httpi->in.pos >= httpi->in.size) return CODE_IN_EOF;
        if (httpi->in.data[httpi->in.pos] == ' ')
        {
            httpi->in.pos++;
            httpi->in.tot++;
            goto start;
        }
        httpi->hdrrd.state++;
        goto start;
    case STATE_HDRRD_VALRD:
        code = httpi_lex_reader(httpi, "\r\0\0\0", 1, -1);
        if (code != CODE_DONE) return code;
        httpi->hdrrd.state = STATE_HDRRD_VAL;
        return CODE_DONE;
    case STATE_HDRRD_CVALRD:
        code = httpi_lex_reader(httpi, "\r;,=", 1, -1);
        if (code != CODE_DONE) return code;
        httpi->hdrrd.state = STATE_HDRRD_CVAL;
        return CODE_DONE;
    case STATE_HDRRD_VAL:
        httpi->hdrrd.state = STATE_HDRRD_SKIPN;
        goto start;
    case STATE_HDRRD_CVAL:
        httpi->hdrrd.state = httpi->lexrd.stop == '\r' ?
            STATE_HDRRD_SKIPN : STATE_HDRRD_CSKIPWS;
        goto start;
    case STATE_HDRRD_SKIPN:
    case STATE_HDRRD_LASTN:
        if (httpi->in.pos >= httpi->in.size) return CODE_IN_EOF;
        if (httpi->in.data[httpi->in.pos] != '\n') return CODE_ERROR;
        httpi->in.pos++;
        httpi->in.tot++;
        httpi->hdrrd.state = httpi->hdrrd.state == STATE_HDRRD_LASTN ?
            STATE_HDRRD_FINISHED : STATE_HDRRD_KEYRD;
        goto start;
    default:
        return CODE_ERROR;
    }
}

int httpi_request_hdr_reader(httpi_t *httpi, const fld_t *fields)
{
    int code;
start:
    switch (httpi->rqhdr.state)
    {
    case 0: /* initial */
        httpi->rqhdr.field = NULL;
        httpi->rqhdr.state = 1;
        httpi->hdrrd.state = 0;
    case 1: /* wait key */
        code = httpi_hdr_reader(httpi, 0);
        if (code != CODE_DONE) return code;
        if (httpi->hdrrd.state == STATE_HDRRD_FINISHED) return CODE_DONE;
        httpi->rqhdr.field = find_field(fields, httpi->out.data);
        httpi->out.pos = 0;
        httpi->setter.state = 0;
        httpi->rqhdr.state = 2;
    case 2: /* read value or composite value */
        code = 0;
        if (httpi->rqhdr.field != NULL)
            code = httpi->rqhdr.field->composite;
        code = httpi_hdr_reader(httpi, code);
        if (code != CODE_DONE) return code;
        if (httpi->rqhdr.field != NULL)
            httpi->rqhdr.field->setter(httpi, httpi->lexrd.stop); else
            httpi->out.pos = 0;
        httpi->rqhdr.state = httpi->lexrd.stop == '\r' ? 1 : 2;
        goto start;
    }
    return CODE_ERROR;
}

#define MPART_STATE httpi->mpart.state
#define MPART_CNTR  httpi->mpart.bndry_cntr
#define MPART_POS   httpi->mpart.bndry_pos

int httpi_mpart(httpi_t *httpi)
{
    int avail;
    avail = httpi->out.size - httpi->out.pos;
    if (avail <= 1) return CODE_OUT_EOF;

start:
    switch (MPART_STATE)
    {
    case STATE_MPART_DATARD:
        while (true)
        {
            char c;
            if (httpi->in.pos >= httpi->in.size) return CODE_IN_EOF;
            c = httpi->in.data[httpi->in.pos];
            if (httpi->mpart.bndry[MPART_CNTR] == c)
            {
                httpi->in.pos++;
                httpi->in.tot++;
                MPART_CNTR++;
                if (MPART_CNTR != httpi->mpart.bndry_size) continue;
                MPART_CNTR = 0;
                MPART_STATE = STATE_MPART_ENDCHECK;
                goto start;
            }
            /* not a boundary */
            if (MPART_CNTR > 0)
            {
                MPART_POS = 0;
                MPART_STATE = STATE_MPART_NOTBNDRY;
                goto start;
            }
            if (httpi->out.pos >= httpi->out.size - 1)
            {
                httpi->out.data[httpi->out.pos] = 0;
                return CODE_OUT_EOF;
            }
            httpi->out.data[httpi->out.pos] = c;
            httpi->out.pos++;
            httpi->in.pos++;
            httpi->in.tot++;
        }
    case STATE_MPART_NOTBNDRY:
        while (true)
        {
            if (httpi->out.pos >= httpi->out.size - 1)
            {
                httpi->out.data[httpi->out.pos] = 0;
                return CODE_OUT_EOF;
            }
            httpi->out.data[httpi->out.pos] = httpi->mpart.bndry[MPART_POS];
            MPART_POS++;
            httpi->out.pos++;
            MPART_CNTR--;
            if (MPART_CNTR != 0) continue;
            MPART_STATE = STATE_MPART_DATARD;
            goto start;
        }
    case STATE_MPART_ENDCHECK: /* skip '-' or '\r' */
        if (httpi->in.pos >= httpi->in.size) return CODE_IN_EOF;
        switch (httpi->in.data[httpi->in.pos])
        {
        case '\r': MPART_STATE = STATE_MPART_SKIPN; break;
        case '-':  MPART_STATE = STATE_MPART_SKIPMINUS; break;
        default: return CODE_ERROR;
        }
        httpi->in.pos++;
        httpi->in.tot++;
        goto start;
    case STATE_MPART_SKIPMINUS: /* skip '-' */
        if (httpi->in.pos >= httpi->in.size) return CODE_IN_EOF;
        if (httpi->in.data[httpi->in.pos] != '-') return CODE_ERROR;
        MPART_STATE = STATE_MPART_SKIPR;
        httpi->in.pos++;
        httpi->in.tot++;
        goto start;
    case STATE_MPART_SKIPR: /* skip 'r' */
        if (httpi->in.pos >= httpi->in.size) return CODE_IN_EOF;
        if (httpi->in.data[httpi->in.pos] != '\r') return CODE_ERROR;
        MPART_STATE = STATE_MPART_LASTN;
        httpi->in.pos++;
        httpi->in.tot++;
        goto start;
    case STATE_MPART_LASTN:
    case STATE_MPART_SKIPN: /* skip 'n' */
        if (httpi->in.pos >= httpi->in.size) return CODE_IN_EOF;
        if (httpi->in.data[httpi->in.pos] != '\n') return CODE_ERROR;
        httpi->in.pos++;
        httpi->in.tot++;
        MPART_STATE = MPART_STATE == STATE_MPART_LASTN ?
            STATE_MPART_FINISHED : STATE_MPART_BNDRY;
        return CODE_DONE;
    case STATE_MPART_BNDRY:
        MPART_STATE = STATE_MPART_DATARD;
        goto start;
    default:
        return CODE_ERROR;
    }
}

/*
returns:
    CODE_DONE
        in states:
        STATE_URLENC_KEY - key readed
        STATE_URLENC_CHUNK - value chunk readed
        STATE_URLENC_END - value readed
        STATE_URLENC_FINISHED - all done
    CODE_IN_EOF
        in any state
        not an error
    CODE_MEM_ERROR
        no memory in pool
*/

int httpi_urlenc(httpi_t *httpi)
{
    char c;
    int code;
start:
    switch (httpi->urlenc.state)
    {
    case STATE_URLENC_INIT:
        httpi->in.tot = 0;
        httpi->out.pos = 0;
        httpi->urlenc.state = STATE_URLENC_KEYRD;
        httpi->used = httpi->pool.used;
    case STATE_URLENC_KEYRD:
        code = httpi_lex_reader(httpi, "=&\0\0", 0, httpi->req.content.length);
        switch (code)
        {
        case CODE_OUT_EOF:
            return CODE_MEM_ERROR;
        case CODE_CNT_LIM:
            if (httpi->out.pos <= 0)
            {
                httpi->urlenc.state = STATE_URLENC_END;
                return CODE_DONE;
            }
        case CODE_DONE:
            httpi->out.pos = http_unescape(httpi->out.data, httpi->out.pos, 1);
            httpi->urlenc.state = STATE_URLENC_KEY;
            return CODE_DONE;
        default:
            return code;
        }
    case STATE_URLENC_KEY:
        if (httpi->in.tot >= httpi->req.content.length) /* CODE_CNT_LIM! */
            goto return_empty_val;
        c = httpi->in.data[httpi->in.pos];
        httpi->in.pos++;
        httpi->in.tot++;
        if (c == '&') goto return_empty_val;
        /* read value now! */
        httpi->urlenc.state = STATE_URLENC_VALRD;
        httpi->out.pos = 0;
    case STATE_URLENC_VALRD:
        code = httpi_lex_reader(httpi, "&\0\0\0", 0, httpi->req.content.length);
        switch (code)
        {
        case CODE_OUT_EOF:
            if (httpi->out.pos < 2) return CODE_MEM_ERROR;
            *(uint32_t *)httpi->urlenc.extra = 0;
            if (httpi->out.data[httpi->out.pos - 1] == '%')
            {
                httpi->urlenc.extra[0] = '%';
                httpi->out.pos--;
            }
            else
            {
                if (httpi->out.data[httpi->out.pos - 2] == '%')
                {
                    httpi->urlenc.extra[0] = '%';
                    httpi->urlenc.extra[1] = httpi->out.data[httpi->out.pos - 1];
                    httpi->out.pos -= 2;
                }
            }
            httpi->out.pos = http_unescape(httpi->out.data, httpi->out.pos, 1);
            httpi->urlenc.state = STATE_URLENC_CHUNK;
            return CODE_DONE;
        case CODE_DONE: /* & */
            httpi->in.pos++;
            httpi->in.tot++;
        case CODE_CNT_LIM:
            httpi->out.pos = http_unescape(httpi->out.data, httpi->out.pos, 1);
            httpi->urlenc.state = STATE_URLENC_END;
            return CODE_DONE;
        default:
            return code;
        }
    case STATE_URLENC_CHUNK:
        for (httpi->out.pos = 0; httpi->urlenc.extra[httpi->out.pos] != 0; httpi->out.pos++)
            httpi->out.data[httpi->out.pos] = httpi->urlenc.extra[httpi->out.pos];
        httpi->urlenc.state = STATE_URLENC_VALRD;
        goto start;
    case STATE_URLENC_END:
        httpi_set_used(httpi, httpi->used);
        if (httpi->in.tot >= httpi->req.content.length) /* CODE_CNT_LIM! */
            httpi->urlenc.state = STATE_URLENC_FINISHED; else
            httpi->urlenc.state = STATE_URLENC_KEYRD;
        goto start;
    case STATE_URLENC_FINISHED:
        return CODE_DONE;
    }
    return CODE_ERROR; /* paranoya */

return_empty_val:
    if (httpi->in.pos >= httpi->in.size) return CODE_MEM_ERROR;
    httpi->out.data[httpi->out.pos] = 0;
    httpi->out.pos = 0;
    httpi->urlenc.state = STATE_URLENC_END;
    return CODE_DONE;
}

static void httpi_reset(httpi_t *httpi)
{
    memset(&httpi->req, 0, sizeof(http_req_t));
    memset(&httpi->post, 0, sizeof(http_post_t));
    httpi->pool.used = 0;
    httpi->in.tot = 0;
    httpi->out.data = httpi->pool.data;
    httpi->out.size = httpi->pool.size;
    httpi->out.pos = 0;
    httpi->state = 0;
    httpi->rqline.state = 0;
}

httpi_t *httpi_init(const void *pool, int size)
{
    httpi_t *httpi;
    if (size - (int)sizeof(httpi_t) < 64) return NULL;
    httpi = (httpi_t *)(((size_t)pool + size - sizeof(httpi_t)) & ~(MEM_ALIGNMENT - 1));
    memset(httpi, 0, sizeof(httpi_t));
    httpi->pool.data = (char *)pool;
    httpi->pool.size = (char *)httpi - (char *)pool;
    httpi->pool.used = 0;
    httpi->in.data = NULL;
    httpi->in.pos = 0;
    httpi->in.tot = 0;
    httpi->in.size = 0;
    httpi_reset(httpi);
    return httpi;
}

void httpi_push(httpi_t *httpi, const void *data, int size)
{
    httpi->in.data = (char *)data;
    httpi->in.size = size;
    httpi->in.pos = 0;
}

void httpi_get_state(httpi_t *httpi, char **pool, int *avail)
{
    *pool = httpi->pool.data + httpi->pool.used + httpi->out.pos + 1;
    *avail = httpi->pool.size - httpi->pool.used - httpi->out.pos - 1;
    if (*avail < 0) *avail = 0;
}

const http_req_t *httpi_request(httpi_t *httpi)
{
    return httpi->state > STATE0_RQHDR ? &httpi->req : NULL;
}

const http_post_t *httpi_posted(httpi_t *httpi)
{
    return httpi->post.name == NULL ? NULL : &httpi->post;
}

#define ERROR(code) \
    { \
        httpi->state = STATE0_ERROR; \
        return code; \
    }

int httpi_pull(httpi_t *httpi)
{
    httpi->post.first = 0;
start:
    switch (httpi->state)
    {
    case STATE0_RQLINE:
        switch (httpi_rqline(httpi))
        {
        case CODE_IN_EOF:  return HTTP_IN_NODATA;
        case CODE_OUT_EOF: ERROR(HTTP_IN_ERR_NOMEM);
        case CODE_ERROR:   ERROR(HTTP_IN_ERR_SYNT);
        case CODE_DONE:
            httpi->state = STATE0_RQHDR;
            httpi->rqhdr.state = 0;
            goto start;
        }
    case STATE0_RQHDR:
        switch (httpi_request_hdr_reader(httpi, hdr_fields))
        {
        case CODE_IN_EOF:  return HTTP_IN_NODATA;
        case CODE_OUT_EOF: ERROR(HTTP_IN_ERR_NOMEM);
        case CODE_ERROR:   ERROR(HTTP_IN_ERR_SYNT);
        case CODE_DONE:
            httpi->state = STATE0_DONE;
            if (httpi->req.content.type != NULL)
            {
                if (strcmp(httpi->req.content.type, MIME_URLENCODED) == 0)
                {
                    httpi->state = STATE0_URLENC;
                    httpi->urlenc.state = 0;
                    memset(&httpi->post, 0, sizeof(http_post_t));
                }
                else
                if (strcmp(httpi->req.content.type, MIME_MPART_FORM) == 0)
                {
                    if (httpi->req.content.boundary == NULL) ERROR(HTTP_IN_ERR_SYNT);
                    if (*httpi->req.content.boundary == 0) ERROR(HTTP_IN_ERR_SYNT);
                    memset(&httpi->post, 0, sizeof(http_post_t));
                    httpi->state = STATE0_MPART_DATA;
                    httpi->mpart.state = 0;
                    httpi->mpart.bndry_cntr = 0;
                    httpi->mpart.bndry_pos = 0;
                    httpi->used = httpi->pool.used;
                }
            }
            return HTTP_IN_REQUEST;
        }
    case STATE0_URLENC:
        switch (httpi_urlenc(httpi))
        {
        case CODE_IN_EOF:  return HTTP_IN_NODATA;
        case CODE_ERROR:   ERROR(HTTP_IN_ERR_SYNT);
        case CODE_DONE:
            switch (httpi->urlenc.state)
            {
            case STATE_URLENC_KEY:
                memset(&httpi->post, 0, sizeof(http_post_t));
                httpi->post.name = httpi->out.data;
                httpi->post.first = 1;
                httpi_inc_used(httpi, httpi->out.pos + 1);
                goto start;
            case STATE_URLENC_END:
                httpi->post.last = 1;
            case STATE_URLENC_CHUNK:
                httpi->post.data = httpi->out.data;
                httpi->post.size = httpi->out.pos;
                return HTTP_IN_POSTDATA;
            case STATE_URLENC_FINISHED:
                 httpi->state = STATE0_DONE;
                 goto start;
            }
        default: /* not possible */
            ERROR(HTTP_IN_ERR_SYNT);
        }

    case STATE0_MPART_DATA:
        switch (httpi_mpart(httpi))
        {
        case CODE_IN_EOF: return HTTP_IN_NODATA;
        case CODE_ERROR:  ERROR(HTTP_IN_ERR_SYNT);
        case CODE_OUT_EOF:
            if (httpi->out.pos <= 0)
                ERROR(HTTP_IN_ERR_NOMEM);
            if (httpi->post.name == NULL)
                ERROR(HTTP_IN_ERR_SYNT);
            httpi->post.data = httpi->out.data;
            httpi->post.size = httpi->out.pos;
            httpi->post.last = 0;
            httpi->out.pos = 0;
            return HTTP_IN_POSTDATA;
        case CODE_DONE:
            switch (httpi->mpart.state)
            {
            case STATE_MPART_FINISHED: httpi->state = STATE0_DONE;       break;
            case STATE_MPART_BNDRY:    httpi->state = STATE0_MPART_INIT; break;
            }
            if (*httpi->mpart.bndry == '-')
            {
                httpi->mpart.bndry_size += 2;
                httpi->mpart.bndry -= 2;
            }
            if (httpi->post.name == NULL && httpi->out.pos > 0)
                ERROR(HTTP_IN_ERR_SYNT);
            if (httpi->post.name == NULL) goto start;
            httpi->post.data = httpi->out.data;
            httpi->post.size = httpi->out.pos;
            httpi->post.last = 1;
            httpi->out.pos = 0;
            return HTTP_IN_POSTDATA;
        }

    case STATE0_MPART_INIT:
        memset(&httpi->post, 0, sizeof(http_post_t));
        httpi->post.first = 1;
        httpi->state = STATE0_MPART_HDR;
        httpi->rqhdr.state = 0;
        httpi->pool.used = httpi->used;
        httpi->out.data = httpi->pool.data;
        httpi->out.size = httpi->pool.size - httpi->pool.used;
        httpi->out.pos = 0;

    case STATE0_MPART_HDR:
        switch (httpi_request_hdr_reader(httpi, mpart_fields))
        {
        case CODE_IN_EOF:  return HTTP_IN_NODATA;
        case CODE_OUT_EOF: ERROR(HTTP_IN_ERR_NOMEM);
        case CODE_ERROR:   ERROR(HTTP_IN_ERR_SYNT);
        case CODE_DONE:
            httpi->state = STATE0_MPART_DATA;
            httpi->mpart.state = STATE_MPART_DATARD;
            goto start;
        }

    case STATE0_DONE:
        httpi->state = STATE0_RESET;
        return HTTP_IN_FINISHED;

    case STATE0_RESET:
        httpi_reset(httpi);
        goto start;

    default:
        ERROR(HTTP_IN_ERR_SYNT);
    }

}
#undef ERROR

#define MIN_OUTPUT_SIZE   256
/* bytes: 8*hex + 2*crlf + (DATA) + 2*crlf; space to ending: 1*'0' + 2*crlf + 2*crlf */
#define CHUNKED_RESERVE   17
#define OUT_FLAG_CHUNKED  1
#define OUT_FLAG_FINISHED 2

struct http_output
{
    char *buff;
    int buffsz;
    int outpos;
    int outsz;
    int flags;
};

#define ALIGNED(addr) (((size_t)addr + MEM_ALIGNMENT - 1) & ~(MEM_ALIGNMENT - 1))

static int write_str(httpo_t *out, const char *str)
{
    while (*str != 0)
    {
        if (out->outsz == out->buffsz) return 0;
        out->buff[(out->outpos + out->outsz) % out->buffsz] = *str;
        out->outsz++;
        str++;
    }
    return 1;
}

static inline int write_uint(httpo_t *out, uint32_t value, int radix)
{
    char s[16];
    sprintf(s, radix == 16 ? "%X" : "%u", value);
    return write_str(out, s);
}

static const char *CONN_TYPE_STR[] =
{
    "", "close", "keep-alive"
};

httpo_t *httpo_init(void *pool, int poolsz)
{
    httpo_t *res;
    char *apool = (char *)ALIGNED(pool);
    poolsz -= (apool - (char *)pool);
    poolsz -= sizeof(struct http_output);
    if (poolsz < MIN_OUTPUT_SIZE) return NULL;
    res = (httpo_t *)apool;
    res->buff = apool + sizeof(struct http_output);
    res->buffsz = poolsz;
    res->outpos = 0;
    res->outsz = 0;
    res->flags = 0;
    return res;
}

void httpo_state(httpo_t *out, char **data, int *size)
{
    *data = out->buff + out->outpos;
    *size = out->buffsz - out->outpos;
    if (*size > out->outsz) *size = out->outsz;
}

void httpo_sended(httpo_t *out, int count)
{
    out->outpos = (out->outpos + count) % out->buffsz;
    out->outsz -= count;
}

int httpo_write_resp(httpo_t *out, const http_resp_t *resp)
{
    int done;
    int outsz;

#define PRNS(s)          done &= write_str(out, s)
#define PRNU(v, r)       done &= write_uint(out, v, r)
#define PRNH(s, var)     { PRNS(s); PRNS(": "); PRNS(var); PRNS("\r\n"); }
#define PRNHU(s, var, r) { PRNS(s); PRNS(": "); PRNU(var, r); PRNS("\r\n"); }
#define PRNHS(s, var)    { if (var != NULL) PRNH(s, var); }

    done = 1;
    outsz = out->outsz;
    PRNS("HTTP/1.1 "); PRNU(resp->code, 10); PRNS(" "); PRNS(http_code_str(resp->code)); PRNS("\r\n");
    PRNHS("Content-Encoding", resp->content.encoding);
    PRNHS("Content-Language", resp->content.language);
    if (resp->content.length >= 0)
        PRNHU("Content-Length", resp->content.length, 10);
    PRNHS("Content-Location", resp->content.location);
    PRNHS("Content-MD5", resp->content.md5);
    /* TODO: http_rng_t  range; */
    PRNHS("Content-Type", resp->content.type);
    PRNHS("Content-Version", resp->content.ver);
    PRNHS("Expires", resp->content.expires);
    PRNHS("Last-Modified", resp->content.modified);
    PRNHS("Link", resp->content.link);
    PRNHS("Title", resp->content.title);
    PRNHS("Connection", CONN_TYPE_STR[resp->connection]);
    PRNHS("Cache-Control", resp->cache_control);
    PRNHS("ETag", resp->etag);
    PRNHS("Location", resp->location);
    PRNHS("Proxy-Authenticate", resp->proxy_authenticate);
    PRNHS("Public", resp->Public);
    if (resp->retry_after > 0)
        PRNHU("Retry-After", resp->retry_after, 10);
    PRNHS("Server", resp->server);
    if (resp->transfer_encoding == TENC_CHUNKED)
        PRNHS("Transfer-Encoding", "chunked");
    PRNHS("Vary", resp->vary);
    PRNHS("WWW-Authenticate", resp->www_authenticate);
    if (resp->exthdr != NULL) PRNS(resp->exthdr);
    PRNS("\r\n");
    if (!done)
    {
        out->outsz = outsz;
        return 0;
    }
    if (resp->transfer_encoding == TENC_CHUNKED)
        out->flags |= OUT_FLAG_CHUNKED;
    return 1;
}

int httpo_write_avail(httpo_t *out)
{
    int size = out->buffsz - out->outsz;
    if (out->flags & OUT_FLAG_CHUNKED)
        size -= CHUNKED_RESERVE;
    return size > 0 ? size : 0;
}

const char hex_digits[] = "0123456789ABCDEF";

static inline void out_write(httpo_t *out, const void *mem, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        out->buff[(out->outpos + out->outsz) % out->buffsz] = ((const char *)mem)[i];
        out->outsz++;
    }
}

void write_chunk_size(httpo_t *out, uint32_t val)
{
    int i;
    char buff[16];
    int len = 0;
    for (i = 0; i < 8; i++)
    {
        int v = val >> 28;
        if (len > 0 || v != 0)
            buff[len++] = hex_digits[v];
        val = val << 4;
    }
    if (len == 0)
        buff[len++] = '0';
    buff[len++] = '\r';
    buff[len++] = '\n';
    out_write(out, buff, len);
}

int httpo_write_data(httpo_t *out, const void *data, int size)
{
    int avail;
    if (size <= 0) return 0;
    avail = out->buffsz - out->outsz;
    if (out->flags & OUT_FLAG_CHUNKED)
    {
        avail -= CHUNKED_RESERVE;
        if (avail <= 0) return 0;
        if (size > avail) size = avail;
        write_chunk_size(out, size);
        out_write(out, data, size);
        out_write(out, "\r\n", 2);
        return size;
    }
    if (size > avail) size = avail;
    out_write(out, data, size);
    return size;
}

void httpo_finished(httpo_t *out)
{
    out->flags |= OUT_FLAG_FINISHED;
    if (out->flags & OUT_FLAG_CHUNKED)
        out_write(out, "0\r\n\r\n", 5);
}

#ifdef __cplusplus
}
#endif
