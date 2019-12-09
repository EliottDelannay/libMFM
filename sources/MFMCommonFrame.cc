/*
 MFMCommonFrame.cc

 Copyright Acquisition group, GANIL Caen, France

 */

#include <iostream>
#include <cmath>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <cstdlib>
#include <sys/time.h>
using namespace std;
#include "MFMCommonFrame.h"
string MFMCommonFrame::indentation = "";
//_______________________________________________________________________________
MFMCommonFrame::MFMCommonFrame(int unitBlock_size, int dataSource,
		int frameType, int revision, int frameSize) {
	/// Constructor of frame with a memory space\n
	/// fill header information : unitBlock_size,dataSource,....
	Init();
	int minsize = MFM_BLOB_HEADER_SIZE;
	if (minsize > frameSize) {
		cout << "Error of frame size (" << frameSize << ") > size of header ("
				<< minsize << ")\n";
		throw(MFM_BLOCK_SIZE);
	}
	SetBufferSize(frameSize);
	MFM_make_header(unitBlock_size, dataSource, frameType, revision, frameSize);
	fFrameSize = frameSize;

}

//_______________________________________________________________________________
MFMCommonFrame::MFMCommonFrame() {
	/// Constructor of a empty frame object
	Init();

}
//_______________________________________________________________________________
MFMCommonFrame::~MFMCommonFrame() {
	/// Destructor
	if (pDataNew) {
		free(pDataNew);
		pDataNew = NULL;
		pData = NULL;
		pData_char = NULL;
		pUserData_char = NULL;
	}
}
//_______________________________________________________________________________

void MFMCommonFrame::Init() {
	///
	/// Initialization of MFMCommonFrame object\n
	///
	fFrameSize   = 0;
	fTimeDiff    = 0;
	SetTimeDiffUs();
	pData        = NULL;
	pDataNew     = NULL;
	pHeader      = NULL;
	fHeaderSize  = MFM_BLOB_HEADER_SIZE;
	pReserveHeader = NULL;
	pData_char   = NULL;
	fIncrement   = 0;
	fBufferSize  = 0;
	fSizeOfUnitBlock = 0;
	fLocalIsBigEndian = (Endianness() == MFM_BIG_ENDIAN);
	fFrameIsBigEndian = false;
	fCountFrame  = 0;
	fTimeStamp   = 0;
	fEventNumber = 0;
	fFrameType   = 0;
	fWantedFrameType =0;
}
//_______________________________________________________________________________
void MFMCommonFrame::SetUserDataPointer()
{
   SetHeaderSizeFromFrameData();
   pUserData_char = pData_char + GetHeaderSize() +6*(int)(HasTimeStamp())+4*(int)(HasEventNumber());
}
//_______________________________________________________________________________
void MFMCommonFrame::SetHeader(MFM_topcommon_header* header) {
	/// Set pointer of on header of frame\n
	pHeader = header;
}
//_______________________________________________________________________________
void MFMCommonFrame::DumpRaw(int dumpsize, int increment) const{
	///
	/// Display dump of frame realised by GetDumpRaw(...\n
	/// dumpsize  : size of dump\n
	/// increment : begin of dump
	///
	cout << (GetDumpRaw(dumpsize, increment)).data();
	return;
}
//_______________________________________________________________________________

string MFMCommonFrame::GetDumpRaw(int dumpsize, int increment) const{
	///
	/// Creat a string of  dump of frame\n
	/// dumpsize  : size of dump if dumpsize =0 , dumpsize = standard = 256\n
	/// increment : begin of dump
	///

	string mydump;
	
	int framesize = GetFrameSize();
	if (dumpsize == 0)
		dumpsize = framesize;

	if ((increment > 0) && (increment > dumpsize))
		fIncrement = increment;

	if (increment < 0) {
		increment = 0;
		fIncrement = 0;
	}

	if ((increment > 0) && (increment <= framesize)) {
		if (dumpsize + fIncrement > framesize)
			dumpsize = framesize - fIncrement;

		if (dumpsize == 0) {
			dumpsize = 256;
			fIncrement = 0;
		}
	}

	GetDumpRaw((void*) pData, dumpsize, fIncrement, &mydump);

	//} else {
	//mydump += "\n\t end of bloc \n";
	//}
	return mydump;
}

//_______________________________________________________________________________

