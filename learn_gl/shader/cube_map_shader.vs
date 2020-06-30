#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);

	// ������ɫ��֮������͸�ӳ���������������������w������Ȼ��õ���z�����������ս�����Ȳ����õ� �������ֵ��
	// ����Ҫ����պ��������� ǿ������zֵ��wֵ��ȡ�͸�ӳ���֮��õ������ֵΪ���ֵ1.ͬʱ��cpp����glDepthFunc(GL_LEQUAL)��
    gl_Position = pos.xyww;
}  