#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform float roundness;
uniform vec4 objectData; // destRec: x, y, width, height
uniform vec4 imageData;  // srcRec: x, y, width, height

out vec4 finalColor;

void main() {
    vec2 texSize = vec2(textureSize(texture0, 0));
    vec2 localUV = (fragTexCoord * texSize - imageData.xy) / imageData.zw;
    localUV = abs(localUV);

    vec2 size = objectData.zw;
    vec2 localPos = localUV * size;
    vec2 center = size * 0.5;

    float maxRadius = min(size.x, size.y) * 0.5;
    float radius = roundness * maxRadius;

    vec2 d = abs(localPos - center);
    vec2 q = d - center + vec2(radius);
    float dist = length(max(q, 0.0)) - radius;

    float aa = fwidth(dist);
    float alpha = 1.0 - smoothstep(0.0, max(aa, 0.5), dist);

    vec4 tex = texture(texture0, fragTexCoord) * fragColor;
    tex.a *= alpha;

    finalColor = tex;
}