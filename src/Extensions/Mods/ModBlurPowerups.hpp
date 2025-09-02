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

#ifndef BAM_BLURPOWERUPS_H
#define BAM_BLURPOWERUPS_H
#pragma once

#include "pch.h"
#include "IMod.hpp"
#include "Extensions/Mods/ModBlurPowerups.BargePowerUp.hpp"
#include "Extensions/Mods/ModBlurPowerups.BoltPowerUp.hpp"
#include "Extensions/Mods/ModBlurPowerups.BoostPowerUp.hpp"
#include "Extensions/Mods/ModBlurPowerups.Textures.hpp"

namespace BAM::Extensions {
  class ModBlurPowerups : public IMod {
    enum class EventType { Activate, ActivateBackwards, Switch };

    /// CONSTANTS

    ImVec2 mDefaultPowerupIconSize;
    ImVec2 mPowerupIconSize;

    /// VARIABLES

    details::ModBlurPowerups::PowerUpType  mActivePowerUp;
    details::ModBlurPowerups::BargePowerUp mBargePowerUp;
    details::ModBlurPowerups::BoltPowerUp  mBoltPowerUp;
    details::ModBlurPowerups::BoostPowerUp mBoostPowerUp;

    /// CONFIG

    bool     mPlaySoundFX;
    float    mSoundFXVolume;
    float    mIconSizeMult;
    ImGuiKey mActivateKey;
    ImGuiKey mActivateBackwardsKey;
    bool     mActivateBackwardsIsModifier;
    ImGuiKey mSwitchKey;

    details::ModBlurPowerups::IPowerUp& getActivePowerUp() {
      switch (mActivePowerUp) {
        case details::ModBlurPowerups::PowerUpType::Bolt:
          return mBoltPowerUp;
        case details::ModBlurPowerups::PowerUpType::Boost:
          return mBoostPowerUp;
        case details::ModBlurPowerups::PowerUpType::Barge:
          return mBargePowerUp;
      }

      return mBoltPowerUp;
    }
    void handleEvent(EventType eventType) {
      auto& powerup = getActivePowerUp();

      switch (eventType) {
        case EventType::Activate: {
          powerup.OnActivate();
        } break;
        case EventType::ActivateBackwards: {
          powerup.OnActivateBackwards();
        } break;
        case EventType::Switch: {
          switch (mActivePowerUp) {
            case details::ModBlurPowerups::PowerUpType::Bolt:
              mActivePowerUp = details::ModBlurPowerups::PowerUpType::Boost;
              break;
            case details::ModBlurPowerups::PowerUpType::Boost:
              mActivePowerUp = details::ModBlurPowerups::PowerUpType::Barge;
              break;
            case details::ModBlurPowerups::PowerUpType::Barge:
              mActivePowerUp = details::ModBlurPowerups::PowerUpType::Bolt;
              break;
          }
          if (mPlaySoundFX) FMODWrapper::Get().PlaySoundFX(FMODWrapper::SoundFX::BlurPowerUpSwitch);
        } break;
      }
    }
    void onXInputUpdate(const XInputWrapper::ControllerInfo& info) {
      if (!mEnabled) return;

      static const auto fn_can_trigger = [&](ImGuiKey key) {
        switch (key) {
          case ImGuiKey_GamepadL2:
            return info.mLeftTrigger > 0.8f;
          case ImGuiKey_GamepadR2:
            return info.mRightTrigger > 0.8f;
          case ImGuiKey_GamepadLStickUp:
            return info.mLeftStick.y > 0.9f;
          case ImGuiKey_GamepadLStickDown:
            return info.mLeftStick.y < -0.9f;
          case ImGuiKey_GamepadLStickLeft:
            return info.mLeftStick.x < -0.9f;
          case ImGuiKey_GamepadLStickRight:
            return info.mLeftStick.x > 0.9f;
          case ImGuiKey_GamepadRStickUp:
            return info.mRightStick.y > 0.9f;
          case ImGuiKey_GamepadRStickDown:
            return info.mRightStick.y < -0.9f;
          case ImGuiKey_GamepadRStickLeft:
            return info.mRightStick.x < -0.9f;
          case ImGuiKey_GamepadRStickRight:
            return info.mRightStick.x > 0.9f;
          case ImGuiKey_GamepadStart:
          case ImGuiKey_GamepadBack:
          case ImGuiKey_GamepadFaceUp:
          case ImGuiKey_GamepadFaceDown:
          case ImGuiKey_GamepadFaceLeft:
          case ImGuiKey_GamepadFaceRight:
          case ImGuiKey_GamepadDpadUp:
          case ImGuiKey_GamepadDpadDown:
          case ImGuiKey_GamepadDpadLeft:
          case ImGuiKey_GamepadDpadRight:
          case ImGuiKey_GamepadL1:
          case ImGuiKey_GamepadR1:
          case ImGuiKey_GamepadL3:
          case ImGuiKey_GamepadR3:
            return info.mKeyStrokes.Find(ImGui::KeyEx::GetVKCode(key)) == XInputWrapper::KeyStatus::Down;
        }

        return false;
      };

      const bool has_activate           = fn_can_trigger(mActivateKey);
      const bool has_activate_backwards = fn_can_trigger(mActivateBackwardsKey);
      const bool has_switch             = fn_can_trigger(mSwitchKey);

      if (has_activate) {
        if (mActivateBackwardsIsModifier && has_activate_backwards)
          handleEvent(EventType::ActivateBackwards);
        else
          handleEvent(EventType::Activate);
      } else if (!mActivateBackwardsIsModifier && has_activate_backwards)
        handleEvent(EventType::ActivateBackwards);
      else if (has_switch)
        handleEvent(EventType::Switch);
    }

