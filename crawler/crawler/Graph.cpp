//
// Created by Buck Shlegeris on 11/20/16.
//

#include "Graph.h"
#include <vector>
#include <algorithm>
#include <memory>

void Graph::add_node(Uri uri) {
    if (this->nodes.count(uri) == 0) {
        this->nodes[uri] = std::shared_ptr<PageNode>(new PageNode(uri));
        this->nodes[uri]->node_status = ENQUEUED;
    }
}

std::shared_ptr<PageNode> Graph::get(Uri uri) {
    if (containsUri(uri)) {
        return this->nodes[uri];
    } else {
        return std::shared_ptr<PageNode>(new PageNode(uri));
    }
}

void Graph::add_neighbor(Uri from_uri, Uri to_uri) {
    if (this->outgoing_links.count(from_uri) == 0) {
        this->outgoing_links[from_uri] = std::set<Uri>();
    }
    this->outgoing_links[from_uri].insert(to_uri);

    if (this->incoming_links.count(to_uri) == 0) {
        this->incoming_links[to_uri] = std::set<Uri>();
    }
    this->incoming_links[to_uri].insert(from_uri);
}

std::set<Uri> Graph::parents(Uri uri) {
    if (this->incoming_links.count(uri) == 0) {
        return std::set<Uri>();
    } else {
        return this->incoming_links[uri];
    }
}

bool Graph::containsUri(Uri uri) {
    return this->nodes.count(uri) == 1;
};

Graph::Graph() {}

unsigned long Graph::size() {
    return this->nodes.size();
}

std::shared_ptr<PageNode> Graph::get(std::string uri) {
    return get(Uri::parse(uri));
};
