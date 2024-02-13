#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QDialog>
#include <QTextEdit>

#include "task.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void CreateWidgets();
    void LayoutWidgets();

    void ReloadList();

private slots:
    void OnCreate();
    void OnEdit();
    QString OnGet();
    void OnRemove();

private:

    QPushButton* m_createTask;
    QPushButton* m_editTask;
    QPushButton* m_getTask;
    QPushButton* m_removeTask;

    QList<Task> loadedTasks;

    QDialog* m_createDialog;
    QTextEdit* m_taskList;
};

#endif // MAINWINDOW_H
