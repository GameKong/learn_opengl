#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader_s.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

// 顶点数据结构体
struct Vertex 
{
    // 顶点位置
    glm::vec3 Position;
    
    // 法线
    glm::vec3 Normal;

    // 纹理坐标
    glm::vec2 TexCoords;

    // 切线
    glm::vec3 Tangent;

    // 双切线
    glm::vec3 Bitangent;

};

// 纹理结构体
struct Texture 
{
    // 纹理id
    unsigned int id;

    // 类型，漫反射、镜面反射等
    string type;

    // 纹理文件路径，用于判断是否已经加载过文件，优化
    string path;
};

// 网格类，一个网格包括多个面。每个面就是一个图元（例如三角形），一个图元包含多个点（用于绘制的顶点）
class Mesh {
public:
    /*  Mesh Data  网格数据*/

    // 顶点数组
    vector<Vertex> vertices;

    // 要绘制的顶点索引数据
    vector<unsigned int> indices;
    
    // 纹理数据
    vector<Texture> textures;

    // 该网格对应的顶点数组对象
    unsigned int VAO;

    /*  Functions  */
    // constructor 构造函数
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        // now that we have all the required data, set the vertex buffers and its attribute pointers. 
        // 利用顶点、索引、纹理数据简历OpenGL对象。顶点数组对象、配置顶点属性指针
        setupMesh();
    }

    // render the mesh
    void Draw(Shader shader)
    {
        // bind appropriate textures 初始化所有纹理的起始个数索引。通过遍历纹理数组用于确定uniform Samplar2D的 变量名
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;

        // 每个网格对应的素材可能包含多个纹理，绘制的时候需要根据纹理信息进行相应的绘制。
        // 以下同个分类材质的不同类型的纹理 命名后给 片段着色器的 采样器设值
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            // 先激活相应的纹理单元
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to stream
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to stream
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to stream
            
            // now set the sampler to the correct texture unit 设置采样器对应的纹理单元
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // and finally bind the texture 将纹理绑定到相关的纹理单元上
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

private:
    /*  Render data  */
    unsigned int VBO, EBO;

    /*  Functions    */
    // initializes all the buffer objects/arrays
    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
       
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals  //offsetof(类型名，成员名）可以得到成员相对于结构体起始位置的字节偏移量
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

        glBindVertexArray(0);
    }
};
#endif