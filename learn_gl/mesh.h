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

// �������ݽṹ��
struct Vertex 
{
    // ����λ��
    glm::vec3 Position;
    
    // ����
    glm::vec3 Normal;

    // ��������
    glm::vec2 TexCoords;

    // ����
    glm::vec3 Tangent;

    // ˫����
    glm::vec3 Bitangent;

};

// ����ṹ��
struct Texture 
{
    // ����id
    unsigned int id;

    // ���ͣ������䡢���淴���
    string type;

    // �����ļ�·���������ж��Ƿ��Ѿ����ع��ļ����Ż�
    string path;
};

// �����࣬һ�������������档ÿ�������һ��ͼԪ�����������Σ���һ��ͼԪ��������㣨���ڻ��ƵĶ��㣩
class Mesh {
public:
    /*  Mesh Data  ��������*/

    // ��������
    vector<Vertex> vertices;

    // Ҫ���ƵĶ�����������
    vector<unsigned int> indices;
    
    // ��������
    vector<Texture> textures;

    // �������Ӧ�Ķ����������
    unsigned int VAO;

    /*  Functions  */
    // constructor ���캯��
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        // now that we have all the required data, set the vertex buffers and its attribute pointers. 
        // ���ö��㡢�������������ݼ���OpenGL���󡣶�������������ö�������ָ��
        setupMesh();
    }

    // render the mesh
    void Draw(Shader shader)
    {
        // bind appropriate textures ��ʼ�������������ʼ����������ͨ������������������ȷ��uniform Samplar2D�� ������
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;

        // ÿ�������Ӧ���زĿ��ܰ�������������Ƶ�ʱ����Ҫ����������Ϣ������Ӧ�Ļ��ơ�
        // ����ͬ��������ʵĲ�ͬ���͵����� ������� Ƭ����ɫ���� ��������ֵ
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            // �ȼ�����Ӧ������Ԫ
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
            
            // now set the sampler to the correct texture unit ���ò�������Ӧ������Ԫ
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // and finally bind the texture ������󶨵���ص�����Ԫ��
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
        // vertex normals  //offsetof(����������Ա�������Եõ���Ա����ڽṹ����ʼλ�õ��ֽ�ƫ����
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