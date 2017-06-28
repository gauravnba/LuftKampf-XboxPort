#include "pch.h"
#include "IManager.h"

namespace LuftKampf
{
	using namespace std;

	unordered_map<string, IManager*> IManager::mManagerMap;

	IManager::IManager(const shared_ptr<DX::DeviceResources>& deviceResources) :
		DX::GameComponent(deviceResources)
	{
	}
}