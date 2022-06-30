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

#include "http.h"

#ifdef __cplusplus
extern "C" {
#endif

const char MIME_APP_ATOM[] =    "application/atom+xml";
const char MIME_APP_JSON[] =    "application/json";
const char MIME_APP_JS[] =      "application/javascript";
const char MIME_APP_OCTSTR[] =  "application/octet-stream";
const char MIME_APP_PDF[] =     "application/pdf";
const char MIME_APP_PS[] =      "application/postscript";
const char MIME_APP_XHTML[] =   "application/xhtml+xml";
const char MIME_APP_XML[] =     "application/xml-dtd";
const char MIME_APP_ZIP[] =     "application/zip";
const char MIME_APP_GZIP[] =    "application/x-gzip";
const char MIME_APP_BTOR[] =    "application/x-bittorrent";
const char MIME_APP_TEX[] =     "application/x-tex";
const char MIME_URLENCODED[] =  "application/x-www-form-urlencoded";
const char MIME_TEXT_HTML[] =   "text/html";
const char MIME_TEXT_JS[] =     "text/javascript";
const char MIME_TEXT_PLAIN[] =  "text/plain";
const char MIME_TEXT_XML[] =    "text/xml";
const char MIME_TEXT_CSS[] =    "text/css";
const char MIME_IMAGE_GIF[] =   "image/gif";
const char MIME_IMAGE_JPEG[] =  "image/jpeg";
const char MIME_IMAGE_PJPEG[] = "image/pjpeg";
const char MIME_IMAGE_PNG[] =   "image/png";
const char MIME_IMAGE_SVG[] =   "image/svg+xml";
const char MIME_IMAGE_TIFF[] =  "image/tiff";
const char MIME_IMAGE_ICON[] =  "image/vnd.microsoft.icon";
const char MIME_IMAGE_WBMP[] =  "image/vnd.wap.wbmp";
const char MIME_MPART_MIXED[] = "multipart/mixed";
const char MIME_MPART_ALT[] =   "multipart/alternative";
const char MIME_MPART_REL[] =   "multipart/related";
const char MIME_MPART_FORM[] =  "multipart/form-data";
const char MIME_MPART_SIGN[] =  "multipart/signed";
const char MIME_MPART_ENCR[] =  "multipart/encrypted";

const char *mime_list[] =
{
    MIME_TEXT_HTML,
    MIME_TEXT_PLAIN,
    MIME_URLENCODED,
    MIME_APP_XHTML,
    MIME_APP_XML,
    MIME_TEXT_CSS,
    MIME_TEXT_JS,
    MIME_TEXT_XML,
    MIME_IMAGE_JPEG,
    MIME_IMAGE_PNG,
    MIME_IMAGE_GIF,
    MIME_IMAGE_SVG,
    MIME_MPART_FORM,
    MIME_MPART_MIXED,
    MIME_APP_JS,
    MIME_APP_JSON,
    MIME_APP_ATOM,
    MIME_APP_OCTSTR,
    MIME_APP_PDF,
    MIME_APP_PS,
    MIME_APP_ZIP,
    MIME_APP_GZIP,
    MIME_APP_BTOR,
    MIME_APP_TEX,
    MIME_IMAGE_PJPEG,
    MIME_IMAGE_TIFF,
    MIME_IMAGE_ICON,
    MIME_IMAGE_WBMP,
    MIME_MPART_ALT,
    MIME_MPART_REL,
    MIME_MPART_SIGN,
    MIME_MPART_ENCR,
    NULL
};

struct http_code
{
    int code;
    const char *str;
};

static struct http_code http_code_list[] =
{
  { 100, "Continue" },
  { 101, "Switching Protocols" },
  { 102, "Processing" },
  { 200, "OK" },
  { 201, "Created" },
  { 202, "Accepted" },
  { 203, "Non-Authoritative Information" },
  { 204, "No Content" },
  { 205, "Reset Content" },
  { 206, "Partial Content" },
  { 207, "Multi Status" },
  { 300, "Multiple Choices" },
  { 301, "Moved Permanently" },
  { 302, "Moved Temporarily" },
  { 303, "See Other" },
  { 304, "Not Modified" },
  { 305, "Use Proxy" },
  { 306, "Switch Proxy" },
  { 307, "Temporary Redirect" },
  { 400, "Bad Request" },
  { 401, "Unauthorized" },
  { 402, "Payment Required" },
  { 403, "Forbidden" },
  { 404, "Not Found" },
  { 405, "Method Not Allowed" },
  { 406, "Not Acceptable" },
  { 407, "Proxy Authentication Required" },
  { 408, "Request Time-out" },
  { 409, "Conflict" },
  { 410, "Gone" },
  { 411, "Length Required" },
  { 412, "Precondition Failed" },
  { 413, "Request Entity Too Large" },
  { 414, "Request-URI Too Large" },
  { 415, "Unsupported Media Type" },
  { 416, "Requested Range Not Satisfiable" },
  { 417, "Expectation Failed" },
  { 422, "Unprocessable Entity" },
  { 423, "Locked" },
  { 424, "Failed Dependency" },
  { 425, "Unordered Collection" },
  { 426, "Upgrade Required" },
  { 444, "No Response" },
  { 449, "Retry With" },
  { 450, "Blocked by Windows Parental Controls" },
  { 451, "Unavailable For Legal Reasons" },
  { 500, "Internal Server Error" },
  { 501, "Not Implemented" },
  { 502, "Bad Gateway" },
  { 503, "Service Unavailable" },
  { 504, "Gateway Time-out" },
  { 505, "HTTP Version not supported" },
  { 506, "Variant Also Negotiates" },
  { 507, "Insufficient Storage" },
  { 508, "Unknown" },
  { 509, "Bandwidth Limit Exceeded" },
  { 510, "Not Extended" }
};

static const char *CONN_TYPE_STR[] =
{
	"", "close", "keep-alive"
};

const char *http_req_val(const http_req_t *req, const char *param, const char *def)
{
    int i;
    for (i = 0; i < req->nparams; i++)
        if (strcmp(req->params[i], param) == 0)
            return req->values[i];
    return def;
}

void http_resp_init(http_resp_t *resp, int code, const char *mime, int flags)
{
    memset(resp, 0, sizeof(http_resp_t));
    resp->code = code;
    resp->connection = flags & RESPF_KEEPALIVE ? CT_KEEP_ALIVE : CT_CLOSE;
    resp->content.type = mime;
    resp->content.length = -1;
    resp->content.language = "en";
    resp->cache_control =  flags & RESPF_NOCACH ? "no-store, no-cache" : NULL;
    if (flags & RESPF_DEFLATE) resp->content.encoding = "deflate"; else
    if (flags & RESPF_GZIP) resp->content.encoding = "gzip";
    resp->transfer_encoding = flags & RESPF_CHUNKED ? TENC_CHUNKED : TENC_NONE;
}

const char *http_dict_mime(const char *mime)
{
    int i;
    if (mime == NULL) return NULL;
    for (i = 0; mime_list[i] != NULL; i++)
        if (strcmp(mime_list[i], mime) == 0)
            return mime_list[i];
    return NULL;
}

int http_resp_len(const http_resp_t *resp)
{
    int tmp;
    return http_resp_str(resp, (char *)&tmp, sizeof(tmp));
}

#if defined(__WINNT__) || defined(_WIN32) || defined(_WIN64)
int correct_snprintf(char *buff, int buff_size, const char *fmt, ...)
{
    int res;
    va_list args;
    va_start(args, fmt);
    res = vsnprintf(buff, buff_size, fmt, args);
    if (res < 0) res = _vscprintf(fmt, args);
    va_end(args);
    return res;
}
#else
#define correct_snprintf snprintf
#endif

int http_resp_str(const http_resp_t *resp, char *str, int size)
{
#define PRN(fmt, arg) \
    { \
        n = correct_snprintf(str, size, fmt, arg); \
        str += n; \
        size -= n; \
        res += n; \
        if (size < 0) size = 0; \
    }

#define PRNSTR(prop, field) if (field != NULL) PRN(prop ": %s\r\n", field)

    int res, n;
    res = 0;

    PRN("HTTP/1.1 %d ", resp->code);
    PRN("%s\r\n", http_code_str(resp->code));
    if (resp->content.disp.disp != NULL)
    {
        PRN("Content-Disposition %s", resp->content.disp.disp);
        if (resp->content.disp.disp_name != NULL)
            PRN(" name=\"%s\"", resp->content.disp.disp_name);
        if (resp->content.disp.disp_file != NULL)
            PRN(" filename=\"%s\"", resp->content.disp.disp_file);
        PRN("%s", "\r\n");
    }
    PRNSTR("Content-Encoding", resp->content.encoding);
    PRNSTR("Content-Language", resp->content.language);
    if (resp->content.length > 0)
        PRN("Content-Length: %d\r\n", resp->content.length);
    PRNSTR("Content-Location", resp->content.location);
    PRNSTR("Content-MD5", resp->content.md5);
    /* TODO: http_rng_t  range; */
    PRNSTR("Content-Type", resp->content.type);
    PRNSTR("Content-Version", resp->content.ver);
    PRNSTR("Expires", resp->content.expires);
    PRNSTR("Last-Modified", resp->content.modified);
    PRNSTR("Link", resp->content.link);
    PRNSTR("Title", resp->content.title);
    PRN("Connection: %s\r\n", CONN_TYPE_STR[resp->connection]);
    PRNSTR("Cache-Control", resp->cache_control);
    PRNSTR("ETag", resp->etag);
    PRNSTR("Location", resp->location);
    PRNSTR("Proxy-Authenticate", resp->proxy_authenticate);
    PRNSTR("Public", resp->Public);
    if (resp->retry_after > 0)
        PRN("Retry-After: %d\r\n", resp->retry_after);
    PRNSTR("Server", resp->server);
    if (resp->transfer_encoding == TENC_CHUNKED)
        PRN("Transfer-Encoding: %s\r\n", "chunked");
    PRNSTR("Vary", resp->vary);
    PRNSTR("WWW-Authenticate", resp->www_authenticate);
    if (resp->exthdr != NULL)
        PRN("%s", resp->exthdr);
    PRN("%s", "\r\n");
    return res;
}

const char *http_code_str(int code)
{
    int i, n;
    n = sizeof(http_code_list) / sizeof(struct http_code);
    for (i = 0; i < n; i++)
        if (http_code_list[i].code == code)
            return http_code_list[i].str;
    return "";
}

#ifdef __cplusplus
}
#endif
