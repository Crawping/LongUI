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
#include "../Component/Element.h"

// LongUI namespace
namespace LongUI {
    // default slider control 默认滑块控件
    class UISlider : public UIControl {
        // super class
        using Super = UIControl;
        // clean this control 清除控件
        virtual void cleanup() noexcept override;
    public:
        // Render 渲染 
        virtual void Render() const noexcept override;
        // udate 刷新
        virtual void Update() noexcept override;
        // do event 事件处理
        virtual bool DoEvent(const LongUI::EventArgument& arg) noexcept override;
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
        void render_chain_foreground() const noexcept;
    public:
        // create 创建
        static auto CreateControl(CreateEventType type, pugi::xml_node) noexcept ->UIControl*;
        // constructor 构造函数
        UISlider(UIContainer* cp) noexcept : Super(cp) { }
    private:
        // something must do before deleted
        void before_deleted() noexcept { Super::before_deleted(); }
        // init with xml-node
        void initialize(pugi::xml_node node) noexcept;
        // init without xml-node
        void initialize() noexcept;
    public:
        // set state
        void SetControlState(ControlState state) noexcept { this->StartRender(m_uiElement.SetBasicState(state)); }
        // get state
        auto GetControlState() const noexcept { m_uiElement.GetNowBasicState(); }
        // get value in [0, 1]
        auto GetValue01() const noexcept { return m_fValue; }
        // get value in [start, end]
        auto GetValueSE() const noexcept { return (m_fEnd - m_fStart) * m_fValue + m_fStart; }
        // is Vertical Slider?
        auto IsVerticalSlider() const noexcept { return m_bVerticalSlider; }
        // is Horizontal Slider?
        auto IsHorizontalSlider() const noexcept { return !m_bVerticalSlider; }
        // set value [0, 1]
        void SetValue01(float value) noexcept;
        // set value [start, end]
        void SetValueSE(float value) noexcept { this->SetValue01((value - m_fStart) / (m_fEnd - m_fStart)); }
    protected:
        // destructor 析构函数
        ~UISlider() noexcept { }
        // deleted function
        UISlider(const UISlider&) = delete;
    protected:
        // slider rect
        D2D1_RECT_F                 m_rcThumb = D2D1_RECT_F{0.f};
        // ui element
        Component::Element4Button   m_uiElement;
        // value range[0, 1]
        float                       m_fValue = 0.f;
        // value change step
        float                       m_fStep = 0.1f;
        // start
        float                       m_fStart = 0.f;
        // end
        float                       m_fEnd = 1.f;
    public:
        // size of thumb
        D2D1_SIZE_F const           thumb_size = D2D1_SIZE_F{10.f, 20.f};
    protected:
        // click posistion
        float                       m_fClickPosition = 0.f;
        // is mouse click in
        bool                        m_bMouseClickIn = false;
        // is mouse move in
        bool                        m_bMouseMoveIn = false;
        // is vertical slider
        bool                        m_bVerticalSlider = false;
        // default background
        bool                        m_bDefaultBK = true;
        // callback
        UICallBack                  m_event;
#ifdef LongUIDebugEvent
    protected:
        // debug infomation
        virtual bool debug_do_event(const LongUI::DebugEventInformation&) const noexcept override;
#endif
    };
#ifdef LongUIDebugEvent
    // 重载?特例化 GetIID
    template<> const IID& GetIID<LongUI::UISlider>() noexcept;
#endif
}