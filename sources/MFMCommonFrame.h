///
/// MFM library manage event MFM format
///
/// MFMCommonFrame Class
/// \file MFMCommonFrame.h
/// \author Legeard Luc
/// \version 1.0
/// \date 2014 01 01
///
/// this  file
///
/*! \mainpage  MFM library
 *
 *
 * \Introduction
 *
 * This MFM package  manage The MultiFrame Metaformat \n
 * allowing read and write MFM frame
 *
 * \section install_sec Installation
 *
 * make clean \n
 * make
 *
 * \section usage_sec Usage
 *
 * A MFMtest.exe executable is generated and contains examples to generate different MFM frames \n
 * The execution of "MFMtest.exe -h " gives a help How to use MFMtest.exe \n
 * Have a look to main file MFMtest.cc to see how to use MFM library and its different classes \n
 *
 */


// META-TYPE  Byte
// [7-Endian][6-Blobness][5][4][3-2-1-0-BlockSize]

#ifndef _MFM_FRAME_COMMON_
#define _MFM_FRAME_COMMON_

#include <stdint.h>
#include "MFMTypes.h"
#include "MError.h"
#include "stdlib.h"
#pragma pack(push, 1)// allow strict alignment
#define MFM_ENDIANNESS_MSK 0x80
#define MFM_BLOBNESS_MSK 0x40
#define MFM_UNIT_BLOCK_SIZE_MSK 0x0F
#define MFM_UNIT_BLOCK_DEFAULT_SIZE 1
#define MFM_BLOCK_SIZE 0x0C
#define MFM_BIG_ENDIAN 0
#define MFM_LITTLE_ENDIAN 0x80
#define MFM_FRAME_SIZE_MASK 0x00FFFFFF
#define MFM_BLOB_HEADER_SIZE 8
#define set_user_data_pointer(A,B) pUserData_char = (char*) &(((A*)pHeader)->B)
#define MFM_COMMON_TYPE_TXT "MFM_UNKNONW_COMMON_FRAME" 

enum MFM_ERR { MFM_ERR_UNIT_BLOCK_SIZE = -100, MFM_ERR_FRAME_SIZE };


struct MFM_common_header {
  unsigned metaType : 8;
  unsigned frameSize : 24;
  unsigned dataSource : 8;
  unsigned frameType : 16;
  unsigned revision : 8;
};

struct MFM_ext_layered_header {
  unsigned headerSize : 16;
  unsigned itemSize : 16;
  unsigned nItems : 32;
};

struct MFM_ext_basic_header {
  unsigned headerSize : 16;
  unsigned itemSize : 16;
  unsigned nItems : 32;
};

struct MFM_ext_blob_header {
};
struct MFM_ext_common_header {
};

struct MFM_layered_header {
  MFM_common_header hd;
  MFM_ext_layered_header ext;
};

struct MFM_topcommon_header {
  MFM_common_header hd;
  MFM_ext_common_header ext;
};
struct MFM_basic_header {
  MFM_common_header hd;
  MFM_ext_basic_header ext;
};

struct MFM_blob_header {
  MFM_common_header hd;
  MFM_ext_layered_header ext;
};

//____________MFMCommonFrame___________________________________________________________

