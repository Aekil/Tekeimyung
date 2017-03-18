#extension GL_ARB_shader_storage_buffer_object : require

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec2 inTexCoords;

layout (location = 0) out vec3 fragNormal;
layout (location = 1) out vec2 fragTexCoords;
layout (location = 2) out vec3 fragPos;
layout (location = 3) flat out uint instanceID;

layout (std140, binding = 0) uniform materialUniformBlock
{
    vec4    ambient;
    vec4    diffuse;
    int     texturesTypes;
    int     faceCamera;
} material;

layout (std140, binding = 1) uniform cameraUniformBlock
{
    mat4 proj;
    mat4 view;
    vec3 pos;
    vec3 dir;
} camera;

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
    mat4 modelView = camera.view * model[gl_InstanceID].transform;

    #ifdef FACE_CAMERA
        float d = sqrt(pow(modelView[0][0], 2) + pow(modelView[1][1], 2) + pow(modelView[2][2], 2));
        // Column 0:
        modelView[0][0] = d;
        modelView[0][1] = 0;
        modelView[0][2] = 0;

        // Column 1:
        modelView[1][0] = 0;
        modelView[1][1] = d;
        modelView[1][2] = 0;

        // Column 2:
        modelView[2][0] = 0;
        modelView[2][1] = 0;
        modelView[2][2] = d;

        fragNormal = -camera.dir;
    #else
        fragNormal = mat3(transpose(inverse(model[gl_InstanceID].transform))) * inNormal;
    #endif

    gl_Position = camera.proj * modelView * vec4(inPosition, 1.0);
    fragTexCoords = inTexCoords;
    fragPos = vec3(model[gl_InstanceID].transform * vec4(inPosition, 1.0));

    instanceID = gl_InstanceID;
}
