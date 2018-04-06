/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2012                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *
 ****************************************************************************/

#include "DSMAC.h"
#include <cstdlib>

// This module is virtual and can not be used directly
Define_Module(DSMAC); 

void DSMAC::startup()
{
	printStateTransitions = par("printStateTransitions");
	if (printStateTransitions) {
		stateDescr[1000] = "MAC_STATE_SETUP";
		stateDescr[1001] = "MAC_STATE_SLEEP";
		stateDescr[1002] = "MAC_STATE_CAP";
		stateDescr[1003] = "MAC_STATE_GTS";
	}

	isLeaderNode = par("isLeaderNode");
	isFFD = par("isFFD");

	// CAP-related parameters
	minCAPLength = par("minCAPLength");

	// Slot parameters
	unitBackoffPeriod = par("unitBackoffPeriod");
	baseSlotDuration = par("baseSlotDuration");
	numSuperframeSlots = par("numSuperframeSlots");
	baseSuperframeDuration = baseSlotDuration * numSuperframeSlots;

	// CSMA parameters
	enableSlottedCSMA = par("enableSlottedCSMA");
	macMinBE = par("macMinBE");
	macMaxBE = par("macMaxBE");
	macMaxCSMABackoffs = par("macMaxCSMABackoffs");
	macMaxFrameRetries = par("macMaxFrameRetries");
	batteryLifeExtention = par("batteryLifeExtention");

	// Physical layer parameters	
	phyDataRate = par("phyDataRate");
	phyDelaySleep2Tx = (double)par("phyDelaySleep2Tx") / 1000.0;
	phyDelayRx2Tx = (double)par("phyDelayRx2Tx") / 1000.0;
	phyDelayForValidCS = (double)par("phyDelayForValidCS") / 1000.0;
	phyLayerOverhead = par("phyFrameOverhead");
	phyBitsPerSymbol = par("phyBitsPerSymbol");
	symbolLen = 1 / (phyDataRate * 1000 / phyBitsPerSymbol);
	ackWaitDuration = symbolLen * unitBackoffPeriod +
			phyDelayRx2Tx * 2 + TX_TIME(ACK_PKT_SIZE);

	// HUB connection parameters
	guardTime = (double)par("guardTime") / 1000.0;
	maxLostBeacons = par("maxLostBeacons");

	// General MAC initialisation
	currentPacket = NULL;
	macState = MAC_STATE_SETUP;
	associatedLeaderNode = -1;
	
	sessionID = 100; // sessionID
	lastLeaderSessionID = 100;

	currentFrameStart = 0;
	GTSstart = 0;
	GTSend = 0;
	CAPend = 0;
	seqNum = 0;

	// Statistics for reporting
	lostBeacons = 0;
	sentBeacons = 0;
	recvBeacons = 0;
	packetoverflow = 0;
	desyncTime = 0;
	desyncTimeStart = 0;
	packetBreak.clear();
	declareOutput("pkt TX state breakdown");
	
	// Leader node initialisation
	if (isLeaderNode) {
	
		if (!isFFD) {
			opp_error("Only full-function devices (isFFD=true) can be leader nodes");
		}

		associatedLeaderNode = SELF_MAC_ADDRESS;
		macBSN = genk_intrand(0, 255) + 1;
		
		//initialise frameOrder and beaconOrder 
		frameOrder = par("frameOrder");
		beaconOrder = par("beaconOrder");
		if (frameOrder < 0 || beaconOrder < 0 || beaconOrder > 14
		    || frameOrder > 14 || beaconOrder < frameOrder) {
			opp_error("Invalid combination of frameOrder and beaconOrder parameters. Must be 0 <= frameOrder <= beaconOrder <= 14");
		}

		beaconInterval = baseSuperframeDuration * (1 << beaconOrder);
		frameInterval = baseSuperframeDuration * (1 << frameOrder);
		CAPlength = numSuperframeSlots;

		if (beaconInterval <= 0 || frameInterval <= 0) {
			opp_error("Invalid parameter combination of baseSlotDuration and numSuperframeSlots");
		}

		// Initialize Leader node session ID table. 
		sessionIDMap.insert(pair <int,int> (1, 100));
		sessionIDMap.insert(pair <int,int> (2, 100));
		sessionIDMap.insert(pair <int,int> (3, 100));
		sessionIDMap.insert(pair <int,int> (4, 100));
		sessionIDMap.insert(pair <int,int> (5, 90)); //outdated SID for "malicious node"

		setTimer(FRAME_START, 0);	//frame start is NOW
	}
}