class MFMCommonFrame
{
protected:

uint32_t fHeaderSize;   		///< Header size in Bytes
void *   pReserveHeader; 		///< Pointer on frame reserve header
char *   pDataNew;  			///< Pointer of beginning frame  if local allocation.
char *   pData_char; 			///< Pointer of beginning frame
char *   pUserData_char; 		///< Pointer of User Data ( after header and timestamp,enventnumber..)
int      fSizeOfUnitBlock; 		///< Size of Unit block in Bytes
bool     fLocalIsBigEndian; 		///< Endianness or running computer  
bool     fFrameIsBigEndian ; 		///< Endianness or Frame
uint16_t fFrameType; 			///< Frame type
int      fWantedFrameType; 		///< Wanted Frame type in case of generation of frame ( simulation ). No usage in read frame
int      fFrameSize; 			///< Frame size
uint64_t fTimeStamp;			//<Time Stamp
uint32_t fEventNumber;			//<EventNumber
private :
mutable int    fIncrement;    			///< memory of incrementation in case of dump.
int      fBufferSize;   			///< available buffer size, it can be higher than frame size.
uint64_t fTimeDiff;  		///< memorise time reference in microsecond
protected :
char *   pCurrentPointerForAdd; ///< Pointer  to keep memory where we can add data or other frames in current frame
char *   pCurrentPointerForRead; ///<Pointer  to keep memory where we can read data or other frames in current frame
long long      fCountFrame        ; /// counter to do statistic ( in case of same frame object is reused to read a file for example)
double fMeanFrameSize ; /// statistic : MeanFrameSize
unsigned long long fNegatifJump   ; /// counter negativ jump in event number
unsigned long long fNoContiJump   ; //  counter of non contigus +1 jump in event number

MError   fError;
//public :
void * pData;     				///< Pointer of beginning frame
MFM_topcommon_header * pHeader; ///<  Pointer on frame header

public :
 static string indentation;

MFMCommonFrame();
MFMCommonFrame(int unitBlock_size, int dataSource,
	 		 int frameType, int revision, int frameSize);
 virtual ~MFMCommonFrame();
 virtual void Init();
 virtual string WhichFrame(uint16_t type =0) const;
 void SetMetaType(int unitBlock_size,bool isablob=false);
 void SetUnitBlockSize(int size);
 void SetDataSource(uint8_t datasource);
 void SetRevision(uint8_t  revision);
 void SetFrameSize(uint32_t  size); 
 void SetFrameType(uint16_t frametype);
 void SetHeader(MFM_topcommon_header* header);
 void SetBufferSize(int size,bool ifinferior=true);
 void SetPointers(void * pt =NULL);
 virtual void SetAttributs(void * pt =NULL);
 virtual void SetAttributsOn4Bytes(void * pt =NULL);
 virtual unsigned char Endianness(void)const;
 bool isBigEndian()const;
 bool is_power_2(int i)const;
 int GetBlobNess()const;
 uint8_t GetMetaType()const;
 
 
 virtual const char * GetTypeText()const    {return MFM_COMMON_TYPE_TXT;} 
 virtual int GetDefinedUnitBlockSize()const {return MFM_UNIT_BLOCK_DEFAULT_SIZE;};
 virtual int GetDefinedHeaderSize()const    {return MFM_BLOB_HEADER_SIZE;};
 virtual int GetDefinedFrameSize()const     {return MFM_BLOB_HEADER_SIZE;};


 unsigned char GetFrameEndianness(void)const;
 uint16_t  GetDataSource()const; 
 uint16_t  GetFrameType()const;
 uint8_t  GetRevision()const;
 int  TestType(char * pt=NULL);
 int  GetHeaderSize()const;
 int  GetBufferSize()const;
 int  GetFrameSize()const;
 void SetFrameTypeFromFrameData();
 void SetFrameSizeFromFrameData();
 void SetTimeStampFromFrameData(){};
 void SetEventNumberFromFrameData(){};
 virtual void  SetUnitBlockSizeFromFrameData();
 virtual void  SetHeaderSizeFromFrameData(){};
 virtual void SetUserDataPointer();
 virtual void *GetPointHeader()const;
 virtual char* GetPointUserData()const;
 virtual uint32_t GetEventNumber()const;
 virtual uint64_t GetTimeStamp()const;
 virtual void SetTimeStamp(uint64_t t) { fTimeStamp=t; }
 virtual void SetEventNumber(uint32_t e) { fEventNumber=e; }
 virtual void DumpRaw(int dumpsize=0, int increment=256) const;
 virtual string GetDumpRaw(int dumpsize=0, int increment=256)const;
 virtual void   GetDumpRaw(void *point,int dumpsize, int increment,string * mydump = NULL)const;
 void  DumpData(char mode='d', bool nozero=false) const {cout << GetDumpData( mode,  nozero);}; 
 virtual string GetDumpData(char mode='d', bool nozero=false) const {string test ="";return test;}; ///  Dump decoded data in if frame class avec this method
 virtual string GetHeaderDisplay(char* infotext=NULL)const;
 void   HeaderDisplay(char* infotext=NULL) const ;
 virtual void   MFM_make_header(int unitBlock_size, int dataSource,
			    int frameType, int revision, int frameSize,bool blob=false);
 virtual void   MFM_fill_header( int unitBlock_size,
				   int dataSource, int frameType, int revision,
				   int frameSize,bool blob=false);
virtual void FillDataWithRamdomValue(uint64_t timestamp,uint32_t eventnumber,int  nbitem){fError.TreatError(2,0,"MFMCommonFrame:FillDataWithRamdomValue should never be here----------\n");};
virtual void FillDataWithRamdomValue(uint64_t timestamp,uint32_t eventnumber){fError.TreatError(2,0,"MFMCommonFrame:FillDataWithRamdomValue should never be here----------\n");}


virtual void GenerateAFrameExample(uint64_t timestamp,uint32_t eventnumber);
virtual void WriteRandomFrame(int lun,int  nbframe,int verbose,int dumsize,int type);


