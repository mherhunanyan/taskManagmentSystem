// comment.cpp
#include "comment.h"

Comment::Comment(const QString& commentId, const QString& author, const QString& content, const QDate& timestamp)
    : commentId(commentId), author(author), content(content), timestamp(timestamp) {}

void Comment::editContent(const QString& newContent) {
    content = newContent;
}
