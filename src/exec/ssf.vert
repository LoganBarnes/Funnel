#version 410
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 world_position;

uniform mat4 screen_from_world = mat4(1.0);

out gl_PerVertex
{
  vec4 gl_Position;
};

void main(void)
{
    gl_Position = screen_from_world * vec4(world_position, 0.0, 1.0);
}
