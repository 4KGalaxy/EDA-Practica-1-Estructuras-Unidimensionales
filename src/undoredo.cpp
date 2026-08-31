#include "undoredo.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> 

template <typename PilaTipo>
UndoRedoManager<PilaTipo>::UndoRedoManager() : document("") {}

template <typename PilaTipo>
void UndoRedoManager<PilaTipo>::applyAction(const Action& act) {
    std::size_t safePos = std::min(act.pos, document.size());

    if (act.type == ActionType::INSERT) {
        document.insert(safePos, act.content);
    } else if (act.type == ActionType::DELETE) {
        std::size_t safeLen = std::min(act.content.size(), document.size() - safePos);
        document.erase(safePos, safeLen);
    } else if (act.type == ActionType::REPLACE) {
        document.replace(safePos, act.content.size(), act.content);
    }
}

template <typename PilaTipo>
Action UndoRedoManager<PilaTipo>::invertAction(const Action& act) {
    if (act.type == ActionType::INSERT) {
        return {ActionType::DELETE, act.pos, act.content};
    } else if (act.type == ActionType::DELETE) {
        return {ActionType::INSERT, act.pos, act.content};
    } else {
        std::size_t safePos = std::min(act.pos, document.size());
        std::size_t safeLen = std::min(act.content.size(), document.size() - safePos);
        std::string currentSub = document.substr(safePos, safeLen);
        return {ActionType::REPLACE, act.pos, currentSub};
    }
}

template <typename PilaTipo>
void UndoRedoManager<PilaTipo>::executeEdit(ActionType type, std::size_t pos, const std::string& content) {
    Action act;
    act.type = type;
    act.pos = std::min(pos, document.size());

    if (type == ActionType::INSERT) {
        act.content = content;
        document.insert(act.pos, content);
    } else if (type == ActionType::DELETE) {
        std::size_t len = std::stoul(content);
        if (act.pos + len > document.size()) {
            len = document.size() - act.pos;
        }
        act.content = document.substr(act.pos, len);
        document.erase(act.pos, len);
    } else if (type == ActionType::REPLACE) {
        std::string oldContent = document.substr(act.pos, content.size());
        document.replace(act.pos, content.size(), content);
        act.content = oldContent;
    }

    undoStack.push(act);

    while (!redoStack.isEmpty()) {
        redoStack.pop();
    }
}

template <typename PilaTipo>
bool UndoRedoManager<PilaTipo>::undo() {
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

template <typename PilaTipo>
bool UndoRedoManager<PilaTipo>::redo() {
    if (redoStack.isEmpty()) {
        std::cout << "[LOG] REDO: No-Op (Pila Redo vacia)\n";
        return false;
    }

    Action actToRedo = redoStack.top();
    redoStack.pop();

    std::size_t safePos = std::min(actToRedo.pos, document.size());

    if (actToRedo.type == ActionType::INSERT) {
        document.insert(safePos, actToRedo.content);
    } else if (actToRedo.type == ActionType::DELETE) {
        std::size_t safeLen = std::min(actToRedo.content.size(), document.size() - safePos);
        document.erase(safePos, safeLen);
    } else if (actToRedo.type == ActionType::REPLACE) {
        std::size_t safeLen = std::min(actToRedo.content.size(), document.size() - safePos);
        std::string currentSub = document.substr(safePos, safeLen);
        document.replace(safePos, actToRedo.content.size(), actToRedo.content);
        actToRedo.content = currentSub;
    }

    undoStack.push(actToRedo);
    std::cout << "[LOG] REDO: Exitoso\n";
    return true;
}

template <typename PilaTipo>
void UndoRedoManager<PilaTipo>::processEventFile(const std::string& filepath) {
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

            if (content.size() >= 2 && content.front() == '"' && content.back() == '"') {
                content = content.substr(1, content.size() - 2);
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

template <typename PilaTipo>
void UndoRedoManager<PilaTipo>::printSummary() const {
    std::cout << "\n========================================\n";
    std::cout << "          RESUMEN DE EJECUCION          \n";
    std::cout << "========================================\n";
    std::cout << "(a) Estado final del documento: \"" << document << "\"\n";
    std::cout << "(c) Elementos finales en Pila Undo: " << undoStack.size() << "\n";
    std::cout << "(c) Elementos finales en Pila Redo: " << redoStack.size() << "\n";
    std::cout << "========================================\n";
}

template <typename PilaTipo>
const std::string& UndoRedoManager<PilaTipo>::getDocument() const { return document; }

template <typename PilaTipo>
std::size_t UndoRedoManager<PilaTipo>::getUndoSize() const { return undoStack.size(); }

template <typename PilaTipo>
std::size_t UndoRedoManager<PilaTipo>::getRedoSize() const { return redoStack.size(); }


template class UndoRedoManager<StackArray<Action>>;
template class UndoRedoManager<StackList<Action>>;