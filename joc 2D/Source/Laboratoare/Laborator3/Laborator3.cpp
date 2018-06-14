#include "Laborator3.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"
#include <math.h>
#include <time.h>

using namespace std;

class Projectile;
class Enemy_ship;

float pozitiaX;
float pozitiaY;

int ok = 0;
bool game_over;

vector<Projectile*> projectiles;
vector<Enemy_ship*> enemy_ships;

Laborator3::Laborator3()
{
}

Laborator3::~Laborator3()
{
}

int my_hp;

void Laborator3::Init()
{
	my_hp = 3;
	game_over = false;

	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	pozitiaX = float( resolution.x ) / 2;
	pozitiaY = float( resolution.y ) / 2;

	glm::vec3 corner = glm::vec3(0, 0, 0);
	
	float squareSide = 75;
	// compute coordinates of square center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;

	srand(time(NULL));
	
	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;
	
	// initialize angularStep
	angularStep = 0;
	

	Mesh* triangle = Object2D::CreateTriangle("triangle", corner, squareSide, glm::vec3(1, 0.7, 0), true);
	AddMeshToList(triangle);

	Mesh* projectile = Object2D::CreateProjectile("projectile", glm::vec3(0, 0, 0), 25, glm::vec3(0, 1, 1), true);
	AddMeshToList(projectile);

	
	
	
}

void Laborator3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);  //1001 face ecranul rosu <----------------------------------------------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}
float scX = 1, scY = 1;
float rotatie = 0;

class Projectile {
public:
	float x, x0;
	float y, y0;
	float a;
	float dX, dY;
	bool dead;
	
	Projectile(float x0, float y0, float a) {
		
		this->x0 = x0;
		this->y0 = y0;
		this->x = x0;
		this->y = y0;
		this->a = a;
		this->dX = 800 * cos(a);
		this->dY = 800 * sin(a);
		this->dead = false;
		
	}
	void update(float dt) {
		this->x += dX * dt;
		this->y += dY * dt;
		
	}
	
};



class Enemy_ship {
public:
	float x, y;
	float a;
	bool dead;
	float scale;
	int hp;
	float speed;
	Mesh* mesh;
	bool hit;

	void handle_collisions() {
		for (Projectile *p : projectiles) {
			if (p->dead) continue;
			float distance = sqrt(pow(p->x - this->x, 2) + pow(p->y - this->y, 2));

			if (distance < 50) {
				hp--;
				if (hp <= 0)
					this->dead = true;
				else {
					this->mesh = Object2D::CreateTriangle("triangle_enemy", glm::vec3(0, 0, 0), 75, glm::vec3(0, 0.3, 0.5), true);
					this->hit = true;
					speed *= 2;
				}
					
				p->dead = true;
				
				break;

			}

		}

		float distance = sqrt(pow(pozitiaX - this->x, 2) + pow(pozitiaY - this->y, 2));

		if (distance < 70) {
			my_hp--;
			this->dead = true;

			if (my_hp == 0)
				game_over = true;
		}
		
	}

	Enemy_ship(float angle, float radius) {
		
		this->x = pozitiaX + radius *cos(angle);
		this->y = pozitiaY + radius *sin(angle);
		this->a = 0;
		this->dead = false;
		this->hp = rand() % 2 + 1;
		this->scale = 1;
		this->speed = 0.7 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0.7)));
		if(this->hp == 1)
			this->mesh = Object2D::CreateTriangle("triangle_enemy", glm::vec3(0, 0, 0), 75, glm::vec3(0, 0, 1), true);
		else
			this->mesh = Object2D::CreateTriangle("triangle_enemy", glm::vec3(0, 0, 0), 75, glm::vec3(1, 0, 1), true);
		this->hit = false;

	}
	void update(float dt) {
		this->a = atan2(pozitiaY - this->y, pozitiaX - this->x);

		float dX = speed * 150 * cos(a);
		float dY = speed * 150 * sin(a);

		this->x += dX * dt;
		this->y += dY * dt;

		float dS = 0;
		if (hit && this->scale > 0.5) 
			dS = -2;
		
		this->scale += dS * dt;
		
		

		handle_collisions();

	}

};

