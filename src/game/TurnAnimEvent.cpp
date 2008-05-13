#include "TurnAnimEvent.h"

#include <cmath>

#include "include/ccmap.h"
#include "include/common.h"
#include "include/Logger.h"
#include "include/ProjectileAnim.h"
#include "include/weaponspool.h"
#include "include/PlayerPool.h"
#include "UnitOrStructure.h"
#include "unittypes.h"
#include "Unit.h"
#include "ActionEventQueue.h"

namespace p {
	extern ActionEventQueue* aequeue;
}

TurnAnimEvent::TurnAnimEvent(Uint32 p, Unit *un, Uint8 dir, Uint8 layer) : UnitAnimEvent(p,un)
{
#ifdef LOOPEND_TURN
    Uint8 loopend=((UnitType*)un->type)->getAnimInfo().loopend;
#endif
    //logger->debug("Turn cons (t%p u%p d%i l%i)\n",this,un,dir,layer);
    Uint8 layerface;
    this->un = un;
    this->dir = dir;
    this->layer = layer;
    stopping = false;
#ifdef LOOPEND_TURN
    layerface = un->getImageNum(layer)&loopend;
    if( ((layerface-dir)&loopend) < ((dir-layerface)&loopend) ) {
#else
    layerface = un->getImageNum(layer)&0x1f;
    if( ((layerface-dir)&0x1f) < ((dir-layerface)&0x1f) ) {
#endif
        turnmod = -(((UnitType *)un->getType())->getTurnMod());
    } else {
        turnmod = (((UnitType *)un->getType())->getTurnMod());
    }
}
TurnAnimEvent::~TurnAnimEvent()
{
    //logger->debug("TurnAnim dest\n");
    if (layer == 0) {
        if (un->turnanim1 == this)
            un->turnanim1 = NULL;
    } else {
        if (un->turnanim2 == this)
            un->turnanim2 = NULL;
    }
}
void TurnAnimEvent::run()
{
#ifdef LOOPEND_TURN
    Uint8 loopend=((UnitType*)un->type)->getAnimInfo().loopend;
#endif
    Uint8 layerface;

    //logger->debug("TurnAnim run (s%i)\n",stopping);
    if (stopping) {
        delete this;
        return;
    }

#ifdef LOOPEND_TURN
    layerface = un->getImageNum(layer)&loopend;
    if( abs((layerface-dir)&loopend) > abs(turnmod) ) {
        layerface += turnmod;
        layerface &= loopend;
#else
    layerface = un->getImageNum(layer)&0x1f;
    if( abs((layerface-dir)&0x1f) > abs(turnmod) ) {
        layerface += turnmod;
        layerface &= 0x1f;
#endif
    } else
        layerface = dir;
//		if (un->getOwner() == p::ppool->getLPlayerNum())
//			printf ("%s line %i: TurnAnimEvent layerface %i, dir = %i, turnmod = %i\n", __FILE__, __LINE__, layerface, dir, turnmod);
		un->setImageNum(layerface,layer);

    if( layerface == dir || !un->isAlive()) {
        delete this;
        return;
    }
    p::aequeue->scheduleEvent(this);
}
    
void TurnAnimEvent::stop()
{
	stopping = true;
}

void TurnAnimEvent::update() 
{	
}

void TurnAnimEvent::changedir(Uint8 ndir) 
{
	stopping = false;
	dir = ndir;
}
