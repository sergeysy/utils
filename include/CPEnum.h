// Module Name:  Code Page Enumerations
// Project:      Code Page Utilities library

// Contain most used code page constants definitions

#pragma once
#ifndef CP_ENUM__H__F0BE7289_62D2_41F5_A10D_40465F7A524F
#define CP_ENUM__H__F0BE7289_62D2_41F5_A10D_40465F7A524F

#include <Windows.h>

typedef enum
{
    // --- Default code page constants:
    CodePage_Ansi = CP_ACP,         // ACP translation; ANSI
    CodePage_Oem  = CP_OEMCP,       // OEM translation
    CodePage_Mac  = CP_MACCP,       // MAC translation
    CodePage_Utf7 = CP_UTF7,        // UTF-7 translation
    CodePage_Utf8 = CP_UTF8,        // UTF-8 translation

    // ---
    CodePage_Win1250 = 1250,        // windows-1250 translation; Latin-2
    CodePage_Win1251 = 1251,        // windows-1251 translation; Cyrillic
    CodePage_Win1252 = 1252,        // windows-1252 translation; Latin 1 (ANSI)
    CodePage_Win936  = 936,         // windows-936, translation; GBK (Simplified Chinese)
    CodePage_Win950  = 950,         // windows-950  translation

    // --- undefined code page
    CodePage_Unknown = -1
};

#endif // CP_ENUM__H__F0BE7289_62D2_41F5_A10D_40465F7A524F
