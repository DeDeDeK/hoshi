#ifndef KAR_H_GAME
#define KAR_H_GAME

#include "audio.h"
#include "structs.h"
#include "datatypes.h"
#include "os.h"
#include "scene.h"
#include "obj.h"
#include "hsd.h"
#include "stadium.h"
#include "rider.h"
#include "machine.h"
#include "stage.h"
#include "event.h"
#include "item.h"
#include "menu.h"

//////////////////////
// Enum Definitions //
//////////////////////

typedef enum GameEntity
{
    GAMEENTITY_MACHINE = 16,
    GAMEENTITY_RIDER,
} GameEntity;

typedef enum GamePLink
{
    GAMEPLINK_SYS,
    GAMEPLINK_1,
    GAMEPLINK_CITYEVENTSPAWN,
    GAMEPLINK_3,
    GAMEPLINK_CITYITEMSPAWN,
    GAMEPLINK_CITYMACHINESPAWN,
    GAMEPLINK_6, // effect probably
    GAMEPLINK_STAGE,
    GAMEPLINK_YAKUMONO,
    GAMEPLINK_MACHINE,
    GAMEPLINK_RIDER,
    GAMEPLINK_11,
    GAMEPLINK_ENEMY,   // dyna blade, tac, meteor, etc
    GAMEPLINK_ITEM,         // anything in ItemKind
    GAMEPLINK_PROJECTILE,   // bomb, plasma bullet, flame, firework etc
    GAMEPLINK_SHADOW,       // circular shadow beneath most objects
    GAMEPLINK_EFFECTMODEL,
    GAMEPLINK_CAMWORLD,
    GAMEPLINK_18,
    GAMEPLINK_CAMHUD,
    GAMEPLINK_20,
    GAMEPLINK_CAMDEBUG,
    GAMEPLINK_AUDIO,
    GAMEPLINK_23,
    GAMEPLINK_24,
    GAMEPLINK_25,
    GAMEPLINK_HUD,
    GAMEPLINK_PAUSEHUD,
    GAMEPLINK_CARDCAM, // renders memcard save prompt
} GamePLink;

typedef enum GameGXLink
{
    GAMEGX_HUDORTHO = 18,
    GAMEGX_HUDMAP,
    GAMEGX_HUDMAPDOTS,
    GAMEGX_HUD,
} GameGXLink;

typedef enum PKind
{
    PKIND_HMN,
    PKIND_CPU,
    PKIND_DEMO,
    PKIND_BOSS,
    PKIND_NONE,
} PKind;

typedef enum AirRideMode
{
    AIRRIDEMODE_RACE,
    AIRRIDEMODE_TIME,
    AIRRIDEMODE_FREE,
} AirRideMode;

typedef enum CityMode
{
    CITYMODE_TRIAL,
    CITYMODE_STADIUM,
    CITYMODE_FREERUN,
} CityMode;

typedef enum TopRideMode
{
    TOPRIDEMODE_RACE,
    TOPRIDEMODE_TIME,
    TOPRIDEMODE_FREE,
} TopRideMode;

typedef enum GameMode
{
    GMMODE_AIRRIDE,
    GMMODE_TOPRIDE,
    GMMODE_CITYTRIAL,
    GMMODE_NUM,
} GameMode;

typedef enum PlayerCamFlag
{
    PLYCAM_OFF, 
    PLYCAM_ON, 
    PLYCAM_LIVE, 
    PLYCAM_REPLAY, 
} PlayerCamFlag;

typedef enum GmIntroState
{
    GMINTRO_END,
    GMINTRO_1,
    GMINTRO_FLYOVER,
    GMINTRO_COUNTDOWN,
} GmIntroState;

typedef enum PauseKind
{
    PAUSEKIND_SYS,      // debug pause (uses Z to frame advance)
    PAUSEKIND_GAME,     // match pause (i dont think any other scene uses this?) allows p_links 0(sys),2,16,18(matchcam),19(misccam),20(hudcam),21(coincam),22(screenflashcam),24(devtext)+ to run
    PAUSEKIND_2,        // unknown what uses this, it blacklists everything
    PAUSEKIND_MATCHEND, // is used when the match ends, it allows p_links 0(sys),2,12(effect2),13(mapmisc),14(misc),15(hud),16,17,18(matchcam),19,20,21,22,24+ to run
    PAUSEKIND_EXPLODE,  // used when a machine blows up. allows 
    PAUSEKIND_5,        //
    PAUSEKIND_6,        //
    PAUSEKIND_7,        //
    PAUSEKIND_8,        //
} PauseKind;

typedef enum AirRideClearKind
{
    AR_CLEAR_RACE_100_LAPS,                    // 0   Race over 100 laps!
    AR_CLEAR_RACE_300_LAPS,                    // 1   Race over 300 laps!
    AR_CLEAR_GLIDE_30_MIN,                     // 2   Glide for more than 30 minutes!
    AR_CLEAR_GLIDE_1_HOUR,                     // 3   Glide for more than 1 hour!
    AR_CLEAR_DEFEAT_300_ENEMIES,               // 4   Defeat over 300 of your enemies!
    AR_CLEAR_DEFEAT_1000_ENEMIES,              // 5   Defeat over 1,000 of your enemies!
    AR_CLEAR_SWALLOW_CHILLY_1ST,               // 6   Swallow Chilly 3+ times and take 1st!
    AR_CLEAR_SWALLOW_PLASMA_1ST,               // 7   Swallow Plasma Wisp 3+ times and take 1st!
    AR_CLEAR_SWALLOW_SWORD_1ST,                // 8   Swallow Sword Knight 3+ times and take 1st!
    AR_CLEAR_SWALLOW_WHEELIE_1ST,              // 9   Swallow Wheelie 3+ times and take 1st!
    AR_CLEAR_GOAL_3_TIMES,                     // 10  Reach the goal a total of 3 times (not Free Run)!
    AR_CLEAR_CHECKER_5500FT_2MIN,              // 11  CHECKER KNIGHTS Race over 5,500ft in 2 min!
    AR_CLEAR_MAGMA_4800FT_2MIN,                // 12  MAGMA FLOWS Race over 4,800ft in 2 min!
    AR_CLEAR_MAGMA_2LAPS_2_20,                 // 13  MAGMA FLOWS Finish 2 laps in under 02:20!
    AR_CLEAR_SWALLOW_200_ENEMIES,              // 14  Swallow 200 or more enemies!
    AR_CLEAR_DEFEAT_100_EXHALED,               // 15  Defeat 100+ enemies with exhaled stars!
    AR_CLEAR_SWALLOW_5_GARBAGE_1ST,            // 16  Swallow 5 consecutive garbage enemies and take 1st!
    AR_CLEAR_BEANSTALK_2LAPS_2_18,             // 17  BEANSTALK PARK Finish 2 laps in under 02:18!
    AR_CLEAR_PASSAGE_2LAPS_2_10,               // 18  MACHINE PASSAGE Finish 2 laps in under 02:10!
    AR_CLEAR_CHECKER_2LAPS_3_05,               // 19  CHECKER KNIGHTS Finish 2 laps in under 03:05!
    AR_CLEAR_MEADOWS_4500FT_2MIN,              // 20  FANTASY MEADOWS Race over 4,500ft in 2 min!
    AR_CLEAR_CELESTIAL_6000FT_2MIN,            // 21  CELESTIAL VALLEY Race over 6,000ft in 2 min!
    AR_CLEAR_SKYSANDS_4000FT_2MIN,             // 22  SKY SANDS Race over 4,000ft in 2 min!
    AR_CLEAR_HILLSIDE_5300FT_2MIN,             // 23  FROZEN HILLSIDE Race over 5,300ft in 2 min!
    AR_CLEAR_FILL_100_BLOCKS,                  // 24  Fill in over 100 Checklist blocks!
    AR_CLEAR_MEADOWS_3LAPS_1_20,               // 25  FANTASY MEADOWS Finish 3 laps in under 01:20!
    AR_CLEAR_CELESTIAL_2LAPS_2_20,             // 26  CELESTIAL VALLEY Finish 2 laps in under 02:20!
    AR_CLEAR_SKYSANDS_2LAPS_2_05,              // 27  SKY SANDS Finish 2 laps in under 02:05!
    AR_CLEAR_SWORD_10_SWINGS_1ST,              // 28  Swing your sword exactly 10 times and take 1st!
    AR_CLEAR_HILLSIDE_2LAPS_2_20,              // 29  FROZEN HILLSIDE Finish 2 laps in under 02:20!
    AR_CLEAR_TORNADO_15_ENEMIES_1ST,           // 30  Defeat 15+ enemies as Tornado and finish 1st!
    AR_CLEAR_QUICKSPIN_10_ENEMIES,             // 31  Defeat 10+ enemies using the Quick Spin!
    AR_CLEAR_QUICKSPIN_20_RIVALS,              // 32  Hit 20+ rivals with your Quick Spin!
    AR_CLEAR_PASSAGE_4500FT_2MIN,              // 33  MACHINE PASSAGE Race over 4,500ft in 2 min!
    AR_CLEAR_BEANSTALK_5500FT_2MIN,            // 34  BEANSTALK PARK Race over 5,500ft in 2 min!
    AR_CLEAR_MEADOWS_3LAPS_1_03,               // 35  FANTASY MEADOWS Finish 3 laps in under 01:03!
    AR_CLEAR_CELESTIAL_2LAPS_1_56,             // 36  CELESTIAL VALLEY Finish 2 laps in under 01:56!
    AR_CLEAR_SKYSANDS_2LAPS_1_45,              // 37  SKY SANDS Finish 2 laps in under 01:45!
    AR_CLEAR_HILLSIDE_2LAPS_1_56,              // 38  FROZEN HILLSIDE Finish 2 laps in under 01:56!
    AR_CLEAR_MAGMA_2LAPS_2_01,                 // 39  MAGMA FLOWS Finish 2 laps in under 02:01!
    AR_CLEAR_BEANSTALK_2LAPS_1_56,             // 40  BEANSTALK PARK Finish 2 laps in under 01:56!
    AR_CLEAR_PASSAGE_2LAPS_1_48,               // 41  MACHINE PASSAGE Finish 2 laps in under 01:48!
    AR_CLEAR_CHECKER_2LAPS_2_40,               // 42  CHECKER KNIGHTS Finish 2 laps in under 02:40!
    AR_CLEAR_TA_MEADOWS_1_12,                  // 43  TA: FANTASY MEADOWS in under 01:12!
    AR_CLEAR_TA_MEADOWS_1_00,                  // 44  TA: FANTASY MEADOWS in under 01:00!
    AR_CLEAR_TA_CELESTIAL_3_20,                // 45  TA: CELESTIAL VALLEY in under 03:20!
    AR_CLEAR_TA_CELESTIAL_2_56,                // 46  TA: CELESTIAL VALLEY in under 02:56!
    AR_CLEAR_TA_SKYSANDS_3_10,                 // 47  TA: SKY SANDS in under 03:10!
    AR_CLEAR_TA_SKYSANDS_2_40,                 // 48  TA: SKY SANDS in under 02:40!
    AR_CLEAR_TA_HILLSIDE_3_14,                 // 49  TA: FROZEN HILLSIDE in under 03:14!
    AR_CLEAR_TA_HILLSIDE_2_50,                 // 50  TA: FROZEN HILLSIDE in under 02:50!
    AR_CLEAR_TA_MAGMA_3_20,                    // 51  TA: MAGMA FLOWS in under 03:20!
    AR_CLEAR_TA_MAGMA_3_04,                    // 52  TA: MAGMA FLOWS in under 03:04!
    AR_CLEAR_TA_BEANSTALK_3_10,                // 53  TA: BEANSTALK PARK in under 03:10!
    AR_CLEAR_TA_BEANSTALK_2_55,                // 54  TA: BEANSTALK PARK in under 02:55!
    AR_CLEAR_TA_PASSAGE_3_10,                  // 55  TA: MACHINE PASSAGE in under 03:10!
    AR_CLEAR_TA_PASSAGE_2_48,                  // 56  TA: MACHINE PASSAGE in under 02:48!
    AR_CLEAR_TA_CHECKER_4_30,                  // 57  TA: CHECKER KNIGHTS in under 04:30!
    AR_CLEAR_TA_CHECKER_4_00,                  // 58  TA: CHECKER KNIGHTS in under 04:00!
    AR_CLEAR_FR_MEADOWS_0_24,                  // 59  FR: FANTASY MEADOWS 1 lap in under 00:24!
    AR_CLEAR_FR_MEADOWS_0_21,                  // 60  FR: FANTASY MEADOWS 1 lap in under 00:21!
    AR_CLEAR_FR_MEADOWS_0_23_WAGON,            // 61  FR: FANTASY MEADOWS under 00:23 on Wagon Star!
    AR_CLEAR_FR_CELESTIAL_1_10,                // 62  FR: CELESTIAL VALLEY 1 lap in under 01:10!
    AR_CLEAR_FR_CELESTIAL_0_57,                // 63  FR: CELESTIAL VALLEY 1 lap in under 00:57!
    AR_CLEAR_FR_CELESTIAL_1_02_SLICK,          // 64  FR: CELESTIAL VALLEY under 01:02 on Slick Star!
    AR_CLEAR_FR_SKYSANDS_1_05,                 // 65  FR: SKY SANDS 1 lap in under 01:05!
    AR_CLEAR_FR_SKYSANDS_0_53,                 // 66  FR: SKY SANDS 1 lap in under 00:53!
    AR_CLEAR_FR_SKYSANDS_1_05_BULK,            // 67  FR: SKY SANDS under 01:05 on Bulk Star!
    AR_CLEAR_FR_HILLSIDE_1_10,                 // 68  FR: FROZEN HILLSIDE 1 lap in under 01:10!
    AR_CLEAR_FR_HILLSIDE_0_58,                 // 69  FR: FROZEN HILLSIDE 1 lap in under 00:58!
    AR_CLEAR_FR_HILLSIDE_1_10_FORMULA,         // 70  FR: FROZEN HILLSIDE under 01:10 on Formula Star!
    AR_CLEAR_FR_MAGMA_1_10,                    // 71  FR: MAGMA FLOWS 1 lap in under 01:10!
    AR_CLEAR_FR_MAGMA_1_01,                    // 72  FR: MAGMA FLOWS 1 lap in under 01:01!
    AR_CLEAR_FR_MAGMA_1_02_TURBO,              // 73  FR: MAGMA FLOWS under 01:02 on Turbo Star!
    AR_CLEAR_FR_BEANSTALK_1_07,                // 74  FR: BEANSTALK PARK 1 lap in under 01:07!
    AR_CLEAR_FR_BEANSTALK_0_58,                // 75  FR: BEANSTALK PARK 1 lap in under 00:58!
    AR_CLEAR_FR_BEANSTALK_0_58_WINGED,         // 76  FR: BEANSTALK PARK under 00:58 on Winged Star!
    AR_CLEAR_FR_PASSAGE_1_05,                  // 77  FR: MACHINE PASSAGE 1 lap in under 01:05!
    AR_CLEAR_FR_PASSAGE_0_56,                  // 78  FR: MACHINE PASSAGE 1 lap in under 00:56!
    AR_CLEAR_FR_PASSAGE_0_57_SWERVE,           // 79  FR: MACHINE PASSAGE under 00:57 on Swerve Star!
    AR_CLEAR_FR_CHECKER_1_35,                  // 80  FR: CHECKER KNIGHTS 1 lap in under 01:35!
    AR_CLEAR_FR_CHECKER_1_20,                  // 81  FR: CHECKER KNIGHTS 1 lap in under 01:20!
    AR_CLEAR_FR_CHECKER_1_25_ROCKET,           // 82  FR: CHECKER KNIGHTS under 01:25 on Rocket Star!
    AR_CLEAR_LAPTIME_SAME_DIGITS,              // 83  Make your lap time's last two digits the same!
    AR_CLEAR_RACE_ALL_COURSES,                 // 84  Race all of the standard Air Ride courses!
    AR_CLEAR_4TH_TO_1ST,                       // 85  Start final lap in 4th, move to 1st to win!
    AR_CLEAR_FINISH_1ST_SPINNING,              // 86  Cross finish line while spinning and take 1st!
    AR_CLEAR_FINISH_1ST_DAMAGED,               // 87  Finish in 1st place while taking damage!
    AR_CLEAR_FINISH_1ST_FLYING,                // 88  Finish in 1st place while flying!
    AR_CLEAR_FINISH_1ST_SLEEP,                 // 89  Finish in 1st place with Sleep ability!
    AR_CLEAR_FINISH_1ST_FIRE,                  // 90  Finish in 1st place with Fire ability!
    AR_CLEAR_FINISH_1ST_NEEDLE,                // 91  Finish in 1st place with Needle ability!
    AR_CLEAR_FINISH_1ST_WING,                  // 92  Finish in 1st place with Wing ability!
    AR_CLEAR_TA_MEADOWS_1_05_SLICK,            // 93  TA: FANTASY MEADOWS under 01:05 on Slick Star!
    AR_CLEAR_DROP_CLIFFS_3_TIMES,              // 94  Drop from the cliffs 3 times in one game!
    AR_CLEAR_MEADOWS_SWALLOW_20_1ST,           // 95  FANTASY MEADOWS: Swallow 20+ enemies and take 1st!
    AR_CLEAR_MEADOWS_1LAP_ABOVE_20MPH,         // 96  FANTASY MEADOWS: 1 lap without dropping below 20 mph!
    AR_CLEAR_BEANSTALK_3LAPS_NO_WHEEL,         // 97  BEANSTALK PARK: 3+ laps without riding the Ferris wheel!
    AR_CLEAR_BEANSTALK_SWALLOW_20_1ST,         // 98  BEANSTALK PARK: Swallow 20+ enemies and take 1st!
    AR_CLEAR_CHECKER_7_SPINPANELS_1ST,         // 99  CHECKER KNIGHTS: Use spin panels 7+ times and take 1st!
    AR_CLEAR_CHECKER_2_WALLS_1ST,              // 100 CHECKER KNIGHTS: Break 2+ walls and finish 1st!
    AR_CLEAR_CHECKER_SWALLOW_20_1ST,           // 101 CHECKER KNIGHTS: Swallow 20+ enemies and finish 1st!
    AR_CLEAR_HILLSIDE_20_ICE_1ST,              // 102 FROZEN HILLSIDE: Split 20+ ice platforms and finish 1st!
    AR_CLEAR_SKYSANDS_BREAK_CORAL_1ST,         // 103 SKY SANDS: Break all coral and finish 1st!
    AR_CLEAR_SKYSANDS_3_QUICKSAND_1ST,         // 104 SKY SANDS: Enter quicksand 3 times and finish 1st!
    AR_CLEAR_TA_CELESTIAL_2_58_JET,            // 105 TA: CELESTIAL VALLEY under 02:58 on Jet Star!
    AR_CLEAR_SKYSANDS_3_TRAPDOOR_1ST,          // 106 SKY SANDS: Open trapdoor exactly 3 times and finish 1st!
    AR_CLEAR_PASSAGE_3_FROM_CANNON,            // 107 MACHINE PASSAGE: Shoot 3 from the cannon at once!
    AR_CLEAR_TA_SKYSANDS_2_40_WAGON,           // 108 TA: SKY SANDS under 02:40 on Wagon Star!
    AR_CLEAR_PASSAGE_1ST_NO_WALLS,             // 109 MACHINE PASSAGE: Finish 1st without touching walls!
    AR_CLEAR_MAGMA_ALL_RAILS_1ST,              // 110 MAGMA FLOWS: Use all volcano rails and finish 1st!
    AR_CLEAR_MAGMA_BUMP_DRAGON,                // 111 MAGMA FLOWS: Bump into a flaming dragon!
    AR_CLEAR_MAGMA_ALL_BOOSTS_1ST,             // 112 MAGMA FLOWS: Use all Boost Panels and finish 1st!
    AR_CLEAR_TA_HILLSIDE_3_10_TURBO,           // 113 TA: FROZEN HILLSIDE under 03:10 on Turbo Star!
    AR_CLEAR_CELESTIAL_BOTH_RAILINGS,          // 114 CELESTIAL VALLEY: Ride both bridge railings!
    AR_CLEAR_TA_MAGMA_3_15_SHADOW,             // 115 TA: MAGMA FLOWS under 03:15 on Shadow Star!
    AR_CLEAR_CELESTIAL_COPY_WHEEL_TREE,        // 116 CELESTIAL VALLEY: Use Copy Chance Wheel on tree!
    AR_CLEAR_TA_BEANSTALK_3_00_ROCKET,         // 117 TA: BEANSTALK PARK under 03:00 on Rocket Star!
    AR_CLEAR_TA_PASSAGE_2_50_REX,              // 118 TA: MACHINE PASSAGE under 02:50 on Rex Wheelie!
    AR_CLEAR_TA_CHECKER_3_55_WARPSTAR,         // 119 TA: CHECKER KNIGHTS under 03:55 on Warpstar!
    AR_CLEAR_NUM = 120,
} AirRideClearKind;

