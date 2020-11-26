
#include <string>
#include <SFML/Graphics.hpp>

#include <Cassert>
#include "windows.h"
#include <cmath> 


using namespace std;

struct Vector2f
{
	float x;
	float y;

	Vector2f()
	{
		x = 0;
		y = 0;
	}

	Vector2f(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2f operator+ (Vector2f v)
	{
		return Vector2f(x + v.x, y + v.y);
	}

	Vector2f operator- (Vector2f v)
	{
		return Vector2f(x - v.x, y - v.y);
	}

	Vector2f operator* (float n)
	{
		return Vector2f(n * x, n * y);
	}

	Vector2f operator/ (float n)
	{
		return Vector2f(x / n, y / n);
	}

	float scalar(Vector2f v)
	{
		return  x * v.x + y * v.y;
	}

	float length()
	{
		return sqrt(x * x + y * y);
	}

	Vector2f normalize()
	{
		float l = this->length();
		return Vector2f(x / l, y / l);
	}

	float angle(Vector2f u)
	{
		return((atan((u.y - y) / (u.x - x))) * 180 / 3.14);
	}

};


struct Sphere
{
public:

	Vector2f position;
	Vector2f velocity;
	Vector2f acceleration;
	int radius;
	int red;
	int green;
	int blue;
	int m;

	Sphere()
	{
		position = { 0, 0 };
		velocity = { 0, 0 };
		acceleration = { 0, 0 };
		radius = 0;
		red = 0;
		green = 0;
		blue = 0;
		m = 0;
	}

	Sphere(Vector2f position, Vector2f velocity, Vector2f acceleration, int radius, int red, int green, int blue, int m)
	{
		this->position = position;
		this->velocity = velocity;
		this->acceleration = acceleration;
		this->radius = radius;
		this->red = red;
		this->green = green;
		this->blue = blue;
		this->m = m;
	}


	void drawSphere(sf::RenderWindow* window)
	{
		int numberofCircles = 10;
		float x_centerofCircles = position.x;
		float y_centerofCircles = position.y;
		sf::CircleShape circle(0, 30);
		for (int i = 0; i < numberofCircles; i++)
		{
			circle.setRadius(radius - i * radius / numberofCircles);
			circle.setFillColor(sf::Color(i * red / numberofCircles, i * green / numberofCircles, i * blue / numberofCircles));
			circle.setPosition(position.x + (position.x - x_centerofCircles) * i / numberofCircles - radius + radius * i / numberofCircles, position.y + (position.y - y_centerofCircles) * i / numberofCircles - radius + radius * i / numberofCircles);
			window->draw(circle);
		}
	}

	void moveSphere(float t)
	{
		position = position + velocity * t;
	}

	void collideSphereWall(int windowX, int windowY, int t)
	{
		if (position.x + radius + velocity.x * t > windowX)
		{
			velocity.x = -abs(velocity.x);
			position.x = position.x + 2 * (windowX - position.x) - 2 * radius;
		}

		if (position.x + velocity.x * t - radius < 0)
		{
			velocity.x = abs(velocity.x);
			position.x = -position.x + 2 * radius;
		}
		if (position.y + radius + velocity.y * t > windowY)
		{
			velocity.y = -abs(velocity.y);
			position.y = position.y + 2 * (windowY - position.y) - 2 * radius;
		}
		if (position.y + velocity.y * t - radius < 0)
		{
			velocity.y = abs(velocity.y);
			position.y = -position.y + 2 * radius;
		}
	}

	bool checkCollisionTwoSphers(Sphere sphere2)
	{
		return (pow(sphere2.position.x - position.x, 2) + pow(sphere2.position.y - position.y, 2) < pow(radius + sphere2.radius, 2));
	}

