// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_COMPONENTS_ANIMATIONCONTROLLER
#define SSVNR_COMPONENTS_ANIMATIONCONTROLLER

#include "Core/NRDependencies.hpp"
#include "Utils/NRUtils.hpp"

namespace nr
{
    class NRCRender;
    class NRCHumanoid;

    class NRCHumanoidAnimationController : public sses::Component
    {
    private:
        NRCRender& cRender;
        NRCHumanoid& cHumanoid;

        ssvs::Tileset tileset{ssvj::fromFile("Data/Tilesets/tilesetHuman.json")
                                  .as<ssvs::Tileset>()};
        ssvj::Val animations{
            ssvj::fromFile("Data/Animations/animationsHuman.json")};
        ssvs::Animation animStand{
            ssvs::getAnimationFromJson(tileset, animations["stand"])};
        ssvs::Animation animJump{
            ssvs::getAnimationFromJson(tileset, animations["jump"])};
        ssvs::Animation animFall{
            ssvs::getAnimationFromJson(tileset, animations["fall"])};
        ssvs::Animation animCrouch{
            ssvs::getAnimationFromJson(tileset, animations["crouch"])};
        ssvs::Animation animRun{
            ssvs::getAnimationFromJson(tileset, animations["run"])};
        ssvs::Animation animWalk{
            ssvs::getAnimationFromJson(tileset, animations["walk"])};
        ssvs::Animation animCrouchWalk{
            ssvs::getAnimationFromJson(tileset, animations["crouchWalk"])};
        ssvs::Animation* currentAnim{nullptr};

    public:
        NRCHumanoidAnimationController(
            sses::Entity& mE, NRCRender& mCRender, NRCHumanoid& mCHumanoid);

        void update(FT mFT) override;
    };
}

#endif
