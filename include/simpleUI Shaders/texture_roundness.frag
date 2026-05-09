#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform float roundness;

out vec4 finalColor;

void main()
{
    vec2 uv = fragTexCoord;

    float radius = roundness * 0.5;

    vec2 q = abs(uv - 0.5) - vec2(0.5 - radius);
    float dist = length(max(q, 0.0)) - radius;

    float aa = fwidth(dist);

    float alpha = 1.0 - smoothstep(0.0, aa, dist);

    vec4 tex = texture(texture0, uv) * fragColor;
    tex.a *= alpha;

    finalColor = tex;
}