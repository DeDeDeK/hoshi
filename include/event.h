#ifndef KAR_H_EVENT
#define KAR_H_EVENT

#include "datatypes.h"
#include "stadium.h"

typedef enum EventKind
{
    EVKIND_DYNABLADE,
    EVKIND_TAC,
    EVKIND_METEOR,
    EVKIND_PILLAR,
    EVKIND_RUNAMOK,
    EVKIND_RESTORATIONAREA,
    EVKIND_RAILFIRE,
    EVKIND_SAMEITEM,
    EVKIND_LIGHTHOUSE,
    EVKIND_SECRETCHAMBER,
    EVKIND_PREDICTION,
    EVKIND_MACHINEFORMATION,
    EVKIND_UFO,
    EVKIND_BOUNCE,
    EVKIND_FOG,
    EVKIND_FAKEPOWERUPS,
    EVKIND_NUM,
} EventKind;

static const char *const EventKind_Names[EVKIND_NUM] = {
    [EVKIND_DYNABLADE]        = "Dyna Blade",
    [EVKIND_TAC]              = "Tac",
    [EVKIND_METEOR]           = "Meteor",
    [EVKIND_PILLAR]           = "Pillar",
    [EVKIND_RUNAMOK]          = "Run Amok",
    [EVKIND_RESTORATIONAREA]  = "Restoration Area",
    [EVKIND_RAILFIRE]         = "Rail Fire",
    [EVKIND_SAMEITEM]         = "All Same Item",
    [EVKIND_LIGHTHOUSE]       = "Lighthouse",
    [EVKIND_SECRETCHAMBER]    = "Secret Chamber",
    [EVKIND_PREDICTION]       = "Prediction",
    [EVKIND_MACHINEFORMATION] = "Machine Formation",
    [EVKIND_UFO]              = "UFO",
    [EVKIND_BOUNCE]           = "Bounce",
    [EVKIND_FOG]              = "Fog",
    [EVKIND_FAKEPOWERUPS]     = "Fake Powerups",
};

// Archive root of GrCity1Event.dat. Loaded into memory whenever the player
// enters City Trial (by fn_grSetupCityEventData, which runs regardless of the
// CT events on/off setting), and stashed at GrData.event_config. The same
// pointer is also stored as EventCheckData.data when events are enabled.
typedef struct EventConfigData
{
    struct
    {
        int delay_min;                     // 0x0
        int delay_max;                     // 0x4
        int occur_chance;                  // 0x8
        int skip_chance;                   // 0xc
        u8 x10[0x4];                             // 0x10, unknown (not read by CityEvent_StateIdle)
        int min_time;                            // 0x14, min match time (frames) before events start; CityEvent_StateIdle gates on City_GetMinSecMs?() >= this
        int prev_kind_max;                       // 0x18, max history entries
        int music_fadeout_frames;                // 0x1c, number of frames to fade out the music
        int starting_delay;                      // 0x20, frames in state 1 before transitioning to state 2
        int cleanup_delay;                       // 0x24, frames in state 3 before returning to idle
        int hud_display_frames;                  // 0x28, frames to display event HUD text
        struct                                   // 0x2c, contains a weight for every event for every type of stadium
        {                                        //
            int arr[STGROUP_NUM][EVKIND_NUM]; //
        } *weights;                        //
        struct                                   // 0x30, per-event parameters, 0xC bytes per EventKind
        {                                        //
            struct                               //
            {                                    //
                int category;                    // 0x00, category for diversity boost (0 or 1)
                int duration;                    // 0x04, event duration in frames
                u8 once_only;                    // 0x08, if 1, can only occur once per match
                u8 is_siren;                     // 0x09, if 1, plays siren + fades music + changes sky
                u8 xa;                           //
                u8 xb;                           //
            } arr[EVKIND_NUM];                  //
        } *param;
    } *event;                                    // 0x00
    struct                                       // 0x04, per-event BGM/sky data, 0x14 bytes per EventKind
    {                                            //
        int bgm_file;                            // 0x00, BGM file index for secondary music
        int sky_preset;                          // 0x04, sky preset index (-1 = no change)
        int location_idx;                        // 0x08, index into event location array
        int location_count;                      // 0x0C, number of locations
        void *event_data;                        // 0x10, pointer to event-specific data
    } *bgm_sky;                                  //
} EventConfigData;

