#include "search_algorithms.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>

ParseResults rai_algo(const std::string& start, std::string& end){
    ParseResults r;
    r.num_requests_sent = 10;
    r.pages_visited = 10;
    r.algo_name = "rai button";
    r.shortest_path = {"a", "b", "c"};

    bool bfs(const map<string, vector<string>>& graph, const string& startUrl, const string& endUrl) {
        queue<string> toVisit;
        unordered_set<string> visited;

        toVisit.push(startUrl);
        visited.insert(startUrl);

        while (!toVisit.empty()) {
            string currentUrl = toVisit.front();
            toVisit.pop();

            if (currentUrl == endUrl) {
                cout << "Found URL using BFS: " << currentUrl << endl;
                return true;
            }

            if (graph.find(currentUrl) != graph.end()) {
                for (const string& neighbor : graph.at(currentUrl)) {
                    if (visited.find(neighbor) == visited.end()) {
                        toVisit.push(neighbor);
                        visited.insert(neighbor);
                    }
                }
            }
        }
        cout << "End URL not found using BFS." << endl;
        return false;
    }

    // DFS Function
    bool dfs(const map<string, vector<string>>& graph, const string& startUrl, const string& endUrl) {
        stack<string> toVisit;
        unordered_set<string> visited;

        toVisit.push(startUrl);

        while (!toVisit.empty()) {
            string currentUrl = toVisit.top();
            toVisit.pop();

            if (visited.find(currentUrl) != visited.end()) {
                continue;
            }

            visited.insert(currentUrl);

            if (currentUrl == endUrl) {
                cout << "Found URL using DFS: " << currentUrl << endl;
                return true;
            }

            if (graph.find(currentUrl) != graph.end()) {
                for (const string& neighbor : graph.at(currentUrl)) {
                    if (visited.find(neighbor) == visited.end()) {
                        toVisit.push(neighbor);
                    }
                }
            }
        }
        cout << "End URL not found using DFS." << endl;
        return false;
    }
//test for commit
    return r;
}
