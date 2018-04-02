//****************************************************************************
//*  Copyright: National ICT Australia,  2007 - 2012                         *
//*  Developed at the ATP lab, Networked Systems research theme              *
//*  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
//*  This file is distributed under the terms in the attached LICENSE file.  *
//*  If you do not find this file, copies can be found by writing to:        *
//*                                                                          *
//*      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
//*      Attention:  License Inquiry.                                        *
//*                                                                          *  
//****************************************************************************/

cplusplus {{
#include "MacPacket_m.h"
}}

class MacPacket;

enum DSMACPacket_type {
	MAC_DSMAC_BEACON_PACKET = 1001;
	MAC_DSMAC_ASSOCIATE_PACKET = 1002;
	MAC_DSMAC_DATA_PACKET = 1003;
	MAC_DSMAC_ACK_PACKET = 1004;
	MAC_DSMAC_GTS_REQUEST_PACKET = 1005;
}

struct DSMACGTSspec {
	int owner;
	int start;
	int length;
}

packet DSMACPacket extends MacPacket {
	int DSMACPacketType enum (DSMACPacket_type);
	int leaderNodeid;
	int srcID;
	int dstID;
	int seqNum;
	int sessionIDPkt;

	// those fields belong to beacon packet (MAC_DSMAC_BEACON_PACKET)
	int beaconOrder;
	int frameOrder;
	int BSN;
	int CAPlength;
	int GTSlength;
	DSMACGTSspec GTSlist[];
}

