#version 410
#extension GL_ARB_separate_shader_objects : enable

uniform vec3 color = vec3(1.0);

layout(location = 0) out vec4 out_color;

void main(void)
{
    out_color = vec4(color, 1.0);
}
