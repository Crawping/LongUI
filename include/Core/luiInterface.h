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

#include "../luibase.h"
#include "../luiconf.h"
#include "../Graphics/luiGrDwrt.h"

// longui namespace
namespace LongUI {
    // LongUI UI Interface
    class LONGUI_NOVTABLE IUIInterface { 
    public:
        // add ref count
        virtual auto STDMETHODCALLTYPE AddRef() noexcept ->ULONG = 0;
        // release
        virtual auto STDMETHODCALLTYPE Release() noexcept ->ULONG = 0;
    };
#define LONGUI_BASIC_INTERFACE_IMPL\
    auto STDMETHODCALLTYPE AddRef() noexcept ->ULONG override final { return 2; }\
    auto STDMETHODCALLTYPE Release() noexcept ->ULONG override final { return 1; };
    // script interface
    class LONGUI_NOVTABLE IUIScript : public IUIInterface {
    public:
        // run a section script with event
        virtual auto Evaluation(const ScriptUI&, const LongUI::EventArgument& arg) noexcept ->bool = 0;
        // alloc the script memory and copy into(may be compiled into byte code), return memory size
        virtual auto AllocScript(const char* utf8) noexcept ->LongUI::ScriptUI = 0;
        // free the script memory
        virtual void FreeScript(ScriptUI&) noexcept = 0;
    };
    // text formatter interface
    class LONGUI_NOVTABLE IUITextFormatter : public IUIInterface {
    public:
        // string pair
        struct StringPair { const wchar_t* begin; const wchar_t* end; };
        /// <summary>
        /// Customs the type of the rich.
        /// </summary>
        /// <param name="config">The configuration.</param>
        /// <param name="format">The format.</param>
        /// <remarks>if in RichType::Type_Custom, will call this</remarks>
        /// <returns></returns>
        virtual auto CustomRichType(const DX::FormatTextConfig& config, const wchar_t* format) noexcept ->IDWriteTextLayout* = 0;
        /// <summary>
        /// create inline img interface.
        /// </summary>
        /// <param name="img">The img.</param>
        /// <returns></returns>
        virtual auto XmlImgInterface(const DX::InlineImage& img) noexcept->IDWriteInlineObject* =0;
        /// /// <summary>
        /// eval string for xml formatting.
        /// </summary>
        /// <param name="pair">The pair.</param>
        /// <returns></returns>
        virtual auto XmlEvalString(StringPair pair) noexcept->StringPair = 0;
        /// <summary>
        /// Frees the string(from XmlEvalString).
        /// </summary>
        /// <param name="pair">The pair.</param>
        /// <returns></returns>
        virtual void XmlFreeString(StringPair pair) noexcept = 0;
    };

