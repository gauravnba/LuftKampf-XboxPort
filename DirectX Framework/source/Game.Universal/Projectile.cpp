#include "pch.h"
#include "Projectile.h"

using namespace DirectX;
using namespace DX;

namespace LuftKampf
{
	Projectile::Projectile(const Transform2D& transform, float radius, const XMFLOAT4& color, const XMFLOAT2& velocity, bool isSolid) :
		mTransform(transform), mRadius(radius),	mColor(color), mVelocity(velocity), mIsSolid(isSolid), mDestroy(false)
	{
	}

	const Transform2D& Projectile::Transform() const
	{
		return mTransform;
	}

	void Projectile::SetTransform(const Transform2D & transform)
	{
		mTransform = transform;
	}

	float Projectile::Radius() const
	{
		return mRadius;
	}

	void Projectile::SetRadius(const float radius)
	{
		mRadius = radius;
	}

	const XMFLOAT4& Projectile::Color() const
	{
		return mColor;
	}

	void Projectile::SetColor(const XMFLOAT4& color)
	{
		mColor = color;
	}

	const XMFLOAT2& Projectile::Velocity() const
	{
		return mVelocity;
	}

	void Projectile::SetVelocity(const XMFLOAT2& velocity)
	{
		mVelocity = velocity;
	}

	bool Projectile::IsSolid() const
	{
		return mIsSolid;
	}

	void Projectile::SetIsSolid(const bool isSolid)
	{
		mIsSolid = isSolid;
	}

	void Projectile::Update(const StepTimer& timer)
	{
		double elapsedTime = timer.GetElapsedSeconds();

		XMFLOAT2 position = mTransform.Position();
		position.x += mVelocity.x * static_cast<float>(elapsedTime);
		position.y += mVelocity.y * static_cast<float>(elapsedTime);

		mTransform.SetPosition(position);
	}

	void Projectile::MarkToDestroy()
	{
		mDestroy = true;
	}

	bool Projectile::IsMarkedForDestruction()
	{
		return mDestroy;
	}
}