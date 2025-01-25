#include "Application.h"
#include "Minecraft.h"

namespace Minecraft
{
    char  App::_dbgtxt[4096];
    char  App::_lnbuff[80];
    int   App::_fps, App::_frames;
    float App::_dt, App::_dtps;
    float App::_sectimer, App::_uptimer;
    DWORD App::_memused, App::_memavail, App::_memtotal;

    void App::Init()
    {
        GraphicsManager::Init();
        Assets::Init();
        Canvas::Init();
        States::Init();
        Input::Init();

        Options::Load();

        Texture* textures[4] = { &Assets::textures["SkyboxLeft"], &Assets::textures["SkyboxFront"], &Assets::textures["SkyboxRight"], &Assets::textures["SkyboxBack"] };
        Renderer::InitSkybox(textures, &Assets::textures["SkyboxTop"], &Assets::textures["SkyboxBottom"]);

        RXDK_LOG_INFO("Finished initializing system\n");
    }

    void App::Dispose()
    {
        GraphicsManager::Dispose();

        RXDK_LOG_INFO("Finished disposing application resources\n");
    }

    void App::Run()
    {
        DWORD prevTime = timeGetTime();
        RXDK_LOG_INFO("Entered main loop\n");

        while (true)
        {
            // Calculate time between frames
            DWORD currentTime = timeGetTime();
            _dt = (currentTime - prevTime) * 0.001f;
            prevTime = currentTime;

            // On each second passed
            _sectimer += _dt;
            if (_sectimer >= 1.0f)
            {
                _sectimer = 0;
                _fps = _frames;
                _dtps = _dt;
                _frames = 0;
            }

            _uptimer += _dt;
            if (_uptimer >= (float)UPDATE_FREQUENCY)
            {
                _uptimer = 0;
                UpdateFixed();
            }

            UpdateDebugText();
            Update();
            Draw();
            _frames++;
        }
    }

    void App::Update()
    {
        Input::Update(_dt);
        States::Update(_dt);
    }

    void App::UpdateFixed()
    {
       States::UpdateFixed();
    }

    void App::UpdateDebugText()
    {
        float percent = ((float)_memused / (float)_memtotal) * 100.0f;

        memset(_dbgtxt, 0, sizeof(_dbgtxt));

        _snprintf(_lnbuff, sizeof(_lnbuff), "MCOGXB\n");
        strcat(_dbgtxt, _lnbuff);

        _snprintf(_lnbuff, sizeof(_lnbuff), "%d FPS @ %d ms\n", _fps, (int)(_dtps * 1000.0f));
        strcat(_dbgtxt, _lnbuff);

        _snprintf(_lnbuff, sizeof(_lnbuff), "RAM: %.0f%% %u/%u BYTES\n", percent, GetUsedMemoryAmount(), GetTotalMemoryAmount());
        strcat(_dbgtxt, _lnbuff);

        if (GameState::instance != NULL && States::GetActive() == GameState::instance)
        {
            _snprintf(_lnbuff, sizeof(_lnbuff), "POS: %.0f, %.0f, %.0f\n", GameState::instance->player.pos.x, GameState::instance->player.pos.y, GameState::instance->player.pos.z);
            strcat(_dbgtxt, _lnbuff);

            _snprintf(_lnbuff, sizeof(_lnbuff), "FACES: %u/%u\n", GameState::instance->world->GetRenderedFaceCount(), GameState::instance->world->GetTotalFaceCount());
            strcat(_dbgtxt, _lnbuff);
        }

    }

    void App::Draw()
    {   
        States::Draw(_dt);
        States::DrawUI(_dt);
        GraphicsManager::Present();
    }

    LPCSTR App::GetDebugText() { return _dbgtxt; }

    int App::GetFPS() { return _fps; }

    float App::GetDeltaTime() { return _dt; }

    float App::GetDeltaTimePeriodic() { return _dtps; }

    DWORD App::GetFreeMemoryAmount()
    {
        MEMORYSTATUS stat;
        GlobalMemoryStatus(&stat);
        _memavail = stat.dwAvailPhys;
        return _memavail;
    }

    DWORD App::GetUsedMemoryAmount()
    {
        MEMORYSTATUS stat;
        GlobalMemoryStatus(&stat);
        _memused = stat.dwTotalPhys - stat.dwAvailPhys;
        return _memused;
    }

    DWORD App::GetTotalMemoryAmount()
    {
        MEMORYSTATUS stat;
        GlobalMemoryStatus(&stat);
        _memtotal = stat.dwTotalPhys;
        return _memtotal;
    }
}