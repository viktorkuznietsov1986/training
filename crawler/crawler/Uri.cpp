//
// Created by Buck Shlegeris on 11/20/16.
//

#include "Uri.h"

bool operator <(const Uri& x, const Uri& y) {
    if (x.query_string != y.query_string) {
        return x.query_string < y.query_string;
    }
    if (x.path != y.path) {
        return x.path < y.path;
    }
    if (x.uri_protocol != y.uri_protocol) {
        return x.uri_protocol < y.uri_protocol;
    }
    if (x.host != y.host) {
        return x.host < y.host;
    }
    if (x.port != y.port) {
        return x.port < y.port;
    }
    if (x.fragment != y.fragment) {
        return x.fragment < y.fragment;
    }

    return false;
}