typedef enum TopRideClearKind
{
    TR_CLEAR_GOAL_20_TIMES,                    // 0   Cross the goal 20 or more times!
    TR_CLEAR_RACE_300_LAPS,                    // 1   Race over 300 laps!
    TR_CLEAR_10_MULTI_RACES,                   // 2   Compete in more than 10 multiplayer races!
    TR_CLEAR_50_MULTI_RACES,                   // 3   Compete in more than 50 multiplayer races!
    TR_CLEAR_FR_100_LAPS,                      // 4   Free Run: Race more than 100 laps!
    TR_CLEAR_TA_GOAL_30_TIMES,                 // 5   Time Attack: Cross the goal 30 or more times!
    TR_CLEAR_1ST_ALL_COURSES,                  // 6   Take 1st place on all courses!
    TR_CLEAR_1LAP_NO_WALL_1ST,                 // 7   Race one lap without hitting a wall and finish 1st!
    TR_CLEAR_20_QUICKSPINS_1ST,                // 8   Do 20+ Quick Spins in one lap and finish 1st!
    TR_CLEAR_ALL_COURSES_NO_ITEMS,             // 9   Complete all courses without using items!
    TR_CLEAR_1ST_ALL_NO_ITEMS,                 // 10  Finish 1st on all courses using no items!
    TR_CLEAR_COLLECT_500_ITEMS,                // 11  Collect 500 items or more!
    TR_CLEAR_ALL_NO_BOOST,                     // 12  Finish all courses without using Boost!
    TR_CLEAR_1ST_ALL_NO_BOOST,                 // 13  Finish 1st on all courses without Boost!
    TR_CLEAR_SAME_ITEM_3_TIMES,                // 14  Get the same item 3 times in one race!
    TR_CLEAR_1ST_QUICKSPIN,                    // 15  Take 1st place while doing a Quick Spin!
    TR_CLEAR_1ST_HAMMER,                       // 16  Take 1st place while holding the Hammer!
    TR_CLEAR_1ST_1LAP_AHEAD,                   // 17  Finish 1st with 1 lap between you and #2!
    TR_CLEAR_1ST_2LAPS_AHEAD,                  // 18  Finish 1st with 2 laps between you and #2!
    TR_CLEAR_20_SPINNER_ITEMS,                 // 19  Get more than 20 Spinner items!
    TR_CLEAR_20_INVINCIBLE_CANDY,              // 20  Get more than 20 Invincible Candy items!
    TR_CLEAR_20_WALKY_ITEMS,                   // 21  Get more than 20 Walky items!
    TR_CLEAR_TORCH_3_WITH_FIRE,                // 22  Torch 3+ rivals using one Fire item!
    TR_CLEAR_3_RIVALS_BUZZ_SAW,                // 23  Send 3+ rivals sailing using one Buzz Saw item!
    TR_CLEAR_HIT_3_BOMB_ITEMS,                 // 24  Hit enemies 3+ times with Bomb items in one game!
    TR_CLEAR_18_ITEM_TYPES,                    // 25  Get over 18 different types of items!
    TR_CLEAR_GRASS_NO_ITEMS_1ST,               // 26  GRASS: Take 1st without using items!
    TR_CLEAR_GRASS_NO_BOOST_1ST,               // 27  GRASS: Take 1st without using Boost!
    TR_CLEAR_GRASS_CPU5_1ST,                   // 28  GRASS: Finish 1st with CPUs set to level 5!
    TR_CLEAR_GRASS_1ST_10_TIMES,               // 29  GRASS: Take 1st 10 or more times!
    TR_CLEAR_GRASS_7LAPS_0_43,                 // 30  GRASS: Finish 7 laps in under 00:43!
    TR_CLEAR_GRASS_100_LAPS,                   // 31  GRASS: Race more than 100 laps!
    TR_CLEAR_GRASS_5_DASH_PANELS_1ST,          // 32  GRASS: Finish 1st and hit 5+ Dash Panels!
    TR_CLEAR_GRASS_30_TREE_BOMBS,              // 33  GRASS: Drop 30+ tree bombs in one race!
    TR_CLEAR_GRASS_1ST_5SEC_AHEAD,             // 34  GRASS: Finish 1st 5 seconds faster than #2!
    TR_CLEAR_SAND_NO_ITEMS_1ST,                // 35  SAND: Take 1st without using items!
    TR_CLEAR_SAND_NO_BOOST_1ST,                // 36  SAND: Take 1st without using Boost!
    TR_CLEAR_SAND_CPU5_1ST,                    // 37  SAND: Finish 1st with CPUs set to level 5!
    TR_CLEAR_SAND_1ST_10_TIMES,                // 38  SAND: Take 1st 10 or more times!
    TR_CLEAR_SAND_7LAPS_0_52,                  // 39  SAND: Finish 7 laps in under 00:52!
    TR_CLEAR_SAND_100_LAPS,                    // 40  SAND: Race more than 100 laps!
    TR_CLEAR_SAND_CATCH_WORM_3_1ST,            // 41  SAND: Take 1st and catch the worm 3+ times!
    TR_CLEAR_SAND_ANT_DOOM_50,                 // 42  SAND: Drop into Ant Doom 50+ times!
    TR_CLEAR_SAND_ANT_DOOM_20_1GAME,           // 43  SAND: Drop into Ant Doom 20 times in one game!
    TR_CLEAR_SAND_1ST_5SEC_AHEAD,              // 44  SAND: Finish 1st 5 seconds faster than #2!
    TR_CLEAR_SKY_NO_ITEMS_1ST,                 // 45  SKY: Take 1st without using items!
    TR_CLEAR_SKY_NO_BOOST_1ST,                 // 46  SKY: Take 1st without using Boost!
    TR_CLEAR_SKY_CPU5_1ST,                     // 47  SKY: Finish 1st with CPUs set to level 5!
    TR_CLEAR_SKY_1ST_10_TIMES,                 // 48  SKY: Take 1st 10 or more times!
    TR_CLEAR_SKY_6LAPS_1_02,                   // 49  SKY: Finish 6 laps in under 01:02!
    TR_CLEAR_SKY_100_LAPS,                     // 50  SKY: Race more than 100 laps!
    TR_CLEAR_SKY_ISLE_KNOB_5_1ST,              // 51  SKY: Take 1st and hit the Isle Knob 5+ times!
    TR_CLEAR_SKY_NO_JUMP_PLATE_1ST,            // 52  SKY: Finish 1st without using the Jump Plate!
    TR_CLEAR_SKY_1ST_5SEC_AHEAD,               // 53  SKY: Finish 1st 5 seconds faster than #2!
    TR_CLEAR_FIRE_NO_ITEMS_1ST,                // 54  FIRE: Take 1st without using items!
    TR_CLEAR_FIRE_NO_BOOST_1ST,                // 55  FIRE: Take 1st without using Boost!
    TR_CLEAR_FIRE_CPU5_1ST,                    // 56  FIRE: Finish 1st with CPUs set to level 5!
    TR_CLEAR_FIRE_1ST_10_TIMES,                // 57  FIRE: Take 1st 10 or more times!
    TR_CLEAR_FIRE_6LAPS_0_53,                  // 58  FIRE: Finish 6 laps in under 00:53!
    TR_CLEAR_FIRE_100_LAPS,                    // 59  FIRE: Race more than 100 laps!
    TR_CLEAR_FIRE_HUGE_ERUPTION_3,             // 60  FIRE: Cause a huge eruption 3+ times!
    TR_CLEAR_FIRE_1ST_FIRE_ITEM,               // 61  FIRE: Finish 1st while holding the Fire item!
    TR_CLEAR_FIRE_1ST_5SEC_AHEAD,              // 62  FIRE: Finish 1st 5 seconds faster than #2!
    TR_CLEAR_WATER_NO_ITEMS_1ST,               // 63  WATER: Take 1st without using items!
    TR_CLEAR_WATER_NO_BOOST_1ST,               // 64  WATER: Take 1st without using Boost!
    TR_CLEAR_WATER_CPU5_1ST,                   // 65  WATER: Finish 1st with CPUs set to level 5!
    TR_CLEAR_WATER_1ST_10_TIMES,               // 66  WATER: Take 1st 10 or more times!
    TR_CLEAR_WATER_5LAPS_1_02,                 // 67  WATER: Finish 5 laps in under 01:02!
    TR_CLEAR_WATER_100_LAPS,                   // 68  WATER: Race more than 100 laps!
    TR_CLEAR_WATER_FALLS_5_1ST,                // 69  WATER: Take 1st and enter the falls 5+ times!
    TR_CLEAR_WATER_1ST_5SEC_AHEAD,             // 70  WATER: Finish 1st 5 seconds faster than #2!
    TR_CLEAR_LIGHT_NO_ITEMS_1ST,               // 71  LIGHT: Take 1st without using items!
    TR_CLEAR_LIGHT_NO_BOOST_1ST,               // 72  LIGHT: Take 1st without using Boost!
    TR_CLEAR_LIGHT_CPU5_1ST,                   // 73  LIGHT: Finish 1st with CPUs set to level 5!
    TR_CLEAR_LIGHT_1ST_10_TIMES,               // 74  LIGHT: Take 1st 10 or more times!
    TR_CLEAR_LIGHT_6LAPS_0_43,                 // 75  LIGHT: Finish 6 laps in under 00:43!
    TR_CLEAR_LIGHT_100_LAPS,                   // 76  LIGHT: Race more than 100 laps!
    TR_CLEAR_LIGHT_RAIL_50,                    // 77  LIGHT: Ride the grind rail 50+ times!
    TR_CLEAR_LIGHT_RAIL_5_1ST,                 // 78  LIGHT: Take 1st and grind the rail 5+ times!
    TR_CLEAR_LIGHT_6_COLUMNS_1ST,              // 79  LIGHT: Finish 1st and bust 6+ columns!
    TR_CLEAR_LIGHT_1ST_5SEC_AHEAD,             // 80  LIGHT: Finish 1st 5 seconds faster than #2!
    TR_CLEAR_METAL_NO_ITEMS_1ST,               // 81  METAL: Take 1st without using items!
    TR_CLEAR_METAL_NO_BOOST_1ST,               // 82  METAL: Take 1st without using Boost!
    TR_CLEAR_METAL_CPU5_1ST,                   // 83  METAL: Finish 1st with CPUs set to level 5!
    TR_CLEAR_METAL_1ST_10_TIMES,               // 84  METAL: Take 1st 10 or more times!
    TR_CLEAR_METAL_5LAPS_0_58,                 // 85  METAL: Finish 5 laps in under 00:58!
    TR_CLEAR_METAL_100_LAPS,                   // 86  METAL: Race more than 100 laps!
    TR_CLEAR_METAL_NO_GEAR_WALLS_1ST,          // 87  METAL: Take 1st without breaking any gear walls!
    TR_CLEAR_METAL_SWITCH_10_1ST,              // 88  METAL: Take 1st and hit the switch 10+ times!
    TR_CLEAR_METAL_5_GEAR_WALLS_1ST,           // 89  METAL: Take 1st and break 5+ gear walls!
    TR_CLEAR_METAL_1ST_5SEC_AHEAD,             // 90  METAL: Finish 1st 5 seconds faster than #2!
    TR_CLEAR_TA_GRASS_0_33,                    // 91  TA: GRASS in under 00:33!
    TR_CLEAR_TA_SAND_0_35,                     // 92  TA: SAND in under 00:35!
    TR_CLEAR_TA_LIGHT_0_38,                    // 93  TA: LIGHT in under 00:38!
    TR_CLEAR_TA_SKY_0_57,                      // 94  TA: SKY in under 00:57!
    TR_CLEAR_TA_WATER_1_06,                    // 95  TA: WATER in under 01:06!
    TR_CLEAR_TA_FIRE_0_46,                     // 96  TA: FIRE in under 00:46!
    TR_CLEAR_TA_METAL_0_57,                    // 97  TA: METAL in under 00:57!
    TR_CLEAR_TA_GRASS_0_28,                    // 98  TA: GRASS in under 00:28!
    TR_CLEAR_TA_SAND_0_29,                     // 99  TA: SAND in under 00:29!
    TR_CLEAR_TA_LIGHT_0_33,                    // 100 TA: LIGHT in under 00:33!
    TR_CLEAR_TA_SKY_0_47,                      // 101 TA: SKY in under 00:47!
    TR_CLEAR_TA_WATER_0_56,                    // 102 TA: WATER in under 00:56!
    TR_CLEAR_TA_FIRE_0_39,                     // 103 TA: FIRE in under 00:39!
    TR_CLEAR_TA_METAL_0_51,                    // 104 TA: METAL in under 00:51!
    TR_CLEAR_FR_GRASS_0_06,                    // 105 FR: GRASS 1 lap in under 00:06!
    TR_CLEAR_FR_SAND_0_06_50,                  // 106 FR: SAND 1 lap in under 00:06:50!
    TR_CLEAR_FR_LIGHT_0_07_50,                 // 107 FR: LIGHT 1 lap in under 00:07:50!
    TR_CLEAR_FR_SKY_0_11,                      // 108 FR: SKY 1 lap in under 00:11!
    TR_CLEAR_FR_WATER_0_12,                    // 109 FR: WATER 1 lap in under 00:12!
    TR_CLEAR_FR_FIRE_0_08,                     // 110 FR: FIRE 1 lap in under 00:08!
    TR_CLEAR_FR_METAL_0_11_50,                 // 111 FR: METAL 1 lap in under 00:11:50!
    TR_CLEAR_FR_GRASS_0_04_50,                 // 112 FR: GRASS 1 lap in under 00:04:50!
    TR_CLEAR_FR_SAND_0_05,                     // 113 FR: SAND 1 lap in under 00:05!
    TR_CLEAR_FR_LIGHT_0_06,                    // 114 FR: LIGHT 1 lap in under 00:06!
    TR_CLEAR_FR_SKY_0_09,                      // 115 FR: SKY 1 lap in under 00:09!
    TR_CLEAR_FR_WATER_0_10_50,                 // 116 FR: WATER 1 lap in under 00:10:50!
    TR_CLEAR_FR_FIRE_0_06_50,                  // 117 FR: FIRE 1 lap in under 00:06:50!
    TR_CLEAR_FR_METAL_0_09_50,                 // 118 FR: METAL 1 lap in under 00:09:50!
    TR_CLEAR_FILL_100_BLOCKS,                  // 119 Fill in over 100 Checklist blocks!
    TR_CLEAR_NUM = 120,
} TopRideClearKind;

