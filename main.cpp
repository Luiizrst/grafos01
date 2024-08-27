#include "Graph.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <vector>

void showMenu() {
    std::cout << "Escolha uma opcao:\n";
    std::cout << "1. Fecho transitivo direto de um vertice\n";
    std::cout << "2. Fecho transitivo indireto de um vertice\n";
    std::cout << "3. Caminho minimo entre dois vertices (Dijkstra)\n";
    std::cout << "4. Caminho minimo entre dois vertices (Floyd)\n";
    std::cout << "5. Arvore Geradora Minima (Prim)\n";
    std::cout << "6. Arvore Geradora Minima (Kruskal)\n";
    std::cout << "7. Caminhamento em profundidade\n";
    std::cout << "8. Raio, Diametro, Centro e Periferia do grafo\n";
    std::cout << "9. Conjunto de vertices de articulacao\n";
    std::cout << "0. Sair\n";
    std::cout << "Escolha: ";
}

void handleOutput(bool to_file, std::ofstream& output_file, const std::string& content) {
    if (to_file) {
        output_file << content;
    } else {
        std::cout << content;
    }
}

std::string formatEdgeLabel(float weight) {
    // Converte o peso da aresta para um inteiro e retorna como string
    int int_weight = static_cast<int>(weight);
    return std::to_string(int_weight);
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        std::cerr << "Uso incorreto! Uso correto: " << argv[0] << " <input_file> <output_file> <Op_Direc> <Op_PesoAresta> <Op_PesoNos>\n";
        return 1;
    }

    const char* input_file_name = argv[1];
    const char* output_file_name = argv[2];
    bool directed = std::stoi(argv[3]) == 1;
    bool weighted_edges = std::stoi(argv[4]) == 1;
    bool weighted_nodes = std::stoi(argv[5]) == 1;

    std::ifstream input_file(input_file_name);
    if (!input_file) {
        std::cerr << "Erro ao abrir o arquivo de entrada: " << input_file_name << "\n";
        return 1;
    }

    Graph graph(input_file);
    graph.set_directed(directed);
    graph.set_weighted_edges(weighted_edges);
    graph.set_weighted_nodes(weighted_nodes);
    input_file.close();

    std::ofstream output_file(output_file_name);
    if (!output_file) {
        std::cerr << "Erro ao abrir o arquivo de saida: " << output_file_name << "\n";
        return 1;
    }

    int option;
    do {
        showMenu();
        std::cin >> option;
        bool save_to_file = false;
        std::string result;
        
        switch(option) {
            case 1: {
                // Fecho transitivo direto de um vértice
                size_t node_id;
                std::cout << "Insira o ID do vertice: ";
                std::cin >> node_id;
                std::unordered_set<int> visited = graph.transitive_closure_direct(node_id);
                result = "Fecho transitivo direto para o vértice " + std::to_string(node_id) + ":\n";
                for (int id : visited) {
                    result += std::to_string(id) + "\n";
                }
                break;
            }
            case 2: {
                // Fecho transitivo indireto de um vértice
                size_t node_id;
                std::cout << "Insira o ID do vertice: ";
                std::cin >> node_id;
                std::unordered_set<int> visited = graph.transitive_closure_indirect(node_id);
                result = "Fecho transitivo indireto para o vértice " + std::to_string(node_id) + ":\n";
                for (int id : visited) {
                    result += std::to_string(id) + "\n";
                }
                break;
            }
            case 3: {
                // Caminho mínimo entre dois vértices usando Dijkstra
                size_t start_id, end_id;
                std::cout << "Insira o ID do vertice de origem: ";
                std::cin >> start_id;
                std::cout << "Insira o ID do vertice de destino: ";
                std::cin >> end_id;

                std::vector<size_t> path = graph.dijkstra(start_id, end_id);
                if (path.empty()) {
                    result = "Não há caminho entre " + std::to_string(start_id) + " e " + std::to_string(end_id) + ".\n";
                } else {
                    result = "Caminho mínimo de " + std::to_string(start_id) + " até " + std::to_string(end_id) + ":\n";
                    for (size_t id : path) {
                        result += std::to_string(id) + " ";
                    }
                    result += "\n";
                }
                break;
            }
            case 4: {
                // Caminho mínimo entre dois vértices usando Floyd-Warshall
                size_t start_id, end_id;
                std::cout << "Insira o ID do vertice de origem: ";
                std::cin >> start_id;
                std::cout << "Insira o ID do vertice de destino: ";
                std::cin >> end_id;

                std::vector<size_t> path = graph.floyd_warshall(start_id, end_id);
                if (path.empty()) {
                    result = "Não há caminho entre " + std::to_string(start_id) + " e " + std::to_string(end_id) + ".\n";
                } else {
                    result = "Caminho mínimo de " + std::to_string(start_id) + " até " + std::to_string(end_id) + ":\n";
                    for (size_t id : path) {
                        result += std::to_string(id) + " ";
                    }
                    result += "\n";
                }
                break;
            }
            case 5: {
                // Árvore Geradora Mínima usando Prim
                size_t start_id;
                std::cout << "Insira o ID do vertice inicial: ";
                std::cin >> start_id;

                Node* start_node = graph.find_node(start_id);
                if (!start_node) {
                    result = "Vértice não encontrado!\n";
                    break;
                }

                // Calcula a AGM usando Prim
                std::vector<Edge> mst_edges = graph.prim_mst(start_id);

                // Gera a AGM no formato dot
                result = "\nGrafo AGM PRIM {\n";
                for (const Edge& edge : mst_edges) {
                    result += "  " + std::to_string(start_id) + " -- " + std::to_string(edge._target_id);
                    if (graph.is_weighted_edges()) {
                        result += " [label=\"" + formatEdgeLabel(edge._weight) + "\"]";
                    }
                    result += ";\n";
                }
                result += "}\n\n";

                break;
            }
            case 6: {
                // Árvore Geradora Mínima usando Kruskal
                // Implementar a função que calcula a árvore geradora mínima usando Kruskal
                result = "Função Kruskal ainda não implementada.\n";
                break;
            }
            case 7: {
                // Caminhamento em profundidade
                size_t start_id;
                std::cout << "Insira o ID do vertice inicial: ";
                std::cin >> start_id;

                Node* start_node = graph.find_node(start_id);
                if (!start_node) {
                    result = "Vértice não encontrado!\n";
                    break;
                }

                std::unordered_set<int> visited;
                graph.dfs_direct(start_node, visited);

                // Adiciona uma mensagem de teste ao arquivo de saída
                result = "Resultado do DFS começando no vértice " + std::to_string(start_id) + ":\n";
                result += "subgraph cluster_dfs {\n";
                result += "  label=\"DFS Caminhamento\";\n";
                for (int id : visited) {
                    result += "  " + std::to_string(id) + ";\n";
                }
                result += "}\n";
                break;
            }
            case 8: {
                // Raio, Diâmetro, Centro e Periferia do grafo
                // Implementar a função que calcula raio, diâmetro, centro e periferia
                result = "Função para calcular raio, diâmetro, centro e periferia ainda não implementada.\n";
                break;
            }
            case 9: {
                // Conjunto de vértices de articulação
                // Implementar a função que encontra o conjunto de vértices de articulação
                result = "Função para encontrar vértices de articulação ainda não implementada.\n";
                break;
            }
            case 0:
                result = "Saindo...\n";
                break;
            default:
                result = "Opcao invalida!\n";
                break;
        }

        if (option != 0) {
            char save_choice;
            std::cout << "Deseja salvar a saída em arquivo (s/n)? ";
            std::cin >> save_choice;
            bool save_to_file = (save_choice == 's' || save_choice == 'S');
            handleOutput(save_to_file, output_file, result);
        }

    } while (option != 0);

    output_file.close();
    return 0;
}
