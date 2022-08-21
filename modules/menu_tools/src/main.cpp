#include <main.h>
#include "menu_tools/include/tools_menu.h"
#include "handlers/draw_handler.h"
#include "menus/utils/menu_mgr.h"
#include "utils/draw.h"

void onCreate();
void onLoad();
void onDraw();
void onUnload();
void onDelete();

ToolsMenu* l_toolsMenu;

namespace tpgz::modules {
void main() {
    g_menuMgr->setCreateHook(onCreate);
    g_menuMgr->setLoadHook(onLoad);
    g_menuMgr->setUnloadHook(onUnload);
    g_menuMgr->setDeleteHook(onDelete);
}
void exit() {
    g_menuMgr->setCreateHook(nullptr);
    g_menuMgr->setLoadHook(nullptr);
    g_menuMgr->setUnloadHook(nullptr);
    g_menuMgr->setDeleteHook(nullptr);
}
}  // namespace tpgz::modules

void onCreate() {
    g_menuMgr->setPersistentData(new Cursor());
}

void onLoad() {
    l_toolsMenu = new ToolsMenu(*(Cursor*)g_menuMgr->getPersistentData());
    g_drawHandler->addHandler(onDraw);
}

void onDraw() {
    l_toolsMenu->draw();
}

void onUnload() {
    g_drawHandler->removeHandler(onDraw);
    delete l_toolsMenu;
}

void onDelete() {
    auto data = g_menuMgr->getPersistentData();
    delete (Cursor*)data;
    g_menuMgr->setPersistentData(nullptr);
}
