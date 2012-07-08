#version 330 core

in vec2 UV;
in vec3 WorldPos, normal;

uniform sampler2D TextureSampler;

layout (location = 0) out vec3 WorldPos_out;
layout (location = 1) out vec3 Diffuse;
layout (location = 2) out vec3 Normal;
layout (location = 3) out vec3 TexCoord;

void main()
{
    WorldPos_out = WorldPos;
    Diffuse = texture2D(TextureSampler, UV).rgb;
    Normal = normalize(normal);
    TexCoord = vec3(UV, 0.0);

}

