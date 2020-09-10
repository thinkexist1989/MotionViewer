#ifndef GLCAMERA_H
#define GLCAMERA_H

#include <QOpenGLFunctions>
#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>
#include <QPoint>

const float YAW         = -45.0f;
const float PITCH       = -45.0f;
const float ZOOM        = 45.0f;
const float SENSITIVITY = 0.002f;
const float SPEED       = 0.0001f;


class GLCamera : protected QOpenGLFunctions
{
public:
    //camera相关向量
    QVector3D pos;
    QVector3D front;
    QVector3D up;
    QVector3D right;
    QVector3D worldUp;
    //欧拉角
    float yaw;
    float pitch;
    //相机选项
    float zoom; //缩放
    float mouseSensitivity; //鼠标灵敏度
    float moveSpeed; //移动速度


public:
    explicit GLCamera(QVector3D cameraPos = QVector3D(-1.0f, 1.0f, -1.0f),
             QVector3D _worldUp = QVector3D(-1.0f, 0.0f, 0.0f),
             float _yaw = YAW,
             float _pitch = PITCH ) :
        zoom(ZOOM),
        mouseSensitivity(SENSITIVITY),
        moveSpeed(SPEED)
    {
        this->pos     = cameraPos;
        this->worldUp = _worldUp;
        this->yaw     = _yaw;
        this->pitch   = _pitch;

        updateCameraVector();

    }

    explicit GLCamera(float posX, float posY, float posZ,
             float upX, float upY, float upZ,
             float _yaw = YAW,
             float _pitch = PITCH ) :
        zoom(ZOOM),
        mouseSensitivity(SENSITIVITY),
        moveSpeed(SPEED)
    {
        this->pos     = QVector3D(posX, posY, posZ);
        this->worldUp = QVector3D(upX, upY, upZ);
        this->yaw     = _yaw;
        this->pitch   = _pitch;

        updateCameraVector();
    }

    //调用getViewMatrix可获取View矩阵
    QMatrix4x4 getViewMatrix()
    {
        QMatrix4x4 m;
        m.lookAt(pos, pos + front, up); //注意：lookAt矩阵的up是指向眼镜上方的向量
        return m;
    }

    void cameraRotate(QPoint d, bool constrainPitch = true)
    {
        //        yaw = fmod(yaw + d.x()*fov*0.001, 360); //fmod is like % operator for float
        //        tilt = fmod(tilt - d.y()*fov*0.001, 360);
        //        pitch = fmod(pitch - d.y()*fov*0.001, 360);

        yaw = fmod(yaw + d.x()*zoom*mouseSensitivity, 360.0);

        pitch -= d.y()*zoom*mouseSensitivity;

        if(constrainPitch)
        {
            if(pitch < -89.0f)
                pitch = -89.0f;
            if(pitch > 89.0f)
                pitch = 89.0f;
        }

        updateCameraVector();
    }

    void cameraTranslate(QPoint d)
    {
        pos -= d.x() * zoom * moveSpeed * right;
        pos += d.y() * zoom * moveSpeed * QVector3D::crossProduct(worldUp, right).normalized();

        updateCameraVector();
    }

private:
    void updateCameraVector()
    {
        front = QVector3D(-sin(qDegreesToRadians(pitch)),
                        cos(qDegreesToRadians(pitch))*sin(qDegreesToRadians(yaw)),
                        cos(qDegreesToRadians(pitch))*cos(qDegreesToRadians(yaw))).normalized();

        right = QVector3D::crossProduct(front, worldUp).normalized();
        up = QVector3D::crossProduct(right, front).normalized();
    }

};

#endif // GLCAMERA_H
