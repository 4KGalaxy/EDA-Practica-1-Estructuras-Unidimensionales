#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <string>

int cantidad_eventos = 1000;

int main() {
    std::string filename = "tests/eventos_masivos_" + std::to_string(cantidad_eventos) + ".txt";
    std::ofstream out(filename);
    
    if (!out.is_open()) return 1;

    std::mt19937 gen(42);
    std::uniform_int_distribution<> actionDist(1, 100);
    
    std::vector<std::string> palabras = {"EAFIT", "Estructuras", "Pila", "C++", "Nodo", "Datos", "Proyecto"};
    std::uniform_int_distribution<> wordDist(0, palabras.size() - 1);

    int current_length = 0; 
    std::vector<int> undo_stack;
    std::vector<int> redo_stack;

    for(int i = 0; i < cantidad_eventos; ++i) {
        int prob = actionDist(gen);
        
        if (prob <= 40) { 
            int pos = (current_length == 0) ? 0 : std::uniform_int_distribution<>(0, current_length)(gen);
            std::string palabra = palabras[wordDist(gen)];
            
            out << "EDIT INSERT " << pos << " \"" << palabra << "\"\n";
            
            undo_stack.push_back(current_length);
            redo_stack.clear();
            current_length += palabra.length();
        } 
        else if (prob <= 70) { 
            if (current_length > 0) {
                int pos = std::uniform_int_distribution<>(0, current_length - 1)(gen);
                int max_len = current_length - pos;
                int len = std::uniform_int_distribution<>(1, max_len > 20 ? 20 : max_len)(gen);
                
                out << "EDIT DELETE " << pos << " " << len << "\n";
                
                undo_stack.push_back(current_length);
                redo_stack.clear();
                current_length -= len;
            } else {
                std::string palabra = palabras[wordDist(gen)];
                out << "EDIT INSERT 0 \"" << palabra << "\"\n";
                
                undo_stack.push_back(current_length);
                redo_stack.clear();
                current_length += palabra.length();
            }
        } 
        else if (prob <= 85) { 
            out << "UNDO\n";
            if (!undo_stack.empty()) {
                redo_stack.push_back(current_length);
                current_length = undo_stack.back();
                undo_stack.pop_back();
            }
        } 
        else { 
            out << "REDO\n";
            if (!redo_stack.empty()) {
                undo_stack.push_back(current_length);
                current_length = redo_stack.back();
                redo_stack.pop_back();
            }
        }
    }

    out.close();
    std::cout << "[EXITO] Se generaron " << cantidad_eventos << " eventos sincronizados en: " << filename << "\n";
    return 0;
}