#ifndef _MFMVamosICFrame_
#define _MFMVamosICFrame_
/*
  MFMVamosICFrame.h

	Copyright Acquisition group, GANIL Caen, France

*/
#define VAMOSIC_NB_STATUS 2
#define VAMOSIC_STD_UNIT_BLOCK_SIZE 4

#include "MFMBlobFrame.h"
#include "MFMNumExoFrame.h"
#pragma pack(push, 1) // force alignment
struct MFM_ICvamos_eventInfo {
  unsigned EventIdx  : 32;
  char     EventTime[6];
};


struct MFM_ICvamos_data{
  unsigned CristalId : 16;
  unsigned Status1   : 16;
  unsigned Status2   : 16;
  unsigned NotUsed1  : 16;
  unsigned Energy    : 16;
  unsigned NotUsed2  : 16;
  unsigned Checksum  : 16;
};


struct MFM_ICvamos_frame{
	 MFM_common_header      Header;
	 MFM_ICvamos_eventInfo  Info;
	 MFM_ICvamos_data       Data;
};

//____________MFMVamosICFrame___________________________________________________________

class MFMVamosICFrame : public MFMNumExoFrame
{
public :
virtual void      SetStatus(int i, uint16_t status);
virtual uint16_t  GetStatus(int i) const;

virtual void      SetEnergy(uint16_t energy);
virtual uint16_t  GetEnergy()const;
virtual void      SetChecksum(uint16_t cksum);
virtual uint16_t  GetChecksum()const;
virtual void      FillEventRandomConst(uint64_t timestamp=0,uint32_t enventnumber=0);

virtual void 	  InitStat();
virtual void 	  FillStat();
virtual string    GetStat(string info);

};
#pragma pack(pop) // free aligment
#endif
