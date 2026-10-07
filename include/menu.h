#ifndef KAR_H_MENU
#define KAR_H_MENU

#include "datatypes.h"
#include "obj.h"
#include "scene.h"
#include "machine.h"

typedef enum MainMenuTopMenuKind
{
    MAINMENU_TOPMENU_AIRRIDE,
    MAINMENU_TOPMENU_TOPRIDE,
    MAINMENU_TOPMENU_CITY,
    MAINMENU_TOPMENU_OPTIONS,
    MAINMENU_TOPMENU_LAN,
} MainMenuTopMenuKind;

typedef enum MainMenuSubmenuKind
{
    MAINMENU_SUBMENU_AIRRIDE,
    MAINMENU_SUBMENU_TOPRIDE,
    MAINMENU_SUBMENU_CITY,
    MAINMENU_SUBMENU_OPTIONS,
    MAINMENU_SUBMENU_AIRRIDE_FREERUN,
    MAINMENU_SUBMENU_TOPRIDE_FREERUN,
    MAINMENU_SUBMENU_AIRRIDE_RECORDS,
    MAINMENU_SUBMENU_TOPRIDE_RECORDS,
    MAINMENU_SUBMENU_CITY_RECORDS,
} MainMenuSubmenuKind;

typedef enum CharacterKind
{
    CKIND_COMPACT,
    CKIND_WARP,
    CKIND_TURBO,
    CKIND_FORMULA,
    CKIND_SLICK,
    CKIND_SWERVE,
    CKIND_WAGON,
    CKIND_BULK,
    CKIND_SHADOW,
    CKIND_WINGED,
    CKIND_JET,
    CKIND_ROCKET,
    CKIND_WHEELIESCOOTER,
    CKIND_WHEELIEBIKE,
    CKIND_REXWHEELIE,
    CKIND_DRAGOON,
    CKIND_HYDRA,
    CKIND_FLIGHT,
    CKIND_DEDEDE,
    CKIND_METAKNIGHT,
    CKIND_NUM,
} CharacterKind;

typedef struct CharacterDesc
{
    u8 rider_kind;      // RiderKind
    u8 is_bike;         // bool
    u8 machine_kind;    // MachineKind within the is_bike class; CharacterDesc_GetMachineKind resolves it
} CharacterDesc;

typedef struct MainMenuData
{
    u8 input_lockout;                     // 0x30
    u8 x31;                               // 0x31
    u8 menu_name_tex_idx;                 // 0x32
    u8 is_in_submenu;                     // 0x33, think: 0 mode select, 1 option list, 2 rumble, 3 sound test, 4 movies, 5 data delete
    u8 top_menu;                          // 0x34
    u8 cursor_val[2];                     // 0x35
    u8 depth;                             // 0x37, either 0 or 1. used to get cursor value above
    MainMenuSubmenuKind submenu_kind : 8; // 0x38, 0 = air ride, 1 = top ride, 2 = city, 3 = options, etc
    MajorKind major_kind : 8;             // 0x39,
    u8 next_screen;                       // 0x3a, GameData.x2b menu screen the picked option leads to (MainMenu_GetNextScreen)
    u8 x3b;                               // 0x3b, cursor row indexing the next_screen table; the movie and data delete list cursor
    u8 delete_confirm_cursor;             // 0x3c, data delete Yes/No window: 0 Yes, 1 No
    u8 delete_confirm_stage;              // 0x3d, 0 closed, 1 first question, 2 second question for All Data
    u8 movie_extra_unlocked;              // 0x3e, movie list: Air Ride clear_kind 0x23 adds a seventh entry
    u8 nopad_mask;                        // 0x3f, rumble panel: bit per port with no controller
    int x40;                              // 0x40
    int x44;                              // 0x44
    int x48;                              // 0x48
    u16 x4c;                              // 0x4c
    s16 soundtest_bgm_kind;               // 0x4e
} MainMenuData;

