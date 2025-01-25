#pragma once
#include "Common.h"

#define OPTIONS_INI_PATH "C:\\MCOG\\options.ini"

namespace Minecraft
{
	class Options
	{
		public:
			static int			render_distance;
			static bool			lighting;
			static bool			fog;
			static bool			frustum_culling;
			static bool			greedy_x;
			static bool			greedy_y;
			static bool			greedy_z;
			static D3DXVECTOR2  lookspd;

		public:
			static void Load();
			static void Save();
	};
}