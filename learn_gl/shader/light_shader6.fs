#version 330 core
out vec4 FragColor;

// 材质
struct Material{
	sampler2D diffuse; // 漫反射材质采样器
	sampler2D specular; // 镜面反射采样器
	float shininess; // 镜面反射强度

};

// 平行光 点光源 投射光源 共用结构体
struct Light {
	vec3 position; // 光源位置
	vec3 direction; // 光源照向的方向或者平行光方向

	float cutOff;	// 内切光角cos
	float outerCutOff; // 外切光角cos

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant; // 衰减常量
	float linear;// 衰减一次项
	float quadratic;// 衰减二次项
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

// 计算平行光对物体的影响
vec3 calcDirLight(Light light, vec3 viewDir, vec3 normal);

// 计算点光源对物体的影响
vec3 calcPointLight(Light light, vec3 FragPos, vec3 viewDir, vec3 normal);

// 计算投射光对物体的影响
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
	// 环境光
	vec3 ambientLight = light.ambient * vec3(texture(material.diffuse, TexCoords));
	
	// 漫反射
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(lightDir,normal),0);
	vec3 diffuseLight = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// 镜面反射
	vec3 reflectVec = normalize(reflect(-lightDir, normal));
	float spe = pow(max(dot(reflectVec , viewDir),0),material.shininess);
	vec3 specularLight = light.specular * spe * vec3(texture(material.specular, TexCoords));


	return ambientLight + diffuseLight + specularLight;
}

vec3 calcPointLight(Light light, vec3 FragPos, vec3 viewDir, vec3 normal)
{
	vec3 lightDir = normalize(light.position - FragPos);

	// 环境光
	vec3 ambientLight = light.ambient * vec3(texture(material.diffuse,TexCoords));

	// 漫反射
	float diff = max(dot(normal,lightDir),0.0);
	vec3 diffuseLight = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));

	// 镜面反射
	vec3 reflectVec = normalize(reflect(-lightDir,normal));
	float spe = pow(max(dot(viewDir,reflectVec),0.0),material.shininess);
	vec3 specularLight = light.specular * spe * vec3(texture(material.specular, TexCoords));

	// 光源与片段的距离
	float distance = length(light.position - FragPos);

	// 衰减
	float attenuation = 1.0 /(light.constant + distance * light.linear + (distance*distance)* light.quadratic);

	return (ambientLight + diffuseLight + specularLight)*attenuation;
}
vec3 calcSpotLight(Light light, vec3 FragPos, vec3 viewDir, vec3 normal)
{
	vec3 lightDir = normalize(light.position - FragPos);

	// 环境光
	vec3 ambientLight = light.ambient * vec3(texture(material.diffuse,TexCoords));

	// 漫反射
	float diff = max(dot(lightDir,normal),0.0);
	vec3 diffuseLight =light.diffuse *  diff * vec3(texture(material.diffuse,TexCoords));

	// 镜面反射
	vec3 reflectVec = normalize(reflect(-lightDir,normal));
	float spe = pow(max(dot(viewDir,reflectVec),0.0),material.shininess);
	vec3 specularLight = light.specular * spe * vec3(texture(material.specular,TexCoords));

	// 光源与片段距离
	float  distance = length(light.position - FragPos);

	// 衰减
	float  attenuation = 1.0/(light.constant + distance * light.linear + (distance * distance) * light.quadratic);

	// 光源直对角与 光源到片段的向量的角度
	float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	ambientLight *= attenuation * intensity;
	diffuseLight *= attenuation * intensity;
	specularLight *= attenuation * intensity;

	return (ambientLight + diffuseLight + specularLight);
}