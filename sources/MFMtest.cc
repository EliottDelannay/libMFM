#include <iostream>
using namespace std;
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#ifndef NO_MFMXML
#include <tinyxml.h>
#endif
#include "MFMAllFrames.h"
#include "DataParameters.h"
#include "DataScalers.h"
#include "MError.h"
#include "Cobo.h"
#include <cstring>
#include <cstdlib>
#include <time.h>
#include "MFMlib.in.h"
#include "CImg.h"
using namespace cimg_library;
#include <string>
#include <netcdfcpp.h>
#include "struct_parameter_NetCDF.h"

MFMCommonFrame * fFrame;
MFMCoboFrame * fCoboframe;
MFMCoboTopoFrame * fCobotopoframe;
MFMExogamFrame * fExoframe;
MFMDiamantFrame * fDiamantframe;
MFMEbyedatFrame * fEbyframe;
MFMCommonFrame* fInsideframe;
MFMMergeFrame * fMergeframe;
MFMOscilloFrame * fOscilloframe;
MFMScalerDataFrame * fScalerframe;
MFMRibfFrame * fRibfframe;
MFMMutantFrame * fMutframe;
MFMHelloFrame * fHelloframe;
MFMXmlDataDescriptionFrame * fDatadescriptionframe;
MFMXmlFileHeaderFrame * fHeaderframe;
MFMChimeraFrame *fChimeraframe;
MFMBoxDiagFrame * fBoxDiagframe;
MFMVamosICFrame * fVamosICframe;
MFMVamosPDFrame * fVamosPDframe;
MFMVamosTACFrame * fVamosTACframe;
MFMNedaFrame * fNedaframe;
MFMNedaCompFrame * fNedaCompframe;
MFMS3BaF2Frame * fS3BaF2frame;
MFMS3AlphaFrame * fS3Alphaframe;
MFMS3RuthFrame * fS3Ruthframe;
MFMS3eGUNFrame * fS3eGUNframe;
MFMReaGenericFrame * fReaGenericframe;
MFMReaScopeFrame * fReaScopeframe;
MFMS3SynchroFrame * fS3Synchroframe;
DataParameters * fDataPara;
DataScalers * fDataScal;

MError Error;
int fDumpsize;
int fSubDumpsize;
int fMaxdump;
int fCount_elseframe;
int fNbFrames; // nb of frame  to generate
int fNbFramesRead; // nb of frame  to read
int fNbSubFrames;
int fNbFramesStart;// no of frame to start the dump if dump is asked
int fNbFramesDump; // no of frame to the dump if dump is asked
uint32_t fEventNumber;

int fVerbose;
int fFormat;
int fNbOfEvent;
const char * fDeffilename = "mfmfile.dat";
long long fPtInFile =0;

#define ADONIS_MSK 0x3FFF

bool InfoCondition();
void Announce();

void ReadUserFrame(MFMCommonFrame* commonframe);
void ReadMergeFrame(MFMCommonFrame* commonframe);
/*
void ReadEbyedatFrame(MFMCommonFrame* commonframe);
void ReadOscilloFrame(MFMCommonFrame* commonframe);
void ReadMutantFrame(MFMCommonFrame* commonframe);
void ReadXmlDataDescriptionFrame(MFMCommonFrame* commonframe);
void ReadXmlHeaderFrame(MFMCommonFrame* commonframe);
void ReadHelloFrame(MFMCommonFrame* commonframe);
void ReadDiamantFrame(MFMCommonFrame* commonframe);
void ReadDefaultFrame(MFMCommonFrame* commonframe);
void ReadChimeraFrame(MFMCommonFrame* commonframe);
void ReadNedaFrame(MFMCommonFrame* commonframe);
void ReadNedaCompFrame(MFMCommonFrame* commonframe);

void ReadReaScopeFrame(MFMCommonFrame* commonframe);


void WriteCoboFrame(int flun, int fNbFrames, bool full = false);
void WriteXmlDataDescriptionFrame(int lun, int fNbFrames);
void WriteNedaBisFrame(int lun, int fNbFrames);
void WriteDiamantFrame(int lun, int fNbFrames);
void WriteMutantFrame(int lun, int fNbFrames);
void WriteHelloFrame(int lun, int fNbFrames);
void WriteOscilloFrame(int lun, int fNbFrames);
void WriteNedaCompFrame(int lun, int fNbFrames);
void WriteReaScopeFrame(int lun, int fNbFrames);
*/
void WriteUserFrame(int flun, int format, int fNbFrames, int fNbSubFrames = 5);
void WriteMergeFrame(int flun, int fNbFrames, int type, int fNbSubFrames = 5);
void deletefunction();
void PrintQuestion();
void ConversionAdonisToFrames(int lun);
void Statistics(bool writeorread = false);
void Help();
void ErrorMessage();

