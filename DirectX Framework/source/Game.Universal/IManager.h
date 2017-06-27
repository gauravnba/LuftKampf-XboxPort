#pragma once

#include "GameComponent.h"
#include <unordered_map>

namespace LuftKampf
{
	class IManager : public DX::GameComponent
	{
	public:
		IManager(const std::shared_ptr<DX::DeviceResources>& deviceResources);

	protected:
		std::unordered_map<std::string, IManager*> mManagerMap;
	};
}