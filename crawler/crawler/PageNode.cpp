//
// Created by Buck Shlegeris on 11/20/16.
//

#include "PageNode.h"

PageNode::PageNode(Uri uri): uri(uri) {}

//todo: this
//std::ostream& operator<<(std::ostream &strm, const PageNode &page_node) {
//    return strm << "A(" << page_node.uri.path << ")";
//}

void PageNode::set_request_type(request_type_t request_type) {
    this->request_type = request_type;
}

void PageNode::set_page_node_status(node_status_t node_status) {
    this->node_status = node_status;
}

void PageNode::set_error(std::string error) {
    this->error = error;
}

void PageNode::set_response_code(int response_code) {
    this->response_code = response_code;
}
