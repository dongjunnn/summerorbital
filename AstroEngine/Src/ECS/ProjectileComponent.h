#pragma once

#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(int spd, Vector2D vel) : speed(spd), velocity(vel)
	{ }
	~ProjectileComponent()
	{ }

	void init() override
	{ 
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
	}

	void update() override
	{
		distance += speed;		// todo game tick 
		if (transform->position.x > 800 ||
			transform->position.x < 0 ||
			transform->position.y > 640 ||
			transform->position.y < 0)
		{
			entity->destroy();
		}
	}

private:

	TransformComponent* transform;
	int speed = 0;
	int distance = 0;
	Vector2D velocity;
};