typedef struct MenuElementData
{
    int x0;            // 0x0
    int kind;          // 0x4
    u8 is_visible : 1; // 0x8, 0x80
    u8 x8_40 : 1;      // 0x8, 0x40
    u8 x8_20 : 1;      // 0x8, 0x20
    u8 x8_10 : 1;      // 0x8, 0x10
    u8 x8_08 : 1;      // 0x8, 0x08
    u8 x8_04 : 1;      // 0x8, 0x04
    u8 x8_02 : 1;      // 0x8, 0x02
    u8 x8_01 : 1;      // 0x8, 0x01
    u8 x9;
    u8 xa;
    u8 xb;
    union 
    {
        struct 
        {
            u8 xc;
            u8 xd;
            u8 cursor_pos_id;       // 0x0E, cursorpos model this belongs to
            u8 id;                  // 0x0F, index in the cursorpos this cursor is
            u8 state;               // 0x10, 0 == unselected, 2 == selected
            u8 timer;               // 0x11, anim timer? counts down from 9 repeatedly
        } cursor1;
        struct 
        {
            u8 xc[0xa8];
            u8 xb4;
            u8 xb5;
            u8 anim_direction;      // 0 = not moving, 1 = moving out, 2 = moving in 
            u8 anim_timer;          // seems to tick down from 4
        } cursor1_pos;
    };
} MenuElementData;

typedef struct SoundTestDesc
{
    u8 bgm;
    u8 x1;
    u8 x2;
    u8 x3;
    u8 kind; // 0 = unk, 1 = play via bgm id
} SoundTestDesc;

static HSD_Archive **stc_MnSelplyAll_archive = (HSD_Archive **)(0x805dd0e0 + 0x6dc);
static SoundTestDesc *stc_soundtest_desc = (SoundTestDesc *)0x80496458; // 62 of these

GOBJ *MenuElement_Create(JOBJDesc *jobjdesc); // 0x801388a8
MenuElementData *MenuElement_AddData(GOBJ *menu_element_gobj, int element_kind); // 0x80138a00
void MainMenu_SetTexAnimFrame(JOBJ *jobj, int frame, float rate);              // 0x80138c1c, texture animations only

CharacterKind SelIcon_GetCKind(int row_idx, int col_idx); // 0x8000b9bc, the grid
CharacterKind SelIcon_GetCKindLinear(int idx); // 0x8000b9a8, the single-row strip
CharacterDesc *Character_GetDesc(CharacterKind ckind); // 0x8000b9dc

// The roster tables those three form an address into, back to back with no slack.
#define SELICON_GRID_ROWS 2
#define SELICON_GRID_COLS 10
static u8 *stc_selicon_ckind_linear = (u8 *)0x804957ec;                 // [CKIND_NUM]
static u8 *stc_selicon_ckind_grid = (u8 *)0x80495800;                   // [SELICON_GRID_ROWS * SELICON_GRID_COLS]
static CharacterDesc *stc_character_desc = (CharacterDesc *)0x80495814; // [CKIND_NUM]

// The reverse of CharacterDesc_GetMachineKind, over stc_machine_ckind_star and
// stc_machine_ckind_bike. The results screens and the time-attack board use it to
// reach a machine's art.
CharacterKind Machine_GetCKind(int is_bike, int class_index); // 0x8000b9f4
static u8 *stc_machine_ckind_star = (u8 *)0x80495850;            // [VCSTAR_NUM]
static u8 *stc_machine_ckind_bike = (u8 *)(0x805dd0e0 - 0x7fbc); // [VCWHEEL_NUM]

// The frame of IfAll3c.dat's ScInfStarIcon TexAnim a machine is drawn with on the
// stadium HUDs (High Jump's opponent heights, the flight-distance marker, the stadium
// icon element): a byte out of stc_machine_icon_frame, read at
// is_bike * MACHINE_ICON_FRAME_BIKE_BASE + class_index with no bound.
int Machine_GetIconFrame(int is_bike, int class_index); // 0x8011584c
#define MACHINE_ICON_FRAME_BIKE_BASE 12
static u8 *stc_machine_icon_frame = (u8 *)0x804a7b84; // [17]

// City Trial's field blips. Create builds one template joint per MachineKind and the
// GObj whose GX callback draws every view's instances; Destroy removes the instances,
// the templates and the GObj.
void CityBlip_Create(void);                     // 0x801226e8
void CityBlip_GXCallback(GOBJ *gobj, int pass); // 0x80122380
void CityBlip_Destroy(void);                    // 0x80122a6c

// How far City Trial's field blip rides over a machine: a float out of
// stc_blip_lift_star or stc_blip_lift_bike, read by class slot with no bound, times
// stc_blip_lift_scale. Its only caller is the blip's per-view placement at 0x80122b38.
float CityBlip_GetMachineLift(int is_bike, int class_index); // 0x800096b8
static float *stc_blip_lift_star = (float *)0x804894a0;  // [VCSTAR_NUM]
static float *stc_blip_lift_bike = (float *)0x804894ec;  // [VCWHEEL_NUM]
static float *stc_blip_lift_scale = (float *)0x805de738; // 0.175

