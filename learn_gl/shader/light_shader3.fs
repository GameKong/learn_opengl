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
	// 环境光照分量 
	vec3 ambient = light.ambient * material.ambient;

	// 漫反射分量 = 光源到顶点的向量 与顶点法线向量 之间的夹系数 * 光源
	vec3 lightVec = normalize(light.position - FragPos);
	vec3 norm = normalize(Normal);
    float difusseStrength = max(dot(lightVec,norm),0.0);
	vec3 diffuse = difusseStrength * light.diffuse * material.diffuse;

	// 镜面反射分类 = 镜面反射系数 * 观察点到顶点的向量 与 反射向量 之间的夹角系数 * 光源
	vec3 reflectVec = normalize(reflect(-lightVec,norm));
	vec3 viewVec = normalize(viewPos - FragPos);
	float spec = pow(max(dot(reflectVec,viewVec),0),material.shininess);	// cos的32次幂，32代表反射强度。反射强度越大，反正高亮点就越小
	vec3 specular = spec * light.specular * material.specular;

	// 三个分量分别于物体颜色点乘 的和 为最终于呈现的颜色
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}