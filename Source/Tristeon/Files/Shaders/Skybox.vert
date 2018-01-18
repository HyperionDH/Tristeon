#version 450
#extension GL_ARB_separate_shader_objects : enable
layout(set = 0, binding = 0) uniform UniformBufferObject {
  mat4 model;
  mat4 view;
  mat4 proj;
} ubo;

out gl_PerVertex {
  vec4 gl_Position;
};

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec3 outPos;

void main()
{
  outPos = pos;
  mat4x4 view = ubo.view;
  view[3] = vec4(1, 0, 0, 0);

  vec4 p = ubo.proj * view * vec4(pos, 1);
  gl_Position = p.xyww;
}