typedef enum CityClearKind
{
    CT_CLEAR_RACE_60_MILES,                    // 0   Race over 60 miles!
    CT_CLEAR_RACE_200_MILES,                   // 1   Race over 200 miles!
    CT_CLEAR_PICKUP_100_ITEMS,                 // 2   Pick up over 100 items!
    CT_CLEAR_PICKUP_500_ITEMS,                 // 3   Pick up over 500 items!
    CT_CLEAR_PICKUP_1000_ITEMS,                // 4   Pick up over 1000 items!
    CT_CLEAR_PICKUP_3000_ITEMS,                // 5   Pick up over 3000 items!
    CT_CLEAR_FR_CHANGE_MACHINE_10,             // 6   FR: Change Air Ride Machines 10+ times!
    CT_CLEAR_BREAK_500_BOXES,                  // 7   Break more than 500 boxes!
    CT_CLEAR_BREAK_1000_BOXES,                 // 8   Break more than 1000 boxes!
    CT_CLEAR_FR_DRIVE_10_MIN,                  // 9   FR: Drive for 10+ minutes!
    CT_CLEAR_FR_DRIVE_30_MIN,                  // 10  FR: Drive for 30+ minutes!
    CT_CLEAR_FR_DRIVE_2_HOURS,                 // 11  FR: Drive for 2+ hours!
    CT_CLEAR_STD_10_STADIUMS,                  // 12  Stadium: Play in 10+ stadium modes!
    CT_CLEAR_STD_20_STADIUMS,                  // 13  Stadium: Play in 20+ stadium modes!
    CT_CLEAR_STD_DR1_0_24,                     // 14  Stadium: DRAG RACE 1 in under 00:24!
    CT_CLEAR_STD_DR1_0_20,                     // 15  Stadium: DRAG RACE 1 in under 00:20!
    CT_CLEAR_STD_DR2_0_24,                     // 16  Stadium: DRAG RACE 2 in under 00:24!
    CT_CLEAR_STD_DR2_0_20,                     // 17  Stadium: DRAG RACE 2 in under 00:20!
    CT_CLEAR_STD_DR3_0_35,                     // 18  Stadium: DRAG RACE 3 in under 00:35!
    CT_CLEAR_STD_DR3_0_27,                     // 19  Stadium: DRAG RACE 3 in under 00:27!
    CT_CLEAR_STD_DR4_0_24,                     // 20  Stadium: DRAG RACE 4 in under 00:24!
    CT_CLEAR_STD_DR4_0_19,                     // 21  Stadium: DRAG RACE 4 in under 00:19!
    CT_CLEAR_STD_HIGH_JUMP_500FT,              // 22  Stadium: HIGH JUMP over 500 feet!
    CT_CLEAR_STD_HIGH_JUMP_1000FT,             // 23  Stadium: HIGH JUMP over 1,000 feet!
    CT_CLEAR_STD_HIGH_JUMP_10SEC,              // 24  Stadium: HIGH JUMP airborne 10+ seconds!
    CT_CLEAR_STD_TARGET_150PTS,                // 25  Stadium: TARGET FLIGHT over 150 points!
    CT_CLEAR_STD_TARGET_90PTS,                 // 26  Stadium: TARGET FLIGHT exactly 90 points!
    CT_CLEAR_STD_TARGET_200PTS,                // 27  Stadium: TARGET FLIGHT perfect 200 points!
    CT_CLEAR_STD_TARGET_15SEC,                 // 28  Stadium: TARGET FLIGHT airborne 15+ seconds!
    CT_CLEAR_STD_TARGET_15_PLAYS,              // 29  Stadium: TARGET FLIGHT play 15+ times!
    CT_CLEAR_STD_TARGET_1500PTS,               // 30  Stadium: TARGET FLIGHT over 1,500 points!
    CT_CLEAR_STD_GLIDER_330FT,                 // 31  Stadium: AIR GLIDER over 330 feet!
    CT_CLEAR_STD_GLIDER_660FT,                 // 32  Stadium: AIR GLIDER over 660 feet!
    CT_CLEAR_STD_GLIDER_1300FT,                // 33  Stadium: AIR GLIDER over 1,300 feet!
    CT_CLEAR_STD_GLIDER_30SEC,                 // 34  Stadium: AIR GLIDER airborne 30+ seconds!
    CT_CLEAR_STD_DD1_KO5,                      // 35  Stadium: DD1 KO rivals 5+ times!
    CT_CLEAR_STD_DD2_KO5,                      // 36  Stadium: DD2 KO rivals 5+ times!
    CT_CLEAR_STD_DD3_KO5,                      // 37  Stadium: DD3 KO rivals 5+ times!
    CT_CLEAR_STD_DD4_KO5,                      // 38  Stadium: DD4 KO rivals 5+ times!
    CT_CLEAR_STD_DD5_KO5,                      // 39  Stadium: DD5 KO rivals 5+ times!
    CT_CLEAR_STD_DD1_BUST_ROCKS,               // 40  Stadium: DD1 bust all rocks on the field!
    CT_CLEAR_STD_DD_KO50,                      // 41  Stadium: DD (All) KO enemies 50+ times!
    CT_CLEAR_STD_DD_KO150,                     // 42  Stadium: DD (All) KO enemies 150+ times!
    CT_CLEAR_STD_KM1_KO50,                     // 43  Stadium: KM1 KO enemies 50+ times!
    CT_CLEAR_STD_KM2_KO30,                     // 44  Stadium: KM2 KO enemies 30+ times!
    CT_CLEAR_STD_KM_KO500,                     // 45  Stadium: KM (All) KO 500+ enemies!
    CT_CLEAR_STD_KM_KO1500,                    // 46  Stadium: KM (All) KO 1,500+ enemies!
    CT_CLEAR_STD_VS_DEDEDE_1MIN,               // 47  Stadium: VS. KING DEDEDE KO in under 1 minute!
    CT_CLEAR_DAMAGE_DYNA_BLADE,                // 48  Do some damage to Dyna Blade!
    CT_CLEAR_TRAMPLED_DYNA_BLADE,              // 49  Get trampled by Dyna Blade!
    CT_CLEAR_BREAK_5_PILLARS,                  // 50  Break 5+ huge pillars!
    CT_CLEAR_BREAK_PILLAR_40SEC,               // 51  Break a huge pillar within 40 seconds!
    CT_CLEAR_STEAL_8_FROM_TAC,                 // 52  Steal 8+ items from Tac!
    CT_CLEAR_METEOR_3_TIMES,                   // 53  Meteor attacks the city 3+ times!
    CT_CLEAR_USE_RESTORATION_AREA,             // 54  Use up one of the restoration areas!
    CT_CLEAR_FILL_100_BLOCKS,                  // 55  Fill in over 100 Checklist blocks!
    CT_CLEAR_ENTER_CASTLE_CHAMBER,             // 56  Go into the castle chamber when it opens!
    CT_CLEAR_FLY_RINGS_5_TIMES,                // 57  Fly through the rings in the sky 5+ times!
    CT_CLEAR_BUST_STAR_POLE,                   // 58  Bust the star pole!
    CT_CLEAR_BUST_STAR_POLE_10,                // 59  Bust the star pole 10+ times!
    CT_CLEAR_OPEN_VOLCANO_HOLES,               // 60  Open all holes around the volcano base!
    CT_CLEAR_WATERWHEEL_10_TIMES,              // 61  Let the waterwheel carry you 10+ times!
    CT_CLEAR_GARDEN_IN_SKY,                    // 62  Make your way to the garden in the sky!
    CT_CLEAR_OPEN_FOREST_PITFALL,              // 63  Open up the pitfall in the forest!
    CT_CLEAR_HIGH_PLAINS_HOLE_3,               // 64  Go into the high plains hole 3+ times!
    CT_CLEAR_BREAK_ALL_ROCKS,                  // 65  Break all volcano rocks and high plains rocks!
    CT_CLEAR_GRIND_RAIL_CRATER,                // 66  Use the grind rail to break into the crater!
    CT_CLEAR_JUMP_BUILDING_10,                 // 67  Jump on top of building 10+ times via super jump ramp!
    CT_CLEAR_DESTROY_ALL_HOUSES,               // 68  Destroy all dilapidated houses!
    CT_CLEAR_KNOCK_DOWN_TREES,                 // 69  Knock down all trees in the forest!
    CT_CLEAR_COPY_WHEEL_BOMB,                  // 70  Get Bomb ability from Copy Chance Wheel!
    CT_CLEAR_COPY_WHEEL_SLEEP,                 // 71  Get Sleep ability from Copy Chance Wheel!
    CT_CLEAR_10_ITEMS_20SEC,                   // 72  Get 10 items within the first 20 seconds!
    CT_CLEAR_DAMAGE_RIVAL_10SEC,               // 73  Do damage to a rival within the first 10 seconds!
    CT_CLEAR_ALL_OFF_MACHINES,                 // 74  All players simultaneously off their machines!
    CT_CLEAR_TIMEOUT_ALL_OFF,                  // 75  Time runs out while all players are off machines!
    CT_CLEAR_TIMEOUT_ALL_RAILS,                // 76  Time runs out while all players are on rails!
    CT_CLEAR_BREAK_5_CPU_MACHINES,             // 77  Break a CPU's machine 5+ times in the city!
    CT_CLEAR_DAMAGE_ALL_3_CPUS,                // 78  Damage all 3 CPUs in one race!
    CT_CLEAR_GET_50_ITEMS,                     // 79  Get 50+ items in one game!
    CT_CLEAR_GET_10_BOOST_PATCHES,             // 80  Get 10+ Boost patches in one game!
    CT_CLEAR_STD_DD1_KO10,                     // 81  Stadium: DD1 KO a rival 10+ times!
    CT_CLEAR_GET_10_TOPSPD_PATCHES,            // 82  Get 10+ Top Speed Patches in one game!
    CT_CLEAR_STD_DD2_KO10,                     // 83  Stadium: DD2 KO a rival 10+ times!
    CT_CLEAR_GET_10_TURN_PATCHES,              // 84  Get 10+ Turn Patches in one game!
    CT_CLEAR_STD_DD4_KO10,                     // 85  Stadium: DD4 KO a rival 10+ times!
    CT_CLEAR_GET_10_CHARGE_PATCHES,            // 86  Get 10+ Charge Patches in one game!
    CT_CLEAR_STD_DD5_KO10,                     // 87  Stadium: DD5 KO a rival 10+ times!
    CT_CLEAR_GET_10_WEIGHT_PATCHES,            // 88  Get 10+ Weight Patches in one game!
    CT_CLEAR_STD_KM1_KO75,                     // 89  Stadium: KM1 KO 75+ enemies solo!
    CT_CLEAR_GET_10_DEFENSE_PATCHES,           // 90  Get 10+ Defense Patches in one game!
    CT_CLEAR_STD_KM2_KO40,                     // 91  Stadium: KM2 KO 40+ enemies solo!
    CT_CLEAR_GET_10_GLIDE_PATCHES,             // 92  Get 10+ Glide Patches in one game!
    CT_CLEAR_GET_30_GLIDE_PATCHES,             // 93  Get 30+ Glide Patches!
    CT_CLEAR_SENSOR_BOMB_KO3,                  // 94  KO rivals 3+ times with Sensor Bombs!
    CT_CLEAR_GOLD_SPIKES_KO3,                  // 95  KO rivals 3+ times with Gold Spikes!
    CT_CLEAR_FIREWORKS_KO10,                   // 96  KO rivals 10+ times with Fireworks!
    CT_CLEAR_EAT_2_MAXIM_TOMATOES,             // 97  Eat 2+ maxim tomatoes in one game!
    CT_CLEAR_DRINK_3_ENERGY_DRINKS,            // 98  Drink 3+ energy drinks in one game!
    CT_CLEAR_STD_DR1_0_26_WARPSTAR,            // 99  Stadium: DR1 under 00:26 on Warpstar!
    CT_CLEAR_STD_DR1_0_17_FORMULA,             // 100 Stadium: DR1 under 00:17 on Formula Star!
    CT_CLEAR_STD_DR2_0_27_WAGON,               // 101 Stadium: DR2 under 00:27 on Wagon Star!
    CT_CLEAR_STD_DR2_0_29_WINGED,              // 102 Stadium: DR2 under 00:29 on Winged Star!
    CT_CLEAR_STD_DR3_0_28_SWERVE,              // 103 Stadium: DR3 under 00:28 on Swerve Star!
    CT_CLEAR_STD_DR3_0_31_WHEELIE,             // 104 Stadium: DR3 under 00:31 on Wheelie Bike!
    CT_CLEAR_STD_DR4_0_33_TURBO,               // 105 Stadium: DR4 under 00:33 on Turbo Star!
    CT_CLEAR_STD_DR4_0_24_REX,                 // 106 Stadium: DR4 under 00:24 on Rex Wheelie!
    CT_CLEAR_EAT_3_SUSHI,                      // 107 Eat 3+ plates of sushi in one race!
    CT_CLEAR_EAT_3_HOT_DOGS,                   // 108 Eat 3+ Hot Dogs in one race!
    CT_CLEAR_UNLOCK_DRAGOON,                   // 109 Unlock Dragoon Parts A, B, and C!
    CT_CLEAR_UNLOCK_HYDRA,                     // 110 Unlock Hydra Parts X, Y, and Z!
    CT_CLEAR_BUST_SCOOTER_ON_COMPACT,          // 111 Bust Wheelie Scooter while riding Compact Star!
    CT_CLEAR_BUST_BIKE_ON_WARPSTAR,            // 112 Bust Wheelie Bike while riding Warpstar!
    CT_CLEAR_BUST_SWERVE_ON_BIKE,              // 113 Bust Swerve Star while riding Wheelie Bike!
    CT_CLEAR_BUST_WARPSTAR_ON_SWERVE,          // 114 Bust Warpstar while riding Swerve Star!
    CT_CLEAR_BUST_FORMULA_ON_TURBO,            // 115 Bust Formula Star while riding Turbo Star!
    CT_CLEAR_BUST_SLICK_ON_FORMULA,            // 116 Bust Slick Star while riding Formula Star!
    CT_CLEAR_BUST_ROCKET_ON_SLICK,             // 117 Bust Rocket Star while riding Slick Star!
    CT_CLEAR_BUST_TURBO_ON_ROCKET,             // 118 Bust Turbo Star while riding Rocket Star!
    CT_CLEAR_COMPLETE_DRAGOON_AND_HYDRA,       // 119 Complete both Dragoon and Hydra in one match!
    CT_CLEAR_NUM = 120,
} CityClearKind;

///////////////////////
// Struct Defintions //
///////////////////////

typedef struct TitleScreenData
{
    u8 state;      // 0xc
    u8 xd;         // 0xd
    u16 timer;     // 0xe
    int x10;       // 0x10
    u8 is_skip_op; // 0x14, bool to skip the opening movie when loading the title screen
    u8 x15;        // 0x15
    u8 x16;        // 0x16
    u8 x17;        // 0x17
} TitleScreenData;

typedef struct PlayerDesc
{
    u8 p_kind;                   // 0x00
    u8 rider_kind;               // 0x01
    u8 is_bike;                  // 0x02
    u8 machine_kind;             // 0x03
    u8 color;                    // 0x04
    u8 rumble;                   // 0x05
    u8 ply;                      // 0x06
    s8 x7;                       // 0x07
    u8 cpu_level;                // 0x08
    s8 x9;                       // 0x09
    u8 xa;                       // 0x0a
    u8 xb;                       // 0x0b
    int xc;                      // 0x0c
    int x10;                     // 0x10
    int x14;                     // 0x14
    int x18;                     // 0x18
    int x1c;                     // 0x1c
    int x20;                     // 0x20
    int x24;                     // 0x24
    int x28;                     // 0x28
    int x2c;                     // 0x2c
} PlayerDesc;

