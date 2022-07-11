#include "menus/position_settings_menu.h"
#include <cstdio>
#include "utils/draw.h"
#include "fifo_queue.h"
#include "menus/settings_menu.h"
#include "libtp_c/include/SSystem/SComponent/c_counter.h"
#include "gz_flags.h"

#define LINE_NUM SPRITES_AMNT
#define NO_SELECTION ((uint8_t)-1)

#define INITAL_SPEED 1.0f
#define INCREMENT_FACTOR 1.05f
#define MAX_SPEED 20.f

Cursor PosSettingsMenu::cursor;

bool init_once = false;

float l_cursorSpeed;
uint8_t l_selItem = NO_SELECTION;

Vec2 g_spriteOffsets[SPRITES_AMNT];

Line lines[LINE_NUM] = {
    {"main menu", SpritesIndex::MENU_INDEX, "Change main menu position", false},
    {"input viewer", SpritesIndex::VIEWER_INDEX, "Change input viewer position", false},
    {"link debug info", SpritesIndex::DEBUG_INFO_INDEX, "Change link debug info position", false},
    {"timer", SpritesIndex::TIMER_SPR_INDEX, "Change timer position", false},
    {"load timer", SpritesIndex::LOAD_TIMER_SPR_INDEX, "Change load timer position", false},
    {"igt timer", SpritesIndex::IGT_TIMER_SPR_INDEX, "Change IGT timer position", false},
    {"fifo queue", SpritesIndex::FIFO_SPR_INDEX, "Change fifo queue position", false}};

void drawCursor(Vec2 pos) {
    bool cycle = (cCt_getFrameCount() / 8) % 2;
    if (GZ_checkDropShadows()) {
        Draw::drawRectOutline(DROP_SHADOWS_RGBA, {pos.x - 10 + 1, pos.y + 1}, {20, 0}, 0xA);
        Draw::drawRectOutline(DROP_SHADOWS_RGBA, {pos.x + 1, pos.y - 10 + 1}, {0, 20}, 0xA);
    }
    Draw::drawRectOutline(cycle ? g_cursorColor : 0xFFFFFFFF, {pos.x - 10, pos.y}, {20, 0}, 0xA);
    Draw::drawRectOutline(cycle ? g_cursorColor : 0xFFFFFFFF, {pos.x, pos.y - 10}, {0, 20}, 0xA);
}

void PosSettingsMenu::draw() {
    cursor.setMode(Cursor::MODE_UNRESTRICTED);

    if (GZ_getButtonTrig(BACK_BUTTON)) {
        if (l_selItem != NO_SELECTION) {
            l_selItem = NO_SELECTION;
        } else {
            init_once = false;
            GZ_setMenu(GZ_SETTINGS_MENU);
            return;
        }
    }

    if (!init_once) {
        l_selItem = NO_SELECTION;
        init_once = true;
    }

    if (GZ_getButtonTrig(SELECTION_BUTTON)) {
        if (cursor.y < SPRITES_AMNT) {
            l_selItem = l_selItem == cursor.y ? NO_SELECTION : cursor.y;
        }
    }

    if (l_selItem != NO_SELECTION && l_selItem < SPRITES_AMNT) {
        if (GZ_getButtonRepeat(GZPad::DPAD_RIGHT, 3)) {
            g_spriteOffsets[l_selItem].x += l_cursorSpeed;
        }
        if (GZ_getButtonRepeat(GZPad::DPAD_LEFT, 3)) {
            g_spriteOffsets[l_selItem].x -= l_cursorSpeed;
        }
        if (GZ_getButtonRepeat(GZPad::DPAD_UP, 3)) {
            g_spriteOffsets[l_selItem].y -= l_cursorSpeed;
        }
        if (GZ_getButtonRepeat(GZPad::DPAD_DOWN, 3)) {
            g_spriteOffsets[l_selItem].y += l_cursorSpeed;
        }

        // Draw visual cursor
        drawCursor(g_spriteOffsets[l_selItem]);
    }

    if (GZ_getButtonPressed(GZPad::DPAD_RIGHT) || GZ_getButtonPressed(GZPad::DPAD_LEFT) ||
        GZ_getButtonPressed(GZPad::DPAD_UP) || GZ_getButtonPressed(GZPad::DPAD_DOWN)) {
        if (l_cursorSpeed < MAX_SPEED) {
            l_cursorSpeed *= INCREMENT_FACTOR;
        }
        if (l_cursorSpeed > MAX_SPEED) {
            l_cursorSpeed = MAX_SPEED;
        }
    } else {
        l_cursorSpeed = INITAL_SPEED;
    }

    cursor.lock_x = l_selItem != NO_SELECTION;
    cursor.lock_y = l_selItem != NO_SELECTION;

    cursor.move(0, LINE_NUM);
    GZ_drawMenuLines(lines, cursor.y, LINE_NUM);
}

void PosSettingsMenu::initDefaults() {
    g_spriteOffsets[VIEWER_INDEX] = {220.f, 380.f};
    g_spriteOffsets[MENU_INDEX] = {25.f, 60.f};
    g_spriteOffsets[DEBUG_INFO_INDEX] = {450.0f, 200.f};
    g_spriteOffsets[TIMER_SPR_INDEX] = {525.0f, 420.f};
    g_spriteOffsets[LOAD_TIMER_SPR_INDEX] = {525.0f, 30.f};
    g_spriteOffsets[IGT_TIMER_SPR_INDEX] = {35.0f, 30.f};
    g_spriteOffsets[FIFO_SPR_INDEX] = {5.0f, 440.f};
}