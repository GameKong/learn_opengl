#include "camera.h"
#include "def_gl.h"
#include "stb_image.h"
#include <iostream>
#include<string>
using namespace std;

// ��ʼ����
int test1();

// �任 ����
int test2();

// 3D
int test3();

// �����
int test4();

// ����1����Դ��������ɫ�ϳ�
int light_test1();

// ����2�������⣬�����䣬���淴���ۺϹ�Դ
int light_test2();

// ����3�����ʡ� ��ν���ʣ�����һ�������ڻ��������������䣬���淴�����������ǿ�ȵ���ɫ����ʵĲ�ͬ���仯
int light_test3();

// ����4���������������淴������
int light_test4();

// ����5 Ͷ���
int light_test5();

// ����6 ���Դ
int light_test6();

// ģ��
int model_test1();

// ��ȡ�ģ�����
int stencil_test1();

// ��ϲ��ԡ� ���޳�����
int blend_test1();

// ֡������ԣ� ������� ��������˾�Ч��
int frame_buffer_test1();

// ��պв��ԣ���ķ���, �������
int cube_map_test1();

// ���巽��Ĳ����������������ԣ����ƶ��㻺��
int buffer_test();

// uniform������ԡ� uniform���壬uniform�顣���shader�е�uniform��ɹ���һ��uniform���壬��uniform��ͻ���󶨵�һ���󶨵����
int uniform_buffer_test();

// ������ɫ��
int geometry_test();

// ʵ����  Ҫ���ƺܶ���ͬ�����ʱ�� ����ʹ��ʵ����������cpu-gpu�Ĺ��̣��Ż����ܡ�����ݵأ�����Ч��
int instance_test();

// �����    �����������MSAA(mutiple sample anti_aliasing)
int anti_aliasing_test();

// gamma������ ��ɫ����ѹ�����Ӻ���ʾ����������Ȼ���2.2���ݵ���ʽ�����ӡ�
// �����Ƿ����Եġ���Ҫ����ɫ���������ֵ�Ͻ���1/2.2���ݵ�gamma��������������Ե���ʽ�仯
int gamma_correction_test();

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const * path, bool gammaCorrection = false);
unsigned int loadCubemap(vector<std::string> faces);

#ifdef TEXT_H

// ���������
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

bool blinn = false;
bool blinnKeyPressed = false;
bool gammaEnabled = false;
bool gammaKeyPressed = false;

// ����ѭ��������������������ĺ���
void processInput(GLFWwindow *window)
{
    // ��ȡESC������״̬����״̬Ϊ���µ�ʱ������ָ������Ӧ���Ƿ�Ӧ�ùر�Ϊtrue
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    // ������ɫģ���Ƿ��� ����blinn-����
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed)
    {
        blinn = !blinn;
        blinnKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
    {
        blinnKeyPressed = false;
    }

    // �����Ƿ���gamma����
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !gammaKeyPressed)
    {
        gammaEnabled = !gammaEnabled;
        gammaKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    {
        gammaKeyPressed = false;
    }
}

// ��������ʱ�Ļص�����������ִ���ˡ��ӿڡ������ã��ӿ��洰�ڵĴ�С�ı�
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// ��������ƶ�ƫ�������������ƫ���Ǻ͸����ǴӶ����� Ŀ��λ�� �Ӷ����ù۲���� �Ӷ��ı�����λ��glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// ���ֻص���������Ұ�Ƕ�
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// ��������ͨ�÷���
unsigned int loadTexture(char const * path, bool gammaCorrection)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum internalFormat;
        GLenum dataFormat;
        if (nrComponents == 1)
        {
            internalFormat = dataFormat = GL_RED;
        }
        else if (nrComponents == 3)
        {
            internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
            dataFormat = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
            dataFormat = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// ������պ�����
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    // �������������������������ͨ�������û����
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;

    // �������������� ���������������ͼ
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            // ��������һ�����������ͼ���ݣ���һ������ ��ʾΪ��������ͼX��������һ���������ݡ� �궼�ǰ��ŵģ����Կ���+1����
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
#endif // TEXT_H