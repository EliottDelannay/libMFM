#ifndef _MFMReaScopeFrame_
#define _MFMReaScopeFrame_
/*
  MFMReaScopeFrame.h

  Copyright Acquisition group, GANIL Caen, France

*/

#include "MFMBasicFrame.h"
#include<map>
#include <vector>
#define REA_SCOPE_HEADERSIZE 30
#define REA_SCOPE_STD_UNIT_BLOCK_SIZE 1
#define REA_SCOPE_FRAMESIZE 512
#define REA_SCOPE_NB_OF_ITEMS 4080
#define REA_SCOPE_BOARD_ID_MASK 0x07ff
#define REA_SCOPE_CHANNEL_ID_MASK 0x001f
#define REA_SCOPE_NB_CHANNELS 16
#pragma pack(push, 1) // force alignment


struct MFM_ReaScope_EventInfo {
  unsigned LocationId   : 16;
  unsigned EventIdx     : 32;
  char     EventTime[6] ;
  unsigned SetupScope   : 16;
};

// ReaScope
struct MFM_ReaScope_Item {
  unsigned Value :  16;
};
struct MFM_ReaScopeCheckSum {
  unsigned CheckSum :  16;
};

struct MFM_ReaScope_Header{
  MFM_basic_header              ReaScopeBasicHeader;
  MFM_ReaScope_EventInfo 	ReaScopeEvtInfo;
};
struct MFM_ReaScope_Frame{
  MFM_ReaScope_Header       ReaScopeReaScopeHeader;
  MFM_ReaScope_Item         MFMReaScopeItem[REA_SCOPE_NB_OF_ITEMS];
  MFM_ReaScopeCheckSum 	    CheckSum;
};

//____________MFMReaScopeFrame___________________________________________________________

class MFMReaScopeFrame : public MFMBasicFrame
{

 private:
  MFM_ReaScope_Frame* pReaScopeFrame;
  long long * fCountNbEventCard;
  long long	int fNbofGoodF0F0;
  long long	int fNbofBadF0F0;

  uint32_t fEndFrame;
 public :

  MFMReaScopeFrame();
  MFMReaScopeFrame(int unitBlock_size, int dataSource,
	       int frameType, int revision, int frameSize,int headerSize,
	       int itemSize, int nItems);
  virtual ~MFMReaScopeFrame();
  virtual void SetPointers(void * pt =NULL);

  virtual void SetAttributs(void * pt =NULL);
  void SetTimeStampFromFrameData();
  void SetEventNumberFromFrameData();
  virtual void SetTimeStamp(uint64_t timestamp);
  virtual void SetEventNumber(uint32_t eventnumber);

  virtual uint16_t GetBoardId()const;
  virtual uint16_t GetChannelId()const ;
  bool HasBoardId () const{ return true;};
  virtual void SetLocationId(uint16_t Id);
  virtual void SetLocationId(uint16_t ChannelId, uint16_t BoardId);
  virtual uint16_t GetLocationId()const;

  virtual uint16_t GetSetupScope()const;
  virtual void SetSetupScope(uint16_t setup);
  
  virtual void ReaScopeGetParametersByItem(MFM_ReaScope_Item *item,uint16_t *value)const;
  virtual void ReaScopeSetParametersByItem(MFM_ReaScope_Item *item,uint16_t value);
  virtual void ReaScopeGetParameters(int i, uint16_t *value)const;
  virtual void ReaScopeSetParameters(int i, uint16_t value);
  virtual uint16_t GetCheckSum()const;
  virtual void SetCheckSum(uint16_t checksum); 
  
  
  virtual void FillEventWithRamdomConst(uint64_t timestamp=0,uint32_t enventnumber=0);
  virtual void GenerateAReaScopeExample(int type,int32_t eventnumber);
  virtual string GetHeaderDisplay(char* infotext)const ;
  virtual string DumpData(char mode='d', bool nozero=false)const;
  virtual void InitStat() ;
  virtual void FillStat();
  virtual string  GetStat(string info);

};
#pragma pack(pop) // free alignment
#endif
