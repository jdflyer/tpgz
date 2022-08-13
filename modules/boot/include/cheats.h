#ifndef LIB_TP_CHEATS
#define LIB_TP_CHEATS

#ifdef WII_PLATFORM
#define CHEAT_AMNT 15
#else
#define CHEAT_AMNT 14
#endif

enum CheatId {
    InfiniteAir,
    InfiniteArrows,
    InfiniteBombs,
    InfiniteHearts,
    InfiniteOil,
    InfiniteRupees,
    InfiniteSlingshot,
    Invincible,
    InvincibleEnemies,
    MoonJump,
    DoorStorage,
    SuperClawshot,
    UnrestrictedItems,
    TransformAnywhere,
#ifdef WII_PLATFORM
    GaleLJA,
#endif
    SuperSpinner,
};

struct Cheat {
    enum CheatId id;
    bool active;
};

void GZ_applyCheats();

extern Cheat g_cheats[CHEAT_AMNT];

#endif