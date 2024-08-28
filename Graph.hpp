// No arquivo graph.hpp

#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Node.hpp"
#include "defines.hpp"
#include "Conjunto.hpp"
#include <unordered_set>
#include <vector>
#include <fstream>
#include <unordered_map>  // Adicionado para usar unordered_map
#include <queue>          // Adicionado para usar priority_queue
#include <limits>         // Adicionado para usar limites numéricos

class Graph
{
public:
    Graph(std::ifstream& instance, bool directed, bool weighted_edges, bool weighted_nodes);    
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
    std::unordered_set<int> transitivo_direto(size_t vertex_id);
    std::unordered_set<int> transitivo_indireto(size_t vertex_id);
    void dfs_direct(Node* node, std::unordered_set<int>& visited);

    Node* find_node(size_t node_id);
    
    // Adicionando a função dijkstra e floyd
    std::vector<size_t> dijkstra(size_t start_id, size_t end_id);
    std::vector<size_t> floyd_warshall(size_t start_id, size_t end_id);

    //prim
    bool is_weighted_edges() const; // Declaração correta do método
    std::vector<Edge> prim_mst(size_t start_id); // Declaração do método prim_mst

    //Kruskal
    std::vector<Edge> kruskal_mst(std::unordered_set<size_t> subset);

    //Raio, Diâmetro, Centro e Periferia do grafo
    std::tuple<float, float, std::unordered_set<size_t>, std::unordered_set<size_t>> calculate_radius_diameter_center_periphery();

    // Conjunto de vértices de articulação
    std::unordered_set<size_t> find_articulation_points();

    void saveGraphAdjacencyList(const std::string& filename) const;

private:
    size_t _number_of_nodes;
    size_t _number_of_edges;
    bool   _directed;
    bool   _weighted_edges;
    bool   _weighted_nodes;
    Node  *_first;
    Node  *_last;

    //bool _weighted_edges;  // Supondo que esta variável indica se as arestas são ponderadas

    void dfs_indirect(Node* node, std::unordered_set<int>& visited);

    // Conjunto de vértices de articulação
    void dfs_articulation(
        size_t node_id,
        size_t& time,
        std::unordered_map<size_t, size_t>& discovery_time,
        std::unordered_map<size_t, size_t>& low_time,
        std::unordered_map<size_t, size_t>& parent,
        std::unordered_set<size_t>& articulation_points,
        std::unordered_set<size_t>& visited
    );


    // Métodos privados adicionados
    void print_nodes();   // Adicione esta linha se desejar implementar a função
    void print_edges();   // Adicione esta linha se desejar implementar a função
};



#endif  // GRAPH_HPP