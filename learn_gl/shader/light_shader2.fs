#version 330 core
out vec4 FragColor;
  
in vec3 FragPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	// �������շ��� = ��������ϵ��*��Դ
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// ��������� = ��Դ����������� �붥�㷨������ ֮��ļ�ϵ�� * ��Դ
	vec3 lightVec = normalize(lightPos - FragPos);
	vec3 norm = normalize(Normal);
    float difusseStrength = max(dot(lightVec,norm),0.0);
	vec3 difusse = difusseStrength * lightColor;

	// ���淴����� = ���淴��ϵ�� * �۲�㵽��������� �� �������� ֮��ļн�ϵ�� * ��Դ
	vec3 reflectVec = normalize(reflect(-lightVec,norm));
	vec3 viewVec = normalize(viewPos - FragPos);

	// cos��32���ݣ�32������ǿ�ȡ�����ǿ��Խ�󣬷����������ԽС
	float spec = pow(max(dot(reflectVec,viewVec),0),32);
	float specularStrength = 0.5;
	vec3 specular = specularStrength * spec * lightColor;

	// ���������ֱ���������ɫ��� �ĺ� Ϊ�����ڳ��ֵ���ɫ
	vec3 result = (ambient + difusse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}