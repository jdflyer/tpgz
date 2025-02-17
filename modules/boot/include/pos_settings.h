#pragma once

#include "libtp_c/include/dolphin/mtx/vec.h"

#define SPRITES_AMNT 8
enum SpritesIndex {
    MENU_INDEX,
    VIEWER_INDEX,
    DEBUG_INFO_INDEX,
    TIMER_SPR_INDEX,
    LOAD_TIMER_SPR_INDEX,
    IGT_TIMER_SPR_INDEX,
    FIFO_SPR_INDEX,
    HEAP_INFO_INDEX
};

extern Vec2 g_spriteOffsets[SPRITES_AMNT];

void GZ_PosSettings_initDefaults();
