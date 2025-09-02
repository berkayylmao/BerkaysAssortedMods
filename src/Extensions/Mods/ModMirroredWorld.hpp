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

#ifndef BAM_MODMIRROREDWORLD_H
#define BAM_MODMIRROREDWORLD_H
#pragma once

#include "pch.h"
#include "IMod.hpp"

namespace BAM::Extensions {
  namespace details::MirroredWorld {
    namespace MW05 {
      namespace FlipSteering {
        inline void __stdcall ParseAction(OpenMW::InputPlayer* input, OpenMW::ActionRef action) {
          input->mControls.mBanking          = -input->mControls.mBanking;
          input->mControls.mSteering         = -input->mControls.mSteering;
          input->mControls.mSteeringVertical = -input->mControls.mSteeringVertical;
          input->mControls.mStrafeVertical   = -input->mControls.mStrafeVertical;
          input->mControls.mStrafeHorizontal = -input->mControls.mStrafeHorizontal;

          switch (action->mId) {
            case OpenMW::ActionData::ActionID::GAMEACTION_STEERLEFT:
              action->mId = OpenMW::ActionData::ActionID::GAMEACTION_STEERRIGHT;
              break;
            case OpenMW::ActionData::ActionID::GAMEACTION_STEERRIGHT:
              action->mId = OpenMW::ActionData::ActionID::GAMEACTION_STEERLEFT;
              break;
            case OpenMW::ActionData::ActionID::GAMEACTION_TURNLEFT:
              action->mId = OpenMW::ActionData::ActionID::GAMEACTION_TURNRIGHT;
              break;
            case OpenMW::ActionData::ActionID::GAMEACTION_TURNRIGHT:
              action->mId = OpenMW::ActionData::ActionID::GAMEACTION_TURNLEFT;
              break;
          }
        }

        inline void __declspec(naked) detour() {
          _asm {
            push ebp
            mov ebp, esp
            pushad
            mov eax, [edx]
            push eax
            push ecx
            call ParseAction
            popad
            push edx
            call dword ptr [eax+0x30]
            test al, al
            mov esp, ebp
            pop ebp
            retn 4
          }
        }
      }  // namespace FlipSteering
      namespace FlipHorizontally {
        inline constexpr std::uintptr_t sBack   = 0x6CF583;
        inline constexpr std::uintptr_t sInner  = 0x7C4B80;
        inline float                    sFactor = -1.0f;

        inline void __declspec(naked) detour() {
          _asm {
            fmul [sFactor]
            call [sInner]
            jmp [sBack]
          }
        }
      }  // namespace FlipHorizontally
      namespace FlipVertically {
        inline constexpr std::uintptr_t sBack   = 0x6CF545;
        inline constexpr std::uintptr_t sInner  = 0x7C4B80;
        inline float                    sFactor = -1.0f;

        inline void __declspec(naked) detour() {
          _asm {
            fmul [sFactor]
            call [sInner]
            jmp [sBack]
          }
        }
      }  // namespace FlipVertically
    }    // namespace MW05
    namespace Carbon {
      namespace FlipSteering {
        inline void __stdcall ParseAction(OpenCarbon::AIVehicleHuman* ai, OpenCarbon::ActionRef action) {
          ai->mSteerLeft      = -ai->mSteerLeft;
          ai->mSteerRight     = -ai->mSteerRight;
          ai->mOverSteerLeft  = -ai->mOverSteerLeft;
          ai->mOverSteerRight = -ai->mOverSteerRight;
          ai->mSteerLeft      = -ai->mSteerLeft;

          switch (action->mId) {
            case OpenCarbon::ActionData::ActionID::GAMEACTION_STEERLEFT:
              action->mId = OpenCarbon::ActionData::ActionID::GAMEACTION_STEERRIGHT;
              break;
            case OpenCarbon::ActionData::ActionID::GAMEACTION_STEERRIGHT:
              action->mId = OpenCarbon::ActionData::ActionID::GAMEACTION_STEERLEFT;
              break;
            case OpenCarbon::ActionData::ActionID::GAMEACTION_OVERSTEERLEFT:
              action->mId = OpenCarbon::ActionData::ActionID::GAMEACTION_OVERSTEERRIGHT;
              break;
            case OpenCarbon::ActionData::ActionID::GAMEACTION_OVERSTEERRIGHT:
              action->mId = OpenCarbon::ActionData::ActionID::GAMEACTION_OVERSTEERLEFT;
              break;
            case OpenCarbon::ActionData::ActionID::GAMEACTION_TURNLEFT:
              action->mId = OpenCarbon::ActionData::ActionID::GAMEACTION_TURNRIGHT;
              break;
            case OpenCarbon::ActionData::ActionID::GAMEACTION_TURNRIGHT:
              action->mId = OpenCarbon::ActionData::ActionID::GAMEACTION_TURNLEFT;
              break;
          }
        }

