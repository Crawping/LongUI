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

#include "UIMarginalable.h"

// LongUI namespace
namespace LongUI {
#if 1
    // base container control class -- 基本容器类
    class UIContainer : public UIMarginalable {
        // super class
        using Super = UIMarginalable;
#else
    // base container control class
    class LongUIAPI UIContainer : public UIControl {
        // super class
        using Super = UIControl;
#endif
    public:
        // render this
        virtual void Render() const noexcept override = 0;
        // update this
        virtual void Update() noexcept override;
        // do event
        virtual bool DoEvent(const LongUI::EventArgument& arg) noexcept override;
        // do mouse event 
        virtual bool DoMouseEvent(const LongUI::MouseEventArgument& arg) noexcept override;
        // recreate this
        virtual auto Recreate() noexcept ->HRESULT override;
    public:
#ifdef _DEBUG
    private:
        // debug for_each
        virtual void debug_for_each(const CUIFunction<void(UIControl*)>& call) noexcept = 0;
    public:
        // debug for_each helper
        template<typename T> void DebugForEach(T call) noexcept {
            CUIFunction<void(UIControl*)> func(call);
            this->debug_for_each(func);
        }
#endif
        // find child control by mouse point
        virtual auto FindChild(const D2D1_POINT_2F& pt) noexcept ->UIControl* ;
        // refresh layout
        virtual void RefreshLayout() noexcept = 0;
        // push back, do push marginalal-control for UIContainer
        virtual void Push(UIControl* child) noexcept = 0;
        // remove child
        virtual void Remove(UIControl* child) noexcept { this->release_child(child); }
    public:
        // ctor
        UIContainer(UIContainer* cp) noexcept;
        // cast to CreateEventType
        auto GetCET() const noexcept { 
            static_assert(sizeof(CreateEventType) == sizeof(this), "bad cast");
            return static_cast<CreateEventType>(reinterpret_cast<size_t>(this)); 
        }
        // before update
        //void BeforeUpdateContainer() noexcept;
    protected:
        // something must do before deleted
        void before_deleted() noexcept;
        // init
        void initialize(pugi::xml_node node) noexcept;
        // init
        void initialize() noexcept;
        // dtor
        ~UIContainer() noexcept {
#ifdef _DEBUG
            for (auto ctrl : m_apMarginalControlRA) {
                assert(ctrl == nullptr && "call before_deleted()!");
            }
#endif
        }
    public:
        // get length/count of children
        auto GetLength() const noexcept { return m_cChildrenCount; }
        // get length/count of children
        auto GetChildrenCount() const noexcept { return m_cChildrenCount; }
    public:
        // get content width - zoomed
        auto GetContentWidthZoomed() const noexcept { return m_2fContentSize.width / m_2fZoom.width; }
        // get content height - zoomed
        auto GetContentHeightZoomed() const noexcept { return m_2fContentSize.height / m_2fZoom.height; }
        // get view.width - zoomed
        auto GetViewWidthZoomed() const noexcept { return this->view_size.width / m_2fZoom.width; }
        // get view.height - zoomed
        auto GetViewHeightZoomed() const noexcept { return this->view_size.height / m_2fZoom.height; }
        // get offset.x - zoomed
        auto GetOffsetXZoomed() const noexcept { return m_2fOffset.x / m_2fZoom.width; }
        // get offset.y - zoomed
        auto GetOffsetYZoomed() const noexcept { return m_2fOffset.y / m_2fZoom.height; }
        // set offset.x -zoomed
        void SetOffsetXZoomed(float value) noexcept { return this->SetOffsetX(value * m_2fZoom.width); }
        // set offset.y - zoomed
        void SetOffsetYZoomed(float value) noexcept { return this->SetOffsetY(value * m_2fZoom.height); }
        // set offset -zoomed
        auto SetOffsetZoomed(int xy, float value) noexcept { return xy ? SetOffsetYZoomed(value) : SetOffsetXZoomed(value); }
    public:
        // get content width - without zooming
        auto GetContentWidth() const noexcept { return m_2fContentSize.width; }
        // get content height - without zooming
        auto GetContentHeight() const noexcept { return m_2fContentSize.height; }
        // get view.width - without zooming
        auto GetViewWidth() const noexcept { return this->view_size.width; }
        // get view.height - without zooming
        auto GetViewHeight() const noexcept { return this->view_size.height; }
        // get offset.x - without zooming
        auto GetOffsetX() const noexcept { return m_2fOffset.x; }
        // get offset.y - without zooming
        auto GetOffsetY() const noexcept { return m_2fOffset.y; }
        // set offset.x - without zooming
        void SetOffsetX(float value) noexcept;
        // set offset.y - without zooming
        void SetOffsetY(float value) noexcept;
        // set offset - without zooming
        auto SetOffset(int xy, float value) noexcept { return xy ? SetOffsetY(value) : SetOffsetX(value); }
        // get zoom in x
        auto GetZoomX() const noexcept { return m_2fZoom.width; }
        // get zoom in y
        auto GetZoomY() const noexcept { return m_2fZoom.height; }
        // set zoom
        void SetZoom(float x, float y) noexcept;
        // get zoom 
        auto GetZoom(int xy) const noexcept { return xy ? this->GetZoomY() : this->GetZoomX(); }
        // get top margin offset
        auto GetTopMarginOffset() const noexcept { return this->margin_rect.top - m_orgMargin.top; }
        // get left margin offset
        auto GetLeftMarginOffset() const noexcept { return this->margin_rect.left - m_orgMargin.left; }
        // get popular child(like radio button)
        auto GetPopularChild() const noexcept { return m_pPopularChild; }
        // set popular child(like radio button)
        void SetPopularChild(UIControl* ctrl) noexcept;
    private:
        // refresh marginal controls
        void refresh_marginal_controls() noexcept;
        // auto template size
        ///void refresh_auto_template_size();
    protected:
        // do render for child
        static void child_do_render(const UIControl* ctrl) noexcept;
        // render chain -> background
        void render_chain_background() const noexcept { return Super::render_chain_background(); }
        // render chain -> mainground
        void render_chain_main() const noexcept;
        // render chain -> foreground
        void render_chain_foreground() const noexcept { return Super::render_chain_foreground(); }
        // release child
        void release_child(UIControl* ctrl) noexcept { 
            assert(ctrl && (ctrl->parent == this || !ctrl->parent)); 
            force_cast(ctrl->parent) = nullptr;
            ctrl->Release(); 
        }
        // after insert
        void after_insert(UIControl* child) noexcept;
    public:
        // get marginal control
        auto GetMarginalControl(UIMarginalable::MarginalControl type) noexcept {
            assert(type >= 0 && type < UIMarginalable::MARGINAL_CONTROL_SIZE && "out of range");
            return m_apMarginalControlRA[type];
        }
        // begin for mc
        auto MCBegin() const noexcept { return m_apMarginalControlCO; }
        // end for mc
        auto MCEnd() const noexcept { return m_ppEndMC; }
    protected:
        // marginal controls - Random Access
        UIMarginalable*         m_apMarginalControlRA[UIMarginalable::MARGINAL_CONTROL_SIZE];
        // marginal controls - continuously
        UIMarginalable*         m_apMarginalControlCO[UIMarginalable::MARGINAL_CONTROL_SIZE];
        // end of marginal controls
        UIMarginalable**        m_ppEndMC = m_apMarginalControlCO;
        // popular child
        UIControl*              m_pPopularChild = nullptr;
        // mouse pointed control
        UIControl*              m_pMousePointed = nullptr;
        // count of children, just make "GetLength/Count" to faster
        uint32_t                m_cChildrenCount = 0;
        // unused float
        float                   m_fUnusedCnr = 0.f;
        // orginal margin
        D2D1_RECT_F             m_orgMargin = D2D1_RECT_F{0.f};
        // template size
        D2D1_SIZE_F             m_2fTemplateSize = D2D1_SIZE_F{0.f};
        // offset position
        D2D1_POINT_2F           m_2fOffset = D2D1_POINT_2F{0.f};
        // size of content
        D2D1_SIZE_F             m_2fContentSize = D2D1_SIZE_F{0.f};
        // zoom size
        D2D1_SIZE_F             m_2fZoom = D2D1_SIZE_F{1.f,1.f};
    public:
        // helper for update
        template<typename TSuperClass, typename TIterator>
        void UpdateHelper(TIterator itrbegin, TIterator itrend) noexcept;
        // helper for render
        template<typename TIterator, typename RT>
        void RenderHelper(RT render_target,TIterator itrbegin, TIterator itrend) const noexcept;
#ifdef LongUIDebugEvent
    protected:
        // debug infomation
        virtual bool debug_do_event(const LongUI::DebugEventInformation&) const noexcept override;
    };
    // 重载?特例化 GetIID
    template<> const IID& GetIID<LongUI::UIContainer>() noexcept;
#else
    };
#endif
    // ------------------------------ INLINE IMPLEMENT -------------------------------
    // helper for update
    template<typename TSuperClass, typename TIterator>
    inline void UIContainer::UpdateHelper(TIterator itrbegin, TIterator itrend) noexcept {
        // update for super
        TSuperClass::Update();
        // change if need refresh
        if (this->IsNeedRefreshWorld()) {
            // limit
            D2D1_RECT_F limit_of_this = {
                this->visible_rect.left + this->margin_rect.left * this->world._11,
                this->visible_rect.top + this->margin_rect.top * this->world._22,
                this->visible_rect.right - this->margin_rect.right * this->world._11,
                this->visible_rect.bottom - this->margin_rect.bottom * this->world._22,
            };
            // refresh children
            for (auto itr = itrbegin; itr != itrend; ++itr) {
                auto ctrl = (*itr);
                // set change
                ctrl->SetControlWorldChanged();
                ctrl->RefreshWorld();
                // world change visible-rect
                D2D1_RECT_F clip_rect; ctrl->GetClipRect(clip_rect);
                auto lt = LongUI::TransformPoint(ctrl->world, reinterpret_cast<D2D1_POINT_2F&>(clip_rect.left));
                auto rb = LongUI::TransformPoint(ctrl->world, reinterpret_cast<D2D1_POINT_2F&>(clip_rect.right));
                ctrl->visible_rect.left = std::max(lt.x, limit_of_this.left);
                ctrl->visible_rect.top = std::max(lt.y, limit_of_this.top);
                ctrl->visible_rect.right = std::min(rb.x, limit_of_this.right);
                ctrl->visible_rect.bottom = std::min(rb.y, limit_of_this.bottom);
            }
            // handed it
            this->ControlLayoutChangeHandled();
        }
        // update marginal control
        for (auto itr = this->MCBegin(); itr != MCEnd(); ++itr) {
            auto ctrl = (*itr);
            ctrl->Update();
            ctrl->AfterUpdate();
        }
        // update children
        for (auto itr = itrbegin; itr != itrend; ++itr) {
            auto ctrl = (*itr);
            ctrl->Update();
            ctrl->AfterUpdate();
        }
    }
    // helper for render
    template<typename TIterator, typename RT>
    inline void UIContainer::RenderHelper(RT target, TIterator itrbegin, TIterator itrend) const noexcept {
        // push clip for children
        {
            D2D1_RECT_F clip_rect; this->GetViewRect(clip_rect);
            target->PushAxisAlignedClip(
                &clip_rect, D2D1_ANTIALIAS_MODE_ALIASED
            );
        }
        // render children
        for (auto itr = itrbegin; itr != itrend; ++itr) {
            this->child_do_render(*itr);
        }
        // pop
        target->PopAxisAlignedClip();
    }
}
