/*
 MFMNumExoFrame.cc
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

#include "MFMNumExoFrame.h"

//_______________________________________________________________________________
MFMNumExoFrame::MFMNumExoFrame(int unitBlock_size, int dataSource,
		int frameType, int revision, int frameSize, int headerSize) {
	/// Constructor for a exogam frame . the header is filled with unitblock_size, data source , frame type , revision , frame, size and header size value
	SetPointers();
	fCountNbEventCard = NULL;
}

//_______________________________________________________________________________
MFMNumExoFrame::MFMNumExoFrame() {
	/// Constructor for a empty exogam frame
	fCountNbEventCard = NULL;
}
//_______________________________________________________________________________
MFMNumExoFrame::~MFMNumExoFrame() {
	/// destructor of NumExo frame
	if (fCountNbEventCard) {
		delete[] fCountNbEventCard;
		fCountNbEventCard = NULL;
	}
}

//_______________________________________________________________________________
void MFMNumExoFrame::SetUserDataPointer() {
	pUserData_char = (char*) &(((MFM_numexo_frame*) pHeader)->Data);
}
//_______________________________________________________________________________
void MFMNumExoFrame::SetAttributs(void * pt) {
	SetPointers(pt);
	MFMBlobFrame::SetAttributs(pt);
	SetTimeStampFromFrameData();
	SetEventNumberFromFrameData();
}
//_______________________________________________________________________________

void MFMNumExoFrame::SetTimeStampFromFrameData() {
	/// Computer, set attibut and return value of time stamp from  frame
	fTimeStamp = 0;
	uint64_t * timeStamp = &(fTimeStamp);
	memcpy(((char*) (&fTimeStamp)),
			((MFM_numexo_frame*) pHeader)->EventInfo.EventTime, 6);
	if (fLocalIsBigEndian != fFrameIsBigEndian)
		SwapInt64((timeStamp), 6);
}
//_______________________________________________________________________________

void MFMNumExoFrame::SetEventNumberFromFrameData() {
	/// Computer, set attibut and return value of event number from  frame
	fEventNumber = 0;
	char * eventNumber = (char*) &(fEventNumber);
	//memcpy(&fEventNumber,((char*)((MFM_numexo_frame*)pHeader)->exoEvtInfo.eventIdx),4);
	fEventNumber = ((MFM_numexo_frame*) pHeader)->EventInfo.EventIdx;
	if (fLocalIsBigEndian != fFrameIsBigEndian)
		SwapInt32((uint32_t *) (eventNumber), 4);
}
//_______________________________________________________________________________
void MFMNumExoFrame::SetTimeStamp(uint64_t timestamp) {
	/// Set value of Time Stamp in frame

	char* pts = (char*) &timestamp;
	timestamp = timestamp & 0x0000ffffffffffff;
	memcpy(((MFM_numexo_frame*) pHeader)->EventInfo.EventTime, pts, 6);
}
//_______________________________________________________________________________
void MFMNumExoFrame::SetEventNumber(uint32_t eventnumber) {
	/// Set Event Number of frame
	((MFM_numexo_frame*) pHeader)->EventInfo.EventIdx = eventnumber;
}

//_______________________________________________________________________________
void MFMNumExoFrame::SetCristalId(uint16_t cristalId) {
	/// Set 16 bits of CristalIdx
	((MFM_numexo_frame*) pHeader)->Data.CristalId = cristalId;
}
//_______________________________________________________________________________
void MFMNumExoFrame::SetCristalId(uint16_t tgRequest, uint16_t idBoard) {

	uint16_t ui, ui2;
	ui2 = 0;
	//trig request form 0 to 4
	ui2 = tgRequest & NUMEXO_CRYS_MASK;

	//id board from 5 to 15
	ui = idBoard & NUMEXO_BOARD_ID_MASK;
	ui = ui << 5;
	ui2 = ui2 | ui;
	SetCristalId(ui2);
}
//_______________________________________________________________________________
uint16_t MFMNumExoFrame::GetCristalId() const {
	uint16_t cristal = 0;
	/// Compute and return the 2 bytes of CristalId()
	cristal = ((MFM_numexo_frame*) pHeader)->Data.CristalId;
	if (fLocalIsBigEndian != fFrameIsBigEndian)
		SwapInt16((&cristal));
	return ((cristal));
}

//_______________________________________________________________________________
uint16_t MFMNumExoFrame::GetTGCristalId() const{
	/// Compute and return extracted Trigger Request value of CristalId
	return (GetCristalId() & NUMEXO_CRYS_MASK);
}
//_______________________________________________________________________________
uint16_t MFMNumExoFrame::GetBoardId()const {
	/// Compute and return id value of Board
	return ((GetCristalId() >> 5) & NUMEXO_BOARD_ID_MASK);
}
//_______________________________________________________________________________
void MFMNumExoFrame::SetChecksum(uint16_t checksum){
	/// Set 16 bits of checksum
	((MFM_numexo_frame*) pHeader)->Data.Checksum = checksum;
}
//_______________________________________________________________________________
uint16_t MFMNumExoFrame::ComputeChecksum()const{
	int i = 0;
	uint16_t *pt;
	uint16_t value=0;
	uint32_t sum=0;
	uint16_t check;
	pt=	(uint16_t*) pHeader;
	cout <<hex;
	// we jump time stamps and event number
	for (i =0;i <4 ;i++){
		value=(uint16_t)pt[i];
		if (fLocalIsBigEndian != fFrameIsBigEndian)
			SwapInt16((&value));
		sum+= value ;
	}
	for (i =9;i <(NUMEXO_FRAMESIZE/2) -1;i++){
		value=(uint16_t)pt[i];
		if (fLocalIsBigEndian != fFrameIsBigEndian)
			SwapInt16((&value));
		sum+= value ;
	}
	sum = sum&0x0000FFFF;
	check = (uint16_t) sum;
	return check;
}
//_______________________________________________________________________________
uint16_t MFMNumExoFrame::GetChecksum()const{
	uint16_t check = 0;
	/// Compute and return the 2 bytes of CristalId()
	check = ((MFM_numexo_frame*) pHeader)->Data.Checksum;
	if (fLocalIsBigEndian != fFrameIsBigEndian)
		SwapInt16((&check));
	return ((check));
}
//_______________________________________________________________________________
 bool MFMNumExoFrame::VerifyChecksum()const{
	return(ComputeChecksum() == GetChecksum());
}
//_______________________________________________________________________________
void MFMNumExoFrame::InitStat() {
	MFMCommonFrame::InitStat();
	int i;
	fCountNbEventCard = new long long[65536];
	for (i = 0; i < 65536; i++) {
		fCountNbEventCard[i] = 0;
	}
}
//____________________________________________________________________
void MFMNumExoFrame::FillStat() {
	MFMCommonFrame::FillStat();
	uint16_t id;
	id = GetCristalId();
	fCountNbEventCard[id]++;

}
//____________________________________________________________________
string MFMNumExoFrame::GetStat(string info) {

	string display("");
	stringstream ss("");
	ss << MFMCommonFrame::GetStat(info);
	int i, j;
	int total = 0;

	for (i = 0; i < 65536; i++) {

		if (fCountNbEventCard[i] != 0) {
			j = i;
			ss << "Card " << ((j >> 5) & NUMEXO_BOARD_ID_MASK);
			j = i;
			ss << " Cristal  " << (j & NUMEXO_CRYS_MASK);
			ss << " NbEvents = " << fCountNbEventCard[i] << "\n";
			total += fCountNbEventCard[i];
		}
	}
	ss << "Total "<<info<<"          = " << total << "\n";
	display = ss.str();
	return display;
}
//_______________________________________________________________________________
void MFMNumExoFrame::FillDataWithRamdomValue(uint64_t timestamp,uint32_t enventnumber)  {

	/// Fill all data of frame with random values to do test
	/// And report time stamp and event number
	//SetFrameType (0);
	SetCristalId(8, 112);
	SetEventNumber(enventnumber);
	//((MFM_numexo_frame*) pHeader)->Data.Data2 = 0;
	//((MFM_numexo_frame*) pHeader)->Data.Data3 = 0;
	SetTimeStamp(timestamp);
	SetChecksum(ComputeChecksum());
}
//_______________________________________________________________________________
string MFMNumExoFrame::GetHeaderDisplay(char* infotext) const{
	stringstream ss;
	string display("");
	display = ss.str(); 
	ss << MFMCommonFrame::GetHeaderDisplay(infotext) ;
	ss << "   Cristal Id = " << GetTGCristalId();
	ss << "   Test Checksum = " << VerifyChecksum();
	display = ss.str();
	return display;
}
//________________________________________________________________________________