void DSMAC::timerFiredCallback(int index)
{
	switch (index) {
		
		// Start of a new superframe
		case FRAME_START: {
			if (isLeaderNode) {	// as a leader node, create and broadcast beacon packet
				beaconPacket = new DSMACPacket("Leader node beacon packet", MAC_LAYER_PACKET);
				beaconPacket->setDstID(BROADCAST_MAC_ADDRESS);
				beaconPacket->setLeaderNodeid(SELF_MAC_ADDRESS);
				beaconPacket->setDSMACPacketType(MAC_DSMAC_BEACON_PACKET);
				
							
				beaconPacket->setSessionIDPkt(getSessionID() + 1); // session ID packet

				beaconPacket->setBeaconOrder(beaconOrder);
				beaconPacket->setFrameOrder(frameOrder);
				if (++macBSN > 255) macBSN = 0;
				beaconPacket->setBSN(macBSN);
				beaconPacket->setCAPlength(numSuperframeSlots);
				
				// GTS fields and CAP length are set in the decision layer
				prepareBeacon_hub(beaconPacket);
				
				beaconPacket->setByteLength(BASE_BEACON_PKT_SIZE + beaconPacket->getGTSlistArraySize() * GTS_SPEC_FIELD_SIZE);
				CAPlength = beaconPacket->getCAPlength();
				CAPend = CAPlength * baseSlotDuration * (1 << frameOrder) * symbolLen;
				sentBeacons++;

				trace() << "Transmitting [Leader node beacon packet] now, BSN = " << macBSN << " LSID = " << getSessionID()+1;
				setMacState(MAC_STATE_CAP);
				toRadioLayer(beaconPacket);
				toRadioLayer(createRadioCommand(SET_STATE, TX));
				setTimer(ATTEMPT_TX, TX_TIME(beaconPacket->getByteLength()));
				beaconPacket = NULL;

				setSessionID(getSessionID() + 1); // increment session ID

				currentFrameStart = getClock() + phyDelayRx2Tx;
				setTimer(FRAME_START, beaconInterval * symbolLen);
			} else {	// if not a leader node, then wait for beacon
				toRadioLayer(createRadioCommand(SET_STATE, RX));
				setTimer(BEACON_TIMEOUT, guardTime * 3);
			}
			break;
		}

		case GTS_START: {
			if (macState == MAC_STATE_SLEEP) {
				toRadioLayer(createRadioCommand(SET_STATE, RX));
			}
			setMacState(MAC_STATE_GTS);
			
			// we delay transmission attempt by the time requred by radio to wake up
			// note that GTS_START timer was scheduled exactly phyDelaySleep2Tx seconds
			// earlier than the actual start time of GTS slot
			setTimer(ATTEMPT_TX, phyDelaySleep2Tx);

			// set a timer to go to sleep after this GTS slot ends
			setTimer(SLEEP_START, phyDelaySleep2Tx + GTSlength);

			// inform the decision layer that GTS has started
			startedGTS_node();
			break;
		}

		// beacon timeout fired - indicates that beacon was missed by this node
		case BEACON_TIMEOUT: {
			lostBeacons++;
			if (lostBeacons >= maxLostBeacons) {
				trace() << "Lost synchronisation with leader node " << associatedLeaderNode;
				setMacState(MAC_STATE_SETUP);
				associatedLeaderNode = -1;
				desyncTimeStart = getClock();
				disconnectedFromLeader_node();
				if (currentPacket) clearCurrentPacket("No leader node");
			} else if (associatedLeaderNode != -1) {
				trace() << "Missed beacon from leader node " << associatedLeaderNode <<
				    ", will wake up to receive next beacon in " <<
				    beaconInterval * symbolLen - guardTime * 3 << " seconds";
				setMacState(MAC_STATE_SLEEP);
				toRadioLayer(createRadioCommand(SET_STATE, SLEEP));
				setTimer(FRAME_START, beaconInterval * symbolLen - guardTime * 3);
			}
			break;
		}
		
		// packet was not received
		case ACK_TIMEOUT: {
			collectPacketHistory("NoAck");
			attemptTransmission("ACK timeout");
			break;
		}

		// previous transmission is reset, attempt a new transmission
		case ATTEMPT_TX: {
			if (getTimer(ACK_TIMEOUT) != -1) break;
			attemptTransmission("ATTEMPT_TX timer");
			break;
		}

		// timer to preform Clear Channel Assessment (CCA) 
		case PERFORM_CCA: {
			if (macState == MAC_STATE_GTS || macState == MAC_STATE_SLEEP) break;
			CCA_result CCAcode = radioModule->isChannelClear();
			if (CCAcode == CLEAR) {
				//Channel clear
				if (--CW > 0) {
					setTimer(PERFORM_CCA, unitBackoffPeriod * symbolLen);
				} else {
					transmitCurrentPacket();
				}
			} else if (CCAcode == BUSY) {
				//Channel busy
				CW = enableSlottedCSMA ? 2 : 1;
				if (++BE > macMaxBE)
					BE = macMaxBE;
				if (++NB > macMaxCSMABackoffs) {
					collectPacketHistory("CSfail");
					currentPacketRetries--;
					attemptTransmission("Current NB exeeded maxCSMAbackoffs");
				} else {
					performCSMACA();
				}
			} else if (CCAcode == CS_NOT_VALID_YET) {
				//Clear Channel Assesment (CCA) pin is not valid yet
				setTimer(PERFORM_CCA, phyDelayForValidCS);
			} else {	
				//Clear Channel Assesment (CCA) pin is not valid at all (radio is sleeping?)
				trace() << "ERROR: isChannelClear() called when radio is not ready";
				toRadioLayer(createRadioCommand(SET_STATE, RX));
			}
			break;
		}

		case SLEEP_START: {
			// SLEEP_START timer can sometimes be scheduled in the end of a frame
			// i.e. when BEACON_ORDER = FRAME_ORDER, overlapping with the interval 
			// when a node already tries to prepare for beacon reception. Thus 
			// check if BEACON_TIMEOUT timer is set before going to sleep
			if (getTimer(BEACON_TIMEOUT) != -1) break;

			cancelTimer(PERFORM_CCA);
			setMacState(MAC_STATE_SLEEP);
			toRadioLayer(createRadioCommand(SET_STATE, SLEEP));
			trace() << "SLEEP STARTED FOR A NODE\n";
			break;
		}
		
		case BACK_TO_SETUP: {
			// This timer is scheduled to the end of the CAP period 
			// when beacon is received, but node is not (yet) connected.
			// So when this timer fires and node is not connected, it 
			// has to go back to setup stage
			if (associatedLeaderNode == -1) setMacState(MAC_STATE_SETUP);
		}
	}
}

