/*
  @raysan5: To avoid conflicting windows.h symbols with raylib, some flags are defined
  WARNING: Those flags avoid inclusion of some Win32 headers that could be required
  by user at some point and won't be included...
*/

/* If defined, the following flags inhibit definition of the indicated items.*/
#define NOGDICAPMASKS     // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#define NOVIRTUALKEYCODES // VK_*
#define NOWINMESSAGES     // WM_*, EM_*, LB_*, CB_*
#define NOWINSTYLES       // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#define NOSYSMETRICS      // SM_*
#define NOMENUS           // MF_*
#define NOICONS           // IDI_*
#define NOKEYSTATES       // MK_*
#define NOSYSCOMMANDS     // SC_*
#define NORASTEROPS       // Binary and Tertiary raster ops
#define NOSHOWWINDOW      // SW_*
#define OEMRESOURCE       // OEM Resource values
#define NOATOM            // Atom Manager routines
#define NOCLIPBOARD       // Clipboard routines
#define NOCOLOR           // Screen colors
#define NOCTLMGR          // Control and Dialog routines
#define NODRAWTEXT        // DrawText() and DT_*
#define NOGDI             // All GDI defines and routines
#define NOKERNEL          // All KERNEL defines and routines
#define NOUSER            // All USER defines and routines
/*#define NONLS             // All NLS defines and routines*/
#define NOMB              // MB_* and MessageBox()
#define NOMEMMGR          // GMEM_*, LMEM_*, GHND, LHND, associated routines
#define NOMETAFILE        // typedef METAFILEPICT
// #define NOMINMAX          // Macros min(a,b) and max(a,b)
#define NOMSG             // typedef MSG and associated routines
#define NOOPENFILE        // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#define NOSCROLL          // SB_* and scrolling routines
#define NOSERVICE         // All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND           // Sound driver routines
#define NOTEXTMETRIC      // typedef TEXTMETRIC and associated routines
#define NOWH              // SetWindowsHook and WH_*
#define NOWINOFFSETS      // GWL_*, GCL_*, associated routines
#define NOCOMM            // COMM driver routines
#define NOKANJI           // Kanji support stuff.
#define NOHELP            // Help engine interface.
#define NOPROFILER        // Profiler interface.
#define NODEFERWINDOWPOS  // DeferWindowPos routines
#define NOMCX             // Modem Configuration Extensions

#define MMNODRV           // Installable driver support
#define MMNOSOUND         // Sound support
#define MMNOWAVE          // Waveform support
#define MMNOMIDI          // MIDI support
#define MMNOAUX           // Auxiliary audio support
#define MMNOMIXER         // Mixer support
#define MMNOTIMER         // Timer support
#define MMNOJOY           // Joystick support
#define MMNOMCI           // MCI support
#define MMNOMMIO          // Multimedia file I/O support
#define MMNOMMSYSTEM      // General MMSYSTEM functions

/* Type required before windows.h inclusion  */
typedef struct tagMSG* LPMSG;

#include <windows.h>

/* Type required by some unused function...  */
typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG  biWidth;
    LONG  biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG  biXPelsPerMeter;
    LONG  biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER, * PBITMAPINFOHEADER;

#include <objbase.h>
#include <mmreg.h>
#include <mmsystem.h>

/* @raysan5: Some required types defined for MSVC/TinyC compiler */
#if defined(_MSC_VER) || defined(__TINYC__)
#include "propidl.h"
#endif

#include "raylib.h"

// #include "platformerEngineUI.h"
#include <cstdlib>
#include <iostream>

#include "RuntimeObjectSystem.h"
#include "StdioLogSystem.h"
#include "SystemTable.h"
#include "RCCppMainLoop.h"

// RCC++ Data
static StdioLogSystem           g_Logger;
static SystemTable              g_SystemTable;

// Froward declarations of RCC++
bool RCCppInit();
void RCCppCleanup();
void RCCppUpdate();

using namespace std;

int main() {
    // Create Window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(0, 0, "2D Platformer Engine");
    SetExitKey(KEY_NULL);
    MaximizeWindow();

    // Initialize RCC++
    RCCppInit();

    SetTargetFPS(60);

    while (!WindowShouldClose() && !g_SystemTable.pRCCppMainLoopI->WindowShouldClose()) {
        RCCppUpdate();
        g_SystemTable.pRCCppMainLoopI->MainLoop();
    }

    // Cleaning up the closing
    RCCppCleanup();

    CloseWindow();
}

bool RCCppInit()
{
    g_SystemTable.pRuntimeObjectSystem = new RuntimeObjectSystem;
    g_SystemTable.pLogger = &g_Logger;
    if (!g_SystemTable.pRuntimeObjectSystem->Initialise(&g_Logger, &g_SystemTable))
    {
        delete g_SystemTable.pRuntimeObjectSystem;
        g_SystemTable.pRuntimeObjectSystem = NULL;
        return false;
    }

    g_SystemTable.pRuntimeObjectSystem->CleanObjectFiles();

    g_SystemTable.pRuntimeObjectSystem->SetAdditionalCompileOptions("/std:c++17");

    return true;
}

void RCCppCleanup()
{
    delete g_SystemTable.pRuntimeObjectSystem;
}

void RCCppUpdate()
{
    //check status of any compile
    if (g_SystemTable.pRuntimeObjectSystem->GetIsCompiledComplete())
    {
        // load module when compile complete
        g_SystemTable.pRuntimeObjectSystem->LoadCompiledModule();
    }

    if (!g_SystemTable.pRuntimeObjectSystem->GetIsCompiling())
    {
        float deltaTime = GetFrameTime();
        g_SystemTable.pRuntimeObjectSystem->GetFileChangeNotifier()->Update(deltaTime);
    }
}