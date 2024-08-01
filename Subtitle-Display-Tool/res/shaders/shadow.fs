#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec2 samplerSize;
uniform vec3 color;
uniform ivec2 offset;

// Output fragment color
out vec4 finalColor;

void main()
{
    vec4 texel = texture(texture0, fragTexCoord);   // Get texel color
    
    int radius = 10;
    float pi = 3.1415926;
    float sigma = 5.;

    vec4 gaussSum = vec4(0.);
    
    for(int x = -radius; x <= radius; x++){
        for(int y = -radius; y <= radius; y++){
            vec2 newUV = vec2(gl_FragCoord.xy + ivec2(x-offset.x,y+offset.y))/samplerSize.xy;
            vec4 newTexCol = texture(texture0, newUV);
            gaussSum += texture(texture0, newUV) * (exp(-(pow(float(x), 2.) + pow(float(y), 2.)) / (2. * pow(sigma, 2.))) / (2. * pi * pow(sigma, 2.)));
        }   
    }
    finalColor = vec4(color, gaussSum.w);
}