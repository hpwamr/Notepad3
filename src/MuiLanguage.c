// encoding: UTF-8
/******************************************************************************
*                                                                             *
*                                                                             *
* Notepad3                                                                    *
*                                                                             *
* MuiLanguage.c                                                               *
*   General MUI Language support functions                                    *
*                                                                             *
*                                                  (c) Rizonesoft 2008-2021   *
*                                                    https://rizonesoft.com   *
*                                                                             *
*                                                                             *
*******************************************************************************/

#include "Helpers.h"

#include <assert.h>
#include <locale.h>
#include <commctrl.h>
#include <muiload.h>

#include "Dialogs.h"
#include "Encoding.h"
#include "Config/Config.h"

#include "MuiLanguage.h"

//=============================================================================


grepWinLng_t grepWinLangResName[] = {
    { MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), L".\\lng\\gwLng\\English (United States) [en-US].lang" },
    { MAKELANGID(LANG_AFRIKAANS, SUBLANG_AFRIKAANS_SOUTH_AFRICA), L".\\lng\\gwLng\\Afrikaans (Suid-Afrika) [af-ZA].lang" },
    { MAKELANGID(LANG_BELARUSIAN, SUBLANG_BELARUSIAN_BELARUS), L".\\lng\\gwLng\\Беларуская (Беларусь) [be-BY].lang" },
    { MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN), L".\\lng\\gwLng\\Deutsch (Deutschland) [de-DE].lang" },
    { MAKELANGID(LANG_GREEK, SUBLANG_GREEK_GREECE), L".\\lng\\gwLng\\Ελληνικά (Ελλάδα) [el-GR].lang" },
    { MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_UK), L".\\lng\\gwLng\\English (United Kingdom) [en-GB].lang" },
    { MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_LATIN_AMERICA), L".\\lng\\gwLng\\Español (América Latina) [es-419].lang" },
    { MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_MODERN), L".\\lng\\gwLng\\Español (España) [es-ES].lang" },
    { MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH), L".\\lng\\gwLng\\Français (France) [fr-FR].lang" },
    { MAKELANGID(LANG_HINDI, SUBLANG_HINDI_INDIA), L".\\lng\\gwLng\\हिन्दी (भारत) [hi-IN].lang" },
    { MAKELANGID(LANG_HUNGARIAN, SUBLANG_HUNGARIAN_HUNGARY), L".\\lng\\gwLng\\Magyar (Magyarország) [hu-HU].lang" },
    { MAKELANGID(LANG_INDONESIAN, SUBLANG_INDONESIAN_INDONESIA), L".\\lng\\gwLng\\Bahasa Indonesia (Indonesia) [id-ID].lang" },
    { MAKELANGID(LANG_ITALIAN, SUBLANG_ITALIAN), L".\\lng\\gwLng\\Italiano (Italia) [it-IT].lang" },
    { MAKELANGID(LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN), L".\\lng\\gwLng\\日本語 （日本）[ja-JP].lang" },
    { MAKELANGID(LANG_KOREAN, SUBLANG_KOREAN), L".\\lng\\gwLng\\한국어 (대한민국) [ko-KR].lang" },
    { MAKELANGID(LANG_DUTCH, SUBLANG_DUTCH), L".\\lng\\gwLng\\Nederlands (Nederland) [nl-NL].lang" },
    { MAKELANGID(LANG_POLISH, SUBLANG_POLISH_POLAND), L".\\lng\\gwLng\\Polski (Polska) [pl-PL].lang" },
    { MAKELANGID(LANG_PORTUGUESE, SUBLANG_PORTUGUESE_BRAZILIAN), L".\\lng\\gwLng\\Português Brasileiro (Brasil) [pt-BR].lang" },
    { MAKELANGID(LANG_PORTUGUESE, SUBLANG_PORTUGUESE), L".\\lng\\gwLng\\Português (Portugal) [pt-PT].lang" },
    { MAKELANGID(LANG_RUSSIAN, SUBLANG_RUSSIAN_RUSSIA), L".\\lng\\gwLng\\Русский (Pоссия) [ru-RU].lang" },
    { MAKELANGID(LANG_SLOVAK, SUBLANG_SLOVAK_SLOVAKIA), L".\\lng\\gwLng\\Slovenčina (Slovensko) [sk-SK].lang" },
    { MAKELANGID(LANG_SWEDISH, SUBLANG_SWEDISH), L".\\lng\\gwLng\\Svenska (Sverige) [sv-SE].lang" },
    { MAKELANGID(LANG_TURKISH, SUBLANG_TURKISH_TURKEY), L".\\lng\\gwLng\\Türkçe (Türkiye) [tr-TR].lang" },
    { MAKELANGID(LANG_VIETNAMESE, SUBLANG_VIETNAMESE_VIETNAM), L".\\lng\\gwLng\\Tiếng Việt (Việt Nam) [vi-VN].lang" },
    { MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), L".\\lng\\gwLng\\简体中文 （中国） [zh-CN].lang" },
    { MAKELANGID(LANG_CHINESE_TRADITIONAL, SUBLANG_CHINESE_TRADITIONAL), L".\\lng\\gwLng\\繁體中文 （台灣） [zh-TW].lang" }
};

