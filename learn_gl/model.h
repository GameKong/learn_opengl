#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shader_s.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

// 加载文件对应的纹理到OpenGL纹理对象中
unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model
{
public:
    /*  Model Data */
   
    // 加载过的纹理存储在这个容器中。用于判断是否已经加载过
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    
    // 该模型包含的所有网格对象
    vector<Mesh> meshes;

    // 模型文件的路径，用于查找纹理文件。所以.obj文件必须与纹理文件同路径
    string directory;

    // 伽马矫正
    bool gammaCorrection;

    /*  Functions   */
    // constructor, expects a filepath to a 3D model. 模型类的构造函数
    Model(string const &path, bool gamma = false) : gammaCorrection(gamma)
    {

        // 读取模型文件到内存中
        loadModel(path);
    }

    // draws the model, and thus all its meshes
    // 绘制模型。内部遍历模型的所以网格对象，然后绘制每一个网格对象
    void Draw(Shader shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

private:
    /*  Functions   */
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    // 加载模型
    void loadModel(string const &path)
    {
        // read file via ASSIMP
        // 第三方库加载.obj模型文件
        Assimp::Importer importer;

        // 读取模型文件，设置读取时的参数，例如把所有网格都以三角形的方式读取
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
      
        // check for errors
        // 检查是否读取成功。1是否成功创建场景对象。2.场景flag是否为未完成。3.场景是否没有根节点
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // retrieve the directory path of the filepath
        // 获取模型文件路径
        directory = path.substr(0, path.find_last_of('/'));

        // process ASSIMP's root node recursively
        // 从根节点开始 递归遍历所有子节点。获取各个节点的网格数据
        processNode(scene->mRootNode, scene);
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene)
    {
        // process each mesh located at the current node
        // 获取给定节点的网格数量
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            // 根据节点的网格索引，在场景中找到对应的ai网格数据
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

            // 解析aiMesh网格数据为我们自己的Mesh对象，然后加入到模型类的mesh容器中
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            // 递归遍历该节点的所有子节点
            processNode(node->mChildren[i], scene);
        }

    }

    // 解析ai网格数据为我们自自定义的网格对象
    Mesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        // data to fill
        // 顶点数据数组、索引数据数组、纹理数据数组
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        // Walk through each of the mesh's vertices 
        // 遍历网格顶点
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions ai网格顶点数据转换为自定义mesh成员
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals ai网格法线数据转换为自定义mesh成员
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            // texture coordinates 
            // 一个ai网格可能存在多组纹理坐标。我们暂时只需要第一组，先判断是否至少有一组纹理坐标数据。然后将ai网格纹理数据转换为自定义mesh成员
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            // tangent ai网格切线数据转换为自定义mesh成员
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent ai网格双切线数据转换为自定义mesh成员
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;

            // 将配置好的该顶点数据加入到顶点容器中。 该容器则容纳了该网格对象所需的所有顶点数据
            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        // 一个ai网格对象有多个面存在面的数组中。每个面就是一个图元。绘制的时候以面为最小单位。每个面有多个顶点 存储在面索引数组中
        // 网格的遍历所有面
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            // 遍历每个面的所有索引加入到自定义网格对象的索引数组中
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        // 每个网格存储了一个材质索引。通过该材质索引在ai场景对象中找到该材质对象
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps 通过材质、类型加载该材质相应类型的纹理 加入到自定义mesh对象的纹理数组中
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        // return a mesh object created from the extracted mesh data
        // 根据配置好的 顶点数组数据、索引数组数据、纹理数组数据构造一个自定义网格对象
        return Mesh(vertices, indices, textures);
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    // 解析材质中的纹理，并且加载纹理
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;

        // 遍历材质的所有特定类型的纹理
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;

            // 给定类型和索引 获取材质特定类型的纹理 将文件名存储在str中
            mat->GetTexture(type, i, &str);

            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            // 判断一下 是否已经加载过该纹理文件
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }

            // 如果没有加载过则加载该纹理文件
            if (!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();

                // 将配置好的纹理对象加入到纹理数组中。返回给mesh对象
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }
};

#ifdef MODEL_FUNC_DEF
// 加载纹理文件，创建并返回OpenGL纹理对象
unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
    string filename = string(path);

    // 纹理资源与模型资源在同一路径
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
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
#endif

#endif