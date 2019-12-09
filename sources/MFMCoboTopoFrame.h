#ifndef _MFMCoboTopoFrame_
#define _MFMCoboTopoFrame_
/*
  MFMCoboTopoFrame.h
	Copyright Acquisition group, GANIL Caen, France
*/

#include "MFMBlobFrame.h"
#include <tinyxml.h>

#pragma pack(push, 1) // force alignment

struct MFM_CoboTopo_Data {
	  unsigned CoboIdx  :  1;
	  unsigned AsadMask :  1;
	  unsigned TpMode   :  1;
	  unsigned Unused   :  1;

};

struct MFM_CoboTopo_frame{
	 MFM_common_header  CoboTopoBlobHeader;
	 MFM_CoboTopo_Data  CoboTopoData;
};

#define MFM_COBOT_FRAME_TYPE_TXT "MFM_COBOT_FRAME_TYPE"
#define COBOT_HEADERSIZE MFM_BLOB_HEADER_SIZE
#define COBOT_STD_UNIT_BLOCK_SIZE 1

//____________MFMCoboTopoFrame___________________________________________________________

class MFMCoboTopoFrame : public MFMBlobFrame
{

TiXmlDocument * MyTinyDoc;

public :

MFMCoboTopoFrame();
MFMCoboTopoFrame(int unitBlock_size, int dataSource,
	 		 int frameType, int revision, int frameSize,int headerSize);
virtual ~MFMCoboTopoFrame();
 void SetAttributs(void * pt =NULL);
 uint16_t CoboGetIdx()const ;
 void CoboSetIdx(uint16_t idx);
 void SetUserDataPointer();
 uint16_t CoboGetAsadMask() const;
 void CoboSetAsadMask(uint16_t asadmask);
 uint16_t CoboGetTpMode () const;
 void CoboSetTpMode (uint16_t mode);
 bool HasTimeStamp()   const { return false; }
 bool HasEventNumber() const { return false; }
const char * GetTypeText()const {return MFM_COBOT_FRAME_TYPE_TXT;} 
int GetDefinedUnitBlockSize()const {return COBOT_STD_UNIT_BLOCK_SIZE;};
int GetDefinedHeaderSize()const {return COBOT_HEADERSIZE;};
 string GetHeaderDisplay(char* infotext) const;
};
#pragma pack(pop) // free aligment
#endif
