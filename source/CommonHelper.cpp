#include "Common.h"

namespace Demo
{

Helper* const Helper::singleton = new Helper();

Helper::Helper()
{
}

Helper* const Helper::getSingle()
{
    return singleton;
}

Real h::clip(Real vValue, Real vMin, Real vMax)
{
    return (vValue < vMin)*vMin + (vMin <= vValue && vValue <= vMax)*vValue + (vMax < vValue)*vMax;
}

} // END namespace
