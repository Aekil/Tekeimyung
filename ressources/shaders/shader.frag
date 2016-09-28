#version 330 core

in vec4 outColor;
in vec2 outTexCoords;
in vec3 outPosition;

out vec4 color;

uniform sampler2D spriteTexture;

void main()
{
    color = vec4(1.0f, 1.0f, 1.0f, 0.0f);
    //color = texture(spriteTexture, outTexCoords);

   //if (color.a <= 0.1)
      //discard;

    //gl_FragDepth = outPosition.y / 50000.0f; // Divide by a big value to make fragment depth between 0 and 1
}