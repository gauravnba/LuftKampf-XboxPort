#include "pch.h"
#include "Player.h"

using namespace std;
using namespace DX;
using namespace DirectX;

namespace LuftKampf
{
	const std::int32_t Player::PlayerWidth = 50;
	const std::int32_t Player::PlayerHeight = 50;

	const float Player::Rotate = 4.5f;
	const float Player::RotateUnderThrust = 3.0f;

	const float Player::Gravity = 1.0f;
	const float Player::Drag = 1.0f;
	const float Player::Acceleration = 2.0f;
	const float Player::MaxVelocity = 1.0f;
	const float Player::ProjectileVelocity = 10.0f;

	Player::Player(const shared_ptr<DX::DeviceResources>& deviceResources) :
		IManager(deviceResources),
		mVelocity(XMFLOAT2(0.0f, 0.0f)), mHealth(100)/* Default health is 100 */, mRotation(1.57079f)/*Rotate sprite by 90 degrees at the start*/, mIsThrusting(false)
	{
		Transform2D playerTransform(Vector2Helper::Zero, mRotation, XMFLOAT2(0.05f, 0.03f));
		mSprite = make_shared<GameSprite>(2, playerTransform, L"Content\\Textures\\Airplane.png");
		
		// Store reference in IManager
		mManagerMap["Player"] = this;
		SpriteManager::AddGameSprite(mSprite);
	}

	void Player::Update(const DX::StepTimer& timer)
	{
		UpdatePosition(static_cast<float>(timer.GetElapsedSeconds()));
	}

	void Player::UpdatePosition(float deltaTime)
	{
		//Clamp player to the bounds of the environment.
		if (mSprite->mTransform.Position().y <= -1440.0f)
		{
			mSprite->mTransform.SetPosition(mSprite->mTransform.Position().x, -1440.0f);
			mVelocity.y = 0;
		}
		if (mSprite->mTransform.Position().y >= 1440)
		{
			mSprite->mTransform.SetPosition(mSprite->mTransform.Position().x, 1440.0f);
			mVelocity.y = 0;
		}

		//apply Gravity
		mVelocity.y -= (Gravity * deltaTime);

		//Apply drag to player
		if (mVelocity.x > 0)
		{
			mVelocity.x -= (Drag * deltaTime);
		}

		if (mVelocity.x < 0)
		{
			mVelocity.x += (Drag * deltaTime);
		}

		//Use thrusters
		if (mIsThrusting && (mVelocity.x <= MaxVelocity) && (mVelocity.x <= MaxVelocity))
		{
			mVelocity.x += (Acceleration * cos(mRotation)) * deltaTime;
			mVelocity.y += (Acceleration * sin(mRotation)) * deltaTime;
		}

		//Set position in frame
		XMFLOAT2 position = mSprite->mTransform.Position();
		position.x += (mVelocity.x * deltaTime);
		position.y += (mVelocity.y * deltaTime);

		//Set Sprite position.
		mSprite->mTransform.SetPosition(position);
		mSprite->mTransform.SetRotation(mRotation);
	}
}