#include "pch.h"
#include "Player.h"

using namespace std;
using namespace DX;
using namespace DirectX;

namespace LuftKampf
{
	const float Player::PlayerWidth = 50;
	const float Player::PlayerHeight = 50;

	const float Player::Rotate = 4.5f;
	const float Player::RotateUnderThrust = 3.0f;

	const float Player::Gravity = 150.0f;
	const float Player::Drag = 100.0f;
	const float Player::Acceleration = 1000.0f;
	const float Player::MaxVelocity = 800.0f;
	const float Player::ShootingDelay = 0.1f;

	Player::Player(const shared_ptr<DX::DeviceResources>& deviceResources, const shared_ptr<KeyboardComponent>& keyboard, const shared_ptr<GamePadComponent>& gamepad, 
		const shared_ptr<OrthographicCamera>& camera, const shared_ptr<ProjectileManager>& projectileManager) :
		IManager(deviceResources),
		mVelocity(XMFLOAT2(0.0f, 0.0f)), mHealth(100)/* Default health is 100 */, mRotation(1.57079f)/*Rotate sprite by 90 degrees at the start*/, mIsThrusting(false),
		mKeyboard(keyboard), mGamepad(gamepad), mCamera(camera), mProjectileManager(projectileManager), mShootingDelay(0.0f)
	{
		Transform2D playerTransform(Vector2Helper::Zero, mRotation, XMFLOAT2(PlayerWidth, PlayerHeight));
		mSprite = make_shared<GameSprite>(2, playerTransform, L"Content\\Textures\\Airplane.png");
		
		// Store reference in IManager
		mManagerMap["Player"] = this;
		SpriteManager::AddGameSprite(mSprite);
	}

	void Player::Update(const DX::StepTimer& timer)
	{
		float deltaTime = static_cast<float>(timer.GetElapsedSeconds());
		CheckForInputs(deltaTime);
		UpdatePosition(deltaTime);
		mCamera->SetPosition(Position().x, Position().y, 1);
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
		if (mIsThrusting && (mVelocity.x <= MaxVelocity) && (mVelocity.y <= MaxVelocity))
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

	void Player::CheckForInputs(float deltaTime)
	{
		// Check for left rotation
		if (mKeyboard->WasKeyDown(Keys::Left) || (mGamepad->WasButtonDown(GamePadButtons::DPadLeft)))
		{
			if (mIsThrusting)
			{
				mRotation += RotateUnderThrust * deltaTime;
			}
			else
			{
				mRotation += Rotate * deltaTime;
			}
		}
		// Check for right rotation
		if (mKeyboard->WasKeyDown(Keys::Right) || (mGamepad->WasButtonDown(GamePadButtons::DPadRight)))
		{
			if (mIsThrusting)
			{
				mRotation -= RotateUnderThrust * deltaTime;
			}
			else
			{
				mRotation -= Rotate * deltaTime;
			}
		}
		// Check for Thrust application
		if(mKeyboard->WasKeyDown(Keys::Up) || (mGamepad->WasButtonDown(GamePadButtons::RightShoulder)))
		{
			UNREFERENCED_PARAMETER(deltaTime);
			mIsThrusting = true;
		}
		else
		{
			mIsThrusting = false;
		}

		// Check for Firing projectile
		if (mKeyboard->WasKeyDown(Keys::X) || (mGamepad->WasButtonDown(GamePadButtons::A)))
		{
			if (mShootingDelay >= ShootingDelay)
			{
				mProjectileManager->CreateProjectile(true, mSprite->mTransform);
				mShootingDelay = 0.0f;
			}
			else
			{
				mShootingDelay += deltaTime;
			}
		}
	}
	
	XMFLOAT2 Player::Position()
	{
		return mSprite->mTransform.Position();
	}

	void Player::DamagePlayer(uint32_t damage)
	{
		mHealth -= damage;
	}
}