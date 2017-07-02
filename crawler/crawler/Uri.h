//
// Created by Buck Shlegeris on 11/20/16.
//

#ifndef WEB_CRAWLER_URI_H
#define WEB_CRAWLER_URI_H


// modified from http://stackoverflow.com/a/11044337/1360429

#include <string>
#include <algorithm>    // find

class Uri {
public:
    std::string uri_protocol, host, port, path, query_string, fragment;

    static Uri parse(const std::string &uri)
    {
        Uri result;

        typedef std::string::const_iterator iterator_t;

        if (uri.length() == 0)
            return result;

        iterator_t uriEnd = uri.end();

        // get query start
        iterator_t queryStart = std::find(uri.begin(), uriEnd, '?');

        // get fragment start
        iterator_t fragmentStart = std::find(uri.begin(), uriEnd, '#');

        // protocol
        iterator_t protocolStart = uri.begin();
        iterator_t protocolEnd = std::find(protocolStart, uriEnd, ':');            //"://");

        iterator_t pathStart;

        if (protocolEnd != uriEnd)
        {
            std::string prot = &*(protocolEnd);
            if ((prot.length() > 3) && (prot.substr(0, 3) == "://")) {
                result.uri_protocol = std::string(protocolStart, protocolEnd);
                protocolEnd += 3;   //      ://
            } else {
                protocolEnd = uri.begin();  // no protocol
            }

            // host
            iterator_t hostStart = protocolEnd;
            pathStart = std::find(hostStart, uriEnd, '/');  // get pathStart

            iterator_t hostEnd = std::find(protocolEnd,
                                           (pathStart != uriEnd) ? pathStart : queryStart,
                                           ':');  // check for port

            result.host = std::string(hostStart, hostEnd);

            if (result.host == ".") {
                result.host = "";
                pathStart++;
            }

            // port
            if ((hostEnd != uriEnd) && ((&*(hostEnd))[0] == ':'))  // we have a port
            {
                hostEnd++;
                iterator_t portEnd = (pathStart != uriEnd) ? pathStart : queryStart;
                result.port = std::string(hostEnd, portEnd);
            }
        }
        else {
            result.uri_protocol = "";
            result.host = "";
            result.port = "";

            pathStart = uri.begin();
        }

        // path
        if (pathStart != uriEnd)
            result.path = std::string(pathStart, queryStart);

        // query
        if (queryStart != uriEnd)
            result.query_string = std::string(queryStart, fragmentStart);

        // fragment
        if (fragmentStart != uriEnd)
            result.fragment = std::string(queryStart, uri.end());

        return result;

    }   // parse

    std::string to_string() const {
        char res[200];
        sprintf(res, "%s://%s%s%s?%s#%s",
                uri_protocol.c_str(), host.c_str(), port.length() ? (":" + port).c_str() : "", path.c_str(), query_string.c_str(), fragment.c_str());
        return std::string(res);
    }

    std::string to_verbose_string() {
        char res[200];
        sprintf(res, "protocol: %s, host: %s, port: %s, path: %s, query_string: %s, fragment: %s",
                uri_protocol.c_str(), host.c_str(), port.length() ? (":" + port).c_str() : "", path.c_str(), query_string.c_str(), fragment.c_str());
        return std::string(res);
    }
};

bool operator <(const Uri& x, const Uri& y);

#endif //WEB_CRAWLER_URI_H
