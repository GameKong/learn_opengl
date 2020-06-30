#version 330 core
out vec4 FragColor;

uniform vec2 viewSize;

void main()
{
	if(gl_FragCoord.x < viewSize.x/2 && gl_FragCoord.y < viewSize.y/2)
		FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	else if(gl_FragCoord.x > viewSize.x/2 && gl_FragCoord.y < viewSize.y/2)
		FragColor = vec4(0.0, 1.0, 0.0, 1.0);
	else if(gl_FragCoord.x < viewSize.x/2 && gl_FragCoord.y > viewSize.y/2)
		FragColor = vec4(0.0, 0.0, 1.0, 1.0);
	else if(gl_FragCoord.x > viewSize.x/2 && gl_FragCoord.y > viewSize.y/2)
		FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}