// Places each player's machine model on the City Trial stat radar screen, lowered by a
// float out of a 19-entry star table at 0x80489558 or a 7-entry bike table at 0x804895a4,
// read at the player's saved (is_bike, class slot) with no bound, times 0.175.
void MnRadar_PlaceMachines(void); // 0x80045e14

// CharacterDesc.machine_kind is a class-relative slot, not a MachineKind: it
// equals the VCKIND only for the vanilla stars.
static inline MachineKind CharacterDesc_GetMachineKind(CharacterDesc *desc)
{
    return MachineKind_FromClassIndex(desc->is_bike, desc->machine_kind);
}

// Rules screens (minors 3, 4 and 5: Air Ride, Top Ride, City Trial). A row id is also the
// frame of the row-name TexAnim on ScMenSelruleFrame, and each mode's think switches on it
// to find the row's widget and storage.
typedef enum RuleRowKind
{
    RULEROW_RULES,        // Laps / Time
    RULEROW_LAPS,         // number, 0 = Recommended
    RULEROW_DAMAGE,       //
    RULEROW_SPEEDHELP,    //
    RULEROW_ENEMIES,      //
    RULEROW_TEMPO,        //
    RULEROW_COURSESELECT, //
    RULEROW_ITEMS,        // Top Ride item amount
    RULEROW_CAMERA,       //
    RULEROW_CAMERAANGLE,  //
    RULEROW_FEATURES,     //
    RULEROW_TIME,         // number, minutes
    RULEROW_STADIUM,      // stadium picker
    RULEROW_EVENTS,       //
    RULEROW_ITEMKIND,     // Top Ride item set, labelled "Items" too
    RULEROW_LAPSORTIME,   // Air Ride number row, laps or minutes by RULEROW_RULES
    RULEROW_MORE,         // "Additional Rules" page button (ScMenSelruleFrame2)
    RULEROW_NONE = 0xff,
} RuleRowKind;

// Frames of the ScMenSelruleContents TexAnim, the value labels.
typedef enum RuleValueLabel
{
    RULELABEL_LAPS,
    RULELABEL_TIME,
    RULELABEL_RECOMMENDED,
    RULELABEL_ON,
    RULELABEL_OFF,
    RULELABEL_WEAK,
    RULELABEL_STRONG,
    RULELABEL_NORMAL,
    RULELABEL_SLOW,
    RULELABEL_SHUFFLE,
    RULELABEL_LOSER,
    RULELABEL_FEW,
    RULELABEL_MANY,
    RULELABEL_INORDER, // untranslated Japanese, unused by the US tables
    RULELABEL_FIXED,
    RULELABEL_DIAGONAL,
    RULELABEL_SIDE,
    RULELABEL_ATTACK,
    RULELABEL_MYSTERY,
    RULELABEL_NONE,
    RULELABEL_NUM,
} RuleValueLabel;

#define RULE_PAGE_NUM 2
#define RULE_ROW_NUM 5
#define RULE_VALUE_NUM 4

// Air Ride and City Trial rows are fixed tables, indexed [page][row] and [page][row][value].
// A value row's label table maps the value index to a RuleValueLabel, 0xff for unused.
static u8 *stc_airride_rule_row_num = (u8 *)(0x805dd0e0 - 0x7f68);    // [RULE_PAGE_NUM]
static u8 *stc_airride_rule_row_kind = (u8 *)0x804965b8;              // [RULE_PAGE_NUM * RULE_ROW_NUM]
static u8 *stc_airride_rule_value_num = (u8 *)0x804965c4;             // [RULE_PAGE_NUM * RULE_ROW_NUM]
static u8 *stc_airride_rule_value_label = (u8 *)0x804965d0;           // [RULE_PAGE_NUM * RULE_ROW_NUM * RULE_VALUE_NUM]
static int *stc_airride_rule_desc = (int *)0x804aa230;                // [RULE_PAGE_NUM * RULE_ROW_NUM * RULE_VALUE_NUM], SisSelrule ids
static u8 *stc_city_rule_row_num = (u8 *)(0x805dd0e0 - 0x7f58);       // one page
static u8 *stc_city_rule_row_kind = (u8 *)(0x805dd0e0 - 0x7f54);      // [RULE_ROW_NUM]
static u8 *stc_city_rule_value_num = (u8 *)(0x805dd0e0 - 0x7f4c);     // [RULE_ROW_NUM]
static u8 *stc_city_rule_value_label = (u8 *)0x80496c90;              // [RULE_ROW_NUM * RULE_VALUE_NUM]
static int *stc_city_rule_desc = (int *)0x804aa338;                   // [RULE_ROW_NUM * RULE_VALUE_NUM]