typedef struct GameData // 805359d8
{
    int x0;                            // 0x0
    int x4;                            // 0x4
    int x8;                            // 0x8
    TitleScreenData title_data;        // 0xc
    int x18;                           // 0x18
    int x1c;                           // 0x1c
    int x20;                           // 0x20
    int x24;                           // 0x24
    u8 x28;                            // 0x28
    u8 x29;                            // 0x29
    u8 is_showed_tutorial_airride : 1; // 0x2a
    u8 is_showed_tutorial_topride : 1; // 0x2a
    u8 is_showed_tutorial_city : 1;    // 0x2a, 0x20
    u8 x2b;                            // 0x2b
    int x2c;                           // 0x2c
    MainMenuData main_menu;            // 0x30
    int x50;                           // 0x50
    int x54;                           // 0x54
    int x58;                           // 0x58
    int x5c;                           // 0x5c
    int x60;                           // 0x60
    int x64;                           // 0x64
    int x68;                           // 0x68
    int x6c;                           // 0x6c
    int x70;                           // 0x70
    int x74;                           // 0x74
    int x78;                           // 0x78
    int x7c;                           // 0x7c
    int x80;                           // 0x80
    int x84;                           // 0x84
    int x88;                           // 0x88
    int x8c;                           // 0x8c
    int x90;                           // 0x90
    int x94;                           // 0x94
    int x98;                           // 0x98
    int x9c;                           // 0x9c
    int xa0;                           // 0xa0
    int xa4;                           // 0xa4
    int xa8;                           // 0xa8
    int xac;                           // 0xac
    int xb0;                           // 0xb0
    int xb4;                           // 0xb4
    int xb8;                           // 0xb8
    int xbc;                           // 0xbc
    int xc0;                           // 0xc0
    int xc4;                           // 0xc4
    int xc8;                           // 0xc8
    int xcc;                           // 0xcc
    int xd0;                           // 0xd0
    int xd4;                           // 0xd4
    int xd8;                           // 0xd8
    int xdc;                           // 0xdc
    int xe0;                           // 0xe0
    int xe4;                           // 0xe4
    int xe8;                           // 0xe8
    int xec;                           // 0xec
    int xf0;                           // 0xf0
    int xf4;                           // 0xf4
    struct
    {
        u8 cursor;             // 0xf8, current grid cursor on the Top Ride course-select screen (0-6 = course, 7 = random)
        u8 xf9[5];             // 0xf9-0xfd
        u16 used_history_mask; // 0xfe, bitmask of recently-picked courses (vanilla anti-repeat for the random button)
    } topride_course_select;           // 0xf8
    int x100;                          // 0x100
    int x104;                          // 0x104
    struct
    {
        u8 x108;      // 0x108
        u8 x109;      // 0x109
        u8 x10a;      // 0x10a
        u8 x10b;      // 0x10b
        int x10c;     // 0x10c
        int x110;     // 0x110
        int x114;     // 0x114
        int x118;     // 0x118
        int x11c;     // 0x11c
        int x120;     // 0x120
        int x124;     // 0x124
        int x128;     // 0x128
        int x12c;     // 0x12c
        u8 x130;      // 0x134
        u8 x131;      // 0x134
        u8 x132;      // 0x134
        u8 p_kind[4]; // 0x133
        u8 icon[4];   // 0x137
        u8 x13b;      // 0x13b
        int x13c;     // 0x13c
        int x140;     // 0x140
        int x144;     // 0x144
        int x148;     // 0x148
        int x14c;     // 0x14c
        int x150;     // 0x150
        int x154;     // 0x154
        u8 x158;      // 0x158
        u8 x159;      // 0x159
        u8 x15a;      // 0x15a
        u8 color[4];  // 0x15b
        u8 x15f;
    } airride_select_ply;
    struct
    {
        u8 x160[0x37];                    // 0x160 to 0x196 - header bytes outside the lobby init range
        // === Lobby data - cleared (memset 0x39) by TopRide_InitSelectData (0x8002cfd8) ===
        u8 x197;                          // 0x197
        u8 init_flag;                     // 0x198, set to 0xFF on InitSelectData; consumed by lobby update
        u8 x199;                          // 0x199, exit-status flag (1 after scene-exit triggered)
        u8 x19a;                          // 0x19a
        u8 active_pad_mask;               // 0x19b, bitmap of slots with controllers attached
        u8 ready_to_start;                // 0x19c, set when all open panels are confirmed
        u8 panel_locked_mask;             // 0x19d, bitmap of panels that consumed input this frame
        u8 hold_b_timer[4];               // 0x19e, per-slot B-hold-to-exit timer
        u8 ply_state[4];                  // 0x1a2, per-slot main state: 0=selecting, 1=ready, 2=in-panel editing
        u8 ply_target[4];                 // 0x1a6, per-slot target panel (cursor moves between panels)
        u8 ply_panel_pos[4];              // 0x1aa, per-slot panel position 0..3 (activation order)
        u8 ply_subcursor[4];              // 0x1ae, per-slot in-panel cursor (0=top, 1=middle, 2=bottom row)
        u8 panel_pkind[4];                // 0x1b2, per-panel state: 0=open, 1=HMN, 2=CPU, 3=OFF
        u8 panel_pkind_ui[4];             // 0x1b6, animated mirror of panel_pkind for UI transitions
        u8 color[4];                      // 0x1ba, per-panel Kirby color (L/R buttons cycle via CSS_topRide_colorChanger)
        u8 panel_cpu_level[4];            // 0x1be, per-panel CPU level (0..4) - bottom row when panel is editing
        u8 panel_handicap[4];             // 0x1c2, per-panel handicap (0..4, "5 bars" bottom row when HMN)
        u8 panel_machine[4];              // 0x1c6, per-panel TR machine kind (0=Free Star, 1=Steer Star) - middle row "Control Type"
        u8 panel_field_d[4];              // 0x1ca, per-panel UI param_13 (uninvestigated)
        u8 x1ce;                          // 0x1ce, flag set to 1 by InitSelectData (extra-unlocks override?)
        u8 x1cf;                          // 0x1cf, flag set to 1 by InitSelectData
    } topride_select_ply;
    struct
    {
        u8 x1d0;                        // 0x1d0
        u8 x1d1[3];                     // 0x1d1
        u8 x1d4;                        // 0x1d4
        u8 is_all_ready;                // 0x1d5, checks for start inputs when this is 1
        u8 ply_is_selecting_bitfield;   // 0x1d6, (ply << 1). flag raised when selecting an icon or cpu/handicap level
        u8 x1d7;                        // 0x1d7
        u8 x1d8;                        // 0x1d8
        u8 is_ready[4];                 // 0x1d9, flag raised when the ply is active and done making their selection
        u8 hold_b_timer[20];            // 0x1dd, i have no idea why there are 20 elements when there are only 4 controller ports lol
        u8 player_state[4];             // 0x1f1, 0 = inactive, 1 = icon select, 2 = handicap/cpu level = 3 = player element
        u8 ply_cursor[4];               // 0x1f5, ply index the players cursor is hovered over
        u8 ply_bar[4];                  // 0x1f9, bar index the player is hovered over 
        u8 icon[4];                     // 0x1fd
        u8 icon_saved[4];               // 0x201, written to after player exits. assumed to preserve the human selected icon
        u8 x205[4];                     // 0x205
        u8 x209[4];                     // 0x209
        u8 x20d[4];                     // 0x20d
        u8 x211[4];                     // 0x211
        u8 x215[4];                     // 0x215, unk, 00 = hmn controlled, 02 = cpu, 03 = none
        u8 x219[4];                     // 0x219, prev value for x215. 
        u8 ply_pkind[4];                // 0x21d
        u8 ply_color[4];                // 0x221
        u8 ply_hmn_handicap[4];         // 0x225
        u8 ply_cpu_handicap[4];         // 0x229
        u8 ply_cpu_level[4];            // 0x22d
        u8 ply_icon_ckind[4];           // 0x231, the ckind belonging to the currently selected icon
        struct
        {
            u8 num;            // 0x235, total number of machines selectable
            u8 c_kind_arr[20]; // 0x236, 0x66, array of c_kind indices
        } machine_select;
        u8 x24a;  // 0x24a
        u8 x24b;  // 0x24b
        int x24c; // 0x24c
        int x250; // 0x250
        int x254; // 0x254
        int x258; // 0x258
    } city_select_ply;
    int x25c;                        // 0x25c
    int x260;                        // 0x260
    int x264;                        // 0x264
    int x268;                        // 0x268
    int x26c;                        // 0x26c
    int x270;                        // 0x270
    int x274;                        // 0x274
    int x278;                        // 0x278
    int x27c;                        // 0x27c
    int x280;                        // 0x280
    int x284;                        // 0x284
    int x288;                        // 0x288
    int x28c;                        // 0x28c
    int x290;                        // 0x290
    int x294;                        // 0x294
    int x298;                        // 0x298
    int x29c;                        // 0x29c
    int x2a0;                        // 0x2a0
    int x2a4;                        // 0x2a4
    int x2a8;                        // 0x2a8
    int x2ac;                        // 0x2ac
    int x2b0;                        // 0x2b0
    int x2b4;                        // 0x2b4
    int x2b8;                        // 0x2b8
    int x2bc;                        // 0x2bc
    int x2c0;                        // 0x2c0
    int x2c4;                        // 0x2c4
    int x2c8;                        // 0x2c8
    int x2cc;                        // 0x2cc
    int x2d0;                        // 0x2d0
    int x2d4;                        // 0x2d4
    int x2d8;                        // 0x2d8
    int x2dc;                        // 0x2dc
    int x2e0;                        // 0x2e0
    int x2e4;                        // 0x2e4
    int x2e8;                        // 0x2e8
    int x2ec;                        // 0x2ec
    int x2f0;                        // 0x2f0
    int x2f4;                        // 0x2f4
    int x2f8;                        // 0x2f8
    int x2fc;                        // 0x2fc
    int x300;                        // 0x300
    int x304;                        // 0x304
    int x308;                        // 0x308
    int x30c;                        // 0x30c
    int x310;                        // 0x310
    int x314;                        // 0x314
    int x318;                        // 0x318
    int x31c;                        // 0x31c
    int x320;                        // 0x320
    int x324;                        // 0x324
    int x328;                        // 0x328
    int x32c;                        // 0x32c
    int x330;                        // 0x330
    int x334;                        // 0x334
    int x338;                        // 0x338
    int x33c;                        // 0x33c
    int x340;                        // 0x340
    int x344;                        // 0x344
    int x348;                        // 0x348
    int x34c;                        // 0x34c
    int x350;                        // 0x350
    u8 x354;                         // 0x354, this gets copied to city_kind 
    u8 x355;                         // 0x355
    u16 x356;                        // 0x356
    u16 x358;                        // 0x358
    u8 x35a;                         // 0x35a, copied to 0xaa6
    u8 x35b;                         // 0x35b
    u8 x35c;                         // 0x35c
    AirRideMode airride_mode : 8;    // 0x35d
    u8 x35e;                         // 0x35e
    u8 x35f;                         // 0x35f
    int x360;                        // 0x360
    int x364;                        // 0x364
    int x368;                        // 0x368
    int x36c;                        // 0x36c
    int x370;                        // 0x370
    int topride_selected_course;     // 0x374, TopRideCourse index (0-6)
    int x378;                        // 0x378
    u8 x37c;                         // 0x37c
    u8 topride_course_valid;         // 0x37d, result of course unlock check
    u8 topride_extra_unlocks[3];     // 0x37e-0x380, booleans from clear_kinds 8, 9, 10
    TopRideMode topride_mode : 8;    // 0x381, 0=Race, 1=Time Attack, 2=Free Run
    u8 x382;                         // 0x382
    u8 x383;                         // 0x383
    int x384;                        // 0x384
    int x388;                        // 0x388
    int x38c;                        // 0x38c
    int x390;                        // 0x390
    struct                           // 0x394
    {                                //
        u16 time_seconds;            // 0x394
        u8 menu_stadium_selection;   // 0x396, selected from settings, sub 1 to get StadiumGroup value
        u8 game_tempo : 2;           // 0x397, 0xC0 (1 = normal = 2 is slow)
        u8 events_enable : 1;        // 0x397, 0x20
        u8 x397_x10 : 1;             // 0x397, 0x10
        u8 x397_x08 : 1;             // 0x397, 0x08
        u8 x397_x04 : 1;             // 0x397, 0x04
        u8 x397_x02 : 1;             // 0x397, 0x02
        u8 x397_x01 : 1;             // 0x397, 0x01
        int x398 : 8;                // 0x398,
        CityMode mode : 8;           // 0x399, which mode was selected from the menu
        int x39a : 8;                // 0x39a,
        int scene : 8;               // 0x39b, 3 = player select screen, 4 = in game, 5 = properties graph, 6 = stadium splash, 7 = stadium, 8 = results screen
        int x39c;                    // 0x39c
        float ply_stats[5][9];       // 0x3a0, copied from playerblock @ 80040024
        u8 is_bike[5];               // 0x454
        u8 machine_kind[5];          // 0x459
        u8 prev_stadium_kind[5];     // 0x45e
        int x464;                    // 0x464
        int x468;                    // 0x468
        int x46c;                    // 0x46c
        int x470;                    // 0x470
        int x474;                    // 0x474
        int x478;                    // 0x478
        int x47c;                    // 0x47c
        int x480;                    // 0x480
        int x484;                    // 0x484
        int x488;                    // 0x488
        int x48c;                    // 0x48c
        int x490;                    // 0x490
        int x494;                    // 0x494
        int x498;                    // 0x498
        int x49c;                    // 0x49c
        int x4a0;                    // 0x4a0
        int x4a4;                    // 0x4a4
        int x4a8;                    // 0x4a8
        int x4ac;                    // 0x4ac
        int x4b0;                    // 0x4b0
        int x4b4;                    // 0x4b4
        int x4b8;                    // 0x4b8
        int x4bc;                    // 0x4bc
        int x4c0;                    // 0x4c0
        int x4c4;                    // 0x4c4
        int x4c8;                    // 0x4c8
        int x4cc;                    // 0x4cc
        int x4d0;                    // 0x4d0
        int x4d4;                    // 0x4d4
        int x4d8;                    // 0x4d8
        int x4dc;                    // 0x4dc
        int x4e0;                    // 0x4e0
        int x4e4;                    // 0x4e4
        int x4e8;                    // 0x4e8
        int x4ec;                    // 0x4ec
        int x4f0;                    // 0x4f0
        int x4f4;                    // 0x4f4
        int x4f8;                    // 0x4f8
        int x4fc;                    // 0x4fc
        int x500;                    // 0x500
        int x504;                    // 0x504
        int x508;                    // 0x508
        int x50c;                    // 0x50c
        int x510;                    // 0x510
        int x514;                    // 0x514
        int x518;                    // 0x518
        int x51c;                    // 0x51c
        int x520;                    // 0x520
        int x524;                    // 0x524
        int x528;                    // 0x528
        int x52c;                    // 0x52c
        int x530;                    // 0x530
        int x534;                    // 0x534
        int x538;                    // 0x538
        int x53c;                    // 0x53c
        int x540;                    // 0x540
        int x544;                    // 0x544
        int x548;                    // 0x548
        int x54c;                    // 0x54c
        int x550;                    // 0x550
        int x554;                    // 0x554
        int x558;                    // 0x558
        int x55c;                    // 0x55c
        int x560;                    // 0x560
        int x564;                    // 0x564
        int x568;                    // 0x568
        int x56c;                    // 0x56c
        int x570;                    // 0x570
        int x574;                    // 0x574
        int x578;                    // 0x578
        int x57c;                    // 0x57c
        int x580;                    // 0x580
        int x584;                    // 0x584
        int x588;                    // 0x588
        int x58c;                    // 0x58c
        int x590;                    // 0x590
        int x594;                    // 0x594
        int x598;                    // 0x598
        int x59c;                    // 0x59c
        int x5a0;                    // 0x5a0
        int x5a4;                    // 0x5a4
        int x5a8;                    // 0x5a8
        u8 x5ac;                     // 0x5ac
        u8 stadium_kind;             // 0x5ad
        u8 x5ae;                     // 0x5ae
        u8 stadium_round;            // 0x5af
        int x5b0;                    // 0x5b0
        int stadium_score[4];        // 0x5b4
    } city;                          //
    int x5c4;                        // 0x5c4
    int x5c8;                        // 0x5c8
    int x5cc;                        // 0x5cc
    int x5d0;                        // 0x5d0
    int x5d4;                        // 0x5d4
    int x5d8;                        // 0x5d8
    int x5dc;                        // 0x5dc
    int x5e0;                        // 0x5e0
    int x5e4;                        // 0x5e4
    int x5e8;                        // 0x5e8
    int x5ec;                        // 0x5ec
    int x5f0;                        // 0x5f0
    int x5f4;                        // 0x5f4
    int x5f8;                        // 0x5f8
    int x5fc;                        // 0x5fc
    int x600;                        // 0x600
    int x604;                        // 0x604
    int x608;                        // 0x608
    int x60c;                        // 0x60c
    int x610;                        // 0x610
    int x614;                        // 0x614
    int x618;                        // 0x618
    int x61c;                        // 0x61c
    int x620;                        // 0x620
    int x624;                        // 0x624
    int x628;                        // 0x628
    int x62c;                        // 0x62c
    int x630;                        // 0x630
    int x634;                        // 0x634
    int x638;                        // 0x638
    int x63c;                        // 0x63c
    int x640;                        // 0x640
    int x644;                        // 0x644
    int x648;                        // 0x648
    int x64c;                        // 0x64c
    int x650;                        // 0x650
    int x654;                        // 0x654
    int x658;                        // 0x658
    int x65c;                        // 0x65c
    int x660;                        // 0x660
    int x664;                        // 0x664
    int x668;                        // 0x668
    int x66c;                        // 0x66c
    int x670;                        // 0x670
    int x674;                        // 0x674
    int x678;                        // 0x678
    int x67c;                        // 0x67c
    int x680;                        // 0x680
    int x684;                        // 0x684
    int x688;                        // 0x688
    int x68c;                        // 0x68c
    int x690;                        // 0x690
    int x694;                        // 0x694
    int x698;                        // 0x698
    int x69c;                        // 0x69c
    int x6a0;                        // 0x6a0
    int x6a4;                        // 0x6a4
    int x6a8;                        // 0x6a8
    int x6ac;                        // 0x6ac
    int x6b0;                        // 0x6b0
    int x6b4;                        // 0x6b4
    int x6b8;                        // 0x6b8
    int x6bc;                        // 0x6bc
    int x6c0;                        // 0x6c0
    int x6c4;                        // 0x6c4
    int x6c8;                        // 0x6c8
    int x6cc;                        // 0x6cc
    int x6d0;                        // 0x6d0
    int x6d4;                        // 0x6d4
    int x6d8;                        // 0x6d8
    int x6dc;                        // 0x6dc
    int x6e0;                        // 0x6e0
    int x6e4;                        // 0x6e4
    int x6e8;                        // 0x6e8
    int x6ec;                        // 0x6ec
    int x6f0;                        // 0x6f0
    int x6f4;                        // 0x6f4
    int x6f8;                        // 0x6f8
    int x6fc;                        // 0x6fc
    int x700;                        // 0x700
    int x704;                        // 0x704
    int x708;                        // 0x708
    int x70c;                        // 0x70c
    int x710;                        // 0x710
    int x714;                        // 0x714
    int x718;                        // 0x718
    int x71c;                        // 0x71c
    int x720;                        // 0x720
    int x724;                        // 0x724
    int x728;                        // 0x728
    int x72c;                        // 0x72c
    int x730;                        // 0x730
    int x734;                        // 0x734
    int x738;                        // 0x738
    int x73c;                        // 0x73c
    int x740;                        // 0x740
    int x744;                        // 0x744
    int x748;                        // 0x748
    int x74c;                        // 0x74c
    int x750;                        // 0x750
    int x754;                        // 0x754
    int x758;                        // 0x758
    int x75c;                        // 0x75c
    int x760;                        // 0x760
    int x764;                        // 0x764
    int x768;                        // 0x768
    int x76c;                        // 0x76c
    int x770;                        // 0x770
    int x774;                        // 0x774
    int x778;                        // 0x778
    int x77c;                        // 0x77c
    int x780;                        // 0x780
    int x784;                        // 0x784
    int x788;                        // 0x788
    int x78c;                        // 0x78c
    int x790;                        // 0x790
    int x794;                        // 0x794
    int x798;                        // 0x798
    int x79c;                        // 0x79c
    int x7a0;                        // 0x7a0
    int x7a4;                        // 0x7a4
    int x7a8;                        // 0x7a8
    int x7ac;                        // 0x7ac
    int x7b0;                        // 0x7b0
    int x7b4;                        // 0x7b4
    int x7b8;                        // 0x7b8
    int x7bc;                        // 0x7bc
    int x7c0;                        // 0x7c0
    int x7c4;                        // 0x7c4
    int x7c8;                        // 0x7c8
    int x7cc;                        // 0x7cc
    int x7d0;                        // 0x7d0
    MajorKind major_cur : 8;         // 0x7d4
    MajorKind major_pending : 8;     // 0x7d5
    u8 request_major_exit;           // 0x7d6
    int x7d8;                        // 0x7d8
    int x7dc;                        // 0x7dc
    HSD_Update update;               // 0x7e0
    int x828;                        // 0x828
    int x82c;                        // 0x82c
    u8 pause_ply;                    // 0x830, player who paused the game
    u8 pause_cursor;                 // 0x831, 0 = resume, 1 = start over, 2 = exit
    u8 intro_state;                  // 0x832
    u8 frames_in_second;             // 0x833
    int seconds_passed;              // 0x834
    int pause_delay;                 // 0x838
    int x83c;                        // 0x83c
    int x840;                        // 0x840
    int x844;                        // 0x844
    int x848;                        // 0x848
    int x84c;                        // 0x84c
    int x850;                        // 0x850
    u8 x854;                         // 0x854
    u8 x855;                         // 0x855
    u8 x856;                         // 0x856
    u8 x857_ply[4];                  // 0x857, accessed by 8000981c
    u8 x858;                         // 0x85b
    int x85c;                        // 0x85c
    int x860;                        // 0x860
    int x864;                        // 0x864
    int x868;                        // 0x868
    int x86c;                        // 0x86c
    int x870;                        // 0x870
    int x874;                        // 0x874
    int x878;                        // 0x878
    int x87c;                        // 0x87c
    int x880;                        // 0x880
    int x884;                        // 0x884
    int x888;                        // 0x888
    int x88c;                        // 0x88c
    int x890;                        // 0x890
    int x894;                        // 0x894
    int x898;                        // 0x898
    int x89c;                        // 0x89c
    int x8a0;                        // 0x8a0
    int x8a4;                        // 0x8a4
    int x8a8;                        // 0x8a8
    int x8ac;                        // 0x8ac
    int x8b0;                        // 0x8b0
    int x8b4;                        // 0x8b4
    int x8b8;                        // 0x8b8
    int x8bc;                        // 0x8bc
    int x8c0;                        // 0x8c0
    int x8c4;                        // 0x8c4
    int x8c8;                        // 0x8c8
    int x8cc;                        // 0x8cc
    int x8d0;                        // 0x8d0
    int x8d4;                        // 0x8d4
    int x8d8;                        // 0x8d8
    int x8dc;                        // 0x8dc
    int x8e0;                        // 0x8e0
    int x8e4;                        // 0x8e4
    int x8e8;                        // 0x8e8
    int x8ec;                        // 0x8ec
    int x8f0;                        // 0x8f0
    int x8f4;                        // 0x8f4
    int x8f8;                        // 0x8f8
    int x8fc;                        // 0x8fc
    int x900;                        // 0x900
    int x904;                        // 0x904
    int x908;                        // 0x908
    int x90c;                        // 0x90c
    int x910;                        // 0x910
    int x914;                        // 0x914
    int x918;                        // 0x918
    int x91c;                        // 0x91c
    int x920;                        // 0x920
    int x924;                        // 0x924
    int x928;                        // 0x928
    int x92c;                        // 0x92c
    int x930;                        // 0x930
    int x934;                        // 0x934
    int x938;                        // 0x938
    int x93c;                        // 0x93c
    int x940;                        // 0x940
    int x944;                        // 0x944
    int x948;                        // 0x948
    int x94c;                        // 0x94c
    int x950;                        // 0x950
    int x954;                        // 0x954
    int x958;                        // 0x958
    int x95c;                        // 0x95c
    int x960;                        // 0x960
    int x964;                        // 0x964
    int x968;                        // 0x968
    int x96c;                        // 0x96c
    int x970;                        // 0x970
    int x974;                        // 0x974
    int x978;                        // 0x978
    int x97c;                        // 0x97c
    int x980;                        // 0x980
    int x984;                        // 0x984
    int x988;                        // 0x988
    int x98c;                        // 0x98c
    int x990;                        // 0x990
    int x994;                        // 0x994
    int x998;                        // 0x998
    int x99c;                        // 0x99c
    int x9a0;                        // 0x9a0
    int x9a4;                        // 0x9a4
    int x9a8;                        // 0x9a8
    int x9ac;                        // 0x9ac
    int x9b0;                        // 0x9b0
    int x9b4;                        // 0x9b4
    int x9b8;                        // 0x9b8
    int x9bc;                        // 0x9bc
    int x9c0;                        // 0x9c0
    int x9c4;                        // 0x9c4
    int x9c8;                        // 0x9c8
    int x9cc;                        // 0x9cc
    int x9d0;                        // 0x9d0
    int x9d4;                        // 0x9d4
    int x9d8;                        // 0x9d8
    int x9dc;                        // 0x9dc
    int x9e0;                        // 0x9e0
    int x9e4;                        // 0x9e4
    int x9e8;                        // 0x9e8
    int x9ec;                        // 0x9ec
    int x9f0;                        // 0x9f0
    int x9f4;                        // 0x9f4
    int x9f8;                        // 0x9f8
    int x9fc;                        // 0x9fc
    int xa00;                        // 0xa00
    int xa04;                        // 0xa04
    int xa08;                        // 0xa08
    int xa0c;                        // 0xa0c
    int xa10;                        // 0xa10
    int xa14;                        // 0xa14
    int xa18;                        // 0xa18
    int xa1c;                        // 0xa1c
    int xa20;                        // 0xa20
    int xa24;                        // 0xa24
    int xa28;                        // 0xa28
    int xa2c;                        // 0xa2c
    int xa30;                        // 0xa30
    int xa34;                        // 0xa34
    int destruction_derby_ko_num[4]; // 0xa38, indexed by ply
    int xa48;                        // 0xa48
    int xa4c;                        // 0xa4c
    int xa50;                        // 0xa50
    int xa54;                        // 0xa54
    int xa58;                        // 0xa58
    int xa5c;                        // 0xa5c
    int xa60;                        // 0xa60
    int xa64;                        // 0xa64
    int xa68;                        // 0xa68
    int xa6c;                        // 0xa6c
    int xa70;                        // 0xa70
    int xa74;                        // 0xa74
    int xa78;                        // 0xa78
    int xa7c;                        // 0xa7c
    int xa80;                        // 0xa80
    int xa84;                        // 0xa84
    int xa88;                        // 0xa88
    int xa8c;                        // 0xa8c
    int xa90;                        // 0xa90
    u8 city_kind;                    // 0xa94, 5 = main city trial. stadium modes are derived here @ 8004051c (0xE is destruction derby)
    u8 xa95;                         // 0xa95
    u8 view_num;                     // 0xa96, number of views to create?
    u8 stage_kind;                   // 0xa97, StageKind
    u8 bgm_override;                 // 0xa98, when this is not 1, it plays it as the song id
    u8 is_always_ura_bgm;            // 0xa99
    u8 xa9a;                         // 0xa9a
    u8 xa9b;                         // 0xa9b
    u16 time_seconds;                // 0xa9c
    int rng_seed_initial;            // 0xaa0
    u8 xaa4;                         // 0xaa4
    u8 xaa5;                         // 0xaa5
    u8 xaa6_80 : 1;                  // 0xaa6, 0x80
    u8 xaa6_40 : 1;                  // 0xaa6, 0x40
    u8 xaa6_20 : 1;                  // 0xaa6, 0x20 (always enabled)
    u8 xaa6_10 : 1;                  // 0xaa6, 0x10 (always enabled)
    u8 xaa6_08 : 1;                  // 0xaa6, 0x08
    u8 tempo : 2;                    // 0xaa6, 0x06
    u8 xaa6_01 : 1;                  // 0xaa6, 0x01
    u8 xaa7_80 : 1;                  // 0xaa7, 0x80
    u8 xaa7_40 : 1;                  // 0xaa7, 0x40
    u8 is_play_music : 1;            // 0xaa7, 0x20
    u8 is_enable_events : 1;         // 0xaa7, 0x10
    u8 is_replay : 1;                // 0xaa7, 0x08
    u8 xaa7_04 : 1;                  // 0xaa7, 0x04
    u8 xaa7_02 : 1;                  // 0xaa7, 0x02
    u8 xaa7_01 : 1;                  // 0xaa7, 0x01
    int xaa8;                        // 0xaa8
    int xaac;                        // 0xaac
    int xab0;                        // 0xab0
    int xab4;                        // 0xab4
    int xab8;                        // 0xab8
    int xabc;                        // 0xabc
    int xac0;                        // 0xac0
    int xac4;                        // 0xac4
    PlayerDesc ply_desc[5];          // 0xac8 - gameplay loops iterate < 5; controller-bound loops (rumble, view) iterate < 4. Slot 4 is allocated and managed but appears unused by vanilla code paths (vestigial over-allocation, parallels stc_playerdata[5]).
    struct                           // 0xbb8
    {
        s8 ply;                      // 0x0
        s8 flag;                     // 0x1 PlayerCamFlag
        s8 x2;                       // 0x2
    } ply_view_desc[4];              //
    int xbc4;                        // 0xbc4
    StadiumResults stadium_results;  // 0xbc8
    int xc24;                        // 0xc24
    int xc28;                        // 0xc28
    int xc2c;                        // 0xc2c
    int xc30;                        // 0xc30
    int xc34;                        // 0xc34
    int xc38;                        // 0xc38
    int xc3c;                        // 0xc3c
    int xc40;                        // 0xc40
    int xc44;                        // 0xc44
    int xc48;                        // 0xc48
    int xc4c;                        // 0xc4c
    int xc50;                        // 0xc50
    int xc54;                        // 0xc54
    int xc58;                        // 0xc58
    int xc5c;                        // 0xc5c
    int xc60;                        // 0xc60
    int xc64;                        // 0xc64
    int xc68;                        // 0xc68
    int xc6c;                        // 0xc6c
    int xc70;                        // 0xc70
    int xc74;                        // 0xc74
    int xc78;                        // 0xc78
    int xc7c;                        // 0xc7c
    int xc80;                        // 0xc80
    int xc84;                        // 0xc84
    int xc88;                        // 0xc88
    int xc8c;                        // 0xc8c
    int xc90;                        // 0xc90
    int xc94;                        // 0xc94
    int xc98;                        // 0xc98
    int xc9c;                        // 0xc9c
    int xca0;                        // 0xca0
    int xca4;                        // 0xca4
    int xca8;                        // 0xca8
    int xcac;                        // 0xcac
    int xcb0;                        // 0xcb0
    int xcb4;                        // 0xcb4
    int xcb8;                        // 0xcb8
    int xcbc;                        // 0xcbc
    int xcc0;                        // 0xcc0
    int xcc4;                        // 0xcc4
    int xcc8;                        // 0xcc8
    int xccc;                        // 0xccc
    int xcd0;                        // 0xcd0
    int xcd4;                        // 0xcd4
    int xcd8;                        // 0xcd8
    int xcdc;                        // 0xcdc
    int xce0;                        // 0xce0
    int xce4;                        // 0xce4
    int xce8;                        // 0xce8
    int xcec;                        // 0xcec
    int xcf0;                        // 0xcf0
    int xcf4;                        // 0xcf4
    int xcf8;                        // 0xcf8
    int xcfc;                        // 0xcfc
    int xd00;                        // 0xd00
    int xd04;                        // 0xd04
    int xd08;                        // 0xd08
    int xd0c;                        // 0xd0c
    int xd10;                        // 0xd10
    int xd14;                        // 0xd14
    int xd18;                        // 0xd18
    int xd1c;                        // 0xd1c
    // === TR slot config table - 4 slots × 9 bytes (0xD20 to 0xD43) ===
    // Committed at TR scene-exit from the lobby fields above. The runtime
    // reader is TopRide_FielderInit (0x802dafb4) which dispatches to
    // TopRide_KirbyInit per slot using this block. Per-slot byte layout:
    //   +0: pkind (TopRidePlayerKind: HMN/CPU/NONE) - accessor TopRide_GetPlayerKind / SetPlayerKind
    //   +1: kirby color - TopRide_GetColor / SetColor
    //   +2: CPU level (0..4) - TopRide_SetCpuLevel
    //   +3: handicap (0..4) - TopRide_SetHandicap
    //   +5: TopRide_SetSlotConfigD25 (zz_8000bfd4_, unclassified)
    //   +6: controller port - TopRide_SetControllerPort
    //   +8: TR machine kind (0=Free Star, 1=Steer Star) - TopRide_GetMachineKind / SetMachineKind
    struct
    {
        u8 pkind;                    // 0xD20+i*9, see TopRidePlayerKind
        u8 color;                    // 0xD21+i*9
        u8 cpu_level;                // 0xD22+i*9
        u8 handicap;                 // 0xD23+i*9
        u8 xD24;                     // 0xD24+i*9
        u8 xD25;                     // 0xD25+i*9
        u8 controller_port;          // 0xD26+i*9
        u8 xD27;                     // 0xD27+i*9
        u8 machine_kind;             // 0xD28+i*9, TopRideMachineKind
    } topride_slot[4];               // 0xd20, 36 bytes total
    int xd44;                        // 0xd44
    int xd48;                        // 0xd48
    int xd4c;                        // 0xd4c
    struct                           // 0xd50, built by Checklist_BuildUnlockBitfields (80007af0)
    {                                //
        u32 airride_unlock_lo;       // 0xd50, bits 0-31 of Air Ride reward unlock status
        u32 airride_unlock_hi;       // 0xd54, bits 32-45 of Air Ride reward unlock status
        u32 citytrial_unlock_lo;     // 0xd58, bits 0-31 of City Trial reward unlock status
        u32 citytrial_unlock_hi;     // 0xd5c, bits 32-43 of City Trial reward unlock status
        u32 stadium_flags_cache;     // 0xd60, copy of GameData + 0x1510 (stadium unlock flags)
        u32 xd64;                    // 0xd64, always 0
    } unlock_cache;                  //
    int xd68;                        // 0xd68
    int xd6c;                        // 0xd6c
    int xd70;                        // 0xd70
    int xd74;                        // 0xd74
    int xd78;                        // 0xd78
    int xd7c;                        // 0xd7c
    int xd80;                        // 0xd80
    int xd84;                        // 0xd84
    int xd88;                        // 0xd88
    int xd8c;                        // 0xd8c
    int xd90;                        // 0xd90
    int xd94;                        // 0xd94
    int xd98;                        // 0xd98
    int xd9c;                        // 0xd9c
    int xda0;                        // 0xda0
    int xda4;                        // 0xda4
    int xda8;                        // 0xda8
    int xdac;                        // 0xdac
    int xdb0;                        // 0xdb0
    int xdb4;                        // 0xdb4
    int xdb8;                        // 0xdb8
    int xdbc;                        // 0xdbc
    int xdc0;                        // 0xdc0
    int xdc4;                        // 0xdc4
    int xdc8;                        // 0xdc8
    int xdcc;                        // 0xdcc
    int xdd0;                        // 0xdd0
    int xdd4;                        // 0xdd4
    int xdd8;                        // 0xdd8
    int xddc;                        // 0xddc
    int xde0;                        // 0xde0
    int xde4;                        // 0xde4
    int xde8;                        // 0xde8
    int xdec;                        // 0xdec
    int xdf0;                        // 0xdf0
    int xdf4;                        // 0xdf4
    int xdf8;                        // 0xdf8
    int xdfc;                        // 0xdfc
    int xe00;                        // 0xe00
    int xe04;                        // 0xe04
    int xe08;                        // 0xe08
    int xe0c;                        // 0xe0c
    int xe10;                        // 0xe10
    int xe14;                        // 0xe14
    int xe18;                        // 0xe18
    int xe1c;                        // 0xe1c
    int xe20;                        // 0xe20
    int xe24;                        // 0xe24
    int xe28;                        // 0xe28
    int xe2c;                        // 0xe2c
    int xe30;                        // 0xe30
    int xe34;                        // 0xe34
    int xe38;                        // 0xe38
    int xe3c;                        // 0xe3c
    int xe40;                        // 0xe40
    int xe44;                        // 0xe44
    int xe48;                        // 0xe48
    int xe4c;                        // 0xe4c
    int xe50;                        // 0xe50
    int xe54;                        // 0xe54
    int xe58;                        // 0xe58
    int xe5c;                        // 0xe5c
    int xe60;                        // 0xe60
    int xe64;                        // 0xe64
    int xe68;                        // 0xe68
    int xe6c;                        // 0xe6c
    int xe70;                        // 0xe70
    int xe74;                        // 0xe74
    int xe78;                        // 0xe78
    int xe7c;                        // 0xe7c
    int xe80;                        // 0xe80
    int xe84;                        // 0xe84
    int xe88;                        // 0xe88
    int xe8c;                        // 0xe8c
    int xe90;                        // 0xe90
    int xe94;                        // 0xe94
    int xe98;                        // 0xe98
    int xe9c;                        // 0xe9c
    int xea0;                        // 0xea0
    int xea4;                        // 0xea4
    int xea8;                        // 0xea8
    int xeac;                        // 0xeac
    int xeb0;                        // 0xeb0
    int xeb4;                        // 0xeb4
    int xeb8;                        // 0xeb8
    int xebc;                        // 0xebc
    int xec0;                        // 0xec0
    int xec4;                        // 0xec4
    int xec8;                        // 0xec8
    int xecc;                        // 0xecc
    GOBJ *clearchecker_gobj;         // 0xed0
    int xed4;                        // 0xed4
    int xed8;                        // 0xed8
    int xedc;                        // 0xedc
    int xee0;                        // 0xee0
    int xee4;                        // 0xee4
    int xee8;                        // 0xee8
    int xeec;                        // 0xeec
    int xef0;                        // 0xef0
    int xef4;                        // 0xef4
    int xef8;                        // 0xef8
    int xefc;                        // 0xefc
    // actual size is 0x1518
} GameData;

