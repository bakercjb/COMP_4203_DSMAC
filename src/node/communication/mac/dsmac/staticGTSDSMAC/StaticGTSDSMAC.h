/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2012                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Yuriy Tselishchev                                            *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *  
 ****************************************************************************/

#ifndef STATIC_GTS_DSMAC_H_
#define STATIC_GTS_DSMAC_H_

#include <string>
#include <vector>

#include "DSMAC.h"
#include "DSMACPacket_m.h"

class StaticGTSDSMAC: public DSMAC {
 protected:
 	/*--- DSMACMac GTS list --- */
	vector<DSMACGTSspec> GTSlist;	// list of GTS specifications (for PAN coordinator)
	int assignedGTS,requestGTS,totalGTS,totalSlots,baseSlot,minCap,frameOrder;
	bool gtsOnly;

	virtual void startup();
	virtual int gtsRequest_hub(DSMACPacket *);
	virtual void prepareBeacon_hub(DSMACPacket *);
	virtual void disconnectedFromPAN_node();
	virtual void assignedGTS_node(int);
	virtual bool acceptNewPacket(DSMACPacket *);
	virtual void transmissionOutcome(DSMACPacket *, bool, string);
};

#endif	//STATIC_GTS_DSMAC
