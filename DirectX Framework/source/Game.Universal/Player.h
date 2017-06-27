#pragma once

#include "IManager.h"

namespace DX
{
	class StepTimer;
}

namespace LuftKampf
{
	struct GameSprite;

	class Player : public IManager
	{
		static const std::int32_t PlayerWidth;
		static const std::int32_t PlayerHeight;

		static const float Rotate;
		static const float RotateUnderThrust;

		static const float Gravity;
		static const float Drag;
		static const float Acceleration;
		static const float MaxVelocity;
		static const float ProjectileVelocity;

	public:
		Player(const std::shared_ptr<DX::DeviceResources>& deviceResources);

		void Update(const DX::StepTimer& timer) override;

		void DamagePlayer(std::uint32_t damage);

	private:
		void UpdatePosition(float deltaTime);

		std::shared_ptr<GameSprite> mSprite;
		DirectX::XMFLOAT2 mVelocity;
		std::int32_t mHealth;
		float mRotation;
		bool mIsThrusting;
	};
}