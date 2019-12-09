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
int MFMBasicFrame::GetItemSize(int type) const{
	/// Return value of Item size without computing it
	/// type is not used in this methode but sometime used in daugther class.
	return fItemSize;

}
//_______________________________________________________________________________
void MFMBasicFrame::SetHeaderBasic(MFM_basic_header* header) {
	/// Set pointer pHeader
	pHeader = (MFM_topcommon_header*)header;
}
//_______________________________________________________________________________
void MFMBasicFrame::SetUserDataPointer(){
	pUserData_char = pData_char+GetDefinedHeaderSize();
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
    if (!pHeader)fError.TreatError(2, 0, "MFMBasicFrame::MFM_make_header : Error of header null");
	MFM_fill_header(unitBlock_size, dataSource, frameType, revision, frameSize,
			headerSize, itemSize, nItems);
	SetPointers();

}
//_______________________________________________________________________________

void MFMBasicFrame::GenerateAFrameExample(uint64_t timestamp,uint32_t eventnumber) {
	/// Generate a example of frame with empty data

	uint32_t unitBlock_size = GetDefinedUnitBlockSize();;
	uint16_t type = GetWantedFrameType();
	uint32_t itemsize =(uint32_t) GetItemSizeFromStructure(type);
	uint32_t revision = 1;
	uint16_t source = 0xff; // standard value when produced by a desktop computer
	uint32_t headersize = GetDefinedHeaderSize();;
	int      nbitem     = GetDefinedNbItems();
	uint32_t framesize  = GetDefinedFrameSize();
	if (framesize == 0) framesize = headersize + itemsize*nbitem;
	/*cout << " GenerateAFrameExample GetWantedFrameType       = "<< GetWantedFrameType()<<"\n";
	cout << " GenerateAFrameExample GetDefinedNbItems        = "<< GetDefinedNbItems() <<"\n";
        cout << " GenerateAFrameExample GetItemSizeFromStructure = "<< GetItemSizeFromStructure(type)<<"\n";
        cout << " GenerateAFrameExample GetDefinedUnitBlockSize  = "<< GetDefinedUnitBlockSize()<<"\n";
        cout << " GenerateAFrameExample GetframeSize             = "<< framesize<<"\n";
        cout << " GenerateAFrameExample GetDefinedFrameSize      = "<< GetDefinedFrameSize()<<"\n";
        cout << " GenerateAFrameExample GetDefinedHeaderSize     = "<< headersize<<"\n";
	// generation of MFM header , in this case, MFM is same for all MFM frames*/
	MFM_make_header(unitBlock_size, source, type, revision, (int) (framesize
			/ unitBlock_size), (headersize / unitBlock_size), itemsize, nbitem);

	FillDataWithRamdomValue(timestamp,eventnumber,nbitem);
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
	uint16_t tmp=((MFM_basic_header*)pHeader)->ext.headerSize;
	if (fLocalIsBigEndian!=fFrameIsBigEndian)
	SwapInt16(&tmp);
	fHeaderSize = (int)tmp*fSizeOfUnitBlock;
}
//_______________________________________________________________________________
