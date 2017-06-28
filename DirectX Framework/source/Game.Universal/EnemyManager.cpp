#include "pch.h"
#include "EnemyManager.h"

using namespace DX;
using namespace std;

namespace LuftKampf
{
	EnemyManager::EnemyManager(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
		Manager(deviceResources), mRandomGenerator(mRandomDevice())
	{
		mManagerMap["EnemyManager"] = this;
	}

	void EnemyManager::CreateEnemy()
	{
		uniform_int_distribution<int32_t> xDirection(0, 1);
		uniform_int_distribution<int32_t> yDirection(0, 1);

		uniform_real_distribution<float> positionX(static_cast<Player*>(mManagerMap["Player"])->Position().x + 640.0f /*Half the window width*/, static_cast<Player*>(mManagerMap["Player"])->Position().x + 1280.0f/*Buffer to spawn within*/);
		float initPositionX = positionX(mRandomGenerator);
		uniform_real_distribution<float> positionY(static_cast<Player*>(mManagerMap["Player"])->Position().y + 360.0f /*Half the window height*/, static_cast<Player*>(mManagerMap["Player"])->Position().y + 720.0f/*Buffer to spawn within*/);
		float initPositionY = positionY(mRandomGenerator);

		if (xDirection(mRandomGenerator)) // Spawn in negative x direction from Player
		{
			initPositionX = -initPositionX;
		}
		if (yDirection(mRandomGenerator)) // Spawn in negative y direction from Player
		{
			initPositionY = -initPositionY;
		}

		uniform_real_distribution<float> angle(0.0f, DirectX::XM_2PI);
		float rotationAngle = angle(mRandomGenerator);

		Transform2D transform;
		transform.SetPosition(initPositionX, initPositionY);
		transform.SetRotation(rotationAngle);
		mEnemies.push_back(make_shared<Enemy>(transform, this));
	}

	void EnemyManager::Update(const StepTimer& timer)
	{
		//Partition the Enemies marked for destruction and then remove them.
		uint32_t destroyAfterThis = static_cast<uint32_t>(mEnemies.size());
		for (uint32_t i = 0; i < destroyAfterThis; ++i)
		{
			if (mEnemies[i]->IsMarkedForDestruction())
			{
				//Get index of the first event from the send that is not expired.
				while ((destroyAfterThis > i) && (mEnemies[--destroyAfterThis]->IsMarkedForDestruction()));
				std::shared_ptr<Enemy> temp = mEnemies[i];
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
			mEnemies[i]->Update(static_cast<float>(timer.GetElapsedSeconds()));
		}
	}

	DirectX::XMFLOAT2 EnemyManager::PlayerPosition()
	{
		return static_cast<Player*>(mManagerMap["Player"])->Position();
	}
}
