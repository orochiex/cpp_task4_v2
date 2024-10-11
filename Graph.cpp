#include "Graph.h"

Graph::Graph(const std::string& filename) {
    loadFromFile(filename);
}

void Graph::loadFromFile(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        throw std::runtime_error("Unable to open file");
    }
    int vertex;
    while (infile >> vertex) {
        int adjacent_vertex;
        while (infile.peek() != '\n' && infile >> adjacent_vertex) {
            adjacency_list[vertex].push_back(adjacent_vertex);
            adjacency_list[adjacent_vertex].push_back(vertex); 
        }
    }
    infile.close();
}

void Graph::renumber(int start_vertex) {
    std::unordered_map<int, int> vertex_layers;
    std::queue<int> vertex_queue;
    vertex_queue.push(start_vertex);
    vertex_layers[start_vertex] = 0;

    while (!vertex_queue.empty()) {
        int current_vertex = vertex_queue.front();
        vertex_queue.pop();
        int current_layer = vertex_layers[current_vertex];

        for (const auto& neighbor : adjacency_list[current_vertex]) {
            if (vertex_layers.find(neighbor) == vertex_layers.end()) {
                vertex_layers[neighbor] = current_layer + 1;
                vertex_queue.push(neighbor);
            }
        }
    }

    for (auto& vertex_layer : vertex_layers) {
        adjacency_list[vertex_layer.second].push_back(vertex_layer.first);
    }
}

const std::unordered_map<int, std::list<int>>& Graph::getAdjacencyList() const {
    return adjacency_list;
}