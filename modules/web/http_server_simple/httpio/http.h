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

#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HTTP_REQ_MAX_PARAMS
#define HTTP_REQ_MAX_PARAMS 16
#endif

#ifndef HTTP_REQ_MAX_ACCEPT
#define HTTP_REQ_MAX_ACCEPT 8
#endif

extern const char MIME_APP_ATOM[];    /* application/atom+xml */
extern const char MIME_APP_JSON[];    /* application/json */
extern const char MIME_APP_JS[];      /* application/javascript */
extern const char MIME_APP_OCTSTR[];  /* application/octet-stream */
extern const char MIME_APP_PDF[];     /* application/pdf */
extern const char MIME_APP_PS[];      /* application/postscript */
extern const char MIME_APP_XHTML[];   /* application/xhtml+xml */
extern const char MIME_APP_XML[];     /* application/xml-dtd */
extern const char MIME_APP_ZIP[];     /* application/zip */
extern const char MIME_APP_GZIP[];    /* application/x-gzip */
extern const char MIME_APP_BTOR[];    /* application/x-bittorrent */
extern const char MIME_APP_TEX[];     /* application/x-tex */
extern const char MIME_URLENCODED[];  /* application/x-www-form-urlencoded */
extern const char MIME_TEXT_HTML[];   /* text/html */
extern const char MIME_TEXT_JS[];     /* text/javascript */
extern const char MIME_TEXT_PLAIN[];  /* text/plain */
extern const char MIME_TEXT_XML[];    /* text/xml */
extern const char MIME_TEXT_CSS[];    /* text/css */
extern const char MIME_IMAGE_GIF[];   /* image/gif */
extern const char MIME_IMAGE_JPEG[];  /* image/jpeg */
extern const char MIME_IMAGE_PJPEG[]; /* image/pjpeg */
extern const char MIME_IMAGE_PNG[];   /* image/png */
extern const char MIME_IMAGE_SVG[];   /* image/svg+xml */
extern const char MIME_IMAGE_TIFF[];  /* image/tiff */
extern const char MIME_IMAGE_ICON[];  /* image/vnd.microsoft.icon */
extern const char MIME_IMAGE_WBMP[];  /* image/vnd.wap.wbmp */
extern const char MIME_MPART_MIXED[]; /* multipart/mixed */
extern const char MIME_MPART_ALT[];   /* multipart/alternative */
extern const char MIME_MPART_REL[];   /* multipart/related */
extern const char MIME_MPART_FORM[];  /* multipart/form-data */
extern const char MIME_MPART_SIGN[];  /* multipart/signed */
extern const char MIME_MPART_ENCR[];  /* multipart/encrypted */

typedef enum http_method
{
	METHOD_NONE,
	METHOD_GET,
	METHOD_POST,
	METHOD_HEAD,
	METHOD_PUT,
	METHOD_CONNECT,
	METHOD_OPTIONS,
	METHOD_DELETE,
	METHOD_TRACE,
	METHOD_PATCH
} http_mt_t;

typedef enum http_conn_type
{
	CT_NONE,
	CT_CLOSE,
	CT_KEEP_ALIVE
} http_ct_t;

/* http content disposition */
typedef struct http_cdh
{
    const char *disp;         /* Content-Disposition: [form-data]; name="File1"; filename="photo.jpg" */
    const char *disp_name;    /* Content-Disposition: form-data; name="[File1]"; filename="photo.jpg" */
    const char *disp_file;    /* Content-Disposition: form-data; name="File1"; filename="[photo.jpg]" */
} http_cdh_t;

typedef struct http_range
{
    uint32_t   range_size;
    uint32_t   range_from;
    uint32_t   range_to;
} http_rng_t;

/* http response/request entity */

typedef struct http_cont
{
    http_cdh_t  disp;        /* Content-Disposition: [form-data; name="File1"; filename="photo.jpg"] */
    const char *encoding;    /* Content-Encoding: [...] */
    const char *language;    /* Content-Language: [en, ase, ru] */
    int         length;      /* Content-Length: [123] */
    const char *location;    /* Content-Location: [...] */
    const char *md5;         /* Content-MD5: [Q2hlY2sgSW50ZWdyaXR5IQ==] */
    http_rng_t  range;       /* Content-Range: [64397516-80496894/160993792] */
    const char *type;        /* Content-Type: [multipart/form-data] */
    const char *boundary;    /* Content-Type: multipart/form-data; boundary="[Asrf456BGe4h]" */
    const char *charset;     /* Content-Type: Content-Type: text/html; charset=[UTF-8] */
    const char *ver;         /* Content-Version: [...] */
    const char *expires;     /* Expires: [Tue, 31 Jan 2012 15:02:53 GMT] */
    const char *modified;    /* Last-Modified: [...] */
    const char *link;        /* link: [...] */
    const char *title;       /* Title: [...] */
} http_cont_t;

