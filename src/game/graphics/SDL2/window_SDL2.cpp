#ifdef RENDER_SDL2

#include "common/meta.h"
#include "window_SDL2.h"
#include "graphics_SDL2.h"
#include "SDL.h"
#include "SDL_Image.h"
#include "SDL_ttf.h"
#include "SDL_syswm.h"
#include "game/graphics/video.h"
#include "common/log.h"
#include <string>
#include "config/config_mgr.h"
#include "common/sysutil.h"

int graphics_init()
{
    // SDL2
    {
        if (SDL_Init(SDL_INIT_VIDEO))
        {
            LOG_ERROR << "[SDL2] Library init ERROR! " << SDL_GetError();
            return -99;
        }
        LOG_INFO << "[SDL2] Library version " << SDL_MAJOR_VERSION << '.' << SDL_MINOR_VERSION << "." << SDL_PATCHLEVEL;

        std::string title;
        title += MAIN_NAME;
        title += ' ';
        title += SUB_NAME;
        title += ' ';
        title += std::to_string(VER_MAJOR);
        title += '.';
        title += std::to_string(VER_MINOR);
        if (VER_PATCH)
        {
            title += '.';
            title += std::to_string(VER_PATCH);
        }

        /*
        for (int i = 0; i < SDL_GetNumRenderDrivers(); ++i)
        {
            SDL_RendererInfo rendererInfo = {};
            SDL_GetRenderDriverInfo(i, &rendererInfo);
            LOG_INFO << rendererInfo.name;
        }
        */
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

        Uint32 flags = SDL_WINDOW_OPENGL;
#ifdef _DEBUG
        flags |= SDL_WINDOW_RESIZABLE;
#endif
        auto mode = ConfigMgr::get("V", cfg::V_WINMODE, cfg::V_WINMODE_WINDOWED);
        if (strEqual(mode, cfg::V_WINMODE_BORDERLESS, true))
        {
            flags |= SDL_WINDOW_BORDERLESS;
        }
        else if (strEqual(mode, cfg::V_WINMODE_FULL, true))
        {
            flags |= SDL_WINDOW_FULLSCREEN;
        }
        else 
        {
            // fallback to windowed
        }
        unsigned w = ConfigMgr::get("V", cfg::V_RES_X, CANVAS_WIDTH);
        unsigned h = ConfigMgr::get("V", cfg::V_RES_Y, CANVAS_HEIGHT);
        gFrameWindow = SDL_CreateWindow(title.c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, flags);
        if (!gFrameWindow)
        {
            LOG_ERROR << "[SDL2] Init window ERROR! " << SDL_GetError();
            return -1;
        }

        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(gFrameWindow, &wmInfo);

#if _WIN32 || _WIN64
        setWindowHandle((void*)&wmInfo.info.win.window);
#endif

        auto vsync = ConfigMgr::get("V", cfg::V_VSYNC, cfg::OFF);
        if (vsync == cfg::ON)
        {
            gFrameRenderer = SDL_CreateRenderer(
                gFrameWindow, -1,
                SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
        }
        else
        {
            gFrameRenderer = SDL_CreateRenderer(
                gFrameWindow, -1,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
        }
        if (!gFrameRenderer)
        {
            LOG_ERROR << "[SDL2] Init renderer ERROR! " << SDL_GetError();
            return -2;
        }
        LOG_DEBUG << "[SDL2] Initializing window and render complete.";
    }

    // SDL_Image
    {
        auto flags = IMG_INIT_JPG | IMG_INIT_PNG;
        if (flags != IMG_Init(flags))
        {
            // error handling
            LOG_ERROR << "[SDL2] Image module init failed. " << IMG_GetError();
            return 1;
        }
        LOG_INFO << "[SDL2] Image module version " << SDL_IMAGE_MAJOR_VERSION << '.' << SDL_IMAGE_MINOR_VERSION << "." << SDL_IMAGE_PATCHLEVEL;
    }
    // SDL_ttf
    {
        if (-1 == TTF_Init())
        {
            // error handling
            LOG_ERROR << "[SDL2] TTF module init failed. " << TTF_GetError();
            return 2;
        }
        LOG_INFO << "[SDL2] TTF module version " << SDL_TTF_MAJOR_VERSION << '.' << SDL_TTF_MINOR_VERSION << "." << SDL_TTF_PATCHLEVEL;
    }

#ifndef VIDEO_DISABLED
	// libav
	video_init();
#endif

    return 0;
}

void graphics_clear()
{
    SDL_RenderClear(gFrameRenderer);
}

void graphics_flush()
{
    SDL_RenderPresent(gFrameRenderer);
}

int graphics_free()
{
    SDL_DestroyRenderer(gFrameRenderer);
    gFrameRenderer = nullptr;
    SDL_DestroyWindow(gFrameWindow);
    gFrameWindow = nullptr;
    TTF_Quit();
    IMG_Quit();
	SDL_Quit();

    return 0;
}

extern bool gEventQuit;
void event_handle()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            gEventQuit = true;
            break;

        case SDL_WINDOWEVENT:
            switch (e.window.event)
            {
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                SetWindowForeground(true);
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                SetWindowForeground(false);
                break;
            default:
                break;
            }
            break;

        default:
            break;
        }
    }
}

#endif