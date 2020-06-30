#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;

void main()
{
	// 将模型矩阵转换为 法线矩阵，防止不对称缩放，导致法线方向不对
	// 输出世界坐标系下的法线向量和片段位置
	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0f));

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}