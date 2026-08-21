#ifndef HOSHISETTINGS_H
#define HOSHISETTINGS_H

#include "scene.h"

typedef enum OptionKind
{
    OPTKIND_VALUE,
    OPTKIND_MENU,
    OPTKIND_SCENE,
    OPTKIND_NUM,
    OPTKIND_ACTION,
} OptionKind;

typedef enum MenuPriority
{
    MENUPRI_VERYHIGH,
    MENUPRI_HIGH,
    MENUPRI_NORMAL,
    MENUPRI_LOW,
    MENUPRI_VERYLOW,
} MenuPriority;

typedef struct MenuDesc MenuDesc;
typedef struct OptionDesc OptionDesc;

struct OptionDesc
{
    char *name;
    char *description;
    OptionKind kind : 16;
    MenuPriority pri : 15;
    unsigned int no_save : 1; // OPTKIND_VALUE: keep this option out of the memory card block
    int *val;
    int min;
    union
    {
        int max;
        int value_num;
    };
    void (*on_change)(int val);
    union
    {
        struct
        {
            char **value_names;
        };
        struct
        {
            MenuDesc *menu_ptr;
        };
        struct
        {
            MajorKind major_idx;
        };
        struct
        {
            int (*on_action)(OptionDesc *self);
            void *user_data;
        };
    };

};

struct MenuDesc
{
    MenuDesc *prev;
    u16 cursor;
    u16 scroll;
    u16 option_num;
    OptionDesc *options[];
};
#pragma pack(push, 1) // Align to 1-byte boundaries
typedef struct MenuSave
{
    u16 hash;
    u8 val;
} MenuSave;
#pragma pack(pop)

#endif