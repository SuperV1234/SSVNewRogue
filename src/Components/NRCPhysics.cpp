// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Core/NRGame.hpp"
#include "Components/NRCPhysics.hpp"

using namespace ssvs;
using namespace sses;
using namespace std;
using namespace sf;
using namespace ssvu;

namespace nr
{
	NRCPhysics::NRCPhysics(World& mWorld, bool mIsStatic, const Vec2i& mPosition, const Vec2i& mSize, bool mAffectedByGravity)
		: world(mWorld), body(world.create(mPosition, mSize, mIsStatic)), affectedByGravity{mAffectedByGravity} { }
	NRCPhysics::~NRCPhysics() { body.destroy(); } // BUG: this has to be called before world is destroyed, or else SEGFAULT - find a way to avoid that!

	void NRCPhysics::init()
	{
		body.setUserData(&getEntity());

		body.onDetection += [&](const DetectionInfo& mDI)
		{
			if(mDI.userData == nullptr) return;
			Entity* entity(static_cast<Entity*>(mDI.userData));
			onDetection(*entity);
		};
		body.onResolution += [&](const ResolutionInfo& mRI)
		{
			onResolution(mRI.resolution);

			lastResolution = mRI.resolution;
			if(mRI.resolution.x > 0) crushedLeft = crushedMax;
			else if(mRI.resolution.x < 0) crushedRight = crushedMax;
			if(mRI.resolution.y > 0) crushedTop = crushedMax;
			else if(mRI.resolution.y < 0) crushedBottom = crushedMax;
		};
		body.onPreUpdate += [&]
		{
			lastResolution = ssvs::zeroVec2i;
			if(crushedLeft > 0) --crushedLeft;
			if(crushedRight > 0) --crushedRight;
			if(crushedTop > 0) --crushedTop;
			if(crushedBottom > 0) --crushedBottom;
		};
	}

	void NRCPhysics::update(FT)
	{
		if(affectedByGravity && body.getVelocity().y < maxVelocityY) body.applyAccel(gravityForce);
	}
}