/* A packet is received from upper layer (Network)
 */
void DSMAC::fromNetworkLayer(cPacket * pkt, int dstMacAddress)
{
	DSMACPacket *macPacket = new DSMACPacket("DSMAC data packet", MAC_LAYER_PACKET);
	encapsulatePacket(macPacket, pkt);
	macPacket->setSrcID(SELF_MAC_ADDRESS);	//if connected to leader node, would have a short MAC address assigned,
											//but we are not using short addresses in this model
	macPacket->setDstID(dstMacAddress);
	macPacket->setDSMACPacketType(MAC_DSMAC_DATA_PACKET);
	macPacket->setSeqNum(seqNum++);
	if (seqNum > 255) seqNum == 0;
	if (!acceptNewPacket(macPacket)) packetoverflow++;
}

void DSMAC::finishSpecific()
{
	if (currentPacket)
		cancelAndDelete(currentPacket);
	if (desyncTimeStart >= 0)
		desyncTime += getClock() - desyncTimeStart;

	map <string,int>::const_iterator iter;
	declareOutput("Packet breakdown");
	if (packetoverflow > 0)
		collectOutput("Packet breakdown", "Failed, buffer overflow", packetoverflow);
	for (iter = packetBreak.begin(); iter != packetBreak.end(); ++iter) {
		if (iter->first.compare("Success") == 0) {
			collectOutput("Packet breakdown", "Success, first try", iter->second);
		} else if (iter->first.compare("Broadcast") == 0) {
			collectOutput("Packet breakdown", "Broadcast", iter->second);
		} else if (iter->first.find("Success") != string::npos) {
			collectOutput("Packet breakdown", "Success, not first try", iter->second);
		} else if (iter->first.find("NoAck") != string::npos) {
			collectOutput("Packet breakdown", "Failed, no ack", iter->second);
		} else if (iter->first.find("CSfail") != string::npos) {
			collectOutput("Packet breakdown", "Failed, busy channel", iter->second);
		} else if (iter->first.find("NoPAN") != string::npos) {
			collectOutput("Packet breakdown", "Failed, no leader node", iter->second);
		} else {
			trace() << "Unknown packet breakdonw category: " <<
				iter->first << " with " << iter->second << " packets";
		}
	}

	if (!isLeaderNode) {
		if (desyncTime > 0) {
			declareOutput("Fraction of time without leader node connection");
			collectOutput("Fraction of time without leader node connection", "",
				SIMTIME_DBL(desyncTime) / SIMTIME_DBL(getClock()));
		}
		declareOutput("Beacons received");
		collectOutput("Beacons received", "", recvBeacons);
	} else {
		declareOutput("Beacons sent");
		collectOutput("Beacons sent", "", sentBeacons);
	}
}


