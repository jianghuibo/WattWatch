#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"threadpool.h"
#include<QStandardItemModel>
#include<QStandardItem>
#include"clientinfo.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initUI();
    void appendToModel(ClientInfo &clientInfo);
    void appendToArm();

private slots:
    void on_pushButton_clicked();

    void on_checkBox_clicked(bool checked);

    void on_pushButton_2_clicked();

signals:

private:
    Ui::MainWindow *ui;
    QList<ClientInfo>slaveLists;    //从机表
    ThreadPool *m_pool;
    QStandardItemModel *m_model;
};
#endif // MAINWINDOW_H
