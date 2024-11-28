#include "./pages/pages.hpp"
#include "the_graph/the_graph.hpp"
#include <string>
#include <unordered_map>

#define CPPHTTPLIB_OPENSSL_SUPPORT
int main(void){
    // HTTP
    httplib::Server svr;

    // cached set of graphs
    std::unordered_map<std::string, Graph> graphs;

    bool worked = svr.set_mount_point("/public", "./public");
    if(!worked){
        std::cout << "yeah so we are FUCKED" << std::endl;
    }

    // landing page
    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        res.set_redirect("/public/html/index.html");
    });

    // someone is typing on an input
    svr.Get("/api/search_hint", [](const httplib::Request &req, httplib::Response &res) {
        pages::search_hint(req, res);
    });

    // called when we need to create a graph to search
    svr.Get("/api/create_tree", [&graphs](const httplib::Request &req, httplib::Response &res) {
        // create the tree
        // TODO maybe move create_graph out of pages since it isn't generating a response
        bool result = pages::create_graph(req, g);
        if(!result){ std::cout << "cant make graph" << std::endl; }

        // return the graph as json
        res.set_content(g.as_json(), "application/json");
    });

    // called when the button is pressed and both inputs are sent in as params
    svr.Get("/api/perform_search", [&graphs](const httplib::Request &req, httplib::Response &res) {
        pages::perform_search(req, res, "", g);
    });

    // rai button
    svr.Get("/api/perform_bfs_search", [&g](const httplib::Request &req, httplib::Response &res) {
        pages::perform_search(req, res, "bfs", g);
    });

    // rai button
    svr.Get("/api/perform_dfs_search", [&g](const httplib::Request &req, httplib::Response &res) {
        pages::perform_search(req, res, "dfs", g);
    });

    // hubert button
    svr.Get("/api/perform_d_search", [&g](const httplib::Request &req, httplib::Response &res) {
        pages::perform_search(req, res, "d", g);
    });

    // hubert button
    svr.Get("/api/perform_a_search", [&g](const httplib::Request &req, httplib::Response &res) {
        pages::perform_search(req, res, "a", g);
    });

    svr.listen("0.0.0.0", 8000);
}

