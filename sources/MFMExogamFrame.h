#ifndef _MFMExogamFrame_
#define _MFMExogamFrame_
/*
  MFMExogamFrame.h

	Copyright Acquisition group, GANIL Caen, France
    Author : Luc Legeard
*/
#define EXO_NB_OUTER 4
#define EXO_NB_INNER_M 2
#define EXO_NB_INNER_T 3
#define EXO_NB_STATUS 3
#define EXO_NB_MAX_PARA 17
#define EXO_FRAMESIZE 52
#define EXO_HEADERFRAMESIZE 18
#define EXO_TRIG_REQ_CRYS_ID_MASK 0x001f
#define EXO_BOARD_ID_MASK 0x07ff
#define EXO_NUMBER_CRISTAL_ID 2
#define EXO_STD_UNIT_BLOCK_SIZE 4
//#define MFM_EXO2_FRAME_TYPE     0x10 /// numexo2 card frame
#define MFM_EXO2_FRAME_TYPE_TXT "MFM_EXO2_FRAME_TYPE"
#include "MFMBlobFrame.h"


#pragma pack(push, 1) // force alignment
struct MFM_exo_eventInfo {
  unsigned EventIdx  : 32;
  char EventTime[6];
};


struct MFM_exo_data{
  unsigned CristalId : 16;
  unsigned Status1   : 16;
  unsigned Status2   : 16;
  unsigned Status3   : 16;
  unsigned DeltaT    : 16;
  unsigned Inner6M   : 16;
  unsigned Inner20M  : 16;
  unsigned Outer1    : 16;
  unsigned Outer2    : 16;
  unsigned Outer3    : 16;
  unsigned Outer4    : 16;
  unsigned BGO       : 16;
  unsigned Csi       : 16;
  unsigned InnerT30  : 16;
  unsigned InnerT60  : 16;
  unsigned InnerT90  : 16;
  unsigned Padding   : 16;
};


struct MFM_exo_header{
	 MFM_common_header  ExoBlobcHeader;
	 MFM_exo_eventInfo  ExoEventInfo;
	 MFM_exo_data       ExoData;
};



//____________MFMExogamFrame___________________________________________________________

class MFMExogamFrame : public MFMBlobFrame
{
long long * fCountNbEventCard;


public :

MFMExogamFrame();
MFMExogamFrame(int unitBlock_size, int dataSource,
	 		 int frameType, int revision, int frameSize,int headerSize);
virtual ~MFMExogamFrame();
virtual void SetPointers(void * pt =NULL);

virtual void SetAttributs(void * pt =NULL);
 void SetTimeStampFromFrameData();
 void SetEventNumberFromFrameData();
virtual void SetTimeStamp(uint64_t timestamp);
virtual void SetEventNumber(uint32_t eventnumber);
virtual string GetHeaderDisplay(char* infotext=NULL)const;
virtual const char * GetTypeText()const {return MFM_EXO2_FRAME_TYPE_TXT;} 
// EXO

virtual void      ExoSetCristalId(uint16_t cristalId);
virtual void      ExoSetCristalId(uint16_t tgRequest, uint16_t idBoard);
virtual uint16_t  ExoGetCristalId() const;
virtual uint16_t  ExoGetTGCristalId()const;
virtual uint16_t  ExoGetBoardId()const;
bool HasBoardId () const{ return true;};

virtual void      ExoSetStatus(int i, uint16_t status);
virtual uint16_t  ExoGetStatus(int i)const;
virtual void      ExoSetDetaT(uint16_t detaT);
virtual uint16_t  ExoGetDeltaT()const;
virtual void      ExoSetInnerM(int i, uint16_t innner);
virtual uint16_t  ExoGetInnerM(int i)const;
virtual void      ExoSetOuter(int i, uint16_t outer);
virtual uint16_t  ExoGetOuter(int i)const;
virtual void      ExoSetBGO(uint16_t bgo);
virtual uint16_t  ExoGetBGO()const;
virtual void      ExoSetCsi(uint16_t Csi);
virtual uint16_t  ExoGetCsi()const;
virtual void      ExoSetInnerT(int i, uint16_t inner);
virtual uint16_t  ExoGetInnerT(int i)const;
virtual void      ExoSetPara(int i, uint16_t value);
virtual uint16_t  ExoGetPara(int i)const;
virtual void      ExoSetPadding(uint16_t padding);
virtual uint16_t  ExoGetPadding()const;
virtual void      FillEventRandomConst(uint64_t timestamp=0,uint32_t enventnumber=0);
virtual void      WriteRandomFrame(int lun, int nbframes,int verbose,int dumpsize);
virtual void      ExtractInfoFrame(int verbose,int dumpsize);
virtual void 	  InitStat();
virtual void 	  FillStat();
virtual string    GetStat(string info);


};
#pragma pack(pop) // free aligment
#endif