int main(int argc, char **argv) {
	MError Error;
	char* tempo[128];
	char *tempos = NULL, *wordtempo=NULL;
	bool writeorread = false;
	int treatedinfo = 0;
	bool all = false;
	char filename[255];

	fDumpsize = 16;
	fSubDumpsize = 16;
	fCount_elseframe = 0;
	fMaxdump = 128;
	fFormat = -1;
	fNbFrames = 20;
	fNbFramesRead = -1;
	fNbSubFrames = 5;
	fNbFramesStart = 0;
	fNbFramesDump = 0;
	fVerbose = 5;

	strcpy(filename, fDeffilename);
	
	fDataPara = new DataParameters();
	fDataScal = new DataScalers();

	fFrame = new MFMCommonFrame();
	fCoboframe = new MFMCoboFrame();
	fCobotopoframe = new MFMCoboTopoFrame();
	fExoframe = new MFMExogamFrame();
	fDiamantframe = new MFMDiamantFrame();
	fEbyframe = new MFMEbyedatFrame();
	fInsideframe = new MFMCommonFrame();
	fMergeframe = new MFMMergeFrame();
	fOscilloframe = new MFMOscilloFrame();
	fScalerframe = new MFMScalerDataFrame();
	fRibfframe = new MFMRibfFrame();
	fMutframe = new MFMMutantFrame();
	fHelloframe = new MFMHelloFrame();
	fDatadescriptionframe = new MFMXmlDataDescriptionFrame();
	fHeaderframe = new MFMXmlFileHeaderFrame();
	fChimeraframe = new MFMChimeraFrame();
	fBoxDiagframe = new MFMBoxDiagFrame();
	fVamosICframe = new MFMVamosICFrame();
	fVamosPDframe = new MFMVamosPDFrame();
	fVamosTACframe = new MFMVamosTACFrame();
	fNedaframe = new MFMNedaFrame();
	fNedaCompframe = new MFMNedaCompFrame();
	fS3BaF2frame = new MFMS3BaF2Frame();
	fS3Alphaframe = new MFMS3AlphaFrame();
	fS3Ruthframe = new MFMS3RuthFrame();
	fS3eGUNframe = new MFMS3eGUNFrame();
	fS3Synchroframe = new MFMS3SynchroFrame();
 	fReaGenericframe = new MFMReaGenericFrame();
	fReaScopeframe = new MFMReaScopeFrame();

	fScalerframe->InitStat();
	fCoboframe->InitStat();
	fExoframe->InitStat();
	fEbyframe->InitStat();
	fMergeframe->InitStat();
	fScalerframe->InitStat();
	fOscilloframe->InitStat();
	fInsideframe->InitStat();
	fRibfframe->InitStat();
	fMutframe->InitStat();
	fDatadescriptionframe->InitStat();
	fHeaderframe->InitStat();
	fHelloframe->InitStat();
	fChimeraframe ->InitStat();
	fBoxDiagframe ->InitStat();
	fVamosICframe ->InitStat();
	fVamosPDframe ->InitStat();
	fVamosTACframe ->InitStat();
	fDiamantframe->InitStat();
	fNedaframe->InitStat();
	fNedaCompframe ->InitStat();
	fS3BaF2frame->InitStat();
	fS3Alphaframe->InitStat();
	fS3Ruthframe->InitStat();
	fS3eGUNframe->InitStat();
	fS3Synchroframe->InitStat();
	fReaGenericframe->InitStat();
	fReaScopeframe->InitStat();
	
	for (int i = 1; i < argc; i++) {
		treatedinfo = 0;

		if ((strcasecmp(argv[i], "-h") == 0) or (strcasecmp(argv[i], "--help")
				== 0)) {
			Help();
			return (0);
		}
		
			if ((strcasecmp(argv[i], "-ver") == 0) or (strcasecmp(argv[i], "--version")
				== 0)) {
			Announce();
			return (0);
		}

		tempos = new char[strlen(argv[i]) + 1];
		tempos = strcpy(tempos, argv[i]);
		strcpy(tempos, argv[i]);
		wordtempo = strtok(tempos, "=");

		if ((strcasecmp(wordtempo, "-n") == 0) or (strcasecmp(wordtempo,
				"--number") == 0)) {
			wordtempo = strtok(NULL, "=");
			if (strcasecmp(wordtempo, "") == 0)
				ErrorMessage();
			fNbFrames = atoi(wordtempo);
			treatedinfo++;
		}
		if ((strcasecmp(wordtempo, "-nr") == 0) or (strcasecmp(wordtempo,
				"--numberread") == 0)) {
			wordtempo = strtok(NULL, "=");
			if (strcasecmp(wordtempo, "") == 0)
				ErrorMessage();
			fNbFramesRead = atoi(wordtempo);
			treatedinfo++;
		}
		if ((strcasecmp(wordtempo, "-nt") == 0) or (strcasecmp(wordtempo,
				"--numberstart") == 0)) {
			wordtempo = strtok(NULL, "=");
			if (strcasecmp(wordtempo, "") == 0)
				ErrorMessage();
			fNbFramesStart = atoi(wordtempo);
			treatedinfo++;
		}
		if ((strcasecmp(wordtempo, "-nd") == 0) or (strcasecmp(wordtempo,
				"--numberdump") == 0)) {
			wordtempo = strtok(NULL, "=");
			if (strcasecmp(wordtempo, "") == 0)
				ErrorMessage();
			fNbFramesDump = atoi(wordtempo);
			treatedinfo++;
		}
		

		if ((strcasecmp(wordtempo, "-sn") == 0) or (strcasecmp(wordtempo,
				"--subnumber") == 0)) {
			wordtempo = strtok(NULL, "=");
			if (strcasecmp(wordtempo, "") == 0)
				ErrorMessage();
			fNbSubFrames = atoi(wordtempo);
			treatedinfo++;
		}

		if ((strcasecmp(wordtempo, "-f") == 0) or (strcasecmp(wordtempo,
				"--file") == 0)) {
			wordtempo = strtok(NULL, "=");

			if (wordtempo == NULL)
				ErrorMessage();
			if (strcasecmp(wordtempo, "") == 0)
				ErrorMessage();
			strcpy(filename, wordtempo);
			treatedinfo++;
		}

		if ((strcasecmp(wordtempo, "-d") == 0) or (strcasecmp(wordtempo,
				"--dump") == 0)) {
			wordtempo = strtok(NULL, "=");
			if (wordtempo == NULL)
				ErrorMessage();
			if (strcasecmp(wordtempo, "") == 0)
				ErrorMessage();
			fDumpsize = atoi(wordtempo);
			treatedinfo++;
		}
		if ((strcasecmp(wordtempo, "-sd") == 0) or (strcasecmp(wordtempo,
				"--subdump") == 0)) {
			wordtempo = strtok(NULL, "=");
			if (wordtempo == NULL)
				ErrorMessage();
			if (strcasecmp(wordtempo, "") == 0)
				ErrorMessage();
			fSubDumpsize = atoi(wordtempo);
			treatedinfo++;
		}

		if ((strcasecmp(wordtempo, "-a") == 0) or (strcasecmp(wordtempo,
				"--all") == 0)) {
			all = true;
			treatedinfo++;
		}
		if ((strcasecmp(wordtempo, "-w") == 0) or (strcasecmp(wordtempo,
				"--write") == 0)) {
			writeorread = true;
			treatedinfo++;
		}

		if ((strcasecmp(wordtempo, "-r") == 0) or (strcasecmp(wordtempo,
				"--read") == 0)) {
			writeorread = false;
			treatedinfo++;
		}
		if ((strcasecmp(wordtempo, "-v") == 0) or (strcasecmp(wordtempo,
				"--verbose") == 0)) {
			wordtempo = strtok(NULL, "=");
			if (wordtempo == NULL)
				ErrorMessage();
			if (strcasecmp(wordtempo, "") == 0)
				ErrorMessage();
			fVerbose = atoi(wordtempo);
			treatedinfo++;
		}
		if ((strcasecmp(wordtempo, "-fo") == 0) or (strcasecmp(wordtempo,
				"--format") == 0)) {
			wordtempo = strtok(NULL, "=");
			if (wordtempo == NULL)
				ErrorMessage();
			if (strcasecmp(wordtempo, "") == 0)
				ErrorMessage();
			fFormat = atoi(wordtempo);
			treatedinfo++;
		}
		if (treatedinfo == 0) {
			ErrorMessage();
			
		}
	 if (tempos) delete [] tempos;
	}
	Announce();
	if ((argc > 1)and (treatedinfo>0)) {
	//_____________________ WRITE ____________________________________________________________
	if (writeorread) {

		cout << endl;
		cout << "-------------------------------------------------" << endl;
		cout << "|    WRITE of " << fNbFrames << " MFMFrame in file  "
				<< filename << "" << endl;
		cout << "-------------------------------------------------" << endl;
		cout << endl;

		fEventNumber = 0;

		int fLun; // Logical Unit Number
		fLun = open(filename, (O_RDWR | O_CREAT | O_TRUNC), 0644);

		if (fFormat == -1) {
			while ((fFormat < 0) or (fFormat > 200)) {
				PrintQuestion();
				cin >> fFormat;
			}
			if (fFormat==0) { deletefunction(); exit(0);}

		}
		WriteUserFrame(fLun, fFormat, fNbFrames, fNbSubFrames);
		close(fLun);

	}

	//_____________________ READ____________________________________________________________

	if (writeorread == false) {
		cout << "|   READ MFMFrame from file " << filename << "" << endl;
		cout << "-------------------------------------------------------------"
				<< endl;

		cout << endl;
		fMaxdump = 128;
		int fLun; // Logical Unit Number
		//------------------------------------------------------------------------
		// Test xml a enlever des que c'est prêt
		//
		
		 //DataParameters datapar;
		// datapar.FillFromActionXMLFile((const char*)filename);

		// datapar.DumpListOfNames();
		 //datapar.CreatXML();
		 //datapar.WriteXML("bidon1.xml");

		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		// Test xml a enlever des que c'est prêt
		/*
		cout <<"-----------------0-----------------------------------\n";
		 DataScalers datascal;
		 cout <<"-----------------1-----------------------------------\n";
		 datascal.FillFromActionXMLFile((const char*)filename);
		 cout <<"-----------------2-----------------------------------\n";
		 datascal.DumpListOfNames();
		 cout <<"-----------------3-----------------------------------\n";
		 datascal.CreatXML();
		 cout <<"-----------------4-----------------------------------\n";
		 datascal.WriteXML("bidon.xml");
		 cout <<"-----------------5-----------------------------------\n";
		 return 0;
		 */
		//------------------------------------------------------------------------

		fLun = open(filename, (O_RDONLY));
		if (fLun <= 0) {
			Error.TreatError(2, 0, "Error open file :", filename);

		}

		int minsizeheader = MFM_BLOB_HEADER_SIZE; // =8
		int cont = 0;
		fNbOfEvent = 0;

		char* vector;
		char ** pvector;
		int vectorsize = minsizeheader;
		vector = (char*) (malloc(vectorsize));
		pvector = &vector;
		int framesize = 0;

		while (fNbOfEvent != fNbFramesRead) {
			framesize = fFrame->ReadInFile(&fLun, pvector, &vectorsize);
			fPtInFile+=framesize;
			if (framesize <= 0)
				break;
			if ((fVerbose > 1) && InfoCondition()) {
				cout << "\n-- Read frame in file : nb = " << fNbOfEvent
						<<"----------------------------------\n";
			}

			ReadUserFrame(fFrame);
			fNbOfEvent++;
			fFrame->SetAttributs();

		}
		if(*pvector) free (*pvector);
	  }
	  Statistics(writeorread);
	}else{ 
	 	Help();
	}
			
       
       deletefunction();
        cout << "-------------------End------------------------" << endl;
	return (0);
}
//_______________________________________________________________________________________________________________________
void deletefunction(){
 if (fDataPara) delete(fDataPara) ;
	if (fDataScal) delete (fDataScal);
	if (fFrame)
		delete (fFrame);
	if (fCoboframe)
		delete (fCoboframe);
	if (fCobotopoframe)
		delete (fCobotopoframe);
	if (fExoframe)
		delete (fExoframe);
	if (fEbyframe)
		delete (fEbyframe);
	if (fDiamantframe)
		delete (fDiamantframe);
	if (fInsideframe)
		delete (fInsideframe);
	if (fMergeframe)
		delete (fMergeframe);
	if (fOscilloframe)
		delete (fOscilloframe);
	if (fScalerframe)
		delete (fScalerframe);
	if (fRibfframe)
		delete (fRibfframe);
	if (fMutframe)
		delete (fMutframe);
	if (fHelloframe)
		delete (fHelloframe);
	if (fDatadescriptionframe)
		delete (fDatadescriptionframe);
	if (fHeaderframe)
		delete (fHeaderframe);
	if (fChimeraframe)
		delete (fChimeraframe);
	if (fBoxDiagframe)
		delete (fBoxDiagframe);
	if (fVamosPDframe)
		delete (fVamosPDframe);
	if (fVamosICframe)
		delete (fVamosICframe);
	if (fVamosTACframe)
		delete (fVamosTACframe);
	if (fNedaframe)
		delete (fNedaframe);
	if (fNedaCompframe)
		delete (fNedaCompframe);
	if (fS3BaF2frame)
		delete (fS3BaF2frame);
	if (fS3Alphaframe)
		delete (fS3Alphaframe);
	if (fS3Ruthframe)
		delete (fS3Ruthframe);
	if (fS3eGUNframe)
		delete (fS3eGUNframe);
	if (fS3Synchroframe)
		delete (fS3Synchroframe);
	if (fReaGenericframe) delete (fReaGenericframe);
	if (fReaScopeframe ) delete (fReaScopeframe);
}


