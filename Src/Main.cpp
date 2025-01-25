#include "Minecraft.h"

extern "C" void __cdecl __CxxFrameHandler3() { RXDK_LOG_INFO("__CxxFrameHandler3() has been called\n"); while (true); }


int RXDKRuntimeDebugLevel = 0;

using namespace Minecraft;

void __cdecl main()
{
    RXDK_SET_DEBUG_LEVEL(RXDK_DEBUG_LEVEL_TRACE);
    RXDK_LOG_INFO("Minecraft Xbox Edition\nDeveloped by: napalmtorch\n");
    App::Init();
    App::Run();
    App::Dispose();
    while (true);
}