void MFMCommonFrame::GetDumpRaw(void *point, int dumpsize, int increment,
		string * mydump) const{

	///  Creat a string of dump of memory space\n
	///  point : pointer bo dump begin\n
	///  if dumpsize =0 , dumpsize = standard = 256 and fGBbuf_increment raz\n
	///  if mydump==NULL dump is displayed else dump is retuen in  string * mydump

	string *mydumploc;

	if (mydump == NULL) {
		string st;
		mydumploc = &st;
	} else {
		mydumploc = mydump;
	}

	int i, k;
	int nbrcol = 16; // nb de colonnes affich�es

	int asciimin = 32; // range min of a char to be ascii character
	char tempo[128] = "";
	char chartmp = 0;

	int asciimax = 127; // idem but max
	int nbrperline = 16; // nbr of bytes per line
	int nbrline = 0; // nbr lines
	int dumpsize2 = dumpsize;
	int dumpsize3 = dumpsize;
	unsigned char *pChar = NULL;
	unsigned char *pChar2 = NULL;

	nbrline = (int) (dumpsize / nbrperline) + (int) ((dumpsize % nbrperline)
			!= 0);

	pChar = (unsigned char *) ((char*) point + fIncrement);
	pChar2 = pChar;

	if (nbrline < 1)
		nbrline = 1;
	if (nbrcol > dumpsize)
		nbrcol = dumpsize;
	if (increment == dumpsize)
		nbrline = 1;
	for (i = 0; i < nbrline; i++) {
		sprintf(tempo, "\n%5d %s ", increment, ": ");
		*mydumploc += tempo;
		for (k = 0; k < nbrcol; k++) {
			sprintf(tempo, "%02hX ", (unsigned short) ((*(pChar2++))));
			*mydumploc += tempo;
			dumpsize3--;
			if (dumpsize3 == 0)
				break;
		}
		*mydumploc += "  ";

		for (k = 0; k < nbrcol; k++) {
			chartmp = pChar[0];
			//memcpy (&chartmp, pChar ,1);
			if ((chartmp >= asciimin) && (chartmp <= asciimax)) {
				sprintf(tempo, "%c", *pChar);
				*mydumploc += tempo;
			} else
				*mydumploc += ".";
			pChar++;
			dumpsize2--;
			if (dumpsize2 == 0)
				break;
		}
		increment += nbrperline;
	}
	*mydumploc += "\n";

	if (mydump == NULL)
		cout << (*mydumploc).data();
}

//_______________________________________________________________________________

bool MFMCommonFrame::isBigEndian() const{
	/// return attribut containing bigendian information
	return fLocalIsBigEndian;
}

//_______________________________________________________________________________

uint8_t MFMCommonFrame::GetMetaType() const{
	/// return MetaType byte of frame
	return pHeader->hd.metaType;
}
//_______________________________________________________________________________
void MFMCommonFrame::SetUnitBlockSizeFromFrameData() {
	/// Compute and return Unit BlockSize of frame
	int tmp = pHeader->hd.metaType & MFM_UNIT_BLOCK_SIZE_MSK;
	fSizeOfUnitBlock = pow((double) 2, tmp);
	
}

//_______________________________________________________________________________
uint16_t MFMCommonFrame::GetDataSource()const {
	/// Return Data Source information of Frame
	return (uint16_t) pHeader->hd.dataSource;
}

//_______________________________________________________________________________
uint16_t MFMCommonFrame::GetFrameType() const{
	/// Return attribut containing frame type
	return fFrameType;
}
//_______________________________________________________________________________
void  MFMCommonFrame::SetFrameTypeFromFrameData() {
	/// Compute, set attribut and return frame type\n
	/// the indianess conversion is realised if necessary

	uint16_t tmp;
	tmp = pHeader->hd.frameType;
	if (fLocalIsBigEndian != fFrameIsBigEndian)
		SwapInt16(&tmp);
	fFrameType = tmp;
}
//_______________________________________________________________________________
uint8_t MFMCommonFrame::GetRevision()const{
	/// Return revision frame informationif

	return ((uint32_t) pHeader->hd.revision);
}
//_______________________________________________________________________________
int MFMCommonFrame::GetBlobNess() const{
	/// Return BlobNess frame information
	return (uint32_t) ((pHeader->hd.metaType & MFM_BLOBNESS_MSK) >> 6);
}
//_______________________________________________________________________________
void MFMCommonFrame::SetFrameSizeFromFrameData() {
	/// Compute, set attribut and return frame size\n
	/// the indianess conversion is realised if necessary

	uint32_t tmp;
	tmp = pHeader->hd.frameSize;
	if (fLocalIsBigEndian != fFrameIsBigEndian)
		SwapInt32(&tmp, 3);
	if (fSizeOfUnitBlock == 0) SetUnitBlockSizeFromFrameData();
	tmp = tmp * (uint32_t) fSizeOfUnitBlock;
	fFrameSize = tmp;
}
//_______________________________________________________________________________
 
void  SetHeaderSizeFromFrameData(){}
//_______________________________________________________________________________

