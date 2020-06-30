#version 330 core
out vec4 FragColor;
  
in vec3 FragPos;
in vec3 Normal;
in vec2 TextureCoord;

struct Material 
{
    sampler2D diffuse;
    sampler2D specular;    
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
uniform bool blinn;

void main()
{
	// 环境光照分量 
	vec3 ambient = light.ambient * texture(material.diffuse, TextureCoord).rgb;

	// 漫反射分量 = 光源到顶点的向量 与顶点法线向量 之间的夹系数 * 光源
	vec3 lightVec = normalize(light.position - FragPos);
	vec3 norm = normalize(Normal);
    float difusseStrength = max(dot(lightVec,norm),0.0);
	vec3 diffuse = difusseStrength * light.diffuse * texture(material.diffuse, TextureCoord).rgb;

	// 镜面反射分类 = 镜面反射系数 * 观察点到顶点的向量 与 反射向量 之间的夹角系数 * 光源
	vec3 reflectVec = normalize(reflect(-lightVec,norm));
	vec3 viewVec = normalize(viewPos - FragPos);

	float spec = pow(max(dot(reflectVec,viewVec),0),material.shininess);	// cos的32次幂，32代表反射强度。反射强度越大，反正高亮点就越小

	// blinn_phone 光照模型
	if(blinn)
    {
        vec3 halfwayDir = normalize(lightVec + viewVec);  
        spec = pow(max(dot(norm, halfwayDir), 0.0), 16.0);
    }
    else
    {
		// 冯氏光照模型
        vec3 reflectDir = reflect(-lightVec, norm);
        spec = pow(max(dot(viewVec, reflectDir), 0.0), 8.0);
    }

	
	vec3 specular = spec * light.specular * texture(material.specular, TextureCoord).rgb;

	// 三个分量分别于物体颜色点乘 的和 为最终于呈现的颜色
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}