#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include <QHBoxLayout>
#include <QWidget>
#include"scene.h"
class ModelViewer : public QWidget
{
    Q_OBJECT
public:
    explicit ModelViewer(QWidget *parent = nullptr): QWidget{parent}
    {
        Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
        QWidget *sceneWidget = QWidget::createWindowContainer(view);
        scene = new Scene(view);
        view-> installEventFilter(this);
        QHBoxLayout *hbox = new QHBoxLayout();
        hbox->addWidget(sceneWidget);
        setLayout(hbox);
    }

    /*
    对鼠标、键盘、滑轮事件进行判断
    */
    bool eventFilter(QObject *obj, QEvent *event)
    {
        //检查事件是否为按键事件
        if (event->type() == QEvent::KeyPress){
            //将事件转换为键盘事件，以便获取键盘按键的详细信息
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            //调用Scene类的函数，将键盘事件传递给场景进行处理，这意味这键盘事件的处理逻辑由场景对象
            //的KeyControls函数来执行
            scene->KeyControls(keyEvent);
        }
        else if(event->type() == QEvent::MouseButtonDblClick)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            scene->MouseControls(mouseEvent);
        }
        else if(event->type() == QEvent::Wheel)
        {
            QWheelEvent *wheel = static_cast<QWheelEvent *>(event);
            scene->WheelControls(wheel);
        }
        //如果事件不是键盘按键事件，或者在处理完键盘事件后，
        //将事件传递给父类的事件过滤器。
        return QObject::eventFilter(obj, event);
    }
signals:
private:
    Scene *scene;
};

#endif // MODELVIEWER_H
