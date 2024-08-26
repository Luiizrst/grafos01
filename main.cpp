#include "Graph.hpp"
#include <iostream>
#include <fstream>

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
        switch(option) {
            case 1: {
                // Fecho transitivo direto de um vértice
                size_t node_id;
                std::cout << "Insira o ID do vertice: ";
                std::cin >> node_id;
                std::unordered_set<int> visited = graph.transitive_closure_direct(node_id);
                output_file << "Fecho transitivo direto para o vértice " << node_id << ":\n";
                for (int id : visited) {
                    output_file << id << "\n";
                }
                break;
            }
            case 2: {
                // Fecho transitivo indireto de um vértice
                size_t node_id;
                std::cout << "Insira o ID do vertice: ";
                std::cin >> node_id;
                std::unordered_set<int> visited = graph.transitive_closure_indirect(node_id);
                output_file << "Fecho transitivo indireto para o vértice " << node_id << ":\n";
                for (int id : visited) {
                    output_file << id << "\n";
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
                // Implementar a função que calcula o caminho mínimo usando Dijkstra
                break;
            }
            case 4: {
                // Caminho mínimo entre dois vértices usando Floyd
                size_t start_id, end_id;
                std::cout << "Insira o ID do vertice de origem: ";
                std::cin >> start_id;
                std::cout << "Insira o ID do vertice de destino: ";
                std::cin >> end_id;
                // Implementar a função que calcula o caminho mínimo usando Floyd
                break;
            }
            case 5: {
                // Árvore Geradora Mínima usando Prim
                // Implementar a função que calcula a árvore geradora mínima usando Prim
                break;
            }
            case 6: {
                // Árvore Geradora Mínima usando Kruskal
                // Implementar a função que calcula a árvore geradora mínima usando Kruskal
                break;
            }
            case 7: {
            // Caminhamento em profundidade
            size_t start_id;
            std::cout << "Insira o ID do vertice inicial: ";
            std::cin >> start_id;

            Node* start_node = graph.find_node(start_id);
            if (!start_node) {
                std::cout << "Vértice não encontrado!\n";
                break;
            }

            std::unordered_set<int> visited;
            graph.dfs_direct(start_node, visited);

            // Adiciona uma mensagem de teste ao arquivo de saída
            output_file << "Resultado do DFS começando no vértice " << start_id << ":\n";
            output_file << "subgraph cluster_dfs {\n";
            output_file << "  label=\"DFS Caminhamento\";\n";
            for (int id : visited) {
                output_file << "  " << id << ";\n";
            }
            output_file << "}\n";
            break;
            }

            case 8: {
                // Raio, Diâmetro, Centro e Periferia do grafo
                // Implementar a função que calcula raio, diâmetro, centro e periferia
                break;
            }
            case 9: {
                // Conjunto de vértices de articulação
                // Implementar a função que encontra o conjunto de vértices de articulação
                break;
            }
            case 0:
                std::cout << "Saindo...\n";
                break;
            default:
                std::cout << "Opcao invalida!\n";
                break;
        }
    } while (option != 0);

    output_file.close();
    return 0;
}
