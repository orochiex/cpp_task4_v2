#pragma once
#include <vector>
#include <unordered_map>
#include <list>
#include <string>
#include <fstream>
#include <stdexcept>
#include <queue>

class Graph {
public:
    Graph(const std::string& filename);
    void renumber(int start_vertex);
    const std::unordered_map<int, std::list<int>>& getAdjacencyList() const;

private:
    std::unordered_map<int, std::list<int>> adjacency_list;
    void loadFromFile(const std::string& filename);
};
