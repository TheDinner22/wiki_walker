#include "search_algorithms.hpp"
#include <string>
#include <utility>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

ParseResults rai_algo_bfs(const std::string& start, const std::string& end, const Graph& graph){
    ParseResults r;
    r.num_requests_sent = 0;
    r.algo_name = "rai bfs";

    queue<pair<string, vector<string>>> toVisit;
    unordered_set<string> visited;

    vector<string> path = {start};
    toVisit.push(make_pair(start, path));
    visited.insert(start);

    while (!toVisit.empty()) {
        pair<string, vector<string>> currentUrl = toVisit.front();
        toVisit.pop();

        if (currentUrl.first == end) {
            cout << "Found URL using BFS: " << currentUrl.first << endl;
            r.pages_visited = visited.size();
            r.shortest_path = currentUrl.second;
            return r;
        }

        // this for loop "sends a request" getting a nodes adjacent nodes would require an http request
        // if the graph weren't already cached
        r.num_requests_sent++;
        for (const string& neighbor : graph.getAdjacent(currentUrl.first)) {
            if (visited.count(neighbor) == 0) {
                vector<string> new_path = currentUrl.second;
                new_path.push_back(neighbor);
                toVisit.push(make_pair(neighbor, new_path));
                visited.insert(neighbor);
            }
        }
    }

    r.pages_visited = visited.size();
    r.shortest_path = {};
    std::cout << "there is no path between: " << start << " -> " << end << std::endl;
    return r;

}
ParseResults rai_algo_dfs(const std::string& start, const std::string& end, const Graph& graph){
    ParseResults r;
    r.num_requests_sent = 0;
    r.algo_name = "rai dfs";

    stack<pair<string, vector<string>>> toVisit;
    unordered_set<string> visited;

    vector<string> path = {start};
    toVisit.push(make_pair(start, path));

    while (!toVisit.empty()) {
        pair<string,vector<string>> currentUrl = toVisit.top();
        toVisit.pop();

        visited.insert(currentUrl.first);

        if (currentUrl.first == end) {
            cout << "Found URL using DFS: " << currentUrl.first << endl;
            r.pages_visited = visited.size();
            r.shortest_path = currentUrl.second;
            return r;
        }

        // this for loop "sends a request" getting a nodes adjacent nodes would require an http request
        // if the graph weren't already cached
        r.num_requests_sent++;
        for (const string& neighbor : graph.getAdjacent(currentUrl.first)) {
            if (visited.count(neighbor) == 0) {
                vector<string> new_path = currentUrl.second;
                new_path.push_back(neighbor);
                toVisit.push(make_pair(neighbor, new_path));
            }
        }
    }

    cout << "End URL not found using DFS." << endl;
    r.pages_visited = visited.size();
    r.shortest_path = {};
    return r;
}

