/**
 MFMBasicFrame.cc
 Copyright Acquisition group, GANIL Caen, France
 **/

#include <iostream>
#include <cmath>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <sstream>
using namespace std;

#include "MFMBasicFrame.h"

//_______________________________________________________________________________
MFMBasicFrame::MFMBasicFrame(int unitBlock_size, int dataSource, int frameType,
		int revision, int frameSize, int headerSize, int itemSize, int nItems) {
/// Constructor of frame with a memory space\n
/// fill header information : unitBlock_size,dataSource,....

	fTimeStamp=0;
    fEventNumber=0;
    fItemSize=0;
    pCurrentItem =NULL;
	MFM_make_header(unitBlock_size, dataSource, frameType, revision, frameSize,
			headerSize, itemSize, nItems);
	SetPointers();
}

//_______________________________________________________________________________
MFMBasicFrame::MFMBasicFrame() {
	/// Constructor of a empty frame object
	fTimeStamp=0;
	fEventNumber=0;
	fItemSize=0;
	pCurrentItem =NULL;
//	cout << "debug constructor of MFMBasicFrame::MFMBasicFrame()\n";
}
//_______________________________________________________________________________
MFMBasicFrame::~MFMBasicFrame() {

}
//_______________________________________________________________________________
void MFMBasicFrame::SetItemSizeFromFrameData() {
	// compute and set fItemSize attibut. Return fItemSize
	uint16_t tmp;
	tmp = ((MFM_basic_header*)pHeader)->ext.itemSize;
	if (fLocalIsBigEndian!=fFrameIsBigEndian)
	SwapInt16(&tmp);
	fItemSize =tmp;
}
//_______________________________________________________________________________
int MFMBasicFrame::GetItemSize() const{
	/// Return value of Item size without computing it
	return fItemSize;

}
//_______________________________________________________________________________
void MFMBasicFrame::SetHeaderBasic(MFM_basic_header* header) {
	/// Set pointer pHeader
	pHeader = (MFM_topcommon_header*)header;
}
//_______________________________________________________________________________
void MFMBasicFrame::SetNbItemsFromFrameData() {
	/// Compute number of items
	unsigned int tmp;
	tmp = (int)(((MFM_basic_header*)pHeader)->ext.nItems);
	if (fLocalIsBigEndian!=fFrameIsBigEndian)
	SwapInt32(&tmp);
	fNumberItems=tmp;
}
//_______________________________________________________________________________
int MFMBasicFrame::GetNbItems()const{
	/// Return value of fNumberItems without computing it
	return fNumberItems;
}
//_______________________________________________________________________________
void MFMBasicFrame::SetHeaderSize(int headersize) {
	/// Set frame header size
	((MFM_basic_header*)pHeader)->ext.headerSize = (uint16_t) headersize;
	fHeaderSize = headersize;

}
//_______________________________________________________________________________
void MFMBasicFrame::SetItemSize(int itemsize) {
	/// Set frame item size
	fItemSize = itemsize;
	((MFM_basic_header*)pHeader)->ext.itemSize = itemsize;
}
//_______________________________________________________________________________
void MFMBasicFrame::SetNbItem(int nbitems) {
	/// Set frame items number
	fNumberItems = nbitems;
	((MFM_basic_header*)pHeader)->ext.nItems = nbitems;
}

//_______________________________________________________________________________
string MFMBasicFrame::GetHeaderDisplay(char* infotext) const {
/// Return a string containing infomation of MFM Header\n
	/// if infotext is not NULL replace the standart "MFM header" title
	stringstream ss;
	ss.str("");
	string display("");
	if (infotext == NULL)
		ss << MFMCommonFrame::GetHeaderDisplay((char*)GetTypeText());
	else
		ss << MFMCommonFrame::GetHeaderDisplay(infotext);
	ss << "  HeaderSize = " << GetHeaderSize();
	ss << "  ItemSize = "   << GetItemSize();
	ss << "  NbItems = "    << GetNbItems();
	display = ss.str();
	return display;
}

//_______________________________________________________________________________
void MFMBasicFrame::MFM_fill_header(int unitBlock_size, int dataSource,
		int frameType, int revision, int frameSize, int headerSize,
		int itemSize, int nItems) {
	/// Fill a Header with a list of parameters:\n
	/// unitBlock_size, dataSource, frameType, revision, frameSize,headerSize,itemSize,nItems
	MFMCommonFrame::MFM_fill_header(unitBlock_size, dataSource, frameType,
			revision, frameSize);
	SetHeaderSize(headerSize);
	fHeaderSize = headerSize*unitBlock_size;
	SetItemSize(itemSize);
	SetNbItem(nItems);
}

//_______________________________________________________________________________
void MFMBasicFrame::MFM_make_header(int unitBlock_size, int dataSource,
		int frameType, int revision, int frameSize, int headerSize,
		int itemSize, int nItems) {
	/// Do memory allocation for frame and\n
	/// fill its Header with a list of parameters:\n
    SetBufferSize(frameSize*unitBlock_size);
    if (!pHeader)cout << " Error of header null\n";;
	MFM_fill_header(unitBlock_size, dataSource, frameType, revision, frameSize,
			headerSize, itemSize, nItems);
	SetPointers();

}

//_______________________________________________________________________________
void * MFMBasicFrame::GetItem(int i) const {
    /// return pointer on i item
	pCurrentItem = (MFMBasicFrame *)(pData_char + (fHeaderSize) + (fItemSize*i));
	return  pCurrentItem;
}
//_______________________________________________________________________________
void MFMBasicFrame::SetAttributs(void * pt){
	/// Initialize a set of attributs (frame size, endianess, type ...) and pointers of frame\n
	/// reading and computing data comming from header of frame\n
	/// if pt==NULL initialization is done with current value of main pointer of frame (pData)\n
	/// else initialization is done with pData = pt
	SetPointers( pt);
	MFMCommonFrame::SetAttributs( pt);
	SetNbItemsFromFrameData();
	SetHeaderSizeFromFrameData();
	SetItemSizeFromFrameData();
}
//_______________________________________________________________________________
void MFMBasicFrame::SetHeaderSizeFromFrameData(){
	/// Compute and set value fHeaderSize
	uint16_t tmp=
			((MFM_basic_header*)pHeader)->ext.headerSize;
	if (fLocalIsBigEndian!=fFrameIsBigEndian)
	SwapInt16(&tmp);
	fHeaderSize = (int)tmp*fSizeOfUnitBlock;
}
//_______________________________________________________________________________
