//
// Created by Buck Shlegeris on 12/11/16.
//

#include <curl/curl.h>
#include <iostream>
#include "CurlWrapper.h"

CurlResult CurlWrapper::request(std::string uri, request_type_t request_type) {
    std::string data;
    CURL* curl = curl_easy_init();

    int http_code = 0;

    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
    // Four second timeout
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 4L);

    if (request_type == GET) {
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrapper::handle);
    } else {
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    }

    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    return CurlResult(http_code, data);
}

size_t CurlWrapper::handle(char *data, size_t size, size_t nmemb, std::string *output) {
    for (int c = 0; c < size * nmemb; c++) {
        output->push_back(data[c]);
    }

    return size * nmemb;
}
