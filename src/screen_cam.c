/*---------------------------------------------------------------------------*
    Handles lowest level save file operations.
        - allocates runtime save struct
        - creates save file on memcard
        - loads save file from memcard
        - returns runtime save file struct
 *---------------------------------------------------------------------------*/

#include "os.h"
#include "obj.h"
#include "text.h"
#include "inline.h"
#include "hoshi.h"
#include "hoshi/screen_cam.h"
#include <string.h>

// Text_CreateCanvas returns the new canvas's ordinal among the canvases already bound to the
// same SIS slot, and Text_CreateTextManual (0x8044f128) selects by that same ordinal. Scenes create
// their own slot-1 canvases before Hook_SceneChange runs, so this is not always 0.
static int stc_screen_canvas_idx;

void ScreenCam_Create()
{
    // create ortho text canvas
    stc_screen_canvas_idx = Text_CreateCanvas(1, 0, 0, 0, 0, HOSHI_SCREENCAM_GXLINK, 0, 63);

    // // create ortho screen cam
    // GOBJ *g = GOBJ_EZCreator(0, 0, 0,
    //                 0, 0,
    //                 HSD_OBJKIND_COBJ, (COBJDesc *)0x805096a0,
    //                 0, 0,
    //                 0, HOSHI_SCREENCAM_GXLINK, 63);
    // COBJ *c = g->hsd_object;
    // CObj_SetOrtho(c, 0, -480, 0, 640);
}
Text *ScreenCam_CreateText()
{
    return Text_CreateText(1, stc_screen_canvas_idx);
}
void ScreenCam_Init()
{
    CODEPATCH_REPLACEFUNC(Hoshi_CreateScreenText, ScreenCam_CreateText); // install function to get screen canvas idx 
}