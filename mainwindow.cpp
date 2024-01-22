#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"modelviewer.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,m_pool(NULL)
{
    ui->setupUi(this);
    m_pool = new ThreadPool();
    //页面初始化
    initUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}
/*
    页面初始化
*/

void MainWindow::initUI()
{
    m_model = new QStandardItemModel(this);
    QStringList headList;
    headList<<"客户端名称"<<"端口号"<<"线程ID"<<"机械臂";
    connect(m_pool,&ThreadPool::sendClintInfo,this,[&](ClientInfo clientInfo){
            slaveLists.append(clientInfo);
            appendToModel(clientInfo);
            appendToArm();
    });
    m_model->setHorizontalHeaderLabels(headList);
    ui->clientView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不允许编辑
    ui->clientView->setModel(m_model);
}

/*
    tableView中添加连接的客户端的信息
*/
void MainWindow::appendToModel(ClientInfo &clientInfo)
{
    QStandardItem *itemName = new QStandardItem(clientInfo.clientName());
    itemName->setCheckable(true);//添加复选框
    QStandardItem *itemPort = new QStandardItem(QString::number(clientInfo.clientPort()));
    QStandardItem *itemThreadId = new QStandardItem(clientInfo.threadId());
    QList<QStandardItem*>rowItem;
    rowItem.append(itemName);
    rowItem.append(itemPort);
    rowItem.append(itemThreadId);
    m_model->appendRow(rowItem);
}

void MainWindow::appendToArm()
{
    ModelViewer *mo = new ModelViewer();
    mo->setFixedSize(60,40);
    int count = ui->clientView->model()->rowCount();
    qDebug()<<"count:"<<count;
    m_model->setData(m_model->index(count-1,3),QVariant::fromValue(mo),Qt::UserRole);
    ui->clientView->setIndexWidget(m_model->index(count-1,3), mo);
}

/*
    启动服务器
*/
void MainWindow::on_pushButton_clicked()
{
    m_pool->start();
}

/*
    如果选中了界面中的全选按钮，那么，tableView中所有项的复选框都将被选中
*/
void MainWindow::on_checkBox_clicked(bool checked)
{
    for(int i = 0;i<m_model->rowCount();i++)
    {
        if(checked)
        {
            m_model->item(i)->setCheckState(Qt::Checked);
        }
        else
        {
            m_model->item(i)->setCheckState(Qt::Unchecked);
        }
    }
}

/*
    删除选中的复选框的数据以及主从表中的数据
*/
void MainWindow::on_pushButton_2_clicked()
{
    for(int i = 0;i<m_model->rowCount();i++)
    {
        QStandardItem *item = m_model->item(i);
        if(item->checkState() == Qt::Checked)
        {
            m_pool->disconnectClient(slaveLists.at(i).clientPort());//断开要删除的客户端的连接
            slaveLists.removeAt(i);//主从表中删除
            m_model->takeRow(i);//删除tableView中的数据
        }
    }
}

