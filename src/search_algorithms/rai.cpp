#include "search_algorithms.hpp"
#include <string>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

ParseResults rai_algo_bfs(const std::string& start, const std::string& end, const Graph& graph){
    ParseResults r;
    r.num_requests_sent = 0;
    r.algo_name = "rai bfs";
    r.shortest_path = {}; // TODO

    queue<string> toVisit;
    unordered_set<string> visited;

    toVisit.push(start);
    visited.insert(start);

    while (!toVisit.empty()) {
        string currentUrl = toVisit.front();
        toVisit.pop();

        if (currentUrl == end) {
            cout << "Found URL using BFS: " << currentUrl << endl;
            // TODO
        }

        // this for loop "sends a request" getting a nodes adjacent nodes would require an http request
        // if the graph weren't already cached
        r.num_requests_sent++;
        for (const string& neighbor : graph.getAdjacent(currentUrl)) {
            if (visited.count(neighbor) == 0) {
                toVisit.push(neighbor);
                visited.insert(neighbor);
            }
        }
    }

    r.pages_visited = visited.size();
    return r;

}
ParseResults rai_algo_dfs(const std::string& start, const std::string& end, const Graph& graph){
    ParseResults r;
    r.num_requests_sent = 0;
    r.algo_name = "rai dfs";
    r.shortest_path = {};

    stack<string> toVisit;
    unordered_set<string> visited;

    toVisit.push(start);

    while (!toVisit.empty()) {
        string currentUrl = toVisit.top();
        toVisit.pop();

        visited.insert(currentUrl);

        if (currentUrl == end) {
            cout << "Found URL using DFS: " << currentUrl << endl;
            // TODO
        }

        // this for loop "sends a request" getting a nodes adjacent nodes would require an http request
        // if the graph weren't already cached
        r.num_requests_sent++;
        for (const string& neighbor : graph.getAdjacent(currentUrl)) {
            if (visited.count(neighbor) == 0) {
                toVisit.push(neighbor);
            }
        }
    }

    cout << "End URL not found using DFS." << endl;
    r.pages_visited = visited.size();
    return r;
}