void MFMCommonFrame::SetMetaType(int unitBlock_size, bool isablob) {
	/// Set MetaType byte of frame\n
	/// realize all coding computation : unitBlock_size , blog and endianness
	unsigned char endianness;
	unsigned char blobness;
	fSizeOfUnitBlock = unitBlock_size;
	double j = log10(unitBlock_size) / log10(2);
	double l = pow((double) 2, (int) j);

	if (fLocalIsBigEndian)
		endianness = MFM_BIG_ENDIAN;
	else
		endianness = MFM_LITTLE_ENDIAN;

	fFrameIsBigEndian = fLocalIsBigEndian;

	if (isablob)
		blobness = MFM_BLOBNESS_MSK;
	else
		blobness = 0;
	if (l != unitBlock_size)
		throw MFM_ERR_UNIT_BLOCK_SIZE; // unitBlock_size is not a power**2

	uint32_t block_size_power_2 = (int) j;

	if (block_size_power_2 > 16)
		throw MFM_ERR_UNIT_BLOCK_SIZE; // unitBlock_size out of range

	pHeader->hd.metaType = (endianness | block_size_power_2 | blobness);

}
//_______________________________________________________________________________
void MFMCommonFrame::SetUnitBlockSize(int bsize) {
	/// Set UnitBlockSize inside metatype byte
	pHeader->hd.metaType = bsize & MFM_UNIT_BLOCK_SIZE_MSK;
}

//_______________________________________________________________________________
void MFMCommonFrame::SetDataSource(uint8_t source) {
	/// Set Data source of frame
	pHeader->hd.dataSource = source;
}

//_______________________________________________________________________________
void MFMCommonFrame::SetFrameType(uint16_t frametype) {
	/// Set type of frame
	fFrameType = frametype;
	if (pHeader!=NULL)
	pHeader->hd.frameType = frametype;
}

//_______________________________________________________________________________
void MFMCommonFrame::SetRevision(uint8_t revision) {
	/// Set revision of frame
	pHeader->hd.revision = revision;
}
//_______________________________________________________________________________
void MFMCommonFrame::SetFrameSize(uint32_t size) {
	/// Set Frame Size of frame
	uint32_t test = size & MFM_FRAME_SIZE_MASK;
	pHeader->hd.frameSize = test;
}

//_______________________________________________________________________________
void MFMCommonFrame::HeaderDisplay(char * infotext) const{
	/// Display  Header information realised by GetHeaderDisplay()\n
	/// if infotext is not NULL replace the standart "MFM header" title
	cout << (GetHeaderDisplay(infotext));
}
//_______________________________________________________________________________
string MFMCommonFrame::GetHeaderDisplay(char* infotext) const{
	/// Return a string containing infomation of MFM Header\n
	/// if infotext is not NULL replace the standart "MFM header" title
	string display("");
	bool blob;
	stringstream ss("");
	

	if ((pHeader->hd.metaType & MFM_BLOBNESS_MSK) == 0)
		blob = false;
	else
		blob = true;
	if (infotext == NULL)
		ss << "Frame Type : " << GetTypeText() << " ";
	else
		ss << MFMCommonFrame::indentation << infotext;
	ss << endl ;
	ss <<MFMCommonFrame::indentation  << "   MetaType = " << dec << (int) GetMetaType() << hex
			<< "(0x" << (int) GetMetaType() << ")" << "  Blob = " << blob
			<< "  unitBlockSize = " << dec << fSizeOfUnitBlock << "  frameSize = "
			<< dec << GetFrameSize() << hex << "(0x" << GetFrameSize() << ")"
			<< "  dataSource = " << dec << GetDataSource() << hex << "(0x"
			<< GetDataSource() << ")" << endl <<
			MFMCommonFrame::indentation<< "   FrameType = " << dec
			<< GetFrameType() << hex << "(0x" << GetFrameType()
			<< ")" << "  revision = " << dec << (int) GetRevision() << hex
			<< "(0x" << (int) GetRevision() << ")" << " pointer = "
			<< (long long*) GetPointHeader() << "\n";
    	if(HasEventNumber()||HasTimeStamp()) ss << MFMCommonFrame::indentation;
    	if(HasEventNumber()) ss << "   EN = " << dec << GetEventNumber();
    	if(HasTimeStamp())   ss << "   TS = " << dec << GetTimeStamp() << " (0x" << hex << GetTimeStamp() << ")";
    	if(HasBoardId())     ss << "   Board =" << dec << GetBoardId() ;
	display = ss.str();
	return display;
}//_______________________________________________________________________________
unsigned char MFMCommonFrame::GetFrameEndianness() const{
	/// Return  BIG_ENDIAN or LITTLE_ENDIAN value of current computer for metaType format

	unsigned char tmp = pHeader->hd.metaType & MFM_ENDIANNESS_MSK;
	return (tmp);

}
//_______________________________________________________________________________
unsigned char MFMCommonFrame::Endianness(void) const{
	/// Return  endianness value of current computer for metaType format
	unsigned char LsbFlag;
	union {
		uint16_t s;
		uint64_t l;
	} b;

	b.l = 0;
	b.s = 1;
	if (b.l == b.s)
		LsbFlag = MFM_LITTLE_ENDIAN; /* LSB (little endian) car short = long  ( case of my x86-64,)*/
	else
		LsbFlag = MFM_BIG_ENDIAN; /* MSB (big endian) car byte inverses */
	return LsbFlag;
}

