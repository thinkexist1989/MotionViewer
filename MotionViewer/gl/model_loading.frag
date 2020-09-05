#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

in vec3 FragPos;
in vec3 Normal;

void main()
{
    //FragColor = texture(texture_diffuse1, TexCoords);
    vec3 lightPos = vec3(12f, 10f, 20f);

    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * vec3(0.7f, 0.7f, 0.7f);
    FragColor = vec4(result, 1.0);
}
