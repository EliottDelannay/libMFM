#ifndef _MFMReaGenericFrame_
#define _MFMReaGenericFrame_
/*
  MFMReaGenericFrame.h

	Copyright Acquisition group, GANIL Caen, France

*/

#include "MFMNumExoFrame.h"

#define REA_GENERIC_NB_STATUS 2
#define MFM_REA_GENE_TYPE_TXT "MFM_REA_GENE_TYPE"

#pragma pack(push, 1) // force alignment

struct MFM_ReaGeneric_data{
  unsigned CristalId : 16;
  unsigned Status1   : 16;
  unsigned Status2   : 16;
  unsigned Type_Tns  : 16;
  unsigned Energy    : 16;
  unsigned Time      : 16;
  unsigned Checksum  : 16;
};

struct MFM_ReaGeneric_frame{
	 MFM_common_header         Header;
	 MFM_numexo_eventInfo      EventInfo;
	 MFM_ReaGeneric_data       Data;
};
enum ReaTnsType {REA_GENERIC_ENERGY_TYPE =0x00,REA_GENERIC_ENERGY_TIME_TYPE=0x01,REA_GENERIC_TIME_TYPE=0x02,REA_GENERIC_CHARGE_TYPE=0x04};

//____________MFMReaGenericFrame___________________________________________________________

class MFMReaGenericFrame : public MFMNumExoFrame
{

public :

 void      SetStatus(int i, uint16_t status);
 uint16_t  GetStatus(int i)const;
 void      SetTypeTns(enum ReaTnsType type);
 enum ReaTnsType  GetTypeTns()const;
 void      SetEnergy(uint16_t energie);
 uint16_t  GetEnergy()const;
 void      SetTime(uint16_t time);
 uint16_t  GetTime()const;
 void      FillDataWithRamdomValue(uint64_t timestamp=0,uint32_t enventnumber=0);
 const char * GetTypeText()const {return MFM_REA_GENE_TYPE_TXT;} 
};
#pragma pack(pop) // free aligment
#endif
