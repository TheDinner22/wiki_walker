#include "./pages/pages.hpp"
#include "the_graph/the_graph.hpp"
#include <string>
#include <unordered_map>

#define CPPHTTPLIB_OPENSSL_SUPPORT

// function straight from here
// https://stackoverflow.com/questions/631664/accessing-environment-variables-in-c
std::string get_env_var( std::string const & key )
{
    char * val = getenv( key.c_str() );
    return val == NULL ? std::string("") : std::string(val);
}

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
        // create the tree (or get a cache hit)
        bool result = pages::create_graph(req, graphs);
        if(!result){ std::cout << "cant make graph" << std::endl; }

        // return the graph as json
        ReqParams params(req);
        res.set_content(graphs[params.graph_name].as_json(), "application/json");
    });

    // called when the button is pressed and both inputs are sent in as params
    svr.Get("/api/perform_search", [&graphs](const httplib::Request &req, httplib::Response &res) {
        pages::perform_search(req, res, "", graphs);
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

    // hubert button
    svr.Get("/api/perform_a_search", [&graphs](const httplib::Request &req, httplib::Response &res) {
        pages::perform_search(req, res, "a", graphs);
    });

    std::string p = get_env_var("PORT");
    std::cout << "port:" << p << std::endl;
    svr.listen("0.0.0.0", std::stoi(p));
}

