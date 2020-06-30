#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);

	// 顶点着色器之后会进行透视除法。顶点各个分量会除以w分量。然后得到的z分量就是最终进行深度测试用的 顶点深度值。
	// 所以要想天空盒在最外侧就 强行设置z值和w值相等。透视除法之后得到的深度值为最大值1.同时在cpp设置glDepthFunc(GL_LEQUAL)。
    gl_Position = pos.xyww;
}  