/* http transfer encoding */
typedef enum http_tenc
{
    TENC_NONE,   /* - */
    TENC_CHUNKED /* Transfer-Encoding: chunked */
} http_tenc_t;

/* http transfer encoding */
typedef struct httpaccept
{
    int         count;
    const char *type[HTTP_REQ_MAX_ACCEPT];  /* text/html */
    int         level[HTTP_REQ_MAX_ACCEPT]; /* level=[1] */
    float       q[HTTP_REQ_MAX_ACCEPT];     /* q=[0.8] */
} httpaccept_t;

typedef struct http_req
{
    /* request line */
    http_mt_t      method;                      /* GET          */
    const char    *uri;                         /*      /path   */
    int            nparams;                     /*              */
    const char    *params[HTTP_REQ_MAX_PARAMS]; /*                      param1, param2 */
    const char    *values[HTTP_REQ_MAX_PARAMS]; /*                      value1, value2 */
    const char    *ver;                         /*                                      HTTP/1.1 */

    /* general header */
    http_ct_t      connection;                  /* Connection:          [keep-alive] */
    char          *via;                         /* Via: [1.0 fred, 1.1 example.com] */

    /* request header */
    httpaccept_t   accept;
    const char    *accept_charset;              /* Accept-Charset:      [utf-8] */
    const char    *accept_encoding;             /* Accept-Encoding:     [gzip, deflate] */
    const char    *accept_language;             /* Accept-Language:     [en-US;q=0.5,en;q=0.3] */
    const char    *authorization;               /* Authorization:       [Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ==] */
    const char    *expect;                      /* Expect:              [100-continue] */
    const char    *from;                        /* From:                [user@example.com] */
    const char    *host;                        /* Host:                [wikipedia.org] */
    const char    *if_match;                    /* If-Match:           "[737060cd8c284d8af7ad3082f209582d]" */
    const char    *if_modified_since;           /* If-Modified-Since:   [Sat, 29 Oct 1994 19:43:31 GMT] */
    const char    *if_none_match;               /* If-None-Match:      "[737060cd8c284d8af7ad3082f209582d]" */
    const char    *if_range;                    /* If-Range:           "[737060cd8c284d8af7ad3082f209582d]" */
    const char    *if_unmodified_since;         /* If-Unmodified-Since: [Sat, 29 Oct 1994 19:43:31 GMT] */
    int            keep_alive;                  /* Keep-Alive:          [300] */
    int            max_forwards;                /* Max-Forwards:        [10] */
    const char    *proxy_authorization;         /* Proxy-Authorization: [Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ==] */
    http_rng_t     range;                       /* Range:         bytes=[50000-99999],250000-399999,500000- */
    const char    *referer;                     /* Referer:             [http://en.wikipedia.org/wiki/Main_Page] */
    const char    *te;                          /* TE:                  [trailers, deflate] */
    const char    *user_agent;                  /* User-Agent:          [Mozilla/5.0] */

    /* content and cookies */
    http_cont_t    content;                     /**/
    const char    *cookie;                      /* Cookie:              [Cookie data] */
} http_req_t;

typedef enum http_resp_flag
{
    RESPF_NONE      = 0,
    RESPF_KEEPALIVE = 1,
    RESPF_CHUNKED   = 2,
    RESPF_NOCACH    = 4,
    RESPF_DEFLATE   = 8,
    RESPF_GZIP      = 16
} http_respf_t;

typedef struct http_resp
{

    char       *mime_ver;                    /* MIME-Version: 1.0 */
    char       *pragma;                      /* Pragma:              [no-cache] */
    int         code;                        /* HTTP/1.1 [200] OK */
    http_cont_t content;                     /**/
    http_ct_t   connection;                  /* Connection: [keep-alive] */
    const char *cache_control;               /* Cache-Control: [no-cache] */
    const char *etag;                        /* ETag: "[56d-9989200-1132c580]" */
    const char *location;                    /* Location: [http://example.com/about.html#contacts] */
    const char *proxy_authenticate;          /* Proxy-Authenticate: [...] */
    const char *Public;                      /* Public: [...] */
    int         retry_after;                 /* Retry-After: [123] */
    const char *server;                      /* Server: [Name] */
    http_tenc_t transfer_encoding;           /* Transfer-Encoding: [chunked] */
    const char *vary;                        /* Vary: [...] */
    const char *www_authenticate;            /* WWW-Authenticate: [...] */
    char       *upgrade;                     /* Upgrade: [HTTP/2.0, SHTTP/1.3, IRC/6.9, RTA/x11] */
    const char *exthdr;                      /* extended header options */
} http_resp_t;

const char *http_req_val(const http_req_t *req, const char *param, const char *def);
void        http_resp_init(http_resp_t *resp, int code, const char *mime, int flags);
const char *http_dict_mime(const char *mime);
const char *http_code_str(int code);
int         http_resp_len(const http_resp_t *resp);
int         http_resp_str(const http_resp_t *resp, char *str, int size);

#ifdef __cplusplus
}
#endif

#endif