// Top Ride rows name their values through a shared option table instead, and an option
// with a clear_kind in stc_topride_rule_unlock only appears once that Top Ride checklist
// box is unlocked.
typedef struct TopRideRuleOption
{
    u8 label; // RuleValueLabel
    u8 desc;  // index into stc_topride_rule_desc
    u8 value; // the row's stored value
} TopRideRuleOption;
#define TOPRIDE_RULE_OPTION_NUM 25
#define TOPRIDE_RULE_DESC_NUM 26
static u8 *stc_topride_rule_row_num = (u8 *)(0x805dd0e0 - 0x7f60);                    // [RULE_PAGE_NUM]
static u8 *stc_topride_rule_row_kind = (u8 *)0x80496760;                              // [RULE_PAGE_NUM * RULE_ROW_NUM]
static u8 *stc_topride_rule_value_num = (u8 *)0x8049676c;                             // [RULE_PAGE_NUM * RULE_ROW_NUM]
static TopRideRuleOption *stc_topride_rule_option = (TopRideRuleOption *)0x80496778;  // [TOPRIDE_RULE_OPTION_NUM]
static u8 *stc_topride_rule_option_id = (u8 *)0x804967c4;                             // [RULE_PAGE_NUM * RULE_ROW_NUM * RULE_VALUE_NUM]
static u8 *stc_topride_rule_unlock = (u8 *)0x804967ec;                                // same shape, 0xff = always shown
static int *stc_topride_rule_desc = (int *)0x804aa2d0;                                // [TOPRIDE_RULE_DESC_NUM], SisSelrule ids

void AirRideRules_MinorLoad(void);                       // 0x8001aa94
void AirRideRules_MinorThink(void);                      // 0x8001ae7c
void AirRideRules_MinorExit(void *data);                 // 0x8001ae58
void AirRideRules_Think(void);                           // 0x800196e4
void AirRideRules_InitData(void);                        // 0x8001a5b8, from Gm_InitData
void AirRideRules_LoadSettings(void);                    // 0x8001a3c0, settings -> AirRideRuleMenuData
void AirRideRules_SaveSettings(void);                    // 0x8001acc4, AirRideRuleMenuData -> settings
void AirRideRules_ClearLongLockout(void);                // 0x8001a604
void AirRideRules_CreateRowValue(s8 page, s8 row);       // 0x8001a62c
void AirRideRules_SelectRow(s8 page, s8 row);            // 0x80018d38
void AirRideRules_DeselectRow(s8 page, s8 row);          // 0x80019240
void AirRideRules_UpdateArrows(s8 page, s8 row);         // 0x80018a2c

void TopRideRules_MinorLoad(void);                       // 0x8001eaac
void TopRideRules_MinorThink(void);                      // 0x8001ecf4
void TopRideRules_MinorExit(void *data);                 // 0x8001ecd0
void TopRideRules_Think(void);                           // 0x8001c608
void TopRideRules_InitData(void);                        // 0x8001d9a4, from Gm_InitData
void TopRideRules_BuildRows(void);                       // 0x8001da14
void TopRideRules_LoadSettings(void);                    // 0x8001d43c
void TopRideRules_SaveSettings(void);                    // 0x8001d74c
void TopRideRules_ClearLongLockout(void);                // 0x8001d9ec
void TopRideRules_CreateRowValue(s8 page, s8 row);       // 0x8001ddf8
void TopRideRules_SelectRow(s8 page, s8 row);            // 0x8001af88
void TopRideRules_DeselectRow(s8 page, s8 row);          // 0x8001bc2c
void TopRideRules_UpdateArrows(s8 page, s8 row);         // 0x8001ae9c

void CityRules_MinorLoad(void);                          // 0x8001fbd8
void CityRules_MinorThink(void);                         // 0x80020168
void CityRules_MinorExit(void *data);                    // 0x800200ac, also saves the settings
void CityRules_Think(void);                              // 0x8001ee60
void CityRules_InitData(void);                           // 0x8001fb64, from Gm_InitData
void CityRules_LoadSettings(void);                       // 0x8001f950, also builds the stadium option list
void CityRules_ClearLongLockout(void);                   // 0x8001fbb0
void CityRules_UpdateArrows(s8 page, s8 row);            // 0x8001ed14

