// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_COMPONENTS_WANDERER
#define SSVNR_COMPONENTS_WANDERER

#include "Core/NRDependencies.hpp"

namespace nr
{
	class NRGame;
	class NRCHumanoid;

	class NRCWanderer : public sses::Component
	{
		private:
			NRGame& game;
			NRCHumanoid& cHumanoid;
			float time{200};
			int dir{0};
			bool tired{false};

		public:
			NRCWanderer(sses::Entity& mE, NRGame& mGame, NRCHumanoid& mCHumanoid);

			void update(FT mFT) override;
	};
}

#endif
