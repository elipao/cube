#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform sampler2D heightmap;
uniform float blendFactor; // Blend factor as a uniform

void main()
{
    vec4 skyboxColor = texture(skybox, TexCoords);
    vec4 heightmapColor = texture(heightmap, TexCoords.xz); // Access the heightmap
    
    // Check if the TexCoords.y indicates the bottom face
    if (TexCoords.y < 0.0) // Modify this condition as needed
    {
        FragColor = mix(skyboxColor, heightmapColor, blendFactor); // Blend the two textures
    }
    else
    {
        FragColor = skyboxColor;
    }
}
