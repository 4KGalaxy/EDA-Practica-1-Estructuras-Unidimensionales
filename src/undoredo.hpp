#ifndef UNDOREDO_HPP
#define UNDOREDO_HPP

#include "stack_array.hpp"
#include <string>

enum class ActionType { INSERT, DELETE, REPLACE };

struct Action {
    ActionType type;
    std::size_t pos;
    std::string content;
};

class UndoRedoManager {
private:
    StackArray<Action> undoStack;
    StackArray<Action> redoStack;
    std::string document;

    void applyAction(const Action& act);
    Action invertAction(const Action& act);

public:
    UndoRedoManager();

    void processEventFile(const std::string& filepath);

    void executeEdit(ActionType type, std::size_t pos, const std::string& content);
    bool undo();
    bool redo();

    void printSummary() const;

    const std::string& getDocument() const;
    std::size_t getUndoSize() const;
    std::size_t getRedoSize() const;
};

#endif