float contor = 0;
float timp = 2;
float redness = 0;

void Laborator3::Update(float deltaTimeSeconds )
{
	if (game_over) {
		float dR = 0.5;


		if(redness < 1)
			redness += dR * deltaTimeSeconds;

		glClearColor( redness, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(pozitiaX, pozitiaY);

		modelMatrix *= Transform2D::Scale(scX, scY);

		modelMatrix *= Transform2D::Rotate(rotatie);

		RenderMesh2D(meshes["triangle"], shaders["VertexColor"], modelMatrix);

		return;
	}

	// TODO: update steps for translation, rotation, scale, in order to create animations
	contor += deltaTimeSeconds;

	glm::ivec2 resolution = window->GetResolution();

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(pozitiaX, pozitiaY);

	modelMatrix *= Transform2D::Scale(scX, scY);

	modelMatrix *= Transform2D::Rotate(rotatie);

	RenderMesh2D(meshes["triangle"], shaders["VertexColor"], modelMatrix);

	for (int i = 0; i < my_hp; i++) {
		glm::mat3 modelMatrixHp = glm::mat3(1);

		modelMatrixHp *= Transform2D::Translate(resolution.x - 50 + i * 10, resolution.y - 70);

		modelMatrixHp *= Transform2D::Rotate(3.14/2);

		modelMatrixHp *= Transform2D::Scale(2, 1);

		RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrixHp);


	}
	


	for (Projectile *p : projectiles) {
		if (p->dead) continue;
		p->update(deltaTimeSeconds);

		glm::mat3 modelMatrixP = glm::mat3(1);
		modelMatrixP *= Transform2D::Translate(p->x, p->y);

		modelMatrixP *= Transform2D::Rotate(p->a);

		RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrixP);

	}

	if (contor >= timp)
	{
		//printf("au trecut 2 sec \n");
		contor = 0;
		if (timp > 0.5)
			timp -= deltaTimeSeconds;
		
		float angle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.14 * 2)));

		float radius = float(min(resolution.y, resolution.x) / 2 - 10);

		Enemy_ship *newE = new Enemy_ship(angle,radius);
		enemy_ships.push_back(newE);


	}
	

	for (Enemy_ship *enemy : enemy_ships) {
		if (enemy->dead) continue;
		enemy->update(deltaTimeSeconds);

		glm::mat3 modelMatrixE = glm::mat3(1);
		modelMatrixE *= Transform2D::Translate(enemy->x, enemy->y);

		modelMatrixE *= Transform2D::Rotate(enemy->a);

		modelMatrixE *= Transform2D::Scale(enemy->scale,enemy->scale);

		RenderMesh2D(enemy->mesh, shaders["VertexColor"], modelMatrixE);

	}



	
}

void Laborator3::FrameEnd()
{

}

void Laborator3::OnInputUpdate(float deltaTimeSeconds, int mods)
{
	if (game_over) return;
	if (window->KeyHold(GLFW_KEY_D)) pozitiaX += 400 * deltaTimeSeconds;
	if (window->KeyHold(GLFW_KEY_A)) pozitiaX -= 400 * deltaTimeSeconds;
	if (window->KeyHold(GLFW_KEY_W)) pozitiaY += 400 * deltaTimeSeconds;
	if (window->KeyHold(GLFW_KEY_S)) pozitiaY -= 400 * deltaTimeSeconds;


}

void Laborator3::OnKeyPress(int key, int mods)
{
	// add key press event
	

}

void Laborator3::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	glm::ivec2 resolution = window->GetResolution();
	if (!game_over)
	rotatie = atan2(-(pozitiaY - (resolution.y - mouseY)), mouseX - pozitiaX);
	
}

void Laborator3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	glm::ivec2 resolution = window->GetResolution();
	if (button == 1)
	{
		float a = atan2(-(pozitiaY - (resolution.y - mouseY)), mouseX - pozitiaX);
		Projectile *newP = new Projectile(pozitiaX, pozitiaY, a);
		projectiles.push_back(newP);
		
		
	}

}

void Laborator3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator3::OnWindowResize(int width, int height)
{
}
