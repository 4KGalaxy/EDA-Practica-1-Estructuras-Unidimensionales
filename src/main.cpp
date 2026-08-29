#include "undoredo.hpp"
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
    
    int i = 1, op;
    std::string files[64];

    std::cout << "--- ARCHIVOS EN /tests ---\n";
    for (const auto& entry : std::filesystem::directory_iterator("tests")) {
        if (entry.path().extension() == ".txt") {
            files[i] = entry.path().string();
            std::cout << " [" << i << "] " << files[i] << "\n";
            i++;
        }
    }

    std::cout << "Selecciona un numero: ";
    std::cin >> op;

    UndoRedoManager manager;
    manager.processEventFile(files[op]);
    manager.printSummary();

    return 0;
}
