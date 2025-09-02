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

#ifndef BAM_MODD3DPOINTVIEW_H
#define BAM_MODD3DPOINTVIEW_H
#pragma once

#include "pch.h"
#include "IMod.hpp"

namespace BAM::Extensions {
  class ModD3DPointView : public IMod {
   protected:
    virtual void _onEnabled() override { DirectXWalker::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT); }
    virtual void _onDisabled() override { DirectXWalker::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); }

   public:
    ModD3DPointView() : IMod("D3D Point View", "Makes game render in D3DFILL_POINT mode.\n(Incompatible with D3D Wireframe View)") {}
  };
}  // namespace BAM::Extensions

#endif  // BAM_MODD3DPOINTVIEW_H