int grepWinLang_CountOf() {
    return COUNTOF(grepWinLangResName);
};


//=============================================================================
//
//  SetMuiLocaleAll
//
static void SetMuiLocaleAll(LPCWSTR pszLocaleStr) {
    if (pszLocaleStr) {
        const WCHAR *const pszLocaleCur = _wsetlocale(LC_ALL, pszLocaleStr);
        if (pszLocaleCur && (StringCchCompareXI(pszLocaleStr, pszLocaleCur) != 0)) {
            //const _locale_t pCurLocale = _get_current_locale();
            _wsetlocale(LC_ALL, L""); // system standard
#ifdef _DEBUG
            WCHAR msg[128];
            StringCchPrintf(msg, COUNTOF(msg), L"Can't set desired locale '%s', using '%s' instead!",
                pszLocaleStr, pszLocaleCur ? pszLocaleCur : L"<default>");
            MsgBoxLastError(msg, ERROR_MUI_INVALID_LOCALE_NAME);
#endif
        }
    }
}


//=============================================================================
//
//  SetCurrentLanguage
//
void SetCurrentLanguage(LANGID iLanguageID) {

    int const langIdx = GetMUILanguageIndexByLangID(iLanguageID);

    assert((langIdx >= 0) && "Faild to get LangID!");

    if (iLanguageID != Globals.iCurrentLANGID) {

        Globals.iCurrentLANGID = iLanguageID; // == MUI_LanguageDLLs[langIdx].LangId

        const WCHAR *const szLocaleName = MUI_LanguageDLLs[langIdx].szLocaleName;

        SetThreadUILanguage(iLanguageID);
        InitMUILanguage(iLanguageID); // MUI Language for common controls
        SetMuiLocaleAll(szLocaleName);

        const WCHAR *const SettingName = L"PreferredLanguageLocaleName";

        if (StringCchCompareXIW(Settings2.PreferredLanguageLocaleName, szLocaleName) != 0) {
            StringCchCopyW(Settings2.PreferredLanguageLocaleName, COUNTOF(Settings2.PreferredLanguageLocaleName), szLocaleName);

            if (Globals.bCanSaveIniFile) {
                if (StringCchCompareXIW(Settings2.PreferredLanguageLocaleName, Defaults2.PreferredLanguageLocaleName) != 0) {
                    IniFileSetString(Paths.IniFile, Constants.Settings2_Section, SettingName, Settings2.PreferredLanguageLocaleName);
                } else {
                    IniFileDelete(Paths.IniFile, Constants.Settings2_Section, SettingName, false);
                }
            }
        }
    }
}


//=============================================================================
//=============================================================================


extern prefix_t  g_mxSBPrefix[STATUS_SECTOR_COUNT];
extern prefix_t  g_mxSBPostfix[STATUS_SECTOR_COUNT];


