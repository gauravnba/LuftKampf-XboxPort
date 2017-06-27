#pragma once

#include "IManager.h"
#include <cstdint>

namespace DX
{
	class StepTimer;
}

namespace LuftKampf
{
	struct GameSprite;

	class Environment : public IManager
	{
	public:
		Environment(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		
		void Update(const DX::StepTimer& stepTimer) override;

	private:
		std::shared_ptr<struct GameSprite> mBackgroundSprite;
		std::shared_ptr<struct GameSprite> mStratosSprite;
		std::shared_ptr<struct GameSprite> mSeaSprite;
	};
}