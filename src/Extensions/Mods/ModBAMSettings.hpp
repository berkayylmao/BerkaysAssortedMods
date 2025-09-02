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

#ifndef BAM_MODBAMSETTINGS_H
#define BAM_MODBAMSETTINGS_H
#pragma once

#include "pch.h"
#include "IMod.hpp"

namespace BAM::Extensions {
  class ModBAMSettings : public IMod {
   public:
    bool     mShowToggleButton;
    ImGuiKey mUIHotkey;

   protected:
    virtual void _drawMenuContent() override {
      bool no_hotkey = mUIHotkey == ImGuiKey_None;

      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, no_hotkey);
      CheckboxOption("Show B.A.M. button on top-left", &mShowToggleButton, [&]() {
        this->GetConfig()["ShowToggleButton"].SetBool(mShowToggleButton);
        this->SaveConfig();
      });
      ImGui::PopItemFlag();
      ImGui::ShowTooltip("You have to set a hotkey before you can edit the B.A.M. button.",
                         no_hotkey && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled));

      KeyCatcherOption("Show/Hide B.A.M. Hotkey", &mUIHotkey, [&]() {
        if (mUIHotkey == ImGuiKey_None) {
          mShowToggleButton = true;
          this->GetConfig()["ShowToggleButton"].SetBool(mShowToggleButton);
          this->SaveConfig();
        }

        this->GetConfig()["UIHotkey"].SetInt(static_cast<std::int32_t>(mUIHotkey));
        this->SaveConfig();
      });
    }

   public:
    ModBAMSettings() :
        IMod("Settings", "Customize B.A.M. to your liking.", "BAMSettings", false, false),
        mShowToggleButton(this->GetConfig()["ShowToggleButton"].GetBool()),
        mUIHotkey(static_cast<ImGuiKey>(this->GetConfig()["UIHotkey"].GetInt())) {}
  };
}  // namespace BAM::Extensions

#endif  // BAM_MODBAMSETTINGS_H
