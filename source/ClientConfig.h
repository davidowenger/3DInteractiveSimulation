#ifndef __CLIENTCONFIG_H__
#define __CLIENTCONFIG_H__

namespace Demo
{

class ClientConfig
{
public:
    static ClientConfig* const          getSingle();

    const ClientConfig();

    void                                init(String& path, const String& fileClientConfig, const String& fileComConfig);
    const String                        getPath(const String &path);
    String                              getString(const String& key, const ConfigType type = CONFIG_CLIENT);
    bool                                getBool(const String& key, const ConfigType type = CONFIG_CLIENT);
    void                                getKeysAssignement(const String &pathFile, IntCode* aKeyAssignement, IntMap* mapClientKeyCode);

protected:
    static ClientConfig* const          singleton;

    void                                loadConfig(StringMap* const mapConfig, const String& pathFile);

    String                              mResourcePath;
    StringMap                           maMapConfig[3];
};

} // END namespace

#endif
