cmake_minimum_required(VERSION 3.3)
project(web_crawler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lcurl")

set(SOURCE_FILES main.cpp Graph.cpp Graph.h PageNode.cpp PageNode.h Uri.h Uri.cpp HtmlHelper.cpp HtmlHelper.h Crawler.cpp Crawler.h CurlWrapper.cpp CurlWrapper.h)

add_executable(crawler ${SOURCE_FILES})