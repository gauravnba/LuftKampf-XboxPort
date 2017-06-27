#include "pch.h"

namespace LuftKampf
{
	IManager::IManager(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
		DX::GameComponent(deviceResources)
	{
	}
}