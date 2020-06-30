#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f; // 偏航角
const float PITCH = 0.0f; // 俯仰角
const float SPEED = 2.5f; // 速度系数
const float SENSITIVITY = 0.1f; // 角度偏转系数
const float ZOOM = 45.0f; // 视野角度值


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;// 速度系数
    float MouseSensitivity;// 角度偏转系数
    float Zoom;// 视野角度值

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        // 创建观察矩阵。摄像机位置，观察的目标点的位置(目前是距离摄像机单位向量的位置，可以改变摄像机的方向)，上向量
        return glm::lookAt(Position, Position + Front, Up);
    }

    //  输入键盘方向键。WASD 改变摄像机的位置从而改变观察矩阵
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        // 移动倍率 = 移动系数*两帧间隔时间 ，这样不管帧率如何变化，每秒移动的距离都是一样的
        float velocity = MovementSpeed * deltaTime;

        // 前后左右移动摄像机。前后的方向是目标方向，左右方向是根据目标方向和上方方向叉乘得到的摄像机右侧向量方向
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // 根据鼠标移动xy偏移量设置 俯仰角和偏航角
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        // 偏移量和
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // 确保俯仰角不能大于90度，小于-90度。防止翻转Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // 根据俯仰角和偏航角更新目标向量Update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // 根据滚轮的偏移量改变视野的角度大小。限制在1到45度之间
    void ProcessMouseScroll(float yoffset)
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

private:
    // 计算摄像机目标的正方向Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // 根据俯仰角和偏航角计算目标向量的xyz值。Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        
        // 将目标向量变为单位向量
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        // 根据前方和世界上方向量求叉乘。可以得到垂直于该平面的摄像机正右方向量，用于使用键盘控件摄像机位置时进行向量的相加
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
#endif