//_______________________________________________________________________________

bool MFMCommonFrame::is_power_2(int i) const{
	/// Test if i is a power of 2\n

	double j = log10(i) / log10(2);
	int k = (int) j;
	cout << i << " - " << i % 2 << " - " << j << " - " << k;

	double l = pow((double) 2, (int) j);
	if (l != i)
		return false;
	else
		return true;
}

//_______________________________________________________________________________

void MFMCommonFrame::MFM_fill_header(int unitBlock_size, int dataSource,
		int frameType, int revision, int frameSize, bool blob) {
	/// Fill a Header with a list of parameters:\n
	/// unitBlock_size, dataSource, frameType, revision, frameSize, blob\n
	if (pHeader) {
		SetMetaType(unitBlock_size, blob);
		SetFrameSize((uint32_t) frameSize);
		SetDataSource((uint8_t) dataSource);
		SetFrameType((uint16_t) frameType);
		SetRevision((uint8_t) revision);

	} else {
		cout << " Error of header null\n";
	}
}
//_______________________________________________________________________________
void MFMCommonFrame::MFM_make_header(int unitBlock_size, int dataSource,
		int frameType, int revision, int frameSize, bool blob) {
	/// Do memory allocation for frame and\n
	/// fill its Header with a list of parameters:\n
	/// unitBlock_size, dataSource, frameType, revision, frameSize, blob\n
	SetBufferSize(frameSize * unitBlock_size);

	if (!pHeader)
		cout << " Error of header null\n";
	MFM_fill_header(unitBlock_size, dataSource, frameType, revision, frameSize,
			blob);
	SetPointers();
}

//_______________________________________________________________________________
void MFMCommonFrame::SetBufferSize(int size, bool ifinferior) {
	/// Do memory allocation or a reallacation for frame\n
	/// if ifinferior==true the allocaton is forced to size event if the acutal size is bigger\n
	//cout <<"debug "<<size<< "  "<<fBufferSize<<"\n";
	if (size == fBufferSize)
		return;
	if (!ifinferior or (size > fBufferSize)) {
		if (pData == NULL)
			pDataNew = (char*) (realloc((void*) pData, size));
		else
			pDataNew = (char*) (realloc((void*) pData, size));
		for (int i = fBufferSize; i < size; i++)
			((char*) pDataNew)[i] = 0;
		pData = pDataNew;
		SetPointers();
		pReserveHeader = NULL;
		fBufferSize = size;
	}
}
//_______________________________________________________________________________
void MFMCommonFrame::SetPointers(void * pt) {
	/// Initialize pointers of frame\n
	/// if pt==NULL initialization is with current value of main pointer of frame (pData)\n
	/// else initialization is done with pData = pt\n
	/// pData must be the reference;
	if (pt != NULL) {
		pData = pt;
	}
	pHeader = (MFM_topcommon_header*) pData;
	fFrameIsBigEndian = (GetFrameEndianness() == MFM_BIG_ENDIAN);
	pData_char = (char*) pData;
	SetUnitBlockSizeFromFrameData();
	SetUserDataPointer();
	
}
//_______________________________________________________________________________
void MFMCommonFrame::SetAttributs(void * pt) {
	/// Initialize a set of attributs (frame size, endianess, type ...) and pointers of frame\n
	/// reading and computing data comming from header of frame\n
	/// if pt==NULL initialization is done with current value of main pointer of frame (pData)\n
	/// else initialization is done with pData = pt
	fLocalIsBigEndian = (Endianness() == MFM_BIG_ENDIAN);
	SetPointers(pt);
	SetFrameTypeFromFrameData();
   	SetFrameSizeFromFrameData();
   	SetTimeStampFromFrameData();
   	SetEventNumberFromFrameData();
}
//_______________________________________________________________________________
void MFMCommonFrame::SetAttributsOn4Bytes(void * pt) {
	/// Initialize a set of attributs (frame size, endianess, type ...) and pointers of frame\n
	/// reading and computing data comming from header of frame\n
	/// if pt==NULL initialization is done with current value of main pointer of frame (pData)\n
	/// else initialization is done with pData = pt
	// this is done only on the first 4th bytes so type is not read
	fLocalIsBigEndian = (Endianness() == MFM_BIG_ENDIAN);
	SetPointers(pt);
	SetUnitBlockSizeFromFrameData();
	SetFrameSizeFromFrameData();
}
//_______________________________________________________________________________
void MFMCommonFrame::ReadAttributsExtractFrame(int verbose,int dumpsize,void * pt){
	SetAttributs(pt);
	ExtractInfoFrame(verbose,dumpsize);
	FillStat();
	TestUserPointer();
	
}
//_______________________________________________________________________________

