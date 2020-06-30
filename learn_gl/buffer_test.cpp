#include <glad/glad.h>
#include <GLFW/glfw3.h>

int buffer_test()
{
    /*
        //复制顶点数据到指定内存
        float data[18];
        float data2[18];
        int vbo;
        glGenBuffer(1,&vbo);
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBufferData(GL_ARRAY_BUFFER,sizeof(data),&data[0], GL_STATIC_DRAW);
        
        // 获取目标当前绑定的缓冲地址。然后写入数据
        void* ptr = glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
        memcpy(ptr,data2,sizeof(data2));
        glUnmapBuffer(GL_ARRAY_BUFFER);
    */

    /*
    //填充数据到缓冲的指定位置，提供一个偏移量  glBufferSubData(GL_ARRAY_BUFFER,10,sizeof(data2),&data2);

    float positions[] = { ... };
    float normals[] = { ... };
    float tex[] = { ... };
    // 填充缓冲
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), &positions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(normals), &normals);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(normals), sizeof(tex), &tex);
    这样子我们就能直接将属性数组作为一个整体传递给缓冲，而不需要事先处理它们了。我们仍可以将它们合并为一个大的数组，再使用glBufferData来填充缓冲，但对于这种工作，使用glBufferSubData会更合适一点。

    我们还需要更新顶点属性指针来反映这些改变：

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(positions)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(positions) + sizeof(normals)));
   
    */

    /*
      //复制顶点数据到指定内存
    void glCopyBufferSubData(GLenum readtarget, GLenum writetarget, GLintptr readoffset,
                         GLintptr writeoffset, GLsizeiptr size);

    float vertexData[] = { ... };                         
    glBindBuffer(GL_COPY_READ_BUFFER, vbo1);
    glBindBuffer(GL_COPY_WRITE_BUFFER, vbo2);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(vertexData));
    我们也可以只将writetarget缓冲绑定为新的缓冲目标类型之一：

    float vertexData[] = { ... };
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBindBuffer(GL_COPY_WRITE_BUFFER, vbo2);
    glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(vertexData));
    */
    return 0;
}

