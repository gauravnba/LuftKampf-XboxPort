#include "pch.h"
#include "EnemyManager.h"

using namespace DX;

namespace LuftKampf
{
	EnemyManager::EnemyManager(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
		IManager(deviceResources)
	{
	}

	void EnemyManager::Update(const StepTimer& timer)
	{
		//Partition the Enemies marked for destruction and then remove them.
		uint32_t destroyAfterThis = mEnemies.size();
		for (uint32_t i = 0; i < destroyAfterThis; ++i)
		{
			if (mEnemies[i]->isMarkedForDestruction())
			{
				//Get index of the first event from the send that is not expired.
				while ((destroyAfterThis > i) && (mEnemies[--destroyAfterThis]->isMarkedForDestruction()));
				Enemy* temp = mEnemies[i];
				mEnemies[i] = mEnemies[destroyAfterThis];
				mEnemies[destroyAfterThis] = temp;
			}
		}
		mEnemies.erase(mEnemies.begin() + destroyAfterThis, mEnemies.end());

		if (mEnemies.size() == 0)
		{
			mNoEnemiesPresent = true;
		}

		if ((mEnemies.size() <= 5) && mNoEnemiesPresent)
		{
			CreateEnemy();
			if (mEnemies.size() == 5)
			{
				mNoEnemiesPresent = false;
			}
		}

		for (uint32_t i = 0; i < mEnemies.size(); i++)
		{
			mEnemies[i]->update(timer.GetElapsedSeconds());
		}
	}
}
