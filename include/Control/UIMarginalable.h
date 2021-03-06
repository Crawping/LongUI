﻿#pragma once
/**
* Copyright (c) 2014-2016 dustpg   mailto:dustpg@gmail.com
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/

#include "UIControl.h"

// LongUI namespace
namespace LongUI {
    /// <summary>
    /// marginal-able control
    /// </summary>
    /// <remarks>
    /// like: hamburger menu, menu bar, tool bar, tab bar, scroll bar, barabara, etc
    /// UIMarginalable just a 'marginal-able' control, not real 'marginal' control
    /// 'marginal' control created by parent in 'Event_TreeBuildingFinished'
    /// normal control created by ui-manager in 'CreateUIWindow'
    /// </remarks>
    class UIMarginalable : public UIControl {
        // 父类声明
        using Super = UIControl;
    public:
        /// <summary>
        /// position of marginal control
        /// </summary>
        enum MarginalControl : uint16_t {
            Control_Unknown= 0xFFFFui16,// unknown
            Control_Left = 0,           // control at left
            Control_Top,                // control at top
            Control_Right,              // control at right
            Control_Bottom,             // control at bottom
            MARGINAL_CONTROL_SIZE       // control-size
        };

        /// <summary>
        /// rule for handling overlapping problem in cross area
        /// </summary>
        enum CrossAreaRule : uint16_t {
            Rule_Greedy = 0,            // menubar like
            Rule_Generous,              // scrollbar like
        };

        /// <summary>
        /// Updates the width of the marginal.
        /// </summary>
        /// <remarks>
        /// this method is designed to be helper to change parent's margin, just change "marginal_width"
        /// more detail too see <see cref="LongUI::UIContainer::refresh_marginal_controls"/>
        /// and, defaultly, container limits the marginal control in "marginal-control-zone", but
        /// if you want render in "view-zone", you can changed view_size, view_pos in this method,
        /// more detail too see <see cref="LongUI::UIScrollBarB::UpdateMarginalWidth"/>
        /// </remarks>
        /// <returns></returns>
        virtual void UpdateMarginalWidth() noexcept { };
        // refresh the world transform while in marginal
        void RefreshWorldMarginal() noexcept;
    public:
        // marginal width, parent's real margin(ltrb) = parent's original(ltrb) + this' marginal_width
        // example: classic scrollbar's marginal_width = it's width/height
        //          modern scrollbar(show when mouse pointed)'s marginal_width = 0
        float                   marginal_width = 0.f;
        // rule for cross area
        CrossAreaRule     const rule = CrossAreaRule::Rule_Generous;
        // marginal type
        MarginalControl   const marginal_type = MarginalControl::Control_Unknown;
    protected:
        // ctor
        UIMarginalable(UIContainer* cp) noexcept : Super(cp) {}
        // something must do before deleted
        void before_deleted() noexcept { Super::before_deleted(); }
        // init
        void initialize(pugi::xml_node node) noexcept;
        // init without xml-node
        void initialize() noexcept;
#ifdef LongUIDebugEvent
    protected:
        // debug infomation
        virtual bool debug_do_event(const LongUI::DebugEventInformation&) const noexcept override;
#endif
    };
#ifdef LongUIDebugEvent
    // 重载?特例化 GetIID
    template<> const IID& GetIID<LongUI::UIMarginalable>() noexcept;
#endif
}