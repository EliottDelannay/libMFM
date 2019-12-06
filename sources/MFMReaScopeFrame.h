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
#define REA_SCOPE_FRAMESIZE 8192
#define REA_SCOPE_NB_OF_ITEMS 4080
#define REA_SCOPE_BOARD_ID_MASK 0x07ff
#define REA_SCOPE_CHANNEL_ID_MASK 0x001f
#define REA_SCOPE_NB_CHANNELS 16
#define MFM_REA_SCOPE_FRAME_TYPE_TXT "MFM_REA_SCOPE_FRAME_TYPE"
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
  void SetAttributs(void * pt =NULL);
  void SetUserDataPointer();
  int GetDefinedNbItems()const {return REA_SCOPE_NB_OF_ITEMS;}

  int GetDefinedUnitBlockSize()const {return REA_SCOPE_STD_UNIT_BLOCK_SIZE;};
  int GetDefinedHeaderSize()const {return REA_SCOPE_HEADERSIZE;};
  int GetItemSizeFromStructure(int type=0)const{ return sizeof (MFM_ReaScope_Item);};
  int GetDefinedFrameSize()const{ return  REA_SCOPE_FRAMESIZE;};
  const char * GetTypeText()const {return MFM_REA_SCOPE_FRAME_TYPE_TXT;}
  
  void SetTimeStampFromFrameData();
  void SetEventNumberFromFrameData();
  virtual void SetTimeStamp(uint64_t timestamp);
  virtual void SetEventNumber(uint32_t eventnumber);

   uint16_t GetBoardId()const;
   bool HasBoardId () const{ return true;};

   uint16_t GetChannelId()const ;

   void SetLocationId(uint16_t Id);
   void SetLocationId(uint16_t ChannelId, uint16_t BoardId);
   uint16_t GetLocationId()const;

   uint16_t GetSetupScope()const;
   void SetSetupScope(uint16_t setup);
  
   void ReaScopeGetParametersByItem(MFM_ReaScope_Item *item,uint16_t *value)const;
   void ReaScopeSetParametersByItem(MFM_ReaScope_Item *item,uint16_t value);
   void ReaScopeGetParameters(int i, uint16_t *value)const;
   void ReaScopeSetParameters(int i, uint16_t value);
   uint16_t GetCheckSum()const;
   void SetCheckSum(uint16_t checksum); 
  
  
   void FillDataWithRamdomValue(  uint64_t timestamp, uint32_t enventnumber,int nbitem);
   string GetHeaderDisplay(char* infotext)const ;
   string DumpData(char mode='d', bool nozero=false)const;
   void InitStat() ;
   void FillStat();
   string  GetStat(string info)const;

};
#pragma pack(pop) // free alignment
#endif