// Per-mode tuning block for the patch-drop pipeline. Three of these live in
// Game3dData at 0x1d4 / 0x1ec / 0x204, one per `patch_drop_mode` (0/1/2).
// Sub-handlers (Rider_TickDropAllUp, Rider_SpawnDropPatchSeq) interpolate
// between the (lo, hi) pairs with a uniform random factor; pair C scales
// the spawn velocity vector, pairs A/B feed two scalar args to SpawnItem
// (semantics not fully nailed down - see docs/patch-drop-system.md).
typedef struct PatchDropModeParams
{
    f32 lo_a;  // 0x00
    f32 hi_a;  // 0x04
    f32 lo_b;  // 0x08
    f32 hi_b;  // 0x0c
    f32 lo_c;  // 0x10
    f32 hi_c;  // 0x14
} PatchDropModeParams;

typedef struct Game3dData
{
    u8 plyview_num;                               // 0x0
    s8 plyview_lookup[4];                         // 0x1, -1 for not present?
    HSD_SObjDesc *hud_sobj;                       // 0x8
    int xc;                                       // 0xc
    int x10;                                      // 0x10
    int x14;                                      // 0x14
    int x18;                                      // 0x18
    int x1c;                                      // 0x1c
    int x20;                                      // 0x20
    int x24;                                      // 0x24
    GOBJ *mapcam_gobj;                            // 0x28
    int x2c;                                      // 0x2c
    int x30;                                      // 0x30
    int x34;                                      // 0x34
    int x38;                                      // 0x38
    int x3c;                                      // 0x3c
    int x40;                                      // 0x40
    int x44;                                      // 0x44
    int x48;                                      // 0x48
    int x4c;                                      // 0x4c
    int x50;                                      // 0x50
    int x54;                                      // 0x54
    int x58;                                      // 0x58
    int x5c;                                      // 0x5c
    int x60;                                      // 0x60
    int x64;                                      // 0x64
    int x68;                                      // 0x68
    int x6c;                                      // 0x6c
    int x70;                                      // 0x70
    int x74;                                      // 0x74
    int x78;                                      // 0x78
    int x7c;                                      // 0x7c
    int x80;                                      // 0x80
    int x84;                                      // 0x84
    int x88;                                      // 0x88
    int x8c;                                      // 0x8c
    int x90;                                      // 0x90
    int x94;                                      // 0x94
    int x98;                                      // 0x98
    int x9c;                                      // 0x9c
    int xa0;                                      // 0xa0
    GOBJ *ply_speedometer_gobj[4];                // 0xa4
    int xb4;                                      // 0xb4
    int xb8;                                      // 0xb8
    int xbc;                                      // 0xbc
    JOBJSet *ScInfPlynum_scene_models;            // 0xc0
    GOBJ *plynum_gobj[4][5];                      // 0xc4
    int x114;                                     // 0x114
    int x118;                                     // 0x118
    int x11c;                                     // 0x11c
    int x120;                                     // 0x120
    int x124;                                     // 0x124
    int x128;                                     // 0x128
    int x12c;                                     // 0x12c
    int x130;                                     // 0x130
    int x134;                                     // 0x134
    int x138;                                     // 0x138
    JOBJSet **ScInfPliconKirby_scene_models;      // 0x13c
    JOBJSet **ScInfPliconDedede_scene_models;     // 0x140
    JOBJSet **ScInfPliconMetaKnight_scene_models; // 0x144
    int x148;                                     // 0x148
    int x14c;                                     // 0x14c
    int x150;                                     // 0x150
    int x154;                                     // 0x154
    int x158;                                     // 0x158
    int x15c;                                     // 0x15c
    int x160;                                     // 0x160
    int x164;                                     // 0x164
    int x168;                                     // 0x168
    int x16c;                                     // 0x16c
    int x170;                                     // 0x170
    int x174;                                     // 0x174
    int x178;                                     // 0x178
    int x17c;                                     // 0x17c
    int x180;                                     // 0x180
    int x184;                                     // 0x184
    int x188;                                     // 0x188
    int x18c;                                     // 0x18c
    int x190;                                     // 0x190
    int x194;                                     // 0x194
    int x198;                                     // 0x198
    int x19c;                                     // 0x19c
    int x1a0;                                     // 0x1a0
    int x1a4;                                     // 0x1a4
    int x1a8;                                     // 0x1a8
    int x1ac;                                     // 0x1ac
    int x1b0;                                     // 0x1b0
    int x1b4;                                     // 0x1b4
    int x1b8;                                     // 0x1b8
    // --- patch-drop pipeline tuning block (0x1bc..0x224) ---
    // See Rider_DropPatches / Rider_TickDropPatches and docs/patch-drop-system.md.
    int patch_drop_mode0_count;                   // 0x1bc, fixed drop count when caller passes 0 with mode 0
    int patch_drop_spawn_arg7;                    // 0x1c0, passed verbatim as r7 (4th int arg) to SpawnItem (0x80253ce4)
    f32 patch_drop_spawn_y_bias;                  // 0x1c4, added to spawn position Y in both sub-handlers
    f32 patch_drop_mode2_factor;                  // 0x1c8, multiplied with sum-of-positive-stats to size mode-2 drops
    f32 patch_drop_mode1_factor;                  // 0x1cc, multiplied with sum-of-positive-stats to size mode-1 drops
    f32 patch_drop_throw_spread;                  // 0x1d0, max throw-spread half-angle in degrees; ×deg2rad×rand (sign by count parity) fans drop throw directions. Read by Rider_TickDropAllUp / Rider_SpawnDropPatchSeq
    PatchDropModeParams patch_drop_mode0_params;  // 0x1d4
    PatchDropModeParams patch_drop_mode1_params;  // 0x1ec
    PatchDropModeParams patch_drop_mode2_params;  // 0x204
    int patch_drop_cooldown_init;                 // 0x21c, frames until the next spawn after a successful one
    int patch_drop_burst_threshold;               // 0x220, when patch_drop_progress reaches this, switch from sequential to burst
    int patch_drop_allup_rng_max;                 // 0x224, mode-0 only: HSD_Randi ceiling for the all-up RNG roll
    // --- end patch-drop tuning block ---
    int x228;                                     // 0x228
    int x22c;                                     // 0x22c
    int x230;                                     // 0x230
    int x234;                                     // 0x234
    int x238;                                     // 0x238
    int x23c;                                     // 0x23c
    int x240;                                     // 0x240
    int x244;                                     // 0x244
    int x248;                                     // 0x248
    int x24c;                                     // 0x24c
    int x250;                                     // 0x250
    int x254;                                     // 0x254
    int x258;                                     // 0x258
    int x25c;                                     // 0x25c
    int x260;                                     // 0x260
    int x264;                                     // 0x264
    int x268;                                     // 0x268
    int x26c;                                     // 0x26c
    int x270;                                     // 0x270
    int x274;                                     // 0x274
    int x278;                                     // 0x278
    int x27c;                                     // 0x27c
    int x280;                                     // 0x280
    int x284;                                     // 0x284
    int x288;                                     // 0x288
    int x28c;                                     // 0x28c
    int x290;                                     // 0x290
    int x294;                                     // 0x294
    int x298;                                     // 0x298
    int x29c;                                     // 0x29c
    int x2a0;                                     // 0x2a0
    int x2a4;                                     // 0x2a4
    int x2a8;                                     // 0x2a8
    int x2ac;                                     // 0x2ac
    int x2b0;                                     // 0x2b0
    int x2b4;                                     // 0x2b4
    int x2b8;                                     // 0x2b8
    int x2bc;                                     // 0x2bc
    int x2c0;                                     // 0x2c0
    int x2c4;                                     // 0x2c4
    int x2c8;                                     // 0x2c8
    int x2cc;                                     // 0x2cc
    int x2d0;                                     // 0x2d0
    int x2d4;                                     // 0x2d4
    int x2d8;                                     // 0x2d8
    int x2dc;                                     // 0x2dc
    int x2e0;                                     // 0x2e0
    int x2e4;                                     // 0x2e4
    int x2e8;                                     // 0x2e8
    int x2ec;                                     // 0x2ec
    int x2f0;                                     // 0x2f0
    int x2f4;                                     // 0x2f4
    int x2f8;                                     // 0x2f8
    int x2fc;                                     // 0x2fc
    int x300;                                     // 0x300
    int x304;                                     // 0x304
    int x308;                                     // 0x308
    int x30c;                                     // 0x30c
    int x310;                                     // 0x310
    int x314;                                     // 0x314
    int x318;                                     // 0x318
    int x31c;                                     // 0x31c
    int x320;                                     // 0x320
    int x324;                                     // 0x324
    int x328;                                     // 0x328
    int x32c;                                     // 0x32c
    int x330;                                     // 0x330
    int x334;                                     // 0x334
    int x338;                                     // 0x338
    int x33c;                                     // 0x33c
    int x340;                                     // 0x340
    int x344;                                     // 0x344
    int x348;                                     // 0x348
    int x34c;                                     // 0x34c
    int x350;                                     // 0x350
    int x354;                                     // 0x354
    int x358;                                     // 0x358
    int x35c;                                     // 0x35c
    int x360;                                     // 0x360
    int x364;                                     // 0x364
    int x368;                                     // 0x368
    int x36c;                                     // 0x36c
    int x370;                                     // 0x370
    int x374;                                     // 0x374
    int x378;                                     // 0x378
    int x37c;                                     // 0x37c
    int x380;                                     // 0x380
    int x384;                                     // 0x384
    int x388;                                     // 0x388
    int x38c;                                     // 0x38c
    int x390;                                     // 0x390
    int x394;                                     // 0x394
    int x398;                                     // 0x398
    int x39c;                                     // 0x39c
    int x3a0;                                     // 0x3a0
    int x3a4;                                     // 0x3a4
    int x3a8;                                     // 0x3a8
    int x3ac;                                     // 0x3ac
    int x3b0;                                     // 0x3b0
    int x3b4;                                     // 0x3b4
    int x3b8;                                     // 0x3b8
    int x3bc;                                     // 0x3bc
    int x3c0;                                     // 0x3c0
    int x3c4;                                     // 0x3c4
    int x3c8;                                     // 0x3c8
    int x3cc;                                     // 0x3cc
    int x3d0;                                     // 0x3d0
    int x3d4;                                     // 0x3d4
    int x3d8;                                     // 0x3d8
    int x3dc;                                     // 0x3dc
    int x3e0;                                     // 0x3e0
    int x3e4;                                     // 0x3e4
    int x3e8;                                     // 0x3e8
    int x3ec;                                     // 0x3ec
    int x3f0;                                     // 0x3f0
    int x3f4;                                     // 0x3f4
    int x3f8;                                     // 0x3f8
    int x3fc;                                     // 0x3fc
    int x400;                                     // 0x400
    int x404;                                     // 0x404
    int x408;                                     // 0x408
    int x40c;                                     // 0x40c
    int x410;                                     // 0x410
    int x414;                                     // 0x414
    int x418;                                     // 0x418
    int x41c;                                     // 0x41c
    int x420;                                     // 0x420
    int x424;                                     // 0x424
    int x428;                                     // 0x428
    int x42c;                                     // 0x42c
    int x430;                                     // 0x430
    int x434;                                     // 0x434
    int x438;                                     // 0x438
    int x43c;                                     // 0x43c
    int x440;                                     // 0x440
    int x444;                                     // 0x444
    int x448;                                     // 0x448
    int x44c;                                     // 0x44c
    int x450;                                     // 0x450
    u8 is_bike[5];                                // 0x454
    u8 machine_kind[5];                           // 0x459
    u8 prev_stadium_kind[5];                      // 0x45e
    int x464;                                     // 0x464
    int x468;                                     // 0x468
    int x46c;                                     // 0x46c
    int x470;                                     // 0x470
    int x474;                                     // 0x474
    int x478;                                     // 0x478
    int x47c;                                     // 0x47c
    int x480;                                     // 0x480
    int x484;                                     // 0x484
    int x488;                                     // 0x488
    int x48c;                                     // 0x48c
    int x490;                                     // 0x490
    int x494;                                     // 0x494
    int x498;                                     // 0x498
    int x49c;                                     // 0x49c
    int x4a0;                                     // 0x4a0
    int x4a4;                                     // 0x4a4
    int x4a8;                                     // 0x4a8
    int x4ac;                                     // 0x4ac
    int x4b0;                                     // 0x4b0
    int x4b4;                                     // 0x4b4
    int x4b8;                                     // 0x4b8
    int x4bc;                                     // 0x4bc
    int x4c0;                                     // 0x4c0
    int x4c4;                                     // 0x4c4
    int x4c8;                                     // 0x4c8
    int x4cc;                                     // 0x4cc
    int x4d0;                                     // 0x4d0
    int x4d4;                                     // 0x4d4
    int x4d8;                                     // 0x4d8
    int x4dc;                                     // 0x4dc
    int x4e0;                                     // 0x4e0
    int x4e4;                                     // 0x4e4
    int x4e8;                                     // 0x4e8
    int x4ec;                                     // 0x4ec
    int x4f0;                                     // 0x4f0
    int x4f4;                                     // 0x4f4
    int x4f8;                                     // 0x4f8
    int x4fc;                                     // 0x4fc
    int x500;                                     // 0x500
    int x504;                                     // 0x504
    int x508;                                     // 0x508
    int x50c;                                     // 0x50c
    int x510;                                     // 0x510
    int x514;                                     // 0x514
    int x518;                                     // 0x518
    int x51c;                                     // 0x51c
    int x520;                                     // 0x520
    int x524;                                     // 0x524
    int x528;                                     // 0x528
    int x52c;                                     // 0x52c
    int x530;                                     // 0x530
    int x534;                                     // 0x534
    int x538;                                     // 0x538
    int x53c;                                     // 0x53c
    int x540;                                     // 0x540
    int x544;                                     // 0x544
    int x548;                                     // 0x548
    int x54c;                                     // 0x54c
    int x550;                                     // 0x550
    int x554;                                     // 0x554
    int x558;                                     // 0x558
    int x55c;                                     // 0x55c
    int x560;                                     // 0x560
    int x564;                                     // 0x564
    int x568;                                     // 0x568
    int x56c;                                     // 0x56c
    int x570;                                     // 0x570
    int x574;                                     // 0x574
    int x578;                                     // 0x578
    int x57c;                                     // 0x57c
    int x580;                                     // 0x580
    int x584;                                     // 0x584
    int x588;                                     // 0x588
    int x58c;                                     // 0x58c
    int x590;                                     // 0x590
    int x594;                                     // 0x594
    int x598;                                     // 0x598
    int x59c;                                     // 0x59c
    int x5a0;                                     // 0x5a0
    int x5a4;                                     // 0x5a4
    int x5a8;                                     // 0x5a8
    u8 x5ac;                                      // 0x5ac
    u8 stadium_kind;                              // 0x5ad
    u8 x5ae;                                      // 0x5ae
    u8 x5af;                                      // 0x5af
    int x5b0;                                     // 0x5b0
    int x5b4;                                     // 0x5b4
    int x5b8;                                     // 0x5b8
    int x5bc;                                     // 0x5bc
    int x5c0;                                     // 0x5c0
    int x5c4;                                     // 0x5c4
    int x5c8;                                     // 0x5c8
    int x5cc;                                     // 0x5cc
    int x5d0;                                     // 0x5d0
    int x5d4;                                     // 0x5d4
    int x5d8;                                     // 0x5d8
    int x5dc;                                     // 0x5dc
    int x5e0;                                     // 0x5e0
    int x5e4;                                     // 0x5e4
    int x5e8;                                     // 0x5e8
    int x5ec;                                     // 0x5ec
    int x5f0;                                     // 0x5f0
    int x5f4;                                     // 0x5f4
    int x5f8;                                     // 0x5f8
    int x5fc;                                     // 0x5fc
    int x600;                                     // 0x600
    int x604;                                     // 0x604
    int x608;                                     // 0x608
    int x60c;                                     // 0x60c
    int x610;                                     // 0x610
    int x614;                                     // 0x614
    int x618;                                     // 0x618
    int x61c;                                     // 0x61c
    int x620;                                     // 0x620
    int x624;                                     // 0x624
    int x628;                                     // 0x628
    int x62c;                                     // 0x62c
    int x630;                                     // 0x630
    int x634;                                     // 0x634
    int x638;                                     // 0x638
    GOBJ *cityui_statchart_gobj[5];               // 0x63c, indexed by ply
    GOBJ *cityui_statbar_gobj[4][9];              // 0x650,
    int x6e0;                                     // 0x6e0
    int x6e4;                                     // 0x6e4
    int x6e8;                                     // 0x6e8
    int x6ec;                                     // 0x6ec
    int x6f0;                                     // 0x6f0
    int x6f4;                                     // 0x6f4
    int x6f8;                                     // 0x6f8
    int x6fc;                                     // 0x6fc
    int x700;                                     // 0x700
    int x704;                                     // 0x704
    int x708;                                     // 0x708
    int x70c;                                     // 0x70c
    int x710;                                     // 0x710
    int x714;                                     // 0x714
    int x718;                                     // 0x718
    int x71c;                                     // 0x71c
    int x720;                                     // 0x720
    int x724;                                     // 0x724
    int x728;                                     // 0x728
    int x72c;                                     // 0x72c
    int x730;                                     // 0x730
    int x734;                                     // 0x734
    int x738;                                     // 0x738
    int x73c;                                     // 0x73c
    int x740;                                     // 0x740
    int x744;                                     // 0x744
    int x748;                                     // 0x748
    int x74c;                                     // 0x74c
    int x750;                                     // 0x750
    int x754;                                     // 0x754
    int x758;                                     // 0x758
    int x75c;                                     // 0x75c
    int x760;                                     // 0x760
    int x764;                                     // 0x764
    int x768;                                     // 0x768
    int x76c;                                     // 0x76c
    int x770;                                     // 0x770
    int x774;                                     // 0x774
    int x778;                                     // 0x778
    int x77c;                                     // 0x77c
    int x780;                                     // 0x780
    int x784;                                     // 0x784
    int x788;                                     // 0x788
    int x78c;                                     // 0x78c
    int x790;                                     // 0x790
    int x794;                                     // 0x794
    int x798;                                     // 0x798
    int x79c;                                     // 0x79c
    int x7a0;                                     // 0x7a0
    int x7a4;                                     // 0x7a4
    int x7a8;                                     // 0x7a8
    int x7ac;                                     // 0x7ac
    int x7b0;                                     // 0x7b0
    int x7b4;                                     // 0x7b4
    int x7b8;                                     // 0x7b8
    int x7bc;                                     // 0x7bc
    int x7c0;                                     // 0x7c0
    int x7c4;                                     // 0x7c4
    int x7c8;                                     // 0x7c8
    int x7cc;                                     // 0x7cc
    int x7d0;                                     // 0x7d0
    int x7d4;                                     // 0x7d4
    int x7d8;                                     // 0x7d8
    int x7dc;                                     // 0x7dc
    int x7e0;                                     // 0x7e0
    int x7e4;                                     // 0x7e4
    int x7e8;                                     // 0x7e8
    int x7ec;                                     // 0x7ec
    int x7f0;                                     // 0x7f0
    int x7f4;                                     // 0x7f4
    int x7f8;                                     // 0x7f8
    int x7fc;                                     // 0x7fc
    int x800;                                     // 0x800
    int x804;                                     // 0x804
    int x808;                                     // 0x808
    int x80c;                                     // 0x80c
    int x810;                                     // 0x810
    int x814;                                     // 0x814
    int x818;                                     // 0x818
    int x81c;                                     // 0x81c
    int x820;                                     // 0x820
    int x824;                                     // 0x824
    int x828;                                     // 0x828
    int x82c;                                     // 0x82c
    u8 x830;                                      // 0x830
    u8 x831;                                      // 0x831
    u8 x832;                                      // 0x832
    u8 frames_in_second;                          // 0x833
    int seconds_passed;                           // 0x834
    int x838;                                     // 0x838
    int x83c;                                     // 0x83c
    int x840;                                     // 0x840
    int x844;                                     // 0x844
    int x848;                                     // 0x848
    int x84c;                                     // 0x84c
    int x850;                                     // 0x850
    int x854;                                     // 0x854
    int x858;                                     // 0x858
    int x85c;                                     // 0x85c
    int x860;                                     // 0x860
    int x864;                                     // 0x864
    int x868;                                     // 0x868
    int x86c;                                     // 0x86c
    int x870;                                     // 0x870
    int x874;                                     // 0x874
    int x878;                                     // 0x878
    int x87c;                                     // 0x87c
    int x880;                                     // 0x880
    int x884;                                     // 0x884
    int x888;                                     // 0x888
    int x88c;                                     // 0x88c
    int x890;                                     // 0x890
    int x894;                                     // 0x894
    int x898;                                     // 0x898
    int x89c;                                     // 0x89c
    int x8a0;                                     // 0x8a0
    int x8a4;                                     // 0x8a4
    int x8a8;                                     // 0x8a8
    int x8ac;                                     // 0x8ac
    int x8b0;                                     // 0x8b0
    int x8b4;                                     // 0x8b4
    int x8b8;                                     // 0x8b8
    int x8bc;                                     // 0x8bc
    int x8c0;                                     // 0x8c0
    int x8c4;                                     // 0x8c4
    int x8c8;                                     // 0x8c8
    int x8cc;                                     // 0x8cc
    int x8d0;                                     // 0x8d0
    int x8d4;                                     // 0x8d4
    int x8d8;                                     // 0x8d8
    int x8dc;                                     // 0x8dc
    int x8e0;                                     // 0x8e0
    int x8e4;                                     // 0x8e4
    int x8e8;                                     // 0x8e8
    int x8ec;                                     // 0x8ec
    int x8f0;                                     // 0x8f0
    int x8f4;                                     // 0x8f4
    int x8f8;                                     // 0x8f8
    int x8fc;                                     // 0x8fc
    int x900;                                     // 0x900
    int x904;                                     // 0x904
    int x908;                                     // 0x908
    int x90c;                                     // 0x90c
    int x910;                                     // 0x910
    int x914;                                     // 0x914
    int x918;                                     // 0x918
    int x91c;                                     // 0x91c
    int x920;                                     // 0x920
    int x924;                                     // 0x924
    int x928;                                     // 0x928
    int x92c;                                     // 0x92c
    int x930;                                     // 0x930
    int x934;                                     // 0x934
    int x938;                                     // 0x938
    int x93c;                                     // 0x93c
    int x940;                                     // 0x940
    int x944;                                     // 0x944
    int x948;                                     // 0x948
    int x94c;                                     // 0x94c
    int x950;                                     // 0x950
    int x954;                                     // 0x954
    int x958;                                     // 0x958
    int x95c;                                     // 0x95c
    int x960;                                     // 0x960
    int x964;                                     // 0x964
    int x968;                                     // 0x968
    int x96c;                                     // 0x96c
    int x970;                                     // 0x970
    int x974;                                     // 0x974
    int x978;                                     // 0x978
    int x97c;                                     // 0x97c
    int x980;                                     // 0x980
    int x984;                                     // 0x984
    int x988;                                     // 0x988
    int x98c;                                     // 0x98c
    int x990;                                     // 0x990
    int x994;                                     // 0x994
    int x998;                                     // 0x998
    int x99c;                                     // 0x99c
    int x9a0;                                     // 0x9a0
    int x9a4;                                     // 0x9a4
    int x9a8;                                     // 0x9a8
    int x9ac;                                     // 0x9ac
    int x9b0;                                     // 0x9b0
    int x9b4;                                     // 0x9b4
    int x9b8;                                     // 0x9b8
    int x9bc;                                     // 0x9bc
    int x9c0;                                     // 0x9c0
    int x9c4;                                     // 0x9c4
    int x9c8;                                     // 0x9c8
    int x9cc;                                     // 0x9cc
    int x9d0;                                     // 0x9d0
    int x9d4;                                     // 0x9d4
    int x9d8;                                     // 0x9d8
    int x9dc;                                     // 0x9dc
    int x9e0;                                     // 0x9e0
    int x9e4;                                     // 0x9e4
    int x9e8;                                     // 0x9e8
    int x9ec;                                     // 0x9ec
    int x9f0;                                     // 0x9f0
    int x9f4;                                     // 0x9f4
    int x9f8;                                     // 0x9f8
    int x9fc;                                     // 0x9fc
    int xa00;                                     // 0xa00
    int xa04;                                     // 0xa04
    int xa08;                                     // 0xa08
    int xa0c;                                     // 0xa0c
    int xa10;                                     // 0xa10
    int xa14;                                     // 0xa14
    int xa18;                                     // 0xa18
    int xa1c;                                     // 0xa1c
    int xa20;                                     // 0xa20
    int xa24;                                     // 0xa24
    int xa28;                                     // 0xa28
    int xa2c;                                     // 0xa2c
    int xa30;                                     // 0xa30
    int xa34;                                     // 0xa34
    int xa38;                                     // 0xa38
    int xa3c;                                     // 0xa3c
    int xa40;                                     // 0xa40
    int xa44;                                     // 0xa44
    int xa48;                                     // 0xa48
    int xa4c;                                     // 0xa4c
    int xa50;                                     // 0xa50
    int xa54;                                     // 0xa54
    int xa58;                                     // 0xa58
    int xa5c;                                     // 0xa5c
    int xa60;                                     // 0xa60
    int xa64;                                     // 0xa64
    int xa68;                                     // 0xa68
    int xa6c;                                     // 0xa6c
    int xa70;                                     // 0xa70
    int xa74;                                     // 0xa74
    int xa78;                                     // 0xa78
    int xa7c;                                     // 0xa7c
    int xa80;                                     // 0xa80
    int xa84;                                     // 0xa84
    int xa88;                                     // 0xa88
    int xa8c;                                     // 0xa8c
    int xa90;                                     // 0xa90
    int xa94;                                     // 0xa94
    int xa98;                                     // 0xa98
    u16 a9c;                                      // 0xa9c
    int xaa0;                                     // 0xaa0
    int xaa4;                                     // 0xaa4
    int xaa8;                                     // 0xaa8
    int xaac;                                     // 0xaac
    int xab0;                                     // 0xab0
    int xab4;                                     // 0xab4
    int xab8;                                     // 0xab8
    int xabc;                                     // 0xabc
    int xac0;                                     // 0xac0
    int xac4;                                     // 0xac4
    int xac8;                                     // 0xac8
    int xacc;                                     // 0xacc
    int xad0;                                     // 0xad0
    int xad4;                                     // 0xad4
    int xad8;                                     // 0xad8
    int xadc;                                     // 0xadc
    int xae0;                                     // 0xae0
    int xae4;                                     // 0xae4
    int xae8;                                     // 0xae8
    int xaec;                                     // 0xaec
    int xaf0;                                     // 0xaf0
    int xaf4;                                     // 0xaf4
    int xaf8;                                     // 0xaf8
    int xafc;                                     // 0xafc
    int xb00;                                     // 0xb00
    int xb04;                                     // 0xb04
    int xb08;                                     // 0xb08
    JOBJSet *ScInfPlynm_scene_models;             // 0xb0c
    int xb10;                                     // 0xb10
    int xb14;                                     // 0xb14
    int xb18;                                     // 0xb18
    int xb1c;                                     // 0xb1c
    int xb20;                                     // 0xb20
    GOBJ *plynm_gobj[4];                          // 0xb24, these are the player indicators in the bottom left of splitscreen matches
    int xb34;                                     // 0xb34
    int xb38;                                     // 0xb38
    int xb3c;                                     // 0xb3c
    int xb40;                                     // 0xb40
    int xb44;                                     // 0xb44
    GOBJ *plyview_pos_gobj;                       // 0xb48
    int xb4c;                                     // 0xb4c
    int xb50;                                     // 0xb50
    int xb54;                                     // 0xb54
    int xb58;                                     // 0xb58
    int xb5c;                                     // 0xb5c
    int xb60;                                     // 0xb60
    int xb64;                                     // 0xb64
    int xb68;                                     // 0xb68
    int xb6c;                                     // 0xb6c
    int xb70;                                     // 0xb70
    int xb74;                                     // 0xb74
    int xb78;                                     // 0xb78
    int xb7c;                                     // 0xb7c
    int xb80;                                     // 0xb80
    int xb84;                                     // 0xb84
    int xb88;                                     // 0xb88
    int xb8c;                                     // 0xb8c
    int xb90;                                     // 0xb90
    int xb94;                                     // 0xb94
    int xb98;                                     // 0xb98
    int xb9c;                                     // 0xb9c
    int xba0;                                     // 0xba0
    int xba4;                                     // 0xba4
    int xba8;                                     // 0xba8
    int xbac;                                     // 0xbac
    int xbb0;                                     // 0xbb0
    int xbb4;                                     // 0xbb4
    int xbb8;                                     // 0xbb8
    int xbbc;                                     // 0xbbc
    int xbc0;                                     // 0xbc0
    int xbc4;                                     // 0xbc4
    int xbc8;                                     // 0xbc8
    int xbcc;                                     // 0xbcc
    int xbd0;                                     // 0xbd0
    int xbd4;                                     // 0xbd4
    int xbd8;                                     // 0xbd8
    int xbdc;                                     // 0xbdc
    int xbe0;                                     // 0xbe0
    int xbe4;                                     // 0xbe4
    int xbe8;                                     // 0xbe8
    int xbec;                                     // 0xbec
    int xbf0;                                     // 0xbf0
    int xbf4;                                     // 0xbf4
    int xbf8;                                     // 0xbf8
    int xbfc;                                     // 0xbfc
    int xc00;                                     // 0xc00
    int xc04;                                     // 0xc04
    int xc08;                                     // 0xc08
    int xc0c;                                     // 0xc0c
    int xc10;                                     // 0xc10
    int xc14;                                     // 0xc14
    int xc18;                                     // 0xc18
    int xc1c;                                     // 0xc1c
    int xc20;                                     // 0xc20
    int xc24;                                     // 0xc24
    int xc28;                                     // 0xc28
    int xc2c;                                     // 0xc2c
    int xc30;                                     // 0xc30
    int xc34;                                     // 0xc34
    int xc38;                                     // 0xc38
    int xc3c;                                     // 0xc3c
    int xc40;                                     // 0xc40
    int xc44;                                     // 0xc44
    int xc48;                                     // 0xc48
    int xc4c;                                     // 0xc4c
    int xc50;                                     // 0xc50
    int xc54;                                     // 0xc54
    int xc58;                                     // 0xc58
    int xc5c;                                     // 0xc5c
    struct                                        // 0xc60
    {
        int x0;                                   // 0x4
        int x4;                                   // 0x4
        GOBJ *score_gobj[4];                // 0x08
        GOBJ *machineicon_gobj[4];                // 0x18
    } highjump_hud;
    int xc88;                                     // 0xc88
    struct                                        // 0xc8c
    {
        int x0;                                   // 0x4
        int x4;                                   // 0x4
        GOBJ *score_gobj[4];                // 0x08
        GOBJ *machineicon_gobj[4];                // 0x18
    } airglider_hud;
    int xcb4;                                     // 0xcb4
    int xcb8;                                     // 0xcb8
    int xcbc;                                     // 0xcbc
    int xcc0;                                     // 0xcc0
    int xcc4;                                     // 0xcc4
    int xcc8;                                     // 0xcc8
    int xccc;                                     // 0xccc
    int xcd0;                                     // 0xcd0
    int xcd4;                                     // 0xcd4
    int xcd8;                                     // 0xcd8
    int xcdc;                                     // 0xcdc
    int xce0;                                     // 0xce0
    int xce4;                                     // 0xce4
    int xce8;                                     // 0xce8
    int xcec;                                     // 0xcec
    int xcf0;                                     // 0xcf0
    int xcf4;                                     // 0xcf4
    int xcf8;                                     // 0xcf8
    int xcfc;                                     // 0xcfc
} Game3dData;

