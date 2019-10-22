#ifndef _MFMBoxDiagFrame_
#define _MFMBoxDiagFrame_
/*
  MFMBoxDiagFrame.h

	Copyright Acquisition group, GANIL Caen, France

*/
#define BOX_DIAG_FRAMESIZE 32
#define BOX_DIAG_HEADERFRAMESIZE 18
#define BOX_DIAG_TRIG_REQ_CRYS_ID_MASK 0x001f
#define BOX_DIAG_BOARD_ID_MASK 0x07ff

#define BOX_DIAG_NB_STATUS 2
#define BOX_DIAG_STD_UNIT_BLOCK_SIZE 4
#define BOX_DIAG_NB_CHANNELS 16

#include "MFMBlobFrame.h"

#pragma pack(push, 1) // force alignment
struct MFM_BoxDiag_eventInfo {
  unsigned EventIdx  : 32;
  char     EventTime[6];
};


struct MFM_BoxDiag_data{
  unsigned CristalId : 16;
  unsigned Status1   : 16;
  unsigned Status2   : 16;
  unsigned TimeStop  : 16;
  unsigned Energy    : 16;
  unsigned Time      : 16;
  unsigned Checksum  : 16;
};


struct MFM_BoxDiag_frame{
	 MFM_common_header      BoxDiagBlobHeader;
	 MFM_BoxDiag_eventInfo  BoxDiagEventInfo;
	 MFM_BoxDiag_data       BoxDiagData;
};



//____________MFMBoxDiagFrame___________________________________________________________

class MFMBoxDiagFrame : public MFMBlobFrame
{

private:

long long * fCountNbEventCard;

uint64_t fTimeStamp;
int fEventNumber ;

public :

MFMBoxDiagFrame();
MFMBoxDiagFrame(int unitBlock_size, int dataSource,
	 		 int frameType, int revision, int frameSize,int headerSize);
virtual ~MFMBoxDiagFrame();
virtual void SetPointers(void * pt =NULL);


virtual void SetBufferSize(int size, bool ifinferior) ;
virtual void SetAttributs(void * pt =NULL);
virtual uint64_t GetTimeStamp();
virtual uint64_t GetTimeStampAttribut();
virtual uint32_t GetEventNumberAttribut();
virtual uint32_t GetEventNumber();

virtual void SetTimeStamp(uint64_t timestamp);
virtual void SetEventNumber(uint32_t eventnumber);
virtual string GetHeaderDisplay(char* infotext=NULL);

void     SetCristalId(uint16_t cristalId) ;
void     SetCristalId(uint16_t tgRequest, uint16_t idBoard);
uint16_t GetCristalId();
uint16_t GetTGCristalId();
uint16_t GetBoardId();

virtual void      SetStatus(int i, uint16_t status);
virtual uint16_t  GetStatus(int i);
virtual void      SetTimeStop(uint16_t energy);
virtual uint16_t  GetTimeStop();
virtual void      SetEnergy(uint16_t energy);
virtual uint16_t  GetEnergy();
virtual void      SetTime(uint16_t energy);
virtual uint16_t  GetTime();
virtual void      SetChecksum(uint16_t cksum);
virtual uint16_t  GetChecksum();
virtual void      FillEventRandomConst(uint64_t timestamp=0,uint32_t enventnumber=0);

virtual void 	  InitStat();
virtual void 	  FillStat();
virtual string    GetStat(string info);

};
#pragma pack(pop) // free aligment
#endif