//_______________________________________________________________________________________________________________________
void PrintQuestion() {
	cout << "Which Frame you want to generate?\n";
	cout << "   1  Cobo\n";
	cout << "   2  Cobo Full\n";
	cout << "   3  Exogam Energy\n";
	cout << "   4  Exogam Oscilloscope\n";
	cout << "   5  NEDA Raw \n";
	cout << "   6  NEDA Compressed  \n";
	cout << "   7  Ebyedat EventNumber\n";
	cout << "   8  Ebyedat TimeStamp\n";
	cout << "   9  Ebyedat EventNumberTimeStamp\n";
	cout << "  10  Scaler data Frame\n";
	cout << "  11  Mutant data Frame\n";
	cout << "  12  RIBF data Frame\n";
	cout << "  13  Hello Frame\n";
	cout << "  14  Box Diagnostic Frame\n";
	cout << "  15  Vamos Ionization Chamber Frame\n";
	cout << "  16  Vamos Position Detector Frame\n";
	cout << "  17  Vamos TAC Frame\n";
	cout << "  18  Diamant Frame\n";
	cout << "  19  S3 BaF2 Frame\n";
	cout << "  20  S3 Si Alpha Frame\n";
	cout << "  21  S3 Rutherford Frame\n";
	cout << "  22  S3 eGUN Frame\n";
	cout << "  23  S3 Synchro Frame\n";
	cout << "  24  ReaGeneric Frame\n";
	cout << "  25  ReaScope Frame\n";
	
	cout << "  26  Xml Data Description Frame\n";
	cout << "  27  Xml Data Header MFM Frame file\n";
	cout << "  99  Mixte of all Frame\n";

	cout << "\n";
	cout << " 100  Merge of Ebyedats in EventNumber\n";
	cout << " 101  Merge of Ebyedats in TimeStamp\n";
	cout << " 102  Merge of Cobo in EventNumber\n";
	cout << "\n";
	cout << " 200  Conversion Adonis in Ebyedat\n";
	cout << "\n";
	cout << "   0  Quit\n";
	cout << " >>";
}
//_______________________________________________________________________________________________________________________
void Announce(){
	cout << "-------------------------------------------------" << endl;
	cout << "---- MFMtest.exe Version : "<<BUILD_MFM_DATE<<" "<<BUILD_MFM_TIME<<" ----" << endl;
	cout << "-------------------------------------------------" << endl;
}
//_______________________________________________________________________________________________________________________
void Help() {
        Announce();
	cout
			<< "\n MFMtest.exe : utility to generate and use MFM frame in different formats \n";
	cout << " The generated frames are stored in a file  \n";
	cout << " Usage  : \n";
	cout << "          MFMtest.exe [-h -r -w -f -n -fo -sn -d -sd -ver -v]      \n";
	cout << "            -h or --help , print this help \n";
	cout << "            -ver or --version , print version \n";
	cout
			<< "            -f=filname or --file=filename , set name of file to read or to write , default is \""
			<< fDeffilename << "\"\n";
	cout << "            -w or --write, set write mode \n";
	cout << "            -r or --read, set read mode \n";
	cout
			<< "            -n=nb or --number=nb,  set nb number of frames to write, default is"
			<< fNbFrames << " \n";
	cout
			<< "            -nr=nb or --numberread=nb,  set nb number of frames to read, default is"
			<< fNbFramesRead << " \n";
	cout
			<< "            -nd=nb or --numberdump=nb,  set nb number of frames to dump if asked, default is all \n";
	cout
			<< "            -nt=nb or --numberstart=nb,  set nb  number of frames to start dump  if  asked (defautlt =  "
			<< fNbFrames << ") \n";
	cout
			<< "            -sn=nb or --subnumber=nb, set nb number (nb) of subframes to write, default is "
			<< fNbFrames << " \n";
	cout
			<< "            -v=nb or --verbose, give more information during read or write(default : "
			<< fVerbose << ") \n";
	cout
			<< "            -d=nb or --dump=nb,  number of bytes to dump.(defautlt = "
			<< fSubDumpsize << ") \n";
	cout
			<< "            -sd=nb or --subdump=nb,  number of bytes to dump in frame inside a merge frame.(defautlt = "
			<< fSubDumpsize << ") \n";
	cout
			<< "            -fo=nb or --format=nb , set format of frame to write, if not precised, you will have this menu to chose\n";
	PrintQuestion();
	cout << endl;
	cout << endl;
	cout
			<< "   Examples MFMtest.exe --write -fo=2 -n=100 : write with a choosen format = 2 and with 100 generated frames\n";
	cout << "            MFMtest.exe --read           : read file \""
			<< fDeffilename << "\" \n";
	cout
			<< "            MFMtest.exe --read -f=run.dat: read \"run.dat\" file \n";
	cout << endl;
}
//_______________________________________________________________________________________________________________________

