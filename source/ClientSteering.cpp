#include "Common.h"

namespace Demo
{

ClientSteering::ClientSteering(
    Rendering* const rendering, Wrapper* const wrapper,
    GameStaticData* const gameStaticData, GameDynData* const gameDynData,
    ClientSquad* const squad
) :
    mDistance(0), mRendering(rendering), mWrapper(wrapper),
    mGameStaticData(gameStaticData), mGameDynData(gameDynData),
    mSquad(squad), mSquadData(squad->mSquadData),
    mLeader(squad->mLeader), mLeaderId(squad->mLeader->mId), maUnit(squad->maUnit),
    maPlayerDynData(gameDynData->aPlayerDynData)
{
}

typedef struct NodeType {
    NodeType(Cell* c) :
        c(c), p(0)
    {
    }
    int heuristic(NodeType* n) {
        return dist(n);
    }
    int cost(NodeType* n) {
        return dist(n);
    }
    int dist(NodeType* n) {
        return (n->c->center - c->center).magnitude();
    }
    CellQueue* getPath() {
        CellQueue* s;
        
        if (p) {
            s = p->getPath();
        } else {
            s = new CellQueue();
        }
        s->push(c);
        return s;
    }
    NodeType*   p;
    Cell*       c;
    int         f;
    int         g;
    int         h;
} Node;

typedef struct NodeSetType {
    NodeSetType() :
        n(0), s(0)
    {
    }
    NodeSetType(Node* n) :
        n(n), s(0)
    {
    }
   ~NodeSetType() {
        if (s) {
            delete s;
        }
    }
    void add(Node* n) {
        if (this->n && this->n->f >= n->f) {
            NodeSet*    s   = new NodeSet(this->n);
            s->s            = this->s;
            this->n         = n;
            this->s         = s;
        } else if (s) {
            this->s         ->add(n);
        } else if (this->n) {
            NodeSet*    s   = new NodeSet(n);
            this->s         = s;
        } else {
            this->n         = n;
        }
    }
    void remove(Node* n) {
        if (n == this->n) {
            if (s) {
                NodeSet* t = s;
                this->n = s->n;
                s = s->s;
                t->s = 0;
                delete t;
            } else {
                n = 0;
            }
        } else if (s) {
            if (n == s->n) {
                NodeSet* t = s;
                s = s->s;
                t->s = 0;
                delete t;
            } else {
                this->s->remove(n);
            }
        }
    }
    Node* get(Cell* c) {
        Node* result = 0;

        if (n && c == n->c) {
            result = n;
        } else if (s) {
            result = s->get(c);
        }
        return result;
    }
    void deleteAll() {
        if (s) {
            s->deleteAll();
        }
        delete n;
    }
    Node*           n;
    NodeSetType*    s;
} NodeSet;

CellQueue* ClientSteering::getPath(Cell* start, Cell* dest, Cell* avoid)
{
    // A* based algorithm
    CellQueue*  result      = new CellQueue();
    Node*       x           = new Node(start);
    Node*       z           = new Node(dest);
    NodeSet     openSet     = NodeSet(x);
    NodeSet     closedSet   = NodeSet();
    NodeSet     allSet      = NodeSet(x);

    x->g    = 0;
    x->h    = x->heuristic(z);
    x->f    = x->g + x->h;

    while (openSet.n) {
         x = openSet.n;

         if (x->c == dest) {
             result = x->getPath();  // reconstruct path with max value
             break;
         }
         openSet .remove(x);
         closedSet.add(x);

         if (x->c != avoid) {
            for (IntVal index = 1 ; index < 7 ; ++index) {
                Cell*   c           = x->c->aConnected[index];

                if (!c || closedSet.get(c)) {
                    continue;
                }
                bool    explored = true;
                Node*   y = openSet.get(c);

                if (!y) {
                    explored = false;
                    y = new Node(c);
                }
                int g = x->g + x->cost(y);

                if (!explored || g < y->g) {
                    y->p = x;
                    y->g = g;
                    y->h = y->heuristic(z);
                    y->f = y->g + y->h;

                    if (!explored) {
                        openSet      .add(y);
                        allSet       .add(y);
                    }
                }
            }
        }
    }
    allSet.deleteAll();
    return result;
}

void ClientSteering::advanceAI()
{
    maUnit[NB_OF_UNITS]->mCoord = ( 
        mSquadData->currentLeaderStrat->mPath->size() ? 
        mSquadData->currentLeaderStrat->mPath->front()->center :
        mSquadData->currentLeaderStrat->mDest->center 
    );
    computeForces(0, 25.0f);
}

void ClientSteering::advance()
{
    // Compute forces according to current squad strategie
    // FOLLOW:              each unit follows the one with smaller index
    // GOTO:                leader is at the center of an arc of 5 units
    // ATTACK_AND_DEFEND:   leader is the center of a rectangle of 4 units
    Real vLimit = 15.0f;

    switch (mSquadData->currentSquadStrat->mType) {
    case FOLLOW:
        maUnit[NB_OF_UNITS + 1]->mCoord = maPlayerDynData[mLeaderId    ]->coord;
        maUnit[NB_OF_UNITS + 2]->mCoord = maPlayerDynData[mLeaderId + 1]->coord;
        maUnit[NB_OF_UNITS + 3]->mCoord = maPlayerDynData[mLeaderId + 2]->coord;
        maUnit[NB_OF_UNITS + 4]->mCoord = maPlayerDynData[mLeaderId + 3]->coord;
        vLimit = 50.0f;
        break;
    case GOTO:
        maUnit[NB_OF_UNITS + 1]->mCoord = maPlayerDynData[mLeaderId]->coord + Vec3( 40.0f, 0.0f, 30.0f).rot(maPlayerDynData[mLeaderId]->orientation);
        maUnit[NB_OF_UNITS + 2]->mCoord = maPlayerDynData[mLeaderId]->coord + Vec3(-40.0f, 0.0f, 30.0f).rot(maPlayerDynData[mLeaderId]->orientation);
        maUnit[NB_OF_UNITS + 3]->mCoord = maPlayerDynData[mLeaderId]->coord + Vec3( 80.0f, 0.0f, 55.0f).rot(maPlayerDynData[mLeaderId]->orientation);
        maUnit[NB_OF_UNITS + 4]->mCoord = maPlayerDynData[mLeaderId]->coord + Vec3(-80.0f, 0.0f, 55.0f).rot(maPlayerDynData[mLeaderId]->orientation);
        break;
    case ATTACK_AND_DEFEND:
        maUnit[NB_OF_UNITS + 1]->mCoord = maPlayerDynData[mLeaderId]->coord + Vec3( 30.0f, 0.0f,  30.0f).rot(maPlayerDynData[mLeaderId]->orientation);
        maUnit[NB_OF_UNITS + 2]->mCoord = maPlayerDynData[mLeaderId]->coord + Vec3(-30.0f, 0.0f,  30.0f).rot(maPlayerDynData[mLeaderId]->orientation);
        maUnit[NB_OF_UNITS + 3]->mCoord = maPlayerDynData[mLeaderId]->coord + Vec3( 30.0f, 0.0f, -30.0f).rot(maPlayerDynData[mLeaderId]->orientation);
        maUnit[NB_OF_UNITS + 4]->mCoord = maPlayerDynData[mLeaderId]->coord + Vec3(-30.0f, 0.0f, -30.0f).rot(maPlayerDynData[mLeaderId]->orientation);
        break;
    }
    for (IntCode vUnitId = 1 ; vUnitId < NB_OF_UNITS ; ++vUnitId) {
        computeForces(vUnitId, vLimit);
    }
}

void ClientSteering::computeForces(IntCode vUnitId, Real vLimit)
{
    Cell* vCell;
    Vec3 vForce;
    Real vMagnitude;
    maUnit[vUnitId]->mAvoidance = Vec3(0.0f, 0.0f, 0.0f);

    for (IntVal id = 0 ; id < 7 && maPlayerDynData[mLeaderId]->cell->aConnected[id] ; ++id) {
        vCell = maPlayerDynData[mLeaderId]->cell->aConnected[id];

        for (IntVal e = 0 ; e < 6 && vCell->aEdge[e] ; ++e) {
            vMagnitude = (vCell->aEdge[e]->center - maPlayerDynData[mLeaderId + vUnitId]->coord).magnitude();
            maUnit[vUnitId]->mAvoidance += vCell->aEdge[e]->normal*((vMagnitude < 40.0f)*12500.0f/(vMagnitude*vMagnitude));
        }
    }
    for (IntVal vUnitIdOther = 0 ; vUnitIdOther < NB_OF_PLAYERS*NB_OF_UNITS ; ++vUnitIdOther) {
        if (vUnitIdOther != mLeaderId + vUnitId) {
            vForce = maPlayerDynData[vUnitIdOther]->coord - maPlayerDynData[mLeaderId + vUnitId]->coord;
            vMagnitude = vForce.normalise();
            maUnit[vUnitId]->mAvoidance -= vForce*((vMagnitude < 40.0f)*12500.0f/(vMagnitude*vMagnitude));
        }
    }
    vForce = maUnit[NB_OF_UNITS + vUnitId]->mCoord - maPlayerDynData[mLeaderId + vUnitId]->coord;
    vMagnitude = vForce.normalise();
    maUnit[vUnitId]->mCohesion = vForce*30.0f;
    maUnit[vUnitId]->mInput->isStable = vMagnitude*vMagnitude < vLimit*vLimit;
}

} // END namespace
