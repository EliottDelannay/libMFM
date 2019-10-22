/*
 MFMBoxDiagFrame.cc
 Copyright Acquisition group, GANIL Caen, France
 */

#include <iostream>
#include <cmath>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
using namespace std;

#include "MFMBoxDiagFrame.h"

//_______________________________________________________________________________
MFMBoxDiagFrame::MFMBoxDiagFrame(int unitBlock_size, int dataSource,
		int frameType, int revision, int frameSize, int headerSize) {
	/// Constructor for a Box Diagnostics  frame . the header is filled with unitblock_size, data source , frame type , revision , frame, size and header size value
	SetPointers();
	fCountNbEventCard=NULL;
}

//_______________________________________________________________________________
MFMBoxDiagFrame::MFMBoxDiagFrame() {
	/// Constructor for a empty Box Diagnostic frame
	fCountNbEventCard=NULL;

}
//_______________________________________________________________________________
MFMBoxDiagFrame::~MFMBoxDiagFrame() {
/// destructor of BoxDiag frame

	if (fCountNbEventCard){
		delete [] fCountNbEventCard;
		fCountNbEventCard=NULL;
	}

}
//_______________________________________________________________________________
void MFMBoxDiagFrame::SetBufferSize(int size, bool ifinferior) {
	MFMBlobFrame::SetBufferSize(size, ifinferior);
	MFMBoxDiagFrame::SetPointers();
}
//_______________________________________________________________________________
void MFMBoxDiagFrame::SetPointers(void * pt) {
	MFMBlobFrame::SetPointers(pt);
	pHeader = (MFM_topcommon_header*) pData;
	pData_char = (char*) pData;
}
//_______________________________________________________________________________
void MFMBoxDiagFrame::SetAttributs(void * pt) {
	SetPointers(pt);
	MFMBlobFrame::SetAttributs(pt);
	pUserData_char=(char*)&(((MFM_BoxDiag_frame*) pHeader)->BoxDiagData);
}
//_______________________________________________________________________________

uint64_t MFMBoxDiagFrame::GetTimeStamp() {
	/// Computer, set attibut and return value of time stamp from  frame
	fTimeStamp = 0;
	uint64_t * timeStamp = &(fTimeStamp);
	memcpy(((char*) (&fTimeStamp)),
			((MFM_BoxDiag_frame*) pHeader)->BoxDiagEventInfo.EventTime, 6);
	if (fLocalIsBigEndian != fFrameIsBigEndian)
		SwapInt64((timeStamp), 6);
	return fTimeStamp;
}
//_______________________________________________________________________________
uint64_t MFMBoxDiagFrame::GetTimeStampAttribut() {
	/// Return attibut of time stamp
	return fTimeStamp;
}

//_______________________________________________________________________________

uint32_t MFMBoxDiagFrame::GetEventNumber() {
	/// Computer, set attibut and return value of event number from  frame
	fEventNumber = 0;
	char * eventNumber = (char*) &(fEventNumber);

	fEventNumber = ((MFM_BoxDiag_frame*) pHeader)->BoxDiagEventInfo.EventIdx;
	if (fLocalIsBigEndian != fFrameIsBigEndian)
		SwapInt32((uint32_t *) (eventNumber), 4);
	return fEventNumber;
}

//_______________________________________________________________________________
uint32_t MFMBoxDiagFrame::GetEventNumberAttribut() {
	/// Return attibut of event number
	return fEventNumber;
}
//_______________________________________________________________________________
void MFMBoxDiagFrame::SetTimeStamp(uint64_t timestamp) {
	/// Set value of Time Stamp in frame

	char* pts = (char*) &timestamp;
	timestamp = timestamp & 0x0000ffffffffffff;
	memcpy(((MFM_BoxDiag_frame*) pHeader)->BoxDiagEventInfo.EventTime, pts, 6);
}
//_______________________________________________________________________________
void MFMBoxDiagFrame::SetEventNumber(uint32_t eventnumber) {
	/// Set Event Number of frame
	((MFM_BoxDiag_frame*) pHeader)->BoxDiagEventInfo.EventIdx = eventnumber;
}

