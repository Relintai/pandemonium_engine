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

#ifndef HTTPIO_H
#define HTTPIO_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "http.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct http_output httpo_t;
typedef struct http_input httpi_t;
typedef struct http_post_info http_post_t;

struct http_post_info
{
    const char *name;
    const char *file;
    const char *disp_type;
    const char *content_type;
    const char *data;         /**< pointer to current post data chunk */
    int         size;         /**< size of current post data chunk */
    int         first;        /**< is it first post data chunk */
    int         last;         /**< is it last post data chunk */
};

/* next codes returns httpi_pull function: */
#define HTTP_IN_NODATA     0    /* have no more input data */
#define HTTP_IN_REQUEST    2    /* http request parsed */
#define HTTP_IN_POSTDATA   32   /* has POST-request data */
#define HTTP_IN_FINISHED   64   /* request parsing is finished */
#define HTTP_IN_ERR_SYNT   256  /* request syntax error */
#define HTTP_IN_ERR_NOMEM  512  /* have no memory in pool */
#define HTTP_IN_ERR_UMETH  1024 /* unknown request method */
#define HTTP_IN_ERR_PARLIM 2048 /* too many request parameters */

/**
 * @brief Initialize new http-input driver
 * @param pool Memory pool buffer
 * @param size Memory pool buffer size
 * @return Http-input driver instance or NULL if \a size value is too small
 * @warning You shouldn't change any data in pool after this call
 *
 * Instance structure will be allocated from a pool.
 * Library has no any dynamic allocation call.
 */
httpi_t *httpi_init(const void *pool, int size);

/**
 * @brief Get the next parsed element
 * @param httpi Http-input driver instance
 * @return One of HTTP_IN_xxx value
 */
int httpi_pull(httpi_t *httpi);

/**
 * @brief Push the received data to a driver instance for it parsing
 * @param httpi Http-input driver instance
 * @param data Pointer to a data
 * @param size Data size
 * @warning You shouldn't change data until the httpi_pull() call not returns HTTP_IN_NODATA value
 */
void httpi_push(httpi_t *httpi, const void *data, int size);

/**
 * @brief Allows to know the pool buffer state
 * @param httpi Http-input driver instance
 * @param pool Output pointer to unused pool space
 * @param avail Output value of unused pool space
 *
 * You can use the unused space to receive the data from a network
 * before the httpi_push() call like this:
 * int size;
 * char *data;
 * httpi_get_state(httpi, &pool, &size);
 * size = recv(client, data, size, 0);
 * httpi_push(httpi, data, size);
 */
void httpi_get_state(httpi_t *httpi, char **pool, int *avail);

/**
 * @brief Get the HTTP-request structure
 * @param httpi Http-input driver instance
 * @return Pointer to the request structure
 *
 * You can do this call if httpi_pull() returned HTTP_IN_REQUEST
 */
const http_req_t  *httpi_request(httpi_t *httpi);

/**
 * @brief Get the next HTTP POST data
 * @param httpi Http-input driver instance
 * @return Pointer to the http_post_t structure
 *
 * You can do this call if httpi_pull() returned HTTP_IN_POSTDATA
 */
const http_post_t *httpi_posted(httpi_t *httpi);

/**
 * @brief Initialize the HTTP-output driver instance
 * @param pool Memory pool buffer
 * @param poolsz Memory pool buffer size
 * @return HTTP-output driver instance
 */
httpo_t *httpo_init(void *pool, int poolsz);

/**
 * @brief Returns the prepared HTTP output data
 * @param out HTTP-output driver instance
 * @param data Output pointer to prepared data
 * @param size Output value of prepared data size
 */
void httpo_state(httpo_t *out, char **data, int *size);

/**
 * @brief Increments the tail pointer of output buffer
 * @param out HTTP-output driver instance
 * @param count The amount of data that has been sent
 */
void httpo_sended(httpo_t *out, int count);

/**
 * @brief Writes the HTTP response to the output buffer
 * @param out HTTP-output driver instance
 * @param resp Responce structure
 * @return 1 if successful, or 0 if not enough memory in the output buffer
 */
int httpo_write_resp(httpo_t *out, const http_resp_t *resp);

/**
 * @brief Writes the response data to the output buffer
 * @param out HTTP-output driver instance
 * @param data Response data
 * @param size Response data size
 * @return 1 if successful, or 0 if not enough memory in the output buffer
 */
int httpo_write_data(httpo_t *out, const void *data, int size);

/**
 * @brief Returns the size of the data available for writing by httpo_write_data
 * @param out HTTP-output driver instance
 * @return Size of the data available for writing
 */
int httpo_write_avail(httpo_t *out);

/**
 * @brief Correctly ends the output stream
 * @param out HTTP-output driver instance
 */
void httpo_finished(httpo_t *out);

#ifdef __cplusplus
}
#endif

#endif
