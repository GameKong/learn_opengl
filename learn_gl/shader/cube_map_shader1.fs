#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{    
    vec3 I = normalize(Position - cameraPos);

	// 反射向量
    vec3 Rl = reflect(I, normalize(Normal));

	// 折射向量
	float ratio = 1.0f/1.52f;
	vec3 Rr = refract(I,normalize(Normal),ratio);

    FragColor = vec4(texture(skybox, Rr).rgb, 1.0);
}