    void createTextures(LPDIRECT3DDEVICE9 pDevice) {
      mBoltPowerUp.CreateTexture(pDevice);
      mBoostPowerUp.CreateTexture(pDevice);
      mBargePowerUp.CreateTexture(pDevice);
    }
    void drawPowerUp(const ImGuiViewport* viewport, details::ModBlurPowerups::IPowerUp& powerup, float posCursorX) {
      if (!powerup.mIconTexture) return;

      const bool   is_selected          = mActivePowerUp == powerup.mType;
      const float  selected_y_shift     = (ImGui::GetFrameHeight() / 2.0f) * mIconSizeMult;
      const float  remaining_locked_pct = powerup.GetRemainingLockedPercentage();
      const ImVec2 cursor_pos = ImVec2(posCursorX, viewport->Size.y - ImGui::GetFontSize() - mPowerupIconSize.y - (is_selected ? selected_y_shift : 0.0f));

      if (powerup.mActivationLocked)
        ImGui::SetCursorPos(cursor_pos + ImVec2(0.0f, mPowerupIconSize.y * remaining_locked_pct));
      else
        ImGui::SetCursorPos(cursor_pos);
      ImGui::Image(powerup.mIconTexture, mPowerupIconSize - ImVec2(0.0f, mPowerupIconSize.y * remaining_locked_pct), ImVec2(0.0f, remaining_locked_pct),
                   ImVec2(1.0f, 1.0f), is_selected ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : ImVec4(0.5f, 0.5f, 0.5f, 0.75f));
      // bg
      ImGui::SetCursorPos(cursor_pos);
      ImGui::Image(powerup.mIconTexture, mPowerupIconSize, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.33f));
    }

   protected:
    virtual void _init() override {
      mDefaultPowerupIconSize = ImVec2(510.0f / ImGui::GetFrameHeight(), 384.0f / ImGui::GetFrameHeight()) * 2.0f;
      mPowerupIconSize        = mDefaultPowerupIconSize * mIconSizeMult;

      FMODWrapper::Get().GetMasterChannelGroup()->setVolume(mSoundFXVolume);
      XInputWrapper::Get().AddCallback([&](const XInputWrapper::ControllerInfo& info) { this->onXInputUpdate(info); });
    }