MUILANGUAGE MUI_LanguageDLLs[] = {
    { IDS_MUI_LANG_EN_US, L"en-US", L"English (United States)\t\t\t[%s]",          MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), true, false }, // internal - must be 1st
    // ----------------------------
    { IDS_MUI_LANG_AF_ZA, L"af-ZA", L"Afrikaans (Suid-Afrika)\t\t\t[%s]",          MAKELANGID(LANG_AFRIKAANS, SUBLANG_AFRIKAANS_SOUTH_AFRICA), false, false },
    { IDS_MUI_LANG_BE_BY, L"be-BY", L"Беларуская (Беларусь)\t\t\t[%s]",            MAKELANGID(LANG_BELARUSIAN, SUBLANG_BELARUSIAN_BELARUS), false, false },
    { IDS_MUI_LANG_DE_DE, L"de-DE", L"Deutsch (Deutschland)\t\t\t[%s]",            MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN), false, false },
    { IDS_MUI_LANG_EL_GR, L"el-GR", L"Ελληνικά (Ελλάδα)\t\t\t[%s]",                MAKELANGID(LANG_GREEK, SUBLANG_GREEK_GREECE), false, false },
    { IDS_MUI_LANG_EN_GB, L"en-GB", L"English (United Kingdom)\t\t\t[%s]",         MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_UK), false, false },
    { IDS_MUI_LANG_ES_419, L"es-419", L"Español (América Latina)\t\t\t[%s]",       MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_LATIN_AMERICA), false, false },
    { IDS_MUI_LANG_ES_ES, L"es-ES", L"Español (España)\t\t\t[%s]",                 MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_MODERN), false, false },
    { IDS_MUI_LANG_FR_FR, L"fr-FR", L"Français (France)\t\t\t[%s]",                MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH), false, false },
    { IDS_MUI_LANG_HI_IN, L"hi-IN", L"हिन्दी (भारत)\t\t\t[%s]",                       MAKELANGID(LANG_HINDI, SUBLANG_HINDI_INDIA), false, false },
    { IDS_MUI_LANG_HU_HU, L"hu-HU", L"Magyar (Magyarország)\t\t\t[%s]",            MAKELANGID(LANG_HUNGARIAN, SUBLANG_HUNGARIAN_HUNGARY), false, false },
    { IDS_MUI_LANG_ID_ID, L"id-ID", L"Bahasa Indonesia (Indonesia)\t\t\t[%s]",     MAKELANGID(LANG_INDONESIAN, SUBLANG_INDONESIAN_INDONESIA), false, false },
    { IDS_MUI_LANG_IT_IT, L"it-IT", L"Italiano (Italia)\t\t\t[%s]",                MAKELANGID(LANG_ITALIAN, SUBLANG_ITALIAN), false, false },
    { IDS_MUI_LANG_JP_JP, L"ja-JP", L"日本語 （日本）\t\t\t[%s]",                   MAKELANGID(LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN), false, false },
    { IDS_MUI_LANG_KO_KR, L"ko-KR", L"한국어 (대한민국)\t\t\t[%s]",                 MAKELANGID(LANG_KOREAN, SUBLANG_KOREAN), false, false },
    { IDS_MUI_LANG_NL_NL, L"nl-NL", L"Nederlands (Nederland)\t\t\t[%s]",           MAKELANGID(LANG_DUTCH, SUBLANG_DUTCH), false, false },
    { IDS_MUI_LANG_PL_PL, L"pl-PL", L"Polski (Polska)\t\t\t[%s]",                  MAKELANGID(LANG_POLISH, SUBLANG_POLISH_POLAND), false, false },
    { IDS_MUI_LANG_PT_BR, L"pt-BR", L"Português Brasileiro (Brasil)\t\t\t[%s]",    MAKELANGID(LANG_PORTUGUESE, SUBLANG_PORTUGUESE_BRAZILIAN), false, false },
    { IDS_MUI_LANG_PT_PT, L"pt-PT", L"Português (Portugal)\t\t\t[%s]",             MAKELANGID(LANG_PORTUGUESE, SUBLANG_PORTUGUESE), false, false },
    { IDS_MUI_LANG_RU_RU, L"ru-RU", L"Русский (Pоссия)\t\t\t[%s]",                 MAKELANGID(LANG_RUSSIAN, SUBLANG_RUSSIAN_RUSSIA), false, false },
    { IDS_MUI_LANG_SK_SK, L"sk-SK", L"Slovenčina (Slovensko)\t\t\t[%s]",           MAKELANGID(LANG_SLOVAK, SUBLANG_SLOVAK_SLOVAKIA), false, false },
    { IDS_MUI_LANG_SV_SE, L"sv-SE", L"Svenska (Sverige)\t\t\t[%s]",                MAKELANGID(LANG_SWEDISH, SUBLANG_SWEDISH), false, false },
    { IDS_MUI_LANG_TR_TR, L"tr-TR", L"Türkçe (Türkiye)\t\t\t[%s]",                 MAKELANGID(LANG_TURKISH, SUBLANG_TURKISH_TURKEY), false, false },
    { IDS_MUI_LANG_VI_VN, L"vi-VN", L"Tiếng Việt (Việt Nam)\t\t\t[%s]",            MAKELANGID(LANG_VIETNAMESE, SUBLANG_VIETNAMESE_VIETNAM), false, false },
    { IDS_MUI_LANG_ZH_CN, L"zh-CN", L"简体中文 （中国）\t\t\t[%s]",                 MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), false, false},
    { IDS_MUI_LANG_ZH_TW, L"zh-TW", L"繁體中文 （台灣）\t\t\t[%s]",                 MAKELANGID(LANG_CHINESE_TRADITIONAL, SUBLANG_CHINESE_TRADITIONAL), false, false}
};

