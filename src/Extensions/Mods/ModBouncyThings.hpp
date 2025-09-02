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

#ifndef BAM_MODBOUNCYTHINGS_H
#define BAM_MODBOUNCYTHINGS_H
#pragma once

#include "pch.h"
#include "IMod.hpp"

namespace BAM::Extensions {
  class ModBouncyThings : public IMod {
    /// BACKUPS

    std::unordered_map<std::uint32_t, NFSPluginSDK::Vector4> mObjectBackups;
    std::unordered_map<std::uint32_t, NFSPluginSDK::Vector4> mWallBackups;

    /// VARIABLES

    std::unordered_map<std::uint32_t, NFSPluginSDK::Vector4> mObjectMultipliers;
    std::unordered_map<std::uint32_t, NFSPluginSDK::Vector4> mWallMultipliers;

    /// CONFIG

    float mMultiplier;

   protected:
    virtual void _init() override {
      const std::array<const char*, 10> list_special = {"vehicle",  "cars",      "cops",    "copsuv",
                                                        "racers",   "tractors",  "traffic", "chopper",
                                                        "trailers", "rubbertire"};
      std::vector<std::uint32_t>        has_special;

      switch (Variables::sCurGameInfo.mGame) {
        case NFSPluginSDK::Game::MW05: {
          while (!OpenMW::Attrib::Database::Get()) std::this_thread::sleep_for(100ms);
          for (const auto& str : list_special) has_special.push_back(OpenMW::Attrib::StringToKey(str));

          OpenMW::RigidBodySpecsEx::ForEach([&](OpenMW::Attrib::Collection* c, OpenMW::Attrib::Layouts::rigidbodyspecslayout* layout) {
            mObjectBackups[c->mKey] = layout->OBJ_ELASTICITY;
            mWallBackups[c->mKey]   = layout->WALL_ELASTICITY;

            if (std::find(std::cbegin(has_special), std::cend(has_special), c->mKey) == std::cend(has_special)) {
              mObjectMultipliers[c->mKey] = NFSPluginSDK::Vector4(1.5f, 1.5f, 1.5f, 3.0f);
              mWallMultipliers[c->mKey]   = NFSPluginSDK::Vector4(2.0f, 2.0f, 2.0f, 4.0f);
            } else {
              mObjectMultipliers[c->mKey] = NFSPluginSDK::Vector4(1.0f, 1.0f, 1.0f, 2.0f);
              mWallMultipliers[c->mKey]   = NFSPluginSDK::Vector4(1.5f, 1.5f, 1.5f, 3.0f);
            }
          });
        } break;
        case NFSPluginSDK::Game::Carbon: {
          while (!OpenCarbon::Attrib::Database::Get()) std::this_thread::sleep_for(100ms);
          for (const auto& str : list_special) has_special.push_back(OpenCarbon::Attrib::StringToKey(str));

          OpenCarbon::RigidBodySpecsEx::ForEach([&](OpenCarbon::Attrib::Collection* c, OpenCarbon::Attrib::Layouts::rigidbodyspecslayout* layout) {
            mObjectBackups[c->mKey] = layout->OBJ_ELASTICITY;
            mWallBackups[c->mKey]   = layout->WALL_ELASTICITY;

            if (std::find(std::cbegin(has_special), std::cend(has_special), c->mKey) == std::cend(has_special)) {
              mObjectMultipliers[c->mKey] = NFSPluginSDK::Vector4(1.5f, 1.5f, 1.5f, 3.0f);
              mWallMultipliers[c->mKey]   = NFSPluginSDK::Vector4(2.0f, 2.0f, 2.0f, 4.0f);
            } else {
              mObjectMultipliers[c->mKey] = NFSPluginSDK::Vector4(1.0f, 1.0f, 1.0f, 2.0f);
              mWallMultipliers[c->mKey]   = NFSPluginSDK::Vector4(1.5f, 1.5f, 1.5f, 3.0f);
            }
          });
        } break;
        case NFSPluginSDK::Game::ProStreet: {
          while (!OpenPS::Attrib::Database::Get()) std::this_thread::sleep_for(100ms);
          for (const auto& str : list_special) has_special.push_back(OpenPS::Attrib::StringToKey(str));

          OpenPS::RigidBodySpecsEx::ForEach([&](OpenPS::Attrib::Collection* c, OpenPS::Attrib::Layouts::rigidbodyspecslayout* layout) {
            mObjectBackups[c->mKey] = layout->OBJ_ELASTICITY;
            mWallBackups[c->mKey]   = layout->WALL_ELASTICITY;

            if (std::find(std::cbegin(has_special), std::cend(has_special), c->mKey) == std::cend(has_special)) {
              mObjectMultipliers[c->mKey] = NFSPluginSDK::Vector4(1.5f, 1.5f, 1.5f, 3.0f);
              mWallMultipliers[c->mKey]   = NFSPluginSDK::Vector4(2.0f, 2.0f, 2.0f, 4.0f);
            } else {
              mObjectMultipliers[c->mKey] = NFSPluginSDK::Vector4(1.0f, 1.0f, 1.0f, 2.0f);
              mWallMultipliers[c->mKey]   = NFSPluginSDK::Vector4(1.5f, 1.5f, 1.5f, 3.0f);
            }
          });
        } break;
      }
    }

