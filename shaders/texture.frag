#version 330

out vec4 gl_FragColor;

in vec3 ex_Color;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
   gl_FragColor = texture(ourTexture, TexCoord);
}