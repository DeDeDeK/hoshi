#ifndef MEX_H_STRUCTS
#define MEX_H_STRUCTS

/*
 *  defines typedefs for structs used throughout m-ex
 */

// Basic Data Structs
typedef struct Vec2 Vec2;
typedef struct Vec3 Vec3;
typedef struct Vec4 Vec4;

// OS
typedef struct OSInfo OSInfo;
typedef struct OSCalendarTime OSCalendarTime;
typedef struct CARDStat CARDStat;
typedef struct OSAlarm OSAlarm;
typedef struct OSThread OSThread;
typedef struct OSThreadQueue OSThreadQueue;
typedef struct OSThreadLink OSThreadLink;
typedef struct OSContext OSContext;
typedef struct OSMutex OSMutex;
typedef struct OSMutexQueue OSMutexQueue;
typedef struct OSMutexLink OSMutexLink;
typedef struct OSHeapCell OSHeapCell;
typedef struct CARDFileInfo CARDFileInfo;
typedef struct RGB565 RGB565;
typedef struct DVDDiskID DVDDiskID;
typedef struct DVDCommandBlock DVDCommandBlock;
typedef struct DVDFileInfo DVDFileInfo;
typedef struct DVDDir DVDDir;
typedef struct DVDDirEntry DVDDirEntry;
typedef struct FSTEntry FSTEntry;
typedef struct FileReadParam FileReadParam;

// GX
typedef struct GXColor GXColor;
typedef struct GXRenderModeObj GXRenderModeObj;
typedef struct GXTexObj GXTexObj;
typedef struct GXPipe GXPipe;

// Audio
typedef struct FGMInstanceData FGMInstanceData;
typedef struct BGMData BGMData;
typedef struct VPB VPB;
typedef struct AXLive AXLive;

// HSD Objects
typedef struct HSD_Obj HSD_Obj;
typedef struct GOBJ GOBJ;
typedef struct GOBJProc GOBJProc;
typedef struct GXList GXList;
typedef struct JOBJ JOBJ;
typedef struct JOBJDesc JOBJDesc;
typedef struct MatAnimDesc MatAnimDesc;
typedef struct MatAnimJointDesc MatAnimJointDesc;
typedef struct AnimJointDesc AnimJointDesc;
typedef struct HSD_VtxDescList HSD_VtxDescList;
typedef struct POBJ POBJ;
typedef struct POBJDesc POBJDesc;
typedef struct DOBJ DOBJ;
typedef struct TOBJ TOBJ;
typedef struct AOBJ AOBJ;
typedef struct MOBJ MOBJ;
typedef struct WOBJ WOBJ;
typedef struct COBJ COBJ;
typedef struct COBJDesc COBJDesc;
typedef struct WOBJDesc WOBJDesc;
typedef struct _HSD_ImageDesc _HSD_ImageDesc;
typedef struct _HSD_Tlut _HSD_Tlut;
typedef struct _HSD_TObjTev _HSD_TObjTev;
typedef struct _HSD_LightPoint _HSD_LightPoint;
typedef struct _HSD_LightPointDesc _HSD_LightPointDesc;
typedef struct _HSD_LightSpot _HSD_LightSpot;
typedef struct _HSD_LightSpotDesc _HSD_LightSpotDesc;
typedef struct _HSD_LightAttn _HSD_LightAttn;
typedef struct LObjDesc LObjDesc;
typedef struct LightAnim LightAnim;
typedef struct LightGroup LightGroup;
typedef struct LOBJ LOBJ;
typedef struct HSD_Fog HSD_Fog;
typedef struct HSD_FogDesc HSD_FogDesc;
typedef struct HSD_SObjDesc HSD_SObjDesc;
typedef struct JOBJSet JOBJSet;

// Archive
typedef struct HSD_Archive HSD_Archive;
typedef struct HSD_ArchiveHeader HSD_ArchiveHeader;
typedef struct HSD_ArchiveRelocationInfo HSD_ArchiveRelocationInfo;
typedef struct HSD_ArchivePublicInfo HSD_ArchivePublicInfo;
typedef struct HSD_ArchiveExternInfo HSD_ArchiveExternInfo;

// Camera
typedef struct CamData CamData;
typedef struct PlayerCamData PlayerCamData;

// Text
typedef struct TextHeapCell TextHeapCell;
typedef struct Text Text;
typedef struct TextCanvas TextCanvas;

// DevText
typedef struct DevText DevText;

// Effects
typedef struct EffectModelDesc EffectModelDesc;
typedef struct PtclDesc PtclDesc;
typedef struct TexGDesc TexGDesc;
typedef struct TexGBank TexGBank;
typedef struct ParticleGroup ParticleGroup;
typedef struct EffectDataTable EffectDataTable;
typedef struct Effect Effect;
typedef struct Particle Particle;
typedef struct ptclGen ptclGen;
typedef struct ptclGenCallback ptclGenCallback;
typedef struct GeneratorAppSRT GeneratorAppSRT;

// Color
typedef struct ColAnimDesc ColAnimDesc;
typedef struct ColAnimSlot ColAnimSlot;
typedef struct ColAnimState ColAnimState;

// Clear Checker
typedef struct RewardEntry RewardEntry;
typedef struct GameClearData GameClearData;

// Item
typedef struct ItemModelDesc ItemModelDesc;
typedef struct ItemDesc ItemDesc;
typedef struct ItemData ItemData;
typedef struct SpawnItem SpawnItem;
typedef struct itData itData;

// Rider
typedef struct RiderData RiderData;

// Collision
typedef struct CollData CollData;

// HSD
typedef struct HSD_Material HSD_Material;
typedef struct HSD_Pad HSD_Pad;
typedef struct HSD_Pads HSD_Pads;
typedef struct HSD_Update HSD_Update;
typedef struct HSD_VI HSD_VI;
typedef struct HSD_ClassInfoHead HSD_ClassInfoHead;
typedef struct HSD_ClassInfo HSD_ClassInfo;
typedef struct HSD_IDEntry HSD_IDEntry;
typedef struct HSD_IDTable HSD_IDTable;
typedef struct HSD_ObjAllocData HSD_ObjAllocData;
typedef struct HSD_PollData HSD_PollData;

// Scene
typedef struct MajorSceneDesc MajorSceneDesc;
typedef struct MinorSceneDesc MinorSceneDesc;
typedef struct SceneInfo SceneInfo;
typedef struct ScMenuCommon ScMenuCommon;

// Preload
typedef struct PreloadHeapLookup PreloadHeapLookup;
typedef struct PreloadHeap PreloadHeap;
typedef struct PreloadHandle PreloadHandle;
typedef struct PreloadHeapDesc PreloadHeapDesc;
typedef struct Preload Preload;
typedef struct PreloadEntryDesc PreloadEntryDesc;
typedef struct PreloadEntry PreloadEntry;
typedef struct PreloadAllocData PreloadAllocData;
typedef struct PreloadTable PreloadTable;

#endif