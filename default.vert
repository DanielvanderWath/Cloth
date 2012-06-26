#version 130
in vec3 position;

uniform mat4 matrix;

void main(void)
{
	gl_Position = matrix * vec4(position, 1.0) ;

	gl_PointSize = 1.0;
}
