//
// Created by Buck Shlegeris on 12/3/16.
//

#include "Crawler.h"
#include "HtmlHelper.h"
#include "CurlWrapper.h"
#include <chrono>
#include <thread>
#include <iostream>

Crawler::Crawler(int max_threads): max_threads(max_threads) {
    curl_global_init(CURL_GLOBAL_ALL);
};

Graph Crawler::crawl(std::string initial_url_string, bool display_results) {
    Uri initial_uri = Uri::parse(initial_url_string);

    this->domain = Uri::parse(initial_url_string);
    this->domain.path = "";
    this->domain.query_string = "";
    this->domain.fragment = "";

    this->enqueue(initial_uri);

    while(!queue.empty() || !currently_being_explored.empty()) {
        printf("Crawling, queue has length %lu, currently_being_explored has size %ld\n", queue.size(), currently_being_explored.size());

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (display_results) {
        printf("\n\ndone crawling! now we're done\n");

        printf("we explored %ld urls!\n\n", graph.size());

        if (errors.empty()) {
            printf("No errors found!\n");
        } else {
            printf("Here are all the complaints found:\n\n");
            for (auto error : errors) {
                printf("%s\n", error.c_str());
            }
        }
    }

    return graph;
}

void Crawler::enqueue(Uri uri) {
    this->queue_mutex.lock();

    if (this->graph.containsUri(uri)) {
        this->queue_mutex.unlock();
        return;
    } else {
        std::cout << "enqueueing " << uri.to_string() << ". Queue has size " << this->queue.size() << std::endl;
    }

    graph.add_node(uri);

    if (this->number_of_running_threads < this->max_threads) {
        number_of_running_threads ++;
        this->spawn_crawling_thread(uri);
    } else {
        this->queue.push_back(uri);
    }

    this->queue_mutex.unlock();
}

void Crawler::spawn_crawling_thread(Uri uri) {
    currently_being_explored_mutex.lock();
    currently_being_explored.insert(uri);
    currently_being_explored_mutex.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    if (uri_should_be_crawled_as_node(uri)) {
        std::thread t (&Crawler::crawl_with_get_request, this, uri);
        t.detach();
    } else {
        std::thread t (&Crawler::crawl_with_head_request, this, uri);
        t.detach();
    }

    return;
}

bool Crawler::uri_should_be_crawled_as_node(Uri uri) {
    if (domain.host != uri.host) {
        return false;
    }

    // prevent you from starting to crawl FTP if you're looking at HTTP
//    if (domain.uri_protocol != uri.uri_protocol) {
//        return false;
//    }

    std::vector<std::string> filetype_list = {"pdf", "jpg", "gif", "js", "css", "png"};

    u_long i = uri.path.find_last_of('.');
    if (i > 0) {
        std::string extension = uri.path.substr(i+1, uri.path.length());

        if (std::find(filetype_list.begin(), filetype_list.end(), extension) != filetype_list.end()) {
            return false;
        }
    }

    return true;
}

void Crawler::crawl_with_get_request(Uri uri) {
    std::shared_ptr<PageNode> node = this->graph.get(uri);
    node->request_type = GET;

    CurlResult result = CurlWrapper::request(uri.to_string(), GET);
    int http_code = result.response_code;

    if (http_code >= 400 || http_code == 0) {
        char msg[200];
        Uri parent_uri;
        for (Uri uri_ : this->graph.parents(uri)) {
            parent_uri = uri_; break;
        }
        sprintf(msg, "When crawling %s, got a %d (linked from %s)", uri.to_string().c_str(), http_code, parent_uri.to_string().c_str());

        node->node_status = FAILURE;
        note_error(std::string(msg));
    } else {
        printf("Response code was %d\n", http_code);
        // todo: require that the response type was text/something

        for (Uri neighbor_uri : HtmlHelper::get_neighbors(result.response_body, uri, &this->errors)) {
            graph.add_neighbor(uri, neighbor_uri);
            enqueue(neighbor_uri);
        }
        node->node_status = SUCCESS;
    }

    finalize_crawl(uri);
}

void Crawler::crawl_with_head_request(Uri uri) {
    std::shared_ptr<PageNode> node = this->graph.get(uri);
    node->request_type = GET;

    CurlResult result = CurlWrapper::request(uri.to_string(), HEAD);
    int http_code = result.response_code;

    if (http_code >= 400 || http_code == 0) {
        char msg[200];
        Uri parent_uri;
        for (Uri uri_ : this->graph.parents(uri)) {
            parent_uri = uri_; break;
        }
        sprintf(msg, "When crawling %s, got a %d (linked from %s)", uri.to_string().c_str(), http_code, parent_uri.to_string().c_str());

        note_error(std::string(msg));
        node->node_status = FAILURE;
        if (http_code != 0) {
            finalize_crawl(uri);
        }
    } else {
        finalize_crawl(uri);
        node->node_status = SUCCESS;
    }
}

void Crawler::finalize_crawl(Uri uri) {
    std::cout << uri.to_string() << std::endl;
    currently_being_explored.erase(uri);

    if (queue.empty()) {
        number_of_running_threads --;
    } else {
        queue_mutex.lock();
        auto queue_url = queue.front();
        queue.pop_front();
        queue_mutex.unlock();

        this->spawn_crawling_thread(queue_url);
    }
}

void Crawler::note_error(std::string error) {
    this->errors_mutex.lock();
    this->errors.push_back(error);
    this->errors_mutex.unlock();
}