//_______________________________________________________________________________
void MFMBoxDiagFrame::SetCristalId(uint16_t cristalId) {
	/// Set 16 bits of CristalIdx
	((MFM_BoxDiag_frame*) pHeader)->BoxDiagData.CristalId = cristalId;
}
//_______________________________________________________________________________
void MFMBoxDiagFrame::SetCristalId(uint16_t tgRequest, uint16_t idBoard) {

				// Methode to fill item in case of MFM_COBO_FRAME_TYPE frame
				uint16_t ui, ui2;
				ui2 = 0;

				//trig request form 0 to 4
				ui2 =tgRequest  & BOX_DIAG_TRIG_REQ_CRYS_ID_MASK;

				//id board from 5 to 15
				ui = idBoard & BOX_DIAG_BOARD_ID_MASK;
				ui = ui << 5;
				ui2 = ui2 | ui;
				SetCristalId(ui2);
}
//_______________________________________________________________________________

uint16_t MFMBoxDiagFrame::GetCristalId() {
	uint16_t cristal = 0;
/// Compute and return the 2 bytes of CristalId()
	cristal = ((MFM_BoxDiag_frame*) pHeader)->BoxDiagData.CristalId;
	if (fLocalIsBigEndian != fFrameIsBigEndian)
		SwapInt16((&cristal));
	return ((cristal));
}

//_______________________________________________________________________________
uint16_t MFMBoxDiagFrame::GetTGCristalId() {
/// Compute and return extracted Trigger Request value of CristalId
	return (GetCristalId() & BOX_DIAG_TRIG_REQ_CRYS_ID_MASK);
}

//_______________________________________________________________________________
uint16_t MFMBoxDiagFrame::GetBoardId() {
/// Compute and return id value of Board
	return ((GetCristalId()>>5) & BOX_DIAG_BOARD_ID_MASK);
}
//_______________________________________________________________________________

void MFMBoxDiagFrame::SetStatus(int i, uint16_t status) {
	if (i < 0 and i > BOX_DIAG_NB_STATUS)
		cout << "MFMBoxDiagFrame::BoxDiagSetStatus Error of status index\n";
	if (i == 0)
		((MFM_BoxDiag_frame*) pHeader)->BoxDiagData.Status1 = status;
	if (i == 1)
		((MFM_BoxDiag_frame*) pHeader)->BoxDiagData.Status2 = status;
}
//_______________________________________________________________________________

uint16_t MFMBoxDiagFrame::GetStatus(int i) {
	/// Set Status (0,1 or 2)
	uint16_t status;
	if (i < 0 and i > BOX_DIAG_NB_STATUS) {
		cout << "MFMBoxDiagFrame::ExGetStatus Error of status index\n";
		return 0;
	} else {
		if (i == 0)
			status = (((MFM_BoxDiag_frame*) pHeader)->BoxDiagData.Status1);
		if (i == 1)
			status = (((MFM_BoxDiag_frame*) pHeader)->BoxDiagData.Status2);
	}
	if (fLocalIsBigEndian != fFrameIsBigEndian)
		SwapInt16(&status);
	return status;
}
//_______________________________________________________________________________

void MFMBoxDiagFrame::SetTimeStop(uint16_t time) {
	/// Set Time
	((MFM_BoxDiag_frame*) pHeader)->BoxDiagData.TimeStop= time;
}
//_______________________________________________________________________________

uint16_t MFMBoxDiagFrame::GetTimeStop() {
	/// Get Time
	uint16_t time;
	time = (((MFM_BoxDiag_frame*) pHeader)->BoxDiagData.TimeStop);
	if (fLocalIsBigEndian != fFrameIsBigEndian)
		SwapInt16(&time);
	return time;
}
//_______________________________________________________________________________

void MFMBoxDiagFrame::SetEnergy(uint16_t energy) {
	/// Set Energy
	((MFM_BoxDiag_frame*) pHeader)->BoxDiagData.Energy = energy;
}

//_______________________________________________________________________________

