#pragma once

#include "MatrixHelper.h"
#include "Transform2D.h"
#include <cstdint>
#include <agile.h>
#include <d3d11_3.h>

namespace LuftKampf
{
	struct GameSprite final
	{
		GameSprite(std::uint32_t spriteIndex, DX::Transform2D transform, const std::wstring& textureFile, bool toDraw = true, const DirectX::XMFLOAT4X4& textureTransform = DX::MatrixHelper::Identity);

		std::wstring mSpriteTexture;
		DirectX::XMFLOAT4X4 mTextureTransform;
		DX::Transform2D mTransform;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSpriteSheet;
		std::uint32_t mSpriteIndex;
		bool mToDraw;
		bool mLoadingComplete;
	};
}