        inline constexpr std::uintptr_t sBack = 0x428957;
        inline void __declspec(naked) detour() {
          _asm {
            pushad
            push ecx
            push edi
            call ParseAction
            popad

            mov eax, [ecx]
            jmp [sBack]
          }
        }
      }  // namespace FlipSteering
      namespace FlipHorizontally {
        inline constexpr std::uintptr_t sBack   = 0x71B8F7;
        inline constexpr std::uintptr_t sInner  = 0x87EAC0;
        inline float                    sFactor = -1.0f;

        inline void __declspec(naked) detour() {
          _asm {
            fmul [sFactor]
            call [sInner]
            jmp [sBack]
          }
        }
      }  // namespace FlipHorizontally
      namespace FlipVertically {
        inline constexpr std::uintptr_t sBack   = 0x71B8B9;
        inline constexpr std::uintptr_t sInner  = 0x87EAC0;
        inline float                    sFactor = -1.0f;

        inline void __declspec(naked) detour() {
          _asm {
            fmul [sFactor]
            call [sInner]
            jmp [sBack]
          }
        }
      }  // namespace FlipVertically
    }    // namespace Carbon
  }      // namespace details::MirroredWorld

  class ModMirroredWorld : public IMod {
    struct ViewModeInfo {
      std::uint32_t original;
      std::uint32_t changeTo;

      ViewModeInfo() = default;
      ViewModeInfo(std::uint32_t o, std::uint32_t n) : original(o), changeTo(n) {}
    };

    /// VARIABLES

    MemoryEditor::Editor::DetourInfo                  mDetourFlipSteering;
    MemoryEditor::Editor::DetourInfo                  mDetourFlipHorizontally;
    MemoryEditor::Editor::DetourInfo                  mDetourFlipVertically;
    MemoryFieldWrapper<NFSPluginSDK::ePrecullerMode>* mpPrecullerMode;
    std::unordered_map<std::uintptr_t, ViewModeInfo>  mViewModeOverrides;

    /// CONFIG

    bool mFlipHorizontally;
    bool mFlipVertically;
    bool mFlipSteering;

   protected:
    virtual void _onEnabled() override {
      GameLoopWalker::ExecuteOnce([&]() {
        if (mFlipVertically) mDetourFlipVertically.Detour();
        if (mFlipHorizontally) mDetourFlipHorizontally.Detour();
        if (mFlipSteering)
          mDetourFlipSteering.Detour(Variables::sCurGameInfo.mGame == NFSPluginSDK::Game::MW05 ? MemoryEditor::MakeType::Call : MemoryEditor::MakeType::Jump);
        if (mpPrecullerMode) *mpPrecullerMode = NFSPluginSDK::ePrecullerMode::Off;
        if ((mFlipVertically || mFlipHorizontally) && !(mFlipVertically && mFlipHorizontally))
          for (const auto& i : mViewModeOverrides) MemoryEditor::Get().GetRawMemory(i.first).SetValue(i.second.changeTo);
      });
    }
    virtual void _onDisabled() override {
      GameLoopWalker::ExecuteOnce([&]() {
        mDetourFlipHorizontally.Restore();
        mDetourFlipVertically.Restore();
        mDetourFlipSteering.Restore();
        if (mpPrecullerMode) *mpPrecullerMode = NFSPluginSDK::ePrecullerMode::On;
        for (const auto& i : mViewModeOverrides) MemoryEditor::Get().GetRawMemory(i.first).SetValue(i.second.original);
      });
    }

