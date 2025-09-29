#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;
uniform vec3 overrideColor; // optional per-instance color

void main()
{
    // small vertex displacement for 'breathing' effect (optional)
    vec3 pos = aPos;
    float disp = sin(time*3.0 + aPos.x*4.0 + aPos.y*3.0) * 0.02;
    pos += normalize(aNormal) * disp;

    vec4 worldPos = model * vec4(pos, 1.0);
    FragPos = vec3(worldPos);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * worldPos;
}
