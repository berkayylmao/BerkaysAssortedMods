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

#ifndef BAM_MODLODCONTROL_H
#define BAM_MODLODCONTROL_H
#pragma once

#include "pch.h"
#include "IMod.hpp"

namespace BAM::Extensions {
  class ModLODControl : public IMod {
    MemoryFieldWrapper<NFSPluginSDK::CARPART_LOD>* mCarLODField;
    MemoryFieldWrapper<NFSPluginSDK::CARPART_LOD>* mTireLODField;
    std::uint32_t                                  mCarLODLevel;
    std::uint32_t                                  mTireLODLevel;

   protected:
    virtual void _onEnabled() override {
      GameLoopWalker::ExecuteOnce([&]() {
        *mCarLODField  = static_cast<NFSPluginSDK::CARPART_LOD>(mCarLODLevel);
        *mTireLODField = static_cast<NFSPluginSDK::CARPART_LOD>(mTireLODLevel);
      });
    }
    virtual void _onDisabled() override {
      GameLoopWalker::ExecuteOnce([&]() {
        *mCarLODField  = NFSPluginSDK::CARPART_LOD::OFF;
        *mTireLODField = NFSPluginSDK::CARPART_LOD::OFF;
      });
    }

    virtual void _drawMenuContent() override {
      static const std::vector<const char*> sLODLevels = {"LODA", "LODB", "LODC", "LODD", "LODE"};

      ComboOption("Car LOD Level:", sLODLevels, &mCarLODLevel, [&]() {
        this->Refresh();

        this->GetConfig()["CarLODLevel"].SetUint(mCarLODLevel);
        this->SaveConfig();
      });
      ComboOption("Car Tire LOD Level:", sLODLevels, &mTireLODLevel, [&]() {
        this->Refresh();

        this->GetConfig()["TireLODLevel"].SetUint(mTireLODLevel);
        this->SaveConfig();
      });
    }

   public:
    ModLODControl() :
        IMod("LOD Control", "Forces all car LODs to the selected values."),
        mCarLODField(nullptr),
        mTireLODField(nullptr),
        mCarLODLevel(this->GetConfig()["CarLODLevel"].GetUint()),
        mTireLODLevel(this->GetConfig()["TireLODLevel"].GetUint()) {
      switch (Variables::sCurGameInfo.mGame) {
        case NFSPluginSDK::Game::MW05: {
          mCarLODField  = std::addressof(OpenMW::Variables::ForceCarLOD);
          mTireLODField = std::addressof(OpenMW::Variables::ForceTireLOD);
        } break;
        case NFSPluginSDK::Game::Carbon: {
          mCarLODField  = std::addressof(OpenCarbon::Variables::ForceCarLOD);
          mTireLODField = std::addressof(OpenCarbon::Variables::ForceTireLOD);
        } break;
        case NFSPluginSDK::Game::ProStreet: {
          mCarLODField  = std::addressof(OpenPS::Variables::ForceCarLOD);
          mTireLODField = std::addressof(OpenPS::Variables::ForceTireLOD);
        } break;
      }
    }
  };
}  // namespace BAM::Extensions

#endif  // BAM_MODLODCONTROL_H
