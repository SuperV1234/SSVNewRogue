// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_UTILS_UTILS
#define SSVNR_UTILS_UTILS

#include "Core/NRDependencies.h"

namespace nr
{
	class NRGame;

	template<typename T> inline constexpr float toPixels(T mValue)			{ return mValue / 100; }
	template<typename T> inline constexpr int toCoords(T mValue)			{ return mValue * 100; }
	template<typename T> inline ssvs::Vec2f toPixels(ssvs::Vec2<T> mValue)	{ return {toPixels(mValue.x), toPixels(mValue.y)}; }
	template<typename T> inline ssvs::Vec2i toCoords(ssvs::Vec2<T> mValue)	{ return {toCoords(mValue.x), toCoords(mValue.y)}; }

	// TODO: refactor or remove these
	sses::Entity* seekEntity(NRGame& mGame, ssvsc::Grid& mGrid, ssvsc::Body& mSeeker, ssvs::Vec2i mTarget, ssvs::Vec2i& mLastPos);
	sses::Entity* seekEntity(NRGame& mGame, ssvsc::Grid& mGrid, ssvsc::Body& mSeeker, ssvs::Vec2i mTarget, const std::string& mTargetGroup, const std::vector<std::string>& mIgnoreGroups, ssvs::Vec2i& mLastPos);
}

#endif
