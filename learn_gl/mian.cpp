#include <glad/glad.h>
#include <GLFW/glfw3.h>

// 因为在头文件中定义了函数。如果没有宏控制，在每次包含头文件的时候都会定义一次，就会造成多次定义编译不通过。所以仅在main函数处定义宏然后包含头文件执行函数定义
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
