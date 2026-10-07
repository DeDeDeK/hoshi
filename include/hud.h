#ifndef KAR_H_HUD
#define KAR_H_HUD

#include "datatypes.h"
#include "obj.h"

typedef enum HUDKind
{
    HUDKIND_0,
    HUDKIND_SPEEDOMETEROUT,
    HUDKIND_SPEEDOMETERIN,
    HUDKIND_TIMER,
    HUDKIND_PLYNUM,
    HUDKIND_PLICON,
    HUDKIND_HPBAR = 12,
    HUDKIND_HUDCAM = 19,
    HUDKIND_ITEMINDICATOR = 25,
    HUDKIND_PLYHUDPOS = 35,
    HUDKIND_LEGENDARYPIECE = 59, // 0x3b, attached by the piece-icon creators at 0x8012ac44 / 0x8012af14
    HUDKIND_CITYPAUSE = 65,   // 0x41, attached by HUD_PauseCreate
    HUDKIND_CITYSTATBG = 66,  // 0x42, attached by CityHUD_CreateStatChart
    HUDKIND_CITYSTATBAR = 67, // 0x43, attached by CityHUD_CreateStatBar
} HUDKind;

typedef struct HudMapIconData
{
    int x0;  // 0x00
    int x4;  // 0x04
    int x8;  // 0x08
    int xc;  // 0x0c
    int x10; // 0x10
    int x14; // 0x14
    int x18; // 0x18
    int x1c; // 0x1c
    int ply; // 0x20
} HudMapIconData;

typedef struct HUDElementData // created by HUD_AddElementData
{
    int x0;            // 0x0
    HUDKind kind;      // 0x4
    u8 ply : 4;        // 0x8, 0xf0, HUD_AddElementData's ply
    u8 view : 2;       // 0x8, 0x0c, HUD_AddElementData's view
    u8 is_visible : 1; // 0x8, 0x02
    u8 x8_01 : 1;      // 0x8, 0x01
    int xc;            //
    int x10;           //
    union
    {
        struct 
        {
            int hud_kind;           // 0x14, is 1 for this
            int hidden;             // 0x18, does not display if this is enabled
            int x1c;                // 0x1c
            int is_visible;         // 0x20
        } ply_num;
        struct 
        {
            int x14;            // 0x14
            int x18;            // 0x18
            int ply;            // 0x1c
            int is_visible;     // 0x20
        } plicon;
        struct
        {
            int x14;          // 0x14
            int x18;          // 0x18
            int x1c;          // 0x1c
            int x20;          // 0x20
            int x24;          // 0x24
            int x28;          // 0x28
            int x2c;          // 0x2c
            int x30;          // 0x30
            int x34;          // 0x34
            int x38;          // 0x38
            int x3c;          // 0x3c
            int x40;          // 0x40
            int x44;          // 0x44
            int x48;          // 0x48
            JOBJ *kirby_jobj; // 0x4c
        } hp_bar;
        struct
        {
           int x14;         // 0x14
           int timer;       // 0x18
           JOBJ *x1c;       // 0x1c
           JOBJ *x20;       // 0x20
           JOBJ *x24;       // 0x24
        } speedometer_outer;
        struct
        {
           int x14;         // 0x14
           int x18;         // 0x18
           int x1c;         // 0x1c
           int x20;         // 0x20
           int x24;         // 0x24
           int x28;         // 0x28
           JOBJ *x2c;       // 0x2c
           JOBJ *x30;       // 0x30
           JOBJ *x34;       // 0x34
           JOBJ *x38;       // 0x38
           JOBJ *x3c;       // 0x3c
           JOBJ *x40;       // 0x40
        } timer;
        struct
        {   
            Vec3 pos[4];           // 0x14
            JOBJ *j[4];            // 0x44
        } ply_hud;
        struct
        {   
            int x14;           // 0x14
            int x18;           // 0x18
            int x1c;           // 0x1c
            int ply;           // 0x20
            JOBJ *bar_j;       // 0x24
            JOBJ *num_right_j; // 0x28
            JOBJ *num_left_j;  // 0x2c
            JOBJ *sign_j;      // 0x30
        } city_stat_bar;
        struct
        {   
            int ply;                // 0x14, the pausing player, tracked from Gm_CheckPaused; also x1c's anim frame
            JOBJ *x18;              // 0x18
            JOBJ *x1c;              // 0x1c
            JOBJ *x20;              // 0x20, hidden in city mode 2
            Vec3 ply_offset[4];     // 0x24, starting translations of j
            JOBJ *j[4];             // 0x54
        } city_pause;
    };
} HUDElementData;

// this is a generic copy of the initial data in the struct above
// for the purpose of facilitating the creation of new hud objects.
typedef struct HUDElementCommonData
{
    int x0;            // 0x0
    HUDKind kind;      // 0x4
    u8 ply : 4;        // 0x8, 0xf0, HUD_AddElementData's ply
    u8 view : 2;       // 0x8, 0x0c, HUD_AddElementData's view
    u8 is_visible : 1; // 0x8, 0x02
    u8 x8_01 : 1;      // 0x8, 0x01
    int xc;            //
    int x10;           //
} HUDElementCommonData;

static HSD_Archive **stc_if_all_archive = (HSD_Archive **)(0x805dd0e0 + 0x690);
static u8 *g_hud_is_hidden = (u8 *)0x8048b5d9;  // this is a hoshi variable, placing it here for convenience

void CityHUD_CreateStatChart(int ply, int view); // 0x80128bb8
void CityHUD_CreateStatBar(int ply, int view, int stat_kind); // 0x80129154

void HUD_PauseCreate(); // 0x801285dc
void HUD_PauseDestroy(); // 0x80128a68
GOBJ *HUD_CreatePlyElement(int view, JOBJDesc *j); // 0x80114ba4, offset to the viewport's screen region in split screen
GOBJ *HUD_CreateMiscElement(JOBJDesc *desc, int p_link, int gx_link, int gx_pri); // 0x801147dc
GOBJ *HUD_CreateTimeUp(int ply); // 0x80114178
GOBJ *HUD_CreateFinish(int ply); // 0x801142fc

void HUD_GXLink(GOBJ *g, int pass); // 0x80114f1c
void HUD_AddElementData(GOBJ *g, HUDKind kind, int ply, int view); // 0x80114e24
void HUD_UpdateElement(JOBJ *j, int frame); // 0x8011503c

HSD_Archive **Gm_GetIfAllCityArchive();   // 0x80112050 - IfAll1c, contains common city trial specific graphics (timer, ready, pause, etc)
HSD_Archive **Gm_GetIfAllScreenArchive(); // 0x80112058 - IfAll1Xs, contains player related HUD that needs to be scaled down based on screen number

// The pair Gm_SetCinematicFreezeStage toggles: Gm_HideHUD / Gm_ShowHUD plus, when the
// view count allows, the player dots, the minimap and three more elements.
void HUD_HideForCinematic(void);   // 0x801129f8
void HUD_ShowAfterCinematic(void); // 0x80112a78

#endif