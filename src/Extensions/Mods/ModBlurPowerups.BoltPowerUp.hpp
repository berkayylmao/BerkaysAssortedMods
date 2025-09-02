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

#ifndef BAM_BLURPOWERUPS_BOLTPOWERUP_H
#define BAM_BLURPOWERUPS_BOLTPOWERUP_H
#pragma once

#include "pch.h"
#include "Extensions/Mods/ModBlurPowerups.IPowerUp.hpp"
#include "Extensions/Mods/ModBlurPowerups.Textures.hpp"

namespace BAM::Extensions ::details::ModBlurPowerups {
  class BoltPowerUp : public IPowerUp {
    /// VARIABLES

    float                 mBoltScreenTime;
    OpenMW::PVehicle*     mBoltMW05;
    OpenCarbon::PVehicle* mBoltCarbon;
    OpenPS::PVehicle*     mBoltPS;

    void sendBolt(Direction boltDirection) {
      if (mActivationLocked) return;

      GameLoopWalker::ExecuteOnce([&, boltDirection]() {
        switch (Variables::sCurGameInfo.mGame) {
          case NFSPluginSDK::Game::MW05: {
            auto* pvehicle = OpenMW::PVehicleEx::GetPlayerInstance();
            if (!pvehicle) return false;

            OpenMW::UMath::Vector3 position = pvehicle->GetPosition();
            OpenMW::UMath::Vector3 direction;
            pvehicle->GetRigidBody()->GetForwardVector(direction);

            if (boltDirection == Direction::Forward) {
              position.x += direction.x * 2.66f;
              position.y += direction.y * 2.66f;
            } else {
              position.x -= direction.x * 2.66f;
              position.y -= direction.y * 2.66f;

              direction.x *= -1.0f;
              direction.y *= -1.0f;
            }

            if (!(mBoltMW05 | OpenMW::PVehicleEx::ValidatePVehicle) || mBoltMW05->IsPlayer()) {
              auto* spawned = OpenMW::PVehicle::Construct(OpenMW::VehicleParams(OpenMW::DriverClass::Remote, OpenMW::Attrib::StringToKey("trafpizza"),
                                                                                direction, position, nullptr, OpenMW::eVehicleParamFlags::SnapToGround)) |
                              OpenMW::PVehicleEx::ValidatePVehicle;
              if (!spawned) return false;

              mBoltMW05 = spawned;
            }

            mBoltMW05->SetVehicleOnGround(position, direction);
            mBoltMW05->GetRigidBody()->SetMass(1000.0f * mBoltStrength);
            mBoltMW05->SetSpeed(200.0f * mBoltStrength);
          } break;
          case NFSPluginSDK::Game::Carbon: {
            auto* pvehicle = OpenCarbon::PVehicleEx::GetPlayerInstance();
            if (!pvehicle) return false;

            OpenCarbon::UMath::Vector3 position = pvehicle->GetPosition();
            OpenCarbon::UMath::Vector3 direction;
            pvehicle->GetRigidBody()->GetForwardVector(direction);

            if (boltDirection == Direction::Forward) {
              position.x += direction.x * 2.66f;
              position.y += direction.y * 2.66f;
            } else {
              position.x -= direction.x * 2.66f;
              position.y -= direction.y * 2.66f;

              direction.x *= -1.0f;
              direction.y *= -1.0f;
            }

            if (!(mBoltCarbon | OpenCarbon::PVehicleEx::ValidatePVehicle) || mBoltCarbon->IsPlayer()) {
              auto instance = OpenCarbon::Attrib::Gen::pvehicle::TryGetInstance("traftaxi");
              if (!instance.mCollection) return false;

              auto* spawned = OpenCarbon::PVehicle::Construct(OpenCarbon::VehicleParams(OpenCarbon::DriverClass::Remote, instance, direction, position, nullptr,
                                                                                        OpenCarbon::eVehicleParamFlags::SnapToGround)) |
                              OpenCarbon::PVehicleEx::ValidatePVehicle;
              if (!spawned) return false;

              mBoltCarbon = spawned;
            }

            mBoltCarbon->SetVehicleOnGround(position, direction);
            mBoltCarbon->GetRigidBody()->SetMass(1000.0f * mBoltStrength);
            mBoltCarbon->SetSpeed(200.0f * mBoltStrength);
          } break;
          case NFSPluginSDK::Game::ProStreet: {
            auto* pvehicle = OpenPS::PVehicleEx::GetPlayerInstance();
            if (!pvehicle) return false;

            OpenPS::UMath::Vector3 position = pvehicle->GetPosition();
            OpenPS::UMath::Vector3 direction;
            pvehicle->GetRigidBody()->GetForwardVector(direction);

            if (boltDirection == Direction::Forward) {
              position.x += direction.x * 2.66f;
              position.y += direction.y * 2.66f;
            } else {
              position.x -= direction.x * 2.66f;
              position.y -= direction.y * 2.66f;

              direction.x *= -1.0f;
              direction.y *= -1.0f;
            }

            if (!(mBoltPS | OpenPS::PVehicleEx::ValidatePVehicle) || mBoltPS->IsPlayer()) {
              auto instance = OpenPS::Attrib::Gen::pvehicle::TryGetInstance("gti");
              if (!instance.mCollection) return false;

              auto* spawned = OpenPS::PVehicle::Construct(OpenPS::VehicleParams(OpenPS::DriverClass::Remote, instance, direction, position, nullptr,
                                                                                OpenPS::eVehicleParamFlags::SnapToGround)) |
                              OpenPS::PVehicleEx::ValidatePVehicle;
              if (!spawned) return false;

              mBoltPS = spawned;
              mBoltPS->SetDriverStyle(OpenPS::DriverStyle::None);
              mBoltPS->SetDriverAidLevel(OpenPS::DriverAidType::AntiBrakeLock, OpenPS::eDriverAssistLevel::King, true);
              mBoltPS->SetDriverAidLevel(OpenPS::DriverAidType::BrakingAssist, OpenPS::eDriverAssistLevel::King, true);
              mBoltPS->SetDriverAidLevel(OpenPS::DriverAidType::RacelineAssist, OpenPS::eDriverAssistLevel::King, true);
              mBoltPS->SetDriverAidLevel(OpenPS::DriverAidType::StabilityManagement, OpenPS::eDriverAssistLevel::King, true);
              mBoltPS->SetDriverAidLevel(OpenPS::DriverAidType::TractionControl, OpenPS::eDriverAssistLevel::King, true);
            }

            mBoltPS->mOnScreenTime = 0.0f;
            mBoltPS->SetVehicleOnGround(position, direction, true);
            mBoltPS->GetRigidBody()->SetMass(1000.0f * mBoltStrength);
            mBoltPS->SetSpeed(200.0f * mBoltStrength);
          } break;
        }

        mActivationLocked = true;
        mBoltScreenTime   = 0.0f;
        if (mPlaySoundFX) FMODWrapper::Get().PlaySoundFX(FMODWrapper::SoundFX::BlurBoltPowerUp);
        return true;
      });
    }

