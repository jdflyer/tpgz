#pragma once

#include "menu.h"
#include "cheats.h"

class CheatsMenu : public Menu {
public:
    CheatsMenu(Cursor&);
    virtual ~CheatsMenu();
    virtual void draw();

private:
    Line lines[CHEAT_AMNT];
};