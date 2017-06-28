#pragma once

namespace DX
{
	class Transform2D;
}

namespace LuftKampf
{
	struct GameSprite;
	class EnemyManager;

	class Enemy final
	{
		static const float ShootDelay;
		static const float ShootingTolerance;
		static const float Velocity;
		static const float EnemyWidth;
		static const float EnemyHeight;
		static const float AngularVelocity;

	public:
		Enemy(DX::Transform2D& transform, EnemyManager* enemyManger);
		Enemy(const Enemy&) = default;
		Enemy(Enemy&&) = default;
		~Enemy();

		bool IsMarkedForDestruction();

		void Update(float deltaTime);

		void DamageEnemy(std::int32_t damage);
		DirectX::XMFLOAT2 Position();

	private:
		bool mIsMarkedForDestruction;
		float mShootDelay;
		float mHealth;
		DirectX::XMFLOAT2 mVelocity;
		std::shared_ptr<GameSprite> mSprite;
		EnemyManager* mEnemyManager;
	};
}