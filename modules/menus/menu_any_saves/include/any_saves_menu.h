#include "menu.h"

#ifdef GCN_PLATFORM
#define ANY_SPECIALS_AMNT 19

enum AnyPracticeIndex {
    ORDON_GATE_CLIP_INDEX,
    BACK_IN_TIME_INDEX,
    GOATS_INDEX,
    HUGO_INDEX,
    EMS_INDEX,
    MIST_INDEX,
    FRST_BIT_INDEX,
    FRST_ESCAPE_INDEX,
    GORGE_VOID_INDEX,
    RUPEE_ROLL_INDEX,
    LANAYRU_GATE_CLIP_INDEX,
    PILLAR_CLIP_INDEX,
    LAKEBED_1_INDEX,
    TOAD_INDEX,
    KARG_INDEX,
    ELDIN_TWILIGHT_INDEX,
    LANAYRU_TWILIGHT_INDEX,
    WATERFALL_SIDEHOP_INDEX,
    BOSS_BUG_INDEX,
    IZA_INDEX,
    NORGOR_INDEX,
    SPR_WARP_SKIP_INDEX,
    SPR_INDEX,
    DARK_HAMMER_INDEX,
    LAKEBED_BK_SKIP_INDEX,
    ONEBOMB_INDEX,
    MDH_TOWER_INDEX,
    MDH_BRIDGE_INDEX,
    BULBLIN_CAMP_INDEX,
    AG_INDEX,
    POE_1_SKIP_INDEX,
    DSS_INDEX,
    STALLORD_INDEX,
    STALLORD2_INDEX,
    CITS_EARLY_INDEX,
    CITS_1_INDEX,
    AERALFOS_SKIP_INDEX,
    CITS_2_INDEX,
    FAN_TOWER_INDEX,
    ARGOROK_INDEX,
    PALACE_1_INDEX,
    PALACE_2_INDEX,
    EARLY_PLATFORM_INDEX,
    ZANT_INDEX,
    HC_INDEX,
    DARKNUT_INDEX,
    HC_TOWER_INDEX,
    BEAST_GANON_INDEX,
    HORSEBACK_GANON_INDEX,

    // Entry used as a count of entries
    ANY_SAVES_COUNT
};
#endif
#ifdef WII_PLATFORM
#define ANY_SPECIALS_AMNT 12
enum AnyPracticeIndex {
    ORDON_GATE_CLIP_INDEX,
    BACK_IN_TIME_INDEX,
    GOATS_INDEX,
    HUGO_INDEX,
    EMS_INDEX,
    MIST_INDEX,
    FRST_INDEX,
    EARLY_GALE_INDEX,
    DEKU_LIKE_INDEX,
    DIABABA_INDEX,
    MAILMAN_SKIP_INDEX,
    LANAYRU_GATE_CLIP_INDEX,
    KARG_FIGHT_INDEX,
    KARG_FLIGHT_INDEX,
    ELDIN_TWILIGHT_INDEX,
    BOMBHOUSE_SKIP_INDEX,
    MIDNA_TEXT_SKIP_INDEX,
    MIDNA_DIVE_INDEX,
    LAKEBED_1_INDEX,
    JAKE_INDEX,
    LANAYRU_TWILIGHT_INDEX,
    WATERFALL_SIDEHOP_INDEX,
    BOSS_BUG_INDEX,
    IZA_INDEX,
    TOAD_INDEX,
    MORPHEEL_INDEX,
    MDH_TOWER_INDEX,
    MDH_BRIDGE_INDEX,
    BULBLIN_CAMP_INDEX,
    AG_INDEX,
    POE_1_SKIP_INDEX,
    DSS_INDEX,
    STALLORD_INDEX,
    SPR_INDEX,
    SPR_BOOST_INDEX,
    SPR_FREEZARD_INDEX,
    DARK_HAMMER_INDEX,
    CITS_EARLY_INDEX,
    CITS_1_INDEX,
    AERALFOS_SKIP_INDEX,
    CITS_2_INDEX,
    FAN_TOWER_INDEX,
    ARGOROK_INDEX,
    PALACE_1_INDEX,
    PALACE_2_INDEX,
    EARLY_PLATFORM_INDEX,
    ZANT_INDEX,
    HC_INDEX,
    DARKNUT_INDEX,
    HC_TOWER_INDEX,
    BEAST_GANON_INDEX,
    HORSEBACK_GANON_INDEX,

    // Entry used as a count of entries
    ANY_SAVES_COUNT
};
#endif

class AnySavesMenu : public Menu {
public:
    AnySavesMenu(Cursor& cursor);
    virtual ~AnySavesMenu();
    virtual void draw();

private:
#ifdef GCN_PLATFORM
    Line lines[ANY_SAVES_COUNT];
#endif
#ifdef WII_PLATFORM
    Line lines[ANY_SAVES_COUNT];
#endif
};