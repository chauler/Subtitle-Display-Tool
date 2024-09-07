#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
uniform float fontSize;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
    vec4 text_color = vec4(1.0, 1.0, 1.0, 1.0);
    float strength = 1.0;
    float pxRange = 8.0;
    vec2 coords = fragTexCoord;

    /* Invert the strength so that 1.0 becomes bold and 0.0 becomes thin */
    float threshold = 0.5;

    vec2 msdfUnit = pxRange/vec2(textureSize(texture0, 0));
    vec3 s = texture(texture0, coords).rgb;
    float sigDist = median(s.r, s.g, s.b) - threshold;
    sigDist *= dot(msdfUnit, 0.5/fwidth(coords));
    float opacity = clamp(sigDist + 0.5, 0.0, 1.0);
    finalColor = mix(vec4(0.0, 0.0, 0.0, 0.0), text_color, opacity);
}