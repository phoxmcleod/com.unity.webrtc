﻿#include "pch.h"
#include "Context.h"
#include "IUnityGraphics.h"
#include "IUnityGraphicsD3D11.h"
#include "GraphicsDevice/GraphicsDevice.h"

namespace WebRTC
{
    IUnityInterfaces* s_UnityInterfaces = nullptr;
    IUnityGraphics* s_Graphics = nullptr;

    Context* s_context;
}
using namespace WebRTC;
//get d3d11 device
static void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType)
{
    switch (eventType)
    {
    case kUnityGfxDeviceEventInitialize:
    {
        GraphicsDevice::GetInstance().Init(s_UnityInterfaces);
        break;
    }
    case kUnityGfxDeviceEventShutdown:
    {
        GraphicsDevice::GetInstance().Shutdown();

        //UnityPluginUnload not called normally
        s_Graphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);
        break;
    }
    case kUnityGfxDeviceEventBeforeReset:
    {
        break;
    }
    case kUnityGfxDeviceEventAfterReset:
    {
        break;
    }
    };
}
// Unity plugin load event
extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces* unityInterfaces)
{
    s_UnityInterfaces = unityInterfaces;
    s_Graphics = unityInterfaces->Get<IUnityGraphics>();
    s_Graphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);
    OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
}
extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginUnload()
{
    s_Graphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);
}

static void UNITY_INTERFACE_API OnRenderEvent(int eventID)
{
    if (s_context != nullptr)
    {
        s_context->EncodeFrame();
    }
}

extern "C" UnityRenderingEvent UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API GetRenderEventFunc(Context* context)
{
    s_context = context;
    return OnRenderEvent;
}
