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

#ifndef BAM_MODSIMULATEDTIMEOFDAY_H
#define BAM_MODSIMULATEDTIMEOFDAY_H
#pragma once

#include "pch.h"
#include "IMod.hpp"

namespace BAM::Extensions {
  class ModSimulatedTimeOfDay : public IMod {
    static constexpr inline std::uintptr_t              CONST_ORIGINAL1_ADDR = 0x72E0DC;
    static constexpr inline std::array<std::uint8_t, 5> CONST_ORIGINAL1      = {0xA3, 0xE0, 0x50, 0xA6, 0x00};

    static constexpr inline std::uintptr_t              CONST_ORIGINAL2_ADDR = 0x72E508;
    static constexpr inline std::uintptr_t              CONST_ORIGINAL3_ADDR = 0x72E087;
    static constexpr inline std::array<std::uint8_t, 6> CONST_ORIGINAL23     = {0x89, 0x0D, 0xE0, 0x50, 0xA6, 0x00};

    static constexpr inline std::uintptr_t               CONST_ORIGINAL4_ADDR = 0x72E59C;
    static constexpr inline std::uintptr_t               CONST_ORIGINAL5_ADDR = 0x72E5F9;
    static constexpr inline std::uintptr_t               CONST_ORIGINAL6_ADDR = 0x72E38C;
    static constexpr inline std::array<std::uint8_t, 10> CONST_ORIGINAL456    = {0xC7, 0x05, 0xE0, 0x50, 0xA6, 0x00, 0x00, 0x00, 0x80, 0x3F};

    /// CONFIG

    bool                  mSimulateTimeOfDay;
    OpenCarbon::TimeOfDay mCustomTOD;

   protected:
    virtual void _onEnabled() override {
      GameLoopWalker::ExecuteOnce([&]() {
        MemoryEditor::Get().Make(MemoryEditor::MakeType::NOP, CONST_ORIGINAL1_ADDR, CONST_ORIGINAL1_ADDR + CONST_ORIGINAL1.size());
        MemoryEditor::Get().Make(MemoryEditor::MakeType::NOP, CONST_ORIGINAL2_ADDR, CONST_ORIGINAL2_ADDR + CONST_ORIGINAL23.size());
        MemoryEditor::Get().Make(MemoryEditor::MakeType::NOP, CONST_ORIGINAL3_ADDR, CONST_ORIGINAL3_ADDR + CONST_ORIGINAL23.size());
        MemoryEditor::Get().Make(MemoryEditor::MakeType::NOP, CONST_ORIGINAL4_ADDR, CONST_ORIGINAL4_ADDR + CONST_ORIGINAL456.size());
        MemoryEditor::Get().Make(MemoryEditor::MakeType::NOP, CONST_ORIGINAL5_ADDR, CONST_ORIGINAL5_ADDR + CONST_ORIGINAL456.size());
        MemoryEditor::Get().Make(MemoryEditor::MakeType::NOP, CONST_ORIGINAL6_ADDR, CONST_ORIGINAL6_ADDR + CONST_ORIGINAL456.size());
      });
    }
    virtual void _onDisabled() override {
      GameLoopWalker::ExecuteOnce([&]() {
        MemoryEditor::Get().GetRawMemory(CONST_ORIGINAL1_ADDR).SetValue(CONST_ORIGINAL1);
        MemoryEditor::Get().GetRawMemory(CONST_ORIGINAL2_ADDR).SetValue(CONST_ORIGINAL23);
        MemoryEditor::Get().GetRawMemory(CONST_ORIGINAL3_ADDR).SetValue(CONST_ORIGINAL23);
        MemoryEditor::Get().GetRawMemory(CONST_ORIGINAL4_ADDR).SetValue(CONST_ORIGINAL456);
        MemoryEditor::Get().GetRawMemory(CONST_ORIGINAL5_ADDR).SetValue(CONST_ORIGINAL456);
        MemoryEditor::Get().GetRawMemory(CONST_ORIGINAL6_ADDR).SetValue(CONST_ORIGINAL456);
      });
    }

    virtual void _onGameTick() override {
      if (mSimulateTimeOfDay)
        if (auto* tod = OpenCarbon::TimeOfDay::GetInstance()) mCustomTOD.mCurrentTimeOfDay = tod->mCurrentTimeOfDay;
      OpenCarbon::Variables::FogCurrentBrightness = mCustomTOD.mCurrentTimeOfDay * 15.0f;
    }

    virtual void _drawMenuContent() override {
      CheckboxOption("Simulate time of day:", &mSimulateTimeOfDay, [&]() {
        this->GetConfig()["SimulateTimeOfDay"].SetBool(mSimulateTimeOfDay);
        this->SaveConfig();
      });

      if (SliderOption("Time of day:", &mCustomTOD.mCurrentTimeOfDay, 0.0f, 1.0f, [&]() {
            this->GetConfig()["TimeOfDay"].SetFloat(mCustomTOD.mCurrentTimeOfDay);
            this->SaveConfig();
          }))
        OpenCarbon::TimeOfDay::GetInstance()->mCurrentTimeOfDay = mCustomTOD.mCurrentTimeOfDay;

      if (InputScalarOption("Update rate:", &mCustomTOD.mUpdateRate, [&]() {
            this->GetConfig()["UpdateRate"].SetFloat(mCustomTOD.mUpdateRate);
            this->SaveConfig();
          }))
        OpenCarbon::TimeOfDay::GetInstance()->mUpdateRate = mCustomTOD.mUpdateRate;

      if (InputScalarOption("Update direction:", &mCustomTOD.mUpdateDirection, [&]() {
            this->GetConfig()["UpdateDirection"].SetInt(mCustomTOD.mUpdateDirection);
            this->SaveConfig();
          }))
        OpenCarbon::TimeOfDay::GetInstance()->mUpdateDirection = mCustomTOD.mUpdateDirection;

      if (InputScalarOption("Sun azimuth:", &mCustomTOD.mSunAzimuth, 0.1f, 0.25f, [&]() {
            this->GetConfig()["SunAzimuth"].SetFloat(mCustomTOD.mSunAzimuth);
            this->SaveConfig();
          }))
        OpenCarbon::TimeOfDay::GetInstance()->mSunAzimuth = mCustomTOD.mSunAzimuth;

      if (InputScalarOption("Sun latitude:", &mCustomTOD.mSunLatitude, 0.1f, 0.25f, [&]() {
            this->GetConfig()["SunLatitude"].SetFloat(mCustomTOD.mSunLatitude);
            this->SaveConfig();
          }))
        OpenCarbon::TimeOfDay::GetInstance()->mSunLatitude = mCustomTOD.mSunLatitude;
    }

   public:
    ModSimulatedTimeOfDay() :
        IMod("Custom Time of Day", "Lets you set a custom time of day.", "Simulated Time of Day"),
        mSimulateTimeOfDay(this->GetConfig()["SimulateTimeOfDay"].GetBool()) {
      mCustomTOD.mCurrentTimeOfDay = this->GetConfig()["TimeOfDay"].GetFloat();
      mCustomTOD.mUpdateRate       = this->GetConfig()["UpdateRate"].GetFloat();
      mCustomTOD.mUpdateDirection  = this->GetConfig()["UpdateDirection"].GetInt();
      mCustomTOD.mSunAzimuth       = this->GetConfig()["SunAzimuth"].GetFloat();
      mCustomTOD.mSunLatitude      = this->GetConfig()["SunLatitude"].GetFloat();
    }
  };
}  // namespace BAM::Extensions

#endif  // BAM_MODSIMULATEDTIMEOFDAY_H
