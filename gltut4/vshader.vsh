#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 myColor;
out vec2 myTexCoord;

uniform mat4 transform;
void main(){
    gl_Position = transform * vec4(aPos, 1.0f);
    myColor = aColor;
    myTexCoord = aTexCoord;
}
