#version 330 core
out vec4 FragColor;

// ����
struct Material{
	sampler2D diffuse; // ��������ʲ�����
	sampler2D specular; // ���淴�������
	float shininess; // ���淴��ǿ��

};

// ƽ�й� ���Դ Ͷ���Դ ���ýṹ��
struct Light {
	vec3 position; // ��Դλ��
	vec3 direction; // ��Դ����ķ������ƽ�йⷽ��

	float cutOff;	// ���й��cos
	float outerCutOff; // ���й��cos

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant; // ˥������
	float linear;// ˥��һ����
	float quadratic;// ˥��������
};

#define PointLightNum 4

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform Light DirLight;
uniform Light PoinLights[PointLightNum];
uniform Light SpotLight;
uniform vec3 viewPos;
uniform Material material;

// ����ƽ�й�������Ӱ��
vec3 calcDirLight(Light light, vec3 viewDir, vec3 normal);

// ������Դ�������Ӱ��
vec3 calcPointLight(Light light, vec3 FragPos, vec3 viewDir, vec3 normal);

// ����Ͷ���������Ӱ��
vec3 calcSpotLight(Light light, vec3 FragPos, vec3 viewDir, vec3 normal);

void main()
{
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 normal = normalize(Normal);

	vec3 result = calcDirLight(DirLight,viewDir, normal);

	for(int i = 0; i < PointLightNum; i++)
	{
		result += calcPointLight(PoinLights[i], FragPos, viewDir, normal);
	}

	result += calcSpotLight(SpotLight, FragPos, viewDir , normal);
	
	FragColor = vec4(result, 1.0);
}

vec3 calcDirLight(Light light, vec3 viewDir, vec3 normal)
{
	// ������
	vec3 ambientLight = light.ambient * vec3(texture(material.diffuse, TexCoords));
	
	// ������
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(lightDir,normal),0);
	vec3 diffuseLight = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// ���淴��
	vec3 reflectVec = normalize(reflect(-lightDir, normal));
	float spe = pow(max(dot(reflectVec , viewDir),0),material.shininess);
	vec3 specularLight = light.specular * spe * vec3(texture(material.specular, TexCoords));


	return ambientLight + diffuseLight + specularLight;
}

vec3 calcPointLight(Light light, vec3 FragPos, vec3 viewDir, vec3 normal)
{
	vec3 lightDir = normalize(light.position - FragPos);

	// ������
	vec3 ambientLight = light.ambient * vec3(texture(material.diffuse,TexCoords));

	// ������
	float diff = max(dot(normal,lightDir),0.0);
	vec3 diffuseLight = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));

	// ���淴��
	vec3 reflectVec = normalize(reflect(-lightDir,normal));
	float spe = pow(max(dot(viewDir,reflectVec),0.0),material.shininess);
	vec3 specularLight = light.specular * spe * vec3(texture(material.specular, TexCoords));

	// ��Դ��Ƭ�εľ���
	float distance = length(light.position - FragPos);

	// ˥��
	float attenuation = 1.0 /(light.constant + distance * light.linear + (distance*distance)* light.quadratic);

	return (ambientLight + diffuseLight + specularLight)*attenuation;
}
vec3 calcSpotLight(Light light, vec3 FragPos, vec3 viewDir, vec3 normal)
{
	vec3 lightDir = normalize(light.position - FragPos);

	// ������
	vec3 ambientLight = light.ambient * vec3(texture(material.diffuse,TexCoords));

	// ������
	float diff = max(dot(lightDir,normal),0.0);
	vec3 diffuseLight =light.diffuse *  diff * vec3(texture(material.diffuse,TexCoords));

	// ���淴��
	vec3 reflectVec = normalize(reflect(-lightDir,normal));
	float spe = pow(max(dot(viewDir,reflectVec),0.0),material.shininess);
	vec3 specularLight = light.specular * spe * vec3(texture(material.specular,TexCoords));

	// ��Դ��Ƭ�ξ���
	float  distance = length(light.position - FragPos);

	// ˥��
	float  attenuation = 1.0/(light.constant + distance * light.linear + (distance * distance) * light.quadratic);

	// ��Դֱ�Խ��� ��Դ��Ƭ�ε������ĽǶ�
	float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	ambientLight *= attenuation * intensity;
	diffuseLight *= attenuation * intensity;
	specularLight *= attenuation * intensity;

	return (ambientLight + diffuseLight + specularLight);
}