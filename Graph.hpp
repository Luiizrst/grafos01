#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Node.hpp"
#include "defines.hpp"
#include <unordered_set>
#include <vector>

class Graph
{
public:
    Graph(std::ifstream& instance);
    Graph();
    ~Graph();

    void remove_node(size_t node_id);
    void remove_edge(size_t node_id_1, size_t node_id_2);
    void add_node(size_t node_id, float weight = 0);
    void add_edge(size_t node_id_1, size_t node_id_2, float weight = 0);
    void print_graph(std::ofstream& output_file);
    void print_graph();
    int connected(size_t node_id_1, size_t node_id_2);

    // Métodos públicos para acessar e modificar variáveis privadas
    void set_directed(bool directed);
    void set_weighted_edges(bool weighted_edges);
    void set_weighted_nodes(bool weighted_nodes);

    // Métodos adicionados
    std::unordered_set<int> transitive_closure_direct(size_t vertex_id);
    std::unordered_set<int> transitive_closure_indirect(size_t vertex_id);
    void dfs_direct(Node* node, std::unordered_set<int>& visited);
    void dfs_indirect(Node* node, std::unordered_set<int>& visited);

    Node* find_node(size_t node_id);


private:
    size_t _number_of_nodes;
    size_t _number_of_edges;
    bool   _directed;
    bool   _weighted_edges;
    bool   _weighted_nodes;
    Node  *_first;
    Node  *_last;
};

#endif  //GRAPH_HPP
