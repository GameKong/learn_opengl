#include "camera.h"
#include "def_gl.h"
#include "stb_image.h"
#include <iostream>
#include<string>
using namespace std;

// 初始纹理
int test1();

// 变换 矩阵
int test2();

// 3D
int test3();

// 摄像机
int test4();

// 光照1，光源和物体颜色合成
int light_test1();

// 光照2，环境光，漫反射，镜面反射综合光源
int light_test2();

// 光照3，材质。 所谓材质，就是一个物体在环境分量，漫反射，镜面反射分量，反射强度的颜色随材质的不同而变化
int light_test3();

// 光照4，漫反射纹理、镜面反射纹理
int light_test4();

// 光照5 投射光
int light_test5();

// 光照6 多光源
int light_test6();

// 模型
int model_test1();

// 深度、模板测试
int stencil_test1();

// 混合测试、 面剔除测试
int blend_test1();

// 帧缓冲测试， 卷积矩阵 给纹理加滤镜效果
int frame_buffer_test1();

// 天空盒测试，光的反射, 光的折射
int cube_map_test1();

// 缓冲方面的操作，分批顶点属性，复制顶点缓冲
int buffer_test();

// uniform缓冲测试。 uniform缓冲，uniform块。多个shader中的uniform块可共用一个uniform缓冲，把uniform块和缓冲绑定到一个绑定点就行
int uniform_buffer_test();

// 几何着色器
int geometry_test();

// 实例化  要绘制很多相同物体的时候 可以使用实例化。减少cpu-gpu的过程，优化性能。例如草地，粒子效果
int instance_test();

// 抗锯齿    多点采样抗锯齿MSAA(mutiple sample anti_aliasing)
int anti_aliasing_test();

// gamma矫正。 颜色（电压）增加后，显示器输出的亮度会以2.2次幂的形式的增加。
// 所以是非线性的。需要在颜色的最终输出值上进行1/2.2次幂的gamma矫正后才能以线性的形式变化
int gamma_correction_test();

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const * path, bool gammaCorrection = false);
unsigned int loadCubemap(vector<std::string> faces);

#ifdef TEXT_H

// 摄像机对象
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

// 在主循环中用来补货键盘输入的函数
void processInput(GLFWwindow *window)
{
    // 获取ESC按键的状态，当状态为按下的时候。设置指定窗口应该是否应该关闭为true
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

    // 控制颜色模型是冯氏 还是blinn-冯氏
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed)
    {
        blinn = !blinn;
        blinnKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
    {
        blinnKeyPressed = false;
    }

    // 控制是否开启gamma矫正
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

// 调整窗口时的回调函数。函数执行了【视口】的设置，视口随窗口的大小改变
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// 根据鼠标移动偏移量，设置相机偏航角和俯仰角从而设置 目标位置 从而设置观察矩阵 从而改变物体位置glfw: whenever the mouse moves, this callback is called
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

// 滚轮回调，设置视野角度
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// 加载纹理通用方法
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

// 加载天空盒纹理
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    // 生成立方体纹理对象，与生成普通对象基本没区别
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;

    // 设置立方体纹理 的六个面的纹理贴图
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            // 设置其中一个面的纹理贴图数据，第一个参数 表示为立方体贴图X正方向那一面设置数据。 宏都是挨着的，所以可以+1遍历
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