//NUM_OF_MUI_LANGUAGES
int MuiLanguages_CountOf()
{
    return COUNTOF(MUI_LanguageDLLs);
};


//=============================================================================
//
//  GetMUILanguageIndexByLangID
//
int GetMUILanguageIndexByLangID(LANGID iLanguageID)
{
    for (int lng = 0; lng < MuiLanguages_CountOf(); ++lng) {
        if (MUI_LanguageDLLs[lng].LangId == iLanguageID) {
            return lng;
        }
    }
    return -1;
}


#if defined(HAVE_DYN_LOAD_LIBS_MUI_LNGS)

//=============================================================================
//
//  CheckAvailableLanguages
//
//
static int _CheckAvailableLanguageDLLs()
{
    WCHAR wchRelPath[MAX_PATH];
    WCHAR wchAbsPath[MAX_PATH];

    int count = 1;
    for (int lng = 1; lng < MuiLanguages_CountOf(); ++lng) {

        if (IsValidLocaleName(MUI_LanguageDLLs[lng].szLocaleName)) {

            //WCHAR wchLngLocalName[LOCALE_NAME_MAX_LENGTH];
            //if (ResolveLocaleName(MUI_LanguageDLLs[i].szLocaleName, wchLngLocalName, LOCALE_NAME_MAX_LENGTH)) {
            //  StringCchCopy(MUI_LanguageDLLs[i].szLocaleName, COUNTOF(MUI_LanguageDLLs[i].szLocaleName), wchLngLocalName); // put back resolved name
            //}

            // get LANGID
            DWORD value = MUI_BASE_LNG_ID;
            if (GetLocaleInfoEx(MUI_LanguageDLLs[lng].szLocaleName, LOCALE_ILANGUAGE | LOCALE_RETURN_NUMBER, (LPWSTR)&value, sizeof(value) / sizeof(WCHAR)) > 0) {
                MUI_LanguageDLLs[lng].LangId = (LANGID)value;
            }

            // check for DLL
            StringCchPrintf(wchRelPath, COUNTOF(wchRelPath), L"lng/%s/np3lng.dll.mui", MUI_LanguageDLLs[lng].szLocaleName);
            PathAbsoluteFromApp(wchRelPath, wchAbsPath, COUNTOF(wchAbsPath), false);
            bool const bAvail = PathIsExistingFile(wchAbsPath);
            MUI_LanguageDLLs[lng].bHasDLL = bAvail;
            count += bAvail ? 1 : 0;
        }
    }
    return count;
}


//=============================================================================
//
//  _LngStrToMultiLngStr
//
//
static bool  _LngStrToMultiLngStr(WCHAR* pLngStr, WCHAR* pLngMultiStr, size_t lngMultiStrSize)
{
    bool rtnVal = true;

    size_t strLen = StringCchLenW(pLngStr, 0);

    if ((strLen > 0) && pLngMultiStr && (lngMultiStrSize > 0)) {
        WCHAR* lngMultiStrPtr = pLngMultiStr;
        WCHAR* last = pLngStr + (Has_UTF16_LE_BOM((char*)pLngStr, (strLen * sizeof(WCHAR))) ? 1 : 0);
        while (last && rtnVal) {
            // make sure you validate the user input
            WCHAR* next = StrNextTok(last, L",; :");
            if (next) {
                *next = L'\0';
            }
            strLen = StringCchLenW(last, LOCALE_NAME_MAX_LENGTH);
            if (strLen && IsValidLocaleName(last)) {
                lngMultiStrPtr[0] = L'\0';
                rtnVal &= SUCCEEDED(StringCchCatW(lngMultiStrPtr, (lngMultiStrSize - (lngMultiStrPtr - pLngMultiStr)), last));
                lngMultiStrPtr += strLen + 1;
            }
            last = (next ? next + 1 : next);
        }
        if (rtnVal && (lngMultiStrSize - (lngMultiStrPtr - pLngMultiStr))) { // make sure there is a double null term for the multi-string
            lngMultiStrPtr[0] = L'\0';
        } else { // fail and guard anyone whom might use the multi-string
            lngMultiStrPtr[0] = L'\0';
            lngMultiStrPtr[1] = L'\0';
        }
    }
    return rtnVal;
}


