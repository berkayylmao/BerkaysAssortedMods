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

#ifndef BAM_MODCUSTOMCARSCALE_H
#define BAM_MODCUSTOMCARSCALE_H
#pragma once

#include "pch.h"
#include "IMod.hpp"

namespace BAM::Extensions {
  class ModCustomCarScale : public IMod {
    static inline const std::array<const char*, 4> CONST_CONFIGARRAY  = {"X", "Y", "Z", "W"};
    static inline const std::array<const char*, 4> CONST_DISPLAYARRAY = {"X: ", "Y: ", "Z: ", "W: "};
    static constexpr float CONST_DEFAULTMATRIX[4][4] = {{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}};

    float mCustomMatrix[4][4];
    float (*mGameMatrix)[4][4];

   protected:
    virtual void _init() override {
      while (!mGameMatrix || (*mGameMatrix)[0][0] == 0.0f) std::this_thread::sleep_for(100ms);
    }

    virtual void _onEnabled() override {
      GameLoopWalker::ExecuteOnce([&]() { std::memcpy(mGameMatrix, mCustomMatrix, sizeof(float) * 4 * 4); });
    }
    virtual void _onDisabled() override {
      GameLoopWalker::ExecuteOnce([&]() { std::memcpy(mGameMatrix, CONST_DEFAULTMATRIX, sizeof(float) * 4 * 4); });
    }
    virtual void _onGameTick() override {
      if (mEnabled) std::memcpy(mGameMatrix, mCustomMatrix, sizeof(float) * 4 * 4);
    }

    virtual void _drawMenuContent() override {
      const float offset_x = ImGui::GetCursorPosX() + (ImGui::CalcTextSize(CONST_DISPLAYARRAY[0]).x * 2.0f);

      for (std::size_t i = 0; i < 4; i++) {
        ImGui::TextUnformatted(CONST_DISPLAYARRAY[i]);
        ImGui::SameLine();
        ImGui::SetCursorPosX(offset_x);

        const float width = ImGui::GetContentRegionAvail().x / 4.0f - ImGui::GetStyle().ItemSpacing.x;

        ImGui::PushID(i);
        for (std::size_t j = 0; j < 4; j++) {
          ImGui::SameLine();
          ImGui::SetCursorPosX(offset_x + (j * width) + (j * ImGui::GetStyle().FramePadding.x));

          ImGui::PushID(j);
          ImGui::SetNextItemWidth(width);
          ImGui::SliderFloat("##CustomMatrixSlider", &mCustomMatrix[i][j], -2.0f, 2.0f, "%.2f");
          if (ImGui::IsItemDeactivatedAfterEdit()) {
            this->GetConfig()["CustomMatrix"][CONST_CONFIGARRAY[i]].GetArray()[j].SetFloat(mCustomMatrix[i][j]);
            this->SaveConfig();
          }
          ImGui::PopID();
        }
        ImGui::PopID();
      }

      ButtonOption("Restore", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f), [&]() {
        GameLoopWalker::ExecuteOnce([&]() {
          std::memcpy(mCustomMatrix, CONST_DEFAULTMATRIX, sizeof(float) * 4 * 4);
          std::memcpy(mGameMatrix, CONST_DEFAULTMATRIX, sizeof(float) * 4 * 4);

          for (std::size_t i = 0; i < 4; i++)
            for (std::size_t j = 0; j < 4; j++) this->GetConfig()["CustomMatrix"][CONST_CONFIGARRAY[i]].GetArray()[j].SetFloat(mCustomMatrix[i][j]);
          this->SaveConfig();
        });
      });
    }

   public:
    ModCustomCarScale() : IMod("Custom Car Scale", "Lets you set a custom car scale."), mCustomMatrix(), mGameMatrix(nullptr) {
      switch (Variables::sCurGameInfo.mGame) {
        case NFSPluginSDK::Game::MW05: {
          mGameMatrix = reinterpret_cast<float(*)[4][4]>(static_cast<float*>(OpenMW::Variables::CarScaleMatrix.GetField()));
        } break;
        case NFSPluginSDK::Game::Carbon: {
          mGameMatrix = reinterpret_cast<float(*)[4][4]>(static_cast<float*>(OpenCarbon::Variables::CarScaleMatrix.GetField()));
        } break;
        case NFSPluginSDK::Game::ProStreet: {
          mGameMatrix = reinterpret_cast<float(*)[4][4]>(static_cast<float*>(OpenPS::Variables::CarScaleMatrix.GetField()));
        } break;
      }

      if (!this->GetConfig().HasMember("CustomMatrix")) {
        rapidjson::Document::AllocatorType& allocator = Config::Get().GetDocument().GetAllocator();

        rapidjson::Value name("CustomMatrix", allocator);
        rapidjson::Value val(rapidjson::Type::kObjectType);
        for (std::size_t i = 0; i < 4; i++) {
          rapidjson::Value sub(rapidjson::Type::kArrayType);
          for (std::size_t j = 0; j < 4; j++) sub.PushBack(CONST_DEFAULTMATRIX[i][j], allocator);

          rapidjson::Value subname(CONST_CONFIGARRAY[i], allocator);
          val.AddMember(subname, sub, allocator);
        }
        this->GetConfig().AddMember(name, val, allocator);
        this->SaveConfig();
      }

      const auto& cst_matrix = this->GetConfig()["CustomMatrix"];
      for (std::size_t i = 0; i < 4; i++)
        for (std::size_t j = 0; j < 4; j++) mCustomMatrix[i][j] = cst_matrix[CONST_CONFIGARRAY[i]].GetArray()[j].GetFloat();
    }
  };
}  // namespace BAM::Extensions

#endif  // BAM_MODCUSTOMCARSCALE_H