void MFMCommonFrame::WriteRandomFrame(int lun,int  nbframe,int verbose,int dumpsize, int  type){
/// write  frames in file with ramdom data
	uint64_t ts;
	int verif;
	uint32_t framesize = 0;
	SetWantedFrameType(type);
	for (int i = 0; i < nbframe; i++) {
		ts = GetTimeStampUs();
		GenerateAFrameExample(ts, i);
		ReadAttributsExtractFrame(verbose, dumpsize);
		framesize=GetFrameSize();
		verif = write(lun, GetPointHeader(), framesize);
		if (verif != framesize)
			fError.TreatError(2, 0, "Error of write");
	}
}
//_______________________________________________________________________________

void MFMCommonFrame::GenerateAFrameExample(uint64_t timestamp,uint32_t eventnumber) {
	/// Generate a example of frame containing random value\n
	/// usable for tests.


	uint32_t unitBlock_size = GetDefinedUnitBlockSize();;
	uint16_t type       = GetWantedFrameType();
	uint32_t framesize  = GetDefinedFrameSize();
	uint32_t revision   = 1;
	uint16_t source     = 0xff; // standard value when produced by a desktop computer
	uint32_t headersize = GetDefinedHeaderSize();
	/*cout <<dec<< "MFMCommonFrame GenerateAFrameExample GetWantedFrameType       = "<< GetWantedFrameType()<<"\n";
        cout << "MFMCommonFrame GenerateAFrameExample GetDefinedUnitBlockSize  = "<< GetDefinedUnitBlockSize()<<"\n";
        cout << "MFMCommonFrame GenerateAFrameExample GetframeSize             = "<< framesize<<"\n";
        cout << "MFMCommonFrame GenerateAFrameExample GetDefinedFrameSize      = "<< GetDefinedFrameSize()<<"\n";
        cout << "MFMCommonFrame GenerateAFrameExample GetDefinedHeaderSize     = "<< headersize<<"\n";
        */
	// generation of MFM header , in this case, MFM is same for all MFM frames
	MFM_make_header(unitBlock_size, source, type, revision, (int) (framesize
			/ unitBlock_size), (headersize / unitBlock_size));
	FillDataWithRamdomValue(timestamp,eventnumber);
}

//_______________________________________________________________________________
/*
void MFMCommonFrame::SwapInt32(uint32_t *Buf, int nbByte, int repeat)const {
	/// Swap a 32 bits(4 Bytes) integer to do endianess conversion\n
	///     Buf   : pointer on integer to convert\n
	///     nByte : number of bytes concerned by swaping (default =4)\n
	///     repeat: nb of repeat in case of a vector to convert. default =1\n
	unsigned char *tmp1, *tmp2;
	uint32_t tempo;
	tempo = 0;
	if (nbByte > 4)
		cout << "MFMCommonFrame::SwapInt32 with nb of Bytes > 4\n";

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
//_______________________________________________________________________________

void MFMCommonFrame::SwapInt64(uint64_t *Buf, int nbByte, int repeat) const{
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
		cout << "MFMCommonFrame::SwapInt64 with nb of Bytes > 8\n";
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

//_______________________________________________________________________________

void MFMCommonFrame::SwapInt16(uint16_t *Buf, int repeat) const{
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
}*/
//_______________________________________________________________________________
int MFMCommonFrame::FillBigBufferFromFile(int fLun, char* vector,
		unsigned int vectorsize,unsigned int *readsize,unsigned int * eventcount) {
	/// Get data from a file, and fill current frame and initialize its attributs and its pointer
	/// if size of actual frame is not enough, and new size is reallocated
	///      fLun   : descriptor of file (given by a previous open)
	///      vector : pointer buffer .
	///      vectorsize : max size
	///      readsize : value of read size
	///      return status : 0 vector full , 1 end of file,   2 error of read  , 3 else error.
	int count = 0;
	int framesize = 0;
	int header4bytes = 4;
	(*readsize) = 0;
    int reste =0,sizetoread=0;
    static int localsizeread =0;
    static int localsizevector =2048;
    static char* localvector = NULL;
    char * vectornew= NULL;

    if (localvector == NULL){
    	localvector = new char[localsizevector];

    }

	while (true) {

		if (localsizeread>0) {
			reste = vectorsize - (*readsize);
			if (localsizeread> reste)
				return 0;
			memcpy((void*)vector,(void*)localvector,localsizeread);
			*readsize += localsizeread;
			localsizeread=0;
			reste = vectorsize - *readsize;
		}

		count = read(fLun, (void*) (localvector), header4bytes);

		if (count <= 0) {
			fError.TreatError(0, 0,
									"End of read file MFMCommonFrame::FillBigBufferFromFile");
			return 1;
		}
		if (count < header4bytes) {
			fError.TreatError(2, 0,
					"Error in read file MFMCommonFrame::FillBigBufferFromFile");
			return 2;
		}

		SetAttributsOn4Bytes(localvector);
		framesize = GetFrameSize();

		if (framesize > 1000000000){
			fError.TreatError(2, framesize, "Crazy Frame size > 1000000000");
			return 3;
		}

		if (localsizevector < framesize) {
			vectornew = (char*) (realloc((void*) (localvector), framesize));
			if (vectornew != NULL) {
				localvector = vectornew;
				localsizevector = framesize;
				SetAttributsOn4Bytes(vector);
			} else {
				fError.TreatError(1, 0,
						"Memory allocation in MFMCommonFrame::FillBigBufferFromFile");
				return 3;
			}
		}

		sizetoread = framesize - header4bytes;
		count = read(fLun, (void*) ((localvector) + header4bytes),sizetoread);
		if (count != sizetoread) {
			fError.TreatError(2, 0,
									"Error in read file MFMCommonFrame::FillBigBufferFromFile");
			return 2;
		}
		localsizeread =framesize;
		(*eventcount)++;
	}
	return 0;
}
//_______________________________________________________________________________
int MFMCommonFrame::ReadInFile(int *fLun, char** vector, int * vectorsize) {
	/// Get data from a file, and fill current frame and initialize its attributs and its pointer
	/// if size of actual frame is not enough, a new size is reallocated
	///      fLun   : descriptor of file (given by a previous open)
	///      vector : pointer on pointer will contain frame . if size isn't big, a new value of pointer
	///      vectorsize of this pointer
	///      return size of read frame.
	int count = 0;
	int framesize = 0;
	char* vectornew = NULL;
	int sizetoread = MFM_BLOB_HEADER_SIZE; // =8	count = read(fLun, (void*) vector, minsizeheader);
	count = read(*fLun, (void*) (*vector), sizetoread);
	if (count <= 0) {
		cout << " end of read file\n";
		return count;
	}
	if (count < sizetoread) {
		cout << " Error in read file\n";
		return count;
	}
	SetAttributs((*vector));
	framesize = GetFrameSize();
	if (framesize > 1000000000)
		fError.TreatError(2, framesize, "Crazy Frame size > 1000000000");
	if (*vectorsize < framesize) {
		vectornew = (char*) (realloc((void*) (*vector), framesize));
		if (vectornew != NULL) {
			(*vector) = vectornew;
			*vectorsize = framesize;
			SetAttributs((*vector));
		} else {
			fError.TreatError(1, 0,
					"Memory allocation in MFMCommonFrame:: ReadInFile");
			return 0;
		}
	}
	sizetoread = framesize - sizetoread;
	count = read(*fLun, (void*) ((*vector) + MFM_BLOB_HEADER_SIZE), sizetoread);
	if (count != sizetoread) {
		cout << " Error in read file\n";
	}
	return framesize;
}

