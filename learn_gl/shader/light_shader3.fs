#version 330 core
out vec4 FragColor;
  
in vec3 FragPos;
in vec3 Normal;

struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light 
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
	// �������շ��� 
	vec3 ambient = light.ambient * material.ambient;

	// ��������� = ��Դ����������� �붥�㷨������ ֮��ļ�ϵ�� * ��Դ
	vec3 lightVec = normalize(light.position - FragPos);
	vec3 norm = normalize(Normal);
    float difusseStrength = max(dot(lightVec,norm),0.0);
	vec3 diffuse = difusseStrength * light.diffuse * material.diffuse;

	// ���淴����� = ���淴��ϵ�� * �۲�㵽��������� �� �������� ֮��ļн�ϵ�� * ��Դ
	vec3 reflectVec = normalize(reflect(-lightVec,norm));
	vec3 viewVec = normalize(viewPos - FragPos);
	float spec = pow(max(dot(reflectVec,viewVec),0),material.shininess);	// cos��32���ݣ�32������ǿ�ȡ�����ǿ��Խ�󣬷����������ԽС
	vec3 specular = spec * light.specular * material.specular;

	// ���������ֱ���������ɫ��� �ĺ� Ϊ�����ڳ��ֵ���ɫ
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}