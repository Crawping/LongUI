﻿#define _CRT_SECURE_NO_WARNINGS
#include "LongUI.h"

static const char* const hello_world = u8R"(<?xml version="1.0" encoding="utf-8"?>
<Window>
    <Text text="Hello, world!"/>
</Window>
)";

// Entry
int WINAPI WinMain(HINSTANCE, HINSTANCE, char*, int nCmdShow) {
    ::OleInitialize(nullptr);
    UIManager.Initialize();
    UIManager.CreateUIWindow(hello_world)->ShowWindow(nCmdShow);
    UIManager.Run();
    UIManager.Uninitialize();
    ::OleUninitialize();
    return 0;
}