void ErrorMessage() {
	cout << " Command not good, see help with MFMtest.exe -h\n";
	exit(0);
}
//_______________________________________________________________________________________________________________________

void ReadUserFrame(MFMCommonFrame* commonframe) {
	commonframe->SetAttributs();
	int type = commonframe->GetFrameType();
	int headersize = commonframe->GetHeaderSize();
	// part of use of frame
	// example

	switch (type) {

	case MFM_COBOF_FRAME_TYPE:
	case MFM_COBO_FRAME_TYPE: {
		fCoboframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_COBOT_FRAME_TYPE: {
		fCobotopoframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_EXO2_FRAME_TYPE: {
		fExoframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_EBY_EN_FRAME_TYPE:
	case MFM_EBY_TS_FRAME_TYPE:
	case MFM_EBY_EN_TS_FRAME_TYPE: {
		fEbyframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_SCALER_DATA_FRAME_TYPE: {
		fScalerframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_RIBF_DATA_FRAME_TYPE: {
		fRibfframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_MUTANT_FRAME_TYPE: {
		fMutframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_HELLO_FRAME_TYPE: {
		fHelloframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_DIAMANT_FRAME_TYPE: {
		fDiamantframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_OSCI_FRAME_TYPE: {
		fOscilloframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_MERGE_EN_FRAME_TYPE:
	case MFM_MERGE_TS_FRAME_TYPE: {
		ReadMergeFrame(commonframe);
		break;
	}
	case MFM_XML_DATA_DESCRIPTION_FRAME_TYPE: {
		fDatadescriptionframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_XML_FILE_HEADER_FRAME_TYPE: {
		fHeaderframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_CHIMERA_DATA_FRAME_TYPE: {
		fChimeraframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_BOX_DIAG_FRAME_TYPE: {
		fBoxDiagframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
		}
	case MFM_VAMOSIC_FRAME_TYPE: {
		fVamosICframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_VAMOSPD_FRAME_TYPE: {
		fVamosPDframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_VAMOSTAC_FRAME_TYPE: {
		fVamosTACframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_NEDA_FRAME_TYPE: {
		fNedaframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_NEDACOMP_FRAME_TYPE: {
		fNedaCompframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_S3_BAF2_FRAME_TYPE: {
		fS3BaF2frame->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_S3_ALPHA_FRAME_TYPE: {
		fS3Alphaframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_S3_RUTH_FRAME_TYPE: {
		fS3Ruthframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_S3_EGUN_FRAME_TYPE: {
		fS3eGUNframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_S3_SYNC_FRAME_TYPE: {
		fS3Synchroframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_REA_SCOPE_FRAME_TYPE: {
		fReaScopeframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}
	case MFM_REA_GENE_FRAME_TYPE: {
		fReaGenericframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}

	default: {
		fCount_elseframe++;
		commonframe->ReadAttributsExtractFrame(fVerbose,fDumpsize, commonframe->GetPointHeader());
		break;
	}

	}// end of switch
}
//_______________________________________________________________________________________________________________________

bool InfoCondition() {
	return (((fNbFramesDump == 0) && (fNbFramesStart <= fNbOfEvent))
			|| ((fNbFramesStart <= fNbOfEvent) && (fNbFramesStart
					+ fNbFramesDump > fNbOfEvent)));
}

//_______________________________________________________________________________________________________________________
void ReadMergeFrame(MFMCommonFrame* commonframe) {
	fMergeframe->SetAttributs(commonframe->GetPointHeader());
	int nbinsideframe = 0;
	nbinsideframe = fMergeframe->GetNbItems();
	fMergeframe->FillStat();
	int framesize = fMergeframe->GetFrameSize();
	fMergeframe->ExtractInfoFrame(fVerbose,fDumpsize);
	fMergeframe->ResetReadInMem();
	for (int i = 0; i < nbinsideframe; i++) {
		fMergeframe->ReadInFrame(fInsideframe);
		fInsideframe->FillStat();
		ReadUserFrame(fInsideframe);
	}
}
//_______________________________________________________________________________________________________________________
void hophop(int a, int &b)
{
  std::cout<< __FILE__ << "//" << __func__ << "(): print hop"<<std::endl;
  a+=12;
  b-=21;
}
//_______________________________________________________________________________________________________________________
typedef float Tdata;
//_______________________________________________________________________________________________________________________
void Display_Parameter(CImg<Tdata> img,int A, int B, int Medium, int Ai, int Hi, int Ti) 
{
	//create a image with 3 graphs (red the frame, green the half amplitude, blue the time between the max and the medium)
	CImg<Tdata> imageC;
	imageC.assign(img.width(),1,1,4,0);
	imageC.get_shared_channel(0)+=img;
	imageC.get_shared_channel(1)+=Medium;
	imageC.get_shared_channel(2)+=A+B;
	imageC.get_shared_channel(3)+=A+B;
	//put x at baseline while amplitude is > treshold 
	cimg_for_inX(imageC,Ai,Hi,i) imageC(i,0,0,2)=B;
	cimg_for_inX(imageC,Ti,Ai,i) imageC(i,0,0,3)=B;
	imageC.display_graph("red = signal, green = threshold, blue = max and 36.8% height positions, yellow = trigger and max");
}//Display_Parameter

//_______________________________________________________________________________________________________________________
//! process exponential decrease data
/** 
 * signal processing for a decreasing exponential curve:
 * measure both amplitude and half time
 * \param img[in] = signal
 * \param A[out] = amplitude
 * \param B[out] = Baseline
 * \param Ai[out] = position max amplitude
 * \param Hi[out] = time at half amplitude
 * \param T[out] = Time between Max and half amplitude
 * \param Medium[out] = treshold of 36.8% amplitude
 *
 * \code
 *   AnExampleOfUse
 * \endcode
 *
 * \see AnOtherFunctionOrAvariable
**/
void Process_Data (CImg<Tdata> img, int &A, int &B, int &Ai, int &Hi, int &T,int &Ti, int &Medium) {
		//find the min and max Amplitude
		B = img.min();
		A = img.max() - B;
		//finding the trigger position
		for (int i=0;img(i)== B; i++)
		{
		  Ti=i;
		}
		// finding the position of the maximum Amplitude		
		for (int i=0; img(i)< A + B; i++)
		{
		   Ai= i+1;
		} 		           
		// find the position of half amplitude and the time
		T = 0;
		Medium = A*0.368 + B;
		Hi=Ai;
		while (img(Hi) > Medium) 
  		{
		   Hi++; 
		   T+=10;
		}
		

}//Process_Data

//_______________________________________________________________________________________________________________________
int trapezoidal_filter(CImg<Tdata> e, CImg<Tdata> &s, int k, int m, double alpha) {
		//create a filter
		int decalage = 2*k + m + 2;
		cimg_for_inX(s,decalage, s.width()-1,n)
		  s(n)=2*s(n-1)-s(n-2) + e(n-1)\
		      -alpha*e(n-2) \
			   -e(n-(k+1)) \
			        +alpha*e(n-(k+2)) \
				     -e(n-(k+m+1)) \
					  +alpha*e(n-(k+m+2)) \
						+e(n-(2*k+m+1)) \
						     -alpha*e(n-(2*k+m+2));
		
}//trapezoidal_filter

/**
 \page pageSchema Schema du signal
 * 
 * \image html Signal_details.png "explanation of the signal"
 *
**/
//_______________________________________________________________________________________________________________________
void Display_Signals(CImg<Tdata> imgR,CImg<Tdata> imgG, int decalage)//! \todo [low] add title as function variable
{

	CImg<Tdata> imageC;
	imageC.assign(imgR.width(),1,1,3,0);
	imageC.get_shared_channel(0)+=imgR;
	imageC.get_shared_channel(1)+=imgG;
	cimg_for_inX(imageC,decalage,imageC.width(),i) imageC(i,0,0,2)=imgR.max();
	imageC.display_graph("red = signal, green = filter");
}//Display_Signals

//_______________________________________________________________________________________________________________________
int Read_Paramaters (int &k, int &m, double &alpha)
{
  ///file name
  string fi="parameters.nc";//=cimg_option("-p","parameters.nc","comment");

  ///parameter class
  CParameterNetCDF fp;
  //open file
  int error=fp.loadFile((char *)fi.c_str());
  if(error){std::cerr<<"loadFile return "<< error <<std::endl;return error;}

  float process; string process_name="trapezoid";
  //load process variable
  error=fp.loadVar(process,&process_name);
  if(error){cerr<<"loadVar return "<< error <<endl;return error;}
  std::cout<<process_name<<"="<<process<<std::endl;
  ///k
  string attribute_name="k";
  if (error = fp.loadAttribute(attribute_name,k)!=0){
    std::cerr<< "Error while loading "<<process_name<<":"<<attribute_name<<" attribute"<<std::endl;
    return error;
  }
  std::cout<<"  "<<attribute_name<<"="<<k<<std::endl;

  ///m
  attribute_name="m";
  if (error = fp.loadAttribute(attribute_name,m)!=0){
    std::cerr<< "Error while loading "<<process_name<<":"<<attribute_name<<" attribute"<<std::endl;
    return error;
  }
  std::cout<<"  "<<attribute_name<<"="<<m<<std::endl;

  ///alpha
  attribute_name="alpha";
  if (error = fp.loadAttribute(attribute_name,alpha)!=0){
    std::cerr<< "Error while loading "<<process_name<<":"<<attribute_name<<" attribute"<<std::endl;
    return error;
  }
  std::cout<<"  "<<attribute_name<<"="<<alpha<<std::endl;
} //Read_Paramater

//_______________________________________________________________________________________________________________________
int Read_Energy_Paramaters (int &n, double &q)
{
  ///file name
  string fi="parameters.nc";//=cimg_option("-p","parameters.nc","comment");

  ///parameter class
  CParameterNetCDF fp;
  //open file
  int error=fp.loadFile((char *)fi.c_str());
  if(error){std::cerr<<"loadFile return "<< error <<std::endl;return error;}

  float process; string process_name="Energy";
  //load process variable
  error=fp.loadVar(process,&process_name);
  if(error){cerr<<"loadVar return "<< error <<endl;return error;}
  std::cout<<process_name<<"="<<process<<std::endl;
  ///k
  string attribute_name="n";
  if (error = fp.loadAttribute(attribute_name,k)!=0){
    std::cerr<< "Error while loading "<<process_name<<":"<<attribute_name<<" attribute"<<std::endl;
    return error;
  }
  std::cout<<"  "<<attribute_name<<"="<<k<<std::endl;

  ///m
  attribute_name="q";
  if (error = fp.loadAttribute(attribute_name,m)!=0){
    std::cerr<< "Error while loading "<<process_name<<":"<<attribute_name<<" attribute"<<std::endl;
    return error;
  }
  std::cout<<"  "<<attribute_name<<"="<<m<<std::endl;
} //Read_Energy_Paramater

//_______________________________________________________________________________________________________________________
float Calculation_Energy(trapeze, Ti, n, q)
{
  int base=0;
  cimg_for_inX(trapeze,Ti-n, Ti,i) base+=trapeze(i);
  int peak=0;
  cimg_for_inX(trapeze,Ti+q, Ti+q+n,x) peak+=trapeze(x);

  int E;
  E=(peak-base)/(i+x)
  
}//Calculation_Energy

//_______________________________________________________________________________________________________________________
void WriteUserFrame(int lun, int format, int fNbFrames, int fNbSubFrames) {

	switch (format) {

	case 1: {
		fCoboframe-> WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_COBO_FRAME_TYPE);
		break;
	}
	case 2: {
		fCoboframe-> WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_COBOF_FRAME_TYPE);
		break;
	}
		//_____________________Exogam____________________________________________________________
	case 3: {
		fExoframe-> WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_EXO2_FRAME_TYPE);
		break;
	}
	case 4: {
		//_____________________  Oscilloscope________________________________________________
		fOscilloframe-> WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_OSCI_FRAME_TYPE);
		break;
	}
		//_____________________ NEDA Raw____________________________________________________________
	case 5: {
		fNedaframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_NEDA_FRAME_TYPE);
		break;
	}
		//_____________________ NEDA Compressed_____________________________________________________
	case 6: {
		fNedaCompframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_NEDACOMP_FRAME_TYPE);
		break;
	}
		//_____________________ Ebyedat event number_____________________________________________________
	case 7: {
		int type = MFM_EBY_EN_FRAME_TYPE;
		fEbyframe-> WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,type);
		break;
	}

		//_____________________ Ebyedat Time stamp ____________
	case 8: {
		int type = MFM_EBY_TS_FRAME_TYPE;
		fEbyframe-> WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,type);
		break;
	}
		//_____________________ Ebyedat event number and time stamp______________________________________________________
	case 9: {
		int type = MFM_EBY_EN_TS_FRAME_TYPE;
		fEbyframe-> WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,type);
		break;
	}
		//_____________________ Scaler data frame______________________________________________________
	case 10: {
		fScalerframe-> WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_SCALER_DATA_FRAME_TYPE);
		break;
	}
		//_____________________ Mutant data frame______________________________________________________
	case 11: {
		fMutframe-> WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_MUTANT_FRAME_TYPE);
		break;
	}
		//_____________________ Ribf data frame______________________________________________________
	case 12: {
		fRibfframe-> WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_RIBF_DATA_FRAME_TYPE);
		break;
	}
		//_____________________ Hello frame______________________________________________________
	case 13: {
		;fHelloframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_HELLO_FRAME_TYPE);
		break;
	}
		//_____________________ Box Diagnostic frame______________________________________________________
	case 14: {
		fBoxDiagframe-> WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_BOX_DIAG_FRAME_TYPE);
		break;
	}
		//_____________________ Vamos IC frame______________________________________________________
	case 15: {
		fVamosICframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_VAMOSIC_FRAME_TYPE);
		break;
	}
		//_____________________ Vamos PD  frame______________________________________________________
	case 16: {
		fVamosPDframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_VAMOSPD_FRAME_TYPE);
		break;
	}
		//_____________________ Vamos PD  frame______________________________________________________
	case 17: {
		fVamosTACframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_VAMOSTAC_FRAME_TYPE);
		break;
	}
		//_____________________ Diamant frame______________________________________________________
	case 18: {
		fDiamantframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_DIAMANT_FRAME_TYPE);
		break;
	}
		//_____________________ S3BaF2F frame______________________________________________________
	case 19: {
		fS3BaF2frame->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_S3_BAF2_FRAME_TYPE);
		break;
	}
		//_____________________ S3Alpha frame______________________________________________________
	case 20: {
		fS3Alphaframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_S3_ALPHA_FRAME_TYPE);
		break;
	}
		//_____________________ S3Ruth frame______________________________________________________
	case 21: {
		fS3Ruthframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_S3_RUTH_FRAME_TYPE);
		break;
	}
		//_____________________ Diamant frame______________________________________________________
	case 22: {
		fS3eGUNframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_S3_EGUN_FRAME_TYPE);
		break;
	}
		//_____________________ S3Synchro frame______________________________________________________
	case 23: {
		fS3Synchroframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_S3_SYNC_FRAME_TYPE);
		break;
	}
		//_____________________ ReaGeneric frame______________________________________________________
	case 24: {
		fReaGenericframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_REA_GENE_FRAME_TYPE);
		break;
	}
		//_____________________ ReaScope frame______________________________________________________
	case 25: {
		//save curve
		fReaScopeframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_REA_SCOPE_FRAME_TYPE);

		//transfer to CImg (or load from .cimg file)
 		CImg<Tdata> image1;
		image1.print("image1 empty",false);
		if(true)
		{//get data from frame
		//construct the image with the numbers of items as width
		int nbitem =  fReaScopeframe->GetNbItems();
		image1.assign(nbitem,1,1,1, -99);
		image1.print("image1 assign",false);	
 		//for (int i=0; i<nbitem;++i)
		cimg_forX(image1,i)
                {//fill the graph with the frame parameters
                  uint16_t value;
                  fReaScopeframe->ReaScopeGetParameters(i,&value);
		  image1(i)=value;
		}
		}//frame
		else
		{//get data from file
			image1.load("Data_PAC_1.cimg");
		}//frame

		image1.print("frame data");
                image1.display_graph("frame data");

		//caracteristic measurements (Amplitude)
		int A, B, Ai, Hi, T, Ti, Medium;
		Process_Data(image1, A, B, Ai, Hi, T, Ti, Medium);

		//text output
		std::cout<< "Amplitude= " << A  <<std::endl<< "Time = " << T << "ns" <<std::endl;
		std::cout<< "Trigger start= " << Ti  <<std::endl;
		std::cout<< "Index of the maximum = " << Ai <<std::endl  <<"Index of the half amplitude = " << Hi <<std::endl;		
		Display_Parameter(image1, A, B, Medium, Ai, Hi, Ti);	
		
		CImg<Tdata> trapeze(image1.width(),1,1,1, B);
		int k, m;
		double alpha;
		Read_Paramaters(k, m, alpha);
		trapezoidal_filter(image1, trapeze, k,m,alpha);
		Display_Signals(image1, trapeze, 2*k + m + 2); 

		//! \todo [medium] energy mesurement
		E=Calculation_Energy(trapeze, Ti, n, q);
		std::cout<< "E= " << E  <<std::endl;
		break;
	}
		//_____________________ XmlDataDescriptionFrame frame______________________________________________________
	case 26: {
		fDatadescriptionframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_XML_DATA_DESCRIPTION_FRAME_TYPE);
		break;
	}
			//_____________________ XmlDataDescriptionFrame frame______________________________________________________
	case 27: {
		fHeaderframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_XML_FILE_HEADER_FRAME_TYPE);
		break;
	}		//_____________________Merge of  Ebyedat in event number_____________________________________________________
	case 99: {
		int type;

		fDatadescriptionframe->WriteRandomFrame(lun,1, fVerbose, fDumpsize,MFM_XML_DATA_DESCRIPTION_FRAME_TYPE);
		fCoboframe->WriteRandomFrame(lun, fNbFrames, fVerbose, fDumpsize, MFM_COBO_FRAME_TYPE);
		fCoboframe->WriteRandomFrame(lun, fNbFrames, fVerbose, fDumpsize, MFM_COBOF_FRAME_TYPE);
		fExoframe-> WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_EXO2_FRAME_TYPE);
		fOscilloframe-> WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_OSCI_FRAME_TYPE);
		fNedaframe-> WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_NEDA_FRAME_TYPE);
		fNedaCompframe-> WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_NEDACOMP_FRAME_TYPE);
		fEbyframe->WriteRandomFrame(lun, fNbFrames, fVerbose, fDumpsize, MFM_EBY_EN_FRAME_TYPE);
		fEbyframe->WriteRandomFrame(lun, fNbFrames, fVerbose, fDumpsize, MFM_EBY_TS_FRAME_TYPE);
		fEbyframe->WriteRandomFrame(lun, fNbFrames, fVerbose, fDumpsize, MFM_EBY_EN_TS_FRAME_TYPE);
		fMutframe->WriteRandomFrame(lun, fNbFrames, fVerbose, fDumpsize, MFM_MUTANT_FRAME_TYPE);
		fRibfframe->WriteRandomFrame(lun, fNbFrames, fVerbose, fDumpsize, MFM_RIBF_DATA_FRAME_TYPE);
		fS3BaF2frame->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_S3_BAF2_FRAME_TYPE);
		fS3Alphaframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_S3_ALPHA_FRAME_TYPE);
		fS3Ruthframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_S3_RUTH_FRAME_TYPE);
		fS3eGUNframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_S3_EGUN_FRAME_TYPE);
		fS3Synchroframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_S3_SYNC_FRAME_TYPE);
		fReaGenericframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_REA_GENE_FRAME_TYPE);
		fHelloframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_HELLO_FRAME_TYPE);
		fBoxDiagframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_VAMOSIC_FRAME_TYPE);
		fVamosICframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_VAMOSIC_FRAME_TYPE);
		fVamosPDframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_VAMOSPD_FRAME_TYPE);
		fVamosTACframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_VAMOSTAC_FRAME_TYPE);
		fDiamantframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_DIAMANT_FRAME_TYPE);
		fReaGenericframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_REA_GENE_FRAME_TYPE);
		fReaScopeframe->WriteRandomFrame(lun,fNbFrames, fVerbose, fDumpsize,MFM_REA_SCOPE_FRAME_TYPE);
		break;
	}
		//_____________________Merge of  Ebyedat in event number_____________________________________________________
	case 100: {
		int type = MFM_EBY_EN_FRAME_TYPE;
		WriteMergeFrame(lun, fNbFrames, type, fNbSubFrames);
		break;
	}
		//_____________________ Merge of  Ebyedat in  time stamp______________________________________________________
	case 101: {
		int type = MFM_EBY_TS_FRAME_TYPE;
		WriteMergeFrame(lun, fNbFrames, type, fNbSubFrames);
		break;
	}
	case 102: {
		int type = MFM_COBO_FRAME_TYPE;
		WriteMergeFrame(lun, fNbFrames, type, fNbSubFrames);
		break;
	}
	case 200: {
		ConversionAdonisToFrames(lun);
		break;
	}

	default:
		cout << "Error : This format is not understood\n\n";
		exit(0);
		break;

	}// end of swich

}

