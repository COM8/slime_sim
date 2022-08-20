#version 450 core

in vec2 fTexCoordinates;

out vec4 outColor;

uniform sampler2D slimeTexture;
uniform vec2 textureSize;
uniform vec2 screenSize;

void main()
{
    float textureAspectRatio = textureSize.x / textureSize.y;
    float screenAspectRatio = screenSize.x / screenSize.y;
    float textureScreenRatio = textureAspectRatio / screenAspectRatio;
    float scaleX = 1.0;
    float scaleY = 1.0;

    if(screenAspectRatio < 1.0) {
        scaleY = textureScreenRatio;
    }
    else {
        scaleX = 1.0 / textureScreenRatio;
    }

    // Get texture coordinates and colors:
    vec2 scaledCoords = fTexCoordinates * vec2(scaleX, scaleY);
    vec4 texSlimeColor = texture(slimeTexture, scaledCoords);

    // Layer textures:
    outColor += texSlimeColor;
}