/*
 * HTTP convenience functions, these just wrap the low-level HTTP API
 * that Zephyr provides
 *
 */

#include <stdio.h>
#include <stdlib.h>

#ifndef HTTP_H
#define HTTP_H

#ifndef HTTP_TIMEOUT
#define HTTP_TIMEOUT 3000
#endif

// HTTP GET operation
// TODO TLS? https_get?
ssize_t http_get(
        const char *host,
        uint16_t port,
        const char *path,
        // TODO headers?
        uint8_t *buf,
        size_t buf_len);

// HTTP POST operation
ssize_t http_post(
        const char *host,
        uint16_t port,
        const char *path,
        // TODO headers?
        const uint8_t *payload_buf,
        size_t payload_buf_len,
        uint8_t *resp_buf,
        size_t resp_buf_len);

#endif
