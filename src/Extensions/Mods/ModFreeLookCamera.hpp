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

#ifndef BAM_MODFREELOOKCAMERA_H
#define BAM_MODFREELOOKCAMERA_H
#pragma once

#include "pch.h"
#include "IMod.hpp"
#include "Extensions/Mods/ModFreeLookCamera.Texture.hpp"

namespace BAM::Extensions {
  namespace FreeLookCamera {
    enum class CameraDevice : std::uint32_t { Gamepad, Mouse, Keyboard };

    inline constinit D3DXMATRIX*(__cdecl* sOrigCreateLookAtMarix)(NFSPluginSDK::Matrix4*, NFSPluginSDK::Vector3&, NFSPluginSDK::Vector3&,
                                                                  NFSPluginSDK::Vector3&) = nullptr;

    inline constinit CameraDevice   sCameraDevice     = CameraDevice::Gamepad;
    inline constinit float          sCameraResetDelay = 3.0f;
    inline constinit ImVec2         sRads             = ImVec2();
    inline constinit float          sRotateSpeed      = 8.0f;
    inline constinit float          sTilts            = 0.0f;
    inline XInputWrapper::StickInfo sRightStick;

    inline void XInputCallback(const XInputWrapper::ControllerInfo& info) {
      if (sCameraDevice != CameraDevice::Gamepad) return;

      sRightStick = info.mRightStick;
      if (info.mKeyStrokes.RTHUMB_PRESS == XInputWrapper::KeyStatus::Up && sIsCameraValid) sFreeCameraEnabled = !sFreeCameraEnabled;
    }

    inline D3DXMATRIX* __cdecl hkCreateLookAtMatrix(NFSPluginSDK::Matrix4* out, NFSPluginSDK::Vector3& eye, NFSPluginSDK::Vector3& at,
                                                    NFSPluginSDK::Vector3& up) {
      switch (Variables::sCurGameInfo.mGame) {
        case NFSPluginSDK::Game::MW05: {
          auto cam                       = static_cast<std::uint32_t>(OpenMW::Variables::GetCurrentCamera());
          FreeLookCamera::sIsCameraValid = NFSPluginSDK::IsGameplayRunning() && cam >= 2 && cam <= 6;
        } break;
        case NFSPluginSDK::Game::Carbon: {
          auto cam                       = static_cast<std::uint8_t>(OpenCarbon::Variables::GetCurrentCamera());
          FreeLookCamera::sIsCameraValid = NFSPluginSDK::IsGameplayRunning() && cam >= 2 && cam <= 6;
        } break;
        case NFSPluginSDK::Game::ProStreet: {
          auto cam                       = static_cast<std::uint8_t>(OpenPS::Variables::GetCurrentCamera());
          FreeLookCamera::sIsCameraValid = NFSPluginSDK::IsGameplayRunning() && cam >= 2 && cam <= 4;
        } break;
        case NFSPluginSDK::Game::World: {
          FreeLookCamera::sIsCameraValid = true;
        } break;
      }

      if (!sFreeCameraEnabled || !sIsCameraValid || !WndProcWalker::IsWindowActive()) {
        sRads  = ImVec2();
        sTilts = 0.0f;
      } else {
        const auto& io = ImGui::GetIO();
        const float dt = sRotateSpeed * io.DeltaTime;

        switch (sCameraDevice) {
          case CameraDevice::Gamepad: {
            if (sRightStick.x != 0.0f || sRightStick.y != 0.0f) {
              float to = std::atan2(-sRightStick.x, sRightStick.y);
              if (std::abs(to) > (3.14f - (3.14f / 8.0f)) && std::abs(to) < (3.14f + (3.14f / 8.0f))) sRads.x = to = 3.14f;
              if (sRads.x == 3.14f && to < 0.0f) sRads.x = -3.14f;

              sRads.x = std::lerp(sRads.x, to, dt);
            } else {
              if (sRads.x == 3.14f)
                sRads.x = 0.0f;
              else
                sRads.x = std::lerp(sRads.x, 0.0f, dt);
            }

            eye.z = std::max(eye.z, at.z) + (sRightStick.y < 0.0f ? (-sRightStick.y * up.z) : 0.0f);
          } break;
          case CameraDevice::Mouse: {
            static constinit float sInactivityTimer = 0.0f;
            static constinit bool  sRadsReset       = false;

            auto mouse = (io.MouseDelta * 8.0f) / io.DisplaySize;
            if (!io.WantCaptureMouse) {
              sRads.x -= mouse.x;
              sRads.y = std::clamp(sRads.y + mouse.y, -3.14f / 2.0f, 3.14f);
            }

            if ((mouse.x == 0.0f && mouse.y == 0.0f) || io.WantCaptureMouse) {
              sInactivityTimer += io.DeltaTime;
            } else {
              sInactivityTimer = 0.0f;
              sRadsReset       = false;
            }

            if (sInactivityTimer > sCameraResetDelay) {
              if (!sRadsReset) {
                sRads.x = std::fmod(sRads.x, 3.14f * 2.0f);
                if (sRads.x > 3.14f)
                  sRads.x -= 3.14f * 2.0f;
                else if (sRads.x < -3.14f)
                  sRads.x += 3.14f * 2.0f;
                sRadsReset = true;
              }

              sRads.x = std::lerp(sRads.x, 0.0f, dt);
              sRads.y = std::lerp(sRads.y, 0.0f, dt);
            }

            eye.z += sRads.y;
          } break;
          case CameraDevice::Keyboard: {
            if (!io.WantCaptureKeyboard || io.WantTextInput) {
              bool q = ImGui::IsKeyDown(ImGuiKey_Q);
              bool e = ImGui::IsKeyDown(ImGuiKey_E);
              if (q && e)
                sRads.x = 3.14f;
              else if (q)
                sRads.x = 3.14f / 2.0f;
              else if (e)
                sRads.x = -3.14f / 2.0f;
              else
                sRads.x = 0.0f;
            }
          } break;
        }

        const auto diff = eye - at;
        const auto sin  = std::sin(sRads.x);
        const auto cos  = std::cos(sRads.x);

        eye.x = cos * diff.x - sin * diff.y + at.x;
        eye.y = sin * diff.x + cos * diff.y + at.y;

        sTilts = sRads.x != 0.0f ? -2578.0f : 0.0f;
      }

      return sOrigCreateLookAtMarix(out, eye, at, up);
    }
  }  // namespace FreeLookCamera

