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

#include "StaticGTSDSMAC.h"

Define_Module(StaticGTSDSMAC);

/***
 * Initialising some parameters, specific to Static GTS module
 * by overriding the startup() method. Important to call startup()
 * of the parent module in the end, otherwise it will not initialize
 ***/
void StaticGTSDSMAC::startup() {
	// initialise GTS-specific parameters
	GTSlist.clear(); totalGTS = 0; assignedGTS = 0;
	requestGTS = par("requestGTS");
	gtsOnly = par("gtsOnly");

	// other parameters are from DSMAC, need to read them for GTS scheduling
	totalSlots = par("numSuperframeSlots"); 	
	baseSlot = par("baseSlotDuration");
	minCap = par("minCAPLength");
	frameOrder = par("frameOrder");
	return DSMAC::startup();
}

/***
 * GTS request received by hub, need to return the number of 
 * slots to be granted. Can return 0 to indicate request denial
 ***/
int StaticGTSDSMAC::gtsRequest_hub(DSMACPacket *gtsPkt) {
	//Length of CAP after lengths of all GTS slots are subtracted
	int CAPlength = totalSlots;
	
	//check if the node already exists in the GTS list
	vector<DSMACGTSspec>::iterator i;
	int total = 0;
	for (i = GTSlist.begin(); i != GTSlist.end(); i++) {
		total++;
		if (i->owner == gtsPkt->getSrcID()) {
			if (i->length == gtsPkt->getGTSlength()) {
				return i->length;
			} else {
				totalGTS -= i->length;
				GTSlist.erase(i);
				total--;
			}
		} else {
			CAPlength -= i->length;
		}
	}
	
	//node not found, or requested slots changed
	if (total >= 7 || (CAPlength - gtsPkt->getGTSlength()) *
	    baseSlot * (1 << frameOrder) < minCap) {
		trace() << "GTS request from " << gtsPkt->getSrcID() <<
		    " cannot be acocmodated";
		return 0;
	}
	
	DSMACGTSspec newGTSspec;
	newGTSspec.length = gtsPkt->getGTSlength();
	totalGTS += newGTSspec.length;
	newGTSspec.owner = gtsPkt->getSrcID();
	GTSlist.push_back(newGTSspec);
	return newGTSspec.length;
}

/***
 * Hub can alter the beacon before broadcasting it
 * In particular, assign GTS slots and set CAP length
 ***/
void StaticGTSDSMAC::prepareBeacon_hub(DSMACPacket *beaconPacket) {
	int CAPlength = totalSlots;
	beaconPacket->setGTSlistArraySize(GTSlist.size());
	for (int i = 0; i < (int)GTSlist.size(); i++) {
		if (CAPlength > GTSlist[i].length) {
			CAPlength -= GTSlist[i].length;
			GTSlist[i].start = CAPlength + 1;
			beaconPacket->setGTSlist(i, GTSlist[i]);
		} else {
			trace() << "Internal ERROR: GTS list corrupted";
			GTSlist.clear(); totalGTS = 0;
			beaconPacket->setGTSlistArraySize(0);	
			CAPlength = totalSlots;
			break;
		}
	}
	beaconPacket->setCAPlength(CAPlength);
}

/***
 * If disconnected from PAN, also need to reset GTS slots
 ***/
void StaticGTSDSMAC::disconnectedFromPAN_node() {
	assignedGTS = 0;
}

/***
 * GTS request was successful
 ***/
void StaticGTSDSMAC::assignedGTS_node(int slots) {
	assignedGTS = slots;
}

/***
 * Transmission of data packet requested earlier is complete
 * status string holds comma separated list of outcomes
 * for each transmission attempt
 ***/
void StaticGTSDSMAC::transmissionOutcome(DSMACPacket *pkt, bool success, string status) {
	if (getAssociatedPAN() != -1) {
		if (assignedGTS == 0 && requestGTS > 0) {
			transmitPacket(newGtsRequest(getAssociatedPAN(), requestGTS));
		} else if (TXBuffer.size()) {
			DSMACPacket *packet = check_and_cast<DSMACPacket*>(TXBuffer.front());
			TXBuffer.pop();
			transmitPacket(packet,0,gtsOnly);
		}
	}
}

bool StaticGTSDSMAC::acceptNewPacket(DSMACPacket *newPacket) 
{
	if (getAssociatedPAN() != -1 && getCurrentPacket() == NULL) {
		transmitPacket(newPacket,0,gtsOnly);
		return true;
	}
	return bufferPacket(newPacket);
}

/***
 * Timers can be accessed by overwriting timerFiredCallback
 **/
/*
void StaticGTSDSMAC::timerFiredCallback(int index) {
	switch(index) {
		case NEW_TIMER: {
			//do something
			break;
		}
		
		default: {
			//important to call the function of the parent module
			DSMAC::timerFiredCallback(index);
		}
	}
}
*/
