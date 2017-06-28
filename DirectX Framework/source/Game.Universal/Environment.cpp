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

		// Initialize the Stratosphere sprite
		tempTransform.SetScale(XMFLOAT2(640.0f, 181.5f));
		tempTransform.SetPosition(0.0f, 1620.0f); /*Vertical offset from centre*/
		mStratosSprite = make_shared<GameSprite>(2, tempTransform, L"Content\\Textures\\Stratosphere.png");
		SpriteManager::AddGameSprite(mStratosSprite);

		// Initialize the Sea sprite
		tempTransform.SetPosition(0.0f, -1620.0f); /*Vertical offset from centre*/
		mSeaSprite = make_shared<GameSprite>(2, tempTransform, L"Content\\Textures\\Sea.png");
		SpriteManager::AddGameSprite(mSeaSprite);

		// Initialize the Coulds sprite
		tempTransform.SetScale(XMFLOAT2(1920.0f, 360.0f));
		tempTransform.SetPosition(0.0f, 720.0f);
		shared_ptr<GameSprite> cloudSprite = make_shared<GameSprite>(0, tempTransform, L"Content\\Textures\\Clouds.png");
		SpriteManager::AddGameSprite(cloudSprite);

		mManagerMap["Environment"] = this;
	}

	void Environment::Update(const StepTimer& stepTimer)
	{
		UNREFERENCED_PARAMETER(stepTimer);
		mSeaSprite->mTransform.SetPosition(static_cast<Player*>(mManagerMap["Player"])->Position().x, mSeaSprite->mTransform.Position().y);
		mStratosSprite->mTransform.SetPosition(static_cast<Player*>(mManagerMap["Player"])->Position().x, mStratosSprite->mTransform.Position().y);
	}
}