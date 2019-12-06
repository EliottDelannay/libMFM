/*
 MFMXmlFileHeaderFrame.cc
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

#include "MFMXmlFileHeaderFrame.h"

//_______________________________________________________________________________
MFMXmlFileHeaderFrame::MFMXmlFileHeaderFrame(int unitBlock_size,
		int dataSource, int frameType, int revision, int frameSize,
		int headerSize) {
	/// Constructor for a exogam frame . the header is filled with unitblock_size, data source , frame type , revision , frame, size and header size value
	SetPointers();
}

//_______________________________________________________________________________
MFMXmlFileHeaderFrame::MFMXmlFileHeaderFrame() {
	/// Constructor for a empty exogam frame

}
//_______________________________________________________________________________
MFMXmlFileHeaderFrame::~MFMXmlFileHeaderFrame() {
	/// destructor of Exogam frame
}
//_______________________________________________________________________________
void MFMXmlFileHeaderFrame::SetPointers(void * pt) {
	MFMBlobFrame::SetPointers(pt);
	pHeader = (MFM_topcommon_header*) pData;
	pData_char = (char*) pData;
}
//_______________________________________________________________________________
void MFMXmlFileHeaderFrame::SetAttributs(void * pt) {
	SetPointers(pt);
	MFMBlobFrame::SetAttributs(pt);
	pUserData_char=pData_char+fHeaderSize;
}
//_______________________________________________________________________________

uint64_t MFMXmlFileHeaderFrame::GetTimeStamp() {
	/// do nothing and return 0;

	return 0;
}
//_______________________________________________________________________________
uint64_t MFMXmlFileHeaderFrame::GetTimeStampAttribut() {
	/// Return attibut of time stamp
	return 0;
}

//_______________________________________________________________________________

uint32_t MFMXmlFileHeaderFrame::GetEventNumber() {
	/// dp nothing and return 0
	return 0;
}

//_______________________________________________________________________________
uint32_t MFMXmlFileHeaderFrame::GetEventNumberAttribut() {
	/// Return attibut of event number
	return 0;
}
//_______________________________________________________________________________
void MFMXmlFileHeaderFrame::SetTimeStamp(uint64_t timestamp) {
	/// Set value of Time Stamp in frame

}
//_______________________________________________________________________________
void MFMXmlFileHeaderFrame::SetEventNumber(uint32_t eventnumber) {
	/// Set Event Number of frame

}
//_______________________________________________________________________________
void MFMXmlFileHeaderFrame::FillDataWithRamdomValue(uint64_t timestamp,uint32_t enventnumber){
	// fill frame with example given in FillExampleOfText()
	string text;
	text =  FillExampleOfText() ;
	int size = text.size();
	cout << " size of xml text"<<size<<"\n";
	SetBufferSize(size + MFM_BLOB_HEADER_SIZE );
	strcpy((char*)(text.data()),(const char*)GetText());
}
//_______________________________________________________________________________
char*  MFMXmlFileHeaderFrame::GetText(){
return ((char*)  &(((MFM_XmlFile_header_frame*) pHeader)->Data));
}
//_______________________________________________________________________________
string MFMXmlFileHeaderFrame::FillExampleOfText() {

	/// Fill text fied wiht a example
	string text = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	text += "<File_Header>";
	text += "<data_format>mfm</data_format>";
	text += "<experiment>";
	text += "<name>test_laser</name>";
	text += "<laboratory>Ganil</laboratory>";
	text += "</experiment>";
	text += "<context>";
	text += "<run_number>0042</run_number>";
	text += "<run_start_time>16-09-14_17h41m54s</run_start_time>";
	text += "<index>1</index>";
	text += "<file_name>run_0042.dat.16-09-14_17h41m54s.1</file_name>";
	text += "<file_creation_time>16-09-14_17h42m56s</file_creation_time>";
	text += "</context>";
	text += "<software>";
	text += "<type>Narval</type>";
	text += "<application>gnarval_mfm_storer</application>";
	text += "<version>v14.10-03</version>";
	text += "</software>";
	text += "<user_comment>comment</user_comment>";
	text += "</File_Header>";
return text;
}

//_______________________________________________________________________________
