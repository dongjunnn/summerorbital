#pragma once
#include "../SceneInstance.h"
#include "../Components/Components.h"

class MovementSystem
{
public:
	MovementSystem(SceneInstance& sceneRef) : scene(sceneRef) {}

	void update()
	{
		auto& view = scene.GetView<TransformComponent, VelocityComponent>();

		for (Entity e : view)
		{
			TransformComponent& t = scene.GetEntityData<TransformComponent>(e);
			VelocityComponent& v = scene.GetEntityData<VelocityComponent>(e);

			t.position.x += v.velocity.x;
			t.position.y += v.velocity.y;
		}
	}

private:
	SceneInstance& scene;
};