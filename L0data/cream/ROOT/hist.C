{
  
  gROOT->Reset();

  TString local="./";

  TFile file(local+="20150419-230934s4.root");
  TTree *EVENT = (TTree *)file.Get("event");

  int  adc[2944];
  int  evt;

  TBranch *branch0 = (TBranch *)EVENT->GetBranch("scd4") ;
  branch0->SetAddress(adc);
  TBranch *branch1 = (TBranch *)EVENT->GetBranch("evt") ;
  branch1->SetAddress(&evt);

  Int_t i, m;
  
  TH1F *hist[2944];
  Char_t ht[2944];

  for (m=0; m<2944;m++)   {
    sprintf(ht,"hist[%d]",m);
    hist[m]=new TH1F(ht,Form("ch# %d",m),100,5,-5);
                         }

  Int_t nbytes;
  Float_t f1, f2;

  Int_t nentries = EVENT->GetEntries() ;
  for(i=0; i<nentries; i++) {
    nbytes += EVENT->GetEvent(i) ;

   for (m=0; m<2944; m++) { 
     f1 = adc[m];
     hist[m]->Fill(f1);
                          }
                            }  

TCanvas *c1=new TCanvas("c1","SCD",10,10,1024,768);
c1->Print("hist.pdf[");

for (m=0; m<2944; m++)   {

Char_t ht[2944];

if (m%16==0) {
  c1->Clear();
  c1->Divide(4,4);
            }

c1.cd(m%16+1);
hist[m]->GetXaxis()->SetTitle("ADC");
hist[m]->Draw();
c1->Update();

if (m%16==15) {
c1->Print("hist.pdf");
            }

               	       }
c1->Print("hist.pdf");
c1->Print("hist.pdf]");

}
