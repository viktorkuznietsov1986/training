//
// Created by Buck Shlegeris on 11/21/16.
//

#include <sstream>
#include <regex>
#include <algorithm>
#include <iostream>
#include "HtmlHelper.h"


void replace_all(std::string &str, const std::string &from, const std::string &to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

std::string HtmlHelper::clean_up_href(std::string href) {
    replace_all(href, " ", "%20");
    return href;
}

std::string HtmlHelper::absolutize_path(std::string path, std::string base_path) {
    if (path == std::string("")) {
        return "";
    } else if (path[0] == '/' && path[1] != '.') {
        // That condition means "If the path starts with a '/' and its second character is not a '.'.
        // So it matches "/foo/bar" but not "foo/bar" or "/./foo/bar".

        return path;
    } else {
        std::vector<std::string> sections;
        {
            std::stringstream initial_path(base_path);
            std::string section;
            while(std::getline(initial_path, section, '/')) {
                if (section.length() > 0) {
                    sections.push_back(section);
                }
            }

            if (base_path[base_path.length() - 1] != '/') {
                sections.pop_back();
            }

            std::stringstream new_path(path);
            while(std::getline(new_path, section, '/')) {
                if (section.length() > 0) {
                    sections.push_back(section);
                }
            }
        }

        std::vector<std::string> out;

        for(auto section: sections) {
            if (section == "." || section == "") {
                // do nothing
            } else if (section == "..") {
                out.pop_back();
            } else {
                out.push_back(section);
            }
        }

        std::stringstream res;
        copy(out.begin(), out.end(), std::ostream_iterator<std::string>(res, "/"));
        std::string res_str = res.str();
        res_str.pop_back();

        return "/" + res_str;
    }
}

std::vector<Uri> HtmlHelper::get_neighbors(std::string body_str, Uri uri, std::vector<std::string> *errors) {
    std::vector<std::string> paths_to_follow = get_url_strings_from_doc(body_str);
    return get_urls_from_page(paths_to_follow, uri, errors);
}

std::vector<Uri> HtmlHelper::get_urls_from_page(
        std::vector<std::string> paths_to_follow,
        Uri parent_uri,
        std::vector<std::string> *errors) {
    std::vector<Uri> out;

    for(auto href : paths_to_follow) {
        href = clean_up_href(href);

        if (href[0] == '#') {
            continue;
        }
        if (href.substr(0, 6) == std::string("mailto")) {
            continue;
        }

        Uri href_uri = Uri::parse(href);

        // default protocol is the parent's protocol
        if (href_uri.uri_protocol == "") href_uri.uri_protocol = parent_uri.uri_protocol;

        if (href_uri.host == "" || href_uri.host == ".") {
            href_uri.host = parent_uri.host;
            href_uri.port = parent_uri.port;
            href_uri.path = absolutize_path(href_uri.path, parent_uri.path);
        }

        out.push_back(href_uri);
    }

    return out;
}

std::vector<std::string> HtmlHelper::get_url_strings_from_doc(std::string body_str) {
    std::vector<std::string> out;

    std::vector<std::regex> regexes({
                    std::regex("<a [^>]*href=\"([^\"]*)"),
                    std::regex("<link [^>]*href=\"([^\"]*)\""),
                    std::regex("<script [^>]*src=\"([^\"]*)\"")});


    std::sregex_iterator end;

    for( auto regex: regexes ) {
        std::sregex_iterator iter(body_str.cbegin(), body_str.cend(), regex);
        for (; iter != end; ++iter) {
            std::smatch match = *iter;
            std::string x = match[1];
            out.push_back(x);
        }
    }

    return out;
}
