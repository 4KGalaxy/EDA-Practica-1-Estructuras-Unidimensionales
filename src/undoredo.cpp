#include "undoredo.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

UndoRedoManager::UndoRedoManager() : document("") {}

void UndoRedoManager::applyAction(const Action& act) {
    if (act.type == ActionType::INSERT) {
        document.insert(act.pos, act.content);
    } else if (act.type == ActionType::DELETE) {
        document.erase(act.pos, act.content.size());
    } else if (act.type == ActionType::REPLACE) {
        document.replace(act.pos, act.content.size(), act.content);
    }
}

Action UndoRedoManager::invertAction(const Action& act) {
    if (act.type == ActionType::INSERT) {
        return {ActionType::DELETE, act.pos, act.content};
    } else if (act.type == ActionType::DELETE) {
        return {ActionType::INSERT, act.pos, act.content};
    } else {
        std::string currentSub = document.substr(act.pos, act.content.size());
        return {ActionType::REPLACE, act.pos, currentSub};
    }
}

void UndoRedoManager::executeEdit(ActionType type, std::size_t pos, const std::string& content) {
    Action act;
    act.type = type;
    act.pos = pos;

    if (type == ActionType::INSERT) {
        act.content = content;
        document.insert(pos, content);
    } else if (type == ActionType::DELETE) {
        std::size_t len = std::stoul(content);
        if (pos + len > document.size()) {
            len = document.size() - pos;
        }
        act.content = document.substr(pos, len);
        document.erase(pos, len);
    } else if (type == ActionType::REPLACE) {
        std::string oldContent = document.substr(pos, content.size());
        document.replace(pos, content.size(), content);
        act.content = oldContent;
    }

    undoStack.push(act);

    while (!redoStack.isEmpty()) {
        redoStack.pop();
    }
}

bool UndoRedoManager::undo() {
    if (undoStack.isEmpty()) {
        std::cout << "[LOG] UNDO: No-Op (Pila Undo vacia)\n";
        return false;
    }

    Action lastAct = undoStack.top();
    undoStack.pop();

    Action invAct = invertAction(lastAct);
    applyAction(invAct);

    redoStack.push(lastAct);
    std::cout << "[LOG] UNDO: Exitoso\n";
    return true;
}

bool UndoRedoManager::redo() {
    if (redoStack.isEmpty()) {
        std::cout << "[LOG] REDO: No-Op (Pila Redo vacia)\n";
        return false;
    }

    Action actToRedo = redoStack.top();
    redoStack.pop();

    if (actToRedo.type == ActionType::INSERT) {
        document.insert(actToRedo.pos, actToRedo.content);
    } else if (actToRedo.type == ActionType::DELETE) {
        document.erase(actToRedo.pos, actToRedo.content.size());
    } else if (actToRedo.type == ActionType::REPLACE) {
        std::string currentSub = document.substr(actToRedo.pos, actToRedo.content.size());
        document.replace(actToRedo.pos, actToRedo.content.size(), actToRedo.content);
        actToRedo.content = currentSub;
    }

    undoStack.push(actToRedo);
    std::cout << "[LOG] REDO: Exitoso\n";
    return true;
}

void UndoRedoManager::processEventFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[ERROR] No se pudo abrir el archivo de eventos: " << filepath << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string command;
        ss >> command;

        if (command == "EDIT") {
            std::string opStr;
            std::size_t pos;
            std::string content;

            ss >> opStr >> pos;
            std::getline(ss, content);

            if (!content.empty() && content[0] == ' ') {
                content.erase(0, 1);
            }

            ActionType type;
            if (opStr == "INSERT") type = ActionType::INSERT;
            else if (opStr == "DELETE") type = ActionType::DELETE;
            else type = ActionType::REPLACE;

            executeEdit(type, pos, content);
        } else if (command == "UNDO") {
            undo();
        } else if (command == "REDO") {
            redo();
        }
    }
    file.close();
}

void UndoRedoManager::printSummary() const {
    std::cout << "\n========================================\n";
    std::cout << "          RESUMEN DE EJECUCION          \n";
    std::cout << "========================================\n";
    std::cout << "(a) Estado final del documento: \"" << document << "\"\n";
    std::cout << "(c) Elementos finales en Pila Undo: " << undoStack.size() << "\n";
    std::cout << "(c) Elementos finales en Pila Redo: " << redoStack.size() << "\n";
    std::cout << "========================================\n";
}

const std::string& UndoRedoManager::getDocument() const { return document; }
std::size_t UndoRedoManager::getUndoSize() const { return undoStack.size(); }
std::size_t UndoRedoManager::getRedoSize() const { return redoStack.size(); }