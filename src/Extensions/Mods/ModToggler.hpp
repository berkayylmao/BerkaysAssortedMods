/*
// clang-format off
//
//    Berkay's Assorted Mods (Berkay's Assorted Mods)
//    Copyright (C) 2022 Berkay Yigit <mail@berkay.link>
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program. If not, see <https://www.gnu.org/licenses/>.
//
// clang-format on
*/

#ifndef BAM_MODTOGGLER_H
#define BAM_MODTOGGLER_H
#pragma once

#include "pch.h"
#include "IMod.hpp"

namespace BAM::Extensions {
  class ModToggler : public IMod {
    std::pair<MemoryFieldWrapper<bool>*, bool> mDisableDamageForAll;
    std::pair<MemoryFieldWrapper<bool>*, bool> mDrawCars;
    std::pair<MemoryFieldWrapper<bool>*, bool> mDrawCarShadows;
    std::pair<MemoryFieldWrapper<bool>*, bool> mDrawHUD;
    std::pair<MemoryFieldWrapper<bool>*, bool> mDrawLightFlares;
    std::pair<MemoryFieldWrapper<bool>*, bool> mInfiniteNOS;
    std::pair<MemoryFieldWrapper<bool>*, bool> mInfiniteSpeedbreaker;
    std::pair<MemoryFieldWrapper<bool>*, bool> mPauseCameraLock;
    std::pair<MemoryFieldWrapper<bool>*, bool> mRearViewModel;
    std::pair<MemoryFieldWrapper<bool>*, bool> mStopUpdatingCamera;
    std::pair<MemoryFieldWrapper<bool>*, bool> mTrafficStopSpawning;

    void setWrapperPointer(std::pair<MemoryFieldWrapper<bool>*, bool>& target, MemoryFieldWrapper<bool>* p) {
      if (!p) {
        target.first = nullptr;
        return;
      }

      target.first  = p;
      target.second = *p;
    }

    void drawOption(const char* name, std::pair<MemoryFieldWrapper<bool>*, bool>& option) {
      if (option.first) CheckboxOption(name, &option.second, [&]() { GameLoopWalker::ExecuteOnce([&]() { *option.first = option.second; }); });
    }

   protected:
    virtual void _drawMenuContent() override {
      drawOption("Disable damage for all:", mDisableDamageForAll);
      drawOption("Draw cars:", mDrawCars);
      drawOption("Draw car shadows:", mDrawCarShadows);
      drawOption("Draw HUD:", mDrawHUD);
      drawOption("Draw light flares:", mDrawLightFlares);
      drawOption("Infinite NOS:", mInfiniteNOS);
      drawOption("Infinite Speedbreaker:", mInfiniteSpeedbreaker);
      drawOption("Pause camera lock:", mPauseCameraLock);
      drawOption("Rearview model:", mRearViewModel);
      drawOption("Stop updating camera:", mStopUpdatingCamera);
      drawOption("Stop spawning traffic:", mTrafficStopSpawning);
    }

   public:
    ModToggler() : IMod("Toggler", "Enable or disable some of the game functions.", "Toggler", false, false) {
      setWrapperPointer(mDisableDamageForAll, nullptr);
      setWrapperPointer(mDrawCars, nullptr);
      setWrapperPointer(mDrawCarShadows, nullptr);
      setWrapperPointer(mDrawHUD, nullptr);
      setWrapperPointer(mDrawLightFlares, nullptr);
      setWrapperPointer(mStopUpdatingCamera, nullptr);
      setWrapperPointer(mInfiniteNOS, nullptr);
      setWrapperPointer(mInfiniteSpeedbreaker, nullptr);
      setWrapperPointer(mPauseCameraLock, nullptr);
      setWrapperPointer(mRearViewModel, nullptr);
      setWrapperPointer(mTrafficStopSpawning, nullptr);

      switch (Variables::sCurGameInfo.mGame) {
        case NFSPluginSDK::Game::MW05: {
          setWrapperPointer(mDrawCars, std::addressof(OpenMW::Variables::DrawCars));
          setWrapperPointer(mDrawHUD, std::addressof(OpenMW::Variables::DrawHUD));
          setWrapperPointer(mDrawLightFlares, std::addressof(OpenMW::Variables::DrawLightFlares));
          setWrapperPointer(mInfiniteNOS, std::addressof(OpenMW::Variables::Tweak_InfiniteNOS));
          setWrapperPointer(mInfiniteSpeedbreaker, std::addressof(OpenMW::Variables::Tweak_InfiniteRaceBreaker));
          setWrapperPointer(mPauseCameraLock, std::addressof(OpenMW::Variables::Tweak_PauseCameraLock));
          setWrapperPointer(mStopUpdatingCamera, std::addressof(OpenMW::Variables::StopUpdatingCamera));
        } break;
        case NFSPluginSDK::Game::Carbon: {
          setWrapperPointer(mDrawCars, std::addressof(OpenCarbon::Variables::DrawCars));
          setWrapperPointer(mDrawCarShadows, std::addressof(OpenCarbon::Variables::DrawCarShadow));
          setWrapperPointer(mDrawLightFlares, std::addressof(OpenCarbon::Variables::DrawLightFlares));
          setWrapperPointer(mStopUpdatingCamera, std::addressof(OpenCarbon::Variables::StopUpdatingCamera));
          setWrapperPointer(mInfiniteSpeedbreaker, std::addressof(OpenCarbon::Variables::Tweak_InfiniteRaceBreaker));
          setWrapperPointer(mPauseCameraLock, std::addressof(OpenCarbon::Variables::Tweak_PauseCameraLock));
          setWrapperPointer(mTrafficStopSpawning, std::addressof(OpenCarbon::Variables::Tweak_TrafficStopSpawning));
        } break;
        case NFSPluginSDK::Game::ProStreet: {
          static MemoryFieldWrapper<bool> sDisableDamage(0xAB9DCD, false);
          setWrapperPointer(mDisableDamageForAll, std::addressof(sDisableDamage));
          setWrapperPointer(mDrawCars, std::addressof(OpenPS::Variables::DrawCars));
          setWrapperPointer(mRearViewModel, std::addressof(OpenPS::Variables::RearViewModel));
        } break;
      }
    }
  };
}  // namespace BAM::Extensions

#endif  // BAM_MODTOGGLER_H
