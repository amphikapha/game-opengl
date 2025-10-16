#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D diffuseTexture;

void main() {
    vec3 texColor = texture(diffuseTexture, TexCoords).rgb;

    vec3 ambient = 0.1 * lightColor * texColor;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * texColor;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfway = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfway), 0.0), 32.0);
    vec3 specular = spec * lightColor * 0.5;
    vec3 color = ambient + diffuse + specular;
    FragColor = vec4(color, 1.0);
}
