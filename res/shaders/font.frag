#version 330 core

uniform sampler2D textTexture;
in vec2 texCoord;
out vec4 fragColor;

vec4 renderSDF(vec2 texCoord) {
    float sdfValue = texture(textTexture, texCoord).r;
    float distance = sdfValue * 2.0 - 1.0; // Convert from [0, 1] to [-1, 1]
    float smoothFactor = 0.25;             // Width of the transition edge
    float alpha = smoothstep(-smoothFactor, smoothFactor, distance);
    return vec4(texture(textTexture, texCoord).rgb, alpha);
}

vec4 renderNonSDF(vec2 texCoord) {
    vec4 color = texture(textTexture, texCoord);
    return vec4(color.rgb, color.a);
}

void main() {
    fragColor = renderNonSDF(texCoord);
}