/* Helper function to change internal MAC state and print a debug statement if neccesary 
 */
void DSMAC::setMacState(int newState)
{
	if (macState == newState)
		return;
	if (printStateTransitions)
		trace() << "MAC state changed from " << stateDescr[macState] << " to " << stateDescr[newState];
	macState = newState;
}

DSMACPacket *DSMAC::newConnectionRequest(int leaderNodeid) {
	DSMACPacket *result = new DSMACPacket("Leader node connection request", MAC_LAYER_PACKET);
	result->setDstID(leaderNodeid);
	result->setLeaderNodeid(leaderNodeid);
	result->setDSMACPacketType(MAC_DSMAC_ASSOCIATE_PACKET);

	result->setSessionIDPkt(getSessionID()); // session ID packet

	result->setSrcID(SELF_MAC_ADDRESS);
	result->setByteLength(COMMAND_PKT_SIZE);

	trace() << "Transmitting Leader node connection request, SID = " << getSessionID();

	
	
	return result;
}

DSMACPacket *DSMAC::newGtsRequest(int leaderNodeid, int slots) {
	DSMACPacket *result = new DSMACPacket("GTS request", MAC_LAYER_PACKET);
	result->setLeaderNodeid(leaderNodeid);
	result->setDstID(leaderNodeid);
	result->setDSMACPacketType(MAC_DSMAC_GTS_REQUEST_PACKET);
	result->setSrcID(SELF_MAC_ADDRESS);
	result->setGTSlength(slots);
	result->setByteLength(COMMAND_PKT_SIZE);
	return result;
}

/* This function will handle a MAC frame received from the lower layer (physical or radio)
 */
