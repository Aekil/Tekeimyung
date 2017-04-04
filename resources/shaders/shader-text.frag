#extension GL_ARB_shader_storage_buffer_object : require

layout (location = 0) in vec3 fragNormal;
layout (location = 1) in vec2 fragTexCoords;
layout (location = 2) in vec3 fragPos;
layout (location = 3) flat in uint instanceID;

layout (location = 0) out vec4 outFragColor;
layout (location = 1) out vec4 outBrightColor;

uniform sampler2D textImage;

struct Model
{
    mat4 transform;
    vec4 color;
};

layout (std430, binding = 3) buffer modelUniformBlock
{
    Model model[];
};

void main()
{
    vec4 color = vec4(1.0f, 1.0f, 1.0f, texture(textImage, fragTexCoords).r);

    outFragColor = color * model[instanceID].color;
    // Don't bloom text
    outBrightColor = vec4(0.0);
}
