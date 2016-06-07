#ifndef __COMMONBASE_H__
#define __COMMONBASE_H__

#include <stack>
#define Coord Vec3

namespace Demo
{
const Real SINPIBY4 = (Real)std::sqrt((double)2)/2;
const Real COSPIBY4 = SINPIBY4;

class Vec3 : public NxOgre::Vec3
{
public:
    Vec3() :
        NxOgre::Vec3()
    {
    }

    Vec3(const Vec3& v) :
        NxOgre::Vec3(v.x, v.y, v.z)
    {
    }

    Vec3(const Real& x, const Real& y, const Real& z) :
        NxOgre::Vec3(x, y, z)
    {
    }

    Vec3(const NxOgre::Vec3& v) :
        NxOgre::Vec3(v.x, v.y, v.z)
    {
    }

    Vec3(const Vector3& v) :
        NxOgre::Vec3(v.x, v.y, v.z)
    {
    }

    Vec3(NxOgre::Quat& q) :
        NxOgre::Vec3(q.x, q.y, q.z)
    {
    }

    inline Vec3 operator- ()  const {
        return Vec3(-this->x, -this->y, -this->z);
    }

    inline Vec3 operator- (const Vec3& v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }

    inline Vec3 operator+ (const Vec3& v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }

    inline Vec3 operator* (const Vec3& v) const {
        return Vec3(x*v.x, y*v.y, z*v.z);
    }

    inline Vec3 operator* (const Real& s) const {
        return Vec3(x*s, y*s, z*s);
    }

    //HINT: no disvision by zero check
    inline Vec3 operator/ (const Vec3& v) const {
        return Vec3(x/v.x, y/v.y, z/v.z);
    }

    //HINT: no disvision by zero check
    inline Vec3 operator/ (const Real& s) const {
        const Real inv = (Real)1.0/s;
        return Vec3(x*inv, y*inv, z*inv);
    }
    
    inline Vec3 rot(NxOgre::Quat& q) const {
        return Vec3(this->rotQuat(q));
    }

    inline NxOgre::Quat rotQuat(NxOgre::Quat& q) const {
        return q*NxOgre::Quat(0, this->x, this->y, this->z)*NxOgre::Quat::invert(q);
    }
    
    inline Vec3 rot(const Real angle, const Vec3& axe) const {
        return Vec3(this->rotQuat(axe.getRotQuat(angle)));
    }
    
    inline NxOgre::Quat rotQuat(const Real angle, const Vec3& axe) const {
        return this->rotQuat(axe.getRotQuat(angle));
    }

    inline NxOgre::Quat getRotQuat(const Real angle) const {
        return NxOgre::Quat(std::cos(angle/2), std::sin(angle/2)*this->x, std::sin(angle/2)*this->y, std::sin(angle/2)*this->z);
    }
    
    Vec3 unit() const
    {
        Vec3 result     = Vec3(*this);
        Real magnitude  = this->magnitude();

        if (magnitude) {
            result /= magnitude;
        }
        return result;
    }

    Real projMagnitude(const Vec3& v) const
    {
        // ab/||a||||b|| . ||a||
        // => ab/||b||
        Real result     = 0;
        Real magnitude  = v.magnitude();

        if (magnitude) {
            result = this->dot(v)/magnitude;
        }
        return result;
    }

    Vec3 proj(const Vec3& v) const
    {
        // ab/||a||||b|| . ||a|| . a/||a||
        // => ab/||a||||b|| . a
        Vec3 result     = Vec3(0, 0, 0);
        Real product    = this->magnitudeSquared()*v.magnitudeSquared();

        if (product) {
            result = this->dot(v)/std::sqrt((double)product)**this;
        }
        return result;
    }
    
    // ***********************************************************
    // ************ ANGLE FROM U TO V: 2D SPACE *****************
    // ***********************************************************
    //
    // cos(a) ~ scalar product of U,V
    // sin(a) ~ scalar triple product of U,V
    //
    // (0,y,0) is the trigonometric "up" axis
    //
    Real angle(const Vec3& v) const
    {
        Real result = this->magnitudeSquared()*v.magnitudeSquared();

        if (result) {
             // Change angle signe when sinA = cos(A - Pi/2) is negative
            result = (this->x*v.x + this->z*v.z)/std::sqrt(result);
            result = (result < -1)*-1 + (-1 <= result && result <= 1)*result + (1 < result)*1;
            result = std::acos(result)*(1 - 2*(this->x*v.z - this->z*v.x < 0));
        }
        return result;
    }
    
