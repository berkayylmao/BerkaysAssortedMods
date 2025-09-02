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

#ifndef BAM_MODCUSTOMTIMEANDDAY_H
#define BAM_MODCUSTOMTIMEANDDAY_H
#pragma once

#include "pch.h"
#include "IMod.hpp"

namespace BAM::Extensions {
  class ModCustomTimeOfDay : public IMod {
    /// VARIABLES

    OpenMW::TimeOfDayEx::Backup                                            mTODBackup;
    std::unordered_map<std::uint32_t, OpenMW::TimeOfDayLightingEx::Backup> mLightingBackups;
    std::unordered_map<std::uint32_t, std::string>                         mNoticeMap;

    /// CONFIG

    bool                                                            mAdvancedMode;
    OpenMW::TimeOfDay                                               mCustomTOD;
    OpenMW::TimeOfDayLightingEx::TimeOfDayLightingData              mSimpleCustomLighting;
    std::vector<OpenMW::TimeOfDayLightingEx::TimeOfDayLightingData> mAdvancedCustomLighting;

    void applyToGame(const OpenMW::TimeOfDayLightingEx::TimeOfDayLightingData& data) {
      GameLoopWalker::ExecuteOnce([&]() { data.ApplyToGame(); });
    }
    void drawLightingEditor(OpenMW::TimeOfDayLightingEx::TimeOfDayLightingData& data) {
      static const auto fn_write_to_config = [=](OpenMW::TimeOfDayLightingEx::TimeOfDayLightingData newData) {
        std::string json_ptr("/Custom Time of Day");
        if (mAdvancedMode && newData.mCollectionKey != OpenMW::TimeOfDayLightingEx::TimeOfDayLightingData::CONST_OVERRIDER_MARKER)
          json_ptr.append(std::vformat("/AdvancedList/{}", std::make_format_args(newData.GetCollectionName())));
        else
          json_ptr.append("/SimpleCustomLighting");
        rapidjson::Pointer ptr(json_ptr);
        rapidjson::SetValueByPointer(Config::Get().GetDocument(), ptr, newData.SerializeToJSON());
        Config::Get().Save();
      };

      rapidjson::Value& config = mAdvancedMode ? this->GetConfig()["AdvancedList"][data.GetCollectionName()] : this->GetConfig()["SimpleCustomLighting"];

      if (SliderOption("Env sky brigtness:", &data.mLayout.EnvSkyBrightness, -20.0f, 20.0f, [&]() {
            config["EnvSkyBrightness"].SetFloat(data.mLayout.EnvSkyBrightness);
            this->SaveConfig();
          })) {
        applyToGame(data);
      };

      if (ColorEditOption("Ambient colour:", data.mLayout.AmbientColour, [&]() {
            OpenMW::Attrib::WriteRGBAToJSON(config["AmbientColour"], data.mLayout.AmbientColour);
            this->SaveConfig();
          })) {
        applyToGame(data);
      };

      if (ColorEditOption("Diffuse colour:", data.mLayout.DiffuseColour, [&]() {
            OpenMW::Attrib::WriteRGBAToJSON(config["DiffuseColour"], data.mLayout.DiffuseColour);
            this->SaveConfig();
          })) {
        applyToGame(data);
      };

      if (ColorEditOption("Fixed function sky colour:", data.mLayout.FixedFunctionSkyColour, [&]() {
            OpenMW::Attrib::WriteRGBAToJSON(config["FixedFunctionSkyColour"], data.mLayout.FixedFunctionSkyColour);
            this->SaveConfig();
          })) {
        applyToGame(data);
      };

      if (ColorEditOption("Fog haze colour:", data.mLayout.FogHazeColour, [&]() {
            OpenMW::Attrib::WriteRGBAToJSON(config["FogHazeColour"], data.mLayout.FogHazeColour);
            this->SaveConfig();
          })) {
        applyToGame(data);
      };

      if (SliderOption("Fog haze colour scale:", &data.mLayout.FogHazeColourScale, -20.0f, 20.0f, [&]() {
            config["FogHazeColourScale"].SetFloat(data.mLayout.FogHazeColourScale);
            this->SaveConfig();
          })) {
        applyToGame(data);
      };

      if (ColorEditOption("Fog sky colour:", data.mLayout.FogSkyColour, [&]() {
            OpenMW::Attrib::WriteRGBAToJSON(config["FogSkyColour"], data.mLayout.FogSkyColour);
            this->SaveConfig();
          })) {
        applyToGame(data);
      };

      if (SliderOption("Fog sky colour scale:", &data.mLayout.FogSkyColourScale, -20.0f, 20.0f, [&]() {
            config["FogSkyColourScale"].SetFloat(data.mLayout.FogSkyColourScale);
            this->SaveConfig();
          })) {
        applyToGame(data);
      };

      if (SliderOption("Fog distance scale:", &data.mLayout.FogDistanceScale, -20.0f, 80.0f, [&]() {
            config["FogDistanceScale"].SetFloat(data.mLayout.FogDistanceScale);
            this->SaveConfig();
          })) {
        applyToGame(data);
      };

      if (SliderOption("Fog in light scatter:", &data.mFogInLightScatter, -20.0f, 80.0f, [&]() {
            config["FogInLightScatter"].SetFloat(data.mFogInLightScatter);
            this->SaveConfig();
          })) {
        applyToGame(data);
      };

      if (SliderOption("Fog sun falloff:", &data.mFogSunFalloff, -20.0f, 80.0f, [&]() {
            config["FogSunFalloff"].SetFloat(data.mFogSunFalloff);
            this->SaveConfig();
          })) {
        applyToGame(data);
      };

      if (ColorEditOption("Specular colour:", data.mLayout.SpecularColour, [&]() {
            OpenMW::Attrib::WriteRGBAToJSON(config["SpecularColour"], data.mLayout.SpecularColour);
            this->SaveConfig();
          })) {
        applyToGame(data);
      };

      if (SliderOption("Car spec scale:", &data.mLayout.CarSpecScale, -20.0f, 20.0f, [&]() {
            config["CarSpecScale"].SetFloat(data.mLayout.CarSpecScale);
            this->SaveConfig();
          })) {
        applyToGame(data);
      };

      auto&       notice = mNoticeMap[data.mCollectionKey];
      const float width  = ImGui::GetContentRegionAvail().x / 2.0f - ImGui::GetStyle().ItemSpacing.x / 2.0f;

      ButtonOption("Copy to clipboard", ImVec2(width, 0.0f), [&]() {
        try {
          rapidjson::StringBuffer                    sb;
          rapidjson::Writer<rapidjson::StringBuffer> w(sb);
          data.SerializeToJSON(w);

          ImGui::SetClipboardText(base64::base64_encode(sb.GetString()).c_str());
          notice = "Copied to clipboard";
        } catch (const std::exception&) {
          notice = "There was an error";
        }
      });
      ImGui::SameLine();

      ButtonOption("Import from clipboard", ImVec2(width, 0.0f), [&]() {
        try {
          auto d = base64::base64_decode(ImGui::GetClipboardText(), true);

          rapidjson::Document json;
          json.Parse(d);

          auto key            = data.mCollectionKey;
          data                = OpenMW::TimeOfDayLightingEx::TimeOfDayLightingData::ReadFromJSON(json);
          data.mCollectionKey = key;
          applyToGame(data);

          fn_write_to_config(data);
          notice = "Imported succesfully";
        } catch (const std::exception&) {
          notice = "There was an error";
        }
      });

      if (mAdvancedMode) {
        ButtonOption("Copy as VltEd ModScript", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f), [&]() {
          try {
            static const char* fmt = R"(# Generated by {0}

game MW C
update_field timeofdaylighting {1} AmbientColour X {2:.3f}
update_field timeofdaylighting {1} AmbientColour Y {3:.3f}
update_field timeofdaylighting {1} AmbientColour Z {4:.3f}
update_field timeofdaylighting {1} AmbientColour W {5:.3f}
update_field timeofdaylighting {1} CarSpecScale {6:.3f}
update_field timeofdaylighting {1} DiffuseColour X {7:.3f}
update_field timeofdaylighting {1} DiffuseColour Y {8:.3f}
update_field timeofdaylighting {1} DiffuseColour Z {9:.3f}
update_field timeofdaylighting {1} DiffuseColour W {10:.3f}
update_field timeofdaylighting {1} EnvSkyBrightness {11:.3f}
update_field timeofdaylighting {1} FixedFunctionSkyColor X {12:.3f}
update_field timeofdaylighting {1} FixedFunctionSkyColor Y {13:.3f}
update_field timeofdaylighting {1} FixedFunctionSkyColor Z {14:.3f}
update_field timeofdaylighting {1} FixedFunctionSkyColor W {15:.3f}
update_field timeofdaylighting {1} FogDistanceScale {16:.3f}
update_field timeofdaylighting {1} FogHazeColour X {17:.3f}
update_field timeofdaylighting {1} FogHazeColour Y {18:.3f}
update_field timeofdaylighting {1} FogHazeColour Z {19:.3f}
update_field timeofdaylighting {1} FogHazeColour W {20:.3f}
update_field timeofdaylighting {1} FogHazeColourScale {21:.3f}
update_field timeofdaylighting {1} FogSkyColourScale {22:.3f}
update_field timeofdaylighting {1} SpecularColour X {23:.3f}
update_field timeofdaylighting {1} SpecularColour Y {24:.3f}
update_field timeofdaylighting {1} SpecularColour Z {25:.3f}
update_field timeofdaylighting {1} SpecularColour W {26:.3f}
update_field timeofdaylighting {1} FogInLightScatter {27:.3f}
update_field timeofdaylighting {1} FogSkyColour X {28:.3f}
update_field timeofdaylighting {1} FogSkyColour Y {29:.3f}
update_field timeofdaylighting {1} FogSkyColour Z {30:.3f}
update_field timeofdaylighting {1} FogSkyColour W {31:.3f}
update_field timeofdaylighting {1} FogSunFalloff {32:.3f}
)";

            const auto        time_str = fmt::format("{0} on {1:%d/%m/%Y %T} (UTC {1:%Oz})", GetModNameWVersionStr(), std::chrono::system_clock::now());
            const std::string clip     = fmt::format(
                fmt::runtime(fmt), time_str, data.GetCollectionName(), data.mLayout.AmbientColour.r, data.mLayout.AmbientColour.g, data.mLayout.AmbientColour.b,
                data.mLayout.AmbientColour.a, data.mLayout.CarSpecScale, data.mLayout.DiffuseColour.r, data.mLayout.DiffuseColour.g,
                data.mLayout.DiffuseColour.b, data.mLayout.DiffuseColour.a, data.mLayout.EnvSkyBrightness, data.mLayout.FixedFunctionSkyColour.r,
                data.mLayout.FixedFunctionSkyColour.g, data.mLayout.FixedFunctionSkyColour.b, data.mLayout.FixedFunctionSkyColour.a,
                data.mLayout.FogDistanceScale, data.mLayout.FogHazeColour.r, data.mLayout.FogHazeColour.g, data.mLayout.FogHazeColour.b,
                data.mLayout.FogHazeColour.a, data.mLayout.FogHazeColourScale, data.mLayout.FogSkyColourScale, data.mLayout.SpecularColour.r,
                data.mLayout.SpecularColour.g, data.mLayout.SpecularColour.b, data.mLayout.SpecularColour.a, data.mFogInLightScatter,
                data.mLayout.FogSkyColour.r, data.mLayout.FogSkyColour.g, data.mLayout.FogSkyColour.b, data.mLayout.FogSkyColour.a, data.mFogSunFalloff);
            ImGui::SetClipboardText(clip.c_str());
            notice = "Copied to clipboard";
          } catch (const std::exception&) {
            notice = "There was an error";
          }
        });

