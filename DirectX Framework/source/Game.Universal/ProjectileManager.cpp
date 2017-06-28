#include "pch.h"
#include "ProjectileManager.h"
#include "Projectile.h"

using namespace std;
using namespace DirectX;
using namespace DX;

namespace LuftKampf
{
	vector<shared_ptr<Projectile>> ProjectileManager::mProjectiles;

	const uint32_t ProjectileManager::CircleResolution = 32;
	const uint32_t ProjectileManager::LineCircleVertexCount = ProjectileManager::CircleResolution + 2;
	const uint32_t ProjectileManager::SolidCircleVertexCount = (ProjectileManager::CircleResolution + 1) * 2;

	const float ProjectileManager::ProjectileRadius = 0.1f;
	const XMFLOAT4 ProjectileManager::PlayerProjectileColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	const XMFLOAT4 ProjectileManager::EnemyProjectileColor = XMFLOAT4(0.5333f, 0.0f, 1.0f, 0.08235f);
	const float ProjectileManager::ProjectileVelocity = 1200.0f;

	ProjectileManager::ProjectileManager(const shared_ptr<DX::DeviceResources>& deviceResources, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(deviceResources, camera), mLoadingComplete(false)
	{
		CreateDeviceDependentResources();
	}

	void ProjectileManager::CreateProjectile(bool isPlayerProjectile, const Transform2D& transform)
	{
		XMFLOAT2 velocity(ProjectileVelocity * cos(transform.Rotation()), ProjectileVelocity * sin(transform.Rotation()));
		if (isPlayerProjectile)
		{
			mProjectiles.emplace_back(make_shared<Projectile>(transform, ProjectileRadius, PlayerProjectileColor, velocity, isPlayerProjectile));
		}
		else
		{
			mProjectiles.emplace_back(make_shared<Projectile>(transform, ProjectileRadius, EnemyProjectileColor, velocity, !isPlayerProjectile));
		}
	}

	void ProjectileManager::CreateDeviceDependentResources()
	{
		auto loadVSTask = ReadDataAsync(L"ShapeRendererVS.cso");
		auto loadPSTask = ReadDataAsync(L"ShapeRendererPS.cso");

		// After the vertex shader file is loaded, create the shader and input layout.
		auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
			ThrowIfFailed(
				mDeviceResources->GetD3DDevice()->CreateVertexShader(
					&fileData[0],
					fileData.size(),
					nullptr,
					mVertexShader.ReleaseAndGetAddressOf()
				)
			);

			// Create an input layout
			ThrowIfFailed(
				mDeviceResources->GetD3DDevice()->CreateInputLayout(
					VertexPosition::InputElements,
					VertexPosition::InputElementCount,
					&fileData[0],
					fileData.size(),
					mInputLayout.ReleaseAndGetAddressOf()
				)
			);

			CD3D11_BUFFER_DESC constantBufferDesc(sizeof(XMFLOAT4X4), D3D11_BIND_CONSTANT_BUFFER);
			ThrowIfFailed(
				mDeviceResources->GetD3DDevice()->CreateBuffer(
					&constantBufferDesc,
					nullptr,
					mVSCBufferPerObject.ReleaseAndGetAddressOf()
				)
			);
		});

		// After the pixel shader file is loaded, create the shader and constant buffer.
		auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
			ThrowIfFailed(
				mDeviceResources->GetD3DDevice()->CreatePixelShader(
					&fileData[0],
					fileData.size(),
					nullptr,
					mPixelShader.ReleaseAndGetAddressOf()
				)
			);

