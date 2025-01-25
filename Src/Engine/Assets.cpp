#include "Engine/Assets.h"
#include "Minecraft.h"

namespace Minecraft
{
    std::map<std::string, Texture> Assets::textures;
    TextureAtlas                   Assets::atlas;
    SpriteFont                     Assets::font;
    std::map<int, BlockMetadata>   Assets::blocks;

    void Assets::Init()
    {
        DWORD mem_before = App::GetUsedMemoryAmount();

        LoadTexture("Font", "D:\\Assets\\Textures\\UI\\Font.bmp", D3DCOLOR_ARGB(255, 0, 0, 0));
        font = SpriteFont(17, 35, -6, -16, 0, 0, &textures["Font"]);

        LoadTexture("Button", "D:\\Assets\\Textures\\UI\\Button.bmp", D3DCOLOR_ARGB(255, 255, 0, 255));
        LoadTexture("ButtonLarge", "D:\\Assets\\Textures\\UI\\ButtonLarge.bmp", D3DCOLOR_ARGB(255, 255, 0, 255));
        LoadTexture("Controls", "D:\\Assets\\Textures\\UI\\Controls.bmp", D3DCOLOR_ARGB(255, 255, 0, 255));
        LoadTexture("Crosshair", "D:\\Assets\\Textures\\UI\\Crosshair.bmp", D3DCOLOR_ARGB(255, 255, 0, 255));
        LoadTexture("SliderGrip", "D:\\Assets\\Textures\\UI\\SliderGrip.bmp", D3DCOLOR_ARGB(255, 255, 0, 255));
        LoadTexture("SwitchOff", "D:\\Assets\\Textures\\UI\\SwitchOff.bmp", D3DCOLOR_ARGB(255, 255, 0, 255));
        LoadTexture("SwitchOn", "D:\\Assets\\Textures\\UI\\SwitchOn.bmp", D3DCOLOR_ARGB(255, 255, 0, 255));

        LoadTexture("SkyboxTop", "D:\\Assets\\Textures\\SkyboxTop.bmp", D3DCOLOR_ARGB(0, 0, 0, 0));
        LoadTexture("SkyboxBottom", "D:\\Assets\\Textures\\SkyboxBottom.bmp", D3DCOLOR_ARGB(0, 0, 0, 0));
        LoadTexture("SkyboxLeft", "D:\\Assets\\Textures\\SkyboxLeft.bmp", D3DCOLOR_ARGB(0, 0, 0, 0));
        LoadTexture("SkyboxRight", "D:\\Assets\\Textures\\SkyboxRight.bmp", D3DCOLOR_ARGB(0, 0, 0, 0));
        LoadTexture("SkyboxBack", "D:\\Assets\\Textures\\SkyboxBack.bmp", D3DCOLOR_ARGB(0, 0, 0, 0));
        LoadTexture("SkyboxFront", "D:\\Assets\\Textures\\SkyboxFront.bmp", D3DCOLOR_ARGB(0, 0, 0, 0));

        LoadTexture("Pixel", "D:\\Assets\\Textures\\UI\\Pixel.bmp");
        LoadTexture("Logo", "D:\\Assets\\Textures\\UI\\Logo.png");

        atlas = TextureAtlas("D:\\Assets\\Textures\\Terrain.bmp", 16, 16, D3DCOLOR_ARGB(255, 255, 0, 255));

        LoadBlocks();

        DWORD mem_after = App::GetUsedMemoryAmount();
        RXDK_LOG_INFO("Initialized asset manager - Size:%uKB Textures:%d\n", (mem_after - mem_before) / 1024, textures.size());
    }