    // Meta
    struct Meta; struct DeviceIndependentMeta;
    // ui res loader
    class LONGUI_NOVTABLE IUIResourceLoader : public IUIInterface {
    public:
        // resource type
        enum ResourceType : uint32_t {
            Type_Null =0,       // none resource
            Type_Bitmap,        // bitmap within ID2D1Bitmap1*
            Type_Brush,         // brush with ID2D1Brush*
            Type_TextFormat,    // text format within IDWriteTextFormat*
            Type_Meta,          // meta within LongUI::Meta
            RESOURCE_TYPE_COUNT,// count of this
        };
    public:
        // get resouce count with type
        virtual auto GetResourceCount(ResourceType type) const noexcept -> size_t = 0;
        // get resouce by index, index in range [0, count),  for Type_Bitmap, Type_Brush, Type_TextFormat
        virtual auto GetResourcePointer(ResourceType type, size_t index) noexcept ->void* = 0;
        // get meta by index, index in range [0, count)
        virtual void GetMeta(size_t index, DeviceIndependentMeta&) noexcept = 0;
    };
    // UI Configure Interface
    class LONGUI_NOVTABLE IUIConfigure : public IUIInterface {
    public:
        // flag
        enum ConfigureFlag : uint32_t {
            // no flag
            Flag_None = 0,
            // all flags
            Flag_All = uint32_t(-1),
            // flag for CPU rendering, if not, will call IUIInterface::ChooseAdapter
            Flag_RenderByCPU = 1 << 0,
            // output debug string?
            Flag_OutputDebugString = 1 << 1,
            // render in anytime, like game
            Flag_RenderInAnytime = 1 << 2,
            // only one system window, like game(all child window will be logic window)
            Flag_OnlyOneSystemWindow = 1 << 3,
            // -------------------------------------------------------------
            // [debug flag in _DEBUG] output font family infomation
            Flag_DbgOutputFontFamily = 1 << 10,
        };
    public:
        /// <summary>
        /// Get flags for configure
        /// </summary>
        /// <returns>flags for configure</returns>
        virtual auto GetConfigureFlag() noexcept ->ConfigureFlag = 0;
        /// <summary>
        /// Get string from table
        /// </summary>
        /// <returns>static string</returns>
        virtual auto GetString(TableString tbl) noexcept -> const wchar_t* = 0;
        /// <summary>
        /// Creates the interfaces.
        /// </summary>
        /// <param name="iid">The interface iid.</param>
        /// <param name="obj">The out interface pointer</param>
        /// <returns>result in HRESULT</returns>
        /// <remarks>
        /// could create:
        ///   - LongUI::IUIResourceLoader*
        ///   - LongUI::IUIScript*
        ///   - LongUI::IUITextFormatter*
        ///   - IDWriteFontCollection*
        /// </remarks>
        virtual auto CreateInterface(const IID& iid, void** obj) noexcept ->HRESULT= 0;
        /// <summary>
        /// get null-end string for template for creating control
        /// </summary>
        /// <returns>null-end string</returns>
        virtual auto GetTemplateString() noexcept ->const char* = 0;
        /// <summary>
        /// Gets the locale name
        /// </summary>
        /// <param name="name">The locale name buffer</param>
        /// <remarks>L"" for local locale name</remarks>
        virtual void GetLocaleName(wchar_t name[/*LOCALE_NAME_MAX_LENGTH*/]) noexcept = 0;
        /// <summary>
        /// Adds the custom control.
        /// </summary>
        /// <remarks>call CUIManager::RegisterControl to add control class</remarks>
        virtual void RegisterSome() noexcept = 0;
        /// <summary>
        /// Chooses the video adapter.
        /// </summary>
        /// <param name="adapters">The adapter array</param>
        /// <param name="length">The length of adapters</param>
        /// <remarks>
        /// if set "Flag_RenderByCPU", you should choose a video card,return the index.
        /// if return code out of range, will set by default(null pointer adapter)
        /// btw, in the adapter list, also include the WARP-adapter
        /// </remarks>
        /// <returns>index of adapters</returns>
        virtual auto ChooseAdapter(const DXGI_ADAPTER_DESC1 adapters[/*length*/], const size_t length /*<=64*/) noexcept ->size_t = 0;
        /// <summary>
        /// Creates the custom window.
        /// </summary>
        /// <param name="type">The type.</param>
        /// <param name="node">The node.</param>
        /// <returns></returns>
        virtual auto CreateCustomWindow(WindowPriorityType type, pugi::xml_node node) noexcept->XUIBaseWindow* = 0;
        /// <summary>
        /// Shows the error.
        /// </summary>
        /// <param name="str_a">String A</param>
        /// <param name="str_b">String B</param>
        /// <returns></returns>
        virtual void ShowError(const wchar_t* str_a, const wchar_t* str_b = nullptr) noexcept = 0;
#ifdef _DEBUG
        /// <summary>
        /// Outputs the debug string in debug mode
        /// </summary>
        /// <param name="level">The debug level.</param>
        /// <param name="string">The debug string.</param>
        /// <param name="flush">if set to <c>true</c> [flush].</param>
        /// <returns></returns>
        virtual auto OutputDebugStringW(DebugStringLevel level, const wchar_t* string, bool flush) noexcept -> void = 0;
#endif
    };
    // UI Undo Redo Commnad
    class LONGUI_NOVTABLE IUICommand : public IUIInterface {
    public:
        // undo
        virtual void Undo() noexcept = 0;
        // redo
        virtual void Redo() noexcept = 0;
    };
    // operator for UIViewport::WindowFlag
    LONGUI_DEFINE_ENUM_FLAG_OPERATORS(IUIConfigure::ConfigureFlag, uint32_t);
}

