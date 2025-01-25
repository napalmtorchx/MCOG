#include "Engine/Options.h"
#include "Minecraft.h"

namespace Minecraft
{
	int  Options::render_distance;
	bool Options::lighting;
	bool Options::fog;
	bool Options::frustum_culling;
	bool Options::greedy_x;
	bool Options::greedy_y;
	bool Options::greedy_z;
	D3DXVECTOR2 Options::lookspd;

	void Options::Load()
	{

	}

	void Options::Save()
	{
		std::vector<std::string> lines;
		lines.push_back("FUCK YOU BUD!");
		lines.push_back("LMFAO");

		FileSystem::WriteLines(OPTIONS_INI_PATH, lines);
		RXDK_LOG_INFO("Saved configuration file\n");
	}
}