    void Assets::LoadBlocks()
    {
        IniParser blocks_ini;
        blocks_ini.Parse("D:\\Assets\\Data\\blocks.ini");

        std::map<std::string, IniSection>::iterator it = blocks_ini.sections.begin();
        while (it != blocks_ini.sections.end())
        {
            IniSection& section = blocks_ini.sections[it->first];
            if (!section.Contains("ID")) { RXDK_LOG_ERROR("Missing 'ID' value for block '%s'\n", it->first.c_str()); return; }

            BlockMetadata metadata;
            metadata.name = it->first;
            metadata.id = section.objects["ID"].ToInt();

            if (section.Contains("TEX_ALL"))
            {
                int tk = section.objects["TEX_ALL"].ToInt();
                for (int i = 0; i < 6; i++) { metadata.textures[i] = &atlas.textures[tk]; }
                RXDK_LOG_INFO("Loaded block - ID:%d Name:%s Textures:{ %d }\n", metadata.id, metadata.name.c_str(), tk);
            }
            else if (section.Contains("TEX_SIDES") && section.Contains("TEX_TOP") && section.Contains("TEX_BOTTOM"))
            {
                int tk_sides = section.objects["TEX_SIDES"].ToInt();
                int tk_top = section.objects["TEX_TOP"].ToInt();
                int tk_bottom = section.objects["TEX_BOTTOM"].ToInt();
                for (int i = 0; i < 4; i++) { metadata.textures[i] = &atlas.textures[tk_sides]; }
                metadata.textures[VOXELFACE_TOP] = &atlas.textures[tk_top];
                metadata.textures[VOXELFACE_BOTTOM] = &atlas.textures[tk_bottom];
                RXDK_LOG_INFO("Loaded block - ID:%d Name:%s Textures:{ %d, %d, %d }\n", metadata.id, metadata.name.c_str(), tk_sides, tk_top, tk_bottom);
            }
            else if (section.Contains("TEX_FRONT") && section.Contains("TEX_BACK") && section.Contains("TEX_LEFT") &&
                section.Contains("TEX_RIGHT") && section.Contains("TEX_TOP") && section.Contains("TEX_BOTTOM"))
            {
                int tk_front = section.objects["TEX_FRONT"].ToInt();
                int tk_right = section.objects["TEX_RIGHT"].ToInt();
                int tk_back = section.objects["TEX_BACK"].ToInt();
                int tk_left = section.objects["TEX_LEFT"].ToInt();
                int tk_top = section.objects["TEX_TOP"].ToInt();
                int tk_bottom = section.objects["TEX_BOTTOM"].ToInt();
                metadata.textures[VOXELFACE_FRONT] = &atlas.textures[tk_front];
                metadata.textures[VOXELFACE_RIGHT] = &atlas.textures[tk_right];
                metadata.textures[VOXELFACE_BACK] = &atlas.textures[tk_back];
                metadata.textures[VOXELFACE_LEFT] = &atlas.textures[tk_left];
                metadata.textures[VOXELFACE_TOP] = &atlas.textures[tk_top];
                metadata.textures[VOXELFACE_BOTTOM] = &atlas.textures[tk_bottom];
                RXDK_LOG_INFO("Loaded block - ID:%d Name:%s Textures:{ %d, %d, %d, %d, %d, %d }\n", metadata.id, metadata.name.c_str(), tk_front, tk_right, tk_back, tk_left, tk_top, tk_bottom);
            }
            else if (it->first != "Air") { RXDK_LOG_ERROR("Missing texture data for block '%s'", it->first.c_str()); return; }

            blocks[metadata.id] = metadata;

            it++;
        }

        blocks_ini.Dispose();
        RXDK_LOG_INFO("Finished loading block metadata - Blocks:%u\n", blocks.size());
    }
    
    void Assets::LoadTexture(const std::string& name, LPCSTR fname, D3DXCOLOR key)
    {
        if (textures.find(name) != textures.end()) { RXDK_LOG_ERROR("Texture with name '%s' already loaded", name); return; }
        textures[name] = Texture(fname, key);
        RXDK_LOG_INFO("Loaded texture - Size:%dx%d Name:%s File:'%s'\n", (int)textures[name].Size().x, (int)textures[name].Size().y, name.c_str(), fname);
    }
}