//=============================================================================
//
//  GetUserPreferredLanguage
//
bool GetUserPreferredLanguage(LPWSTR pszPrefLocaleName, int cchBuffer, LANGID* pLangID)
{
    int res = 0;
    LANGID lngID = *pLangID;
    WCHAR wchLngLocalName[LOCALE_NAME_MAX_LENGTH+1];

    if (StrIsNotEmpty(pszPrefLocaleName)) {
        res = ResolveLocaleName(pszPrefLocaleName, wchLngLocalName, COUNTOF(wchLngLocalName));
        if (res > 0) {
            // get LANGID
            DWORD value = MUI_BASE_LNG_ID;
            res = GetLocaleInfoEx(wchLngLocalName, LOCALE_ILANGUAGE | LOCALE_RETURN_NUMBER, (LPWSTR)&value, sizeof(value) / sizeof(WCHAR));
            if (res > 0) {
                lngID = (LANGID)value;
            }
        }
    }

    if (res == 0) { // No preferred language defined or retrievable, try to get User UI Language
        //~GetUserDefaultLocaleName(pszPrefLocaleName, cchBuffer);
        ULONG numLngs = 0;
        DWORD cchLngsBuffer = 0;
        BOOL hr = GetUserPreferredUILanguages(MUI_LANGUAGE_NAME, &numLngs, NULL, &cchLngsBuffer);
        if (hr) {
            WCHAR* pwszLngsBuffer = AllocMem((cchLngsBuffer + 2) * sizeof(WCHAR), HEAP_ZERO_MEMORY);
            if (pwszLngsBuffer) {
                hr = GetUserPreferredUILanguages(MUI_LANGUAGE_NAME, &numLngs, pwszLngsBuffer, &cchLngsBuffer);
                if (hr && (numLngs > 0)) {
                    // get the first
                    StringCchCopy(wchLngLocalName, COUNTOF(wchLngLocalName), pwszLngsBuffer);
                    // TODO: deprecated
                    lngID = LANGIDFROMLCID(LocaleNameToLCID(wchLngLocalName, 0));
                    res = 1;
                }
                FreeMem(pwszLngsBuffer);
            }
        }
        if (res == 0) { // last try (deprecated)
            lngID = GetUserDefaultUILanguage();
            res = LangIDToLocaleName(lngID, wchLngLocalName, COUNTOF(wchLngLocalName));
        }
    }
    if (res != 0) {
        *pLangID = lngID;
        StringCchCopy(pszPrefLocaleName, cchBuffer, wchLngLocalName);
        return true;
    }
    return false;
}


