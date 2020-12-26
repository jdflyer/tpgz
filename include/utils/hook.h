
#define HOOK_AMNT 9
enum HookIndex {
    HK_LIB_INIT_INDEX = 0,
    HK_LIB_GAME_LOOP_INDEX = 1,
    HK_LIB_DRAW_INDEX = 2,
    HK_LIB_READ_CONTROLLER_INDEX = 3,
    HK_SUPER_CLAWSHOT_INDEX = 4,
    HK_DISABLE_GRAV_INDEX = 5,
    HK_UNRESTRICTED_ITEMS_INDEX = 6,
    HK_TRANSFORM_ANYWHERE_INDEX = 7,
    HK_INVINCIBLE_ENEMIES_INDEX = 8,
};

namespace Hook {
void apply_hooks();
}  // namespace Hook