// The rules screens' element layer, one call per widget operation. Slot 0 of a row is its
// collapsed value; slots 1-4 are the options laid out while the row has the cursor.
void RuleMenu_LoadAirRide(void);                                   // 0x80132cdc, archive, camera, light, symbols
void RuleMenu_LoadTopRide(void);                                   // 0x80132db4
void RuleMenu_LoadCity(void);                                      // 0x80132e8c
void RuleMenu_CreateCObj(void);                                    // 0x801395ac
void RuleMenu_CreateLObj(void);                                    // 0x8013aa60
void RuleMenu_CreateBackground(void);                              // 0x80132f64, Bg and Panel
void RuleMenu_CreateDescription(void);                             // 0x80132f88
void RuleMenu_LoadSisFile(void);                                   // 0x8013b974, SisSelrule.dat into SIS slot 0
void RuleMenu_CreateTextCanvas(void);                              // 0x8013b648
void RuleMenu_ShowDescriptionText(s8 text_id);                     // 0x8013b9a8, -1 clears
void RuleMenu_CreatePage(s8 page, s8 row_num);                     // 0x80132fac
void RuleMenu_CreateRow(s8 page, s8 row, s8 value_num, s8 label);  // 0x80132fcc, label is a RuleRowKind
void RuleMenu_SelectRow(s8 page, s8 row);                          // 0x80133024
void RuleMenu_DeselectRow(s8 page, s8 row);                        // 0x80133044
void RuleMenu_SetRowLabel(s8 page, s8 row, s8 label);              // 0x80133064
void RuleMenu_CreatePageButton(s8 page, s8 row);                   // 0x80133084
void RuleMenu_SelectPageButton(s8 page);                           // 0x801330a4
void RuleMenu_DeselectPageButton(s8 page);                         // 0x801330c4
void RuleMenu_CreateValue(s8 page, s8 row, s8 slot, s8 label);     // 0x801330e4, label is a RuleValueLabel
void RuleMenu_SetValueLabel(s8 page, s8 row, s8 slot, s8 label);   // 0x80133104
void RuleMenu_ShowValue(s8 page, s8 row, s8 slot);                 // 0x80133124
void RuleMenu_HideValue(s8 page, s8 row, s8 slot);                 // 0x80133144
void RuleMenu_HighlightValue(s8 page, s8 row, s8 slot);            // 0x80133164
void RuleMenu_UnhighlightValue(s8 page, s8 row, s8 slot);          // 0x80133184
void RuleMenu_CreateNumber(s8 page, s8 row, s8 value, s8 unit);    // 0x801331a4, unit 0 none, 1 "min"
void RuleMenu_SetNumber(s8 value, s8 unit);                        // 0x801331c4, below 1 shows "Recommended"
void RuleMenu_ExpandNumber(void);                                  // 0x801331e8
void RuleMenu_CollapseNumber(void);                                // 0x80133208
void RuleMenu_CreateStadium(s8 page, s8 row, s8 frame);            // 0x80133228, frame is a stadium option kind
void RuleMenu_SetStadium(s8 frame);                                // 0x80133248
void RuleMenu_ExpandStadium(void);                                 // 0x80133268
void RuleMenu_CollapseStadium(void);                               // 0x80133288
void RuleMenu_CreateCursor(s8 page, s8 row, s8 slot, s8 anim);     // 0x801332a8, slot is 0-based, drawn on slot + 1
void RuleMenu_MoveCursor(s8 page, s8 row, s8 slot);                // 0x801332c8
void RuleMenu_SetCursorAnim(s8 anim);                              // 0x801332e8, 1 for the stadium row
void RuleMenu_ShowCursor(void);                                    // 0x80133308
void RuleMenu_HideCursor(void);                                    // 0x80133328
void RuleMenu_ShowLeftArrow(void);                                 // 0x80133348
void RuleMenu_HideLeftArrow(void);                                 // 0x80133368
void RuleMenu_ShowRightArrow(void);                                // 0x80133388
void RuleMenu_HideRightArrow(void);                                // 0x801333a8
void RuleMenu_SetAirRideDescription(s8 page, s8 row, s8 value);    // 0x801333c8, stc_airride_rule_desc
void RuleMenu_SetTopRideDescription(s8 desc);                      // 0x80133418, stc_topride_rule_desc
void RuleMenu_SetCityDescription(s8 page, s8 row, s8 value);       // 0x80133450, stc_city_rule_desc
void RuleMenu_HideDescription(void);                               // 0x801334a0, until the panel intro ends
void RuleMenu_TurnPageNext(void);                                  // 0x801334c0
void RuleMenu_TurnPagePrev(void);                                  // 0x801334f0
void RuleMenu_Destroy(void);                                       // 0x80133520
void Menu_HideDescription(void);                                   // 0x80146800, ScMenuCommon description text
void Menu_ShowDescription(void);                                   // 0x801466d0

