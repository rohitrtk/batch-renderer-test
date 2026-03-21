#version 410 core

out vec4 FragColour;

in vec2 v_TexCoord;
in vec4 v_Colour;

uniform sampler2D u_Texture;

void main() {
  vec4 texColour = texture(u_Texture, v_TexCoord);

  if(texColour.a <= 0.01) {
    discard;
  }
  
  FragColour = texColour * v_Colour;
}
