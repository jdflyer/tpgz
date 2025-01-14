#include "menus/menu_settings/include/settings_menu.h"
#include <cstdio>
#include "utils/card.h"
#include "gz_flags.h"
#include "fifo_queue.h"
#include "rels/include/defines.h"
#include "menus/utils/menu_mgr.h"

#define MAX_RELOAD_OPTIONS 2
#define MAX_CURSOR_COLOR_OPTIONS 6

KEEP_FUNC SettingsMenu::SettingsMenu(Cursor& cursor)
    : Menu(cursor), lines{
                        {"area reload behavior:", AREA_RELOAD_BEHAVIOR_INDEX,
                         "Load area: reload last area | Load file = reload last file", false,
                         nullptr, MAX_RELOAD_OPTIONS},
                        {"cursor color:", CURSOR_COLOR_INDEX, "Change cursor color", false, nullptr,
                         MAX_CURSOR_COLOR_OPTIONS},
                        {"font:", FONT_INDEX, "Change font", false, nullptr, FONT_OPTIONS_COUNT},
                        {"drop shadows", DROP_SHADOWS_INDEX, "Adds shadows to all font letters",
                         true, &g_dropShadows},
                        {"swap equips", SWAP_EQUIPS_INDEX,
                         "Swap equips when loading practice files", true, &g_swap_equips_flag},
                        {"save card", SAVE_CARD_INDEX, "Save settings to memory card"},
                        {"load card", LOAD_CARD_INDEX, "Load settings from memory card"},
                        {"delete card", DELETE_CARD_INDEX, "Delete settings from memory card"},
                        {"menu positions", POS_SETTINGS_MENU_INDEX,
                         "Change menu object positions (A to toggle selection, DPad to move)",
                         false},
                    } {}

SettingsMenu::~SettingsMenu() {}

void SettingsMenu::draw() {
    cursor.setMode(Cursor::MODE_LIST);

    if (GZ_getButtonTrig(BACK_BUTTON)) {
        g_menuMgr->pop();
        return;
    }

    // static Storage storage;
    if (GZ_getButtonTrig(SELECTION_BUTTON)) {
        switch (cursor.y) {
        case DROP_SHADOWS_INDEX:
            g_dropShadows = !g_dropShadows;
            break;
        case POS_SETTINGS_MENU_INDEX:
            g_menuMgr->push(MN_POS_SETTINGS_INDEX);
            return;
        case SAVE_CARD_INDEX: {
            static Storage storage;
            storage.file_name = "tpgz01";
            storage.sector_size = SECTOR_SIZE;
            snprintf(storage.file_name_buffer, sizeof(storage.file_name_buffer), storage.file_name);
#ifndef WII_PLATFORM
            storage.result = CARDProbeEx(0, nullptr, &storage.sector_size);
            if (storage.result == Ready) {
                GZ_storeMemCard(storage);
            }
#else
            GZ_storeMemCard(storage);
#endif  // WII_PLATFORM
            break;
        }
        case LOAD_CARD_INDEX: {
            static Storage storage;
            storage.file_name = "tpgz01";
            storage.sector_size = SECTOR_SIZE;
            snprintf(storage.file_name_buffer, sizeof(storage.file_name_buffer), storage.file_name);
#ifndef WII_PLATFORM
            storage.result = CARDProbeEx(0, NULL, &storage.sector_size);
            if (storage.result == Ready) {
                GZ_loadMemCard(storage);
            }
#else
            GZ_loadMemCard(storage);
#endif  // WII_PLATFORM
            break;
        }
        case DELETE_CARD_INDEX: {
            static Storage storage;
            storage.file_name = "tpgz01";
            storage.sector_size = SECTOR_SIZE;
            snprintf(storage.file_name_buffer, sizeof(storage.file_name_buffer), storage.file_name);
#ifndef WII_PLATFORM
            storage.result = CARDProbeEx(0, nullptr, &storage.sector_size);
            if (storage.result == Ready) {
                GZ_deleteMemCard(storage);
            }
#else
            GZ_deleteMemCard(storage);
#endif  // WII_PLATFORM
            break;
        }
        case SWAP_EQUIPS_INDEX:
            g_swap_equips_flag = !g_swap_equips_flag;
            break;
        }
    }

    ListMember reload_opt[MAX_RELOAD_OPTIONS] = {"load area", "load file"};

    ListMember cursorCol_opt[MAX_CURSOR_COLOR_OPTIONS] = {"green",  "blue",   "red",
                                                          "orange", "yellow", "purple"};

    // handle list rendering
    switch (cursor.y) {
    case AREA_RELOAD_BEHAVIOR_INDEX:
        cursor.x = g_reloadType;
        cursor.move(MAX_RELOAD_OPTIONS, MENU_LINE_NUM);

        if (cursor.y == AREA_RELOAD_BEHAVIOR_INDEX) {
            g_reloadType = cursor.x;
        }
        break;
    case CURSOR_COLOR_INDEX:
        cursor.x = g_cursorColorType;
        cursor.move(MAX_CURSOR_COLOR_OPTIONS, MENU_LINE_NUM);

        if (cursor.y == CURSOR_COLOR_INDEX) {
            g_cursorColorType = cursor.x;
        }
        break;
    case FONT_INDEX: {
        cursor.x = g_fontType;
        uint32_t old_font = g_fontType;
        cursor.move(FONT_OPTIONS_COUNT, MENU_LINE_NUM);

        if (cursor.y == FONT_INDEX) {
            g_fontType = cursor.x;
        }
        if (old_font != g_fontType) {
            if (g_fontType >= 0 && g_fontType < FONT_OPTIONS_COUNT) {
                char buf[40];
                snprintf(buf, sizeof(buf), "tpgz/fonts/%s.fnt", g_font_opt[g_fontType].member);
                Font::loadFont(buf);
            }
        }
        break;
    }
    default:
        cursor.move(0, MENU_LINE_NUM);
        break;
    }

    lines[AREA_RELOAD_BEHAVIOR_INDEX].printf(" <%s>", reload_opt[g_reloadType].member);
    lines[CURSOR_COLOR_INDEX].printf(" <%s>", cursorCol_opt[g_cursorColorType].member);
    lines[FONT_INDEX].printf(" <%s>", g_font_opt[g_fontType].member);

    GZ_drawMenuLines(lines, cursor.y, MENU_LINE_NUM);
}