  class ModFreeLookCamera : public IMod {
    std::uintptr_t mFrom;
    std::uintptr_t mTilts;
    std::uintptr_t mTiltsAddrBackup;

    bool               mIconEnabled;
    LPDIRECT3DTEXTURE9 mIconTexture;

   protected:
    virtual void _onEnabled() override {
      FreeLookCamera::sRads = ImVec2();
      GameLoopWalker::ExecuteOnce([&]() {
        // Disable tilts
        if (mTilts) MemoryEditor::Get().GetRawMemory(mTilts).SetValue(&FreeLookCamera::sTilts);
        // Hook camera
        MemoryEditor::Get().Make(MemoryEditor::MakeType::Call, mFrom, reinterpret_cast<std::uintptr_t>(&FreeLookCamera::hkCreateLookAtMatrix));
      });
    }
    virtual void _onDisabled() override {
      GameLoopWalker::ExecuteOnce([&]() {
        // Restore tilts
        if (mTilts) MemoryEditor::Get().GetRawMemory(mTilts).SetValue(mTiltsAddrBackup);
        // Restore camera
        MemoryEditor::Get().Make(MemoryEditor::MakeType::Call, mFrom, reinterpret_cast<std::uintptr_t>(FreeLookCamera::sOrigCreateLookAtMarix));
      });
    }

