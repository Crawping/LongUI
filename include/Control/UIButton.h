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

#include "UIText.h"
#include "../Component/Element.h"

// LongUI namespace
namespace LongUI {
    // default button control 默认按钮控件
    class UIButton : public UIText {
        // super class
        using Super = UIText;
        // clean this control 清除控件
        virtual void cleanup() noexcept override;
    public:
        // Render 渲染 
        virtual void Render() const noexcept override;
        // udate 刷新
        virtual void Update() noexcept override;
        // do event
        virtual bool DoEvent(const EventArgument& arg) noexcept override;
        // do mouse event
        virtual bool DoMouseEvent(const MouseEventArgument& arg) noexcept override;
        // recreate 重建
        virtual auto Recreate() noexcept ->HRESULT override;
    protected:
        // ui call
        virtual bool uniface_addevent(SubEvent sb, UICallBack&& call) noexcept override;
        // render chain -> background
        void render_chain_background() const noexcept;
        // render chain -> mainground
        void render_chain_main() const noexcept { return Super::render_chain_main(); }
        // render chain -> foreground
        void render_chain_foreground() const noexcept { return Super::render_chain_foreground(); }
    public:
        // create 创建
        static auto CreateControl(CreateEventType, pugi::xml_node) noexcept ->UIControl*;
        // constructor 构造函数
        UIButton(UIContainer* cp) noexcept : Super(cp) {}
        // get state
        auto GetControlState() const noexcept { return m_uiElement.GetNowBasicState(); }
        // set state
        void SetControlState(ControlState state) noexcept;
    protected:
        // something must do before deleted
        void before_deleted() noexcept { Super::before_deleted(); }
        // initialize, maybe you want call v-method
        void initialize(pugi::xml_node node) noexcept;
        // destructor 析构函数
        ~UIButton() noexcept {};
        // deleted function
        UIButton(const UIButton&) = delete;
    protected:
        // callback
        UICallBack                  m_event;
        // color of border
        D2D1_COLOR_F                m_aBorderColor[STATE_COUNT];
        // element
        Component::Element4Button   m_uiElement;
        // target state when clicked
        ControlState                m_tarStateClick = LongUI::State_Hover;
        // unused var
        char                        btnunused[sizeof(void*)-sizeof(ControlState)];
#ifdef LongUIDebugEvent
    protected:
        // debug infomation
        virtual bool debug_do_event(const LongUI::DebugEventInformation&) const noexcept override;
#endif
    };
#ifdef LongUIDebugEvent
    // 重载?特例化 GetIID
    template<> const IID& GetIID<LongUI::UIButton>() noexcept;
#endif
}