    virtual void _onEnabled() override { createTextures(DirectXWalker::GetDevice()); }
    virtual void _onDisabled() override {
      mBoltPowerUp.mActivationLocked  = false;
      mBoostPowerUp.mActivationLocked = false;
      mBargePowerUp.mActivationLocked = false;
    }
    virtual void _onGameTick() override {
      mBoltPowerUp.OnTick();
      mBoostPowerUp.OnTick();
      mBargePowerUp.OnTick();

      const bool has_activate = !ImGui::IsGamepadKey(mActivateKey) && ImGui::IsKeyPressed(mActivateKey, false);
      const bool has_activate_backwards =
          !ImGui::IsGamepadKey(mActivateBackwardsKey) &&
          (mActivateBackwardsIsModifier ? ImGui::IsKeyDown(mActivateBackwardsKey) : ImGui::IsKeyPressed(mActivateBackwardsKey, false));
      const bool has_switch = !ImGui::IsGamepadKey(mSwitchKey) && ImGui::IsKeyPressed(mSwitchKey, false);

      if (has_activate) {
        if (mActivateBackwardsIsModifier && has_activate_backwards)
          handleEvent(EventType::ActivateBackwards);
        else
          handleEvent(EventType::Activate);
      } else if (!mActivateBackwardsIsModifier && has_activate_backwards)
        handleEvent(EventType::ActivateBackwards);
      else if (has_switch)
        handleEvent(EventType::Switch);
    }

    virtual void _drawMenuContent() override {
      CheckboxOption("Play sound effects:", &mPlaySoundFX, [&]() {
        mBoltPowerUp.mPlaySoundFX  = mPlaySoundFX;
        mBoostPowerUp.mPlaySoundFX = mPlaySoundFX;
        mBargePowerUp.mPlaySoundFX = mPlaySoundFX;

        this->GetConfig()["PlaySoundFX"].SetBool(mPlaySoundFX);
        this->SaveConfig();
      });

      SliderOption("Sound effects volume:", &mSoundFXVolume, 0.0f, 1.0f, [&]() {
        mSoundFXVolume = std::clamp(mSoundFXVolume, 0.0f, 1.0f);
        // TODO: Instead of master volume, set SoundGroup volume
        FMODWrapper::Get().GetMasterChannelGroup()->setVolume(mSoundFXVolume);

        this->GetConfig()["SoundFXVolume"].SetFloat(mSoundFXVolume);
        this->SaveConfig();
      });

      if (SliderOption("Icon size multiplier:", &mIconSizeMult, 0.0f, 5.0f, [&]() {
            this->GetConfig()["IconSizeMultiplier"].SetFloat(mIconSizeMult);
            this->SaveConfig();
          })) {
        mPowerupIconSize = mDefaultPowerupIconSize * mIconSizeMult;
      };

      KeyCatcherOption("Activate key:", &mActivateKey, [&]() {
        this->GetConfig()["ActivateKey"].SetInt(static_cast<std::int32_t>(mActivateKey));
        this->SaveConfig();
      });

      KeyCatcherOption("Activate backwards key:", &mActivateBackwardsKey, [&]() {
        this->GetConfig()["ActivateBackwardsKey"].SetInt(static_cast<std::int32_t>(mActivateBackwardsKey));
        this->SaveConfig();
      });

      CheckboxOption("Make backwards key a modifier:", &mActivateBackwardsIsModifier, [&]() {
        this->GetConfig()["ActivateBackwardsIsModifier"].SetBool(mActivateBackwardsIsModifier);
        this->SaveConfig();
      });

      KeyCatcherOption("Switch powerup key:", &mSwitchKey, [&]() {
        this->GetConfig()["SwitchKey"].SetInt(static_cast<std::int32_t>(mSwitchKey));
        this->SaveConfig();
      });

      OptionsHeader(
          "Barge Powerup",
          [&]() {
            SliderOption("Lock time:", &mBargePowerUp.mLockTime, 0.0f, 10.0f, [&]() {
              this->GetConfig()["BargeConfig"]["LockTime"].SetFloat(mBargePowerUp.mLockTime);
              this->SaveConfig();
            });
            CheckboxOption("Shake camera:", &mBargePowerUp.mShakeCamera, [&]() {
              this->GetConfig()["BargeConfig"]["ShakeCamera"].SetBool(mBargePowerUp.mShakeCamera);
              this->SaveConfig();
            });
            SliderOption("Radius:", &mBargePowerUp.mRadius, 1.0f, 200.0f, [&]() {
              this->GetConfig()["BargeConfig"]["EffectRadius"].SetFloat(mBargePowerUp.mRadius);
              this->SaveConfig();
            });
            SliderOption("Push force:", &mBargePowerUp.mForce, 1.0f, 200.0f, [&]() {
              this->GetConfig()["BargeConfig"]["EffectForce"].SetFloat(mBargePowerUp.mForce);
              this->SaveConfig();
            });
          },
          ImGuiTreeNodeFlags_DefaultOpen);
      OptionsHeader(
          "Bolt Powerup",
          [&]() {
            SliderOption("Lock time:", &mBoltPowerUp.mLockTime, 0.0f, 10.0f, [&]() {
              this->GetConfig()["BoltConfig"]["LockTime"].SetFloat(mBoltPowerUp.mLockTime);
              this->SaveConfig();
            });
            CheckboxOption("Destroy bolt on collision:", &mBoltPowerUp.mDestroyBoltOnCollision, [&]() {
              this->GetConfig()["BoltConfig"]["DestroyBoltOnCollision"].SetBool(mBoltPowerUp.mDestroyBoltOnCollision);
              this->SaveConfig();
            });
            SliderOption("Radius:", &mBoltPowerUp.mBoltStrength, 1.0f, 11.0f, [&]() {
              this->GetConfig()["BoltConfig"]["BoltStrength"].SetFloat(mBoltPowerUp.mBoltStrength);
              this->SaveConfig();
            });
          },
          ImGuiTreeNodeFlags_DefaultOpen);
      OptionsHeader(
          "Boost Powerup",
          [&]() {
            SliderOption("Lock time:", &mBoostPowerUp.mLockTime, 0.0f, 10.0f, [&]() {
              this->GetConfig()["BoostConfig"]["LockTime"].SetFloat(mBoostPowerUp.mLockTime);
              this->SaveConfig();
            });
            SliderOption("Boost multiplier:", &mBoostPowerUp.mMultiplier, 1.0f, 11.0f, [&]() {
              this->GetConfig()["BoostConfig"]["BoostMultiplier"].SetFloat(mBoostPowerUp.mMultiplier);
              this->SaveConfig();
            });
          },
          ImGuiTreeNodeFlags_DefaultOpen);
    }
    virtual void _drawOverlay(ImGuiViewport* viewport) override {
      if (!NFSPluginSDK::IsGameplayRunning()) return;

      drawPowerUp(viewport, mBoltPowerUp, (viewport->Size.x / 2.0f) - (mPowerupIconSize.x / 2.0f) - (ImGui::GetFrameHeight()) - mPowerupIconSize.x);
      drawPowerUp(viewport, mBoostPowerUp, (viewport->Size.x / 2.0f) - (mPowerupIconSize.x / 2.0f));
      drawPowerUp(viewport, mBargePowerUp, (viewport->Size.x / 2.0f) + (mPowerupIconSize.x / 2.0f) + (ImGui::GetFrameHeight()));
    }
    virtual void _d3dDeviceReset(LPDIRECT3DDEVICE9 pDevice) override { createTextures(pDevice); }

