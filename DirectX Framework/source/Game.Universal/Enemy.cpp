#include "pch.h"
#include "Enemy.h"

using namespace DirectX;
using namespace DX;
using namespace std;

namespace LuftKampf
{
	const float Enemy::ShootDelay = 0.1f;
	const float Enemy::ShootingTolerance = 0.08727f;
	const float Enemy::EnemyWidth = 40.0f;
	const float Enemy::EnemyHeight = 40.0f;
	const float Enemy::AngularVelocity = 0.85f;
	const float Enemy::Velocity = 300.0f;

	Enemy::Enemy(Transform2D& transform, EnemyManager* enemyManager) :
		mEnemyManager(enemyManager), mIsMarkedForDestruction(false)
	{
		transform.SetRotation(1.57079f); // Rotate by 90 degrees
		transform.SetScale(XMFLOAT2(EnemyWidth, EnemyHeight));
		mSprite = make_shared<GameSprite>(2, transform, L"Content\\Textures\\Enemy.png");

		//Load the texture
		auto loadSpriteSheetAndCreateSpritesTask = Concurrency::create_task([this]() {
			ThrowIfFailed(CreateWICTextureFromFile(mEnemyManager->DeviceResources()->GetD3DDevice(), mSprite->mSpriteTexture.c_str(), nullptr, mSprite->mSpriteSheet.ReleaseAndGetAddressOf()));
		});

		// Once the cube is loaded, the object is ready to be rendered.
		loadSpriteSheetAndCreateSpritesTask.then([this]() {
			mSprite->mLoadingComplete = true;
		});

		SpriteManager::AddGameSprite(mSprite);
	}

	Enemy::~Enemy()
	{
		mSprite->mToDraw = false;
	}

	bool Enemy::IsMarkedForDestruction()
	{
		return mIsMarkedForDestruction;
	}

	void Enemy::Update(float deltaTime)
	{
		deltaTime;
		float vectorToPlayerX = mEnemyManager->PlayerPosition().x - mSprite->mTransform.Position().x;
		float vectorToPlayerY = mEnemyManager->PlayerPosition().y - mSprite->mTransform.Position().y;

		float playerDirection = atan2f(vectorToPlayerY, vectorToPlayerX);
		float angleToPlayer = playerDirection - mSprite->mTransform.Rotation();

		// Rotate Enemy
		mSprite->mTransform.SetRotation(mSprite->mTransform.Rotation() + (angleToPlayer * AngularVelocity * deltaTime));

		//// Apply Velocity to Enemy
		mVelocity.x = Velocity * cos(mSprite->mTransform.Rotation()) * deltaTime;
		mVelocity.y = Velocity * sin(mSprite->mTransform.Rotation()) * deltaTime;
		mSprite->mTransform.SetPosition(mSprite->mTransform.Position().x + mVelocity.x, mSprite->mTransform.Position().y + mVelocity.y);

		if (angleToPlayer < ShootingTolerance && angleToPlayer > -ShootingTolerance)
		{
			if (mShootDelay > ShootDelay)
			{
				ProjectileManager::CreateProjectile(false, mSprite->mTransform);

				mShootDelay = 0;
			}
			else
			{
				mShootDelay += deltaTime;
			}
		}

		// Clamp rotation to 360 degrees.
		if (mSprite->mTransform.Rotation() >= DirectX::XM_2PI)
		{
			mSprite->mTransform.SetRotation(mSprite->mTransform.Rotation() - DirectX::XM_2PI);
		}
		else if (mSprite->mTransform.Rotation() <= -DirectX::XM_2PI)
		{
			mSprite->mTransform.SetRotation(mSprite->mTransform.Rotation() + DirectX::XM_2PI);
		}
	}
}