//=============================================================================
//
//  LoadLanguageResources
//
//
LANGID LoadLanguageResources(LPCWSTR localeName)
{
    FreeLanguageResources(); // reset

    int const iInternalLngIndex = max_i(0, GetMUILanguageIndexByLangID(MUI_BASE_LNG_ID));

    // 1st check language resources
    Globals.iAvailLngCount = _CheckAvailableLanguageDLLs();

    // set the appropriate fallback list
    int iLngIndex = -1;
    WCHAR tchAvailLngs[2 * (LOCALE_NAME_MAX_LENGTH + 1)] = { L'\0' };
    for (int lng = 0; lng < MuiLanguages_CountOf(); ++lng) {
        if (StringCchCompareXIW(MUI_LanguageDLLs[lng].szLocaleName, localeName) == 0) {
            if (MUI_LanguageDLLs[lng].bHasDLL && (lng > 0)) {
                StringCchCatW(tchAvailLngs, COUNTOF(tchAvailLngs), MUI_LanguageDLLs[lng].szLocaleName);
                StringCchCatW(tchAvailLngs, COUNTOF(tchAvailLngs), L";");
            }
            iLngIndex = lng;
            break;
        }
    }
    StringCchCatW(tchAvailLngs, COUNTOF(tchAvailLngs), MUI_LanguageDLLs[iInternalLngIndex].szLocaleName); // en-US fallback

    // NOTES:
    // an application developer that makes the assumption the fallback list provided by the
    // system / OS is entirely sufficient may or may not be making a good assumption based  mostly on:
    // A. your choice of languages installed with your application
    // B. the languages on the OS at application install time
    // C. the OS users propensity to install/uninstall language packs
    // D. the OS users propensity to change language settings

    WCHAR tchUserLangMultiStrg[LARGE_BUFFER] = { L'\0' };
    if (!_LngStrToMultiLngStr(tchAvailLngs, tchUserLangMultiStrg, COUNTOF(tchUserLangMultiStrg))) {
        MsgBoxLastError(L"Trying to load available Language resources!", ERROR_MUI_INVALID_LOCALE_NAME);
    }
    ULONG langCount = 0;
    // using SetProcessPreferredUILanguages is recommended for new applications (esp. multi-threaded applications)
    SetProcessPreferredUILanguages(0, L"\0\0", &langCount); // clear
    if (!SetProcessPreferredUILanguages(MUI_LANGUAGE_NAME, tchUserLangMultiStrg, &langCount) || (langCount == 0)) {
        DbgMsgBoxLastError(L"Trying to set preferred Language!", ERROR_RESOURCE_LANG_NOT_FOUND);
    }
    //~else {
    //~  SetThreadPreferredUILanguages(0, L"\0\0", &langCount); // clear
    //~  SetThreadPreferredUILanguages(MUI_LANGUAGE_NAME, tchUserLangMultiStrg, &langCount);
    //~}

    // obtains access to the proper resource container
    // for standard Win32 resource loading this is normally a PE module - use LoadLibraryEx

    Globals.hLngResContainer = NULL; //(!)

    if (iLngIndex == iInternalLngIndex) {

        Globals.hLngResContainer = Globals.hInstance;
        MUI_LanguageDLLs[iInternalLngIndex].bIsActive = true;

    } else if ((iLngIndex >= 0) && MUI_LanguageDLLs[iLngIndex].bHasDLL) {

        Globals.hLngResContainer = LoadMUILibrary(L"lng/np3lng.dll", 
                                                  MUI_LANGUAGE_NAME | MUI_LANGUAGE_EXACT,
                                                  MUI_LanguageDLLs[iLngIndex].LangId);
        if (Globals.hLngResContainer) {
            MUI_LanguageDLLs[iLngIndex].bIsActive = true;
            MUI_LanguageDLLs[iInternalLngIndex].bIsActive = false;
        } else {
            //DbgMsgBoxLastError(L"LoadMUILibrary", 0);
            iLngIndex = -1;
        }
    }

    if (!Globals.hLngResContainer || (iLngIndex < 0)) {
        // fallback to MUI_BASE_LNG_ID
        Globals.hLngResContainer = Globals.hInstance;
        MUI_LanguageDLLs[iInternalLngIndex].bIsActive = true;
        iLngIndex = iInternalLngIndex;

        const WCHAR *const suprMsg = L"MsgPrefLanguageNotAvailable";
        InfoBoxLng(MB_ICONWARNING, suprMsg, IDS_WARN_PREF_LNG_NOT_AVAIL, localeName);
        int const noMsg = IniFileGetInt(Paths.IniFile, Constants.SectionSuppressedMessages, suprMsg, 0);
        if (noMsg && Globals.bCanSaveIniFile) {
            IniFileSetString(Paths.IniFile, Constants.Settings2_Section, L"PreferredLanguageLocaleName", MUI_LanguageDLLs[iInternalLngIndex].szLocaleName);
        }
    }

    // ===  update language dependent items  ===

    for (cpi_enc_t enc = 0; enc < Encoding_CountOf(); ++enc) {
        Encoding_SetLabel(enc);
    }

    // ------------------------------------------------------------
    const WCHAR* const StatusBar_Section = L"Statusbar Settings";
    // ------------------------------------------------------------

    WCHAR tchStatusBar[MIDSZ_BUFFER] = { L'\0' };
    WCHAR tchDefaultStrg[MIDSZ_BUFFER] = { L'\0' };

    GetLngString(IDS_MUI_STATUSBAR_PREFIXES, tchDefaultStrg, COUNTOF(tchDefaultStrg));
    IniFileGetString(Paths.IniFile, StatusBar_Section, L"SectionPrefixes", tchDefaultStrg, tchStatusBar, COUNTOF(tchStatusBar));
    ReadStrgsFromCSV(tchStatusBar, g_mxSBPrefix, STATUS_SECTOR_COUNT, MICRO_BUFFER, L"_PRFX_");

    GetLngString(IDS_MUI_STATUSBAR_POSTFIXES, tchDefaultStrg, COUNTOF(tchDefaultStrg));
    IniFileGetString(Paths.IniFile, StatusBar_Section, L"SectionPostfixes", tchDefaultStrg, tchStatusBar, COUNTOF(tchStatusBar));
    ReadStrgsFromCSV(tchStatusBar, g_mxSBPostfix, STATUS_SECTOR_COUNT, MICRO_BUFFER, L"_POFX_");

    return MUI_LanguageDLLs[iLngIndex].LangId;
}


