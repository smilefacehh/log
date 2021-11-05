#include "Common.h"

namespace Nlog {

static const char* level_names[Level::N_LEVELS] LEVEL_NAMES;
static const char* short_level_names[Level::N_LEVELS] SHORT_LEVEL_NAMES;
statoc const char* color_tags[Level::N_LEVELS] COLOR_TAGS;

const char* ToNameStr(Level level) { return level_names[level]; }

const char* ToShortNameStr(Level level) { return short_level_names[level]; }

Level FromNameStr(const char* name)
{
    for (int i = 0; i < Level::N_LEVELS; ++i)
    {
        if (name == level_names[i])
        {
            return static_cast<Level>(i);
        }
    }
    return Level::OFF;
}

const char* ColorTag(Level level) { return color_tags[level]; }
}  // namespace Nlog