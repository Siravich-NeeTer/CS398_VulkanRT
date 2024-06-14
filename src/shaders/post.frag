
#version 450

layout(location = 0) out vec4 fragColor;


void main()
{
    vec2 uv = gl_FragCoord.xy/vec2(1280, 768);
    fragColor = vec4(uv, 0, 1);
}