    virtual void _onEnabled() override {
      GameLoopWalker::ExecuteOnce([&]() {
        switch (Variables::sCurGameInfo.mGame) {
          case NFSPluginSDK::Game::MW05: {
            OpenMW::RigidBodySpecsEx::ForEach([&](OpenMW::Attrib::Collection* c, OpenMW::Attrib::Layouts::rigidbodyspecslayout* layout) {
              layout->OBJ_ELASTICITY  = mObjectMultipliers[c->mKey] * mMultiplier;
              layout->WALL_ELASTICITY = mWallMultipliers[c->mKey] * mMultiplier;
            });
          } break;
          case NFSPluginSDK::Game::Carbon: {
            OpenCarbon::RigidBodySpecsEx::ForEach([&](OpenCarbon::Attrib::Collection* c, OpenCarbon::Attrib::Layouts::rigidbodyspecslayout* layout) {
              layout->OBJ_ELASTICITY  = mObjectMultipliers[c->mKey] * mMultiplier;
              layout->WALL_ELASTICITY = mWallMultipliers[c->mKey] * mMultiplier;
            });
          } break;
          case NFSPluginSDK::Game::ProStreet: {
            OpenPS::RigidBodySpecsEx::ForEach([&](OpenPS::Attrib::Collection* c, OpenPS::Attrib::Layouts::rigidbodyspecslayout* layout) {
              layout->OBJ_ELASTICITY  = mObjectMultipliers[c->mKey] * mMultiplier;
              layout->WALL_ELASTICITY = mWallMultipliers[c->mKey] * mMultiplier;
            });
          } break;
        }
      });
    }
    virtual void _onDisabled() override {
      GameLoopWalker::ExecuteOnce([&]() {
        switch (Variables::sCurGameInfo.mGame) {
          case NFSPluginSDK::Game::MW05: {
            OpenMW::RigidBodySpecsEx::ForEach([&](OpenMW::Attrib::Collection* c, OpenMW::Attrib::Layouts::rigidbodyspecslayout* layout) {
              layout->OBJ_ELASTICITY  = mObjectBackups[c->mKey];
              layout->WALL_ELASTICITY = mWallBackups[c->mKey];
            });
          } break;
          case NFSPluginSDK::Game::Carbon: {
            OpenCarbon::RigidBodySpecsEx::ForEach([&](OpenCarbon::Attrib::Collection* c, OpenCarbon::Attrib::Layouts::rigidbodyspecslayout* layout) {
              layout->OBJ_ELASTICITY  = mObjectBackups[c->mKey];
              layout->WALL_ELASTICITY = mWallBackups[c->mKey];
            });
          } break;
          case NFSPluginSDK::Game::ProStreet: {
            OpenPS::RigidBodySpecsEx::ForEach([&](OpenPS::Attrib::Collection* c, OpenPS::Attrib::Layouts::rigidbodyspecslayout* layout) {
              layout->OBJ_ELASTICITY  = mObjectBackups[c->mKey];
              layout->WALL_ELASTICITY = mWallBackups[c->mKey];
            });
          } break;
        }
      });
    }

    virtual void _drawMenuContent() override {
      SliderOption("Bouncy multiplier:", &mMultiplier, 0.0f, 11.0f, [&]() {
        if (mEnabled) _onEnabled();

        this->GetConfig()["Multiplier"].SetFloat(mMultiplier);
        this->SaveConfig();
      });
      ImGui::ShowTooltip("Note: This slider only controls the AMOUNT of bounciness.", ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort));
    }

   public:
    ModBouncyThings() :
        IMod("Bouncy Things", "Lets you change how things bounce off of each other."),
        mMultiplier(this->GetConfig()["Multiplier"].GetFloat()) {}
  };
}  // namespace BAM::Extensions

#endif  // BAM_MODBOUNCYTHINGS_H