// Reward type encoding for RewardEntry.reward_type
typedef enum RewardType
{
    REWARD_FILLER           = 0x00, // Checkbox filler ("Check off an empty box of your choice!")
    REWARD_BONUS_MOVIE      = 0x01, // Bonus movie (Air Ride)
    REWARD_EXTRA_RULE       = 0x02, // Extra rule (Top Ride only)
    REWARD_STADIUM          = 0x03, // New stadium (City Trial only)
    REWARD_SOUND_TEST       = 0x04, // Sound test entry (param=0x01)
    REWARD_MUSIC            = 0x05, // Music track (param=0x01)
    REWARD_ENDING           = 0x06, // Ending movie (param=0x02)
    REWARD_COURSE           = 0x07, // Course unlock (Nebula Belt, Air Ride only)
    REWARD_PAUSE_POWERUPS   = 0x08, // Pause screen power-up display (City Trial only)
    REWARD_MACHINE_WINGED_STAR     = 0x09,
    REWARD_MACHINE_WAGON_STAR      = 0x0A,
    REWARD_MACHINE_SWERVE_STAR     = 0x0B,
    REWARD_MACHINE_BULK_STAR       = 0x0C,
    REWARD_MACHINE_WHEELIE_BIKE    = 0x0D,
    REWARD_MACHINE_SLICK_STAR      = 0x0E,
    REWARD_MACHINE_FORMULA_STAR    = 0x0F,
    REWARD_MACHINE_SHADOW_STAR     = 0x10,
    REWARD_MACHINE_WHEELIE_SCOOTER = 0x11,
    REWARD_MACHINE_ROCKET_STAR     = 0x12,
    REWARD_MACHINE_TURBO_STAR      = 0x13,
    REWARD_MACHINE_JET_STAR        = 0x14,
    REWARD_MACHINE_REX_WHEELIE     = 0x15,
    REWARD_KING_DEDEDE      = 0x16, // King Dedede character
    REWARD_META_KNIGHT      = 0x17, // Meta Knight character
    REWARD_DRAGOON          = 0x18, // Dragoon complete machine (City Trial)
    REWARD_HYDRA            = 0x19, // Hydra complete machine (City Trial)
    REWARD_DRAGOON_PART_A   = 0x1A, // Dragoon Part A (City Trial)
    REWARD_DRAGOON_PART_B   = 0x1B, // Dragoon Part B (City Trial)
    REWARD_DRAGOON_PART_C   = 0x1C, // Dragoon Part C (City Trial)
    REWARD_HYDRA_PART_X     = 0x1D, // Hydra Part X (City Trial)
    REWARD_HYDRA_PART_Y     = 0x1E, // Hydra Part Y (City Trial)
    REWARD_HYDRA_PART_Z     = 0x1F, // Hydra Part Z (City Trial)
    REWARD_COLOR_GREEN      = 0x20, // Green Kirby
    REWARD_COLOR_PURPLE     = 0x21, // Purple Kirby
    REWARD_COLOR_BROWN      = 0x22, // Brown Kirby
    REWARD_COLOR_WHITE      = 0x23, // White Kirby
    REWARD_ITEM_CHICKIE     = 0x24, // New Item: Chickie (Top Ride)
    REWARD_ITEM_WHO_PAINT   = 0x25, // New Item: Who? Paint (Top Ride)
    REWARD_ITEM_LANTERN     = 0x26, // New Item: Lantern (Top Ride)
} RewardType;

