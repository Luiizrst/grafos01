#include "Graph.hpp"
#include "Node.hpp"
#include <stack>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <limits>


Graph::Graph(std::ifstream& instance)
    : _number_of_nodes(0), _number_of_edges(0), _directed(false),
      _weighted_edges(false), _weighted_nodes(false), _first(nullptr), _last(nullptr)
{
    std::string line;
    size_t node_id_1, node_id_2;
    float weight;

    // Lê a primeira linha para obter o número de vértices
    if (std::getline(instance, line)) {
        std::istringstream iss(line);
        size_t num_vertices;
        if (iss >> num_vertices) {
            // Cria os vértices
            for (size_t i = 1; i <= num_vertices; ++i) {
                add_node(i);  // Adiciona nós com ID de 1 até num_vertices
                std::cout << "Nó adicionado: " << i << " com peso: 0\n";
            }
        } else {
            std::cerr << "Número de vértices inválido na primeira linha do arquivo.\n";
            return;
        }
    }

    // Processa as arestas
    while (std::getline(instance, line)) {
        std::istringstream iss(line);

        // Verifica se a linha tem o formato esperado
        if (!(iss >> node_id_1 >> node_id_2 >> weight)) {
            std::cerr << "Formato inválido na linha: " << line << std::endl;
            continue; // Pular para a próxima linha
        }

        // Mensagem de depuração para verificar a leitura dos valores
        std::cout << "Leitura bruta: " << node_id_1 << " " << node_id_2 << " " << weight << "\n";

        // Adiciona aresta
        add_edge(node_id_1, node_id_2, weight);

        // Mensagens de depuração
        std::cout << "Aresta adicionada: " << node_id_1 << " -> " << node_id_2 << " com peso: " << weight << "\n";
    }
}


Graph::Graph()
    : _number_of_nodes(0), _number_of_edges(0), _directed(false),
      _weighted_edges(false), _weighted_nodes(false), _first(nullptr), _last(nullptr)
{
}

Graph::~Graph()
{
    Node* current = _first;
    while (current) {
        Node* next = current->_next_node;
        Edge* edge = current->_first_edge;
        while (edge) {
            Edge* next_edge = edge->_next_edge;
            delete edge;
            edge = next_edge;
        }
        delete current;
        current = next;
    }
}

void Graph::remove_node(size_t node_id)
{
    Node* node_to_remove = find_node(node_id);
    if (!node_to_remove) return;

    Edge* edge = node_to_remove->_first_edge;
    while (edge) {
        Edge* next_edge = edge->_next_edge;
        if (!_directed) {
            Node* target_node = find_node(edge->_target_id);
            if (target_node) {
                Edge* target_edge = target_node->_first_edge;
                Edge* prev_edge = nullptr;
                while (target_edge) {
                    if (target_edge->_target_id == node_id) {
                        if (prev_edge) prev_edge->_next_edge = target_edge->_next_edge;
                        else target_node->_first_edge = target_edge->_next_edge;
                        delete target_edge;
                        break;
                    }
                    prev_edge = target_edge;
                    target_edge = target_edge->_next_edge;
                }
            }
        }
        delete edge;
        edge = next_edge;
    }

    if (node_to_remove == _first) _first = node_to_remove->_next_node;
    if (node_to_remove == _last) _last = node_to_remove->_previous_node;
    if (node_to_remove->_previous_node) node_to_remove->_previous_node->_next_node = node_to_remove->_next_node;
    if (node_to_remove->_next_node) node_to_remove->_next_node->_previous_node = node_to_remove->_previous_node;

    delete node_to_remove;
    --_number_of_nodes;
}

void Graph::remove_edge(size_t node_id_1, size_t node_id_2)
{
    Node* node1 = find_node(node_id_1);
    Node* node2 = find_node(node_id_2);
    if (!node1 || !node2) return;

    Edge* edge = node1->_first_edge;
    Edge* prev_edge = nullptr;
    while (edge) {
        if (edge->_target_id == node_id_2) {
            if (prev_edge) prev_edge->_next_edge = edge->_next_edge;
            else node1->_first_edge = edge->_next_edge;
            delete edge;
            break;
        }
        prev_edge = edge;
        edge = edge->_next_edge;
    }

    if (!_directed) {
        edge = node2->_first_edge;
        prev_edge = nullptr;
        while (edge) {
            if (edge->_target_id == node_id_1) {
                if (prev_edge) prev_edge->_next_edge = edge->_next_edge;
                else node2->_first_edge = edge->_next_edge;
                delete edge;
                break;
            }
            prev_edge = edge;
            edge = edge->_next_edge;
        }
    }
    --_number_of_edges;
}

