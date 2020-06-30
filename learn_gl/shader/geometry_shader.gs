#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

out vec3 aFragColor;

const float MAGNITUDE = 0.2;

void GenerateLine(int index)
{
	aFragColor = vec3(1.0,1.0,0.0);
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();

	aFragColor = vec3(0.0,1.0,0.0);
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE;
    EmitVertex();

    EndPrimitive();
}

void main()
{
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}