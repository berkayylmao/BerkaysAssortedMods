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

#ifndef BAM_MODDISABLEGAMEPAUSE_H
#define BAM_MODDISABLEGAMEPAUSE_H
#pragma once

#include "pch.h"
#include "IMod.hpp"

namespace BAM::Extensions {
  namespace DisableGamePause {
    inline constinit bool   sDisablePause = false;
    inline LRESULT CALLBACK WndProc(HWND, UINT uMsg, WPARAM wParam, LPARAM) {
      if (!sDisablePause) return FALSE;

      switch (uMsg) {
        case WM_MOUSEMOVE:
        case WM_MOUSELEAVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONDBLCLK:
        case WM_XBUTTONDOWN:
        case WM_XBUTTONDBLCLK:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_XBUTTONUP:
        case WM_MOUSEWHEEL:
        case WM_MOUSEHWHEEL:
          return !WndProcWalker::IsWindowActive();
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
          return !WndProcWalker::IsWindowActive();
        case WM_ACTIVATE:
        case WM_ACTIVATEAPP:
          return (LOWORD(wParam) == WA_INACTIVE) || wParam == FALSE;
      }

      return FALSE;
    }
  }  // namespace DisableGamePause

  class ModDisableGamePause : public IMod {
    /// STATICS

    static inline std::uint32_t sGamePauseRequestDisableBytes;
    static inline std::uint32_t sGamePauseRequestRestoreBytes;

    /// VARIABLES

    MemoryEditor::Editor::RawMemory mGamePauseRequestMemory;
    bool                            mBlockGamePause;
    bool                            mBlockWindowPause;

   protected:
    virtual void _onEnabled() override {
      if (mBlockGamePause) GameLoopWalker::ExecuteOnce([&]() { mGamePauseRequestMemory.SetValue<std::uint32_t>(sGamePauseRequestDisableBytes); });
      DisableGamePause::sDisablePause = mBlockWindowPause;
    }
    virtual void _onDisabled() override {
      GameLoopWalker::ExecuteOnce([&]() { mGamePauseRequestMemory.SetValue<std::uint32_t>(sGamePauseRequestRestoreBytes); });
      DisableGamePause::sDisablePause = false;
    }

    virtual void _drawMenuContent() override {
      CheckboxOption("Block game pause:", &mBlockGamePause, [&]() {
        this->Refresh();
        this->GetConfig()["BlockGamePause"].SetBool(mBlockGamePause);
        this->SaveConfig();
      });
      CheckboxOption("Block window pause:", &mBlockWindowPause, [&]() {
        this->Refresh();
        this->GetConfig()["BlockWindowPause"].SetBool(mBlockWindowPause);
        this->SaveConfig();
      });
    }

   public:
    ModDisableGamePause() :
        IMod("Disable Game Pause", "Blocks some of the game's automatic pause actions."),
        mGamePauseRequestMemory(NULL),
        mBlockGamePause(this->GetConfig()["BlockGamePause"].GetBool()),
        mBlockWindowPause(this->GetConfig()["BlockWindowPause"].GetBool()) {
      switch (Variables::sCurGameInfo.mGame) {
        case NFSPluginSDK::Game::MW05: {
          mGamePauseRequestMemory       = MemoryEditor::Get().GetRawMemory(0x525B3E);
          sGamePauseRequestDisableBytes = 0x90C3C031;
          sGamePauseRequestRestoreBytes = 0x91CAE4A1;
        } break;
        case NFSPluginSDK::Game::Carbon: {
          mGamePauseRequestMemory       = MemoryEditor::Get().GetRawMemory(0x4A6300);
          sGamePauseRequestDisableBytes = 0x90C3C031;
          sGamePauseRequestRestoreBytes = 0xAD180D8B;
        } break;
        case NFSPluginSDK::Game::ProStreet: {
          mGamePauseRequestMemory       = MemoryEditor::Get().GetRawMemory(0x5059C6);
          sGamePauseRequestDisableBytes = 0x90C3C031;
          sGamePauseRequestRestoreBytes = 0x0EDC0539;
        } break;
      }

      DisableGamePause::sDisablePause = mBlockWindowPause;
      WndProcWalker::AddWndProc(DisableGamePause::WndProc);
    }
  };
}  // namespace BAM::Extensions

#endif  // BAM_MODDISABLEGAMEPAUSE_H