void Graph::add_node(size_t node_id, float weight)
{
    // Verifica se o nó já existe
    if (find_node(node_id)) {
        std::cout << "Nó " << node_id << " já existe. Não adicionado.\n"; // Depuração
        return;
    }

    // Cria um novo nó
    Node* new_node = new Node;
    new_node->_id = node_id;
    new_node->_weight = weight;
    new_node->_number_of_edges = 0;
    new_node->_first_edge = nullptr;
    new_node->_next_node = nullptr;
    new_node->_previous_node = _last;

    if (_last) _last->_next_node = new_node;
    else _first = new_node;

    _last = new_node;
    ++_number_of_nodes;

    // Mensagem de depuração
    //std::cout << "Nó adicionado: " << node_id << " com peso: " << weight << "\n";
}


void Graph::add_edge(size_t node_id_1, size_t node_id_2, float weight)
{
    Node* node1 = find_node(node_id_1);
    Node* node2 = find_node(node_id_2);

    if (!node1 || !node2) {
        std::cout << "Não é possível adicionar aresta. Um ou ambos os nós não existem.\n"; // Depuração
        return;
    }

    // Adiciona aresta de node_id_1 para node_id_2
    Edge* new_edge = new Edge;
    new_edge->_target_id = node_id_2;
    new_edge->_weight = weight;
    new_edge->_next_edge = node1->_first_edge;
    node1->_first_edge = new_edge;
    ++node1->_number_of_edges;

    // Se o grafo não for direcionado, adicione a aresta na direção oposta
    if (!_directed) {
        new_edge = new Edge;
        new_edge->_target_id = node_id_1;
        new_edge->_weight = weight;
        new_edge->_next_edge = node2->_first_edge;
        node2->_first_edge = new_edge;
        ++node2->_number_of_edges;
    }
    ++_number_of_edges;

    // Mensagem de depuração
    //std::cout << "Aresta adicionada: " << node_id_1 << " -> " << node_id_2 << " com peso: " << weight << "\n";
}


void Graph::print_graph(std::ofstream& output_file)
{
    output_file << "digraph G {\n";
    Node* current = _first;
    while (current) {
        Edge* edge = current->_first_edge;
        while (edge) {
            output_file << "  " << current->_id << " -> " << edge->_target_id;
            if (_weighted_edges) {
                output_file << " [label=\"" << edge->_weight << "\"]";
            }
            output_file << ";\n";
            edge = edge->_next_edge;
        }
        current = current->_next_node;
    }
    output_file << "}\n";
}

void Graph::print_graph()
{
    std::ofstream output_file("graph_output.dot");
    print_graph(output_file);
    output_file.close();
    std::cout << "Graph data has been written to graph_output.dot\n";
}

int Graph::connected(size_t node_id_1, size_t node_id_2)
{
    Node* node1 = find_node(node_id_1);
    Node* node2 = find_node(node_id_2);
    if (!node1 || !node2) return 0;

    Edge* edge = node1->_first_edge;
    while (edge) {
        if (edge->_target_id == node_id_2) {
            return 1; // Conectado
        }
        edge = edge->_next_edge;
    }
    return 0; // Não conectado
}

Node* Graph::find_node(size_t node_id)
{
    Node* current = _first;
    while (current) {
        if (current->_id == node_id) {
            return current;
        }
        current = current->_next_node;
    }
    return nullptr;
}

std::unordered_set<int> Graph::transitive_closure_direct(size_t vertex_id)
{
    std::unordered_set<int> direct_neighbors;
    Node* start_node = find_node(vertex_id);
    if (!start_node) return direct_neighbors;

    // Adiciona apenas os vértices diretamente conectados
    Edge* edge = start_node->_first_edge;
    while (edge) {
        direct_neighbors.insert(edge->_target_id);
        edge = edge->_next_edge;
    }

    return direct_neighbors;
}


std::unordered_set<int> Graph::transitive_closure_indirect(size_t vertex_id)
{
    std::unordered_set<int> visited;
    Node* start_node = find_node(vertex_id);
    if (!start_node) return visited;

    // Inicia a DFS a partir do nó inicial
    dfs_indirect(start_node, visited);

    return visited;
}

void Graph::dfs_direct(Node* node, std::unordered_set<int>& visited)
{
    if (visited.find(node->_id) != visited.end()) return;

    visited.insert(node->_id);
    Edge* edge = node->_first_edge;
    while (edge) {
        Node* neighbor = find_node(edge->_target_id);
        if (neighbor) dfs_direct(neighbor, visited);
        edge = edge->_next_edge;
    }
}

