#include "./pages/pages.hpp"
#include "the_graph/the_graph.hpp"
#include <string>
#include <unordered_map>
#include <fstream>

#define CPPHTTPLIB_OPENSSL_SUPPORT
int main(void){
    // HTTP
    httplib::Server svr;

    // cached set of graphs
    std::unordered_map<std::string, Graph> graphs;

    bool worked = svr.set_mount_point("/public", "./public");
    if(!worked){
        std::cout << "Failed to serve public dir." << std::endl;
    }

    // landing page
    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        res.set_redirect("/public/html/index.html");
    });

    // about us page
    svr.Get("/about", [](const httplib::Request &, httplib::Response &res) {
        res.set_redirect("/public/html/about.html");
    });

    // example use page
    svr.Get("/example", [](const httplib::Request &, httplib::Response &res) {
        res.set_redirect("/public/html/example.html");
    });

    // techs used page
    svr.Get("/technologies", [](const httplib::Request &, httplib::Response &res) {
        res.set_redirect("/public/html/technologies.html");
    });

    // results page
    svr.Get("/results", [](const httplib::Request &, httplib::Response &res) {
        res.set_redirect("/public/html/results.html");
    });

    // someone is typing on an input box
    svr.Get("/api/search_hint", [](const httplib::Request &req, httplib::Response &res) {
        pages::search_hint(req, res);
    });

    // called when we need to create a graph to search
    svr.Get("/api/create_tree", [&graphs](const httplib::Request &req, httplib::Response &res) {
        // create the tree (or get a cache hit)
        bool result = pages::create_graph(req, graphs);
        if(!result){ std::cout << "cant make graph" << std::endl; }

        // return the graph as json
        ReqParams params(req);
        res.set_content(graphs[params.graph_name].as_json(), "application/json");
    });

    // rai button
    svr.Get("/api/perform_bfs_search", [&graphs](const httplib::Request &req, httplib::Response &res) {
        pages::perform_search(req, res, "bfs", graphs);
    });

    // rai button
    svr.Get("/api/perform_dfs_search", [&graphs](const httplib::Request &req, httplib::Response &res) {
        pages::perform_search(req, res, "dfs", graphs);
    });

    // hubert button
    svr.Get("/api/perform_d_search", [&graphs](const httplib::Request &req, httplib::Response &res) {
        pages::perform_search(req, res, "d", graphs);
    });

    // favicon for website
    svr.Get("/favicon.ico", [](const httplib::Request &req, httplib::Response &res) {
        res.set_redirect("/public/imgs/favicon.ico");
    });

    svr.listen("0.0.0.0", 8000);
}

