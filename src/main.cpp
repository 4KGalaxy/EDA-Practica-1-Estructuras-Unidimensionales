#include "undoredo.hpp"
#include <iostream>
#include <filesystem>

int main() {
    int i = 1, op;
    std::string files[10];

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
