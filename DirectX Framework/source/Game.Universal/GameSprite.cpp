#include "pch.h"
#include "GameSprite.h"

using namespace std;

namespace LuftKampf
{

	GameSprite::GameSprite(std::uint32_t spriteIndex, DX::Transform2D transform, const wstring& textureFile, bool toDraw /* = true */, const DirectX::XMFLOAT4X4& textureTransform /* = DX::MatrixHelper::Identity */) :
		mSpriteIndex(spriteIndex), mTransform(transform), mSpriteTexture(textureFile), mToDraw(toDraw), mTextureTransform(textureTransform)
	{
	}
}