// MnSelrule element implementations behind RuleMenu_*. Each IndexXSymbol binds the
// ScMenSelrule* public into ScMenuCommon.rules; userdata kinds are 0xa2 (Bg) to 0xab (Cursor).
void MnSelrule_IndexBgSymbol(void);                                  // 0x80177e78, Air Ride
void MnSelrule_IndexBgm2dSymbol(void);                               // 0x80177ec4, Top Ride
void MnSelrule_IndexBgCtSymbol(void);                                // 0x80177f10, City Trial
void MnSelrule_CreateBg(void);                                       // 0x80177f5c
void MnSelrule_DestroyBg(void);                                      // 0x80177ffc
void MnSelrule_BgThink(GOBJ *gobj);                                  // 0x80177dd8, intro anim, then the loop
void MnSelrule_IndexPanelSymbol(void);                               // 0x80178120
void MnSelrule_CreatePanel(void);                                    // 0x8017816c
void MnSelrule_DestroyPanel(void);                                   // 0x8017820c
void MnSelrule_PanelThink(GOBJ *gobj);                               // 0x801780d0, shows the description after 5 frames
void MnSelrule_PlayPanelNext(void);                                  // 0x80178040
void MnSelrule_PlayPanelPrev(void);                                  // 0x80178088
void MnSelrule_IndexFposSymbol(void);                                // 0x80178860
void MnSelrule_CreateFpos(s8 page, s8 row_num);                      // 0x801788ac
void MnSelrule_DestroyFpos(void);                                    // 0x80178ab0
void MnSelrule_FposThink(GOBJ *gobj);                                // 0x80178674
void MnSelrule_GetRowPos(s8 page, s8 row, Vec3 *out);                // 0x80178250
void MnSelrule_SlidePageInLeft(s8 page);                             // 0x801782d4
void MnSelrule_SlidePageOutLeft(s8 page);                            // 0x801783bc
void MnSelrule_SlidePageInRight(s8 page);                            // 0x801784a4
void MnSelrule_SlidePageOutRight(s8 page);                           // 0x8017858c
void MnSelrule_IndexFrameSymbol(void);                               // 0x80178d64
void MnSelrule_CreateFrame(s8 page, s8 row, s8 label);               // 0x80178db0
void MnSelrule_DestroyFrames(void);                                  // 0x80178f48
void MnSelrule_FrameThink(GOBJ *gobj);                               // 0x80178c94
void MnSelrule_SelectFrame(s8 page, s8 row);                         // 0x80178b1c
void MnSelrule_DeselectFrame(s8 page, s8 row);                       // 0x80178b98
void MnSelrule_SetFrameLabel(s8 page, s8 row, s8 label);             // 0x80178c14
void MnSelrule_IndexFrame2Symbol(void);                              // 0x80179190
void MnSelrule_CreateFrame2(s8 page, s8 row);                        // 0x801791dc
void MnSelrule_DestroyFrame2(void);                                  // 0x80179324
void MnSelrule_Frame2Think(GOBJ *gobj);                              // 0x801790c0
void MnSelrule_SelectFrame2(s8 page);                                // 0x80178fd8
void MnSelrule_DeselectFrame2(s8 page);                              // 0x8017904c
void MnSelrule_IndexCposSymbol(void);                                // 0x80179618
void MnSelrule_CreateCpos(s8 page, s8 row, s8 value_num);            // 0x80179664
void MnSelrule_DestroyCpos(void);                                    // 0x801798b0
void MnSelrule_CposThink(GOBJ *gobj);                                // 0x801794b0
void MnSelrule_GetSlotPos(s8 page, s8 row, s8 slot, Vec3 *out);      // 0x80179414
void MnSelrule_GetSlotScale(s8 page, s8 row, Vec3 *out);             // 0x80179390
void MnSelrule_IndexContentsSymbol(void);                            // 0x80179d64
void MnSelrule_CreateContents(s8 page, s8 row, s8 slot, s8 label);   // 0x80179db0
void MnSelrule_DestroyContents(void);                                // 0x8017a014
void MnSelrule_ContentsThink(GOBJ *gobj);                            // 0x80179bf0
void MnSelrule_SetContentsLabel(s8 page, s8 row, s8 slot, s8 label); // 0x80179a68
void MnSelrule_ShowContents(s8 page, s8 row, s8 slot);               // 0x80179b00
void MnSelrule_HideContents(s8 page, s8 row, s8 slot);               // 0x80179b78
void MnSelrule_HighlightContents(s8 page, s8 row, s8 slot);          // 0x80179940
void MnSelrule_UnhighlightContents(s8 page, s8 row, s8 slot);        // 0x801799d4
void MnSelrule_IndexNumSymbol(void);                                 // 0x8017a4ac
void MnSelrule_CreateNum(s8 page, s8 row, s8 value, s8 unit);        // 0x8017a4f8
void MnSelrule_DestroyNum(void);                                     // 0x8017a798
void MnSelrule_NumThink(GOBJ *gobj);                                 // 0x8017a3d8
void MnSelrule_SetNum(s8 value, s8 unit);                            // 0x8017a26c
void MnSelrule_ExpandNum(void);                                      // 0x8017a0b4
void MnSelrule_CollapseNum(void);                                    // 0x8017a190
void MnSelrule_IndexStadiumSymbol(void);                             // 0x8017aab0
void MnSelrule_CreateStadium(s8 page, s8 row, s8 frame);             // 0x8017aafc
void MnSelrule_DestroyStadium(void);                                 // 0x8017ac4c
void MnSelrule_StadiumThink(GOBJ *gobj);                             // 0x8017a9dc
void MnSelrule_SetStadium(s8 frame);                                 // 0x8017a994
void MnSelrule_ExpandStadium(void);                                  // 0x8017a7dc
void MnSelrule_CollapseStadium(void);                                // 0x8017a8b8
void MnSelrule_IndexCursorSymbol(void);                              // 0x8017b140
void MnSelrule_CreateCursor(s8 page, s8 row, s8 slot, s8 anim);      // 0x8017b18c
void MnSelrule_DestroyCursor(void);                                  // 0x8017b388
void MnSelrule_CursorThink(GOBJ *gobj);                              // 0x8017afd8
void MnSelrule_MoveCursor(s8 page, s8 row, s8 slot);                 // 0x8017addc
void MnSelrule_SetCursorAnim(s8 anim);                               // 0x8017ad80
void MnSelrule_ShowCursor(void);                                     // 0x8017af78
void MnSelrule_HideCursor(void);                                     // 0x8017afa8
void MnSelrule_ShowLeftArrow(void);                                  // 0x8017ac90
void MnSelrule_HideLeftArrow(void);                                  // 0x8017accc
void MnSelrule_ShowRightArrow(void);                                 // 0x8017ad08
void MnSelrule_HideRightArrow(void);                                 // 0x8017ad44