uint16_t MFMBoxDiagFrame::GetEnergy() {
	/// Get Energy
	uint16_t energy;
	energy = (((MFM_BoxDiag_frame*) pHeader)->BoxDiagData.Energy);
	if (fLocalIsBigEndian != fFrameIsBigEndian)
		SwapInt16(&energy);
	return energy;
}
//_______________________________________________________________________________

void MFMBoxDiagFrame::SetTime(uint16_t time) {
	/// Set Time
	((MFM_BoxDiag_frame*) pHeader)->BoxDiagData.Time= time;
}
//_______________________________________________________________________________

uint16_t MFMBoxDiagFrame::GetTime() {
	/// Get Time
	uint16_t time;
	time = (((MFM_BoxDiag_frame*) pHeader)->BoxDiagData.Time);
	if (fLocalIsBigEndian != fFrameIsBigEndian)
		SwapInt16(&time);
	return time;
}
//_______________________________________________________________________________

void MFMBoxDiagFrame::SetChecksum(uint16_t cksum) {
	/// Set Checksum data
	(((MFM_BoxDiag_frame*) pHeader)->BoxDiagData.Checksum) = cksum;
}
//_______________________________________________________________________________

uint16_t MFMBoxDiagFrame::GetChecksum() {
	/// Compute and return value of Checksum
	uint16_t cksum;
	cksum = (((MFM_BoxDiag_frame*) pHeader)->BoxDiagData.Checksum);
	if (fLocalIsBigEndian != fFrameIsBigEndian)
		SwapInt16(&cksum);
	return cksum;
}

//_______________________________________________________________________________
void MFMBoxDiagFrame::FillEventRandomConst(uint64_t timestamp,
		uint32_t enventnumber) {

	/// Fill all data of frame with random values to do test
	/// And report time stamp and event number
	int value = random();
	uint16_t uivalue = (uint16_t) (4294967296 * value / RAND_MAX);
	SetTimeStop(uivalue+2);
	SetEnergy(uivalue);
	SetTime(uivalue+1);
	SetCristalId(8,112);
	SetEventNumber(enventnumber);
	SetTimeStamp(timestamp);
}
//_______________________________________________________________________________
string MFMBoxDiagFrame::GetHeaderDisplay(char* infotext) {
	stringstream ss("");
	string display("");
	display = ss.str();
	if (infotext==NULL)
	ss << MFMCommonFrame::GetHeaderDisplay((char*)"--BoxDiag Frame--");
	else
	ss << MFMCommonFrame::GetHeaderDisplay(infotext);
	ss << "   BoardId ="<< GetBoardId();
	ss << "   Cristal Id ="<<GetTGCristalId();
	ss << "   EN = " << GetEventNumber();
	ss << "   TS = " << GetTimeStamp();

	display = ss.str();
	return display;
}

//____________________________________________________________________
void MFMBoxDiagFrame::FillStat() {
	MFMCommonFrame::FillStat();
	uint16_t id ;
	id = GetCristalId();
	fCountNbEventCard[id]++;

}
//_______________________________________________________________________________
void MFMBoxDiagFrame::InitStat() {
	MFMCommonFrame::InitStat();
	int i;
	fCountNbEventCard = new long long[65536];
	for ( i = 0;i<65536;i++){
		fCountNbEventCard[i]=0;
	}

}
//____________________________________________________________________
string  MFMBoxDiagFrame::GetStat(string info){

	string display("");
	stringstream ss("");
	ss << MFMCommonFrame::GetStat( info);
    int i, j; int total =0;

	for ( i=0;i<65536;i++ ){

		if (fCountNbEventCard[i]!=0){
			j =i;
			ss << "Card "<< ((j>>5) & BOX_DIAG_BOARD_ID_MASK);
			j =i;
			ss << " Cristal  "<< (j& BOX_DIAG_TRIG_REQ_CRYS_ID_MASK );
			ss << " NbEvents = "<< fCountNbEventCard[i] <<"\n";
			total += fCountNbEventCard[i];
		}
	}
	ss<<"Total MFMBoxDiagFrame       = "<< total<<"\n";
	display = ss.str();
	return display;
}