//=============================================================================
//
//  FreeLanguageResources
//
//
void FreeLanguageResources() {
    CloseNonModalDialogs();
    int const iInternalLngIndex = GetMUILanguageIndexByLangID(MUI_BASE_LNG_ID);
    if (Globals.hLngResContainer != Globals.hInstance) {
        FreeMUILibrary(Globals.hLngResContainer);
        Globals.hLngResContainer = Globals.hInstance;
    }
    for (int i = 0; i < MuiLanguages_CountOf(); ++i) {
        MUI_LanguageDLLs[i].bIsActive = (iInternalLngIndex == i);
    }
}


//=============================================================================
//
//  InsertLanguageMenu
//
//

static HMENU s_hmenuLanguage = NULL;

bool InsertLanguageMenu(HMENU hMenuBar) {

    // check, if we need a language switching menu
    if (Globals.iAvailLngCount < 2) {
        Settings.PreferredLocale4DateFmt = false;
        return false;
    }

    if (s_hmenuLanguage) {
        DestroyMenu(s_hmenuLanguage);
    }
    s_hmenuLanguage = CreatePopupMenu();

    WCHAR wchMenuItemFmt[128] = { L'\0' };
    WCHAR wchMenuItemStrg[196] = { L'\0' };
    for (int lng = 0; lng < MuiLanguages_CountOf(); ++lng) {
        if (MUI_LanguageDLLs[lng].bHasDLL) {
            StringCchCopy(wchMenuItemFmt, COUNTOF(wchMenuItemFmt), MUI_LanguageDLLs[lng].szMenuItem);
            StringCchPrintfW(wchMenuItemStrg, COUNTOF(wchMenuItemStrg), wchMenuItemFmt, MUI_LanguageDLLs[lng].szLocaleName);
            AppendMenu(s_hmenuLanguage, MF_ENABLED | MF_STRING, MUI_LanguageDLLs[lng].rid, wchMenuItemStrg);
        }
    }

    // --- insert ---
    int const pos = GetMenuItemCount(hMenuBar) - 1;
    if (pos >= 0) {
        GetLngString(IDS_MUI_MENU_LANGUAGE, wchMenuItemStrg, COUNTOF(wchMenuItemStrg));
        //return InsertMenu(hMenuBar, pos, MF_BYPOSITION | MF_POPUP | MF_STRING, (UINT_PTR)s_hmenuLanguage, wchMenuItemStrg);
        bool const res = InsertMenu(hMenuBar, IDM_VIEW_TABSASSPACES, MF_BYCOMMAND | MF_POPUP | MF_STRING, (UINT_PTR)s_hmenuLanguage, wchMenuItemStrg);
        GetLngString(IDS_USE_LOCALE_DATEFMT, wchMenuItemStrg, COUNTOF(wchMenuItemStrg));
        InsertMenu(hMenuBar, IDM_VIEW_TABSASSPACES, MF_BYCOMMAND | MF_STRING, (UINT_PTR)IDS_USE_LOCALE_DATEFMT, wchMenuItemStrg);
        InsertMenu(hMenuBar, IDM_VIEW_TABSASSPACES, MF_BYCOMMAND | MF_SEPARATOR, (UINT_PTR)NULL, NULL);
        return res;
    }
    return false;
}

