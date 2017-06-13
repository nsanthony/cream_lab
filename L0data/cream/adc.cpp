#include "CreamFile.h"
#include "CreamCAL.h"
#include "CreamTBCD.h"
#include "CreamSCD1.h"
#include "CreamSCD2.h"
#include "CreamSCD3.h"
#include "CreamSCD4.h"
#include "CreamBSD.h"
#include "timez.h"

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <cstdlib>

#include <iostream>
using namespace std;
#include <string>
#include <cstdlib>

// ROOT Headers
#include "TApplication.h"
#include "TTimeStamp.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TArc.h"
#include "TH1.h"
#include "TSystem.h"

TFile *fout;
TTree *tree;

struct evtroot {
public:
	unsigned	evt;
        unsigned        time[6];
	unsigned	trig[9];

        struct {
                unsigned adc[2560];
        } cal;
	struct {
                unsigned adc[1280];
        } tbcd;
	struct {
		unsigned adc[2944];
	} scd1;
	struct {
		unsigned adc[2944];
	} scd2;
        struct {
                unsigned adc[2944];
        } scd3;
        struct {
                unsigned adc[2944];
        } scd4;
        struct {
                unsigned adc[36];
                unsigned adc2[36];
                unsigned fevent[2];
                unsigned fmode[2];
                unsigned ftrig[2];
        } bsd;
} sevt;

std::string ExtractFilename( const std::string& path )
{
	return path.substr( path.find_last_of( '/' ) +1 );
}

std::string ChangeExtension( const std::string& path, const std::string& ext )
{
	std::string filename = ExtractFilename( path );
	return filename.substr( 0, filename.find_last_of( '.' ) ) +ext;
}

void process (CreamFile& cfile)
	{
	unsigned  nevent;
        unsigned  TimeStamp[6];

	DEvent    ev;
	unsigned  i;

	CreamCAL cal;
	CreamTBCD tbcd;
	CreamSCD1 scd1;
	CreamSCD2 scd2;
        CreamSCD3 scd3;
        CreamSCD4 scd4;
	CreamBSD bsd;
          
	for(nevent = 1; ; nevent++) {
		/* clear event data */
		ev.Clear();

		/* read an event */
		if ( cfile.ReadEvent( ev ) == -1 ) break;

		cal.Run(ev);
		tbcd.Run(ev);
		scd1.Run(ev);
		scd2.Run(ev);
                scd3.Run(ev);
                scd4.Run(ev);
		bsd.Run(ev);

		sevt.evt = nevent;		
		ev.GetTimeStamp(&TimeStamp[0]) ;

                sevt.trig[0]= ev.TriggerQ();
                sevt.trig[1]= ev.CalibrationTriggerQ();
                sevt.trig[2]= ev.ExternalTriggerQ();
                sevt.trig[3]= ev.CalorimeterTriggerQ();
                sevt.trig[4]= ev.CDTRG1TriggerQ();
                sevt.trig[5]= ev.CDTRG2TriggerQ();
                sevt.trig[6]= ev.ZCalibrationTriggerQ();
		sevt.trig[7]= ev.EHiTriggerQ();
                sevt.trig[8]= ev.ELowTriggerQ();

                for (i=0;i<2560;i++)
                       sevt.cal.adc[i]=cal.GetADC(i);

                for (i=0;i<1280;i++)
                        sevt.tbcd.adc[i]=tbcd.GetADC(i);

        	for (i=0;i<2944;i++) {
			sevt.scd1.adc[i]=scd1.GetADC(i);
			sevt.scd2.adc[i]=scd2.GetADC(i);
                        sevt.scd3.adc[i]=scd3.GetADC(i);
                        sevt.scd4.adc[i]=scd4.GetADC(i);	
				}
                for (i=0;i<6;i++)
			sevt.time[i]= TimeStamp[i];	

                for (i=0;i<36;i++)
                sevt.bsd.adc2[i]=bsd.GetADC(i);

                for (i=0;i<16;i++) {
                        sevt.bsd.adc[i]=sevt.bsd.adc2[i*2+1];
                        sevt.bsd.adc[i+16]=sevt.bsd.adc2[i*2];
                        }        

                for (i=16;i<18;i++) {
                                sevt.bsd.adc[i+16]=sevt.bsd.adc2[i*2+1];
                                sevt.bsd.adc[i+18]=sevt.bsd.adc2[i*2];
                        }
        
                sevt.bsd.fevent[0]=bsd.GetEventNum();
                sevt.bsd.fevent[1]=bsd.GetEventCnt();
                sevt.bsd.fmode[0]=bsd.GetTriMode();
                sevt.bsd.fmode[1]=bsd.GetConfMode();
                sevt.bsd.ftrig[0]=bsd.GetSyncNum();
                sevt.bsd.ftrig[1]=bsd.GetTriStatus();
	
		tree->Fill();                       
	} // for nevent;
}

void do_file()
	{
	CreamFile cfile;

        char  filename[100];
	char cbuf[256];
	string   fpath ;
	string   rfile ;
	string   wfile ;

	fpath += "./LIST" ;
	FILE *fin ;
	fin = fopen(fpath.c_str(),"r") ;

	if ( fin == 0 ) {
	      cerr << "cannot open file " << fpath << endl;
	      exit( 1 );
	}

	while ( fgets(cbuf, 256, fin) != 0 ) {
	  if ( cbuf[0] == '#' ) continue; // skip comment lines
	  sscanf(cbuf,"%s\n",filename) ;	

		wfile= "./ROOT/";
		wfile+= ChangeExtension (filename, ".root");
	        fout = new TFile(wfile.c_str(),"RECREATE") ;
	        tree = new TTree("event","EVENT") ;

		tree->Branch("evt",&sevt.evt,"evt/i") ;
		tree->Branch("time",&sevt.time,"time[6]/i") ;
		tree->Branch("trig",&sevt.trig,"trig[9]/i") ;
		tree->Branch("cal",&sevt.cal,"adc[2560]/i") ;
                tree->Branch("tbcd",&sevt.tbcd,"adc[1280]/i") ;
		tree->Branch("scd1",&sevt.scd1,"adc[2944]/i") ;
		tree->Branch("scd2",&sevt.scd2,"adc[2944]/i") ;
		tree->Branch("scd3",&sevt.scd3,"adc[2944]/i") ;
                tree->Branch("scd4",&sevt.scd4,"adc[2944]/i") ;
		tree->Branch("bsd",&sevt.bsd,"adc[36]/i") ;
		tree->Branch("fevent", &sevt.bsd.fevent, "fevent[2]/i");
  		tree->Branch("fmode", &sevt.bsd.fmode, "fmode[2]/i");
  		tree->Branch("ftrig", &sevt.bsd.ftrig, "ftrig[2]/i");

		cout <<filename<<" is reading...\n";
		if ( cfile.Open( filename ) == 0 ) {
			process ( cfile);
                        cfile.Close();
		}
		else
			fprintf(stderr, "ERROR: cannot open file %s\n", filename);

	        fout->Write() ;
	        fout->Close() ;
		cout <<wfile<<" is generated...\n";
	} //while
}

int main()
	{

	do_file( );

	return 0;
}