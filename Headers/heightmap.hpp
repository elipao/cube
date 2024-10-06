#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

#include <shader.hpp>

// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
#include <stb_image.h>

struct Vertex {
	// position
	glm::vec3 Position;
	// texCoords
	glm::vec2 TexCoords;
};

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Heightmap
{
public:
	//heightmap attributes
	int width, height;

	// VAO for heightmap
	unsigned int VAO;

	// pointer to data - data is an array so can be accessed by data[x]. 
	//       - this is an uint8 array (so values range from 0-255)
	unsigned char *data;

	// heightmap vertices
	std::vector<Vertex> vertices;
	// indices for EBO
	std::vector<unsigned int> indices;


	// constructor
	Heightmap(const char* heightmapPath)
	{
		// load heightmap data
		load_heightmap(heightmapPath);

		// create heightmap verts from the image data - (you have to write this)
		create_heightmap();

		// free image data
		stbi_image_free(data);

		// create_indices - create the indices array (you have to write this)
		create_indices();

		// setup the VBO, VAO, and EBO and send the information to OpenGL (you need to write this)
		setup_heightmap();
	}

	// render the heightmap mesh (you need to write this)
	void Draw(Shader shader, unsigned int textureID)
	{
		// You must:
		// -  active proper texture unit before binding
		glActiveTexture(GL_TEXTURE0);
        // Bind the texture
		glBindTexture(GL_TEXTURE_2D, textureID);
		// Use the shader program
		shader.use();
		// Bind the VAO
		glBindVertexArray(VAO);
		// Draw the heightmap
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		// always good practice to set everything back to defaults once configured.
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);

		// always good practice to set everything back to defaults once configured.
		glActiveTexture(GL_TEXTURE0);
	}

private:

	unsigned int VBO, EBO;

	// Load the image data
	void load_heightmap(const char* heightmapPath)
	{
		int nrChannels;
		data = stbi_load(heightmapPath, &width, &height, &nrChannels, 0);
		if (!data)
		{
			std::cout << "Failed to load heightmap" << std::endl;
		}

		//  Data is in a char c-array and can be access via  
		//           float(data[x*height + y]) / 255.0f 
		//    where x and y are varables between 0 and width or height  (just use a black and white image)
	}


	Vertex make_vertex(int x, int y)
	{
		Vertex v;
		// XYZ coords
		v.Position.x = (float)x;
		v.Position.y = (float)(*(data + y * width + x)) / 255.0f; // height from heightmap
		v.Position.z = (float)y;

		// Texture Coords
		v.TexCoords.x = (float)x / (float)(width - 1);
		v.TexCoords.y = (float)y / (float)(height - 1);

		return v;
	}
	

	// convert heightmap to floats, set position and texture vertices using the subfunction make_vertex
	void create_heightmap()
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				vertices.push_back(make_vertex(x, y));
			}
		}
	}


	// create the indicies array for the EBO (so what indicies correspond with triangles to for rendering)
	void create_indices()
	{
		for (int y = 0; y < height - 1; y++)
		{
			for (int x = 0; x < width - 1; x++)
			{
				unsigned int topLeft = y * width + x;
				unsigned int topRight = topLeft + 1;
				unsigned int bottomLeft = (y + 1) * width + x;
				unsigned int bottomRight = bottomLeft + 1;

				// Two triangles per square
				indices.push_back(topLeft);
				indices.push_back(bottomLeft);
				indices.push_back(topRight);

				indices.push_back(topRight);
				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);
			}
		}


	}


	// create buffers/arrays for the VAO, VBO, and EBO 
	// Notes
	//  -  sizeof(Vertex) returns the size of the vertex
	//  -  to get the pointer the underlying array, you can use "&vertices[0]"
	void setup_heightmap()
	{

		// Generate and bind the VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// Generate and bind the VBO
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		// Generate and bind the EBO
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// Set vertex attributes
		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);
		// TexCoords
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, TexCoords)));
		glEnableVertexAttribArray(1);

		// Unbind the VAO
		glBindVertexArray(0);
		

	}

};
#endif
