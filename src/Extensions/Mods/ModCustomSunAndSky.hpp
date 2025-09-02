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

#ifndef BAM_MODCUSTOMSUNANDSKY_H
#define BAM_MODCUSTOMSUNANDSKY_H
#pragma once

#include "pch.h"
#include "IMod.hpp"

namespace BAM::Extensions {
  class ModCustomSunAndSky : public IMod {
    /// VARIABLES

    std::unordered_map<std::uint32_t, OpenPS::SunAndSkyEx::Backup> mBackups;
    std::unordered_map<std::uint32_t, std::string>                 mNoticeMap;

    /// CONFIG

    bool                                            mAdvancedMode;
    OpenPS::SunAndSkyEx::SunAndSkyData              mSimpleCustomSunAndSky;
    std::vector<OpenPS::SunAndSkyEx::SunAndSkyData> mAdvancedCustomSunAndSky;

    void applyToGame(const OpenPS::SunAndSkyEx::SunAndSkyData& data) {
      GameLoopWalker::ExecuteOnce([&]() { data.ApplyToGame(); });
    }
    void drawSunAndSkyEditor(OpenPS::SunAndSkyEx::SunAndSkyData& data) {
      static const auto fn_write_to_config = [=](OpenPS::SunAndSkyEx::SunAndSkyData newData) {
        std::string json_ptr("/Custom Sun and Sky");
        if (mAdvancedMode && newData.mCollectionKey != OpenPS::SunAndSkyEx::SunAndSkyData::CONST_OVERRIDER_MARKER)
          json_ptr.append(std::vformat("/AdvancedList/{}", std::make_format_args(newData.GetCollectionName())));
        else
          json_ptr.append("/SimpleCustomSunAndSky");
        rapidjson::Pointer ptr(json_ptr);
        rapidjson::SetValueByPointer(Config::Get().GetDocument(), ptr, newData.SerializeToJSON());
        Config::Get().Save();
      };

      rapidjson::Value& config = mAdvancedMode ? this->GetConfig()["AdvancedList"][data.GetCollectionName()] : this->GetConfig()["SimpleCustomSunAndSky"];

      CheckboxOption("Overcast:", &data.mOvercast, [&]() {
        applyToGame(data);

        config["Overcast"].SetBool(data.mOvercast);
        this->SaveConfig();
      });
      CheckboxOption("SDOnly:", &data.mSDOnly, [&]() {
        applyToGame(data);

        config["SDOnly"].SetBool(data.mSDOnly);
        this->SaveConfig();
      });
      CheckboxOption("FEOnly:", &data.mFEOnly, [&]() {
        applyToGame(data);

        config["FEOnly"].SetBool(data.mFEOnly);
        this->SaveConfig();
      });

      static std::vector<const char*> sSkyNames = {"None",    "Style 1", "Style 2",  "Style 3",  "Style 4",  "Style 5",  "Style 6",  "Style 7",
                                                   "Style 8", "Style 9", "Style 10", "Style 11", "Style 12", "Style 13", "Style 14", "Style 15"};
      OptionsHeader("Sky related", [&]() {
        ComboOption("Sky dome style:", sSkyNames, &data.mSkyStyle, [&]() { applyToGame(data); });

        if (SliderOption("Sky dome rotation:", &data.mSkyDomeRotation, 0.0f, 360.0f, [&]() {
              config["SkyDomeRotation"].SetFloat(data.mSkyDomeRotation);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Lens flare index:", &data.mLensFlareIndex, [&]() {
              config["LensFlareIndex"].SetInt(data.mLensFlareIndex);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Latitude override:", &data.mLatitudeOverride, [&]() {
              config["LatitudeOverride"].SetFloat(data.mLatitudeOverride);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Longitude override:", &data.mLongitudeOverride, [&]() {
              config["LongitudeOverride"].SetFloat(data.mLongitudeOverride);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Sky latitude override:", &data.mSkyLatitudeOverride, [&]() {
              config["SkyLatitudeOverride"].SetFloat(data.mSkyLatitudeOverride);
              this->SaveConfig();
            }))
          applyToGame(data);
      });
      OptionsHeader("Ambient related", [&]() {
        if (ColorEditOption("Ambient colour:", data.mAmbientColour, [&]() {
              OpenPS::Attrib::WriteRGBAToJSON(config["AmbientColour"], data.mAmbientColour);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (ColorEditOption("Car ambient colour:", data.mCarAmbientColour, [&]() {
              OpenPS::Attrib::WriteRGBAToJSON(config["CarAmbientColour"], data.mCarAmbientColour);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Car ambient saturation:", &data.mCarAmbientSaturation, [&]() {
              config["CarAmbientSaturation"].SetFloat(data.mCarAmbientSaturation);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Bloom intensity:", &data.mBloomIntensity, [&]() {
              config["BloomIntensity"].SetFloat(data.mBloomIntensity);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Bloom threshold:", &data.mBloomThreshold, [&]() {
              config["BloomThreshold"].SetFloat(data.mBloomThreshold);
              this->SaveConfig();
            }))
          applyToGame(data);
      });
      OptionsHeader("Fog related", [&]() {
        if (ColorEditOption("Fog colour:", data.mFogColour, [&]() {
              OpenPS::Attrib::WriteRGBAToJSON(config["FogColour"], data.mFogColour);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Fog start:", &data.mFogStart, [&]() {
              config["FogStart"].SetFloat(data.mFogStart);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Fog end:", &data.mFogEnd, [&]() {
              config["FogEnd"].SetFloat(data.mFogEnd);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Fog exponent:", &data.mFogExponent, [&]() {
              config["FogExponent"].SetFloat(data.mFogExponent);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Fog mix amount:", &data.mFogMixAmount, [&]() {
              config["FogMixAmount"].SetFloat(data.mFogMixAmount);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Fog falloff:", &data.mFogFalloffPs2, [&]() {
              config["FogFalloffPs2"].SetFloat(data.mFogFalloffPs2);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Fog falloff X:", &data.mFogFalloffXPs2, [&]() {
              config["FogFalloffXPs2"].SetFloat(data.mFogFalloffXPs2);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Fog falloff Y:", &data.mFogFalloffYPs2, [&]() {
              config["FogFalloffYPs2"].SetFloat(data.mFogFalloffYPs2);
              this->SaveConfig();
            }))
          applyToGame(data);
      });
      OptionsHeader("Specular related", [&]() {
        if (ColorEditOption("Specular colour:", data.mSpecularColour, [&]() {
              OpenPS::Attrib::WriteRGBAToJSON(config["SpecularColour"], data.mSpecularColour);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Specular size:", &data.mSpecularSize, [&]() {
              config["SpecularSize"].SetFloat(data.mSpecularSize);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Road specular size:", &data.mRoadSpecularSize, [&]() {
              config["RoadSpecularSize"].SetFloat(data.mRoadSpecularSize);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Specular intensity:", &data.mSpecularIntensity, [&]() {
              config["SpecularIntensity"].SetFloat(data.mSpecularIntensity);
              this->SaveConfig();
            }))
          applyToGame(data);
      });
      OptionsHeader("Sun related", [&]() {
        if (ColorEditOption("Car sun colour:", data.mCarSunColour, [&]() {
              OpenPS::Attrib::WriteRGBAToJSON(config["CarSunColour"], data.mCarSunColour);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (ColorEditOption("Sun colour:", data.mSunColour, [&]() {
              OpenPS::Attrib::WriteRGBAToJSON(config["SunColour"], data.mSunColour);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Sun latitude:", &data.mSunLatitude, [&]() {
              config["SunLatitude"].SetFloat(data.mSunLatitude);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Sun longitude:", &data.mSunLongitude, [&]() {
              config["SunLongitude"].SetFloat(data.mSunLongitude);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Sun intensity:", &data.mSunIntensity, [&]() {
              config["SunIntensity"].SetFloat(data.mSunIntensity);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Shadow mix amount:", &data.mShadowMixAmount, [&]() {
              config["ShadowMixAmount"].SetFloat(data.mShadowMixAmount);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Shadow softness:", &data.mShadowSoftness, [&]() {
              config["ShadowSoftness"].SetFloat(data.mShadowSoftness);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("'TreesHector':", &data.mTreesHector, [&]() {
              config["TreesHector"].SetFloat(data.mTreesHector);
              this->SaveConfig();
            }))
          applyToGame(data);

        if (InputScalarOption("Visible longitude:", &data.mVisibleLongitude, [&]() {
              config["VisibleLongitude"].SetFloat(data.mVisibleLongitude);
              this->SaveConfig();
            }))
          applyToGame(data);
      });

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
          if (auto* str = ImGui::GetClipboardText()) {
            auto d = base64::base64_decode(str, true);

            rapidjson::Document json;
            json.Parse(d);

            auto key            = data.mCollectionKey;
            data                = OpenPS::SunAndSkyEx::SunAndSkyData::ReadFromJSON(json);
            data.mCollectionKey = key;
            applyToGame(data);

            fn_write_to_config(data);
            notice = "Imported succesfully";
          } else {
            notice = "There was an error reading from clipboard";
          }
        } catch (const std::exception&) {
          notice = "There was an error";
        }
      });

      if (mAdvancedMode) {
        ButtonOption("Copy as VltEd ModScript", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f), [&]() {
          try {
            static const char* fmt = R"(# Generated by {0}
# You may get "Field 'xyz' does not exist in the node '{1}'." errors, that's ok.
# You may use the 'add_field' command to add the missing fields, should you want to.
# But, if you are editing a *_deg_* node, please note that they get most of their values from their parent.
# For example, "sky_style_01_deg_000" only has the sun angle values and inherits other values from "sky_style_01".
# You may use the 'add_field' command here to force separate values depending on the sun's angle.

game PS
update_field sunandsky {1} SkyDomeRotation {2:.3f}
update_field sunandsky {1} SunLatitude {3:.3f}
update_field sunandsky {1} AmbientColour X {4:.3f}
update_field sunandsky {1} AmbientColour Y {5:.3f}
update_field sunandsky {1} AmbientColour Z {6:.3f}
update_field sunandsky {1} AmbientColour W {7:.3f}
update_field sunandsky {1} BloomIntensity {8:.3f}
update_field sunandsky {1} BloomThreshold {9:.3f}
update_field sunandsky {1} CarAmbientSaturation {10:.3f}
update_field sunandsky {1} FEOnly {11:s}
update_field sunandsky {1} FogColour X {12:.3f}
update_field sunandsky {1} FogColour Y {13:.3f}
update_field sunandsky {1} FogColour Z {14:.3f}
update_field sunandsky {1} FogColour W {15:.3f}
update_field sunandsky {1} FogStart {16:.3f}
update_field sunandsky {1} FogEnd {17:.3f}
update_field sunandsky {1} FogExponent {18:.3f}
update_field sunandsky {1} FogFalloffPs2 {19:.3f}
update_field sunandsky {1} FogFalloffXPs2 {20:.3f}
update_field sunandsky {1} FogFalloffYPs2 {21:.3f}
update_field sunandsky {1} FogMixAmount {22:.3f}
update_field sunandsky {1} LensFlareIndex {23:d}
update_field sunandsky {1} Overcast {24:s}
update_field sunandsky {1} RoadSpecularSize {25:.3f}
update_field sunandsky {1} SDOnly {26:s}
update_field sunandsky {1} ShadowMixAmount {27:.3f}
update_field sunandsky {1} ShadowSoftness {28:.3f}
update_field sunandsky {1} SkyDomeStyle {29:d}
update_field sunandsky {1} SpecularColour X {30:.3f}
update_field sunandsky {1} SpecularColour Y {31:.3f}
update_field sunandsky {1} SpecularColour Z {32:.3f}
update_field sunandsky {1} SpecularColour W {33:.3f}
update_field sunandsky {1} SpecularIntensity {34:.3f}
update_field sunandsky {1} SpecularSize {35:.3f}
update_field sunandsky {1} SunColour X {36:.3f}
update_field sunandsky {1} SunColour Y {37:.3f}
update_field sunandsky {1} SunColour Z {38:.3f}
update_field sunandsky {1} SunColour W {39:.3f}
update_field sunandsky {1} SunIntensity {40:.3f}
update_field sunandsky {1} SunLongitude {41:.3f}
update_field sunandsky {1} TreesHector {42:.3f}
update_field sunandsky {1} VisibleLongitude {43:.3f}
)";

            const auto        time_str = fmt::format("{0} on {1:%d/%m/%Y %T} (UTC {1:%Oz})", GetModNameWVersionStr(), std::chrono::system_clock::now());
            const std::string clip     = fmt::format(
                fmt::runtime(fmt), time_str, data.GetCollectionName(), data.mSkyDomeRotation, data.mSunLatitude, data.mAmbientColour.r, data.mAmbientColour.g,
                data.mAmbientColour.b, data.mAmbientColour.a, data.mBloomIntensity, data.mBloomThreshold, data.mCarAmbientSaturation,
                data.mFEOnly ? "True" : "False", data.mFogColour.r, data.mFogColour.g, data.mFogColour.b, data.mFogColour.a, data.mFogStart, data.mFogEnd,
                data.mFogExponent, data.mFogFalloffPs2, data.mFogFalloffXPs2, data.mFogFalloffYPs2, data.mFogMixAmount, data.mLensFlareIndex,
                data.mOvercast ? "True" : "False", data.mRoadSpecularSize, data.mSDOnly ? "True" : "False", data.mShadowMixAmount, data.mShadowSoftness,
                static_cast<std::uint32_t>(data.mSkyStyle), data.mSpecularColour.r, data.mSpecularColour.g, data.mSpecularColour.b, data.mSpecularColour.a,
                data.mSpecularIntensity, data.mSpecularSize, data.mSunColour.r, data.mSunColour.g, data.mSunColour.b, data.mSunColour.a, data.mSunIntensity,
                data.mSunLongitude, data.mTreesHector, data.mVisibleLongitude);
            ImGui::SetClipboardText(clip.c_str());
            notice = "Copied to clipboard";
          } catch (const std::exception&) {
            notice = "There was an error";
          }
        });

        ButtonOption("Restore", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f), [&]() {
          try {
            const auto& backup = mBackups[data.mCollectionKey];
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
      while (!OpenPS::Attrib::Database::Get()) std::this_thread::sleep_for(100ms);
      OpenPS::SunAndSkyEx::LoadKeys();

      rapidjson::Document::AllocatorType& allocator = Config::Get().GetDocument().GetAllocator();
      OpenPS::SunAndSkyEx::ForEach([&](OpenPS::Attrib::Collection* c) {
        mBackups[c->mKey] = OpenPS::SunAndSkyEx::Backup(c);

        if (std::find_if(std::cbegin(mAdvancedCustomSunAndSky), std::cend(mAdvancedCustomSunAndSky),
                         [&](const OpenPS::SunAndSkyEx::SunAndSkyData& v) { return v.mCollectionKey == c->mKey; }) == mAdvancedCustomSunAndSky.cend()) {
          auto d = OpenPS::SunAndSkyEx::SunAndSkyData::CreateFromCollection(c);
          mAdvancedCustomSunAndSky.push_back(d);

          rapidjson::Value name(d.GetCollectionName(), allocator);
          rapidjson::Value value(d.SerializeToJSON(), allocator);
          this->GetConfig()["AdvancedList"].AddMember(name, value, allocator);
        }
      });
      this->SaveConfig();

      std::sort(std::begin(mAdvancedCustomSunAndSky), std::end(mAdvancedCustomSunAndSky),
                [](const OpenPS::SunAndSkyEx::SunAndSkyData& l, const OpenPS::SunAndSkyEx::SunAndSkyData& r) {
                  return l.GetCollectionName() < r.GetCollectionName();
                });
    }

    virtual void _onEnabled() override {
      GameLoopWalker::ExecuteOnce([&]() {
        if (mAdvancedMode)
          for (const auto& c : mAdvancedCustomSunAndSky) c.ApplyToGame();
        else
          mSimpleCustomSunAndSky.ApplyToGame();
      });
    }
    virtual void _onDisabled() override {
      GameLoopWalker::ExecuteOnce([&]() {
        for (const auto& backup : mBackups) backup.second.Restore();
      });
    }

    virtual void _drawMenuContent() override {
      if (ImGui::Checkbox("Advanced Mode", &mAdvancedMode) && mEnabled) {
        _onDisabled();
        _onEnabled();

        this->GetConfig()["AdvancedMode"].SetBool(mAdvancedMode);
        this->SaveConfig();
      }

      if (mAdvancedMode) {
        if (OpenPS::Variables::TheGameFlowManager != NFSPluginSDK::GameFlowState::InFrontEnd)
          ImGui::Text("Active key: %s", OpenPS::SunAndSkyEx::sKeysMap[OpenPS::SunAndSky::Get()->mCurrentKey].c_str());

        static std::array<char, 100> filter = {0};
        static std::string           filter_str;
        if (InputTextOption("Key filter", filter.data(), filter.size())) filter_str = filter.data();

        for (auto& data : mAdvancedCustomSunAndSky) {
          const auto& name = data.GetCollectionName();
          if (filter[0] != '\0' && name.find(filter_str) == std::string::npos) continue;

          OptionsHeader(name.c_str(), [&]() { drawSunAndSkyEditor(data); });
        }
      } else {
        drawSunAndSkyEditor(mSimpleCustomSunAndSky);
      }
    }

   public:
    ModCustomSunAndSky() :
        IMod("Custom Time of Day", "Lets you set a custom time of day.", "Custom Sun and Sky"),
        mAdvancedMode(this->GetConfig()["AdvancedMode"].GetBool()),
        mSimpleCustomSunAndSky(OpenPS::SunAndSkyEx::SunAndSkyData::ReadFromJSON(GetConfig()["SimpleCustomSunAndSky"])) {
      // force key in case it was altered
      mSimpleCustomSunAndSky.mCollectionKey = OpenPS::SunAndSkyEx::SunAndSkyData::CONST_OVERRIDER_MARKER;

      for (const auto& o : this->GetConfig()["AdvancedList"].GetObject()) {
        auto instance = OpenPS::SunAndSkyEx::SunAndSkyData::ReadFromJSON(o.value);
        if (instance.mCollectionKey) mAdvancedCustomSunAndSky.push_back(instance);
      }
    }
  };
}  // namespace BAM::Extensions

#endif  // BAM_MODCUSTOMSUNANDSKY_H