 int  ReadInFile(int *fLun,char** vector, int * vectosize);
 int  FillBigBufferFromFile(int fLun,char* vector, unsigned int  vectosize,unsigned int *readsize,unsigned int *eventcount);
 int  ReadInMem(char** vector);
virtual void ExtractInfoFrame(int verbose,int dumpsize);
virtual void ReadAttributsExtractFrame(int verbose,int dumpsize,void * pt=NULL);
 uint64_t GetTimeStampUs(uint64_t diff= 0)const;
 uint64_t SetTimeDiffUs();
 uint64_t GenerateATimeStamp()const;
 bool IsAEbyedat(int type =0)const;
 bool IsACobo(int type =0)const;
 bool IsAMutant(int type =0)const;
 bool IsAScaler(int type=0)const;
 bool IsABlobType(int type =0)const;
virtual void FillStat();
virtual void InitStat();
 void IncrementNegativJump();
 void IncrementNoContiJump();
virtual string GetStat(string info = "UnknowFrame" ) const;
virtual void PrintStat(string ="UnknowFrame" )const;
virtual bool HasTimeStamp() const {return true; }
virtual bool HasEventNumber() const {return true; } 
virtual bool HasBoardId() const {return false; }
virtual uint16_t GetBoardId() const { return 0; }
 void SetWantedFrameType(uint32_t type){ fWantedFrameType = type;}
 int  GetWantedFrameType() const { return fWantedFrameType;}
 int  GetCountFrame() const;
virtual void  TestUserPointer() const;

virtual void Print(int /*verbose*/=0, int /*dumpsize*/=0) const {
        HeaderDisplay();
        DumpRaw();
        cout << endl;
    }


 bool IsType(uint16_t t) const{
  return fFrameType==t;
}

virtual void debug_frame() const;
virtual void FrameSpecificInitialisations(const string&) {}

};
#pragma pack(pop) // free aligment

//____________________________________________________________________
// Utility functions
inline void SwapInt64(uint64_t *Buf, int nbByte =8, int repeat=1) {
    /// Swap a 64 bits(8 Bytes) integer to do endianess conversion \n
    ///		Buf   : pointer on integer to convert\n
    ///		nByte : number of bytes concerned by swaping (default = 8)\n
    ///     repeat: nb of repeat in case of a vector to convert. default =1
    unsigned char *tmp1, *tmp2;
    uint64_t tempo;
    tempo = 0;
    tmp1 = (unsigned char*) Buf;
    tmp2 = (unsigned char*) (&tempo);
    uint64_t *Bufloc = Buf;
    if (nbByte > 8)
        cout << "SwapInt64 with nb of Bytes > 8\n";
    for (int j = 0; j < repeat; j++) {
        Bufloc = Buf + j * nbByte;
        tmp2 = (unsigned char*) (&tempo);
        tmp1 = (unsigned char*) Bufloc;
        for (int i = 0; i < nbByte; i++) {
            tempo = ((tempo) << 8);
            memcpy(tmp2, tmp1 + i, 1);
        }
        *Bufloc = tempo;
    }
}

//____________________________________________________________________
inline void SwapInt16(uint16_t *Buf, int repeat=1) {
    /// Swap a 16 bits(2 Bytes) integer to do endianess conversion
    ///     Buf   : pointer on integer to convert
    ///     repeat: nb of repeat in case of a vector to convert. default =1
    typedef struct mot16 {
        unsigned char Byte1;
        unsigned char Byte2;
    } SW_MOT16;
    SW_MOT16 Temp, *Mot16;

    Mot16 = (SW_MOT16 *) Buf;

    for (int i = 0; i < repeat; i++, Mot16++) {
        Temp.Byte1 = Mot16->Byte2;
        Temp.Byte2 = Mot16->Byte1;
        *Mot16 = Temp;
    }
}
//_______________________________________________________________________________
inline uint64_t GenerateATimeStamp() {
    ///
    ///generate a time stamp with computer clock for simulation
    ///
    uint64_t ts= 0 ;
    clock_t t = clock();
    if (t>=0 )
        ts=(uint64_t)t;
    return ts;
}
//____________________________________________________________________
inline void SwapInt32(uint32_t *Buf, int nbByte=4, int repeat=1) {
    /// Swap a 32 bits(4 Bytes) integer to do endianess conversion\n
    ///     Buf   : pointer on integer to convert\n
    ///     nByte : number of bytes concerned by swaping (default =4)\n
    ///     repeat: nb of repeat in case of a vector to convert. default =1\n
    unsigned char *tmp1, *tmp2;
    uint32_t tempo;
    tempo = 0;
    if (nbByte > 4)
        cout << "SwapInt32 with nb of Bytes > 4\n";

    uint32_t *Bufloc = Buf;
    for (int j = 0; j < repeat; j++) {
        Bufloc = Buf + j * nbByte;
        tmp1 = (unsigned char*) Bufloc;
        tmp2 = (unsigned char*) (&tempo);
        for (int i = 0; i < nbByte; i++) {
            tempo = ((uint32_t) (tempo) << 8);
            memcpy(tmp2, tmp1 + i, 1);
        }
        *Buf = tempo;
    }
}
//____________________________________________________________________
#endif
