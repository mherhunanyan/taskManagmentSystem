#include "task.h"
#include "comment.h"

Task::Task(const QString& taskId, const QString& title, const QString& description, const QDate& deadline)
    : taskId(taskId), title(title), description(description), deadline(deadline) {}

void Task::update(const QString& details) {
    // Implement update method
}

void Task::setPriority(TaskPriority priority) {
    // Implement setPriority method
}

void Task::assignToUser(const User& user) {
    // Implement assignToUser method
}

void Task::addComment(const Comment& comment) {
    comments.append(comment);
}

void Task::changeStatus(const TaskStatus status) {
    // Implement changeStatus method
}
