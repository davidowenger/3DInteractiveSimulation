#include "Common.h"

namespace Demo
{

ClientConfig* const ClientConfig::singleton = new ClientConfig();

ClientConfig::ClientConfig() :
    mResourcePath("")
{
}

ClientConfig* const ClientConfig::getSingle()
{
    return singleton;
}

void ClientConfig::init(String& path, const String& fileClientConfig, const String& fileComConfig)
{
    mResourcePath = path;

    // Load user prefs from user modifiable text file
    loadConfig(&maMapConfig[CONFIG_CLIENT], mResourcePath + fileClientConfig);
    loadConfig(&maMapConfig[CONFIG_COM   ], mResourcePath + fileComConfig);
}

const String ClientConfig::getPath(const String &path)
{
    return mResourcePath + path;
}

void ClientConfig::loadConfig(StringMap* const mapConfig, const String &pathFile)
{
    ConfigFile                              cf;
    ConfigFile::SettingsMultiMap*           settings;
    ConfigFile::SettingsMultiMap::iterator  i;

    // Go through all sections & settings in the demo config file
    cf.load(pathFile);
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    while (seci.hasMoreElements()) {
        settings = seci.getNext();

        for (i = settings->begin(); i != settings->end(); i++) {
            (*mapConfig)[i->first] = i->second;
        }
    }
}

String ClientConfig::getString(const String& key, const ConfigType type)
{
    return maMapConfig[type][key];
}

bool ClientConfig::getBool(const String& key, const ConfigType type)
{
    return maMapConfig[type][key] == "Yes";
}

void ClientConfig::getKeysAssignement(const String &pathFile, IntCode* const aKeyAssignement, IntMap* const mapClientKeyCode)
{
    StringMap::iterator     it;

    IntMap                  mapOISKeyCode = IntMap();

    // Use OIS KeyCode enum keys and values
    mapOISKeyCode["KC_UNASSIGNED"] = 0x00;
    mapOISKeyCode["KC_ESCAPE"] = 0x01;
    mapOISKeyCode["KC_1"] = 0x02;
    mapOISKeyCode["KC_2"] = 0x03;
    mapOISKeyCode["KC_3"] = 0x04;
    mapOISKeyCode["KC_4"] = 0x05;
    mapOISKeyCode["KC_5"] = 0x06;
    mapOISKeyCode["KC_6"] = 0x07;
    mapOISKeyCode["KC_7"] = 0x08;
    mapOISKeyCode["KC_8"] = 0x09;
    mapOISKeyCode["KC_9"] = 0x0A;
    mapOISKeyCode["KC_0"] = 0x0B;
    mapOISKeyCode["KC_MINUS"] = 0x0C;   // - on main keyboard
    mapOISKeyCode["KC_EQUALS"] = 0x0D;
    mapOISKeyCode["KC_BACK"] = 0x0E;   // backspace
    mapOISKeyCode["KC_TAB"] = 0x0F;
    mapOISKeyCode["KC_Q"] = 0x10;
    mapOISKeyCode["KC_W"] = 0x11;
    mapOISKeyCode["KC_E"] = 0x12;
    mapOISKeyCode["KC_R"] = 0x13;
    mapOISKeyCode["KC_T"] = 0x14;
    mapOISKeyCode["KC_Y"] = 0x15;
    mapOISKeyCode["KC_U"] = 0x16;
    mapOISKeyCode["KC_I"] = 0x17;
    mapOISKeyCode["KC_O"] = 0x18;
    mapOISKeyCode["KC_P"] = 0x19;
    mapOISKeyCode["KC_LBRACKET"] = 0x1A;
    mapOISKeyCode["KC_RBRACKET"] = 0x1B;
    mapOISKeyCode["KC_RETURN"] = 0x1C;   // Enter on main keyboard
    mapOISKeyCode["KC_LCONTROL"] = 0x1D;
    mapOISKeyCode["KC_A"] = 0x1E;
    mapOISKeyCode["KC_S"] = 0x1F;
    mapOISKeyCode["KC_D"] = 0x20;
    mapOISKeyCode["KC_F"] = 0x21;
    mapOISKeyCode["KC_G"] = 0x22;
    mapOISKeyCode["KC_H"] = 0x23;
    mapOISKeyCode["KC_J"] = 0x24;
    mapOISKeyCode["KC_K"] = 0x25;
    mapOISKeyCode["KC_L"] = 0x26;
    mapOISKeyCode["KC_SEMICOLON"] = 0x27;
    mapOISKeyCode["KC_APOSTROPHE"] = 0x28;
    mapOISKeyCode["KC_GRAVE"] = 0x29;   // accent
    mapOISKeyCode["KC_LSHIFT"] = 0x2A;
    mapOISKeyCode["KC_BACKSLASH"] = 0x2B;
    mapOISKeyCode["KC_Z"] = 0x2C;
    mapOISKeyCode["KC_X"] = 0x2D;
    mapOISKeyCode["KC_C"] = 0x2E;
    mapOISKeyCode["KC_V"] = 0x2F;
    mapOISKeyCode["KC_B"] = 0x30;
    mapOISKeyCode["KC_N"] = 0x31;
    mapOISKeyCode["KC_M"] = 0x32;
    mapOISKeyCode["KC_COMMA"] = 0x33;
    mapOISKeyCode["KC_PERIOD"] = 0x34;   // . on main keyboard
    mapOISKeyCode["KC_SLASH"] = 0x35;   // / on main keyboard
    mapOISKeyCode["KC_RSHIFT"] = 0x36;
    mapOISKeyCode["KC_MULTIPLY"] = 0x37;   // * on numeric keypad
    mapOISKeyCode["KC_LMENU"] = 0x38;   // left Alt
    mapOISKeyCode["KC_SPACE"] = 0x39;
    mapOISKeyCode["KC_CAPITAL"] = 0x3A;
    mapOISKeyCode["KC_F1"] = 0x3B;
    mapOISKeyCode["KC_F2"] = 0x3C;
    mapOISKeyCode["KC_F3"] = 0x3D;
    mapOISKeyCode["KC_F4"] = 0x3E;
    mapOISKeyCode["KC_F5"] = 0x3F;
    mapOISKeyCode["KC_F6"] = 0x40;
    mapOISKeyCode["KC_F7"] = 0x41;
    mapOISKeyCode["KC_F8"] = 0x42;
    mapOISKeyCode["KC_F9"] = 0x43;
    mapOISKeyCode["KC_F10"] = 0x44;
    mapOISKeyCode["KC_NUMLOCK"] = 0x45;
    mapOISKeyCode["KC_SCROLL"] = 0x46;   // Scroll Lock
    mapOISKeyCode["KC_NUMPAD7"] = 0x47;
    mapOISKeyCode["KC_NUMPAD8"] = 0x48;
    mapOISKeyCode["KC_NUMPAD9"] = 0x49;
    mapOISKeyCode["KC_SUBTRACT"] = 0x4A;   // - on numeric keypad
    mapOISKeyCode["KC_NUMPAD4"] = 0x4B;
    mapOISKeyCode["KC_NUMPAD5"] = 0x4C;
    mapOISKeyCode["KC_NUMPAD6"] = 0x4D;
    mapOISKeyCode["KC_ADD"] = 0x4E;   // + on numeric keypad
    mapOISKeyCode["KC_NUMPAD1"] = 0x4F;
    mapOISKeyCode["KC_NUMPAD2"] = 0x50;
    mapOISKeyCode["KC_NUMPAD3"] = 0x51;
    mapOISKeyCode["KC_NUMPAD0"] = 0x52;
    mapOISKeyCode["KC_DECIMAL"] = 0x53;   // . on numeric keypad
    mapOISKeyCode["KC_OEM_102"] = 0x56;   // < > | on UK/Germany keyboards
    mapOISKeyCode["KC_F11"] = 0x57;
    mapOISKeyCode["KC_F12"] = 0x58;
    mapOISKeyCode["KC_F13"] = 0x64;   //                     (NEC PC98)
    mapOISKeyCode["KC_F14"] = 0x65;   //                     (NEC PC98)
    mapOISKeyCode["KC_F15"] = 0x66;   //                     (NEC PC98)
    mapOISKeyCode["KC_KANA"] = 0x70;   // (Japanese keyboard)
    mapOISKeyCode["KC_ABNT_C1"] = 0x73;   // / ? on Portugese (Brazilian) keyboards
    mapOISKeyCode["KC_CONVERT"] = 0x79;   // (Japanese keyboard)
    mapOISKeyCode["KC_NOCONVERT"] = 0x7B;   // (Japanese keyboard)
    mapOISKeyCode["KC_YEN"] = 0x7D;   // (Japanese keyboard)
    mapOISKeyCode["KC_ABNT_C2"] = 0x7E;   // Numpad . on Portugese (Brazilian) keyboards
    mapOISKeyCode["KC_NUMPADEQUALS"] = 0x8D;   // = on numeric keypad (NEC PC98)
    mapOISKeyCode["KC_PREVTRACK"] = 0x90;   // Previous Track (KC_CIRCUMFLEX on Japanese keyboard)
    mapOISKeyCode["KC_AT"] = 0x91;   //                     (NEC PC98)
    mapOISKeyCode["KC_COLON"] = 0x92;   //                     (NEC PC98)
    mapOISKeyCode["KC_UNDERLINE"] = 0x93;   //                     (NEC PC98)
    mapOISKeyCode["KC_KANJI"] = 0x94;   // (Japanese keyboard)
    mapOISKeyCode["KC_STOP"] = 0x95;   //                     (NEC PC98)
    mapOISKeyCode["KC_AX"] = 0x96;   //                     (Japan AX)
    mapOISKeyCode["KC_UNLABELED"] = 0x97;   //                        (J3100)
    mapOISKeyCode["KC_NEXTTRACK"] = 0x99;   // Next Track
    mapOISKeyCode["KC_NUMPADENTER"] = 0x9C;   // Enter on numeric keypad
    mapOISKeyCode["KC_RCONTROL"] = 0x9D;
    mapOISKeyCode["KC_MUTE"] = 0xA0;   // Mute
    mapOISKeyCode["KC_CALCULATOR"] = 0xA1;   // Calculator
    mapOISKeyCode["KC_PLAYPAUSE"] = 0xA2;   // Play / Pause
    mapOISKeyCode["KC_MEDIASTOP"] = 0xA4;   // Media Stop
    mapOISKeyCode["KC_VOLUMEDOWN"] = 0xAE;   // Volume -
    mapOISKeyCode["KC_VOLUMEUP"] = 0xB0;   // Volume +
    mapOISKeyCode["KC_WEBHOME"] = 0xB2;   // Web home
    mapOISKeyCode["KC_NUMPADCOMMA"] = 0xB3;   // );on numeric keypad (NEC PC98)
    mapOISKeyCode["KC_DIVIDE"] = 0xB5;   // / on numeric keypad
    mapOISKeyCode["KC_SYSRQ"] = 0xB7;
    mapOISKeyCode["KC_RMENU"] = 0xB8;   // right Alt
    mapOISKeyCode["KC_PAUSE"] = 0xC5;   // Pause
    mapOISKeyCode["KC_HOME"] = 0xC7;   // Home on arrow keypad
    mapOISKeyCode["KC_UP"] = 0xC8;   // UpArrow on arrow keypad
    mapOISKeyCode["KC_PGUP"] = 0xC9;   // PgUp on arrow keypad
    mapOISKeyCode["KC_LEFT"] = 0xCB;   // LeftArrow on arrow keypad
    mapOISKeyCode["KC_RIGHT"] = 0xCD;   // RightArrow on arrow keypad
    mapOISKeyCode["KC_END"] = 0xCF;   // End on arrow keypad
    mapOISKeyCode["KC_DOWN"] = 0xD0;   // DownArrow on arrow keypad
    mapOISKeyCode["KC_PGDOWN"] = 0xD1;   // PgDn on arrow keypad
    mapOISKeyCode["KC_INSERT"] = 0xD2;   // Insert on arrow keypad
    mapOISKeyCode["KC_DELETE"] = 0xD3;   // Delete on arrow keypad
    mapOISKeyCode["KC_LWIN"] = 0xDB;   // Left Windows key
    mapOISKeyCode["KC_RWIN"] = 0xDC;   // Right Windows key
    mapOISKeyCode["KC_APPS"] = 0xDD;   // AppMenu key
    mapOISKeyCode["KC_POWER"] = 0xDE;   // System Power
    mapOISKeyCode["KC_SLEEP"] = 0xDF;   // System Sleep
    mapOISKeyCode["KC_WAKE"] = 0xE3;   // System Wake
    mapOISKeyCode["KC_WEBSEARCH"] = 0xE5;   // Web Search
    mapOISKeyCode["KC_WEBFAVORITES"] = 0xE6;   // Web Favorites
    mapOISKeyCode["KC_WEBREFRESH"] = 0xE7;   // Web Refresh
    mapOISKeyCode["KC_WEBSTOP"] = 0xE8;   // Web Stop
    mapOISKeyCode["KC_WEBFORWARD"] = 0xE9;   // Web Forward
    mapOISKeyCode["KC_WEBBACK"] = 0xEA;   // Web Back
    mapOISKeyCode["KC_MYCOMPUTER"] = 0xEB;   // My Computer
    mapOISKeyCode["KC_MAIL"] = 0xEC;   // Mail
    mapOISKeyCode["KC_MEDIASELECT"] = 0xED;    // Media Select

    // Load user keys assignement from user modifiable text file
    loadConfig(&maMapConfig[CONFIG_KEYS], mResourcePath + pathFile);

    for (it = maMapConfig[CONFIG_KEYS].begin() ; it != maMapConfig[CONFIG_KEYS].end() ; it++) {
        if (mapClientKeyCode->count(it->first) && mapOISKeyCode.count(it->second)) {
            aKeyAssignement[(*mapClientKeyCode)[it->first]] = (IntCode)mapOISKeyCode[it->second];
        }
    };
}

} // END namespace
