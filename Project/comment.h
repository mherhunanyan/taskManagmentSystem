#ifndef COMMENT_H
#define COMMENT_H
#include <QString>
#include <QDate>

class Comment
{
public:
    Comment(const QString& commentId, const QString& author, const QString& content, const QDate& timestamp);
    void editContent(const QString& newContent);

    QString commentId;
    QString author;
    QString content;
    QDate timestamp;
};

#endif // COMMENT_H
