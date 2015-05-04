#include "httpapi.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

static size_t write_data(void *data, size_t size, size_t nitems, void *user) {
	size_t total_size = size * nitems;
	struct http_response *response = (struct http_response*)user;

	response->data = realloc(response->data, response->length + total_size + 1);
	if (!response->data) {
		fprintf(stderr, "[http\twarn] ran out of memory while reallocating"
						"response structure!");
		return 0;
	}

	memcpy(response->data + response->length, data, total_size);
	response->length += total_size;
	response->data[response->length] = 0;

	return total_size;
}


struct http_response http_get(char *url) {
	CURL *curl_handle;
	CURLcode res;

	struct http_response response = {0};

	curl_global_init(CURL_GLOBAL_ALL);

	curl_handle = curl_easy_init();

	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&response);
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "apollo/1.0 (http://tehsvk.net)");
	curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1);

	res = curl_easy_perform(curl_handle);

	if (res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
	} else {
		long http_code;
		curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &http_code);
		response.response_code = http_code;
	}

	return response;
}
