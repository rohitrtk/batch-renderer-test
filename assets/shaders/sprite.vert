#version 410 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec4 a_Colour;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec2 v_TexCoord;
out vec4 v_Colour;

void main() {
  v_TexCoord = a_TexCoord;
  v_Colour = a_Colour;

  gl_Position = u_Projection * u_View * vec4(a_Position, 1.0);
}