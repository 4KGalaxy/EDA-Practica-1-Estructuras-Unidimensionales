#include "undoredo.hpp"
#include "stack_array.hpp"
#include "stack_list.hpp"
#include <iostream>
#include <filesystem>
#include "firewall_runner.hpp"

int main() {
    std::cout << "1) Problema 1 - undo/redo\n2) Problema 2 -Firewall\nSelecciona: ";
    int problema;
    std::cin >> problema;

    if(problema == 2){
        correrProblema2();
        return 0;
    }
    
    std::cout << "\n--- CONFIGURACION DE LA PILA ---\n";
    std::cout << "1) Usar Arreglo Dinamico (StackArray)\n";
    std::cout << "2) Usar Lista Enlazada (StackList)\n";
    std::cout << "Selecciona la memoria a usar: ";
    int tipo_pila;
    std::cin >> tipo_pila;

    int i = 1, op;
    std::string files[64];

    std::cout << "\n--- ARCHIVOS EN /tests ---\n";
    for (const auto& entry : std::filesystem::directory_iterator("tests")) {
        if (entry.path().extension() == ".txt") {
            files[i] = entry.path().string();
            std::cout << " [" << i << "] " << files[i] << "\n";
            i++;
        }
    }

    std::cout << "Selecciona un numero: ";
    std::cin >> op;

    if (tipo_pila == 2) {
        UndoRedoManager<StackList<Action>> manager;
        manager.processEventFile(files[op]);
        manager.printSummary();
    } else {
        UndoRedoManager<StackArray<Action>> manager;
        manager.processEventFile(files[op]);
        manager.printSummary();
    }

    return 0;
}