    virtual void _drawMenuContent() override {
      static const std::vector<const char*> sDevices = {"Gamepad", "Mouse", "Keyboard"};

      ComboOption("Camera device:", sDevices, &FreeLookCamera::sCameraDevice, [&]() {
        FreeLookCamera::sFreeCameraEnabled = true;

        this->GetConfig()["CameraDevice"].SetUint(static_cast<std::uint32_t>(FreeLookCamera::sCameraDevice));
        this->SaveConfig();
      });
      ImGui::ShowTooltip(std::vformat(R"(The device you want to use to look around with.
- Gamepad: Forza Horizon 5 style. Look around with the right stick. Free look camera can be toggled on/off with R3 (Right stick press).
- Mouse: GTA:SA style. Look around with the mouse and the camera will reset after {:.2f} seconds of inactivity.
- Keyboard: GTA:SA style. Q to look left, E to look right, Q+E to look back.)",
                                      std::make_format_args(FreeLookCamera::sCameraResetDelay))
                             .c_str(),
                         ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort));
      if (FreeLookCamera::sCameraDevice == FreeLookCamera::CameraDevice::Mouse) {
        SliderOption("Camera reset delay:", &FreeLookCamera::sCameraResetDelay, 1.0f, 10.0f, [&]() {
          this->GetConfig()["CameraResetDelay"].SetFloat(FreeLookCamera::sCameraResetDelay);
          this->SaveConfig();
        });
      }
      if (FreeLookCamera::sCameraDevice == FreeLookCamera::CameraDevice::Gamepad) {
        SliderOption("Camera rotate speed:", &FreeLookCamera::sRotateSpeed, 0.0f, 32.0f, [&]() {
          this->GetConfig()["RotateSpeed"].SetFloat(FreeLookCamera::sRotateSpeed);
          this->SaveConfig();
        });
        CheckboxOption("Show camera toggle status", &mIconEnabled, [&]() {
          this->GetConfig()["DrawCameraToggle"].SetBool(mIconEnabled);
          this->SaveConfig();
        });
        ImGui::ShowTooltip("The toggle icon is shown on the bottom-right of the screen", ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort));
      }
    }
    virtual void _drawOverlay(ImGuiViewport* viewport) override {
      if (!mIconEnabled || !NFSPluginSDK::IsGameplayRunning() || !FreeLookCamera::sIsCameraValid) return;
      if (!mIconTexture)
        D3DXCreateTextureFromFileInMemory(DirectXWalker::GetDevice(), FreeLookCamera::Icon::CONST_DATA, FreeLookCamera::Icon::CONST_SIZE, &this->mIconTexture);

      const ImVec2 icon_size(ImGui::GetFrameHeight() / 1.5f, ImGui::GetFrameHeight() / 1.5f);
      const ImVec2 icon_pos(viewport->Pos + viewport->Size - ImVec2(icon_size.x * 1.25f, icon_size.y));
      const ImVec2 fade_pos(icon_pos - icon_size);

      auto* draw_list = ImGui::GetForegroundDrawList(viewport);
      draw_list->AddRectFilledMultiColor(fade_pos, viewport->Pos + viewport->Size, IM_COL32_BLACK_TRANS, IM_COL32_BLACK_TRANS, IM_COL32_BLACK,
                                         IM_COL32_BLACK_TRANS);
      draw_list->AddImage(mIconTexture, icon_pos, icon_pos + icon_size, ImVec2(), ImVec2(1.0f, 1.0f),
                          FreeLookCamera::sFreeCameraEnabled ? IM_COL32_WHITE : IM_COL32(190, 190, 190, 190));
    }
    virtual void _d3dDeviceReset(LPDIRECT3DDEVICE9 pDevice) override {
      if (mIconTexture) {
        mIconTexture->Release();
        mIconTexture = nullptr;
      }

      D3DXCreateTextureFromFileInMemory(pDevice, FreeLookCamera::Icon::CONST_DATA, FreeLookCamera::Icon::CONST_SIZE, &this->mIconTexture);
    }

   public:
    ModFreeLookCamera() :
        IMod("Free Look Camera", "Lets you look around in-game."), mFrom(0), mTilts(0), mTiltsAddrBackup(0), mIconEnabled(true), mIconTexture(nullptr) {
      std::uintptr_t orig_fn = 0;
      switch (Variables::sCurGameInfo.mGame) {
        case NFSPluginSDK::Game::MW05: {
          mFrom   = 0x47DCBC;
          mTilts  = 0x47D506;
          orig_fn = 0x6CF0A0;
        } break;
        case NFSPluginSDK::Game::Carbon: {
          mFrom   = 0x492E5B;
          mTilts  = 0x492353;
          orig_fn = 0x71B430;
        } break;
        case NFSPluginSDK::Game::ProStreet: {
          mFrom   = 0x650A9F;
          orig_fn = 0x6FF060;
        } break;
        case NFSPluginSDK::Game::World: {
          mFrom   = injector::mem_ptr(injector::aslr_ptr(0x7DAE17).get()).as_int();
          orig_fn = injector::mem_ptr(injector::aslr_ptr(0x635D50).get()).as_int();
        } break;
      }
      FreeLookCamera::sOrigCreateLookAtMarix =
          reinterpret_cast<D3DXMATRIX*(__cdecl*)(NFSPluginSDK::Matrix4*, NFSPluginSDK::Vector3&, NFSPluginSDK::Vector3&, NFSPluginSDK::Vector3&)>(orig_fn);
      FreeLookCamera::sRotateSpeed      = this->GetConfig()["RotateSpeed"].GetFloat();
      FreeLookCamera::sCameraDevice     = static_cast<FreeLookCamera::CameraDevice>(this->GetConfig()["CameraDevice"].GetUint());
      FreeLookCamera::sCameraResetDelay = this->GetConfig()["CameraResetDelay"].GetFloat();
      mIconEnabled                      = this->GetConfig()["DrawCameraToggle"].GetBool();
      if (mTilts) mTiltsAddrBackup = MemoryEditor::Get().GetRawMemory(mTilts).GetValue<std::uintptr_t>();

      XInputWrapper::Get().AddCallback(FreeLookCamera::XInputCallback);
    }
  };
}  // namespace BAM::Extensions

#endif  // BAM_MODFREELOOKCAMERA_H
