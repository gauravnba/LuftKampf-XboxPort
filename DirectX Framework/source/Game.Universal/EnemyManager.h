#pragma once

#include "Manager.h"
#include <random>

namespace LuftKampf
{
	class Enemy;

	class EnemyManager final : public Manager
	{
	public:
		EnemyManager(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		EnemyManager(const EnemyManager&) = delete;
		EnemyManager& operator=(const EnemyManager&) = delete;
		EnemyManager& operator=(EnemyManager&&) = delete;
		EnemyManager(EnemyManager&&) = delete;
		~EnemyManager() = default;

		void CreateEnemy();
		void Update(const DX::StepTimer& timer) override;

		DirectX::XMFLOAT2 PlayerPosition();

	private:
		std::vector<std::shared_ptr<Enemy>> mEnemies;
		bool mNoEnemiesPresent;
		std::random_device mRandomDevice;
		std::default_random_engine mRandomGenerator;
	};
}