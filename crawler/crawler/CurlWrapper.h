//
// Created by Buck Shlegeris on 12/11/16.
//

#ifndef WEB_CRAWLER_CURLWRAPPER_H
#define WEB_CRAWLER_CURLWRAPPER_H


#include <string>
#include "PageNode.h"

class CurlResult {
public:
    int response_code;
    std::string response_body;

    CurlResult(int response_code, std::string response_body):
            response_code(response_code), response_body(response_body) {
    }
};

class CurlWrapper {

public:
    static CurlResult request(std::string uri, request_type_t request_type);
    static size_t handle(char * data, size_t size, size_t nmemb, std::string *output);
};



#endif //WEB_CRAWLER_CURLWRAPPER_H
