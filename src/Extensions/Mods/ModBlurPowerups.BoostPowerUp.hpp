/*
// clang-format off
//
//    Berkay's Assorted Mods (Berkay's Assorted Mods)
//    Copyright (C) 2025 Berkay Yiğit <contact@withberkay.com>
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

#ifndef BAM_BLURPOWERUPS_BOOSTPOWERUP_H
#define BAM_BLURPOWERUPS_BOOSTPOWERUP_H
#pragma once

#include "pch.h"
#include "Extensions/Mods/ModBlurPowerups.IPowerUp.hpp"
#include "Extensions/Mods/ModBlurPowerups.Textures.hpp"

namespace BAM::Extensions ::details::ModBlurPowerups {
  class BoostPowerUp : public IPowerUp {
    /// VARIABLES

    bool mNeedToSuperBoostPlayer;

    void doBoost(Direction direction) {
      if (mActivationLocked) return;

      GameLoopWalker::ExecuteOnce([&, direction]() {
        switch (Variables::sCurGameInfo.mGame) {
          case NFSPluginSDK::Game::MW05: {
            auto* pvehicle = OpenMW::PVehicleEx::GetPlayerInstance();
            if (!pvehicle) return false;

            if (direction == Direction::Forward) {
              pvehicle->SetSpeed(std::clamp(pvehicle->GetSpeed() * mMultiplier, 40.0f, 120.0f + Random::Get().Generate(1.0f, 10.0f)));
            } else {
              pvehicle->SetSpeed(4.0f);  // 15km/h
              mNeedToSuperBoostPlayer = true;
            }
          } break;
          case NFSPluginSDK::Game::Carbon: {
            auto* pvehicle = OpenCarbon::PVehicleEx::GetPlayerInstance();
            if (!pvehicle) return false;

            if (direction == Direction::Forward) {
              pvehicle->SetSpeed(std::clamp(pvehicle->GetSpeed() * mMultiplier, 40.0f, 120.0f + Random::Get().Generate(1.0f, 10.0f)));
            } else {
              pvehicle->SetSpeed(4.0f);  // 15km/h
              mNeedToSuperBoostPlayer = true;
            }
          } break;
          case NFSPluginSDK::Game::ProStreet: {
            auto* pvehicle = OpenPS::PVehicleEx::GetPlayerInstance();
            if (!pvehicle) return false;

            if (direction == Direction::Forward) {
              pvehicle->SetSpeed(std::clamp(pvehicle->GetSpeed() * mMultiplier, 40.0f, 120.0f + Random::Get().Generate(1.0f, 10.0f)));
            } else {
              pvehicle->SetSpeed(4.0f);  // 15km/h
              mNeedToSuperBoostPlayer = true;
            }
          } break;
        }

        mActivationLocked = true;
        if (mPlaySoundFX)
          FMODWrapper::Get().PlaySoundFX(direction == Direction::Forward ? FMODWrapper::SoundFX::BlurBoostForwardPowerUp
                                                                         : FMODWrapper::SoundFX::BlurBoostBackwardPowerUp);
        return true;
      });
    }

   protected:
    virtual bool _activate() override {
      doBoost(Direction::Forward);
      return true;
    }
    virtual bool _activateBackwards() override {
      doBoost(Direction::Backward);
      return true;
    }
    virtual void _onTick() override {
      if (!mNeedToSuperBoostPlayer) return;

      static float dt = 0.0f;
      dt += ImGui::GetIO().DeltaTime;
      if (dt < 1.5f) return;
      dt = 0.0f;

      switch (Variables::sCurGameInfo.mGame) {
        case NFSPluginSDK::Game::MW05: {
          auto* pvehicle = OpenMW::PVehicleEx::GetPlayerInstance();
          if (!pvehicle) return;

          pvehicle->SetSpeed(80.0f);  // 288 km/h
        } break;
        case NFSPluginSDK::Game::Carbon: {
          auto* pvehicle = OpenCarbon::PVehicleEx::GetPlayerInstance();
          if (!pvehicle) return;

          pvehicle->SetSpeed(80.0f);  // 288 km/h
        } break;
        case NFSPluginSDK::Game::ProStreet: {
          auto* pvehicle = OpenPS::PVehicleEx::GetPlayerInstance();
          if (!pvehicle) return;

          pvehicle->SetSpeed(80.0f);  // 288 km/h
        } break;
      }

      if (mPlaySoundFX) FMODWrapper::Get().PlaySoundFX(FMODWrapper::SoundFX::BlurBoostForwardPowerUp);
      mNeedToSuperBoostPlayer = false;
    }

   public:
    /// CONFIG

    float mMultiplier;

    void CreateTexture(LPDIRECT3DDEVICE9 pDevice) {
      if (mIconTexture) {
        mIconTexture->Release();
        mIconTexture = nullptr;
      }

      D3DXCreateTextureFromFileInMemory(pDevice, sBoostTexture.data(), sBoostTexture.size(), &this->mIconTexture);
    }

    BoostPowerUp(rapidjson::Value& configObj) :
        IPowerUp(PowerUpType::Boost, configObj["LockTime"].GetFloat()),
        mMultiplier(configObj["BoostMultiplier"].GetFloat()),
        mNeedToSuperBoostPlayer(false) {}
  };
}  // namespace BAM::Extensions::details::ModBlurPowerups

#endif  // BAM_BLURPOWERUPS_BOOSTPOWERUP_H
