#include "Object2D.h"

#include <Core/Engine.h>

Mesh* Object2D::CreateTriangle(std::string name, glm::vec3 center, float length, glm::vec3 color, bool fill)
{
	glm::vec3 ship_center = center;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(ship_center, color),
		VertexFormat(ship_center + glm::vec3(length/2, length/2, 0), color),
		VertexFormat(ship_center + glm::vec3(-length / 2, length/2, 0), color),
		VertexFormat(ship_center, color),
		VertexFormat(ship_center + glm::vec3(-length / 2,- length / 2, 0), color),
		VertexFormat(ship_center + glm::vec3(length/2, -length/2, 0), color)
		
	};

	Mesh* triangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2};
	
	if (!fill) {
		triangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(3);
		indices.push_back(4);
		indices.push_back(5);
	}

	triangle->InitFromData(vertices, indices);
	return triangle;
}

Mesh* Object2D::CreateProjectile(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner + glm::vec3(length / 8, -length / 8, 0), color),
		VertexFormat(corner + glm::vec3(length / 8, length/8, 0), color),
		VertexFormat(corner + glm::vec3(length, length / 8, 0), color),
		VertexFormat(corner + glm::vec3(length, -length / 8, 0), color)

	};

	Mesh* projectile = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2 ,3};

	if (!fill) {
		projectile->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	projectile->InitFromData(vertices, indices);
	return projectile;
}