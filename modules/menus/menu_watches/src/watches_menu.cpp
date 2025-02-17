#include "menus/menu_watches/include/watches_menu.h"
#include <cstdio>
#include "watches.h"
#include "utils/lines.h"
#include "memory_editor.h"
#include "gz_flags.h"
#include "rels/include/defines.h"
#include "menus/utils/menu_mgr.h"

#define WATCH_COLUMNS 6
#define WATCH_ADDRESS_X_OFFSET 25.0f
#define WHITE_RGBA 0xFFFFFFFF
#define SPEED_THRESHOLD 30

#ifdef GCN_PLATFORM
#define MEMORY_BUTTON (GZPad::Y)
#define MEMORY_TEXT "Y"
#endif
#ifdef WII_PLATFORM
#define MEMORY_BUTTON (GZPad::ONE)
#define MEMORY_TEXT "1"
#endif

WatchesMenu::WatchesMenu(Cursor& cursor, WatchesData& data)
    : Menu(cursor), button_held_counter(data.button_held_counter),
      l_scrollSpeed(data.l_scrollSpeed), l_addrIdx(data.l_addrIdx), l_offsetIdx(data.l_offsetIdx) {}

WatchesMenu::~WatchesMenu() {}

void WatchesMenu::drawMemoryLines() {
    const float watch_x_pos_x_offset =
        WATCH_ADDRESS_X_OFFSET + maxF(GZ_getTextWidth("Address"), GZ_getTextWidth("0x80000000")) +
        5.0f;
    const float watch_y_pos_x_offset =
        watch_x_pos_x_offset + maxF(GZ_getTextWidth("X"), GZ_getTextWidth("<000>")) + 5.0f;
    const float watch_hex_x_offset =
        watch_y_pos_x_offset + maxF(GZ_getTextWidth("Y"), GZ_getTextWidth("<000>")) + 5.0f;
    const float watch_type_x_offset =
        watch_hex_x_offset + maxF(GZ_getTextWidth("Hex"), GZ_getTextWidth("<false>")) + 5.0f;
    const float watch_offset_x_offset =
        watch_type_x_offset + maxF(GZ_getTextWidth("Type"), GZ_getTextWidth("<u32>")) + 5.0f;
    const float watch_visible_x_offset =
        watch_offset_x_offset + maxF(GZ_getTextWidth("Offset"), GZ_getTextWidth("0x0000")) + 5.0f;

    GZ_drawText("Address", WATCH_ADDRESS_X_OFFSET, 60.0f, WHITE_RGBA, GZ_checkDropShadows());
    GZ_drawText("X", watch_x_pos_x_offset, 60.0f, WHITE_RGBA, GZ_checkDropShadows());
    GZ_drawText("Y", watch_y_pos_x_offset, 60.0f, WHITE_RGBA, GZ_checkDropShadows());
    GZ_drawText("Hex", watch_hex_x_offset, 60.0f, WHITE_RGBA, GZ_checkDropShadows());
    GZ_drawText("Type", watch_type_x_offset, 60.0f, WHITE_RGBA, GZ_checkDropShadows());
    GZ_drawText("Offset", watch_offset_x_offset, 60.0f, WHITE_RGBA, GZ_checkDropShadows());
    GZ_drawText("Visible", watch_visible_x_offset, 60.0f, WHITE_RGBA, GZ_checkDropShadows());

    for (int i = 0; i < MAX_WATCHES; i++) {
        const float line_y_offset = (80.0f + (i * 20.0f));
        char watch_address[11];
        char watch_x[8];
        char watch_y[8];
        char watch_hex[8];
        char watch_type[6];
        char watch_offset[7];
        char watch_visible[4];

        snprintf(watch_address, sizeof(watch_address), "0x%08X", g_watches[i].address);
        snprintf(watch_x, sizeof(watch_x), "%.0f", g_watches[i].x);
        snprintf(watch_y, sizeof(watch_y), "%.0f", g_watches[i].y);
        snprintf(watch_hex, sizeof(watch_hex), "%s", g_watches[i].hex ? "true" : "false");

        switch (g_watches[i].type) {
        case MEM_TYPE_U8:
            snprintf(watch_type, sizeof(watch_type), "u8");
            break;
        case MEM_TYPE_U16:
            snprintf(watch_type, sizeof(watch_type), "u16");
            break;
        case MEM_TYPE_U32:
            snprintf(watch_type, sizeof(watch_type), "u32");
            break;
        case MEM_TYPE_S8:
            snprintf(watch_type, sizeof(watch_type), "s8");
            break;
        case MEM_TYPE_S16:
            snprintf(watch_type, sizeof(watch_type), "s16");
            break;
        case MEM_TYPE_S32:
            snprintf(watch_type, sizeof(watch_type), "s32");
            break;
        case MEM_TYPE_F32:
            snprintf(watch_type, sizeof(watch_type), "f32");
            break;
        case MEM_TYPE_STR:
            snprintf(watch_type, sizeof(watch_type), "str");
            break;
        }
        snprintf(watch_offset, sizeof(watch_offset), "0x%04X", g_watches[i].offset);
        snprintf(watch_visible, sizeof(watch_visible), "%s", g_watches[i].visible ? "[X]" : "[ ]");

        if (g_watches[i].line_selected) {
            switch (cursor.x) {
            case WatchAddress:
                if (g_watches[i].value_selected) {
                    if (GZ_getButtonRepeat(GZPad::DPAD_RIGHT)) {
                        if (l_addrIdx == 9) {
                            l_addrIdx = 3;
                        } else if (l_addrIdx >= 3 && l_addrIdx < 9) {
                            l_addrIdx++;
                        }
                    }
                    if (GZ_getButtonRepeat(GZPad::DPAD_LEFT)) {
                        if (l_addrIdx == 3) {
                            l_addrIdx = 9;
                        } else if (l_addrIdx > 3 && l_addrIdx <= 9) {
                            l_addrIdx--;
                        }
                    }
                    if (GZ_getButtonRepeat(GZPad::DPAD_UP)) {
                        if (l_addrIdx == 3 && g_watches[i].address >= 0x81000000) {
                            g_watches[i].address = 0x817FFFFF;
                        } else if (l_addrIdx <= 9 && l_addrIdx >= 3) {
                            g_watches[i].address += 1 << ((9 - l_addrIdx) * 4);
                        }
                        if (g_watches[i].address > 0x817FFFFF) {
                            g_watches[i].address = 0x817FFFFF;
                        }
                    }
                    if (GZ_getButtonRepeat(GZPad::DPAD_DOWN)) {
                        if (l_addrIdx <= 9 && l_addrIdx >= 3) {
                            g_watches[i].address -= 1 << ((9 - l_addrIdx) * 4);
                        }
                        if (g_watches[i].address < 0x80000000) {
                            g_watches[i].address = 0x80000000;
                        }
                    }

                    GZ_drawSelectChar(watch_address, WATCH_ADDRESS_X_OFFSET, line_y_offset,
                                      l_addrIdx, 9, WHITE_RGBA);
                } else {
                    GZ_drawText(watch_address, WATCH_ADDRESS_X_OFFSET, line_y_offset, CURSOR_RGBA,
                                GZ_checkDropShadows());
                }
                GZ_drawText(watch_x, watch_x_pos_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_y, watch_y_pos_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_hex, watch_hex_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_type, watch_type_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_offset, watch_offset_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_visible, watch_visible_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                break;
            case WatchX:
                if (g_watches[i].value_selected) {
                    if (GZ_getButtonRepeat(GZPad::DPAD_RIGHT)) {
                        if (g_watches[i].x >= 0.0f && g_watches[i].x < 600.0f) {
                            g_watches[i].x += l_scrollSpeed;
                        }
                    }
                    if (GZ_getButtonRepeat(GZPad::DPAD_LEFT)) {
                        if (g_watches[i].x > 0.0f && g_watches[i].x <= 600.0f) {
                            g_watches[i].x -= l_scrollSpeed;
                        }
                    }
                    if (g_watches[i].x < 0) {
                        g_watches[i].x = 0;
                    }
                    if (g_watches[i].x > 600) {
                        g_watches[i].x = 600;
                    }
                    snprintf(watch_x, sizeof(watch_x), "<%.0f>", g_watches[i].x);
                    GZ_drawText(watch_x, watch_x_pos_x_offset - 8.0f, line_y_offset, CURSOR_RGBA,
                                GZ_checkDropShadows());
                } else {
                    GZ_drawText(watch_x, watch_x_pos_x_offset, line_y_offset, CURSOR_RGBA,
                                GZ_checkDropShadows());
                }
                GZ_drawText(watch_address, WATCH_ADDRESS_X_OFFSET, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_y, watch_y_pos_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_hex, watch_hex_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_type, watch_type_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_offset, watch_offset_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_visible, watch_visible_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                break;
            case WatchY:
                if (g_watches[i].value_selected) {
                    if (GZ_getButtonRepeat(GZPad::DPAD_RIGHT)) {
                        if (g_watches[i].y >= 0.0f && g_watches[i].y < 500.0f) {
                            g_watches[i].y += l_scrollSpeed;
                        }
                    }
                    if (GZ_getButtonRepeat(GZPad::DPAD_LEFT)) {
                        if (g_watches[i].y > 0.0f && g_watches[i].y <= 500.0f) {
                            g_watches[i].y -= l_scrollSpeed;
                        }
                    }
                    if (g_watches[i].y < 0) {
                        g_watches[i].y = 0;
                    }
                    if (g_watches[i].y > 500) {
                        g_watches[i].y = 500;
                    }
                    snprintf(watch_y, sizeof(watch_y), "<%.0f>", g_watches[i].y);
                    GZ_drawText(watch_y, watch_y_pos_x_offset - 8.0f, line_y_offset, CURSOR_RGBA,
                                GZ_checkDropShadows());
                } else {
                    GZ_drawText(watch_y, watch_y_pos_x_offset, line_y_offset, CURSOR_RGBA,
                                GZ_checkDropShadows());
                }
                GZ_drawText(watch_address, WATCH_ADDRESS_X_OFFSET, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_x, watch_x_pos_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_hex, watch_hex_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_type, watch_type_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_offset, watch_offset_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_visible, watch_visible_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());

                break;
            case WatchHex:
                if (g_watches[i].value_selected) {
                    if (GZ_getButtonRepeat(GZPad::DPAD_RIGHT)) {
                        g_watches[i].hex = !g_watches[i].hex;
                    }
                    if (GZ_getButtonRepeat(GZPad::DPAD_LEFT)) {
                        g_watches[i].hex = !g_watches[i].hex;
                    }
                    snprintf(watch_hex, sizeof(watch_hex), "<%s>",
                             g_watches[i].hex ? "true" : "false");
                    GZ_drawText(watch_hex, watch_hex_x_offset - 8.0f, line_y_offset, CURSOR_RGBA,
                                GZ_checkDropShadows());
                } else {
                    GZ_drawText(watch_hex, watch_hex_x_offset, line_y_offset, CURSOR_RGBA,
                                GZ_checkDropShadows());
                }
                GZ_drawText(watch_address, WATCH_ADDRESS_X_OFFSET, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_x, watch_x_pos_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_y, watch_y_pos_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_type, watch_type_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_offset, watch_offset_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_visible, watch_visible_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                break;
            case WatchType:
                if (g_watches[i].value_selected) {
                    if (GZ_getButtonRepeat(GZPad::DPAD_RIGHT)) {
                        if (g_watches[i].type == MEM_TYPE_STR) {
                            g_watches[i].type = MEM_TYPE_U8;
                        } else if (g_watches[i].type >= MEM_TYPE_U8 && g_watches[i].type < MEM_TYPE_STR) {
                            g_watches[i].type++;
                        }
                    }
                    if (GZ_getButtonRepeat(GZPad::DPAD_LEFT)) {
                        if (g_watches[i].type == MEM_TYPE_U8) {
                            g_watches[i].type = MEM_TYPE_STR;
                        } else if (g_watches[i].type > MEM_TYPE_U8 && g_watches[i].type <= MEM_TYPE_STR) {
                            g_watches[i].type--;
                        }
                    }
                    switch (g_watches[i].type) {
                    case MEM_TYPE_U8:
                        snprintf(watch_type, sizeof(watch_type), "<u8>");
                        break;
                    case MEM_TYPE_U16:
                        snprintf(watch_type, sizeof(watch_type), "<u16>");
                        break;
                    case MEM_TYPE_U32:
                        snprintf(watch_type, sizeof(watch_type), "<u32>");
                        break;
                    case MEM_TYPE_S8:
                        snprintf(watch_type, sizeof(watch_type), "<s8>");
                        break;
                    case MEM_TYPE_S16:
                        snprintf(watch_type, sizeof(watch_type), "<s16>");
                        break;
                    case MEM_TYPE_S32:
                        snprintf(watch_type, sizeof(watch_type), "<s32>");
                        break;
                    case MEM_TYPE_F32:
                        snprintf(watch_type, sizeof(watch_type), "<f32>");
                        break;
                    case MEM_TYPE_STR:
                        snprintf(watch_type, sizeof(watch_type), "<str>");
                    }
                    GZ_drawText(watch_type, watch_type_x_offset - 8.0f, line_y_offset, CURSOR_RGBA,
                                GZ_checkDropShadows());
                } else {
                    GZ_drawText(watch_type, watch_type_x_offset, line_y_offset, CURSOR_RGBA,
                                GZ_checkDropShadows());
                }
                GZ_drawText(watch_address, WATCH_ADDRESS_X_OFFSET, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_x, watch_x_pos_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_y, watch_y_pos_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_hex, watch_hex_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_offset, watch_offset_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_visible, watch_visible_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                break;
            case WatchOffset:
                if (g_watches[i].value_selected) {
                    if (GZ_getButtonRepeat(GZPad::DPAD_RIGHT)) {
                        if (l_offsetIdx == 5) {
                            l_offsetIdx = 2;
                        } else if (l_offsetIdx >= 2 && l_offsetIdx < 5) {
                            l_offsetIdx++;
                        }
                    }
                    if (GZ_getButtonRepeat(GZPad::DPAD_LEFT)) {
                        if (l_offsetIdx == 2) {
                            l_offsetIdx = 5;
                        } else if (l_offsetIdx > 2 && l_offsetIdx <= 5) {
                            l_offsetIdx--;
                        }
                    }
                    if (GZ_getButtonRepeat(GZPad::DPAD_UP)) {
                        g_watches[i].offset += (0x100000 >> (l_offsetIdx * 4));
                        if (g_watches[i].offset > 0xFFFF) {
                            g_watches[i].offset = 0xFFFF;
                        }
                    }
                    if (GZ_getButtonRepeat(GZPad::DPAD_DOWN)) {
                        g_watches[i].offset -= (0x100000 >> (l_offsetIdx * 4));
                        if (g_watches[i].offset < 0x0000) {
                            g_watches[i].offset = 0x0000;
                        }
                    }
                    GZ_drawSelectChar(watch_offset, watch_offset_x_offset, line_y_offset,
                                      l_offsetIdx, 5, WHITE_RGBA);
                } else {
                    GZ_drawText(watch_offset, watch_offset_x_offset, line_y_offset, CURSOR_RGBA,
                                GZ_checkDropShadows());
                }
                GZ_drawText(watch_address, WATCH_ADDRESS_X_OFFSET, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_x, watch_x_pos_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_y, watch_y_pos_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_hex, watch_hex_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_type, watch_type_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                GZ_drawText(watch_visible, watch_visible_x_offset, line_y_offset, WHITE_RGBA,
                            GZ_checkDropShadows());
                break;
            }
        } else {
            int y = cursor.y;
            GZ_drawText(watch_address, WATCH_ADDRESS_X_OFFSET, line_y_offset,
                        (y == i ? CURSOR_RGBA : WHITE_RGBA), GZ_checkDropShadows());
            GZ_drawText(watch_x, watch_x_pos_x_offset, line_y_offset,
                        (y == i ? CURSOR_RGBA : WHITE_RGBA), GZ_checkDropShadows());
            GZ_drawText(watch_y, watch_y_pos_x_offset, line_y_offset,
                        (y == i ? CURSOR_RGBA : WHITE_RGBA), GZ_checkDropShadows());
            GZ_drawText(watch_hex, watch_hex_x_offset, line_y_offset,
                        (y == i ? CURSOR_RGBA : WHITE_RGBA), GZ_checkDropShadows());
            GZ_drawText(watch_type, watch_type_x_offset, line_y_offset,
                        (y == i ? CURSOR_RGBA : WHITE_RGBA), GZ_checkDropShadows());
            GZ_drawText(watch_offset, watch_offset_x_offset, line_y_offset,
                        (y == i ? CURSOR_RGBA : WHITE_RGBA), GZ_checkDropShadows());
            GZ_drawText(watch_visible, watch_visible_x_offset, line_y_offset,
                        (y == i ? CURSOR_RGBA : WHITE_RGBA), GZ_checkDropShadows());
        }
    }
}

bool checkMemLineSelected() {
    bool return_value = false;
    for (int i = 0; i < MAX_WATCHES; i++) {
        if (g_watches[i].line_selected) {
            return_value = true;
        }
    }
    return return_value;
}

bool checkLineValSelected() {
    bool return_value = false;
    for (int i = 0; i < MAX_WATCHES; i++) {
        if (g_watches[i].value_selected) {
            return_value = true;
        }
    }

    return return_value;
}

void WatchesMenu::draw() {
    cursor.setMode(Cursor::MODE_UNRESTRICTED);

    if (GZ_getButtonTrig(BACK_BUTTON)) {
        if (checkLineValSelected()) {
            for (int i = 0; i < MAX_WATCHES; i++) {
                g_watches[i].value_selected = false;
            }
            g_watches[cursor.y].value_selected = false;
            cursor.lock_x = false;
        } else if (checkMemLineSelected()) {
            for (int i = 0; i < MAX_WATCHES; i++) {
                g_watches[i].line_selected = false;
            }
            g_watches[cursor.y].line_selected = false;
            cursor.lock_y = false;
        } else {
            g_menuMgr->pop();
            return;
        }
    }

    if (GZ_getButtonTrig(SELECTION_BUTTON)) {
        if (g_watches[cursor.y].value_selected) {
            g_watches[cursor.y].value_selected = false;
            cursor.lock_x = false;
        } else if (g_watches[cursor.y].line_selected) {
            g_watches[cursor.y].value_selected = true;
            cursor.lock_x = true;
        } else {
            g_watches[cursor.y].line_selected = true;
            cursor.lock_y = true;
        }
    }

    if (GZ_getButtonTrig(GZPad::Z)) {
        g_watches[cursor.y].visible = !g_watches[cursor.y].visible;
    }

    if (GZ_getButtonTrig(MEMORY_BUTTON)) {
        if (g_watches[cursor.y].offset > 0 && *(uint32_t*)g_watches[cursor.y].address != 0) {
            g_memoryEditor_addressIndex =
                *(uint32_t*)g_watches[cursor.y].address + g_watches[cursor.y].offset;
        } else {
            g_memoryEditor_addressIndex = g_watches[cursor.y].address;
        }
        g_menuMgr->push(MN_MEMORY_EDITOR_INDEX);
    }

    if (GZ_getButtonPressed(GZPad::DPAD_RIGHT) || GZ_getButtonPressed(GZPad::DPAD_LEFT)) {
        if (l_scrollSpeed < 20.0f) {
            l_scrollSpeed += 1.0f;
        }
        if (l_scrollSpeed > 20.0f) {
            l_scrollSpeed = 20.0f;
        }
    } else {
        l_scrollSpeed = 1.0f;
    }

    cursor.move(WATCH_COLUMNS, MAX_WATCHES);
    GZ_drawText("Press Z to enable/disable watch. " MEMORY_TEXT " to jump to editor address", 25.0f,
                440.f, WHITE_RGBA, GZ_checkDropShadows());
    drawMemoryLines();
}