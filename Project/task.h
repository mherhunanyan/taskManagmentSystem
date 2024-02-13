#ifndef TASK_H
#define TASK_H
#include <QString>
#include <QDate>
#include <QList>
#include "enums.h"
#include "user.h"
#include "comment.h"

class Comment;
class Task
{
public:
    Task(const QString& taskId, const QString& title, const QString& description, const QDate& deadline);
    void update(const QString& details);
    void setPriority(TaskPriority priority);
    void assignToUser(const User& user);
    void addComment(const Comment& comment);
    void addAtachment(const QString path);
    void changeStatus(const TaskStatus status);

    QString taskId;
    QString title;
    QString description;
    QDate deadline;
    TaskPriority priority;
    TaskStatus status;
    User assignee;
    User creator;
    QList<QString> tags;
    QDate creationDate;
    QDate lastUpdated;
    float estimatedTime;
    QList<QString> attachments;
    QList<Comment> comments;
};

#endif // TASK_H
