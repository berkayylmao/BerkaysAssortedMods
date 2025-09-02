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

#ifndef BAM_BLURPOWERUPS_IPOWERUP_H
#define BAM_BLURPOWERUPS_IPOWERUP_H
#pragma once

#include "pch.h"

namespace BAM::Extensions ::details::ModBlurPowerups {
  enum class PowerUpType : std::uint8_t { Bolt, Boost, Barge };

  struct IPowerUp {
   protected:
    enum class Direction : std::uint8_t { Forward, Backward };

    virtual bool _activate() { return true; }
    virtual bool _activateBackwards() { return true; }
    virtual void _onTick() {}

   public:
    /// CONFIG

    float mLockTime;

    /// VARIABLES

    float              mRemainingLockedTime;
    PowerUpType        mType;
    bool               mActivationLocked;
    bool               mPlaySoundFX;
    LPDIRECT3DTEXTURE9 mIconTexture;

    bool  OnActivate() { return _activate(); }
    bool  OnActivateBackwards() { return _activateBackwards(); }
    float GetRemainingLockedPercentage() {
      if (!mActivationLocked) return 0.0f;
      return std::clamp(1.0f - (mRemainingLockedTime / mLockTime), 0.0f, 1.0f);
    }

    virtual void OnTick() {
      if (mActivationLocked) {
        mRemainingLockedTime += ImGui::GetIO().DeltaTime;
        if (mRemainingLockedTime > mLockTime) {
          mActivationLocked    = false;
          mRemainingLockedTime = 0.0f;
        }
      }

      _onTick();
    }

    IPowerUp(PowerUpType type, float lockTime) :
        mType(type), mLockTime(lockTime), mRemainingLockedTime(0.0f), mActivationLocked(false), mPlaySoundFX(true), mIconTexture(nullptr) {}
    IPowerUp(PowerUpType type) : IPowerUp(type, 2.0f) {}
  };
}  // namespace BAM::Extensions::details::ModBlurPowerups

#endif  // BAM_BLURPOWERUPS_IPOWERUP_H
