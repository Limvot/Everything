#version 330 core

layout (location = 0) in vec3 position_modelspace;
layout (location = 1) in vec3 normal_modelspace;
layout (location = 2) in vec2 UV_in;

uniform mat4 M, V, MVP;
uniform vec3 lightPosition_worldspace, LightColor_in;
uniform float LightPower_in;

out vec2 UV;
out vec3 WorldPos, normal;


void main() {
                                          //Output the position of the vertex in clip space\
    gl_Position = MVP * vec4(position_modelspace, 1);
    WorldPos = M * vec4(position_modelspace, 1);
    UV = UV_in;                                                                                   //Pass the UVs through
    vec3 normal_cameraspace = (V * M * vec4(normal_modelspace,0)).xyz;                           //Here we transform everything to camera space
    normal = normalize(normal_cameraspace);                                                           //Pass the normalized vectors through
}