void DSMAC::fromRadioLayer(cPacket * pkt, double rssi, double lqi)
{
	DSMACPacket *rcvPacket = dynamic_cast<DSMACPacket*>(pkt);
	if (!rcvPacket) {
		return;
	}

	if (rcvPacket->getDstID() != SELF_MAC_ADDRESS && 
			rcvPacket->getDstID() != BROADCAST_MAC_ADDRESS) {
		return;
	}


	switch (rcvPacket->getDSMACPacketType()) {

		/* received a BEACON frame */
		case MAC_DSMAC_BEACON_PACKET: {
			
			
			if (isLeaderNode)
				break;			//Leader nodes ignore beacons from other leader nodes
			if (associatedLeaderNode != -1 && associatedLeaderNode != rcvPacket->getLeaderNodeid()) 
				break;			//Ignore, if associated to another leader node

			if (rcvPacket->getSessionIDPkt() <= getLastLeaderSessionID()) {
				trace() << "Outdated beacon frame. Ignoring packet. Last LSID on file = " << getLastLeaderSessionID();
				trace() << "LSID received = " << rcvPacket->getSessionIDPkt();
				break;			//Ignore, outdated session ID
			}



			// Special case for node 5, our malicious node
			if (SELF_MAC_ADDRESS == 5) {
				setLastLeaderSessionID(rcvPacket->getSessionIDPkt()); // latest leader id
				setSessionID(90);
				
			} else {
				setLastLeaderSessionID(rcvPacket->getSessionIDPkt()); // latest leader id
				setSessionID(getSessionID() + 1);
			}


			//cancel beacon timeout message (if present)
			cancelTimer(BEACON_TIMEOUT);
			recvBeacons++;

			//this node is connected to this leader node (or will try to connect), update frame parameters
			double offset = TX_TIME(rcvPacket->getByteLength());
			currentFrameStart = getClock() - offset;	//frame start is in the past
			lostBeacons = 0;
			frameOrder = rcvPacket->getFrameOrder();
			beaconOrder = rcvPacket->getBeaconOrder();
			beaconInterval = baseSuperframeDuration * (1 << beaconOrder);
			macBSN = rcvPacket->getBSN();
			CAPlength = rcvPacket->getCAPlength();
			CAPend = CAPlength * baseSlotDuration * (1 << frameOrder) * symbolLen;
			GTSstart = 0;
			GTSend = 0;
			GTSlength = 0;

			for (int i = 0; i < (int)rcvPacket->getGTSlistArraySize(); i++) {
				if (rcvPacket->getGTSlist(i).owner == SELF_MAC_ADDRESS) {
					GTSstart = (rcvPacket->getGTSlist(i).start - 1) *
						baseSlotDuration * (1 << frameOrder) * symbolLen;
					GTSend = GTSstart + rcvPacket->getGTSlist(i).length *
					    baseSlotDuration * (1 << frameOrder) * symbolLen;
					GTSlength = GTSend - GTSstart;
					trace() << "GTS slot from " << getClock() + GTSstart << 
						" to " << getClock() + GTSend << " length " << GTSlength;
				}
			}

			setMacState(MAC_STATE_CAP);
			if (associatedLeaderNode == rcvPacket->getLeaderNodeid()) {
				if (GTSstart != CAPend)
					// set timer to sleep after CAP, unless GTS slots starts right after
					setTimer(SLEEP_START, CAPend - offset);
				if (GTSstart > 0) {
					// set GTS timer phyDelaySleep2Tx seconds earlier as radio can be sleeping
					setTimer(GTS_START, GTSstart - phyDelaySleep2Tx - offset);
				} 
			} else {
				setTimer(BACK_TO_SETUP, CAPend - offset);
			}
			

			receiveBeacon_node(rcvPacket);
			attemptTransmission("CAP started");
			setTimer(FRAME_START, baseSuperframeDuration * (1 << beaconOrder) *
				 symbolLen - guardTime - offset);
			break;
		}

		// received request to associate
		case MAC_DSMAC_ASSOCIATE_PACKET:{
		
			// only leader nodes can accept association requests
			// if multihop communication is to be allowed - then this has to be changed
			// in particular, any FFD can become a coordinator and accept requests
			if (!isLeaderNode)
				break;

			// if leader node id does not match - do nothing
			if (rcvPacket->getLeaderNodeid() != SELF_MAC_ADDRESS)
				break;



			map <int, int> :: iterator itr; //leader node session ID table

			itr = sessionIDMap.find(rcvPacket->getSrcID());

			trace() << "SID from node: " << rcvPacket->getSessionIDPkt();
			trace() << "SID stored for node[" << rcvPacket->getSrcID() <<  "] : " << itr->second << "\n\n";

			// if session ID is expired, deny.
			if (rcvPacket->getSessionIDPkt() <= itr->second) {
				trace() << "outdated SID from regular node detected: " << rcvPacket->getSessionIDPkt();
				break;
			}

			
			// Update session ID table of node
			sessionIDMap.erase(rcvPacket->getSrcID());
			sessionIDMap.insert(pair <int,int> (rcvPacket->getSrcID(), rcvPacket->getSessionIDPkt()));


			for(auto elem : sessionIDMap)
			{
			   trace() << "MAP CONTENTS for node[" << rcvPacket->getSrcID() << "]:" << " " << elem.second << "\n";
			}


			if (associationRequest_hub(rcvPacket)) {
				trace() << "Accepting association request from node [" << rcvPacket->getSrcID() << "] SID = " << getSessionID();
				// update associatedDevices and reply with an ACK
				associatedDevices[rcvPacket->getSrcID()] = true;
				DSMACPacket *ackPacket = new DSMACPacket("Leader node connection response", MAC_LAYER_PACKET);
				ackPacket->setLeaderNodeid(SELF_MAC_ADDRESS);
				ackPacket->setDSMACPacketType(MAC_DSMAC_ACK_PACKET);
				ackPacket->setDstID(rcvPacket->getSrcID());
				ackPacket->setByteLength(ACK_PKT_SIZE);

				toRadioLayer(ackPacket);
				toRadioLayer(createRadioCommand(SET_STATE, TX));
				setTimer(ATTEMPT_TX, TX_TIME(ACK_PKT_SIZE));
			} else {
				trace() << "Denied association request from " << rcvPacket->getSrcID();
				// Need to send a packet to deny the reuqest. 
				// But current implementation always accepts them
			}
			break;
		}

		// received GTS request
		case MAC_DSMAC_GTS_REQUEST_PACKET:{
		
			// only leader nodes can accept GTS requests
			if (!isLeaderNode)
				break;
				
			// if leader node id does not match - do nothing	
			if (rcvPacket->getLeaderNodeid() != SELF_MAC_ADDRESS)
				break;
				
			trace() << "Received GTS request from " << rcvPacket->getSrcID();
			
			// reply with an ACK
			DSMACPacket *ackPacket = new DSMACPacket("Leader node GTS response", MAC_LAYER_PACKET);
			ackPacket->setLeaderNodeid(SELF_MAC_ADDRESS);
			ackPacket->setDSMACPacketType(MAC_DSMAC_ACK_PACKET);
			ackPacket->setDstID(rcvPacket->getSrcID());
			ackPacket->setByteLength(ACK_PKT_SIZE);
			// gtsRequest_hub function handles the decision on the amount of GTS slots to allocate
			ackPacket->setGTSlength(gtsRequest_hub(rcvPacket));

			toRadioLayer(ackPacket);
			toRadioLayer(createRadioCommand(SET_STATE, TX));
			setTimer(ATTEMPT_TX, TX_TIME(ACK_PKT_SIZE));

			break;
		}

		// ack frames are handled by a separate function
		case MAC_DSMAC_ACK_PACKET:{
			if (rcvPacket->getDstID() != SELF_MAC_ADDRESS)
				break;
			handleAckPacket(rcvPacket);
			break;
		}

		// data frame
		case MAC_DSMAC_DATA_PACKET:{
			if (isNotDuplicatePacket(rcvPacket)) {
				dataReceived_hub(rcvPacket);
				toNetworkLayer(decapsulatePacket(rcvPacket));
			} else {
				trace() << "Packet [" << rcvPacket->getName() << "] from node " 
						<< rcvPacket->getSrcID() << " is a duplicate";
			}

			// If the frame was sent to broadcast address, nothing else needs to be done
			if (rcvPacket->getDstID() == BROADCAST_MAC_ADDRESS)
				break;

			// otherwise, generate and send an ACK
			DSMACPacket *ackPacket = new DSMACPacket("Ack packet", MAC_LAYER_PACKET);
			ackPacket->setLeaderNodeid(SELF_MAC_ADDRESS);
			ackPacket->setDSMACPacketType(MAC_DSMAC_ACK_PACKET);
			ackPacket->setDstID(rcvPacket->getSrcID());
			ackPacket->setSeqNum(rcvPacket->getSeqNum());
			ackPacket->setByteLength(ACK_PKT_SIZE);

			toRadioLayer(ackPacket);
			toRadioLayer(createRadioCommand(SET_STATE, TX));
			setTimer(ATTEMPT_TX, TX_TIME(ACK_PKT_SIZE));

			break;
		}

		default:{
			trace() << "WARNING: unknown packet type received [" << rcvPacket->getName() << "]";
		}
	}
}

