#ifndef _MFMScalerDataFrame_
#define _MFMScalerDataFrame_

#include <stdlib.h>
using namespace std;
/*
  MFMScalerDataFrame.h

	Copyright Acquisition group, GANIL Caen, France
*/

#include "MFMBasicFrame.h"

#define SCALER_DATA_HEADERSIZE 28;
#define SCALER_STD_UNIT_BLOCK_SIZE 4;
#define	MFM_SCALER_DATA_FRAME_TYPE_TXT	"MFM_SCALER_DATA_FRAME_TYPE"
#pragma pack(push, 1) // force alignment

struct MFM_ScalerData_Info {
  char eventTime[6];
  unsigned eventIdx  : 32;
};

struct MFM_ScalerData_Item {
	  uint32_t Label;
	  uint64_t Count;
	  uint64_t Frequency;
	  int32_t  Status;
	  uint64_t Tics;
	  int32_t  AcqStatus;
	};


struct MFM_ScalerData_header{
	 MFM_basic_header ScalerDataBasicheader;
	 MFM_ScalerData_Info ScalerData_Info;
	 uint16_t Unused;
     };


//____________MFMScalerDataFrame___________________________________________________________

class MFMScalerDataFrame : public MFMBasicFrame
{

public :

MFMScalerDataFrame();
MFMScalerDataFrame(int unitBlock_size, int dataSource,
	 		 int frameType, int revision, int frameSize,int headerSize,
		       int itemSize, int nItems);
virtual ~MFMScalerDataFrame();

virtual void SetPointers(void * pt =NULL);
virtual void SetAttributs(void * pt =NULL);
uint32_t  GetEventNumber() const;
uint32_t  GetEventFrom() const;
virtual uint64_t GetTimeStamp();
void SetTimeStampFromFrameData();
void SetEventNumberFromFrameData();
virtual void SetTimeStamp(uint64_t timestamp);
virtual void SetEventNumber(uint32_t eventnumber);
virtual void GetValuesByItem(MFM_ScalerData_Item *item,uint32_t * label,
		uint64_t *count, uint64_t *frequency,int32_t * status,uint64_t * tics, int32_t* acqstatus);
virtual void SetValuesByItem(MFM_ScalerData_Item *item,uint32_t  label,
		uint64_t count, uint64_t frequency,int32_t status,uint64_t tics, int32_t acqstatus);
virtual void GetValues(int i,uint32_t * label, uint64_t *count, uint64_t *frequency,
		int32_t * status,uint64_t * tics, int32_t* acqstatus) ;
virtual void SetValues(int i,uint32_t  label, uint64_t count, uint64_t frequency,
		int32_t status, uint64_t tics, int32_t acqstatus);
virtual void FillScalerWithRamdomConst2(int nbitem, uint64_t timestamp=0,uint32_t enventnumber=0);
virtual void FillScalerWithRamdomConst(uint64_t timestamp,uint32_t enventnumber);
virtual void FillScalerWithVector(uint64_t timestamp,uint32_t EventCounter,int64_t * fVector, int sizeofvector);
virtual void GenerateAScalerExample(uint64_t timestamp,uint32_t eventnumber,int nbChannels);
virtual string GetHeaderDisplay(char* infotext)const ;
void WriteRandomFrame(int lun,int  nbframe,int verbose,int dumsize);
const char * GetTypeText()const {return MFM_SCALER_DATA_FRAME_TYPE_TXT;}
void ExtracInfoFrame(int verbose,int dumpsize);
virtual string GetDumpData(char mode='d', bool nozero=false);
virtual void DumpData(char mode='d', bool nozero=false);
virtual string GetDumpTextData() ;
virtual void DumpTextData() ;
};
#pragma pack(pop) // free alignment
#endif
