//
// Created by Buck Shlegeris on 12/3/16.
//

#ifndef WEB_CRAWLER_CRAWLER_H
#define WEB_CRAWLER_CRAWLER_H


#include <queue>
#include <set>
#include <mutex>
#include <curl/curl.h>
#include "Graph.h"

class Crawler {
    int number_of_running_threads;
    int max_threads;
    std::deque<Uri> queue;
    std::mutex queue_mutex;
    std::set<Uri> currently_being_explored;
    std::mutex currently_being_explored_mutex;

    std::mutex errors_mutex;
    std::vector<std::string> errors;
    Uri domain;
    Graph graph;
public:
    Crawler(int max_threads);
    Graph crawl(std::string initialUrlString, bool displayResults);

private:
    void enqueue(Uri uri);
    void spawn_crawling_thread(Uri uri);

    bool uri_should_be_crawled_as_node(Uri uri);

    void crawl_with_get_request(Uri uri);
    void crawl_with_head_request(Uri uri);
    void note_error(std::string err);

    void finalize_crawl(Uri uri);
};


#endif //WEB_CRAWLER_CRAWLER_H
