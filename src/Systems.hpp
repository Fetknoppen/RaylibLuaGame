#pragma once

#include "entt.hpp"
#include "Components.hpp"

#define RAYMATH_IMPLEMENTATION
#include "raymath.h"

class System {
public:
	//virtual ~System() = 0;
	virtual bool OnUpdate(entt::registry& registry, float delta) = 0;
};

// class PhysicsSystem : public System {
// public:
// 	//virtual ~PhysicsSystem() override;
// 	bool OnUpdate(entt::registry& registry, float delta) final {
// 		auto view = registry.view<PhysichsComponent, Transform>();
// 		view.each([&](PhysichsComponent& physComp, Transform& transform) {
// 			const float g = -9.82f;
// 			physComp.velocity.y = physComp.velocity.y + g * delta;
			
// 				if (transform.translation.y <= 0.0f && physComp.jumpTimer <= 0.0f) {
// 					transform.translation.y = 0.0f;
// 					physComp.velocity.y = 0.0f;
// 					physComp.grounded = true;
// 				}
// 				else{
// 					transform.translation.y += physComp.velocity.y + delta;
// 				}
// 			if (physComp.jumpTimer > 0.0f){
// 				physComp.jumpTimer -= delta;
// 			}
// 		});
// 		return false;
// 	}
// };

// class ModelMoveSystem : public System {
// public:
// 	//virtual ~ModelMoveSystem() override;
// 	bool OnUpdate(entt::registry& registry, float delta) final {
// 		auto view = registry.view<Transform, MeshComponent>();
// 		view.each([](Transform& transform, MeshComponent& meshComp) {
// 			meshComp.model.transform = MatrixRotateXYZ(QuaternionToEuler(transform.rotation));
// 		});
// 		return false;
// 	}
// };

// class PlayerMovementSystem : public System{
// public:
// 	//virtual ~PlayerMovementSystem() override;
// 	bool OnUpdate(entt::registry& registry, float delta) final {
// 		auto view = registry.view<Transform, PlayerComponent, PhysichsComponent, Camera, LivingComponent>();
// 		view.each([](Transform& transform, PlayerComponent& playerComp, PhysichsComponent& physComp, const Camera& cam, const LivingComponent& livComp) {
// 			Vector3 forward = Vector3Subtract(transform.translation, cam.position);
// 			forward.y = 0.0f;
// 			forward = Vector3Normalize(forward);
// 			playerComp.forwardVec = forward;
// 			Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, Vector3({0.0f, 1.0f, 0.0f})));
// 			//Vector3 step = Vector3(0.0f, 0.0f, 0.0f);
// 			// if (IsKeyDown(playerComp.forward) && !IsKeyDown(playerComp.back)) {
// 			// 	//Go forward 
// 			// 	//transform.translation = Vector3Add(Vector3Scale(forward, (float)livComp.speed), transform.translation);
// 			// }
// 			// else if (IsKeyDown(playerComp.back) && !IsKeyDown(playerComp.forward)) {
// 			// 	//Go back
// 			// 	//Vector3 dir = Vector3Scale(forward, -1);
// 			// 	//transform.translation = Vector3Add(Vector3Scale(dir, (float)livComp.speed), transform.translation);
// 			// }
// 			if (IsKeyDown(playerComp.left) && !IsKeyDown(playerComp.right)) {
// 				//Go left
// 				Vector3 dir = Vector3Scale(right, -1);
// 				transform.translation = Vector3Add(Vector3Scale(dir, (float)livComp.speed), transform.translation);
// 				playerComp.yaw = 90.0f;
// 			}
// 			else if (IsKeyDown(playerComp.right) && !IsKeyDown(playerComp.left)) {
// 				//Go righ
// 				transform.translation = Vector3Add(Vector3Scale(right, (float)livComp.speed), transform.translation);
// 				playerComp.yaw = -90.0f;
// 			}
// 			transform.rotation = QuaternionFromEuler(DEG2RAD * playerComp.pitch, DEG2RAD *  playerComp.yaw, DEG2RAD * playerComp.roll);
// 			if (IsKeyDown(playerComp.jump) && physComp.grounded) {
// 				physComp.velocity.y = 10.0f;
// 				physComp.jumpTimer = physComp.jumpTimerOrig;
// 				physComp.grounded = false;
// 			}
// 		});
// 		return false;
// 	};
// };

// class PlayerCameraSystem : public System {
// public:
// 	//virtual ~PlayerCameraSystem() override;
// 	bool OnUpdate(entt::registry& registry, float delta) final {
// 		auto view = registry.view<Camera, Transform, PlayerComponent>();
// 		view.each([](Camera& cam, Transform& transform, PlayerComponent& playerComp) {

// 			cam.target = transform.translation;

// 			// float angle = Vector3Angle(Vector3(1.0f, 0.0f, 0.0f), playerComp.forwardVec);
// 			// angle -= DEG2RAD * 90;
// 			// if (playerComp.forwardVec.z < 0) {
// 			// 	angle = -angle;
// 			// 	angle += DEG2RAD * 180;
// 			// }
// 			// playerComp.yaw = angle;
// 			// transform.rotation = QuaternionFromEuler(DEG2RAD * playerComp.pitch, DEG2RAD * playerComp.yaw, DEG2RAD * playerComp.roll);
// 		});
// 		return false;
// 	}
	
// };