typedef struct EventCheckData
{
    EventConfigData *data;
    int state; // 0 = no event, 1 = event starting, 2 = event active, 3 = event ending
    EventKind cur_kind;
    int xc;
    int timer;             // 0x10, counts up
    int event_time;        // 0x14, event triggers when timer matches this number
    int prev_kind[10];     // 0x18, last 10 events that have occurred
    int prev_kind_num;     // 0x40, number of events that occurred this match
    u8 x44[0x40];          // 0x44
    EventKind reserve[16]; // 0x84
    int reserve_kind_num;  // 0xc4. max is 16 (Gr_EventGene_Reserve_Num)
} EventCheckData;

typedef struct EventFunction
{
    void *x0;
    void *x4;
    void *x8;
    void *xc;
    int (*check)(EventCheckData *gp);
} EventFunction;

static GOBJ **stc_eventcheck_gobj = (GOBJ **)(0x805dd0e0 + 0x618);
static int *stc_event_machineformation_loadnum = (int *)(0x805dd0e0 + 0x750); // number of machines spawned for the machine formation event
// note: 0x80538088 is the Audio3D global (audio_3d_data, audio.h), not an event global
static EventFunction (*stc_event_function)[EVKIND_NUM] = (void *)0x804a5410;

// Event SIS ID lookup table. Indices 0-15 = vanilla event names, 16-38 = stadium name lookups
// for prediction event (kind 10). Custom entries placed after stadium range.
static int *stc_event_sis_id_table = (int *)0x804a7b98;

// Event state handler dispatch table. 4 entries: [state0, state1, state2, state3].
// Each is a function pointer: void (*)(EventCheckData *).
static void (**stc_event_state_table)(EventCheckData *) = (void (**)(EventCheckData *))0x804a5604;

// Meteor event globals (r13-relative). Used by BehaviorInit helpers to read zone/speed data.
// stc_meteor_data is checked non-null as guard; stc_meteor_event_data holds zone table (+0x0C) and speed table (+0x04).
static volatile int *stc_meteor_data = (volatile int *)(0x805dd0e0 + 0x650);              // 0x805dd730
static volatile int *stc_meteor_event_data = (volatile int *)(0x805dd0e0 + 0x654);        // 0x805dd734
static volatile int *stc_meteor_spawn_count = (volatile int *)(0x805dd0e0 + 0x658);       // 0x805dd738

void CityEvent_ModifyItemFallDesc(EventKind evkind);
void CityEvent_ShowHudText(int event_kind, int hud_display_frames); // 0x80113fb4 - gates on IsInCity||IsInStadium, then calls stadiumPrediction
void CityEvent_SetSisText(int sis_text_index);                     // 0x801169fc - creates/replaces event HUD text object with SIS entry at given index. Stores text at Gm_Get3dData()+0x48. Pass -1 to destroy only.
void *Event_GetInstanceData(EventCheckData *ev_chk); // table lookup: data[0x04][cur_kind * 20 + 16]. returns event instance data pointer
void event_fakeItems_applyEffect(void *event_data); // applies fake item effect using the event data pointer
// Returns 1 while a legendary-machine (Dragoon/Hydra) assembly cinematic is
// running. Reads GameData+0xA8C, the active assembly cinematic GObj pointer:
// set by LegendaryMachine_StartAssembly and cleared to 0 when the cinematic
// completes. NOT a stadium check - Machine_OnTouchItem uses it to suppress
// certain item pickups during the cutscene. For "in a stadium" use
// CityTrial_IsInStadium(); for "on the open CT map" use Gm_IsInCity().
int Gm_IsLegendaryAssembling(); // 0x8000c934
#endif