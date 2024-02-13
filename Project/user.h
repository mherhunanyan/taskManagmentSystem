#ifndef USER_H
#define USER_H
#include <QString>

class User {
public:
    User(QString name, QString surname, QString id) :name(name), surname(surname), id(id) {}
    User() {}
    QString name;
    QString surname;
    QString id;
};

#endif // USER_H