void DSMAC::handleAckPacket(DSMACPacket * rcvPacket)
{
	if (currentPacket == NULL) {
		trace() << "WARNING received ACK packet while currentPacket == NULL";
		return;
	}
		
	cancelTimer(ACK_TIMEOUT);	

	switch (currentPacket->getDSMACPacketType()) {

		//received an ack while waiting for a response to association request
		case MAC_DSMAC_ASSOCIATE_PACKET: {
			associatedLeaderNode = rcvPacket->getLeaderNodeid();
			if (desyncTimeStart >= 0) {
				desyncTime += getClock() - desyncTimeStart;
				desyncTimeStart = -1;
			}
			trace() << "Associated with leader node:" << associatedLeaderNode;
			setMacState(MAC_STATE_CAP);
			clearCurrentPacket("Success",true);
			connectedToLeader_node();
			break;
		}

		//received an ack while waiting for a response to data packet
		case MAC_DSMAC_DATA_PACKET: {
			if (currentPacket->getSeqNum() == rcvPacket->getSeqNum()) {
				trace() << "Data packet successfully transmitted to " << rcvPacket->getSrcID() 
						<< ", local clock " << getClock();
				clearCurrentPacket("Success",true);
			} else {
				collectPacketHistory("NoAck");
				attemptTransmission("Wrong SeqNum in Ack");
			}
			break;
		}

		case MAC_DSMAC_GTS_REQUEST_PACKET:{
			assignedGTS_node(rcvPacket->getGTSlength());
			clearCurrentPacket("Success",true);
			break;
		}

		default:{
			trace() << "WARNING: received unexpected ACK to packet [" << currentPacket->getName() << "]";
			break;
		}
	}
}

