#include "skin.h"
#include "game/graphics/sprite_lane.h"
#include "game/scene/scene_context.h"
#include <execution>
#include <algorithm>

vSkin::vSkin()
{
    _texNameMap["Black"] = std::make_shared<TextureFull>(0x000000ff);
    _texNameMap["White"] = std::make_shared<TextureFull>(0xffffffff);
    _texNameMap["Error"] = std::make_shared<TextureFull>(0xff00ffff);
}

void vSkin::update()
{
	timestamp t;
#ifdef _DEBUG
	for(const auto& s: _sprites)
#else
	std::for_each(std::execution::par_unseq, _sprites.begin(), _sprites.end(), [&t](const auto& s)
#endif
	{
		switch (s->type())
		{
        case SpriteTypes::GLOBAL:
        {
            auto& ref = (std::shared_ptr<SpriteGlobal>&)s;
            if (gSprites[ref->get()]) ref->set(gSprites[ref->get()]);
            ref->update(t);
            break;
        }
		case SpriteTypes::NOTE_VERT:
		{
			auto& ref = (std::shared_ptr<SpriteLaneVertical>&)s;
			if (ref->haveDst() && context_chart.scrollObj != nullptr && context_chart.started)
			{
				ref->update(t);
				ref->updateNoteRect(t, &*context_chart.scrollObj);
			}
			break;
		}
		default:
			s->update(t);
			break;
		}
	}
#ifndef _DEBUG
    );
#endif
}

void vSkin::draw() const
{
    for (auto& s : _sprites)
        s->draw();
}