typedef enum RewardParam
{
    REWARDPARAM_NONE    = 0x00, // Default for most reward types
    REWARDPARAM_AUDIO   = 0x01, // Sound test / music rewards
    REWARDPARAM_ENDING  = 0x02, // Ending movie reward
} RewardParam;

typedef struct RewardEntry
{
    u8 reward_type;     // 0x00, RewardType enum value (u8 to preserve 3-byte struct layout)
    u8 reward_param;    // 0x01, RewardParam enum value
    u8 clear_kind;      // 0x02, which ClearKind (0-119) this reward maps to
} RewardEntry;

// Indexed by RewardType. Returns "?" for unrecognized values.
static inline const char *Reward_TypeName(u8 rtype)
{
    static const char *kRewardTypeNames[] = {
        [REWARD_FILLER]                  = "Filler",
        [REWARD_BONUS_MOVIE]             = "BonusMovie",
        [REWARD_EXTRA_RULE]              = "ExtraRule",
        [REWARD_STADIUM]                 = "Stadium",
        [REWARD_SOUND_TEST]              = "SoundTest",
        [REWARD_MUSIC]                   = "Music",
        [REWARD_ENDING]                  = "Ending",
        [REWARD_COURSE]                  = "Course",
        [REWARD_PAUSE_POWERUPS]          = "PausePowerups",
        [REWARD_MACHINE_WINGED_STAR]     = "Machine:WingedStar",
        [REWARD_MACHINE_WAGON_STAR]      = "Machine:WagonStar",
        [REWARD_MACHINE_SWERVE_STAR]     = "Machine:SwerveStar",
        [REWARD_MACHINE_BULK_STAR]       = "Machine:BulkStar",
        [REWARD_MACHINE_WHEELIE_BIKE]    = "Machine:WheelieBike",
        [REWARD_MACHINE_SLICK_STAR]      = "Machine:SlickStar",
        [REWARD_MACHINE_FORMULA_STAR]    = "Machine:FormulaStar",
        [REWARD_MACHINE_SHADOW_STAR]     = "Machine:ShadowStar",
        [REWARD_MACHINE_WHEELIE_SCOOTER] = "Machine:WheelieScooter",
        [REWARD_MACHINE_ROCKET_STAR]     = "Machine:RocketStar",
        [REWARD_MACHINE_TURBO_STAR]      = "Machine:TurboStar",
        [REWARD_MACHINE_JET_STAR]        = "Machine:JetStar",
        [REWARD_MACHINE_REX_WHEELIE]     = "Machine:RexWheelie",
        [REWARD_KING_DEDEDE]             = "Char:KingDedede",
        [REWARD_META_KNIGHT]             = "Char:MetaKnight",
        [REWARD_DRAGOON]                 = "Dragoon",
        [REWARD_HYDRA]                   = "Hydra",
        [REWARD_DRAGOON_PART_A]          = "DragoonPartA",
        [REWARD_DRAGOON_PART_B]          = "DragoonPartB",
        [REWARD_DRAGOON_PART_C]          = "DragoonPartC",
        [REWARD_HYDRA_PART_X]            = "HydraPartX",
        [REWARD_HYDRA_PART_Y]            = "HydraPartY",
        [REWARD_HYDRA_PART_Z]            = "HydraPartZ",
        [REWARD_COLOR_GREEN]             = "Color:Green",
        [REWARD_COLOR_PURPLE]            = "Color:Purple",
        [REWARD_COLOR_BROWN]             = "Color:Brown",
        [REWARD_COLOR_WHITE]             = "Color:White",
        [REWARD_ITEM_CHICKIE]            = "Item:Chickie",
        [REWARD_ITEM_WHO_PAINT]          = "Item:WhoPaint",
        [REWARD_ITEM_LANTERN]            = "Item:Lantern",
    };
    if (rtype < (sizeof(kRewardTypeNames) / sizeof(kRewardTypeNames[0])) && kRewardTypeNames[rtype])
        return kRewardTypeNames[rtype];
    return "?";
}

typedef struct TopRideStats // 0x34 = 52 bytes. see gmGetClearcheckerType1_2Ptr
{
    u8 x0;                  // 0x00, unknown
    u8 x1;                  // 0x01, unknown
    u8 counters_02[3];      // 0x02, 3 byte-sized stat counters
    u8 pad_05;              // 0x05, padding (for halfword alignment)
    u16 counter_06;         // 0x06, halfword counter (likely total laps)
    u8 counters_08[37];     // 0x08, 37 byte-sized stat counters
    u8 flags_2d;            // 0x2D, bitfield (7 bits used, likely per-course flags)
    u8 flags_2e;            // 0x2E, bitfield (7 bits used, likely per-course flags)
    u8 flags_2f;            // 0x2F, bitfield (7 bits used, likely per-course flags)
    u8 counters_30[2];      // 0x30, 2 byte-sized stat counters
    u8 pad_32[2];           // 0x32, unused
} TopRideStats;

typedef struct GameClearData // 0xF4 bytes per mode. see gmGetClearcheckerTypeP
{
    u8 new_unlock_flag;     // 0x00, nonzero when new unlocks exist requiring visual update
    u8 display_state;       // 0x01, high nibble = pending new unlocks, low nibble bit 0 = shown/acknowledged
    u8 checkbox_filler_num;      // 0x02, number of checkbox fillers available to use
    u8 checkbox_filler_list_len; // 0x03, length of filler list shown in checklist UI (max 5)
    u8 grid_mapping[120];   // 0x04, grid_mapping[clear_kind] = visual_position (0-119, col = pos%12, row = pos/12)
    struct
    {
        u8 x0_80 : 1;       // 0x80
        u8 x0_40 : 1;       // 0x40
        u8 x0_20 : 1;       // 0x20
        u8 is_visible : 1;  // 0x10, is visible in the checklist
        u8 has_reward : 1;  // 0x08, set by Checklist_SetRewardFlagOnUnlocks when slot is unlocked
        u8 is_unlocked : 1; // 0x04, raised after displaying the unlocked animation
        u8 is_filler : 1;   // 0x02, checkbox filler was used
        u8 is_new : 1;      // 0x01, raised when objective is completed, pending acknowledgement
    } clear[120];           // 0x7C
} GameClearData;

typedef struct gmGameParams
{
    int x0;              // 0x00
    int x4;              // 0x04
    int x8;              // 0x08
    int xc;              // 0x0c
    int x10;             // 0x10
    int x14;             // 0x14
    int patch_max;       // 0x18, max stat patch value (returned by Patch_GetMaxValue as s8)
    int patch_min;       // 0x1c, min stat patch value (returned by Patch_GetMinValue as s8)
} gmGameParams;

typedef struct gmDataAll
{
    gmGameParams *game_params; // 0x0, game parameters including patch min/max
    struct // 0x4
    {
        int x0;                  // unk
        int weights[STKIND_NUM]; // 0x4, indexed by StadiumKind
    } *stadium_weights;
    struct // 0x8
    {
        u8 city_kind; // 0x0
        u8 gr_kind;   // 0x1
        u8 x2;        // 0x2
        u8 x3;        // 0x3
        u8 x4;        // 0x4
        u8 x5;        // 0x5
    } *stadium_desc;  // array of these, STKIND_NUM
} gmDataAll;

typedef struct PlayerData
{
    u8 x0[0x34];
    float hp;                  // 0x34
    float max_hp;              // 0x38
    GOBJ *rider_gobj;          // 0x3C
    GOBJ *machine_gobj;        // 0x40
    union {                    // 0x44
        struct {
            float weight;
            float boost;
            float top_speed;
            float turn;
            float charge;
            float glide;
            float offense;
            float defense;
            float hp;
        };
        float values[9];
    } stats;
    u8 x68[0x8B0 - 0x68];
    u32 objects_destroyed_num;  // 0x8B0 running total of number of breakable objects destroyed (e.g. star pole, coral, rocks, houses, trees)
    u8 x8B4[0x908 - 0x8B4];
    u16 x_bit15 : 1;            // bit 15 (MSB)
    u16 x908_flag6 : 1;         // bit 14 (byte 0x908, bit 6)
    u16 x908_flag5 : 1;         // bit 13 (byte 0x908, bit 5)
    u16 hydra_piece_2 : 1;      // bit 12 (byte 0x908, bit 4)
    u16 hydra_piece_1 : 1;      // bit 11 (byte 0x908, bit 3)
    u16 hydra_piece_0 : 1;      // bit 10 (byte 0x908, bit 2)
    u16 dragoon_piece_2 : 1;    // bit 9  (halfword bit 9)
    u16 dragoon_piece_1 : 1;    // bit 8  (halfword bit 8)
    u16 dragoon_piece_0 : 1;    // bit 7  (halfword bit 7)
    u16 x_bits0_6 : 7;          // bits 6-0 (LSB side)
    u8 x90A[0x90C - 0x90A];
} PlayerData;

typedef struct LegendaryPieceData           // 80ae2cec
{                                           //
    void *config;                           // 0x0, pointer to legendary_pieces config data (adjusted by player count)
    struct                                  // 0x38 bytes each
    {                                       // machine[0] = Dragoon, machine[1] = Hydra
        u8 next_piece_index;                // 0x0, which piece to spawn next (0-2)
        float spawn_progress[3];            // 0x4, match progress threshold for each piece to spawn
        int item_kind[3];                   // 0x10, ITKIND for each piece (Dragoon: 0x3a-0x3c, Hydra: 0x37-0x39)
        int x1c[3];                         // 0x1c, set on spawn, initialized to -1
        int x28[3];                         // 0x28, set on spawn, initialized to -1
        u8 req_spawn : 1;                   // 0x34, 0x80, set when match progress passes threshold
        u8 is_enabled : 1;                  // 0x34, 0x40, set during init if rng passes spawn chance
    } machine[2];                           //
} LegendaryPieceData;

typedef struct grBoxGeneObj              // r13 + 0x608
{                                        //
    struct                               //
    {                                    //
        u8 it_kind[ITKIND_NUM - 1];      // 0x0
        u8 chance[ITKIND_NUM - 1];       // 0x44
        u8 num;                          // 0x88
    } item_group_spawn[BOXKIND_NUM];     // is used for sky drops and box breaks
    u8 sameitem_it_kind[ITKIND_NUM - 1]; // 0x19b
    u8 sameitem_chance[ITKIND_NUM - 1];  // 0x1df
    u8 sameitem_num;                     // 0x223
    // the following item pool is used when spawning more than 1 patch power up from a blue box
    u8 subsequent_it_kind[40]; // 0x224
    u8 subsequent_chance[40];  // 0x24c
    u8 subsequent_num;         // 0x274
    int x278;                  // 0x278
} grBoxGeneObj;

typedef struct grBoxGeneInfo // r13 + 0x610
{                            //
    int x0;                  // 0x0
    int x4;                  // 0x4
    struct                   // 0x8
    {                        //
        void *x0;            // 0x0
        ItemFallDesc *desc;  // 0x4
        int desc_num;        // 0x8
    } *fall_timer_desc;      //
    struct                   // 0xc, referred to as item lots data @ 800ec068?
    {
        struct // 0x0
        {
            u8 blue_small_chance;
            u8 blue_medium_chance;
            u8 blue_large_chance;
            u8 green_small_chance;
            u8 green_medium_chance;
            u8 green_large_chance;
            u8 red_small_chance;
            u8 red_medium_chance;
            u8 red_large_chance;
        } *box_spawn_chances;
        struct // 0x4
        {
            int it_kind;
            u16 fall_chance[STGROUP_NUM];
            u16 same_item_chance;
            u16 unk_chance;
        } *item_spawn;
        int item_spawn_num; // 0x8
        struct              // 0xc, 2 of these: [0] = Dragoon, [1] = Hydra
        {
            u8 chance_to_spawn;             // 0x0, probability out of 100
            u8 spawn_progress_range[3][2];  // 0x1, [piece_index][0=min, 1=max] match progress values
        } *legendary_pieces; //
        struct               // 0x10
        {
            ItemKind it_kind;
            u16 chance[EVKIND_NUM]; //
            u16 chance_unk1;        // 0x24, gets stored to unk_chance in grBoxGeneObj
        } *event_spawn;
        int event_spawn_num; // 0x14
        struct               // 0x18
        {
            int it_kind;        // 0x0
            u16 chance_dyna;         // 0x4, dyna blade hits/exits (patches-only pool; possibly other rare misc sources untested)
            u16 chance_tac;          // 0x6, hitting tac
            u16 chance_meteor;       // 0x8, drops after a meteor explodes
            u16 chance_destructible; // 0xA, yaku-break objects: star pole (gryakubreakcoral.c), event pillar + volcano walls (gryakubreakrock.c), houses (gryakubreakhouse.c)
            u16 chance_chamber; // 0xC, secret chamber
            u16 chance_ufo;     // 0xE, ufo
        } *event_source_drop;
        int event_source_drop_num;    // 0x1c
        int x20;                      // 0x20
        int x24;                      // 0x24
        int x28;                      // 0x28
        int x2c;                      // 0x2c
        int x30;                      // 0x30
        int x34;                      // 0x34
        int x38;                      // 0x38
        int x3c;                      // 0x3c
        int x40;                      // 0x40
        int x44;                      // 0x44
        int x48;                      // 0x48
        int x4c;                      // 0x4c
        int x50;                      // 0x50
        int x54;                      // 0x54
        int x58;                      // 0x58
        int x5c;                      // 0x5c
        int x60;                      // 0x60
        int x64;                      // 0x64
        int x68;                      // 0x68
        int x6c;                      // 0x6c
        int x70;                      // 0x70
        int x74;                      // 0x74
        int x78;                      // 0x78
        int x7c;                      // 0x7c
        int x80;                      // 0x80
        int x84;                      // 0x84
        int x88;                      // 0x88
        int x8c;                      // 0x8c
        int x90;                      // 0x90
        int x94;                      // 0x94
        int x98;                      // 0x98
        int x9c;                      // 0x9c
    } *item_desc;                     //
    int x10;                          // 0x10, referred to as item lots data @ 800ec068?
    int x14;                          // 0x14, referred to as item lots data @ 800ec068?
    int x18;                          // 0x18
    int item_area_pos_num;            // 0x1c, number of item areas
    int cur_num_items;                // 0x20, current count of items spawned. Inc/dec by CityItemSpawn_IncrementNum/DecrementNum. The actual cap is ItemFallDesc.item_max.
    int total_spawn_count;            // 0x24, lifetime cumulative spawn count (only increments, written by CityItemSpawn_IncrementNum on positive deltas)
    int total_num;                    // 0x28
    int spawn_timer;                  // 0x2c
    int x30;                          // 0x30
    ItemKind same_item_it_kind;       // 0x34, item used when EVKIND_SAMEITEM is active
    int event_active_flags;           // 0x38 (or'd with EventKind)
    ItemFallDesc cur_event_fall_desc; // 0x3c
    int x4c;                          // 0x4c
    int x50;                          // 0x50
    int x54;                          // 0x54
    int x58;                          // 0x58
    int x5c;                          // 0x5c
    int x60;                          // 0x60
    int x64;                          // 0x64
    int x68;                          // 0x68
    int x6c;                          // 0x6c
    int x70;                          // 0x70
    int x74;                          // 0x74
    int x78;                          // 0x78
    int x7c;                          // 0x7c
    int x80;                          // 0x80
    int x84;                          // 0x84
    int x88;                          // 0x88
    int x8c;                          // 0x8c
    int x90;                          // 0x90
    int x94;                          // 0x94
    int x98;                          // 0x98
    int x9c;                          // 0x9c
    int xa0;                          // 0xa0
    int xa4;                          // 0xa4
    int xa8;                          // 0xa8
    int xac;                          // 0xac
    int xb0;                          // 0xb0
    int xb4;                          // 0xb4
    int xb8;                          // 0xb8
    int xbc;                          // 0xbc
    int xc0;                          // 0xc0
    int xc4;                          // 0xc4
    int xc8;                          // 0xc8
    int xcc;                          // 0xcc
    int xd0;                          // 0xd0
    int xd4;                          // 0xd4
    int xd8;                          // 0xd8
    int xdc;                          // 0xdc
    int xe0;                          // 0xe0
    int xe4;                          // 0xe4
    int xe8;                          // 0xe8
    int xec;                          // 0xec
    int xf0;                          // 0xf0
    int xf4;                          // 0xf4
    int xf8;                          // 0xf8
    int xfc;                          // 0xfc
    int x100;                         // 0x100
    int x104;                         // 0x104
    int x108;                         // 0x108
    int x10c;                         // 0x10c
    int x110;                         // 0x110
    int x114;                         // 0x114
    int x118;                         // 0x118
    int x11c;                         // 0x11c
    int x120;                         // 0x120
    int x124;                         // 0x124
    int x128;                         // 0x128
    int x12c;                         // 0x12c
    int x130;                         // 0x130
    int x134;                         // 0x134
    int x138;                         // 0x138
    int x13c;                         // 0x13c
    int x140;                         // 0x140
    int x144;                         // 0x144
    int x148;                         // 0x148
    int x14c;                         // 0x14c
    int x150;                         // 0x150
    int x154;                         // 0x154
    int x158;                         // 0x158
    int x15c;                         // 0x15c
    int x160;                         // 0x160
    int x164;                         // 0x164
    int x168;                         // 0x168
    int x16c;                         // 0x16c
    int x170;                         // 0x170
    int x174;                         // 0x174
    int x178;                         // 0x178
    int x17c;                         // 0x17c
    int x180;                         // 0x180
    int x184;                         // 0x184
    int x188;                         // 0x188
    int x18c;                         // 0x18c
    int x190;                         // 0x190
    int x194;                         // 0x194
    int x198;                         // 0x198
    int x19c;                         // 0x19c
    int x1a0;                         // 0x1a0
    int x1a4;                         // 0x1a4
    int x1a8;                         // 0x1a8
    int x1ac;                         // 0x1ac
    int x1b0;                         // 0x1b0
    int x1b4;                         // 0x1b4
    int x1b8;                         // 0x1b8
    int x1bc;                         // 0x1bc
    int x1c0;                         // 0x1c0
    int x1c4;                         // 0x1c4
    int x1c8;                         // 0x1c8
    int x1cc;                         // 0x1cc
    int x1d0;                         // 0x1d0
    int x1d4;                         // 0x1d4
    int x1d8;                         // 0x1d8
    int x1dc;                         // 0x1dc
    int x1e0;                         // 0x1e0
    int x1e4;                         // 0x1e4
    int x1e8;                         // 0x1e8
    int x1ec;                         // 0x1ec
    int x1f0;                         // 0x1f0
    int x1f4;                         // 0x1f4
    int x1f8;                         // 0x1f8
    int x1fc;                         // 0x1fc
    int x200;                         // 0x200
    int x204;                         // 0x204
    int x208;                         // 0x208
    int x20c;                         // 0x20c
    int x210;                         // 0x210
    int x214;                         // 0x214
    int x218;                         // 0x218
    int x21c;                         // 0x21c
    int x220;                         // 0x220
    int x224;                         // 0x224
    int x228;                         // 0x228
    int x22c;                         // 0x22c
    int x230;                         // 0x230
    int x234;                         // 0x234
    int x238;                         // 0x238
    int x23c;                         // 0x23c
    int x240;                         // 0x240
    int x244;                         // 0x244
    int x248;                         // 0x248
    int x24c;                         // 0x24c
    int x250;                         // 0x250
    int x254;                         // 0x254
    int x258;                         // 0x258
    int x25c;                         // 0x25c
    int x260;                         // 0x260
    int x264;                         // 0x264
    int x268;                         // 0x268
    int x26c;                         // 0x26c
    int x270;                         // 0x270
    int x274;                         // 0x274
    int x278;                         // 0x278
    int x27c;                         // 0x27c
    int x280;                         // 0x280
    int x284;                         // 0x284
    int x288;                         // 0x288
    int x28c;                         // 0x28c
    int timer_minutes;                // 0x290
    int timer_seconds_in_minute;      // 0x294
    int timer_subseconds;             // 0x298, out of 100
    int match_subseconds_left;        // 0x29c,
    int match_initial_subseconds;     // 0x2a0, initial time at the beginning of the game
    float match_progress;             // 0x2a4, goes from 0 -> 1
    int flags_x2a8;                   // 0x2a8, 0x40 = is_match_intro
    int x2ac;                         // 0x2ac
    int x2b0;                         // 0x2b0
    int x2b4;                         // 0x2b4
    int x2b8;                         // 0x2b8
    int x2bc;                         // 0x2bc
    int x2c0;                         // 0x2c0
    int x2c4;                         // 0x2c4
    int x2c8;                         // 0x2c8
} grBoxGeneInfo;

