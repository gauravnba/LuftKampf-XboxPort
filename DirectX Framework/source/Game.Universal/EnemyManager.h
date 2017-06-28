#pragma once

#include "IManager.h"

namespace LuftKampf
{
	class Enemy;

	class EnemyManager final : public IManager
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

	private:
		std::vector<std::shared_ptr<Enemy>> mEnemies;
		bool mNoEnemiesPresent;
	};
}