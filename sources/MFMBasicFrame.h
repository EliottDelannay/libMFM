#ifndef _MFMBasicFrame_
#define _MFMBasicFrame_
/*
  MFMBasicFrame.cc

	Copyright Acquisition group, GANIL Caen, France

*/
#include "MFMCommonFrame.h"
#pragma pack(push, 1) // force alignment
#define MFM_BASIC_TYPE_TXT "MFM_BASIC_FRAME" 
#define MFM_BASIC_DEFAULT_HEADERSIZE 16
#define MFM_DEFAULT_NB_ITEMS 20
//____________MFMBasicFrame___________________________________________________________

class MFMBasicFrame : public MFMCommonFrame
{
protected:
MFM_basic_header * pBasicHeader;//< pointer on header

uint32_t           fItemSize; //<Item Size
uint32_t           fNumberItems;//Number of items
mutable void       * pCurrentItem; // current pointer on item
void  SetHeaderSizeFromFrameData();
void  SetItemSizeFromFrameData();
void  SetNbItemsFromFrameData();
public :

MFMBasicFrame();
MFMBasicFrame(int unitBlock_size, int dataSource,
	 		 int frameType, int revision, int frameSize,int headerSize,
		       int itemSize, int nItems);
virtual ~MFMBasicFrame();
int  GetItemSize(int type=0) const;

virtual int  GetNbItems()  const;
 void SetHeaderBasic(MFM_basic_header* header) ;
 void SetHeaderSize(int headersize);
 void SetItemSize(int itemsize);
 void SetNbItem(int nbitem);
virtual void SetAttributs(void * pt=NULL);
 void *GetItem(int i) const ;
 void *GetCurrentItem()const{return pCurrentItem;} ;
virtual string GetHeaderDisplay(char* infotext=NULL) const;
virtual void MFM_make_header(int unitBlock_size, int dataSource,
			    int frameType, int revision, int frameSize,int headerSize,
			       int itemSize, int nItems);
virtual void MFM_fill_header(int unitBlock_size,
				   int dataSource, int frameType, int revision,
				   int frameSize,int headerSize,
			       int itemSize, int nItems);
virtual void FillDataWithRamdomValue(uint64_t timestamp,uint32_t eventnumber,int nbitem ){fError.TreatError(2,0,"MFMBasicFrame:FillDataWithRamdomValue should never be here----------\n");};
virtual void GenerateAFrameExample(uint64_t timestamp,uint32_t eventnumber);

virtual int  GetItemSizeFromStructure(int type=0) const{fError.TreatError(2,0,"MFMBasicFrame:GetItemSizeFromStructure should never be here----------\n");};
virtual const char * GetTypeText()const {return MFM_BASIC_TYPE_TXT;} 
virtual int GetDefinedUnitBlockSize()const {return MFM_UNIT_BLOCK_DEFAULT_SIZE;};
virtual int GetDefinedHeaderSize()const {return MFM_BASIC_DEFAULT_HEADERSIZE;};
virtual int GetDefinedNbItems()const{ return MFM_DEFAULT_NB_ITEMS;};
};
#pragma pack(pop) // free aligment
#endif