// The memory card prompt (major MJRKIND_CARD, minor MNRKIND_CARD) and its window,
// MnDialogueAll.dat. The prompt keeps its state at the start of GameData while it runs.
typedef enum CardPromptChoices
{
    CARDPROMPT_CHOICES_NONE,   // message only, both choices blank (SisDialogue 7) and no cursors
    CARDPROMPT_CHOICES_YESNO,  // SisDialogue 3 / 4
    CARDPROMPT_CHOICES_OKRETRY, // SisDialogue 2 / 5
} CardPromptChoices;

typedef struct CardPromptData // GameData+0x0
{
    s8 lockout;       // 0x0, input is ignored for this many scene frames
    s8 cursor;        // 0x1, 0 left, 1 right, 2 none
    s8 message;       // 0x2, SisDialogue premade id; X steps it when dblevel >= 3
    s8 left_text;     // 0x3, SisDialogue premade id
    s8 right_text;    // 0x4
    s8 state;         // 0x5, CardPrompt_MenuStateChange state, 0-28
    s8 choices;       // 0x6, CardPromptChoices
    s8 answer;        // 0x7, set on A or Start: 0 left, 1 right, 2 nothing this frame
    u8 x8;            // 0x8, confirming the right choice instead errors to state 16
    u8 x9;            // 0x9
    u8 xa;            // 0xa
    u8 xb;            // 0xb
} CardPromptData;

CardPromptData *CardPrompt_GetData(void); // 0x80047844, Gm_GetGameData
void CardPrompt_MinorLoad(void);          // 0x80047d60
void CardPrompt_MinorLeave(void *data);   // 0x80047f0c

