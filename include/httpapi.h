#ifndef _HTTP_GET_H_
#define _HTTP_GET_H_
#include <stdint.h>

struct http_response {
	uint8_t *data;
	uint32_t length;
	uint16_t response_code;
};

struct http_response http_get(char *url);

#endif
