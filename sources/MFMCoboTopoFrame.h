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

struct MFM_CoboTopo_header{
	 MFM_common_header  CoboTopoBlobHeader;
	 MFM_CoboTopo_Data  CoboTopoData;
};

#define MFM_COBOT_FRAME_TYPE_TXT "MFM_EXO2_FRAME_TYPE"

//____________MFMCoboTopoFrame___________________________________________________________

class MFMCoboTopoFrame : public MFMBlobFrame
{

TiXmlDocument * MyTinyDoc;

public :

MFMCoboTopoFrame();
MFMCoboTopoFrame(int unitBlock_size, int dataSource,
	 		 int frameType, int revision, int frameSize,int headerSize);
virtual ~MFMCoboTopoFrame();
virtual void SetPointers(void * pt =NULL);
virtual void SetAttributs(void * pt =NULL);
virtual uint16_t CoboGetIdx()const ;
virtual void CoboSetIdx(uint16_t idx);
virtual uint16_t CoboGetAsadMask() const;
virtual void CoboSetAsadMask(uint16_t asadmask);
virtual uint16_t CoboGetTpMode () const;
virtual void CoboSetTpMode (uint16_t mode);
virtual bool HasTimeStamp()   const { return false; }
virtual bool HasEventNumber() const { return false; }
 const char * GetTypeText()const {return MFM_COBOT_FRAME_TYPE_TXT;} 
 string GetHeaderDisplay(char* infotext) const;
 void ExtracInfoFrame(int verbose,int dumpsize);
};
#pragma pack(pop) // free aligment
#endif
