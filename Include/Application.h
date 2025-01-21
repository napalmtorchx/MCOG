#pragma once
#include "Common.h"

#define UPDATE_FREQUENCY (1.0f / 20.0f)

namespace Minecraft
{
	class App
	{
        private:
            static char   _dbgtxt[];
            static char   _lnbuff[];
            static int    _fps, _frames;
            static float  _dt, _dtps;
            static float  _sectimer, _uptimer;
            static DWORD  _memused, _memavail, _memtotal;

        public:
            static void Init();
            static void Dispose();
            static void Run();
            static void Update();
            static void UpdateFixed();
            static void UpdateDebugText();
            static void Draw();

        public:
            static LPCSTR GetDebugText();
            static int GetFPS();
            static float GetDeltaTime();
            static float GetDeltaTimePeriodic();
            static DWORD GetFreeMemoryAmount();
            static DWORD GetUsedMemoryAmount();
            static DWORD GetTotalMemoryAmount();
	};
}