/* Finishes the transmission attempt(s) for current packet
 * Records packet history and performs transmission outcome callback
 */
void DSMAC::clearCurrentPacket(const char * s, bool success) {
	if (currentPacket == NULL) return;
	if (s) collectPacketHistory(s);
	if (success) currentPacketSuccess = true;
	if (currentPacket->getDSMACPacketType() == MAC_DSMAC_DATA_PACKET) {
		if (currentPacket->getDstID() != BROADCAST_MAC_ADDRESS)
			packetBreak[currentPacketHistory]++;
		else
			packetBreak["Broadcast"]++;
	}
	trace() << "Transmission outcome for [" << currentPacket->getName() << "]: " << currentPacketHistory;
	
	// transmissionOutcome callback below might request another transmission by
	// calling transmitPacket(). Therefore, we save and clear the currentPacket 
	// variable and delete it only _after_ the callback.
	DSMACPacket *tmpPacket = currentPacket;
	currentPacket = NULL;
	transmissionOutcome(tmpPacket, currentPacketSuccess, currentPacketHistory);
	cancelAndDelete(tmpPacket);
}

// This function provides a new packet to be transmitted by the MAC
// It may not be transmitted immediately, but MAC will keep it untill 
// one of the following happens:
// 1) All transmission attempts were exausted (if specified)
// 2) Delay limit was exceeded (if specified)
// 3) Packet can not be delivered, e.g. if leader node connection is lost
// 4) transmitPacket called again, replacing the old packet
void DSMAC::transmitPacket(DSMACPacket *pkt, int retries, bool state, double limit) {
	clearCurrentPacket();
	trace() << "transmitPacket([" << pkt->getName() << "]," << retries << "," << state << "," << limit << ")";
	currentPacket = pkt;
	currentPacketGtsOnly = state;
	currentPacketHistory = "";
	currentPacketLimit = limit;
	currentPacketSuccess = false;
	if (currentPacket->getDstID() == BROADCAST_MAC_ADDRESS) {
		currentPacketRetries = 1;
		currentPacketResponse = 0;
	} else {
		currentPacketRetries = retries == 0 ? macMaxFrameRetries : retries;
		currentPacketResponse = ackWaitDuration;
	}
	if (getTimer(ATTEMPT_TX) < 0 && getTimer(ACK_TIMEOUT) < 0)
		attemptTransmission("transmitPacket() called");
}

// This function will initiate a transmission (or retransmission) attempt
void DSMAC::attemptTransmission(const char * descr)
{
	cancelTimer(ATTEMPT_TX);
	if (macState == MAC_STATE_SLEEP || macState == MAC_STATE_SETUP) return;
	trace() << "Attempt transmission, description: " << descr;
	
	// if a packet already queued for transmission - check avaliable retries and delay
	if (currentPacket && (currentPacketRetries == 0 || (currentPacketLimit > 0 && 
				(simTime() - currentPacket->getCreationTime()) > currentPacketLimit))) {
		clearCurrentPacket();
		return;
	}
	
	if (currentPacket) {
		if (macState == MAC_STATE_GTS) {	
			trace() << "Transmitting [" << currentPacket->getName() << "] in GTS";
			transmitCurrentPacket();
		} else if (macState == MAC_STATE_CAP && currentPacketGtsOnly == false) {
			trace() << "Transmitting [" << currentPacket->getName() << "] in CAP, starting CSMA_CA";
			NB = 0;
			CW = enableSlottedCSMA ? 2 : 1;
			BE = batteryLifeExtention ? (macMinBE < 2 ? macMinBE : 2) : macMinBE;
			performCSMACA();	
		} else {
			trace() << "Skipping transmission attempt in CAP due to GTSonly flag";
		}
	} else {
		trace() << "Nothing to transmit";
	}
}

// continue CSMA-CA algorithm
void DSMAC::performCSMACA()
{
	//generate a random delay, multiply it by backoff period length
	int rnd = genk_intrand(1, (1 << BE) - 1) + 1;
	simtime_t CCAtime = rnd * (unitBackoffPeriod * symbolLen);

	//if using slotted CSMA - need to locate backoff period boundary
	if (enableSlottedCSMA) {
		simtime_t backoffBoundary = (ceil((getClock() - currentFrameStart) / (unitBackoffPeriod * symbolLen)) -
		     (getClock() - currentFrameStart) / (unitBackoffPeriod * symbolLen)) * (unitBackoffPeriod * symbolLen);
		CCAtime += backoffBoundary;
	}

	trace() << "CSMA/CA random backoff value: " << rnd << ", in " << CCAtime << " seconds";

	//set a timer to perform carrier sense after calculated time
	setTimer(PERFORM_CCA, CCAtime);
}