void Graph::dfs_indirect(Node* node, std::unordered_set<int>& visited)
{
    if (visited.find(node->_id) != visited.end()) return;

    visited.insert(node->_id);
    Edge* edge = node->_first_edge;
    while (edge) {
        Node* neighbor = find_node(edge->_target_id);
        if (neighbor) dfs_indirect(neighbor, visited);
        edge = edge->_next_edge;
    }
}

std::vector<size_t> Graph::dijkstra(size_t start_id, size_t end_id) {
    std::unordered_map<size_t, float> distances;
    std::unordered_map<size_t, size_t> predecessors;
    for (Node* node = _first; node != nullptr; node = node->_next_node) {
        distances[node->_id] = std::numeric_limits<float>::infinity();
        predecessors[node->_id] = -1; // Indica que não há predecessor
    }
    distances[start_id] = 0;

    auto compare = [&distances](size_t left, size_t right) {
        return distances[left] > distances[right];
    };
    std::priority_queue<size_t, std::vector<size_t>, decltype(compare)> pq(compare);
    pq.push(start_id);

    while (!pq.empty()) {
        size_t current = pq.top();
        pq.pop();

        Node* current_node = find_node(current);
        if (!current_node) continue;

        for (Edge* edge = current_node->_first_edge; edge != nullptr; edge = edge->_next_edge) {
            size_t neighbor_id = edge->_target_id;
            float weight = edge->_weight;
            float new_dist = distances[current] + weight;

            if (new_dist < distances[neighbor_id]) {
                distances[neighbor_id] = new_dist;
                predecessors[neighbor_id] = current;
                pq.push(neighbor_id);
            }
        }
    }

    std::vector<size_t> path;
    for (size_t at = end_id; at != -1; at = predecessors[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());

    if (path.front() != start_id) {
        return {}; // Caminho não encontrado
    }

    return path;
}

std::vector<size_t> Graph::floyd_warshall(size_t start_id, size_t end_id) {
    std::unordered_map<size_t, std::unordered_map<size_t, float>> dist;
    std::unordered_map<size_t, std::unordered_map<size_t, size_t>> next;

    // Inicializa as distâncias e predecessores
    for (Node* u = _first; u != nullptr; u = u->_next_node) {
        size_t u_id = u->_id;
        dist[u_id][u_id] = 0;  // Distância de um nó para ele mesmo é 0
        next[u_id][u_id] = u_id;
        
        for (Edge* e = u->_first_edge; e != nullptr; e = e->_next_edge) {
            dist[u_id][e->_target_id] = e->_weight;
            next[u_id][e->_target_id] = e->_target_id;
        }
    }

    // Algoritmo de Floyd-Warshall
    for (Node* k = _first; k != nullptr; k = k->_next_node) {
        size_t k_id = k->_id;
        for (Node* i = _first; i != nullptr; i = i->_next_node) {
            size_t i_id = i->_id;
            for (Node* j = _first; j != nullptr; j = j->_next_node) {
                size_t j_id = j->_id;
                if (dist[i_id].count(k_id) && dist[k_id].count(j_id)) {
                    float new_dist = dist[i_id][k_id] + dist[k_id][j_id];
                    auto it = dist[i_id].find(j_id);
                    if (it == dist[i_id].end() || new_dist < it->second) {
                        dist[i_id][j_id] = new_dist;
                        next[i_id][j_id] = next[i_id][k_id];
                    }
                }
            }
        }
    }

    // Reconstrói o caminho
    std::vector<size_t> path;
    size_t u = start_id;
    path.push_back(u);

    while (u != end_id) {
        if (next[u].count(end_id) == 0) {
            return {};  // Caminho não encontrado
        }
        u = next[u][end_id];
        path.push_back(u);
    }

    return path;
}

std::vector<Edge> Graph::prim_mst(size_t start_id) {
    std::vector<Edge> mst_edges;
    std::unordered_set<size_t> in_mst;
    auto compare = [](Edge* e1, Edge* e2) { return e1->_weight > e2->_weight; };
    std::priority_queue<Edge*, std::vector<Edge*>, decltype(compare)> pq(compare);

    Node* start_node = find_node(start_id);
    if (!start_node) return mst_edges;

    // Adiciona as arestas do nó inicial à fila de prioridades
    Edge* edge = start_node->_first_edge;
    while (edge) {
        pq.push(edge);
        edge = edge->_next_edge;
    }
    in_mst.insert(start_node->_id);

    while (!pq.empty()) {
        Edge* min_edge = pq.top();
        pq.pop();

        if (in_mst.find(min_edge->_target_id) == in_mst.end()) {
            mst_edges.push_back(*min_edge);

            Node* next_node = find_node(min_edge->_target_id);
            if (next_node) {
                in_mst.insert(next_node->_id);
                edge = next_node->_first_edge;
                while (edge) {
                    if (in_mst.find(edge->_target_id) == in_mst.end()) {
                        pq.push(edge);
                    }
                    edge = edge->_next_edge;
                }
            }
        }
    }

    return mst_edges;
}

bool Graph::is_weighted_edges() const {
    return _weighted_edges;
}



// Implementação do método kruskal
std::vector<Edge> Graph::kruskal_mst() {
    std::vector<Edge> mst_edges;
    std::vector<Edge> edges;
    std::unordered_map<size_t, Node*> node_map;

    // Coleta todas as arestas do grafo
    for (Node* node = _first; node != nullptr; node = node->_next_node) {
        Edge* edge = node->_first_edge;
        while (edge) {
            if (node->_id < edge->_target_id || _directed) {
                edges.push_back(*edge);
            }
            edge = edge->_next_edge;
        }
    }

    // Ordena arestas por peso
    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a._weight < b._weight;
    });

    // Inicializa o Disjoint Set
    DisjointSet ds(_number_of_nodes);

    // Adiciona arestas ao MST usando o algoritmo de Kruskal
    for (const Edge& edge : edges) {
        size_t root1 = ds.find(edge._source_id);
        size_t root2 = ds.find(edge._target_id);

        if (root1 != root2) {
            mst_edges.push_back(edge);
            ds.union_sets(root1, root2);
        }
    }

    return mst_edges;
}

