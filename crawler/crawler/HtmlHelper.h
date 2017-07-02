//
// Created by Buck Shlegeris on 11/21/16.
//

#ifndef WEB_CRAWLER_HTMLHELPER_H
#define WEB_CRAWLER_HTMLHELPER_H


#include <string>
#include <vector>
#include "Uri.h"

class HtmlHelper {
    static std::string clean_up_href(std::string href);

    static std::vector<Uri> get_urls_from_page(std::vector<std::string> paths_to_follow, Uri parent_uri, std::vector<std::string> *errors);
    static std::vector<std::string> get_url_strings_from_doc(std::string body_str);

public:
    static std::vector<Uri> get_neighbors(std::string body_str, Uri uri, std::vector<std::string> *errors);

    static std::string absolutize_path(std::string path, std::string base_path);
};


#endif //WEB_CRAWLER_HTMLHELPER_H