        ButtonOption("Restore", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f), [&]() {
          try {
            const auto& backup = mLightingBackups[data.mCollectionKey];
            backup.Restore();
            data = backup.mData;
            fn_write_to_config(data);

            notice = "Restored default values";
          } catch (const std::exception&) {
            notice = "There was an error";
          }
        });
      }
      if (mNoticeMap.find(data.mCollectionKey) != mNoticeMap.cend()) ImGui::TextUnformatted(notice.c_str());
    }

   protected:
    virtual void _init() override {
      while (!OpenMW::TimeOfDay::GetInstance() || !OpenMW::Attrib::Database::Get()) std::this_thread::sleep_for(100ms);
      OpenMW::TimeOfDayLightingEx::LoadKeys();

      mTODBackup = OpenMW::TimeOfDayEx::Backup(OpenMW::TimeOfDay::GetInstance());

      rapidjson::Document::AllocatorType& allocator = Config::Get().GetDocument().GetAllocator();
      OpenMW::TimeOfDayLightingEx::ForEach([&](OpenMW::Attrib::Collection* c) {
        mLightingBackups[c->mKey] = OpenMW::TimeOfDayLightingEx::Backup(c);

        if (std::find_if(std::cbegin(mAdvancedCustomLighting), std::cend(mAdvancedCustomLighting),
                         [&](const OpenMW::TimeOfDayLightingEx::TimeOfDayLightingData& v) { return v.mCollectionKey == c->mKey; }) ==
            mAdvancedCustomLighting.cend()) {
          auto d = OpenMW::TimeOfDayLightingEx::TimeOfDayLightingData::CreateFromCollection(c);
          mAdvancedCustomLighting.push_back(d);

          rapidjson::Value name(d.GetCollectionName(), allocator);
          rapidjson::Value value(d.SerializeToJSON(), allocator);
          this->GetConfig()["AdvancedList"].AddMember(name, value, allocator);
        }
      });
      this->SaveConfig();

      std::sort(std::begin(mAdvancedCustomLighting), std::end(mAdvancedCustomLighting),
                [](const OpenMW::TimeOfDayLightingEx::TimeOfDayLightingData& l, const OpenMW::TimeOfDayLightingEx::TimeOfDayLightingData& r) {
                  return l.GetCollectionName() < r.GetCollectionName();
                });
    }

    virtual void _onEnabled() override {
      GameLoopWalker::ExecuteOnce([&]() {
        if (auto* tod = OpenMW::TimeOfDay::GetInstance()) {
          tod->mUpdateRate       = mCustomTOD.mUpdateRate;
          tod->mUpdateDirection  = mCustomTOD.mUpdateDirection;
          tod->mCurrentTimeOfDay = mCustomTOD.mCurrentTimeOfDay;
          tod->mSunAzimuth       = mCustomTOD.mSunAzimuth;
          tod->mSunLatitude      = mCustomTOD.mSunLatitude;
        }

        if (mAdvancedMode)
          for (const auto& c : mAdvancedCustomLighting) c.ApplyToGame();
        else
          mSimpleCustomLighting.ApplyToGame();
      });
    }
    virtual void _onDisabled() override {
      GameLoopWalker::ExecuteOnce([&]() {
        mTODBackup.Restore();
        for (const auto& backup : mLightingBackups) backup.second.Restore();
      });
    }

    virtual void _drawMenuContent() override {
      if (SliderOption("Time of day:", &mCustomTOD.mCurrentTimeOfDay, 0.0f, 1.0f, [&]() {
            this->GetConfig()["TimeOfDay"].SetFloat(mCustomTOD.mCurrentTimeOfDay);
            this->SaveConfig();
          }))
        OpenMW::TimeOfDay::GetInstance()->mCurrentTimeOfDay = mCustomTOD.mCurrentTimeOfDay;

      if (InputScalarOption("Update rate:", &mCustomTOD.mUpdateRate, [&]() {
            this->GetConfig()["UpdateRate"].SetFloat(mCustomTOD.mUpdateRate);
            this->SaveConfig();
          }))
        OpenMW::TimeOfDay::GetInstance()->mUpdateRate = mCustomTOD.mUpdateRate;

      if (InputScalarOption("Update direction:", &mCustomTOD.mUpdateDirection, [&]() {
            this->GetConfig()["UpdateDirection"].SetInt(mCustomTOD.mUpdateDirection);
            this->SaveConfig();
          }))
        OpenMW::TimeOfDay::GetInstance()->mUpdateDirection = mCustomTOD.mUpdateDirection;

      if (InputScalarOption("Sun azimuth:", &mCustomTOD.mSunAzimuth, 0.1f, 0.25f, [&]() {
            this->GetConfig()["SunAzimuth"].SetFloat(mCustomTOD.mSunAzimuth);
            this->SaveConfig();
          }))
        OpenMW::TimeOfDay::GetInstance()->mSunAzimuth = mCustomTOD.mSunAzimuth;

      if (InputScalarOption("Sun latitude:", &mCustomTOD.mSunLatitude, 0.1f, 0.25f, [&]() {
            this->GetConfig()["SunLatitude"].SetFloat(mCustomTOD.mSunLatitude);
            this->SaveConfig();
          }))
        OpenMW::TimeOfDay::GetInstance()->mSunLatitude = mCustomTOD.mSunLatitude;

      CheckboxOption("Advanced Mode", &mAdvancedMode, [&]() {
        _onDisabled();
        _onEnabled();

        this->GetConfig()["AdvancedMode"].SetBool(mAdvancedMode);
        this->SaveConfig();
      });

      if (mAdvancedMode) {
        static std::array<char, 100> filter = {0};
        static std::string           filter_str;
        if (InputTextOption("Key filter", filter.data(), filter.size())) filter_str = filter.data();

        for (auto& data : mAdvancedCustomLighting) {
          const auto& name = data.GetCollectionName();
          if (filter[0] != '\0' && name.find(filter_str) == std::string::npos) continue;

          OptionsHeader(name.c_str(), [&]() { drawLightingEditor(data); });
        }
      } else {
        drawLightingEditor(mSimpleCustomLighting);
      }
    }

   public:
    ModCustomTimeOfDay() :
        IMod("Custom Time of Day", "Lets you set a custom time of day."),
        mAdvancedMode(this->GetConfig()["AdvancedMode"].GetBool()),
        mSimpleCustomLighting(OpenMW::TimeOfDayLightingEx::TimeOfDayLightingData::ReadFromJSON(GetConfig()["SimpleCustomLighting"])) {
      // load customtod
      mCustomTOD.mCurrentTimeOfDay = this->GetConfig()["TimeOfDay"].GetFloat();
      mCustomTOD.mUpdateRate       = this->GetConfig()["UpdateRate"].GetFloat();
      mCustomTOD.mUpdateDirection  = this->GetConfig()["UpdateDirection"].GetInt();
      mCustomTOD.mSunAzimuth       = this->GetConfig()["SunAzimuth"].GetFloat();
      mCustomTOD.mSunLatitude      = this->GetConfig()["SunLatitude"].GetFloat();

      // force key in case it was altered
      mSimpleCustomLighting.mCollectionKey = OpenMW::TimeOfDayLightingEx::TimeOfDayLightingData::CONST_OVERRIDER_MARKER;

      for (const auto& o : this->GetConfig()["AdvancedList"].GetObject()) {
        auto instance = OpenMW::TimeOfDayLightingEx::TimeOfDayLightingData::ReadFromJSON(o.value);
        if (instance.mCollectionKey) mAdvancedCustomLighting.push_back(instance);
      }
    }
  };
}  // namespace BAM::Extensions

#endif  // BAM_MODCUSTOMTIMEANDDAY_H
