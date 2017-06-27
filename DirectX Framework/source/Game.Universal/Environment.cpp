#include "pch.h"
#include "Environment.h"

using namespace std;
using namespace DX;
using namespace DirectX;

namespace LuftKampf
{
	Environment::Environment(const shared_ptr<DX::DeviceResources>& deviceResources) :
		IManager(deviceResources)
	{
		Transform2D tempTransform;

		tempTransform.SetScale(XMFLOAT2(0.5f, 0.1f));
		//tempTransform.SetPosition(0.0f, 1620.0f); /*Vertical offset from centre*/
		mStratosSprite = make_shared<GameSprite>(2, tempTransform, L"Content\\Textures\\Stratosphere.png");
		SpriteManager::AddGameSprite(mStratosSprite);

		tempTransform.SetScale(XMFLOAT2(0.5f, 0.5f));
		mBackgroundSprite = make_shared<GameSprite>(0, tempTransform, L"Content\\Textures\\Environment.png");
		SpriteManager::AddGameSprite(mBackgroundSprite);

		tempTransform.SetPosition(0.0f, -1620.0f); /*Vertical offset from centre*/
		mSeaSprite = make_shared<GameSprite>(2, tempTransform, L"Content\\Textures\\Sea.png");
		SpriteManager::AddGameSprite(mSeaSprite);
	}

	void Environment::Update(const StepTimer& stepTimer)
	{
		UNREFERENCED_PARAMETER(stepTimer);
		//mBackgroundSprite->mTransform.SetPosition(mParent->GameSprite.x, 0.0f);
	}
}