    // ***********************************************************
    // ************ ANGLE FROM U TO V: 3D SPACE *****************
    // ***********************************************************
    //
    // cos(a) ~ scalar product of U,V
    // sin(a) ~ scalar triple product of U,V
    //
    // <this> is the chosen trigonometric "up" axis
    //
    NxOgre::Real angle(NxOgre::Quat& orientation, Vec3 v) const
    {
        Vec3 u = Vec3(0, 0, 1).rot(orientation);
        Real result = u.magnitudeSquared()*v.magnitudeSquared();

        if (result) {
            result = u.dot(v)/std::sqrt(result);
            result = (result < -1)*-1 + (-1 <= result && result <= 1)*result + (1 < result)*1;
            result = std::acos(result)*(1 - 2*(u.cross(v).dot(this->rot(orientation)) < 0));
        }
        return result;
    }
};

typedef IntCode ConfigType;
typedef IntCode InterfaceState;
typedef IntCode ClientMode;
typedef IntCode KeyCode;

const   IntVal              NB_OF_CELLS             =   280;
const   IntVal              CELL_RADIUS             =    60;
const   IntVal              NB_OF_RESSOURCES        =     4;
const   IntVal              MAX_AMMO                =   100;
const   IntVal              MAX_HEALTH              =   100;
const   IntVal              MAX_FUEL                =   100;
const   IntVal              NB_OF_PLAYERS           =     2;
const   IntVal              NB_OF_UNITS             =     5;
const   IntVal              NB_OF_VIRTUAL_UNITS     =     NB_OF_UNITS;

const   ConfigType          CONFIG_CLIENT           =     0;
const   ConfigType          CONFIG_COM              =     1;
const   ConfigType          CONFIG_KEYS             =     2;

// Interface state values are bitmask components
const   InterfaceState      INTERFACE_QUIT          =    0;
const   InterfaceState      INTERFACE_GAME          =    1;
const   InterfaceState      INTERFACE_UI            =    2;

const   IntCode             NB_OF_MODES             =     2;
const   ClientMode          MODE_PLAYER             =     0;
const   ClientMode          MODE_GHOST              =     1;

typedef struct CommandLineType {
    const CommandLineType(const bool isRemote, const bool isServer) :
        isRemote(isRemote), isServer(isServer)
    {}
    const bool isRemote;
    const bool isServer;
} CommandLine;

typedef struct ReferentielType {
    ReferentielType(const Vec3& position = Vec3(0, 0, 0), const NxOgre::Quat& orientation = NxOgre::Quat::IDENTITY) :
        orientation(orientation), position(position), speed(0), acceleration(0), rotation(0)
    {}
    NxOgre::Quat            orientation;
    Vec3                    position;
    Real                    speed;
    Real                    acceleration;
    Real                    rotation;
} Referentiel;

typedef struct InputType
{
    const InputType() :
        vMove(Vec3(0, 0, 0)), vRotate(Vec3(0, 0, 0)), isStable(false)
    {}
    bool* aKey;
    bool* aButton;
    Real* aAxe;
    Vec3  vMove;
    Vec3  vRotate;
    bool  isStable;
    bool  isInSight;
} Input;

typedef struct EdgeType {
    const EdgeType(Coord center, Vec3 const  normal, const bool isWalkable) :
        center(center), normal(normal), isWalkable(isWalkable)
    {}
    Coord           center;
    Vec3            normal;
    const bool      isWalkable;
} Edge;

typedef struct CellType {
    CellType() :
        id(0), center(Coord()), radius(0),
        aEdge(0), aConnected(0),
        cover(0), sight(0), width(0)
    {}
    const CellType(
        const IntCode id, Coord center, const IntVal radius,
        Edge** const aEdge, CellType** const aConnected,
        const IntCode cover, const IntCode sight, const IntCode width
    ) :
        id(id), center(center), radius(radius),
        aEdge(aEdge), aConnected(aConnected),
        cover(cover), sight(sight), width(width)
    {}
    const IntCode                   id;
    const IntVal                    radius;
    Coord                           center;
    Edge** const                    aEdge;
    CellType** const                aConnected;
    const IntCode                   cover;
    const IntCode                   sight;
    const IntCode                   width;
} Cell;

typedef std::queue<Cell*> CellQueue;

typedef struct PlayerDynDataType {
    const PlayerDynDataType() :
        unitId(0), aUnit(0), name(""),
        isThread(true), isRemote(false),
        isHuman(false), isActive(false),
        cell(0), coord(Coord())
    {
        aRessource[0] = 100;
        aRessource[1] = 100;
        aRessource[2] = 100;
    }
    const PlayerDynDataType(
        const IntVal unitId, PlayerDynDataType** const aUnit,
        const bool isRemote, const bool isThread,
        const String& name, const bool isHuman, const bool isActive,
        const Coord& coord, Cell* const cell
    ) :
        unitId(unitId), aUnit(aUnit),
        isThread(isThread), isRemote(isRemote),
        name(name), isHuman(isHuman), isActive(isActive),
        coord(coord), cell(cell)
    {
        aRessource[0] = 100;
        aRessource[1] = 100;
        aRessource[2] = 100;
    }
    const IntVal                    unitId;
    PlayerDynDataType** const       aUnit;
    const bool                      isRemote;
    const bool                      isThread;
    String                          name;
    bool                            isHuman;
    bool                            isActive;
    Cell*                           cell;
    Coord                           coord;
    NxOgre::Quat                    orientation;
    IntCode                         aRessource[3];
} PlayerDynData;

typedef std::vector<PlayerDynData**> PlayerDynDataVec;

typedef struct RessourceStaticDataType {
    const RessourceStaticDataType(Cell* const cell, Coord coord) :
        cell(cell), coord(coord)
    {}
    Cell* const                   cell;
    Coord                         coord;
} RessourceStaticData;

typedef struct RessourceDynDataType {
    const RessourceDynDataType(IntCode level, bool isFree) :
        level(level), isFree(isFree)
    {}
    IntCode                 level;
    bool                    isFree;
} RessourceDynData;

typedef struct GameStaticDataType {
    const GameStaticDataType(
        Cell** const aCell = 0, RessourceStaticData** const aRessourceStaticData = 0
    ) :
        aCell(aCell), aRessourceStaticData(aRessourceStaticData)
    {}
    Cell** const                    aCell;
    RessourceStaticData** const     aRessourceStaticData;
} GameStaticData;

typedef struct GameDynDataType {
    const GameDynDataType(
        PlayerDynData** const aPlayerDynData, RessourceDynData** const aRessourceDynData
    ) :
        aPlayerDynData(aPlayerDynData), aRessourceDynData(aRessourceDynData)
    {}
    PlayerDynData** const    aPlayerDynData;
    RessourceDynData** const aRessourceDynData;
} GameDynData;

} // END namespace

#endif
