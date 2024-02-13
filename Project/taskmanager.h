#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "task.h"
#include <QJsonObject>
#include <QJsonArray>

class TaskManager
{
public:
    static void saveTasks(const QList<Task>& tasks, const QString& filename);
    static QList<Task> loadTasks(const QString& filename);

private:
    static QJsonObject taskToJson(const Task& task);
    static Task taskFromJson(const QJsonObject& json);
};

#endif // TASKMANAGER_H
