#ifndef _MFMHelloFrame_
#define _MFMHelloFrame_
/*
  MFMHelloFrame.h
	Copyright Acquisition group, GANIL Caen, France
*/
#define HEL_USER_SIZE 21
#define HEL_HEADERFRAMESIZE 18
#define HEL_FRAMESIZE   HEL_HEADERFRAMESIZE+HEL_USER_SIZE// fully random value , just for test
#define HEL_STD_UNIT_BLOCK_SIZE 1
#define MFM_HELLO_FRAME_TYPE_TXT "MFM_HELLO_FRAME_TYPE"

#include "MFMBlobFrame.h"

#pragma pack(push, 1) // force alignment
struct MFM_hel_eventInfo {
  char EventTime[6];
  unsigned EventIdx  : 32;
};

struct MFM_hel_data{
};

struct MFM_hel_header{
	 MFM_common_header  HelBlobcHeader;
	 MFM_hel_eventInfo  HelEventInfo;
	 MFM_hel_data       HelData;
};

//____________MFMHelloFrame___________________________________________________________

class MFMHelloFrame : public MFMBlobFrame
{

public :

MFMHelloFrame();
MFMHelloFrame(int unitBlock_size, int dataSource,
	 		 int frameType, int revision, int frameSize,int headerSize);
virtual ~MFMHelloFrame();
void SetUserDataPointer() ;
void SetAttributs(void * pt =NULL);
void SetTimeStampFromFrameData();
void SetEventNumberFromFrameData();
virtual void SetTimeStamp(uint64_t timestamp);
virtual void SetEventNumber(uint32_t eventnumber);
virtual string GetHeaderDisplay(char* infotext=NULL) const;
void FillDataWithRamdomValue(  uint64_t timestamp, uint32_t enventnumber);
 const char * GetTypeText()const {return MFM_HELLO_FRAME_TYPE_TXT;} 
 int GetDefinedUnitBlockSize()const {return HEL_STD_UNIT_BLOCK_SIZE;};
 int GetDefinedHeaderSize()const {return HEL_HEADERFRAMESIZE;};
 int GetDefinedFrameSize()const {return HEL_FRAMESIZE;};

};
#pragma pack(pop) // free aligment
#endif