//_______________________________________________________________________________________________________________________
void MFMCommonFrame::ExtractInfoFrame(int verbose,int dumpsize){ 
// extract informations Frames  and print informations
     
	if ((verbose> 1) ) {
		HeaderDisplay();
		if (verbose > 3) {
			int framesize = GetFrameSize();
			int dump = dumpsize;
			if (framesize < dump)
				dump = framesize;
			DumpRaw(dump, 0);
		}
	}
	if (verbose >=9) DumpData();
}
//_______________________________________________________________________________
int MFMCommonFrame::ReadInMem(char **pt) {
	/// Get data from a memory, and fill current frame and initialize its attributs and its pointer
	//  return size of read frame.
	int framesize = 0;
	SetAttributs((*pt));
	framesize = GetFrameSize();
	(*pt) = (*pt) + framesize;
	return (framesize);
}
//_______________________________________________________________________________
uint64_t MFMCommonFrame::GetTimeStampUs(uint64_t diff)const {
	/// give time in useconde from last SetTimeDiffUs() or since diff if diff >=0
	/// if diff =0 => since 1970
	/// GetTimeStampUs() is used  for simulation
	struct timeval tv;
	gettimeofday(&tv, NULL);
	uint64_t tstime;
	tstime = tv.tv_sec * (uint64_t) 1000000 + tv.tv_usec;
	if (diff == 0)
		tstime -= diff;
	else
		tstime -= fTimeDiff;
	return tstime;
}
//_______________________________________________________________________________
uint64_t MFMCommonFrame::GenerateATimeStamp() const{
	///
	///generate a time stamp with computer clock for simulation
	///
	uint64_t ts = 0;
	clock_t t = clock();
	if (t >= 0)
		ts = (uint64_t) t;
	return ts;
}
//_______________________________________________________________________________
uint64_t MFMCommonFrame::SetTimeDiffUs() {
	/// give time in useconde from 1970
	/// SetTimeDiffUs() is used for simulation
	struct timeval tv;
	gettimeofday(&tv, NULL);
	fTimeDiff = (tv.tv_sec * (uint64_t) 1000000 + tv.tv_usec);
	return fTimeDiff;
}
//_______________________________________________________________________________
int MFMCommonFrame::GetHeaderSize()const {
	/// Return header size without computing it.
	return fHeaderSize;
}  
//_______________________________________________________________________________
int MFMCommonFrame::GetBufferSize() const{
	/// Return buffer size without computing it.
	return fBufferSize;
}
//_______________________________________________________________________________
int MFMCommonFrame::GetFrameSize() const {
	/// Return frame size without computing it.
	return fFrameSize;
}
//_______________________________________________________________________________
void *MFMCommonFrame::GetPointHeader() const{
	/// Return pointer of begining of frame.
	return pData;
}
//_______________________________________________________________________________
char* MFMCommonFrame::GetPointUserData() const{
	/// Get pointer after header of frame\n
	return pUserData_char;
}
//_______________________________________________________________________________
uint32_t MFMCommonFrame::GetEventNumber() const {
	/// Method supposed to be overloaded so return 0
	return fEventNumber;
}
//_______________________________________________________________________________
uint64_t MFMCommonFrame::GetTimeStamp() const{
	/// Method supposed to be overloaded so return 0
	return ((uint64_t) fTimeStamp);
}
//_______________________________________________________________________________
string MFMCommonFrame::WhichFrame(uint16_t type) const{

	string tempos = "UNKNOWN_TYPE";
	if (type == 0) {
		type = fFrameType;
	}
	if (type == MFM_COBO_FRAME_TYPE)
		tempos = "MFM_COBO_FRAME_TYPE";
	if (type == MFM_COBOF_FRAME_TYPE)
		tempos = "MFM_COBOF_FRAME_TYPE";
	if (type == MFM_COBOT_FRAME_TYPE)
		tempos = "MFM_COBOT_FRAME_TYPE";
	if (type == MFM_EXO2_FRAME_TYPE)
		tempos = "MFM_EXO2_FRAME_TYPE";
	if (type == MFM_OSCI_FRAME_TYPE)
		tempos = "MFM_OSCI_FRAME_TYPE";
	if (type == MFM_NEDA_FRAME_TYPE)
		tempos = "MFM_NEDA_FRAME_TYPE";
	if (type == MFM_NEDACOMP_FRAME_TYPE)
		tempos = "MFM_NEDACOMP_FRAME_TYPE";
	if (type == MFM_EBY_EN_FRAME_TYPE)
		tempos = "MFM_EBY_EN_FRAME_TYPE";
	if (type == MFM_EBY_TS_FRAME_TYPE)
		tempos = "MFM_EBY_TS_FRAME_TYPE";
	if (type == MFM_EBY_EN_TS_FRAME_TYPE)
		tempos = "MFM_EBY_EN_TS_FRAME_TYPE";
	if (type == MFM_SCALER_DATA_FRAME_TYPE)
		tempos = "MFM_SCALER_DATA_FRAME_TYPE";
	if (type == MFM_MERGE_EN_FRAME_TYPE)
		tempos = "MFM_MERGE_EN_FRAME_TYPE";
	if (type == MFM_MERGE_TS_FRAME_TYPE)
		tempos = "MFM_MERGE_TS_FRAME_TYPE";
	if (type == MFM_XML_DATA_DESCRIPTION_FRAME_TYPE)
		tempos = "MFM_XML_DATA_DESCRIPTION_FRAME_TYPE";
	if (type == MFM_RIBF_DATA_FRAME_TYPE)
		tempos = "MFM_RIBF_DATA_FRAME_TYPE";
	if (type == MFM_MUTANT_FRAME_TYPE)
		tempos = "MFM_MUTANT_FRAME_TYPE";
	if (type == MFM_CHIMERA_DATA_FRAME_TYPE)
		tempos = "MFM_CHIMERA_DATA_FRAME_TYPE";
	if (type == MFM_HELLO_FRAME_TYPE)
		tempos = "MFM_HELLO_FRAME_TYPE";
	if (type == MFM_XML_FILE_HEADER_FRAME_TYPE)
		tempos = "MFM_XML_FILE_HEADER_FRAME_TYPE";
	if (type == MFM_VAMOSIC_FRAME_TYPE)
		tempos = "MFM_VAMOSIC_FRAME_TYPE";
	if (type == MFM_VAMOSPD_FRAME_TYPE)
		tempos = "MFM_VAMOSPD_FRAME_TYPE";
	if (type == MFM_DIAMANT_FRAME_TYPE)
		tempos = "MFM_DIAMANT_FRAME_TYPE";
	if (type == MFM_S3_BAF2_FRAME_TYPE)
		tempos = "MFM_S3BaF2_FRAME_TYPE";
	if (type == MFM_S3_ALPHA_FRAME_TYPE)
		tempos = "MFM_S3_ALPHA_FRAME_TYPE";
	if (type == MFM_S3_RUTH_FRAME_TYPE)
		tempos = "MFM_S3_RUTH_FRAME_TYPE";
	if (type == MFM_S3_EGUN_FRAME_TYPE)
		tempos = "MFM_S3_EGUN_FRAME_TYPE";
	if (type == MFM_S3_SYNC_FRAME_TYPE)
		tempos = "MFM_S3_SYNC_FRAME_TYPE";
	if (type == MFM_REA_GENE_FRAME_TYPE)
		tempos = "MFM_REA_GENE_FRAME_TYPE";
	if (type == MFM_REA_SCOPE_FRAME_TYPE)
		tempos = "MFM_REA_SCOPE_FRAME_TYPE";
	return tempos;
}
//_______________________________________________________________________________
bool MFMCommonFrame::IsABlobType(int type)const {

	bool retour = false;
	int blobness = GetBlobNess();
	if (blobness > 0)
		retour = true;
	return retour;
}
//_______________________________________________________________________________
bool MFMCommonFrame::IsAEbyedat(int type) const{
	if (type == 0)
		type = fFrameType;
	if ((type == MFM_EBY_EN_FRAME_TYPE) || (type == MFM_EBY_TS_FRAME_TYPE)
			|| (type == MFM_EBY_EN_TS_FRAME_TYPE)) {
		return true;
	}
	return false;
}
//_______________________________________________________________________________
bool MFMCommonFrame::IsAScaler(int type) const{
	if (type == 0)
		type = fFrameType;
	if (type == MFM_SCALER_DATA_FRAME_TYPE) {
		return true;
	}
	return false;
}
//_______________________________________________________________________________
bool MFMCommonFrame::IsACobo(int type)const {
	if (type == 0)
		type = fFrameType;
	if ((type == MFM_COBO_FRAME_TYPE) || (type == MFM_COBOF_FRAME_TYPE)
			|| (type == MFM_COBOT_FRAME_TYPE)) {
		return true;
	}
	return false;
}
//_______________________________________________________________________________
bool MFMCommonFrame::IsAMutant(int type) const{
	if (type == 0)
		type = fFrameType;
	if ((type == MFM_MUTANT_FRAME_TYPE) || (type == MFM_MUTANT1_FRAME_TYPE)
			|| (type == MFM_MUTANT2_FRAME_TYPE) || (type
			== MFM_MUTANT3_FRAME_TYPE)) {
		return true;
	}
	return false;
}
//_______________________________________________________________________________
void MFMCommonFrame::InitStat() {
	fCountFrame = 0;
	fMeanFrameSize = 0;
	fNegatifJump = 0;
	fNoContiJump = 0;
}
//_______________________________________________________________________________
void MFMCommonFrame::IncrementNegativJump() {
	fNegatifJump++;
}
//_______________________________________________________________________________
void MFMCommonFrame::IncrementNoContiJump() {
	fNoContiJump++;
}
//_______________________________________________________________________________
void MFMCommonFrame::FillStat() {
	fCountFrame++;
	uint32_t framesize = GetFrameSize();
	fMeanFrameSize += framesize;
}
//_______________________________________________________________________________
string MFMCommonFrame::GetStat(string info)const {
	string display("");
	stringstream ss("");
	ss << "Number of " << info << " Frames : " << fCountFrame << endl;
	if (fCountFrame != 0)
		ss << "Mean Frame Size " << (double) (fMeanFrameSize / fCountFrame)
				<< endl;
	ss << "Negativ Jumps " << fNegatifJump << endl;
	ss << "No Contigus Jumps " << fNoContiJump << endl;
	display = ss.str();
	return display;
}
//_______________________________________________________________________________
void MFMCommonFrame::PrintStat(string info) const{
	cout << (GetStat(info));
}
//_______________________________________________________________________________
int MFMCommonFrame::GetCountFrame() const{
	return fCountFrame;
}

//_______________________________________________________________________________
void MFMCommonFrame::TestUserPointer() const
{ 
if  (GetPointUserData() != pData_char + GetDefinedHeaderSize()){
    fError.TreatError ( 0,2," GetPointHeader() no good!");
    debug_frame() ;
    }
   
}
//_______________________________________________________________________________
void MFMCommonFrame::debug_frame() const
{
    cout << "Frame begins at " << pData << " (also beginning of header " << pHeader << ")" << endl;
    cout << "   -- frame size = " << GetFrameSize() << " bytes" << endl;
    cout << "   -- header size = " << GetHeaderSize() << " bytes" << endl;
    cout << "   -- user data at " << (void*)GetPointUserData() << endl << endl;
    cout << "Memory layout:" << endl;
    unsigned char* p = (unsigned char*)GetPointHeader();
    unsigned char* end_frame = p + GetFrameSize();
    while(p < end_frame)
    {
        cout << (void*)p << " : " << hex << (unsigned int)(*p);
        if(p == (unsigned char*)GetPointUserData()) cout << " <== first user data";
        cout << dec << endl;
        ++p;
    }
}
//_______________________________________________________________________________