//_______________________________________________________________________________________________________________________
void WriteMergeFrame(int lun, int fNbFrames, int type, int fNbSubFrames) {
	uint32_t eventnum;
	int32_t unitBlock_size ;
	uint32_t mergeFramesize = 0;
	uint32_t revision = 1;
	uint32_t headersize = 0;
	uint64_t timestamp = 0;
	int verif;
        uint32_t nbitem = fNbSubFrames;
	char info[64];
	
	fEventNumber = 0;
	mergeFramesize = 0;
	bool cobotag = false;
	int merge_type;
	unitBlock_size= fMergeframe->GetDefinedUnitBlockSize();

	MFMCommonFrame *insideframe[nbitem];

	if (type == MFM_EBY_TS_FRAME_TYPE) {
		headersize = MERGE_TS_HEADERSIZE;
		merge_type = MFM_MERGE_TS_FRAME_TYPE;
		strcpy(info, "--Header Ebyedat  with TS--");
	}

	if (type == MFM_EBY_EN_FRAME_TYPE) {
		headersize = MERGE_EN_HEADERSIZE;
		merge_type = MFM_MERGE_EN_FRAME_TYPE;
		strcpy(info, "-- Header Ebyedat with EN--");
	}

	if (type == MFM_EBY_EN_TS_FRAME_TYPE) {
		headersize = MERGE_TS_HEADERSIZE;
		merge_type = MFM_MERGE_TS_FRAME_TYPE;
		strcpy(info, "-- Header Ebyedat with TS and EN--");
	}

	if (type == MFM_COBO_FRAME_TYPE) {
		cobotag = true;
		headersize = MERGE_EN_HEADERSIZE;
		merge_type = MFM_MERGE_EN_FRAME_TYPE;
		strcpy(info, "-- Header COBO --");
		for (int j = 0; j < nbitem; j++) {
			insideframe[j] = (MFMCommonFrame*) (new MFMCoboFrame());
		}
	} else {
		MFMEbyedatFrame * ebyframe2p[nbitem];
		for (int j = 0; j < nbitem; j++) {
			insideframe[j] = (MFMCommonFrame*) (new MFMEbyedatFrame());
		}
	}
	// generation of fNbFrames contents
	

	
	for (int i = 0; i < fNbFrames; i++) {
		fEventNumber++;
		mergeFramesize = headersize;
		int framesizelocal;
		uint64_t timestamp;
		timestamp = fMergeframe->GetTimeStampUs();

		for (int j = 0; j < nbitem; j++) {
			if (cobotag) {
				((MFMCoboFrame*) insideframe[j])->SetWantedFrameType(type);
				((MFMCoboFrame*) insideframe[j])->GenerateAFrameExample( timestamp++, fEventNumber);
				((MFMCoboFrame*) insideframe[j])->SetAttributs();
				fCoboframe->SetAttributs(insideframe[j]->GetPointHeader());
				fCoboframe->SetWantedFrameType(type);
				framesizelocal = fCoboframe->GetFrameSize();
				fCoboframe->FillStat();
			} else {
			((MFMEbyedatFrame*) insideframe[j])->SetWantedFrameType(type);
				((MFMEbyedatFrame*) insideframe[j])->GenerateAFrameExample(timestamp++,fEventNumber);
				((MFMEbyedatFrame*) insideframe[j])->SetAttributs();
				fEbyframe->SetWantedFrameType(type);
				fEbyframe->SetAttributs(insideframe[j]->GetPointHeader());
				framesizelocal = fEbyframe->GetFrameSize();
				fEbyframe->FillStat();
			}
			mergeFramesize += insideframe[j]->GetFrameSize();
			framesizelocal = fInsideframe->GetFrameSize();
			fInsideframe->FillStat();
			
		}
		cout << "-- Merge Frame -with " << nbitem
				<< " inside frames , so begin to have a look to inside frames first------\n";
		fMergeframe->MFM_make_header(unitBlock_size, 0, merge_type, revision,
				(int) (mergeFramesize / unitBlock_size), (headersize
						/ unitBlock_size), 0, nbitem);
		fMergeframe->SetTimeStamp(timestamp);
		fMergeframe->SetEventNumber(fEventNumber);
		fMergeframe->SetAttributs();
		fMergeframe->ResetAdd();
		for (int j = 0; j < nbitem; j++) {
			int insframesize = insideframe[j]->GetFrameSize();
			cout << " ----> inside frame ---- ";
			fMergeframe->AddFrame(insideframe[j]);
			insideframe[j]->ExtractInfoFrame(fVerbose, fDumpsize);
		}
		fMergeframe->ExtractInfoFrame(fVerbose, fDumpsize);	
		verif = write(lun, fMergeframe->GetPointHeader(), mergeFramesize);

		if (verif != mergeFramesize)
			Error.TreatError(2, 0, "Error of write");
	}
	for (int j = 0; j < nbitem; j++) {
			 if (insideframe[j]) delete insideframe[j];
		}

}

