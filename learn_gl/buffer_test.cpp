#include <glad/glad.h>
#include <GLFW/glfw3.h>

int buffer_test()
{
    /*
        //���ƶ������ݵ�ָ���ڴ�
        float data[18];
        float data2[18];
        int vbo;
        glGenBuffer(1,&vbo);
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBufferData(GL_ARRAY_BUFFER,sizeof(data),&data[0], GL_STATIC_DRAW);
        
        // ��ȡĿ�굱ǰ�󶨵Ļ����ַ��Ȼ��д������
        void* ptr = glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
        memcpy(ptr,data2,sizeof(data2));
        glUnmapBuffer(GL_ARRAY_BUFFER);
    */

    /*
    //������ݵ������ָ��λ�ã��ṩһ��ƫ����  glBufferSubData(GL_ARRAY_BUFFER,10,sizeof(data2),&data2);

    float positions[] = { ... };
    float normals[] = { ... };
    float tex[] = { ... };
    // ��仺��
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), &positions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(normals), &normals);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(normals), sizeof(tex), &tex);
    ���������Ǿ���ֱ�ӽ�����������Ϊһ�����崫�ݸ����壬������Ҫ���ȴ��������ˡ������Կ��Խ����Ǻϲ�Ϊһ��������飬��ʹ��glBufferData����仺�壬���������ֹ�����ʹ��glBufferSubData�������һ�㡣

    ���ǻ���Ҫ���¶�������ָ������ӳ��Щ�ı䣺

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(positions)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(positions) + sizeof(normals)));
   
    */

    /*
      //���ƶ������ݵ�ָ���ڴ�
    void glCopyBufferSubData(GLenum readtarget, GLenum writetarget, GLintptr readoffset,
                         GLintptr writeoffset, GLsizeiptr size);

    float vertexData[] = { ... };                         
    glBindBuffer(GL_COPY_READ_BUFFER, vbo1);
    glBindBuffer(GL_COPY_WRITE_BUFFER, vbo2);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(vertexData));
    ����Ҳ����ֻ��writetarget�����Ϊ�µĻ���Ŀ������֮һ��

    float vertexData[] = { ... };
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBindBuffer(GL_COPY_WRITE_BUFFER, vbo2);
    glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(vertexData));
    */
    return 0;
}

