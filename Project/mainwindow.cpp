#include "mainwindow.h"
#include "taskmanager.h"

#include <QDir>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileInfo>

QString TaskInfo(Task* task)
{
    QString taskDetails;
    taskDetails += "\tTask ID: " + task->taskId + "\n";
    taskDetails += "\tTitle: " + task->title + "\n";
    taskDetails += "\tDescription: " + task->description + "\n";
    taskDetails += "\tCreationDate: " + task->creationDate.toString() + "\n";
    taskDetails += "\tDeadline: " + task->deadline.toString() + "\n";
    for(int i = 0; i < task->comments.size(); i++)
    {
        taskDetails += "\n\tComment: " + QString::number(i + 1)
                       + "\n\t\t" + "Author - "+ task->comments[i].author +
                        + "\n\t\t" + "Comment - "+ task->comments[i].content +
                       + "\n\t\t" + "Date - "+ task->comments[i].timestamp.toString() + "\n"
            ;
    }
    return taskDetails;
}

QString currentPath = __FILE__;
QString taskFilePath = QFileInfo(currentPath).absolutePath() + "/task.json"; // Construct the file path

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    CreateWidgets();
    LayoutWidgets();

    loadedTasks = TaskManager::loadTasks(taskFilePath); // Update the path here
    for(const auto& item : loadedTasks){
        qDebug() << item.description;
    }

    ReloadList();
    setFixedSize(700,500);
}

MainWindow::~MainWindow() {}

void MainWindow::CreateWidgets()
{
    int width = 100;
    int height = 20;
    m_createTask = new QPushButton("Create Task", this);
    m_createTask->setFixedSize(width, height);
    connect(m_createTask, SIGNAL(clicked(bool)), this, SLOT(OnCreate()));

    m_editTask = new QPushButton("Edit Task", this);
    m_editTask->setFixedSize(width, height);
    connect(m_editTask, SIGNAL(clicked(bool)), this, SLOT(OnEdit()));

    m_getTask = new QPushButton("Get Task", this);
    m_getTask->setFixedSize(width, height);
    connect(m_getTask, SIGNAL(clicked(bool)), this, SLOT(OnGet()));

    m_removeTask = new QPushButton("Remove Task", this);
    m_removeTask->setFixedSize(width, height);
    connect(m_removeTask, SIGNAL(clicked(bool)), this, SLOT(OnRemove()));

    m_taskList = new QTextEdit(QString("TASK LIST"), this);
    m_taskList->setReadOnly(true);
    m_taskList->setContentsMargins(30, 5, 5, 5);
    m_taskList->setStyleSheet("background-color : white; color : blue;");

}

void MainWindow::LayoutWidgets()
{
    QWidget* mainWidget = new QWidget();
    QVBoxLayout*  buttonLayout = new QVBoxLayout();
    QHBoxLayout*  mainLayout = new QHBoxLayout();

    buttonLayout->addWidget(m_createTask);
    buttonLayout->addWidget(m_editTask);

    buttonLayout->addWidget(m_getTask);

    buttonLayout->addWidget(m_removeTask);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(m_taskList);

    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

}

void MainWindow::ReloadList()
{
    QString list="";
    for(int i = 0; i < loadedTasks.size(); ++i ){

        list += "Task - " + QString::number(i + 1) + "\n\n" + TaskInfo(&loadedTasks[i]) + "\n\n\n";
    }
    m_taskList->setText(list);

}

void MainWindow::OnCreate()
{
    QString taskId = QInputDialog::getText(this, tr("Create Task ID"), tr("Enter task Id"));
    QString taskTitle = QInputDialog::getText(this, tr("Create Task ID"), tr("Enter task Title"));
    QString taskDescription = QInputDialog::getText(this, tr("Create Task ID"), tr("Enter task Description"));
    QString taskDeadline = QInputDialog::getText(this, tr("Create Task ID"), tr("Enter task Deadline"));

    Task* newTask= new Task(taskId, taskTitle, taskDescription, QDate::fromString(taskDeadline, Qt::DateFormat::ISODate));

    newTask->creationDate = QDate::currentDate();

    loadedTasks.append(*newTask);
    TaskManager::saveTasks(loadedTasks, taskFilePath);


    QMessageBox::information(this, tr("Task Created"), TaskInfo(newTask));
    ReloadList();
}

void MainWindow::OnEdit()
{
    QString taskId = QInputDialog::getText(this, tr("Edit Task"), tr("Enter task Id"));

    Task* foundTask = nullptr;
    for(int i = 0; i < loadedTasks.size(); ++i ){
        if(loadedTasks[i].taskId == taskId){
            foundTask = &loadedTasks[i];
            break;
        }
    }

    if (foundTask != nullptr) {



        QMessageBox::information(this, tr("Task Details"), TaskInfo(foundTask));

        QString newTitle = QInputDialog::getText(this, tr("Edit Task Title"), tr("Enter new task Title"), QLineEdit::Normal, foundTask->title);
        QString newDescription = QInputDialog::getText(this, tr("Edit Task Description"), tr("Enter new task Description"), QLineEdit::Normal, foundTask->description);
        QString newDeadline = QInputDialog::getText(this, tr("Edit Task Deadline"), tr("Enter new task Deadline"), QLineEdit::Normal, foundTask->deadline.toString());

        foundTask->title = newTitle;
        foundTask->description = newDescription;
        foundTask->deadline = QDate::fromString(newDeadline);

        QMessageBox::information(this, tr("Task Updated Successfully"), TaskInfo(foundTask));
    }
    else{
        QMessageBox::warning(this, tr("Edit Task"), tr("Task not found!"));
    }
    ReloadList();
}

QString MainWindow::OnGet()
{
    QString taskId = QInputDialog::getText(this, tr("Get Task"), tr("Enter taskId"));

    Task* foundTask = nullptr;
    for(int i = 0; i < loadedTasks.size(); ++i ){
        if(loadedTasks[i].taskId == taskId){
            foundTask = &loadedTasks[i];
            break;
        }
    }

    if (foundTask != nullptr) {

        QString content = QInputDialog::getText(this, tr("Task Details"), TaskInfo(foundTask) + QString("\n\nAdd Comment"));
        if(!content.isEmpty())
        {
            QString author = QInputDialog::getText(this, tr("Enter Author"), tr("Author"));
            foundTask->comments.append(Comment(QString::number(foundTask->comments.size() + 1), author, content, QDate::currentDate()));
            TaskManager::saveTasks(loadedTasks, taskFilePath);
            ReloadList();
        }
        return taskId;
    } else {
        QMessageBox::warning(this, tr("Get Task"), tr("Task not found!"));
        return "";
    }
}

void MainWindow::OnRemove()
{
    QString taskId = QInputDialog::getText(this, tr("Get Task"), tr("Enter taskId"));

    Task* foundTask = nullptr;
    int i = 0;
    for(; i < loadedTasks.size(); ++i ){
        if(loadedTasks[i].taskId == taskId){
            foundTask = &loadedTasks[i];
            break;
        }
    }

    if (foundTask != nullptr) {

        QString info = TaskInfo(foundTask);
        loadedTasks.remove(i);
        TaskManager::saveTasks(loadedTasks, taskFilePath);
        QMessageBox::information(this, tr("Task Removed"), info);
    } else {
        QMessageBox::warning(this, tr("Remove Task"), tr("Task not found!"));
    }
    ReloadList();
}
