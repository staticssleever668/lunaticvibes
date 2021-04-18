#pragma once
#include "game/graphics/sprite.h"
#include "game/graphics/video.h"
#include "types.h"
#include <vector>
#include <map>
#include <string>
#include <memory>

enum class eSkinType
{
    LR2,

};

typedef unsigned long timeMS;

typedef StringContent Token;
typedef std::vector<StringContent> Tokens;

typedef std::shared_ptr<vSprite> pSprite;
typedef std::shared_ptr<Texture> pTexture;
typedef std::shared_ptr<sVideo>  pVideo;
typedef std::shared_ptr<Image>   pImage;
typedef std::shared_ptr<TTFFont> pFont;

inline std::array<pSprite, SPRITE_GLOBAL_MAX> gSprites{ nullptr };

class vSkin
{
public:

protected:
    vSkin();
public:
	virtual ~vSkin() = default;

protected:
    bool _loaded = false;
public:
    constexpr bool isLoaded() { return _loaded; }
    virtual int setExtendedProperty(std::string&& key, void* value) = 0;

////////////////////////////////////////////////////////////////////////////////
// Images
protected:
    std::map<std::string, pFont>  _fontNameMap;

////////////////////////////////////////////////////////////////////////////////
// Textures
protected:
    std::map<std::string, pTexture> _textureNameMap;    // Use this to get texture instance from name
	std::map<std::string, pVideo>   _vidNameMap;	// Use this to get video instance from name

    //TextureFull _texBlack;   // _textureNameMap["BlackDot"]
    //TextureFull _texWhite;   // _textureNameMap["WhiteDot"]
    //TextureFull _texError;   // _textureNameMap["Error"]

////////////////////////////////////////////////////////////////////////////////\
// Videos
public:
	const static size_t SKIN_VIDEO_SLOT_MAX = 32;
	std::array<sVideo, SKIN_VIDEO_SLOT_MAX> _video;

////////////////////////////////////////////////////////////////////////////////
// Texture Cache for resources require dynamic loading, such as BG, banner
protected:
    bool _enableTextureCache = false;
    std::map<Path, pTexture> _texDynPathMap;
public:
    void setDynamicLoadTexture(bool enabled);

////////////////////////////////////////////////////////////////////////////////
// Sprite elements
protected:
    std::list<pSprite> _sprites;                    // Push instance on parsing
    std::list<pSprite> _sprites_parent;             // sublist of _sprites, managed by impl
    std::list<pSprite> _sprites_child;              // sublist of _sprites, managed by impl

// functional support
protected:
    std::list<std::map<Rect, pSprite>> _mouseCursorAreaMap;

////////////////////////////////////////////////////////////////////////////////
public:
    virtual void update();
    virtual void update_mouse(int x, int y);
    virtual void update_mouse_click(int x, int y);
    virtual void draw() const;

    ///////////////////////////////////////////////////////////
    // Info defined by header
    struct skinInfo
    {
        eMode mode;
        std::string name;
        std::string maker;

        // General 
        unsigned timeIntro; // #STARTINPUT | before this time the scene cannot receive input. Implemented by derived class
        unsigned timeOutro; // #FADEOUT

        // Decide
        unsigned timeDecideSkip; // #SKIP | after this time the scene can be skipped. Solely for decide scene
        unsigned timeDecideExpiry; // #SCENETIME | switch to play scene if not skipped

        // Play
        unsigned timeStartLoading;
        unsigned timeMinimumLoad;
        unsigned timeGetReady;
        unsigned timeFailed;

        // Result
        unsigned timeResultRank; //ignored
        unsigned timeResultRecord; //ignored
    } info{};

    ///////////////////////////////////////////////////////////
};