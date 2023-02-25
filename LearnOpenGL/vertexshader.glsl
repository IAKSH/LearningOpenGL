#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;
out vec4 ourPos;

uniform vec2 xyoffset;
uniform mat4 transform;

void main()
{
    vec4 temp = transform * vec4(aPos,1.0);
    gl_Position = vec4(temp.x + (xyoffset.x / 2),temp.y + (xyoffset.y / 2),temp.z,1.0);
    ourPos = gl_Position;
    ourColor = aColor;
    TexCoord = aTexCoord;
}