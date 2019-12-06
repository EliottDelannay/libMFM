/*
 MFMHelloFrame.cc
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

#include "MFMHelloFrame.h"

//_______________________________________________________________________________
MFMHelloFrame::MFMHelloFrame(int unitBlock_size, int dataSource,
		int frameType, int revision, int frameSize, int headerSize) {
	/// Constructor for a Hello frame . the header is filled with unitblock_size, data source , frame type , revision , frame, size and header size value
	SetPointers();
}

//_______________________________________________________________________________
MFMHelloFrame::MFMHelloFrame() {
	/// Constructor for a empty Hello frame

}
//_______________________________________________________________________________
MFMHelloFrame::~MFMHelloFrame() {
	/// destructor of Hello frame
}
//_______________________________________________________________________________
void MFMHelloFrame::SetAttributs(void * pt) {
	SetPointers(pt);
	MFMBlobFrame::SetAttributs(pt);
	SetTimeStampFromFrameData();
	SetEventNumberFromFrameData();
	}
//_______________________________________________________________________________
void MFMHelloFrame::SetUserDataPointer() {
	pUserData_char = (char*) &(((MFM_hel_header*) pHeader)->HelData);
}
//_______________________________________________________________________________
void  MFMHelloFrame::SetTimeStampFromFrameData() {
	/// Computer, set attibut and return value of time stamp from  frame
	fTimeStamp = 0;
	uint64_t * timeStamp = &(fTimeStamp);
	memcpy(((char*) (&fTimeStamp)),
			((MFM_hel_header*) pHeader)->HelEventInfo.EventTime, 6);
	if (fLocalIsBigEndian != fFrameIsBigEndian)
		SwapInt64((timeStamp), 6);
}
//_______________________________________________________________________________

void  MFMHelloFrame::SetEventNumberFromFrameData() {
	/// Computer, set attibut and return value of event number from  frame
	fEventNumber = 0;
	char * eventNumber = (char*) &(fEventNumber);
	//memcpy(&fEventNumber,((char*)((MFM_hel_header*)pHeader)->helEvtInfo.eventIdx),4);
	fEventNumber = ((MFM_hel_header*) pHeader)->HelEventInfo.EventIdx;
	if (fLocalIsBigEndian != fFrameIsBigEndian)
		SwapInt32((uint32_t *) (eventNumber), 4);
}
//_______________________________________________________________________________
void MFMHelloFrame::SetTimeStamp(uint64_t timestamp) {
	/// Set value of Time Stamp in frame

	char* pts = (char*) &timestamp;
	timestamp = timestamp & 0x0000ffffffffffff;
	memcpy(((MFM_hel_header*) pHeader)->HelEventInfo.EventTime, pts, 6);
}
//_______________________________________________________________________________
void MFMHelloFrame::SetEventNumber(uint32_t eventnumber) {
	/// Set Event Number of frame
	((MFM_hel_header*) pHeader)->HelEventInfo.EventIdx = eventnumber;
}
//_______________________________________________________________________________
void MFMHelloFrame::FillDataWithRamdomValue(  uint64_t timestamp, uint32_t enventnumber){
	SetEventNumber(enventnumber);
	SetTimeStamp(timestamp);
	strcpy (GetPointUserData(),(char*) MFM_HELLO_FRAME_TYPE_TXT); // cpy text of name of 
}
//_______________________________________________________________________________
string MFMHelloFrame::GetHeaderDisplay(char* infotext)const {
	stringstream ss;
	string display("");
	display = ss.str();
	ss << MFMCommonFrame::GetHeaderDisplay(infotext);
	ss << "   EN = " << GetEventNumber();
	ss << "   TS = " << GetTimeStamp();
	display = ss.str();
	return display;
}

//_______________________________________________________________________________
