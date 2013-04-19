#include "Components/NRCHumanoidAnimationController.h"
#include "Components/NRCRender.h"
#include "Components/NRCHumanoid.h"

using namespace ssvs;
using namespace sses;
using namespace std;
using namespace sf;
using namespace ssvsc;
using namespace ssvu;
using namespace ssvuj;

namespace nr
{
	using Action = NRCHumanoid::Action;

	NRCHumanoidAnimationController::NRCHumanoidAnimationController(Entity& mEntity, NRCRender& mCRender, NRCHumanoid& mCHumanoid) : Component(mEntity, "humanoidAnimationController"),
		cRender(mCRender), cHumanoid(mCHumanoid) { }

	void NRCHumanoidAnimationController::update(float mFrameTime)
	{
		cRender.setFlippedX(cHumanoid.isFacingLeft());
		cRender.setOffset({0, -1});

		Action action(cHumanoid.getAction());

		switch(action)
		{
			case Action::WALKING: currentAnim = &animWalk; break;
			case Action::RUNNING: currentAnim = &animRun; break;
			case Action::STANDING: currentAnim = &animStand; break;
			case Action::JUMPING: currentAnim = &animJump; break;
			case Action::FALLING: currentAnim = &animFall; break;
			case Action::CROUCHING: cRender.setOffset({0, -3}); currentAnim = &animCrouch; break;
			case Action::CROUCHWALKING: cRender.setOffset({0, -3}); currentAnim = &animCrouchWalk; break;
		}

		if(currentAnim == nullptr) return;
		currentAnim->update(mFrameTime);
		for(auto& s : cRender.getSprites()) s.setTextureRect(tileset[currentAnim->getCurrentLabel()]);
	}
}