//=============================================================================
//
//  DynamicLanguageMenuCmd() - Handles IDS_MUI_LANG_XX_YY messages
//
void DynamicLanguageMenuCmd(int cmd) {

    int const iLngIdx = (cmd - IDS_MUI_LANG_EN_US); // consecutive IDs

    if ((iLngIdx < 0) || (iLngIdx >= MuiLanguages_CountOf())) {
        return;
    }
    if (!MUI_LanguageDLLs[iLngIdx].bIsActive) {

        DestroyMenu(Globals.hMainMenu);

        // desired language
        LANGID const desiredLngID = MUI_LanguageDLLs[iLngIdx].LangId;
        LPCWSTR desiredLocaleName = MUI_LanguageDLLs[iLngIdx].szLocaleName;
        LoadLanguageResources(desiredLocaleName);
        SetCurrentLanguage(desiredLngID);

        Globals.hMainMenu = LoadMenu(Globals.hLngResContainer, MAKEINTRESOURCE(IDR_MUI_MAINMENU));
        if (!Globals.hMainMenu) {
            MsgBoxLastError(L"LoadMenu()", 0);
            CloseApplication();
            return;
        }

        InsertLanguageMenu(Globals.hMainMenu);
        SetMenu(Globals.hwndMain, (Settings.ShowMenubar ? Globals.hMainMenu : NULL));
    }
}


#endif  // HAVE_DYN_LOAD_LIBS_MUI_LNGS


//=============================================================================
//
//  LoadLngStringW()
//
int LoadLngStringW(UINT uID, LPWSTR lpBuffer, int nBufferMax)
{
    const int nLen = LoadStringW(Globals.hLngResContainer, uID, lpBuffer, nBufferMax);
    return (nLen != 0) ? nLen : LoadStringW(Globals.hInstance, uID, lpBuffer, nBufferMax);
}

//=============================================================================
//
//  LoadLngStringW2MB()
//
int LoadLngStringW2MB(UINT uID, LPSTR lpBuffer, int nBufferMax)
{
    int len = 0;
    WCHAR * const pBuffer = (WCHAR *)AllocMem(sizeof(WCHAR) * nBufferMax, HEAP_ZERO_MEMORY);
    if (pBuffer) {
        const int nLen = LoadStringW(Globals.hLngResContainer, uID, pBuffer, nBufferMax);
        if (nLen == 0) {
            LoadStringW(Globals.hInstance, uID, pBuffer, nBufferMax);
        }
        len = WideCharToMultiByte(Encoding_SciCP, 0, pBuffer, -1, lpBuffer, nBufferMax, NULL, NULL);
        FreeMem(pBuffer);
    }
    return len;
}

//=============================================================================
//
//  LoadLngStringA()
//
int LoadLngStringA(UINT uID, LPSTR lpBuffer, int nBufferMax)
{
    const int nLen = LoadStringA(Globals.hLngResContainer, uID, lpBuffer, nBufferMax);
    return (nLen != 0) ? nLen : LoadStringA(Globals.hInstance, uID, lpBuffer, nBufferMax);
}


//=============================================================================
//
//  FormatLngStringW()
//
int FormatLngStringW(LPWSTR lpOutput, int nOutput, UINT uIdFormat, ...)
{
    WCHAR* const pBuffer = AllocMem(sizeof(WCHAR) * nOutput, HEAP_ZERO_MEMORY);
    if (pBuffer) {
        if (LoadLngStringW(uIdFormat, pBuffer, nOutput)) {
            StringCchVPrintfW(lpOutput, nOutput, pBuffer, (LPVOID)((PUINT_PTR)& uIdFormat + 1));
        }
        FreeMem(pBuffer);
        return (int)StringCchLenW(lpOutput, nOutput);
    }
    return 0;
}

//=============================================================================
//
//  FormatLngStringA()
//
int FormatLngStringA(LPSTR lpOutput, int nOutput, UINT uIdFormat, ...)
{
    CHAR* const pBuffer = AllocMem(sizeof(CHAR) * nOutput, HEAP_ZERO_MEMORY);
    if (pBuffer) {
        if (LoadLngStringA(uIdFormat, pBuffer, nOutput)) {
            StringCchVPrintfA(lpOutput, nOutput, pBuffer, (LPVOID)((PUINT_PTR)& uIdFormat + 1));
        }
        FreeMem(pBuffer);
        return (int)StringCchLenA(lpOutput, nOutput);
    }
    return 0;
}




//=============================================================================

///   End of MuiLanguage.c   ///
