#pragma once

#include "DrawableGameComponent.h"
#include "Manager.h"
#include <DirectXMath.h>
#include <vector>

namespace DX 
{
	class Transform2D;
}

namespace LuftKampf
{
	class Projectile;
	class Field;

	class ProjectileManager final : public DX::DrawableGameComponent
	{
	public:
		ProjectileManager(const std::shared_ptr<DX::DeviceResources>& deviceResources, const std::shared_ptr<DX::Camera>& camera);

		static void CreateProjectile(bool isPlayerProjectile, const DX::Transform2D& transform);

		virtual void CreateDeviceDependentResources() override;
		virtual void ReleaseDeviceDependentResources() override;
		virtual void Update(const DX::StepTimer& timer) override;
		virtual void Render(const DX::StepTimer& timer) override;

	private:
		void InitializeLineVertices();
		void InitializeTriangleVertices();
		void DrawProjectile(const Projectile& projectile);
		void DrawSolidProjectile(const Projectile& projectile);

		static const std::uint32_t CircleResolution;
		static const std::uint32_t LineCircleVertexCount;
		static const std::uint32_t SolidCircleVertexCount;

		static const float ProjectileRadius;
		static const DirectX::XMFLOAT4 PlayerProjectileColor;
		static const DirectX::XMFLOAT4 EnemyProjectileColor;
		static const float ProjectileVelocity;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mLineVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mTriangleVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVSCBufferPerObject;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mPSCBufferPerObject;
		bool mLoadingComplete;
		static std::vector<std::shared_ptr<Projectile>> mProjectiles;
	};
}