    virtual void _drawMenuContent() override {
      CheckboxOption("Flip horizontally:", &mFlipHorizontally, [&]() {
        this->Refresh();
        this->GetConfig()["FlipHorizontally"].SetBool(mFlipHorizontally);
        this->SaveConfig();
      });
      CheckboxOption("Flip vertically:", &mFlipVertically, [&]() {
        this->Refresh();
        this->GetConfig()["FlipVertically"].SetBool(mFlipVertically);
        this->SaveConfig();
      });
      CheckboxOption("Flip steering:", &mFlipSteering, [&]() {
        this->Refresh();
        this->GetConfig()["FlipSteering"].SetBool(mFlipSteering);
        this->SaveConfig();
      });
    }

   public:
    ModMirroredWorld() :
        IMod("Mirrored World", "Flips the game world on selected axes."),
        mFlipHorizontally(this->GetConfig()["FlipHorizontally"].GetBool()),
        mFlipVertically(this->GetConfig()["FlipVertically"].GetBool()),
        mFlipSteering(this->GetConfig()["FlipSteering"].GetBool()),
        mpPrecullerMode(nullptr) {
      switch (Variables::sCurGameInfo.mGame) {
        case NFSPluginSDK::Game::MW05: {
          mDetourFlipSteering = MemoryEditor::Get().GetDetour(0x6A89EA, reinterpret_cast<std::uintptr_t>(&details::MirroredWorld::MW05::FlipSteering::detour));
          mDetourFlipHorizontally =
              MemoryEditor::Get().GetDetour(0x6CF57E, reinterpret_cast<std::uintptr_t>(&details::MirroredWorld::MW05::FlipHorizontally::detour));
          mDetourFlipVertically =
              MemoryEditor::Get().GetDetour(0x6CF540, reinterpret_cast<std::uintptr_t>(&details::MirroredWorld::MW05::FlipVertically::detour));
          mpPrecullerMode = std::addressof(OpenMW::Variables::PrecullerMode);
        } break;
        case NFSPluginSDK::Game::Carbon: {
          mDetourFlipSteering =
              MemoryEditor::Get().GetDetour(0x428951, reinterpret_cast<std::uintptr_t>(&details::MirroredWorld::Carbon::FlipSteering::detour));
          mDetourFlipHorizontally =
              MemoryEditor::Get().GetDetour(0x71B8F2, reinterpret_cast<std::uintptr_t>(&details::MirroredWorld::Carbon::FlipHorizontally::detour));
          mDetourFlipVertically =
              MemoryEditor::Get().GetDetour(0x71B8B4, reinterpret_cast<std::uintptr_t>(&details::MirroredWorld::Carbon::FlipVertically::detour));
          mpPrecullerMode = std::addressof(OpenCarbon::Variables::PrecullerMode);

          ViewModeInfo info(2, 3);
          mViewModeOverrides[0x71BF63] = info;
          mViewModeOverrides[0x72D9F8] = info;
          mViewModeOverrides[0x72E334] = info;
          mViewModeOverrides[0x72E625] = info;
          mViewModeOverrides[0x72EDA0] = info;
          mViewModeOverrides[0x731251] = info;
        } break;
      }
    }
  };
}  // namespace BAM::Extensions

#endif  // BAM_MODMIRROREDWORLD_H