//_______________________________________________________________________________________________________________________
void ConversionAdonisToFrames(int lun) {
	int sizeoftrace = 4048;
	int readsize;
	string filename;
	uint32_t eventnum;
	int verif;
	char* vector, *vector1, *vector2;
	char ** pvector;
	vector = (char*) (malloc(sizeoftrace));
	pvector = &vector;
	vector1 = (char*) (malloc(sizeoftrace));
	vector2 = (char*) (malloc(sizeoftrace));
	int type = MFM_EBY_EN_FRAME_TYPE;
	int fLun2;
	filename = "mfmfile2.dat";
	MFMEbyedatFrame * ebyframe = new MFMEbyedatFrame();
	cout << "File name to convert in buffer of " << sizeoftrace << " points?\n";
	//cin >> filename;
	cout << "filename  == " << (filename.data()) << "\n";
	fLun2 = open(filename.data(), (O_RDONLY));
	if (lun <= 0) {
		Error.TreatError(2, 0, "Error open file :", filename.data());
		exit(0);
	}
	uint32_t unitBlock_size = 0;
	uint32_t itemsize = 0;
	uint32_t nbitem = 0;
	uint32_t framesize = 0;
	uint32_t revision = 1;
	uint32_t headersize = EBYEDAT_EN_HEADERSIZE;
	uint16_t label = 0;
	uint16_t value = 0;
	unitBlock_size = 2;
	itemsize = 4;
	int fEventNumber = 0;
	cout << " begin of transformation\n";
	while (true) {
		readsize = read(fLun2, (void*) (vector), sizeoftrace);

		cout << " readsize = " << readsize << " " << sizeoftrace << "\n";
		if (readsize > 0) {
			cout << "." << flush;
			nbitem = readsize;
			ebyframe->MFM_make_header(unitBlock_size, 0, type, revision,
					readsize, (headersize / unitBlock_size), itemsize, nbitem);
			ebyframe->SetEventNumber(fEventNumber++);
			for (int i = 0; i < nbitem; i++) {
				label = ((char*) vector)[i] >> 14;
				value = ((char*) vector)[i] & ADONIS_MSK;
				ebyframe->EbyedatSetParameters(i, label, value);
				cout << "label = " << label << " value= " << value << "\n";
			}
			if (true) {
				ebyframe->HeaderDisplay(
						(char*) "-- Header of Ebyedat event number Frame --");
				cout << "TimeStamp = " << ebyframe->GetTimeStamp() << "\n";
				eventnum = (unsigned long long) ebyframe->GetEventNumber();
				cout << "EbventNumber = " << eventnum << "\n";
				framesize = ebyframe->GetFrameSize();
				ebyframe->DumpRaw(64, 0);
			}
			verif = write(lun, ebyframe->GetPointHeader(), framesize);
			if (verif != framesize)
				Error.TreatError(2, 0, "Error of write");
		}
		if (readsize != sizeoftrace)
			break;

	}

	free(vector);

}
//_______________________________________________________________________________________________________________________
void Statistics(bool writeorread) {
	if (fVerbose > 0) {
		cout << endl;
		cout << "----------------------------------------------" << endl;
		if (writeorread)
			cout << "|             WRITE STATISTICS                |" << endl;
		else
			cout << "|             READ STATISTICS                 |" << endl;
		cout << "----------------------------------------------" << endl;
	
		if (fHeaderframe->GetCountFrame() != 0) {
			fHeaderframe->PrintStat("Data Header Xml");
			cout << "----------------------------------------------" << endl;
		}
		if (fDatadescriptionframe->GetCountFrame() != 0) {
			fDatadescriptionframe->PrintStat("Data Description");
			cout << "----------------------------------------------" << endl;
		}

		if (fCoboframe->GetCountFrame() != 0) {
			fCoboframe->PrintStat("Cobo");
			cout << "----------------------------------------------" << endl;
		}
		if (fCobotopoframe->GetCountFrame() != 0) {
			fCobotopoframe->PrintStat("CoboTopo");
			cout << "----------------------------------------------" << endl;
		}
		if (fEbyframe->GetCountFrame() != 0) {
			fEbyframe->PrintStat("EbyEdat");
			cout << "----------------------------------------------" << endl;
		}

		if (fExoframe->GetCountFrame() != 0) {
			fExoframe ->PrintStat("Exogam");
			cout << "----------------------------------------------" << endl;
		}
		if (fBoxDiagframe->GetCountFrame() != 0) {
			fBoxDiagframe ->PrintStat("Box  Diagnostic");
			cout << "----------------------------------------------" << endl;
		}
		if (fVamosICframe->GetCountFrame() != 0) {
			fVamosICframe ->PrintStat("Vamos Ionization Chamber");
			cout << "----------------------------------------------" << endl;
		}
		if (fVamosTACframe->GetCountFrame() != 0) {
				fVamosTACframe ->PrintStat("Vamos TAC");
				cout << "----------------------------------------------" << endl;
			}
		if (fVamosPDframe->GetCountFrame() != 0) {
			fVamosPDframe ->PrintStat("Vamos Position Detector");
			cout << "----------------------------------------------" << endl;
		}
		if (fNedaframe->GetCountFrame() != 0) {
			fNedaframe ->PrintStat("NEDA");
			cout << "----------------------------------------------" << endl;
		}
		if (fNedaCompframe->GetCountFrame() != 0) {
			fNedaCompframe ->PrintStat("NEDA Compressed");
			cout << "----------------------------------------------" << endl;
		}
		if (fDiamantframe->GetCountFrame() != 0) {
			fDiamantframe ->PrintStat("Diamant");
			cout << "----------------------------------------------" << endl;
		}
		if (fS3BaF2frame->GetCountFrame() != 0) {
			fS3BaF2frame ->PrintStat("S3 BaF2");
			cout << "----------------------------------------------" << endl;
		}
		if (fS3Alphaframe->GetCountFrame() != 0) {
			fS3Alphaframe ->PrintStat("S3 Alpha");
			cout << "----------------------------------------------" << endl;
		}
		if (fS3Ruthframe->GetCountFrame() != 0) {
			fS3Ruthframe ->PrintStat("Rutherford");
			cout << "----------------------------------------------" << endl;
		}
		if (fS3eGUNframe->GetCountFrame() != 0) {
			fS3eGUNframe ->PrintStat("S3 eGUN");
			cout << "----------------------------------------------" << endl;
		}
		if (fS3Synchroframe->GetCountFrame() != 0) {
			fS3Synchroframe ->PrintStat("S3 Synchro");
			cout << "----------------------------------------------" << endl;
		}
		if (fScalerframe->GetCountFrame() != 0) {
			string text = "Scaler";
			fScalerframe-> PrintStat(text);
			cout << "----------------------------------------------" << endl;
		}
		if (fOscilloframe->GetCountFrame() != 0) {
			string text = "Oscillo";
			fOscilloframe-> PrintStat(text);
			cout << "----------------------------------------------" << endl;
		}
		if (fRibfframe->GetCountFrame() != 0) {
			fRibfframe-> PrintStat("RIBF");
			cout << "----------------------------------------------" << endl;
		}
		if (fMutframe->GetCountFrame() != 0) {
			fMutframe->PrintStat("MUTANT");
			cout << "----------------------------------------------" << endl;
		}
		if (fHelloframe->GetCountFrame() != 0) {
			fHelloframe->PrintStat("HELLO");
			cout << "----------------------------------------------" << endl;
		}
		if (fMergeframe->GetCountFrame() != 0) {
			fMergeframe->PrintStat("Merge");
			fInsideframe->PrintStat("InsideFrame");
		}

		cout << "----------------------------------------------" << endl;
		cout << "Number of ElseFrame   :" << fCount_elseframe << endl;
		
		cout << "\n";

	}
}
//_______________________________________________________________________________________________________________________
