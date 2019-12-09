#ifndef _MFMMergeFrame_
#define _MFMMergeFrame_
/*
  MFMMergeFrame.h

	Copyright Acquisition group, GANIL Caen, France

*/

#include "MFMBasicFrame.h"

#define MERGE_EN_HEADERSIZE 20
#define MERGE_TS_HEADERSIZE 24

#pragma pack(push, 1) // force alignment
#define MERGE_STD_UNIT_BLOCK_SIZE 2 
#define MFM_MERGE_EN_FRAME_TYPE_TXT "MFM_MERGE_EN_FRAME_TYPE"
#define MFM_MERGE_TS_FRAME_TYPE_TXT "MFM_MERGE_TS_FRAME_TYPE" 

struct MFM_Merge_ENeventInfo {
  unsigned eventIdx  : 32;
};
struct MFM_Merge_TSeventInfo {
	  char     eventTime[6];
	  uint32_t deltaTime;
	};
// Merge

struct MFM_Merge_ENheader{
	 MFM_basic_header      MergeBasicHeader;
	 MFM_Merge_ENeventInfo MergeEvtInfo;
};
struct MFM_Merge_TSheader{
	 MFM_basic_header      MergeBasicHeader;
	 MFM_Merge_TSeventInfo MergeEvtInfo;
};



//____________MFMMergeFrame___________________________________________________________

class MFMMergeFrame : public MFMBasicFrame
{

private:

public :

MFMMergeFrame();
MFMMergeFrame(int unitBlock_size, int dataSource,
	 	int frameType, int revision, int frameSize,int headerSize,
		       int itemSize, int nItems);
virtual ~MFMMergeFrame();

virtual void SetAttributs(void * pt =NULL);
void	SetTimeStampFromFrameData();
void	SetEventNumberFromFrameData();

virtual const char * GetTypeText()const {
	if (GetFrameType() == MFM_MERGE_EN_FRAME_TYPE) return  MFM_MERGE_EN_FRAME_TYPE_TXT;
	if (GetFrameType() == MFM_MERGE_TS_FRAME_TYPE) return  MFM_MERGE_TS_FRAME_TYPE_TXT;
	return NULL;
	}

int GetItemSizeFromStructure(int type=0) const{return 0;};
int GetDefinedUnitBlockSize()const {return MERGE_STD_UNIT_BLOCK_SIZE;};
int GetDefinedHeaderSize() const{
	if (GetFrameType() == MFM_MERGE_EN_FRAME_TYPE) return  MERGE_EN_HEADERSIZE;
	if (GetFrameType() == MFM_MERGE_TS_FRAME_TYPE) return  MERGE_TS_HEADERSIZE;
	return 0;
}
int GetDefinedFrameSize() const {return 0;};

virtual uint32_t GetDeltaTime()const;
virtual void SetDeltaTime(uint32_t deltatule);
virtual void SetTimeStamp(uint64_t timestamp);
virtual void SetEventNumber(uint32_t eventnumber);
virtual void AddFrame(MFMCommonFrame* frame);
virtual void ReadInFrame(MFMCommonFrame* frame);
virtual void ResetAdd();
virtual void ResetReadInMem();
bool HasTimeStamp()const ;
bool HasEventNumber() const;

};
#pragma pack(pop) // free alignment
#endif
