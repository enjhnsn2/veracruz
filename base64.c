/*
 * base64 utilities
 *
 */

#include "base64.h"
#include <kernel.h>
#include <string.h>

// compute size after encoding
size_t base64_encode_size(size_t in_len) {
    size_t x = in_len;
    if (in_len % 3 != 0) {
        x += 3 - (in_len % 3);
    }

    return 4*(x/3);
}

static const char BASE64_ENCODE[] = (
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/");

// encode base64
ssize_t base64_encode(
        const uint8_t *in, size_t in_len,
        char *out, size_t out_len) {
    size_t e_len = base64_encode_size(in_len);
    if (e_len+1 > out_len) {
        return -EOVERFLOW;
    }
    out[e_len] = '\0';

    for (size_t i=0, j=0; i < in_len; i += 3, j += 4) {
        size_t v = in[i];
        v = i+1 < e_len ? (v << 8 | in[i+1]) : (v << 8);
        v = i+2 < e_len ? (v << 8 | in[i+2]) : (v << 8);

        out[j]   = BASE64_ENCODE[(v >> 18) & 0x3f];
        out[j+1] = BASE64_ENCODE[(v >> 12) & 0x3f];

        if (i+1 < in_len) {
            out[j+2] = BASE64_ENCODE[(v >> 6) & 0x3f];
        } else {
            out[j+2] = '=';
        }

        if (i+2 < in_len) {
            out[j+3] = BASE64_ENCODE[v & 0x3f];
        } else {
            out[j+3] = '=';
        }
    }

    return e_len;
}

// compute size after decoding
size_t base64_decode_size(const char *in) {
    size_t in_len = strlen(in);

    size_t x = 3*(in_len/4);
    for (size_t i = 0; i < in_len && in[in_len-i-1] == '='; i++) {
        x -= 1;
    }

    return x;
}

static const int8_t BASE64_DECODE[] = {
    62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1,
    -1, -1, -1, -1, -1, -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
    -1, -1, -1, -1, -1, -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
    36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
};

static bool base64_isvalid(char c) {
    if (c >= '0' && c <= '9') {
        return true;
    } else if (c >= 'A' && c <= 'Z') {
        return true;
    } else if (c >= 'a' && c <= 'z') {
        return true;
    } else if (c == '+' || c == '/' || c == '=') {
        return true;
    } else {
        return false;
    }
}

// decode base64
ssize_t base64_decode(
        const char *in,
        char *out, size_t out_len) {
    size_t in_len = strlen(in);
    if (in_len % 4 != 0) {
        return -EINVAL;
    }

    size_t d_len = base64_decode_size(in);
    if (d_len > out_len) {
        return -EOVERFLOW;
    }

    for (size_t i = 0; i < in_len; i++) {
        if (!base64_isvalid(in[i])) {
            return -EILSEQ;
        }
    }

    for (size_t i=0, j=0; i < in_len; i += 4, j += 3) {
        size_t v = BASE64_DECODE[in[i]-43];
        v = (v << 6) | BASE64_DECODE[in[i+1]-43];
        v = in[i+2] == '=' ? (v << 6) : ((v << 6) | BASE64_DECODE[in[i+2]-43]);
        v = in[i+3] == '=' ? (v << 6) : ((v << 6) | BASE64_DECODE[in[i+3]-43]);

        out[j] = (v >> 16) & 0xff;

        if (in[i+2] != '=') {
            out[j+1] = (v >> 8) & 0xff;
        }

        if (in[i+3] != '=') {
            out[j+2] = v & 0xff;
        }
    }

    return d_len;
}

