#ifndef _MFMVamosTACFrame_
#define _MFMVamosTACFrame_
/*
  MFMVamosTACFrame.h

	Copyright Acquisition group, GANIL Caen, France

*/
#define VAMOSTAC_FRAMESIZE 32
#define VAMOSTAC_HEADERFRAMESIZE 18

#define VAMOSTAC_NB_STATUS 2
#define VAMOSTAC_STD_UNIT_BLOCK_SIZE 4


#include "MFMNumExoFrame.h"
#pragma pack(push, 1) // force alignment
struct MFM_TACvamos_eventInfo {
  unsigned EventIdx  : 32;
  char     EventTime[6];
};


struct MFM_TACvamos_data{
  unsigned CristalId : 16;
  unsigned Status1   : 16;
  unsigned Status2   : 16;
  unsigned NotUsed1  : 16;
  unsigned Time      : 16;
  unsigned NotUsed2  : 16;
  unsigned Checksum  : 16;
};


struct MFM_TACvamos_frame{
	 MFM_common_header       Header;
	 MFM_TACvamos_eventInfo  Info;
	 MFM_TACvamos_data       Data;
};



//____________MFMVamosTACFrame___________________________________________________________

class MFMVamosTACFrame : public MFMNumExoFrame
{
public :
virtual void      SetStatus(int i, uint16_t status);
virtual uint16_t  GetStatus(int i)const;

virtual void      SetTime(uint16_t time);
virtual uint16_t  GetTime()const;
virtual void      SetChecksum(uint16_t cksum);
virtual uint16_t  GetChecksum()const;
virtual void      FillEventRandomConst(uint64_t timestamp=0,uint32_t enventnumber=0);

virtual void 	  InitStat();
virtual void 	  FillStat();
virtual string    GetStat(string info);

};
#pragma pack(pop) // free aligment
#endif
