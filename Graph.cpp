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
        for (size_t i = 1; i <= num_vertices; ++i) {
            add_node(i);  // Adiciona nós com ID de 1 até num_vertices
            std::cout << "Nó adicionado: " << i << " com peso: 0\n";
        }
    } else {
        std::cerr << "Número de vértices inválido na primeira linha do arquivo.\n";
        return;
    }
}

while (std::getline(instance, line)) {
    std::istringstream iss(line);
    if (!(iss >> node_id_1 >> node_id_2 >> weight)) {
        std::cerr << "Formato inválido na linha: " << line << std::endl;
        continue; // Pular para a próxima linha
    }
    std::cout << "Leitura bruta: " << node_id_1 << " " << node_id_2 << " " << weight << "\n";
    add_edge(node_id_1, node_id_2, weight);
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
        std::cout << "Nó " << node_id << " já existe. Não adicionado.\n";
        return;
    }

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

}

void Graph::add_edge(size_t node_id_1, size_t node_id_2, float weight)
{
    Node* node1 = find_node(node_id_1);
    Node* node2 = find_node(node_id_2);

    if (!node1 || !node2) {
        std::cout << "Não é possível adicionar aresta. Um ou ambos os nós não existem.\n";
        return;
    }

    // Verifique se a aresta já existe
    Edge* existing_edge = node1->_first_edge;
    while (existing_edge) {
        if (existing_edge->_target_id == node_id_2) {
            std::cout << "Aresta entre " << node_id_1 << " e " << node_id_2 << " já existe.\n";
            return;
        }
        existing_edge = existing_edge->_next_edge;
    }

    Edge* new_edge = new Edge;
    new_edge->_target_id = node_id_2;
    new_edge->_weight = weight;
    new_edge->_next_edge = node1->_first_edge;
    node1->_first_edge = new_edge;
    ++node1->_number_of_edges;

    if (!_directed) {
        new_edge = new Edge;
        new_edge->_target_id = node_id_1;
        new_edge->_weight = weight;
        new_edge->_next_edge = node2->_first_edge;
        node2->_first_edge = new_edge;
        ++node2->_number_of_edges;
    }
    ++_number_of_edges;

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
    std::unordered_set<int> visited;
    Node* start_node = find_node(vertex_id);
    if (!start_node) return visited;

    std::stack<Node*> stack;
    stack.push(start_node);
    while (!stack.empty()) {
        Node* current = stack.top();
        stack.pop();
        if (visited.find(current->_id) == visited.end()) {
            visited.insert(current->_id);
            Edge* edge = current->_first_edge;
            while (edge) {
                Node* neighbor = find_node(edge->_target_id);
                if (neighbor && visited.find(neighbor->_id) == visited.end()) {
                    stack.push(neighbor);
                }
                edge = edge->_next_edge;
            }
        }
    }
    return visited;
}

std::unordered_set<int> Graph::transitive_closure_indirect(size_t vertex_id)
{
    std::unordered_set<int> visited;
    Node* start_node = find_node(vertex_id);
    if (!start_node) return visited;

    std::stack<Node*> stack;
    stack.push(start_node);
    while (!stack.empty()) {
        Node* current = stack.top();
        stack.pop();
        if (visited.find(current->_id) == visited.end()) {
            visited.insert(current->_id);
            Edge* edge = current->_first_edge;
            while (edge) {
                Node* neighbor = find_node(edge->_target_id);
                if (neighbor && visited.find(neighbor->_id) == visited.end()) {
                    stack.push(neighbor);
                }
                edge = edge->_next_edge;
            }
        }
    }
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


void Graph::set_directed(bool directed) { _directed = directed; }
void Graph::set_weighted_edges(bool weighted_edges) { _weighted_edges = weighted_edges; }
void Graph::set_weighted_nodes(bool weighted_nodes) { _weighted_nodes = weighted_nodes; }
