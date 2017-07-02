#include <iostream>
#include <string>
#include <curl/curl.h>
#include "Crawler.h"
#include "HtmlHelper.h"
#include "CurlWrapper.h"

using namespace std;


void already_passing_tests();
void failing_test_1();
void failing_test_2();
void failing_test_3();
void failing_test_4();

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "wrong number of arguments, usage: ./crawler TEST_NUMBER" << std::endl;

        return 1;
    }

    int command = std::stoi(argv[1]);

    switch (command) {
        case 0: already_passing_tests(); break;
        case 1: failing_test_1(); break;
        case 2: failing_test_2(); break;
        case 3: failing_test_3(); break;
        case 4: failing_test_4(); break;
        default: std::cout << "That command is not valid; commands have to be in {0, 1, 2, 3, 4}" << std::endl;
    }


    return 0;
}

void assert(bool assertion, std::string msg) {
    std::cerr << (assertion ? "Test passed: " : "Test failed: ") << msg << std::endl;
}

void already_passing_tests() {
    assert(HtmlHelper::absolutize_path("./page3", "/web-crawler-test-site/test4/cynical.html") == "/web-crawler-test-site/test4/page3",
           "absolutize is broken");
    assert(Uri::parse("SVG_logo.svg").path == "SVG_logo.svg", "parsing works");
    assert(Uri::parse("./SVG_logo.svg").path == "./SVG_logo.svg", "parsing works");
    assert(HtmlHelper::absolutize_path("./SVG_logo.svg", "/") == std::string("/SVG_logo.svg"),
        "absolutize path is returning the right thing"
    );

    Crawler crawler(5);
    Graph graph = crawler.crawl("https://triplebyte.com/", true);

    assert(graph.get("http://cryptopals.com/")->node_status == SUCCESS, "Crawler can find Cryptopals");
    assert(graph.get("https://triplebyte.com/diversity")->node_status == SUCCESS, "Crawler can find diversity page");
    assert(graph.get("https://mixpanel.com/f/partner")->node_status == SUCCESS, "Crawler can find our linked Mixpanel page");
    assert(graph.get("https://triplebyte.com/startup")->node_status == SUCCESS, "Crawler can find our startup page");
    assert(graph.get("https://en.wikipedia.org/wiki/George_Marsaglia")->node_status == NONE, "Crawler did not scrape some random Wikipedia page");
}

void failing_test_1() {
    Crawler crawler(10);
    Graph graph = crawler.crawl("http://triplebyte.github.io/web-crawler-test-site/test1/", true);

    assert(graph.get("http://triplebyte.github.io/web-crawler-test-site/test1/SVG_logo.svg")->request_type == HEAD,
           "Crawler requested the SVG with a HEAD request");
}

void failing_test_2() {
    Crawler crawler(10);
    Graph graph = crawler.crawl("http://triplebyte.github.io/web-crawler-test-site/test2/", true);

    assert(graph.get("http://triplebyte.github.io/web-crawler-test-site/test2/youaretootrusting.html")->node_status == SUCCESS,
           "Crawler correctly finds a broken link");
}

void failing_test_3() {
    Crawler crawler(5);
    Graph graph = crawler.crawl("http://triplebyte.github.io/web-crawler-test-site/test3/", true);

    assert(graph.get("http://triplebyte.github.io/web-crawler-test-site/test3/")->node_status == SUCCESS,
           "Crawler does not hang on a weird page");
}

void failing_test_4() {
    Crawler crawler(5);
    Graph graph = crawler.crawl("http://triplebyte.github.io/web-crawler-test-site/test4/", true);

    for (auto i : graph.nodes) {
        std::cout << i.first.to_string() << std::endl;
    }

    assert(graph.get("http://triplebyte.github.io/web-crawler-test-site/test4/page3")->node_status == SUCCESS,
           "Crawler successfully crawls another weird page");
}
