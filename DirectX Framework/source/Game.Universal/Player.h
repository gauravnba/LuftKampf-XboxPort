#pragma once

#include "Manager.h"
#include <functional>

namespace DX
{
	class StepTimer;
	class KeyboardComponent;
	class GamePadComponent;
	class OrthographicCamera;
}

namespace LuftKampf
{
	struct GameSprite;
	class ProjectileManager;

	class Player final : public Manager
	{
		static const float PlayerWidth;
		static const float PlayerHeight;

		static const float Rotate;
		static const float RotateUnderThrust;

		static const float Gravity;
		static const float Drag;
		static const float Acceleration;
		static const float MaxVelocity;
		static const float ShootingDelay;

	public:
		Player(const std::shared_ptr<DX::DeviceResources>& deviceResources, const std::shared_ptr<DX::KeyboardComponent>& keyboard, 
			const std::shared_ptr<DX::GamePadComponent>& gamepad, const std::shared_ptr<DX::OrthographicCamera>& camera);
		Player(const Player&) = delete;
		Player& operator=(const Player&) = delete;
		Player(Player&&) = delete;
		~Player() = default;

		void Update(const DX::StepTimer& timer) override;

		void DamagePlayer(std::uint32_t damage);

		DirectX::XMFLOAT2 Position();

	private:
		void UpdatePosition(float deltaTime);
		void CheckForInputs(float deltaTime);

		std::shared_ptr<GameSprite> mSprite;
		DirectX::XMFLOAT2 mVelocity;
		std::int32_t mHealth;
		float mRotation;
		bool mIsThrusting;
		float mShootingDelay;

		std::shared_ptr<DX::KeyboardComponent> mKeyboard;
		std::shared_ptr<DX::GamePadComponent> mGamepad;
		std::shared_ptr<DX::OrthographicCamera> mCamera;
	};
}