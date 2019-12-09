#ifndef _MFMChimeraFrame_
#define _MFMChimeraFrame_
/*
  MFMChimeraFrame.h
	Copyright Acquisition group, GANIL Caen, France
	e.d.f.  2016  (test class for chimera daq) 
*/

#define CHI_HEADERFRAMESIZE 20
#define CHI_FRAMESIZE  20
#define MFM_CHIMERA_DATA_FRAME_TYPE_TXT "MFM_CHIMERA_DATA_FRAME_TYPE_TXT"
#define MFM_CHIMERA_DATA_STD_UNIT_BLOCK_SIZE 1
#include "MFMBlobFrame.h"

#pragma pack(push, 1) // force alignment
struct MFM_CHI_eventInfo {
  char EventTime[6];
  unsigned EventIdx  : 32;
  unsigned reserved : 16; 
};

struct MFM_CHI_data{
};

struct MFM_CHI_frame{
	 MFM_common_header  CHIBlobcHeader;
	 MFM_CHI_eventInfo  CHIEventInfo;
 	 MFM_CHI_data       CHIData;
};

//____________MFMChimeraFrame___________________________________________________________

class MFMChimeraFrame : public MFMBlobFrame
{

public :

MFMChimeraFrame();
MFMChimeraFrame(int unitBlock_size, int dataSource,
	 		 int frameType, int revision, int frameSize,int headerSize);
virtual ~MFMChimeraFrame();

 void SetAttributs(void * pt =NULL);
 void SetTimeStampFromFrameData();
 void SetEventNumberFromFrameData();
 void SetUserDataPointer();
 
 const char * GetTypeText()const {return MFM_CHIMERA_DATA_FRAME_TYPE_TXT;} 
 int GetDefinedUnitBlockSize()const {return MFM_CHIMERA_DATA_STD_UNIT_BLOCK_SIZE;};
 
 void SetTimeStamp(uint64_t timestamp);
 void SetEventNumber(uint32_t eventnumber);

 void    FillDataWithRamdomValue(uint64_t timestamp=0,uint32_t enventnumber=0);

};
#pragma pack(pop) // free aligment
#endif
