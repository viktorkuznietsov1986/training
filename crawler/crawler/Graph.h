//
// Created by Buck Shlegeris on 11/20/16.
//

#ifndef WEB_CRAWLER_GRAPH_H
#define WEB_CRAWLER_GRAPH_H


#include "PageNode.h"
#include <string>
#include <map>
#include <set>
#include <memory>

class Graph {
public:
    std::map<Uri, std::shared_ptr<PageNode> > nodes;
    std::map<Uri, std::set<Uri> > outgoing_links;
    std::map<Uri, std::set<Uri> > incoming_links;
    
    Graph();
    void add_node(Uri uri);
    std::shared_ptr<PageNode> get(Uri uri);
    std::shared_ptr<PageNode> get(std::string uri);
    void add_neighbor(Uri from_uri, Uri to_uri);
    std::set<Uri> parents(Uri uri);
    bool containsUri(Uri uri);
    unsigned long size();
};


#endif //WEB_CRAWLER_GRAPH_H
