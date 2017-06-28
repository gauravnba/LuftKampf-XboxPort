#pragma once

#include "Manager.h"
#include <cstdint>

namespace DX
{
	class StepTimer;
}

namespace LuftKampf
{
	struct GameSprite;

	class Environment final : public Manager
	{
	public:
		Environment(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		Environment(const Environment&) = delete;
		Environment(Environment&&) = delete;
		Environment& operator=(const Environment&) = delete;
		Environment& operator=(Environment&&) = delete;
		~Environment() = default;
		
		void Update(const DX::StepTimer& stepTimer) override;

	private:
		std::shared_ptr<struct GameSprite> mStratosSprite;
		std::shared_ptr<struct GameSprite> mSeaSprite;
	};
}