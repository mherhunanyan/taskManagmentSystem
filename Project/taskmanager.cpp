#include "taskmanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

void TaskManager::saveTasks(const QList<Task>& tasks, const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonArray tasksArray;
    for (const Task& task : tasks) {
        tasksArray.append(taskToJson(task));
    }

    QJsonDocument saveDoc(tasksArray);
    file.write(saveDoc.toJson());
    file.close();
}

QList<Task> TaskManager::loadTasks(const QString& filename) {
    QList<Task> tasks;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open load file.");
        return tasks;
    }

    QByteArray saveData = file.readAll();
    QJsonArray tasksArray = QJsonDocument::fromJson(saveData).array();
    file.close();

    for (const QJsonValue& value : tasksArray) {
        tasks.append(taskFromJson(value.toObject()));
    }

    return tasks;
}

QJsonObject TaskManager::taskToJson(const Task& task) {
    QJsonObject jsonTask;
    jsonTask["taskId"] = task.taskId;
    jsonTask["title"] = task.title;
    jsonTask["description"] = task.description;
    jsonTask["deadline"] = task.deadline.toString(Qt::ISODate);
    jsonTask["priority"] = task.priority;
    jsonTask["status"] = task.status;
    jsonTask["assignee"] = task.assignee.name;
    jsonTask["creator"] = task.creator.name;

    // Convert tags from QStringList to QJsonArray
    QJsonArray tagsArray;
    for (const QString& tag : task.tags) {
        tagsArray.append(tag);
    }
    jsonTask["tags"] = tagsArray;

    jsonTask["creationDate"] = task.creationDate.toString(Qt::ISODate);
    jsonTask["lastUpdated"] = task.lastUpdated.toString(Qt::ISODate);
    jsonTask["estimatedTime"] = task.estimatedTime;

    // Convert attachments from QStringList to QJsonArray
    QJsonArray attachmentsArray;
    for (const QString& attachment : task.attachments) {
        attachmentsArray.append(attachment);
    }
    jsonTask["attachments"] = attachmentsArray;

    // Serialize comments
    QJsonArray commentsArray;
    for (const Comment& comment : task.comments) {
        QJsonObject jsonComment;
        jsonComment["commentId"] = comment.commentId;
        jsonComment["author"] = comment.author;
        jsonComment["content"] = comment.content;
        jsonComment["timestamp"] = comment.timestamp.toString(Qt::ISODate);
        commentsArray.append(jsonComment);
    }
    jsonTask["comments"] = commentsArray;

    return jsonTask;
}


Task TaskManager::taskFromJson(const QJsonObject& json) {
    Task task(
        json["taskId"].toString(),
        json["title"].toString(),
        json["description"].toString(),
        QDate::fromString(json["deadline"].toString(), Qt::ISODate)
        );

    task.priority = static_cast<TaskPriority>(json["priority"].toInt());
    task.status = static_cast<TaskStatus>(json["status"].toInt());
    task.assignee.name = json["assignee"].toString();
    task.creator.name = json["creator"].toString();

    // Convert tags from QJsonArray to QStringList
    task.tags.clear();
    QJsonArray tagsArray = json["tags"].toArray();
    for (const QJsonValue& value : tagsArray) {
        task.tags.append(value.toString());
    }

    task.creationDate = QDate::fromString(json["creationDate"].toString(), Qt::ISODate);
    task.lastUpdated = QDate::fromString(json["lastUpdated"].toString(), Qt::ISODate);
    task.estimatedTime = json["estimatedTime"].toDouble();

    // Convert attachments from QJsonArray to QStringList
    task.attachments.clear();
    QJsonArray attachmentsArray = json["attachments"].toArray();
    for (const QJsonValue& value : attachmentsArray) {
        task.attachments.append(value.toString());
    }

    // Deserialize comments
    task.comments.clear();
    QJsonArray commentsArray = json["comments"].toArray();
    for (const QJsonValue& value : commentsArray) {
        QJsonObject jsonComment = value.toObject();
        task.comments.append(
            Comment(
                jsonComment["commentId"].toString(),
                jsonComment["author"].toString(),
                jsonComment["content"].toString(),
                QDate::fromString(jsonComment["timestamp"].toString(), Qt::ISODate)
                )
            );
    }

    return task;
}
