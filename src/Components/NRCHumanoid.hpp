// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_COMPONENTS_HUMANOID
#define SSVNR_COMPONENTS_HUMANOID

#include "Core/NRDependencies.hpp"
#include "Utils/NRSensor.hpp"

namespace nr
{
    class NRGame;
    class NRCPhysics;

    class NRCHumanoid : public sses::Component
    {
    public:
        enum class Action
        {
            Walking,
            Running,
            Crouching,
            CrouchWalking,
            Standing,
            Jumping,
            Falling
        };

    private:
        NRCPhysics& cPhysics;
        NRSensor unCrouchSensor, autoCrouchTopSensor, autoCrouchBottomSensor;
        Body& body;
        int standingHeight;
        Action action;
        bool facingLeft{false}, jumpReady{false}, crouching{false},
            autoCrouching{false};
        float crouchSpeed{40.f}, walkSpeed{50.f}, runSpeed{150.f},
            jumpSpeed{420.f};
        int crouchingHeight{900};

    public:
        // ssvrpg::Value<int> additionalSpeed{0};
        // ssvu::MonoManager<ssvrpg::Modifier<int>> modifierManager;

        NRCHumanoid(sses::Entity& mE, NRCPhysics& mCPhysics);

        void update(FT mFT) override;

        void unCrouch();
        void crouch(bool mForce = false);
        void move(int mDirection, bool mWalk);
        void jump();

        inline bool isFacingLeft() { return facingLeft; }
        inline bool isJumpReady() { return jumpReady; }
        inline bool isInAir()
        {
            return body.getShape().getY() != body.getOldShape().getY();
        }
        inline Action getAction() { return action; }
    };
}

#endif
