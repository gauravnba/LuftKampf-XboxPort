#pragma once

#include "GameComponent.h"
#include <unordered_map>

namespace LuftKampf
{
	class Manager : public DX::GameComponent
	{
	public:
		Manager(const std::shared_ptr<DX::DeviceResources>& deviceResources);

	protected:
		static std::unordered_map<std::string, Manager*> mManagerMap;
	};
}