	void collideTwoSphers(Sphere * sphere2, int t, int windowX, int windowY)
	{
		float dv1 = velocity.x;
		float du1 = velocity.y;
		float dv2 = sphere2->velocity.x;
		float du2 = sphere2->velocity.y;

		float x1 = position.x;
		float y1 = position.y;
		float x2 = sphere2->position.x;
		float y2 = sphere2->position.y;

		float dist = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
		position.x = position.x + (radius + sphere2->radius - dist) * (x1 - x2) / (2 * dist);
		position.y = position.y + (radius + sphere2->radius - dist) * (y1 - y2) / (2 * dist);
		sphere2->position.x = sphere2->position.x + (radius + sphere2->radius - dist) * (x2 - x1) / (2 * dist);
		sphere2->position.y = sphere2->position.y + (radius + sphere2->radius - dist) * (y2 - y1) / (2 * dist);

		if ((x1 + radius + velocity.x * t > windowX) or (x1 + velocity.x * t - radius < 0))
		{
			sphere2->position.x = sphere2->position.x + (radius + sphere2->radius - dist) * (x2 - x1) / dist;
		}

		if ((x2 + sphere2->radius + sphere2->velocity.x * t > windowX) or (x2 + velocity.x * t - radius < 0))
		{
			position.x = position.x + (radius + sphere2->radius - dist) * (x1 - x2) / dist;
			sphere2->position.x = x2;
		}
		if ((y1 + radius + velocity.y * t > windowY) or (y1 + velocity.y * t - radius < 0))
		{
			sphere2->position.y = sphere2->position.y + (radius + sphere2->radius - dist) * (y2 - y1) / dist;
		}

		if ((y2 + sphere2->radius + sphere2->velocity.y * t > windowY) or (y2 + velocity.y * t - radius < 0))
		{
			position.y = position.y + (radius + sphere2->radius - dist) * (y1 - y2) / dist;
			sphere2->position.y = y2;
		}

		velocity.x = (2 * sphere2->m * dv2 + (m - sphere2->m) * dv1) / (m + sphere2->m);
		velocity.y = (2 * sphere2->m * du2 + (m - sphere2->m) * du1) / (m + sphere2->m);
		sphere2->velocity.x = (2 * m * dv1 + (sphere2->m - m) * dv2) / (m + sphere2->m);
		sphere2->velocity.y = (2 * m * du1 + (sphere2->m - m) * du2) / (m + sphere2->m);

		assert(dist);
	}

	void changeVelocity(float t)
	{
		velocity = velocity + acceleration * t;
	}
};



class Game
{
public:
	Game();
	void run();
private:
	void ProcessEvents();
	void update();
	void render();
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
private:
	sf::RenderWindow myWindow;
	sf::CircleShape myPlayer;
	bool movingLeft = 0, movingUp = 0, movingRight = 0, movingDown = 0, shot = 0;
	char *direction = new char[0];
	Sphere spheres[100];

	int i = 0; float t = 0.1;

};

Game::Game() : myWindow(sf::VideoMode(1920, 1080), "SFML Game"), myPlayer()
{
	myPlayer.setRadius(40.f);
	myPlayer.setFillColor(sf::Color::Red);
	myPlayer.setPosition(100.f, 100.f);
}

void Game::run()
{
	while (myWindow.isOpen())
	{
		ProcessEvents();
		update();
		render();
	}
}

void Game::ProcessEvents()
{
	sf::Event event;
	while (myWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed: handlePlayerInput(event.key.code, true); break;
		case sf::Event::KeyReleased: handlePlayerInput(event.key.code, false); break;
		case sf::Event::Closed: myWindow.close(); break;
		}
	}
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::W) movingUp = isPressed, direction = new char['W'];
	else if (key == sf::Keyboard::S) movingDown = isPressed, direction = new char['S'];
	else if (key == sf::Keyboard::A) movingLeft = isPressed, direction = new char['A'];
	else if (key == sf::Keyboard::D) movingRight = isPressed, direction = new char['D'];
	else if (key == sf::Keyboard::X) shot = isPressed;
}

void Game::update()
{
	sf::Vector2f movement(0.f, 0.f);
	sf::Vector2f pos(0.f, 0.f);
	if (movingUp) movement.y -= 5.f;
	if (movingDown) movement.y += 5.f;
	if (movingLeft) movement.x -= 5.f;
	if (movingRight) movement.x += 5.f;
	if (shot and (movement.x != 0 or movement.y != 0)) {spheres[i] = Sphere(), pos = myPlayer.getPosition(); spheres[i] = { {pos.x + myPlayer.getRadius(), pos.y + myPlayer.getRadius()} , {movement.x * 20, movement.y * 20},  {0, 0}, 10, 255, 0, 0, 10 * (i + 1) }; i += 1; };
	if (i == 100) i = 0;

	myPlayer.move(movement);
}


void Game::render()
{
	myWindow.clear();
	
	for (int j = 0; j < 100; j++)
	{
		if (spheres[j].radius != 0)
		{
			spheres[j].moveSphere(t);
			spheres[j].drawSphere(&myWindow);
		};
		
	};
    myWindow.draw(myPlayer);
	myWindow.display();

}



int main()
{
	Game game;
	game.run();

	return 0;
}