//Raio, Diâmetro, Centro e Periferia do grafo
std::tuple<float, float, std::unordered_set<size_t>, std::unordered_set<size_t>> Graph::calculate_radius_diameter_center_periphery() {
    // Calcular distâncias mínimas entre todos os pares de vértices usando Floyd-Warshall
    std::unordered_map<size_t, std::unordered_map<size_t, float>> dist;
    std::unordered_map<size_t, std::unordered_map<size_t, size_t>> next;

    for (Node* u = _first; u != nullptr; u = u->_next_node) {
        size_t u_id = u->_id;
        dist[u_id][u_id] = 0;
        next[u_id][u_id] = u_id;
        
        for (Edge* e = u->_first_edge; e != nullptr; e = e->_next_edge) {
            dist[u_id][e->_target_id] = e->_weight;
            next[u_id][e->_target_id] = e->_target_id;
        }
    }

    for (Node* k = _first; k != nullptr; k = k->_next_node) {
        size_t k_id = k->_id;
        for (Node* i = _first; i != nullptr; i = i->_next_node) {
            size_t i_id = i->_id;
            for (Node* j = _first; j != nullptr; j = j->_next_node) {
                size_t j_id = j->_id;
                if (dist[i_id].count(k_id) && dist[k_id].count(j_id)) {
                    float new_dist = dist[i_id][k_id] + dist[k_id][j_id];
                    if (dist[i_id].find(j_id) == dist[i_id].end() || new_dist < dist[i_id][j_id]) {
                        dist[i_id][j_id] = new_dist;
                        next[i_id][j_id] = next[i_id][k_id];
                    }
                }
            }
        }
    }

    // Calcular o raio, diâmetro, centro e periferia
    float radius = std::numeric_limits<float>::infinity();
    float diameter = 0;
    std::unordered_map<size_t, float> eccentricities;
    std::unordered_set<size_t> centers;
    std::unordered_set<size_t> peripheries;

    for (Node* u = _first; u != nullptr; u = u->_next_node) {
        size_t u_id = u->_id;
        float eccentricity = 0;
        for (Node* v = _first; v != nullptr; v = v->_next_node) {
            size_t v_id = v->_id;
            if (dist[u_id].count(v_id)) {
                eccentricity = std::max(eccentricity, dist[u_id][v_id]);
            }
        }
        eccentricities[u_id] = eccentricity;
        radius = std::min(radius, eccentricity);
        diameter = std::max(diameter, eccentricity);
    }

    for (const auto& [node_id, ecc] : eccentricities) {
        if (ecc == radius) {
            centers.insert(node_id);
        }
        if (ecc == diameter) {
            peripheries.insert(node_id);
        }
    }

    // Retornar os resultados
    return std::make_tuple(radius, diameter, centers, peripheries);
}


void Graph::set_directed(bool directed) { _directed = directed; }
void Graph::set_weighted_edges(bool weighted_edges) { _weighted_edges = weighted_edges; }
void Graph::set_weighted_nodes(bool weighted_nodes) { _weighted_nodes = weighted_nodes; }