   public:
    ModBlurPowerups() :
        IMod("Blur Powerups", "Simulates powerups from Blur the game."),
        mPlaySoundFX(this->GetConfig()["PlaySoundFX"].GetBool()),
        mSoundFXVolume(this->GetConfig()["SoundFXVolume"].GetFloat()),
        mIconSizeMult(this->GetConfig()["IconSizeMultiplier"].GetFloat()),
        mActivateKey(static_cast<ImGuiKey>(this->GetConfig()["ActivateKey"].GetInt())),
        mActivateBackwardsKey(static_cast<ImGuiKey>(this->GetConfig()["ActivateBackwardsKey"].GetInt())),
        mActivateBackwardsIsModifier(this->GetConfig()["ActivateBackwardsIsModifier"].GetBool()),
        mSwitchKey(static_cast<ImGuiKey>(this->GetConfig()["SwitchKey"].GetInt())),
        mDefaultPowerupIconSize(ImVec2()),
        mPowerupIconSize(ImVec2()),
        mActivePowerUp(details::ModBlurPowerups::PowerUpType::Boost),
        mBoltPowerUp(this->GetConfig()["BoltConfig"]),
        mBoostPowerUp(this->GetConfig()["BoostConfig"]),
        mBargePowerUp(this->GetConfig()["BargeConfig"]) {
      mBoltPowerUp.mPlaySoundFX  = mPlaySoundFX;
      mBoostPowerUp.mPlaySoundFX = mPlaySoundFX;
      mBargePowerUp.mPlaySoundFX = mPlaySoundFX;
    }
  };
}  // namespace BAM::Extensions

#endif  // BAM_BLURPOWERUPS_H
