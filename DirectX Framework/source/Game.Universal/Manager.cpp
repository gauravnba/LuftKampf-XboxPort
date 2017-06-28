#include "pch.h"
#include "Manager.h"

namespace LuftKampf
{
	using namespace std;

	unordered_map<string, Manager*> Manager::mManagerMap;

	Manager::Manager(const shared_ptr<DX::DeviceResources>& deviceResources) :
		DX::GameComponent(deviceResources)
	{
	}
}