typedef struct MnResultsCityBoard4Data
{
    u8 x0[0xc]; //
    struct
    {
        u8 xc;          // 0xc
        u8 color_idx;   // 0xd
        int x10;        // 0x10
        JOBJ *score_j;  // 0x14
        void *x18;      //
        void *x1c;      //
        void *x20;      //
        JOBJ *border_j; // 0x24
        JOBJ *plynum_j; // 0x28
        u8 x2c[0x58];   //
    } ply[4];
} MnResultsCityBoard4Data;

typedef struct BoxData
{
    u8 x0[0x148];        // 0x0
    HurtData *hurt_data; // 0x148
} BoxData;

typedef struct BGMDesc
{
    BGMKind bgm;
    int x4;
    char *path; // relative to /audio
    int xc;
} BGMDesc;

typedef struct PlyViewPosData
{
    int x0;                     // 0x0
    int x4;                     // 0x4
    int x8;                     // 0x8
    int xc;                     // 0xc
    int x10;                    // 0x10
    Vec3 plyview_center_pos[4]; // 0x14
} PlyViewPosData;

//////////////////////
// Static Variables //
//////////////////////

static LegendaryPieceData **stc_legendary_piece_data = (LegendaryPieceData **)(0x805dd0e0 + 0x600);
static grBoxGeneObj **stc_grBoxGeneObj = (grBoxGeneObj **)(0x805dd0e0 + 0x608);
static grBoxGeneInfo **stc_grBoxGeneInfo = (grBoxGeneInfo **)(0x805dd0e0 + 0x610);
static itCommonDataAll **stc_it_common_data = (itCommonDataAll **)(0x805dd0e0 + 0x7f0);
static cmMainParamCommon **stc_cm_main_param = (cmMainParamCommon **)0x8055747c;
static gmDataAll **stc_gmdataall = (gmDataAll **)(0x805dd0e0 + 0x494);
static int *stc_clearchecker_sfx_last_frame = (int *)(0x805dd0e0 + 0x4B0); // 0x805dd590, last frame ClearChecker_SetNewUnlock played its SFX (one-frame cooldown)
static int *stc_city_machine_num = (int *)(0x805dd0e0 + 0x754); //
static u8 *stc_city_starting_machine = (u8 *)0x80495816;
static PlayerData *stc_playerdata = (PlayerData *)0x8055a9f0; // 5 of these (slots 0-4). Vanilla iterates < 5; unused slots are set to PKIND_NONE via plSetPlayerKind. Slot 4 has no direct xrefs anywhere in the binary - it is allocated/iterated/zeroed but appears to be dead over-allocation (PKIND_DEMO is used by title screen, menu radar, and TR results, but always targets slots 0-3).
static u8 *stc_clear_num = (u8 *)0x805d51d0;                          // array indexed by GMMODE, stores clear count per mode
static RewardEntry **stc_reward_table_ptrs = (RewardEntry **)0x8049755C; // 3 pointers to per-mode reward tables. Indexed by GMMODE
static u8 *stc_special_rewards = (u8 *)0x804AD270;                     // 5 special reward indices per mode (15 bytes total)
static u8 **stc_audio_preview_tables = (u8 **)0x804AD2EC;              // 3 pointers to per-mode checklist audio preview tables ({reward_index, song_id} pairs, 0xFF-terminated). Indexed by GMMODE

static BGMDesc *stc_bgm_desc = (BGMDesc *)0x80498750;

///////////////
// Functions //
///////////////

TitleScreenData *TitleScreen_GetData();
GameData *Gm_GetGameData();
Game3dData *Gm_Get3dData();

void CityTrial_DecideStadium();

void Gm_HidePauseHUD();
void Gm_ShowHUD();

int Gm_IsInCity();
int CityTrial_IsInStadium();  // 0x8000ad48, checks if city_kind is a stadium (7-18)

GmIntroState Gm_GetIntroState();
CityMode Gm_GetCityMode();
AirRideMode Gm_GetAirRideMode();      // 0x8003d5f0 - returns GameData[0x35d]
int Gm_GetAirRidePlayerSlot();         // 0x8003d644 - returns GameData[0x35f], active player slot for Free Run / Time Attack
float Ply_GetCityStatNum(int ply, int stat_idx, int unk);
GOBJ *Ply_GetRiderGObj(int ply);
GOBJ *Ply_GetMachineGObj(int ply);
int Ply_GetColor(int ply);
int Ply_CheckIfCPU(int ply);
int Ply_IsViewOn(int ply);
int Ply_GetViewIndex(int ply);
RiderKind Ply_GetRiderKind2(int ply);
PKind Ply_GetPKind(int ply);
RiderKind Ply_GetRiderKind(int ply);
void Ply_SetRiderKind(RiderKind kind);
void Ply_SetMachineKind(int ply, MachineKind kind);
int Ply_GetMachineKind(int ply);
void Ply_SetMachineIsBike(int ply, int is_bike);
int Ply_GetMachineIsBike(int ply);
void Ply_AddDeath(int ply, DmgLog *dmg_log, int is_bike, MachineKind machine_kind);
void Ply_SetHP(int ply, float hp);
int Ply_GetAllUpCollected(int ply);
int Ply_SetAllUpCollected(int ply, int num);

void Gm_FadeOutMusic(int frame_duration);
int Gm_GetPlyViewNum();
ItemGroup Gm_GetItemGroup(ItemKind it_kind);
// Clear Checker core
u8 Checklist_GetRewardNum(GameMode gm);                                // 80049c20, returns number of rewards for a mode
u8 Checklist_GetClearKindFromRewardIndex(GameMode gm, u8 reward_index); // 80049c84, returns clear_kind for a reward index
int ClearChecker_CheckUnlocked(GameMode gm, u8 reward_index);          // 80049e24, checks has_reward bit for a reward index's clear_kind
void ClearChecker_SetNewUnlockSilent(GameMode gm, u8 clear_kind);     // 80049fcc, marks clear_kind newly completed without SFX (Top Ride checklist evaluator path)
void ClearChecker_SetNewUnlock(GameMode gm, u8 clear_kind);            // 8004a054, marks clear_kind as newly completed
int ClearChecker_GetFrameIndex(void);                                   // 80005ce0, returns current frame index used for SFX cooldown in SetNewUnlock
u8 ClearChecker_GetKindClear(GameMode gm, u8 clear_kind);              // 8004a130, returns status byte for a clear_kind
int ClearChecker_CheckForNewUnlocks(GameMode gm);                      // 8004a1a4, scans for is_new && !is_unlocked
void Checklist_SetRewardFlagOnUnlocks();                                // 8017df5c, sets has_reward on unlocked slots, rebuilds grid
void Checklist_BuildUnlockBitfields();                                  // 80007af0, caches unlock status into GameData + 0xD50 bitfields
int Checklist_IsCacheValid();                                           // 8007b650, returns 1 if unlock bitfield cache is valid
int Checklist_CheckCachedUnlock_AirRide(s8 reward_index);               // 80007e34, fast bit-test against cached Air Ride unlock bitfield
int Checklist_CheckCachedUnlock_CityTrial(s8 reward_index);             // 80007e8c, fast bit-test against cached City Trial unlock bitfield
GameClearData *gmGetClearcheckerTypeP(GameMode mode);                   // 800076a0, returns ClearCheckerData for mode
GameClearData *gmGetClearcheckerP();                                    // 80006c20, returns base ClearCheckerData (Air Ride)
u8 Gm_GetClearChecker();                                                // 8017cf14, returns ClearChecker UI state byte
void ClearChecker_GetRewardFromClearKind(GameMode gm, u8 clear_kind, u8 *out_reward_index, u8 *out_reward_param); // 80049ec4, reverse lookup: clear_kind → reward_index + reward_param
void ClearChecker_ResetAllData(void);                                    // 8000c604, resets all clear data for all 3 modes (erase menu)
int ClearChecker_ShouldShowNewUnlocks(GameMode gm);                      // 8000c6f0, returns 1 if mode has new unlocks pending display
void ClearChecker_MarkNewUnlocksShown(GameMode gm);                      // 8000c734, marks new unlocks as displayed for mode

// Grant a checkbox filler for the given mode. checkbox_filler_num is uncapped (u8, max 255).
// checkbox_filler_list_len controls the displayed filler icons in the UI (capped at 5).
static inline void Checklist_GrantFiller(GameMode mode)
{
    GameClearData *clear_data = gmGetClearcheckerTypeP(mode);
    clear_data->checkbox_filler_num++;
    if (clear_data->checkbox_filler_list_len < 5)
    {
        clear_data->checkbox_filler_list_len++;
    }
}

// Top Ride
s8 TopRide_GetSelectedCourse(void);                                      // 8000b2b8, returns currently selected Top Ride course index (0-6)
void TopRide_SetSelectedCourse(int course);                              // 8000b2dc, sets selected Top Ride course index
void TopRide_SetCourseValid(int valid);                                  // 8000b5ac, sets GameData.topride_course_valid
void TopRide_SetExtraUnlocks(int unlock0, int unlock1, int unlock2);     // 8000b5dc, sets GameData.topride_extra_unlocks[0..2]
TopRideMode TopRide_GetMode(void);                                       // 8003ea9c, returns Top Ride mode (0=Race, 1=Time, 2=Free)
int TopRide_GetTimeAttackPlayerSlot(void);                               // 8003eaf0, returns active player slot for Top Ride Time Attack
TopRideStats *TopRide_GetStats(void);                                    // 80287040, returns TopRideStats pointer (via gmGetClearcheckerType1_2Ptr)

// Top Ride Kirby (player) structs and globals - see topride.h

// Clear Checker reward query callers
int AirRide_CheckCourseUnlocked(s8 input);                              // 8000c0e0, checks reward index 34 (Nebula Belt) when input==8
int AirRide_CheckMachineUnlocked(s8 machine_class, s8 machine_id);      // 8000c364, maps to Air Ride reward indices 19-31 (machines 0x09-0x15)
int AirRide_CheckCharacterAvailable(CharacterKind ckind);               // 8002090c, checks if a CharacterKind is selectable on the Air Ride select screen
int AirRide_CheckCharacterUnlocked(s8 character);                       // 8000c488, maps 1→32 (Dedede), 2→33 (Meta Knight)
int CityTrial_CheckLegendaryMachineUnlocked(int machine);               // 8000c508, maps 4→34 (Hydra), 8→30 (Dragoon)
int AirRide_CheckBonusUnlocked(s8 bonus);                               // 8000c584, maps 1→35 (Bonus Movie), 2→36 (Ending)
int CityTrial_CheckStadiumIsUnlocked(s8 stadium_kind);                  // 8000c17c, maps StadiumKind 3-22 → reward indices 37-42
int Pause_CheckStatsUnlocked();                                         // 8000c768, checks City Trial reward index 43
int Gm_IsGrKindCity(GroundKind gr_kind);
int Gm_IsDestructionDerby();
void CitySelect_Cursor6Update(int ply, int color_idx);
void AirRideSelect_Cursor6Update(int ply, int color_idx);
int Gm_CheckPauseKind(PauseKind pause_kind);
void Gm_Pause(PauseKind pause_kind);
void Gm_Resume(PauseKind pause_kind);
void Gm_PlayPauseSFX();
void Gm_PauseAllSFX();
void Gm_ResumeAllSFX();

float Gm_GetDownVector(Vec3 *pos, Vec3 *out); // 800ceb18. unsure what the float is

void Gm_SetCameraNormal();
int Gm_IsDamageEnabled();
int Gm_IsReplay();

void Pad_StopRumbleAll();

int hash_32(const void *data, int size);
int hash_32_str(const void *data);
void Gm_LoadGroundFGMBank(GroundKind gr_kind); //

// Legendary Machine Pieces (Dragoon & Hydra)
void LegendaryPieces_Init();                                               // 800ecfac, initializes piece spawn data for City Trial
int CityItemSpawn_CheckToSpawnLegendaryPiece(float match_progress);        // 800ed2f0, checks if a piece should spawn based on match progress
void CityItemSpawn_SpawnLegendaryPiece(int spawner, int param_2, int param_3); // 800ed384, spawns the next legendary piece
void LegendaryPiece_MarkAsSpawned(int spawner, int item_kind);             // 80252f10, marks the item to spawn as a legendary piece
void LegendaryPiece_ClearSpawnRequest(int spawner);                        // 80252e74, clears pending spawn flag after piece spawns
int Ply_GetDragoonCollection(int ply);                                     // 8022cdc8, returns count of dragoon pieces collected (0-3)
void Ply_UpdateDragoonCollection(int ply, int piece_bits);                  // 8022cd64, OR's piece_bits into dragoon collection flags
int Ply_GetHydraCollection(int ply);                                       // 8022cd04, returns count of hydra pieces collected (0-3)
void Ply_UpdateHydraCollection(int ply, int piece_bits);                    // 8022cca0, OR's piece_bits into hydra collection flags
int Ply_GetHydraPieceMask(int ply);                                        // 8022cce8, raw 3-bit hydra piece bitmask (PlayerData+0x908 bits 2-4); cf. Ply_GetHydraCollection for the popcount
void Ply_SetHydraPieceMask(int ply, int mask);                             // 8022ccc8, overwrites the 3-bit hydra piece bitmask (not OR; cf. Ply_UpdateHydraCollection)
int Ply_GetDragoonPieceMask(int ply);                                      // 8022cdac, raw 3-bit dragoon piece bitmask (PlayerData+0x908 bits 7-9); cf. Ply_GetDragoonCollection for the popcount
void Ply_SetDragoonPieceMask(int ply, int mask);                           // 8022cd8c, overwrites the 3-bit dragoon piece bitmask (not OR; cf. Ply_UpdateDragoonCollection)
void Ply_OnLegendaryPieceCollect(int ply, int piece_count);                // 8027a4e8, plays SFX based on piece collection progress
void Ply_MarkLegendaryMachineAssembled(int ply, int machine_index);        // 80231198, marks legendary machine as assembled (0=Dragoon, 1=Hydra)
void Ply_PlayFGM(int fgm_id, int ply, int param_3);                       // 80277c84, plays a positional sound effect for a player

// Source enum passed to CityItem_GetEventItem; dispatched through a 13-entry
// jump table at 0x804a5290. Inputs 4-8, 10, 11 are unmapped (return -1).
typedef enum EventDropSource
{
    EVDROP_DYNA          = 0,  // Dyna Blade hits/exits
    EVDROP_TAC           = 1,  // Tac (cat enemy)
    EVDROP_METEOR        = 2,  // Meteor explosion
    EVDROP_DESTRUCTIBLE  = 3,  // yaku-break objects (rock/house/coral). Reached only via City_SpawnMiscItems descriptor, never as a literal.
    EVDROP_CHAMBER       = 9,  // Secret Chamber
    EVDROP_UFO           = 12, // UFO
} EventDropSource;

ItemKind CityItem_GetEventItem(EventDropSource source);                    // 80254114, weighted random pick from event_source_drop[] using the source's column. Returns -1 if no item.
ItemKind _CityItem_GetEventItem(EventDropSource source);                   // 800ebe44, internal - public CityItem_GetEventItem just tail-calls this.
void City_SpawnMiscItems(int *desc, ...);                                  // 80104db0, public dispatch: desc[8]==1 → directed cone (shootPowerUps_); desc[8]==0 → omnidirectional (_City_SpawnMiscItems). desc[7] = drop_source (-1 → fall back to CityEvent_GetRandomItem).

// Spawn one item with a randomized throw velocity. Builds a spawn descriptor
// (CityItemSpawn_CalculateLifetime → CityItem_Create), validates the throw
// direction (asserts in itlib.c:853-856), and writes velocity + flags onto
// the resulting item. Used by the patch-drop pipeline (spawn_group=3) and
// yakumono-break helpers (spawn_group=4/5/6). See docs/patch-drop-system.md.
void CityItem_Throw(ItemKind item_kind, int spawn_group, Vec3 *position, Vec3 *velocity, int item_flags, f32 scalar_a, f32 scalar_b); // 80253ce4

// Yaku-break (destructible object) drop handlers. All three feed into City_SpawnMiscItems.
void GrYakuBreakRock_DropItems(int param);   // 8010203c, gryakubreakrock.c - volcano walls + event pillars
void GrYakuBreakHouse_DropItems(int param);  // 80102794, gryakubreakhouse.c - destructible houses
void GrYakuBreakCoral_DropItems(int param);  // 801040fc, gryakubreakcoral.c - "BigStar" / star pole

// Legendary Machine Assembly
typedef struct LegendaryAssemblyParams
{
    int machine_index;    // 0x00, 0 = Dragoon, 1 = Hydra
    u8 ply;               // 0x04, player index
    u8 pad[3];            // 0x05
    Vec3 pos;             // 0x08, machine position
    Vec3 up;              // 0x14, machine up vector
    Vec3 forward;         // 0x20, machine forward vector
} LegendaryAssemblyParams;

void LegendaryMachine_StartAssembly(LegendaryAssemblyParams *params);      // 80283cf0, starts assembly cinematic

#endif