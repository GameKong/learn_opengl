#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ��Ϊ��ͷ�ļ��ж����˺��������û�к���ƣ���ÿ�ΰ���ͷ�ļ���ʱ�򶼻ᶨ��һ�Σ��ͻ���ɶ�ζ�����벻ͨ�������Խ���main�����������Ȼ�����ͷ�ļ�ִ�к�������
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TEXT_H
#include "test.h"

#define MODEL_FUNC_DEF
#include "model.h"

int main()
{
   int t = gamma_correction_test();
   return t;
}
