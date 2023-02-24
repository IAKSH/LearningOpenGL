#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec4 ourPos;
in vec2 TexCoord;
uniform sampler2D texture0;
uniform sampler2D texture1;
void main()
{
    FragColor = mix(texture(texture0,TexCoord),texture(texture1,TexCoord),0.5) * vec4(ourColor.x - (ourPos.x / 2.0),ourColor.y - (ourPos.y / 2.0),ourColor.z - (ourPos.z / 1.5), 0.1);
}