			CD3D11_BUFFER_DESC constantBufferDesc(sizeof(XMFLOAT4), D3D11_BIND_CONSTANT_BUFFER);
			ThrowIfFailed(
				mDeviceResources->GetD3DDevice()->CreateBuffer(
					&constantBufferDesc,
					nullptr,
					mPSCBufferPerObject.ReleaseAndGetAddressOf()
				)
			);
		});

		auto createVerticesAndProjectilesTask = (createPSTask && createVSTask).then([this]() {
			InitializeLineVertices();
			InitializeTriangleVertices();
		});

		// Once the cube is loaded, the object is ready to be rendered.
		createVerticesAndProjectilesTask.then([this]() {
			mLoadingComplete = true;
		});
	}

	void ProjectileManager::ReleaseDeviceDependentResources()
	{
		mLoadingComplete = false;
		mVertexShader.Reset();
		mPixelShader.Reset();
		mInputLayout.Reset();
		mLineVertexBuffer.Reset();
		mTriangleVertexBuffer.Reset();
		mVSCBufferPerObject.Reset();
		mPSCBufferPerObject.Reset();
	}

	void ProjectileManager::Update(const StepTimer& timer)
	{
		//Partition the projectiles marked for destruction and then remove them.
		uint32_t destroyAfterThis = static_cast<uint32_t>(mProjectiles.size());
		for (uint32_t i = 0; i < destroyAfterThis; ++i)
		{
			if (mProjectiles[i]->IsMarkedForDestruction())
			{
				//Get index of the first event from the send that is not expired.
				while ((destroyAfterThis > i) && (mProjectiles[--destroyAfterThis]->IsMarkedForDestruction()));
				shared_ptr<Projectile> temp = mProjectiles[i];
				mProjectiles[i] = mProjectiles[destroyAfterThis];
				mProjectiles[destroyAfterThis] = temp;
			}
		}

		mProjectiles.erase(mProjectiles.begin() + destroyAfterThis, mProjectiles.end());

		for (auto& projectile : mProjectiles)
		{
			// Check if Projectiles are out of bounds.
			if ((projectile->Transform().Position().x > (mCamera->Position().x + 1280.0f)) 
				|| (projectile->Transform().Position().x < (mCamera->Position().x - 1280.0f))
				|| (projectile->Transform().Position().y > (mCamera->Position().y + 720.0f))
				|| (projectile->Transform().Position().y < (mCamera->Position().y - 720.0f)))
			{
				projectile->MarkToDestroy();
			}

			projectile->Update(timer);
		}
	}

	void ProjectileManager::Render(const StepTimer & timer)
	{
		UNREFERENCED_PARAMETER(timer);
		
		// Loading is asynchronous. Only draw geometry after it's loaded.
		if (!mLoadingComplete)
		{
			return;
		}

		ID3D11DeviceContext* direct3DDeviceContext = mDeviceResources->GetD3DDeviceContext();		
		direct3DDeviceContext->IASetInputLayout(mInputLayout.Get());

		direct3DDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
		direct3DDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

		direct3DDeviceContext->VSSetConstantBuffers(0, 1, mVSCBufferPerObject.GetAddressOf());
		direct3DDeviceContext->PSSetConstantBuffers(0, 1, mPSCBufferPerObject.GetAddressOf());

		for (const auto& projectile : mProjectiles)
		{
			if (projectile->IsSolid())
			{
				DrawSolidProjectile(*projectile);
			}
			else
			{
				DrawProjectile(*projectile);
			}
		}
	}

	void ProjectileManager::DrawProjectile(const Projectile& projectile)
	{
		ID3D11DeviceContext* direct3DDeviceContext = mDeviceResources->GetD3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		static const UINT stride = sizeof(VertexPosition);
		static const UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, mLineVertexBuffer.GetAddressOf(), &stride, &offset);

		const XMMATRIX wvp = XMMatrixTranspose(XMMatrixScaling(projectile.Radius(), projectile.Radius(), projectile.Radius()) * projectile.Transform().WorldMatrix() * mCamera->ViewProjectionMatrix());
		direct3DDeviceContext->UpdateSubresource(mVSCBufferPerObject.Get(), 0, nullptr, reinterpret_cast<const float*>(wvp.r), 0, 0);

		direct3DDeviceContext->UpdateSubresource(mPSCBufferPerObject.Get(), 0, nullptr, &projectile.Color(), 0, 0);

		direct3DDeviceContext->Draw(LineCircleVertexCount, 0);
	}

	void ProjectileManager::DrawSolidProjectile(const Projectile & projectile)
	{
		ID3D11DeviceContext* direct3DDeviceContext = mDeviceResources->GetD3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		static const UINT stride = sizeof(VertexPosition);
		static const UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, mTriangleVertexBuffer.GetAddressOf(), &stride, &offset);

		const XMMATRIX wvp = XMMatrixTranspose(XMMatrixScaling(projectile.Radius(), projectile.Radius(), projectile.Radius()) * projectile.Transform().WorldMatrix() * mCamera->ViewProjectionMatrix());
		direct3DDeviceContext->UpdateSubresource(mVSCBufferPerObject.Get(), 0, nullptr, reinterpret_cast<const float*>(wvp.r), 0, 0);

		direct3DDeviceContext->UpdateSubresource(mPSCBufferPerObject.Get(), 0, nullptr, &projectile.Color(), 0, 0);

		direct3DDeviceContext->Draw(SolidCircleVertexCount, 0);
	}

	void ProjectileManager::InitializeLineVertices()
	{
		const float increment = XM_2PI / CircleResolution;

		VertexPosition vertices[LineCircleVertexCount];

		for (int i = 0; i < CircleResolution; i++)
		{
			VertexPosition& vertex = vertices[i];

			vertex.Position.x = cosf(i * increment);
			vertex.Position.y = sinf(i * increment);
			vertex.Position.z = 0.0f;
			vertex.Position.w = 1.0f;
		}

		// Closing line to complete the circle
		vertices[CircleResolution] = VertexPosition(vertices[0]);

		// Axis line for visualizing rotation
		vertices[CircleResolution + 1] = VertexPosition(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
		vertexBufferDesc.ByteWidth = sizeof(VertexPosition) * LineCircleVertexCount;
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData = { 0 };
		vertexSubResourceData.pSysMem = vertices;
		ThrowIfFailed(mDeviceResources->GetD3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, mLineVertexBuffer.ReleaseAndGetAddressOf()));
	}

	void ProjectileManager::InitializeTriangleVertices()
	{
		const float increment = XM_2PI / CircleResolution;
		const XMFLOAT4 center(0.0f, 0.0f, 0.0f, 1.0f);

		vector<VertexPosition> vertices;
		vertices.reserve(SolidCircleVertexCount);
		for (int i = 0; i <= CircleResolution; i++)
		{
			VertexPosition vertex;
			vertex.Position.x = cosf(i * increment);
			vertex.Position.y = sinf(i * increment);
			vertex.Position.z = 0.0f;
			vertex.Position.w = 1.0f;

			vertices.push_back(vertex);
			vertices.push_back(center);
		}

		assert(vertices.size() == SolidCircleVertexCount);
				
		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
		vertexBufferDesc.ByteWidth = sizeof(VertexPosition) * static_cast<uint32_t>(vertices.size());
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData = { 0 };
		vertexSubResourceData.pSysMem = &vertices[0];
		ThrowIfFailed(mDeviceResources->GetD3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, mTriangleVertexBuffer.ReleaseAndGetAddressOf()));
	}
}
