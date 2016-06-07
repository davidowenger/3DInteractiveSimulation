#ifndef __COMMONHELPER_H__
#define __COMMONHELPER_H__

namespace Demo
{

typedef unsigned short                        IntCode;
typedef unsigned int                          IntVal;
typedef std::map<std::string, int>            IntMap;
typedef std::map<std::string, std::string>    StringMap;

class Helper
{
public:
    static Helper* const                singleton;

    static Helper* const                getSingle();

    const Helper();

    template<typename T> inline String  appendNum(String& base, T number) const
    {
        std::ostringstream oss;

        oss << base << number;

        return oss.str();
    }
};

namespace h 
{
    Real clip(Real vValue, Real vMin, Real vMax);
}

} // END namespace

#endif
 