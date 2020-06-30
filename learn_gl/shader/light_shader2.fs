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
	// 环境光照分量 = 环境光照系数*光源
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// 漫反射分量 = 光源到顶点的向量 与顶点法线向量 之间的夹系数 * 光源
	vec3 lightVec = normalize(lightPos - FragPos);
	vec3 norm = normalize(Normal);
    float difusseStrength = max(dot(lightVec,norm),0.0);
	vec3 difusse = difusseStrength * lightColor;

	// 镜面反射分类 = 镜面反射系数 * 观察点到顶点的向量 与 反射向量 之间的夹角系数 * 光源
	vec3 reflectVec = normalize(reflect(-lightVec,norm));
	vec3 viewVec = normalize(viewPos - FragPos);

	// cos的32次幂，32代表反射强度。反射强度越大，反正高亮点就越小
	float spec = pow(max(dot(reflectVec,viewVec),0),32);
	float specularStrength = 0.5;
	vec3 specular = specularStrength * spec * lightColor;

	// 三个分量分别于物体颜色点乘 的和 为最终于呈现的颜色
	vec3 result = (ambient + difusse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}