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

typedef struct EventCheckData
{
    struct
    {
        struct
        {
            int delay_min;                     // 0x0
            int delay_max;                     // 0x4
            int occur_chance;                  // 0x8
            int skip_chance;                   // 0xc
            int min_time;                            // 0x10, min match time (frames) before events start
            u8 x14[0x4];                             // 0x14
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
        } *event;                                    //
        struct                                       // 0x04, per-event BGM/sky data, 0x14 bytes per EventKind
        {                                            //
            int bgm_file;                            // 0x00, BGM file index for secondary music
            int sky_preset;                          // 0x04, sky preset index (-1 = no change)
            int location_idx;                        // 0x08, index into event location array
            int location_count;                      // 0x0C, number of locations
            void *event_data;                        // 0x10, pointer to event-specific data
        } *bgm_sky;                                  //
    } *data;
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

typedef struct EventGlobal
{
    u8 x0[0x42];
    u8 x42_80 : 1;          // 0x42, 0x80
    u8 x42_40 : 1;          // 0x42, 0x40
    u8 x42_20 : 1;          // 0x42, 0x20
    u8 is_song_playing : 1; // 0x42, 0x10
} EventGlobal;

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
static EventGlobal *stc_event_global = (EventGlobal *)0x80538088;
static EventFunction (*stc_event_function)[EVKIND_NUM] = (void *)0x804a5410;

// Meteor event globals (r13-relative). Used by BehaviorInit helpers to read zone/speed data.
// stc_meteor_data is checked non-null as guard; stc_meteor_event_data holds zone table (+0x0C) and speed table (+0x04).
static volatile int *stc_meteor_data = (volatile int *)(0x805dd0e0 + 0x650);              // 0x805dd730
static volatile int *stc_meteor_event_data = (volatile int *)(0x805dd0e0 + 0x654);        // 0x805dd734
static volatile int *stc_meteor_spawn_count = (volatile int *)(0x805dd0e0 + 0x658);       // 0x805dd738

void CityEvent_ModifyItemFallDesc(EventKind evkind);
void CityEvent_ShowHudText(int event_kind, int hud_display_frames); // 0x80113fb4 — gates on IsInCity||IsInStadium, then calls stadiumPrediction
void CityEvent_SetSisText(int sis_text_index);                     // 0x801169fc — creates/replaces event HUD text object with SIS entry at given index. Stores text at Gm_Get3dData()+0x48. Pass -1 to destroy only.
void *Event_GetInstanceData(EventCheckData *ev_chk); // table lookup: data[0x04][cur_kind * 20 + 16]. returns event instance data pointer
void event_fakeItems_applyEffect(void *event_data); // applies fake item effect using the event data pointer
int Gm_IsStadiumMode(); // returns 1 if in stadium mode, used as gate check for copy abilities in Machine_OnTouchItem
#endif