/* Transmit a packet by sending it to the radio */
void DSMAC::transmitCurrentPacket()
{
	if (currentPacket == NULL) {
		trace() << "WARNING: transmitCurrentPacket() called while currentPacket == NULL";
		return;
	}
	
	//check if transmission is allowed given current time and tx time
	simtime_t txTime = TX_TIME(currentPacket->getByteLength()) + currentPacketResponse;
	simtime_t txEndTime = getClock() + txTime;
	int allowTx = 1;
	
	if (macState == MAC_STATE_CAP) {	//currently in CAP
		if (currentFrameStart + CAPend < txEndTime && CAPend != GTSstart)
			//transmission will not fit in CAP 
			allowTx = 0;
	} else if (macState == MAC_STATE_GTS) {	//currently in GTS
		if (currentFrameStart + GTSend < txEndTime)
			//transmission will not fit in GTS
			allowTx = 0;
	}
	
	if (allowTx) {
		if (currentPacket->getDSMACPacketType() == MAC_DSMAC_DATA_PACKET) {
			if (macState == MAC_STATE_CAP) collectOutput("pkt TX state breakdown", "Contention");
			else collectOutput("pkt TX state breakdown", "Contention-free");
		}
		//decrement retry counter, set transmission end timer and modify mac and radio states.
		currentPacketRetries--;
		trace() << "Transmitting [" << currentPacket->getName() << "] now, remaining attempts " 
				<< currentPacketRetries;
		setTimer(currentPacketResponse > 0 ? ACK_TIMEOUT : ATTEMPT_TX, txTime);
		toRadioLayer(currentPacket->dup());
		toRadioLayer(createRadioCommand(SET_STATE, TX));
	} else {
		//transmission not allowed
		trace() << "txTime " << txTime << " CAP:" << (currentFrameStart + CAPend - getClock()) << 
				" GTS:" << (currentFrameStart + GTSend - getClock());
		trace() << "Transmission of [" << currentPacket->getName() << "] stopped, not enough time";
	}
}

// String s represents an outcome of most recent transmission attempt for 
// current packet, it is saved (appended) to the final packet history
void DSMAC::collectPacketHistory(const char *s)
{
	if (!currentPacket) {
		trace() << "WARNING: collectPacketState called while currentPacket==NULL, string:"<<s;
		return;
	}
	if (currentPacketHistory.size()) {
		currentPacketHistory.append(",");
		currentPacketHistory.append(s);
	} else {
		currentPacketHistory = s;
	}
}

//--- DECISION LAYER FUNCTIONS ---//

// A function to accept new packet from the Network layer
// ACTION: 	check if packet can be transmitted immediately
//			otherwise accept only if there is room in the buffer
bool DSMAC::acceptNewPacket(DSMACPacket *newPacket) 
{
	if (getAssociatedLeaderNode() != -1 && getCurrentPacket() == NULL) {
		transmitPacket(newPacket);
		return true;
	}
	return bufferPacket(newPacket);
}

// A function to react to beacon reception
// ACTION:  if not associated to a PAN, create and transmit connection request
void DSMAC::receiveBeacon_node(DSMACPacket *beacon) 
{
	if (getAssociatedLeaderNode() == -1) 
		transmitPacket(newConnectionRequest(beacon->getLeaderNodeid()));
}


void DSMAC::setSessionID(int newSessionID) {
	sessionID = newSessionID;
}

void DSMAC::setLastLeaderSessionID(int oldSessionID) {
	lastLeaderSessionID = oldSessionID;
}


// A function to react to packet transmission callback
// ACTION: Simply transmit next packet from the buffer if associated to PAN
void DSMAC::transmissionOutcome(DSMACPacket *pkt, bool success, string history) 
{
	if (getAssociatedLeaderNode() != -1 && TXBuffer.size()) {
		DSMACPacket *packet = check_and_cast<DSMACPacket*>(TXBuffer.front());
		TXBuffer.pop();
		transmitPacket(packet);
	}
}

