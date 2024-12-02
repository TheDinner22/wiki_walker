#include "search_algorithms.hpp"
#include "the_graph/the_graph.hpp"
#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <map>

ParseResults hubert_algo_dijkstra(const std::string& start, const std::string& end, const Graph& g){
    // disgusting check by Joseph I am ashamed that I wrote the following if statement
    if(g.get_graph().count(start) == 0 || g.get_graph().count(end) == 0){
        ParseResults r;
        r.num_requests_sent = 0;
        r.pages_visited = 0;
        r.shortest_path = {};
        r.algo_name = "hubert button dijkstra";
        return r;
    }


    ParseResults r;
    r.num_requests_sent = 0;
    r.algo_name = "hubert button dijkstra";

    int graph_size = g.num_nodes();
    std::unordered_set<int> visited;
    
    std::vector<std::string> shortest_path;

    //first element is distance, the second is the index of string (use assoc to decode)
    std::priority_queue<std::pair<int,int> , std::vector<std::pair<int,int>>, std::greater<std::pair<int,int>> > distance_costs;
    std::vector<int> prev; prev.reserve(graph_size);
    std::vector<int> distances; distances.reserve(graph_size);

    //association maps
    std::map<int, std::string> index_to_link;
    std::map<std::string, int> link_to_index;
    
    //assign initial values to distances, association map, prev
    std::unordered_map<std::string, std::unordered_set<std::string>>::iterator iter;
    int end_index;
    int start_index;
    int index = 0;
    for(auto iter = g.get_graph().begin(); iter != g.get_graph().end(); iter++)
    {
        index_to_link[index] = iter->first;
        link_to_index[iter->first] = index;
        if(iter->first == end)
        {
            end_index = index;
        }

        if(iter->first == start)
        {
            distance_costs.push(std::make_pair(0,index));
            distances[index] = 0;
            start_index = index;
        }
        else
        {
            distances[index] = std::numeric_limits<int>::max();
        }
        prev[index] = -1;
        index++;
    }

    while(!(distance_costs.empty()))
    {
        int curr_ind = distance_costs.top().second;
        int distance = distance_costs.top().first;
        std::string curr_string = index_to_link[curr_ind];
        if(visited.count(curr_ind) == 0)
        {
            visited.insert(curr_ind);
            if(curr_ind == end_index)
            {
                break;
            }

            // each time we go through this for loop, we must "send a get request"
            // (IE: query the graph for neighbors)
            r.num_requests_sent++;
            for(auto neighbor: g.getAdjacent(curr_string))
            {
                //if distance stored for vertex is greater than current distance
                if(distances[link_to_index[neighbor]] >= distance + 1)
                {
                    distance_costs.push(std::make_pair(distance+1,link_to_index[neighbor]));
                    prev[link_to_index[neighbor]] = curr_ind;
                    distances[link_to_index[neighbor]] = distance + 1;
                }
            }
        }
        distance_costs.pop();
    }

    int prev_ind = end_index;
    while(prev_ind != -1)
    {
        shortest_path.push_back(index_to_link[prev_ind]);
        prev_ind = prev[prev_ind];
    }

    // shortest_path is backwards
    std::reverse(shortest_path.begin(), shortest_path.end());

    r.pages_visited = visited.size();
    r.shortest_path = shortest_path;

    return r;
}

ParseResults hubert_algo_a_star(const std::string& start, const std::string& end, const Graph& g){
    ParseResults r;
    r.num_requests_sent = 10;
    r.pages_visited = 10;
    r.algo_name = "hubert button a star";

    //first element is distance, the second is the index of string (use assoc to decode)
    std::priority_queue<std::pair<int,int> , std::vector<std::pair<int,int>>, std::greater<std::pair<int,int>> > distance_costs;
    
    int graph_size = g.num_nodes();
    std::unordered_set<int> visited;
    std::vector<std::string> shortest_path;
    int prev[graph_size];
    int heur_distances[graph_size];
    int distances[graph_size];

    //association maps
    std::map<int, std::string> index_to_link;
    std::map<std::string, int> link_to_index;

    //assign initial values to distances, association map, and prev 
    std::unordered_map<std::string, std::unordered_set<std::string>>::iterator iter;
    int end_index;
    int start_index;
    int index = 0;
    for(auto iter = g.get_graph().begin(); iter != g.get_graph().end(); iter++)
    {
        index_to_link[index] = iter->first;
        link_to_index[iter->first] = index;
        if(iter->first == end)
        {
            end_index = index;
        }

        if(iter->first == start)
        {
            

            start_index = index;
        }
        else
        {
            distances[index] = std::numeric_limits<int>::max();
        }
        prev[index] = -1;
        index++;
    }

    //BFS to assign exact heuristic to graph (edgewise distance from end node to vertex)
    std::queue<std::string> toVisit;
    std::queue<int> heur_dists;
    std::unordered_set<std::string> visited_bfs;
    toVisit.push(end);
    visited_bfs.emplace(end);
    heur_dists.push(0);
    int heur_dist;
    heur_distances[end_index] = 0;
    while (!toVisit.empty()) 
    {
        std::string currentUrl = toVisit.front();
        toVisit.pop();
        heur_dist = heur_dists.front();
        heur_dists.pop();
        heur_distances[link_to_index[currentUrl]] = heur_dist;

        // this for loop "sends a request" getting a nodes adjacent nodes would require an http request
        // if the graph weren't already cached
        for (std::string neighbor : g.getAdjacent(currentUrl)) 
        {
            if (visited_bfs.count(neighbor) == 0) 
            {
                toVisit.push(neighbor);
                visited_bfs.emplace(neighbor);
                heur_dists.push(heur_dist + 1);
            }
        }
    }
    distances[start_index] = heur_distances[start_index];
    distance_costs.push(std::make_pair((0 + heur_distances[start_index]),start_index));
    while(!(distance_costs.empty()))
    {
        int curr_ind = distance_costs.top().second;
        int distance = distance_costs.top().first;
        std::string curr_string = index_to_link[curr_ind];
        std::vector<std::string>::iterator iter;
        if(visited.count(curr_ind) == 0)
        {
            visited.insert(curr_ind);
            if(curr_ind == end_index)
            {
                break;
            }
            for(iter = g.getAdjacent(curr_string).begin(); iter != g.getAdjacent(curr_string).end(); iter++)
            {
                //if distance stored for vertex is greater than current distance + heur
                int new_dist_cost = distance + 1 + heur_distances[link_to_index[*iter]];
                if(distances[link_to_index[*iter]] >= new_dist_cost)
                {
                    distance_costs.push(std::make_pair(new_dist_cost,link_to_index[*iter]));
                    prev[link_to_index[*iter]] = curr_ind;
                    distances[link_to_index[*iter]] = new_dist_cost;
                }
            }
        }
        distance_costs.pop();
    }

    int prev_ind = end_index;
    while(prev_ind != -1)
    {
        shortest_path.push_back(index_to_link[prev_ind]);
        prev_ind = prev[prev_ind];
    }
    
    std::reverse(shortest_path.begin(), shortest_path.end());
    r.shortest_path = shortest_path;
    return r;
}