HSD_Archive **MnDialogue_GetArchiveSlot(void);  // 0x80138e24, r13 slot Gm_LoadGameFile fills
void MnDialogue_FreeArchive(void);              // 0x80138e2c
void MnDialogue_Load(void);                     // 0x80138e68, archive, element pool, camera, light, symbols
void MnDialogue_CreateCObj(void);               // 0x8013a37c, from ScMenuCommon.dialogue.sobj into cam_gobj
void MnDialogue_CreateLObj(void);               // 0x8013b3f8
void MnDialogue_CreateWindow(void);             // 0x80138ef4, Bg, both cursors, cursor placement
void MnDialogue_InitText(void);                 // 0x80138f1c, SIS file, canvas, the three texts
void MnDialogue_Destroy(void);                  // 0x80138f44, Menu_DestroyCommon included
void MnDialogue_IndexBgSymbol(void);            // 0x8017bd08
void MnDialogue_CreateBg(void);                 // 0x8017bd54
void MnDialogue_DestroyBg(void);                // 0x8017be84
void MnDialogue_BgThink(GOBJ *gobj);            // 0x8017bce4
void MnDialogue_IndexCursorSymbol(void);        // 0x8017c0f4
void MnDialogue_CreateCursors(void);            // 0x8017c140
void MnDialogue_DestroyCursors(void);           // 0x8017c1dc
void MnDialogue_CursorThink(GOBJ *gobj);        // 0x8017c0f0, empty
void MnDialogue_PlaceCursors(void);             // 0x8017bf78
void MnDialogue_GetCursorPos(s8 cursor, Vec3 *out); // 0x8017b654
void MnDialogue_HighlightCursor(int cursor);    // 0x8017bf20, pose frame 1
void MnDialogue_UnhighlightCursor(int cursor);  // 0x8017bec8, pose frame 0
void MnDialogue_ShowCursor(int cursor);         // 0x8017c068
void MnDialogue_HideCursor(int cursor);         // 0x8017c0ac
void MnDialogue_LoadSisFile(void);              // 0x8017c390, SisDialogue.dat into SIS slot 0
void MnDialogue_CreateTextCanvas(void);         // 0x8017c3c4
void MnDialogue_CreateTexts(void);              // 0x8017c418, ScMenuCommon.text description_text, x30, x34
void MnDialogue_SetMessage(s8 text_id);         // 0x8017c5a0
void MnDialogue_SetLeftChoice(s8 text_id);      // 0x8017c5d8
void MnDialogue_SetRightChoice(s8 text_id);     // 0x8017c610
void MnDialogue_ProjectMessagePos(Vec3 *out);   // 0x8017b778, Bg joints projected through cam_gobj
void MnDialogue_ProjectMessageRight(Vec3 *out); // 0x8017b82c
void MnDialogue_ProjectMessageBottom(Vec3 *out); // 0x8017b8e0
void MnDialogue_ProjectLeftChoicePos(Vec3 *out); // 0x8017ba14
void MnDialogue_ProjectChoiceRight(Vec3 *out);  // 0x8017bac8
void MnDialogue_ProjectChoiceBottom(Vec3 *out); // 0x8017bb7c
void MnDialogue_ProjectRightChoicePos(Vec3 *out); // 0x8017bc30
float MnDialogue_GetMessageWidth(void);         // 0x8017b6f8
float MnDialogue_GetMessageHeight(void);        // 0x8017b738
float MnDialogue_GetChoiceWidth(void);          // 0x8017b994
float MnDialogue_GetChoiceHeight(void);         // 0x8017b9d4
void Menu_DestroyTexts(void);                   // 0x8017c244, every ScMenuCommon.text slot
void Menu_DestroyCommon(void);                  // 0x80131928, camera, light, menu GObjs and archives

// The Options data delete list and its Yes/No window (MnAll.dat ScMenOpdelwin). The
// window is one model whose frame is prompt * 2 + cursor: prompt 0 "Delete this data?",
// 1 "Are you sure?"; cursor 0 Yes, 1 No. Question and answers are baked into the textures.
void MainMenu_DataDeleteThink(void);                       // 0x80017050
void MainMenu_MovieSelectThink(void);                      // 0x80016da0
void MainMenu_RumbleThink(void);                           // 0x800174fc
void MainMenu_OpenDeleteConfirm(s8 prompt, s8 cursor);     // 0x80132618
void MainMenu_SetDeleteConfirm(s8 prompt, s8 cursor);      // 0x80132638
void MainMenu_CloseDeleteConfirm(void);                    // 0x80132658
void _MainMenu_OpenDeleteConfirm(s8 prompt, s8 cursor);    // 0x8014a9d8
void _MainMenu_SetDeleteConfirm(s8 prompt, s8 cursor);     // 0x8014a8ec
void _MainMenu_CloseDeleteConfirm(void);                   // 0x8014aaa8
void MainMenu_IndexDeleteConfirmSymbol(void);              // 0x8014a98c
void MainMenu_DeleteConfirmThink(GOBJ *gobj);              // 0x8014a958

#endif