   protected:
    virtual bool _activate() override {
      sendBolt(Direction::Forward);
      return true;
    }
    virtual bool _activateBackwards() override {
      sendBolt(Direction::Backward);
      return true;
    }
    virtual void _onTick() override {
      static bool move = false;

      switch (Variables::sCurGameInfo.mGame) {
        case NFSPluginSDK::Game::MW05: {
          if (mBoltMW05 | OpenMW::PVehicleEx::ValidatePVehicle) {
            mBoltScreenTime += std::clamp(mBoltMW05->GetOnScreenTime() - mBoltScreenTime, 0.0f, FLT_MAX);
            move = mBoltScreenTime > 5.0f;

            if (mDestroyBoltOnCollision) {
              if (auto* col = mBoltMW05->mCollisionBody) move |= col->HasHadCollision() || col->HasHadWorldCollision();
              move |= mBoltMW05->InShock();
            }

            if (move) {
              mBoltMW05->SetAnimating(true);
              mBoltMW05->mRigidBody->SetPosition(OpenMW::UMath::Vector3());
              mBoltMW05->SetAnimating(false);
            }
          }
        } break;
        case NFSPluginSDK::Game::Carbon: {
          if (mBoltCarbon | OpenCarbon::PVehicleEx::ValidatePVehicle) {
            mBoltScreenTime += std::clamp(mBoltCarbon->GetOnScreenTime() - mBoltScreenTime, 0.0f, FLT_MAX);
            move = mBoltScreenTime > 5.0f;

            if (mDestroyBoltOnCollision) {
              if (auto* col = mBoltCarbon->mCollisionBody) move |= col->HasHadCollision() || col->HasHadWorldCollision();
              move |= mBoltCarbon->InShock();
            }

            if (move) {
              mBoltCarbon->SetAnimating(true);
              mBoltCarbon->mRigidBody->SetPosition(OpenCarbon::UMath::Vector3());
              mBoltCarbon->SetAnimating(false);
            }
          }
        } break;
        case NFSPluginSDK::Game::ProStreet: {
          if (mBoltPS | OpenPS::PVehicleEx::ValidatePVehicle) {
            mBoltScreenTime += std::clamp(mBoltPS->GetOnScreenTime() - mBoltScreenTime, 0.0f, FLT_MAX);
            move = mBoltScreenTime > 5.0f;

            if (mDestroyBoltOnCollision) {
              if (auto* col = mBoltPS->mCollisionBody) move |= col->HasHadCollision() || col->HasHadWorldCollision();
              move |= mBoltPS->InShock();
            }

            if (move) {
              mBoltPS->SetAnimating(true);
              mBoltPS->mRigidBody->SetPosition(OpenPS::UMath::Vector3(-1000.0f, -1000.0f, -1000.0f));
              mBoltPS->SetAnimating(false);
            }
          }
        } break;
      }

      move = false;
    }

   public:
    /// CONFIG

    float mBoltStrength;
    bool  mDestroyBoltOnCollision;

    void CreateTexture(LPDIRECT3DDEVICE9 pDevice) {
      if (mIconTexture) {
        mIconTexture->Release();
        mIconTexture = nullptr;
      }

      D3DXCreateTextureFromFileInMemory(pDevice, sBoltTexture.data(), sBoltTexture.size(), &this->mIconTexture);
    }

    BoltPowerUp(rapidjson::Value& configObj) :
        IPowerUp(PowerUpType::Bolt, configObj["LockTime"].GetFloat()),
        mBoltStrength(configObj["BoltStrength"].GetFloat()),
        mDestroyBoltOnCollision(configObj["DestroyBoltOnCollision"].GetBool()),
        mBoltScreenTime(0.0f),
        mBoltMW05(nullptr),
        mBoltCarbon(nullptr),
        mBoltPS(nullptr) {}
  };
}  // namespace BAM::Extensions::details::ModBlurPowerups

#endif  // BAM_BLURPOWERUPS_BOLTPOWERUP_H
