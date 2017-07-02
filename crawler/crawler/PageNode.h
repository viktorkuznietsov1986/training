//
// Created by Buck Shlegeris on 11/20/16.
//

#ifndef WEB_CRAWLER_PAGENODE_H
#define WEB_CRAWLER_PAGENODE_H

#include "Uri.h"

enum node_status_t {
    NONE, ENQUEUED, SUCCESS, FAILURE
};

enum request_type_t {
    GET, HEAD
};

class PageNode {
public:
    Uri uri;

    request_type_t request_type;
    node_status_t node_status = NONE;
    std::string error;
    int response_code;

    PageNode(Uri uri);

    void set_request_type(request_type_t request_type);
    void set_page_node_status(node_status_t node_status);
    void set_error(std::string error);
    void set_response_code(int response_code);
};



#endif //WEB_CRAWLER_PAGENODE_H
