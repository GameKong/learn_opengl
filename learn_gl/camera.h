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
const float YAW = -90.0f; // ƫ����
const float PITCH = 0.0f; // ������
const float SPEED = 2.5f; // �ٶ�ϵ��
const float SENSITIVITY = 0.1f; // �Ƕ�ƫתϵ��
const float ZOOM = 45.0f; // ��Ұ�Ƕ�ֵ


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
    float MovementSpeed;// �ٶ�ϵ��
    float MouseSensitivity;// �Ƕ�ƫתϵ��
    float Zoom;// ��Ұ�Ƕ�ֵ

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
        // �����۲���������λ�ã��۲��Ŀ����λ��(Ŀǰ�Ǿ����������λ������λ�ã����Ըı�������ķ���)��������
        return glm::lookAt(Position, Position + Front, Up);
    }

    //  ������̷������WASD �ı��������λ�ôӶ��ı�۲����
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        // �ƶ����� = �ƶ�ϵ��*��֡���ʱ�� ����������֡����α仯��ÿ���ƶ��ľ��붼��һ����
        float velocity = MovementSpeed * deltaTime;

        // ǰ�������ƶ��������ǰ��ķ�����Ŀ�귽�����ҷ����Ǹ���Ŀ�귽����Ϸ������˵õ���������Ҳ���������
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // ��������ƶ�xyƫ�������� �����Ǻ�ƫ����
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        // ƫ������
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // ȷ�������ǲ��ܴ���90�ȣ�С��-90�ȡ���ֹ��תMake sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // ���ݸ����Ǻ�ƫ���Ǹ���Ŀ������Update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // ���ݹ��ֵ�ƫ�����ı���Ұ�ĽǶȴ�С��������1��45��֮��
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
    // ���������Ŀ���������Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // ���ݸ����Ǻ�ƫ���Ǽ���Ŀ��������xyzֵ��Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        
        // ��Ŀ��������Ϊ��λ����
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        // ����ǰ���������Ϸ��������ˡ����Եõ���ֱ�ڸ�ƽ�����������ҷ�����������ʹ�ü��̿ؼ������λ��ʱ�������������
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
#endif