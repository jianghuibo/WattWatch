#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QWidget>

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QPointLight>
#include <Qt3DRender/QDirectionalLight>
#include <Qt3DCore/QTransform>
#include <Qt3DWindow>

#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QTorusMesh>

#include <QKeyEvent>
#include <QTransform>
#include <QMesh>

class Scene : public QObject
{
    Q_OBJECT
public:
    Scene(Qt3DExtras::Qt3DWindow *view)
    {
        //QEntity 用于表示场景中的实体。
        rootEntity = new Qt3DCore::QEntity();
        //将 rootEntity 设置为 view 对象的根实体，以便在 view 中渲染场景。
        view->setRootEntity(rootEntity);
        //设置view颜色
        view->defaultFrameGraph()->setClearColor(QColor(255, 255, 255));
        Camera(view);
        StartScene();
    }

    void KeyControls(QKeyEvent *event)
    {
        //检查是否按下了ctrl键
        if (event->modifiers().testFlag(Qt::ControlModifier)){
            //上下左右旋转
            if(event->key()==Qt::Key_Up){
                transform->setRotationX(transform->rotationX()-6);
            }
            if(event->key()==Qt::Key_Down){
                transform->setRotationX(transform->rotationX()+6);
            }
            if(event->key()==Qt::Key_Left){
                transform->setRotationY(transform->rotationY()-6);
            }
            if(event->key()==Qt::Key_Right){
                transform->setRotationY(transform->rotationY()+6);
            }//检查是否按下了shift键
        }else if (event->modifiers().testFlag(Qt::ShiftModifier)) {
            //z负轴移动
            if(event->key()==Qt::Key_Up){
                transform->setTranslation(QVector3D(transform->translation().x(),transform->translation().y(),transform->translation().z()-1));
            }
            //Z正轴移动
            if(event->key()==Qt::Key_Down){
                transform->setTranslation(QVector3D(transform->translation().x(),transform->translation().y(),transform->translation().z()+1));
            }
        }else{
            //根据方向键来移动
            if(event->key()==Qt::Key_Up){
                transform->setTranslation(QVector3D(transform->translation().x(),transform->translation().y()+1,transform->translation().z()));
                qDebug()<<"上"<<transform->translation();
            }
            if(event->key()==Qt::Key_Down){
                transform->setTranslation(QVector3D(transform->translation().x(),transform->translation().y()-1,transform->translation().z()));
                qDebug()<<"下"<<transform->translation();
            }
            if(event->key()==Qt::Key_Left){
                transform->setTranslation(QVector3D(transform->translation().x()-1,transform->translation().y(),transform->translation().z()));
                qDebug()<<"左"<<transform->translation();
            }
            if(event->key()==Qt::Key_Right){
                transform->setTranslation(QVector3D(transform->translation().x()+1,transform->translation().y(),transform->translation().z()));
                qDebug()<<"右"<<transform->translation();
            }
        }
    }

    void MouseControls(QMouseEvent *event)
    {
        if(event->button() == Qt::LeftButton)
        {
            transform->setRotationX(transform->rotationX()-6);
        }
        else if(event->button() == Qt::RightButton)
        {
            transform->setRotationX(transform->rotationX()+6);
        }
    }

    void WheelControls(QWheelEvent *event)
    {
        if(event->angleDelta().y()>0)
        {
            transform->setRotationY(transform->rotationY()-6);
        }
        else if(event->angleDelta().y()<0)
        {
            transform->setRotationY(transform->rotationY()+6);
        }
    }

private:
    Qt3DCore::QEntity *rootEntity;
    Qt3DCore::QEntity *model;
    Qt3DCore::QTransform *transform;
    Qt3DRender::QDirectionalLight *light;

    void StartScene()
    {
        //这个实体对象将作为场景中的模型
        model = new Qt3DCore::QEntity(rootEntity);
        //这个网格对象用于表示一个圆环模型。
        Qt3DRender::QMesh *mesh = new Qt3DRender::QMesh();
        QUrl data =QUrl::fromLocalFile("C:/Users/jiang/Desktop/FParms.obj");
        mesh->setSource(data);
        //Qt3DExtras::QTorusMesh *torusMesh = new Qt3DExtras::QTorusMesh(model);
        // torusMesh->setRadius(5);//设置圆环的半径
        // torusMesh->setMinorRadius(1);//设置圆环的小半径为 1。
        // torusMesh->setRings(100);//设置圆环的环数为 100。
        // torusMesh->setSlices(20);//设置圆环的切片数为 20。
        // //将圆环网格对象作为组件添加到模型实体中。
        model->addComponent(mesh);

        SetupTransform();//设置模型的变换（位置，旋转，缩放）
        SetupMaterial();//设置模型的材质（颜色，纹理）
        SetupLighting();//设置场景的光照。
    }

    void Camera(Qt3DExtras::Qt3DWindow *view)
    {
        //获取 view 对象的相机对象，并将其指针存储在名为 camera 的局部变量中。
        //QCamera 用于定义观察场景的视角。
        Qt3DRender::QCamera *camera = view->camera();
        //设置相机的透视投影参数。这里使用了透视投影，
        //参数依次为视场角度（45.0度）、宽高比（16:9）、
        //近裁剪面距离（0.1）和远裁剪面距离（1000.0）。
        camera->lens()->setPerspectiveProjection(45.0f,16.0f/9.0f, 0.1f, 1000.0f);
        //设置相机的位置为 (0, 0, 40)，即相机在场景中的位置。
        camera->setPosition(QVector3D(0, 0, 40));
        //设置相机的视点中心为 (0, 0, 0)，即相机观察的目标点。
        camera->setViewCenter(QVector3D(0, 0, 0));
    }

    void SetupMesh(Qt3DRender::QMesh *mesh)
    {
        model->addComponent(mesh);
    }

    void SetupTransform()
    {
        //这个变换对象用于定义模型的位置、旋转和缩放。
        transform = new Qt3DCore::QTransform(model);
        //设置模型的缩放因子为 (1, 1, 1)，表示不进行缩放。
        transform->setScale3D(QVector3D(1, 1, 1));
        //设置模型的旋转。创建一个四元数，表示绕 (1, 0, 0) 轴旋转 0 度，即不进行旋转。
        transform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 0));
        //设置3D图像的位置
        transform->setTranslation(QVector3D(-27, -16, 23));
        //将变换对象作为组件添加到模型实体中。
        model->addComponent(transform);
    }

    void SetupMaterial()
    {
        //QPhongMaterial 是一个用于渲染光照效果的材质。
        Qt3DRender::QMaterial *material = new Qt3DExtras::QPhongMaterial(model);
        model->addComponent(material);
    }

    void SetupLighting()
    {
        //这个实体对象将用于表示光源。
        Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
        //这个光源对象用于定义方向光。
        light = new Qt3DRender::QDirectionalLight(lightEntity);
        //设置光源的颜色为白色。
        light->setColor("white");
        //设置光源的强度为 1。
        light->setIntensity(0.8);
        //设置光源的世界方向为 (0, 0, -1)，即指向场景的负 z 轴方向。
        light->setWorldDirection(QVector3D(0,0,-1));
        //将光源对象作为组件添加到光源实体中。
        lightEntity->addComponent(light);
    }

    QPoint m_lastMousePos;
};

#endif // SCENE_H
