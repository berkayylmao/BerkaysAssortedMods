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

#ifndef BAM_BLURPOWERUPS_BARGEPOWERUP_H
#define BAM_BLURPOWERUPS_BARGEPOWERUP_H
#pragma once

#include "pch.h"
#include "Extensions/Mods/ModBlurPowerups.IPowerUp.hpp"
#include "Extensions/Mods/ModBlurPowerups.Textures.hpp"

namespace BAM::Extensions ::details::ModBlurPowerups {
  class BargePowerUp : public IPowerUp {
    void pushObjectsAway() {
      if (mActivationLocked) return;

      GameLoopWalker::ExecuteOnce([&]() {
        switch (Variables::sCurGameInfo.mGame) {
          case NFSPluginSDK::Game::MW05: {
            auto* player_pvehicle = OpenMW::PVehicleEx::GetPlayerInstance();
            if (!player_pvehicle) return false;

            const auto& player_pos      = player_pvehicle->GetPosition();
            const auto  danger_zone_min = player_pos - OpenMW::UMath::Vector3(mRadius, mRadius, mRadius);
            const auto  danger_zone_max = player_pos + OpenMW::UMath::Vector3(mRadius, mRadius, mRadius);

            OpenMW::PVehicleEx::ForEachInstance([=](OpenMW::PVehicle* pvehicle) {
              if (pvehicle->IsPlayer()) return;

              const auto& pos = pvehicle->GetPosition();
              if (pos.x > danger_zone_min.x && pos.x < danger_zone_max.x && pos.y > danger_zone_min.y && pos.y < danger_zone_max.y &&
                  pos.z > danger_zone_min.z && pos.z < danger_zone_max.z) {
                const auto v = (player_pos - pos).Normalize();

                pvehicle->GetRigidBody()->SetLinearVelocity(OpenMW::UMath::Vector3(
                    // x
                    v.x * -mForce,
                    // y
                    v.y * -mForce,
                    // z
                    0.0f));
              }
            });
          } break;
          case NFSPluginSDK::Game::Carbon: {
            auto* player_pvehicle = OpenCarbon::PVehicleEx::GetPlayerInstance();
            if (!player_pvehicle) return false;

            const auto& player_pos      = player_pvehicle->GetPosition();
            const auto  danger_zone_min = player_pos - OpenCarbon::UMath::Vector3(mRadius, mRadius, mRadius);
            const auto  danger_zone_max = player_pos + OpenCarbon::UMath::Vector3(mRadius, mRadius, mRadius);

            OpenCarbon::PVehicleEx::ForEachInstance([=](OpenCarbon::PVehicle* pvehicle) {
              if (pvehicle->IsPlayer()) return;

              const auto& pos = pvehicle->GetPosition();
              if (pos.x > danger_zone_min.x && pos.x < danger_zone_max.x && pos.y > danger_zone_min.y && pos.y < danger_zone_max.y &&
                  pos.z > danger_zone_min.z && pos.z < danger_zone_max.z) {
                const auto v = (player_pos - pos).Normalize();

                pvehicle->GetRigidBody()->SetLinearVelocity(OpenCarbon::UMath::Vector3(
                    // x
                    v.x * -mForce,
                    // y
                    v.y * -mForce,
                    // z
                    0.0f));
              }
            });
          } break;
          case NFSPluginSDK::Game::ProStreet: {
            auto* player_pvehicle = OpenPS::PVehicleEx::GetPlayerInstance();
            if (!player_pvehicle) return false;

            const auto& player_pos      = player_pvehicle->GetPosition();
            const auto  danger_zone_min = player_pos - OpenPS::UMath::Vector3(mRadius, mRadius, mRadius);
            const auto  danger_zone_max = player_pos + OpenPS::UMath::Vector3(mRadius, mRadius, mRadius);

            OpenPS::PVehicleEx::ForEachInstance([=](OpenPS::PVehicle* pvehicle) {
              if (pvehicle->IsPlayer()) return;

              const auto& pos = pvehicle->GetPosition();
              if (pos.x > danger_zone_min.x && pos.x < danger_zone_max.x && pos.y > danger_zone_min.y && pos.y < danger_zone_max.y &&
                  pos.z > danger_zone_min.z && pos.z < danger_zone_max.z) {
                const auto v = (player_pos - pos).Normalize();

                pvehicle->GetRigidBody()->SetLinearVelocity(OpenPS::UMath::Vector3(
                    // x
                    v.x * -mForce,
                    // y
                    v.y * -mForce,
                    // z
                    0.0f));
              }
            });
          } break;
        }

        mActivationLocked = true;
        if (mShakeCamera) {
          switch (Variables::sCurGameInfo.mGame) {
            case NFSPluginSDK::Game::MW05: {
              OpenMW::Game::ShakeCamera();
            } break;
            case NFSPluginSDK::Game::Carbon: {
              OpenCarbon::Game::ShakeCamera();
            } break;
            case NFSPluginSDK::Game::ProStreet: {
              OpenPS::Game::ShakeCamera();
            } break;
          }
        }

        if (mPlaySoundFX) FMODWrapper::Get().PlaySoundFX(FMODWrapper::SoundFX::BlurBargePowerUp);
        return true;
      });
    }

   protected:
    virtual bool _activate() override {
      pushObjectsAway();
      return true;
    }
    virtual bool _activateBackwards() override {
      pushObjectsAway();
      return true;
    }

   public:
    /// CONFIG

    bool  mShakeCamera;
    float mRadius;
    float mForce;

    void CreateTexture(LPDIRECT3DDEVICE9 pDevice) {
      if (mIconTexture) {
        mIconTexture->Release();
        mIconTexture = nullptr;
      }

      D3DXCreateTextureFromFileInMemory(pDevice, sBargeTexture.data(), sBargeTexture.size(), &this->mIconTexture);
    }

    BargePowerUp(rapidjson::Value& configObj) :
        IPowerUp(PowerUpType::Barge, configObj["LockTime"].GetFloat()),
        mShakeCamera(configObj["ShakeCamera"].GetBool()),
        mRadius(configObj["EffectRadius"].GetFloat()),
        mForce(configObj["EffectForce"].GetFloat()) {}
  };
}  // namespace BAM::Extensions::details::ModBlurPowerups

#endif  // BAM_BLURPOWERUPS_BARGEPOWERUP_H
