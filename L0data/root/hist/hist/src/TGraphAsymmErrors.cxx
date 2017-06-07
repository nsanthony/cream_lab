// @(#)root/hist:$Id: TGraphAsymmErrors.cxx 30393 2009-09-24 08:45:21Z couet $
// Author: Rene Brun   03/03/99

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include <string.h>

#include "Riostream.h"
#include "TROOT.h"
#include "TGraphAsymmErrors.h"
#include "TStyle.h"
#include "TMath.h"
#include "TArrow.h"
#include "TBox.h"
#include "TVirtualPad.h"
#include "TF1.h"
#include "TH1.h"
#include "TVector.h"
#include "TVectorD.h"
#include "TClass.h"

ClassImp(TGraphAsymmErrors)

namespace {
   unsigned long GLOBAL_k;   // used to pass k[i] into equations
   unsigned long GLOBAL_N;   // used to pass N[i] into equations
   double        CONFLEVEL;  // confidence level for the interval
}


//______________________________________________________________________________
/* Begin_Html
<center><h2>TGraphAsymmErrors class</h2></center>
A TGraphAsymmErrors is a TGraph with assymetric error bars.
<p>
The TGraphAsymmErrors painting is permofed thanks to the
<a href="http://root.cern.ch/root/html/TGraphPainter.html">TGraphPainter</a>
class. All details about the various painting options are given in
<a href="http://root.cern.ch/root/html/TGraphPainter.html">this class</a>.
<p>
The picture below gives an example:
End_Html
Begin_Macro(source)
{
   c1 = new TCanvas("c1","A Simple Graph with assymetric error bars",200,10,700,500);
   c1->SetFillColor(42);
   c1->SetGrid();
   c1->GetFrame()->SetFillColor(21);
   c1->GetFrame()->SetBorderSize(12);
   Int_t n = 10;
   Double_t x[n]   = {-0.22, 0.05, 0.25, 0.35, 0.5, 0.61,0.7,0.85,0.89,0.95};
   Double_t y[n]   = {1,2.9,5.6,7.4,9,9.6,8.7,6.3,4.5,1};
   Double_t exl[n] = {.05,.1,.07,.07,.04,.05,.06,.07,.08,.05};
   Double_t eyl[n] = {.8,.7,.6,.5,.4,.4,.5,.6,.7,.8};
   Double_t exh[n] = {.02,.08,.05,.05,.03,.03,.04,.05,.06,.03};
   Double_t eyh[n] = {.6,.5,.4,.3,.2,.2,.3,.4,.5,.6};
   gr = new TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);
   gr->SetTitle("TGraphAsymmErrors Example");
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   gr->Draw("ALP");
   return c1;
}
End_Macro */


//______________________________________________________________________________
TGraphAsymmErrors::TGraphAsymmErrors(): TGraph()
{
   // TGraphAsymmErrors default constructor.

   fEXlow       = 0;
   fEYlow       = 0;
   fEXhigh      = 0;
   fEYhigh      = 0;
}


//______________________________________________________________________________
TGraphAsymmErrors::TGraphAsymmErrors(const TGraphAsymmErrors &gr)
       : TGraph(gr)
{
   // TGraphAsymmErrors copy constructor

   if (!CtorAllocate()) return;
   Int_t n = fNpoints*sizeof(Double_t);
   memcpy(fEXlow, gr.fEXlow, n);
   memcpy(fEYlow, gr.fEYlow, n);
   memcpy(fEXhigh, gr.fEXhigh, n);
   memcpy(fEYhigh, gr.fEYhigh, n);
}


//______________________________________________________________________________
TGraphAsymmErrors& TGraphAsymmErrors::operator=(const TGraphAsymmErrors &gr)
{
   // TGraphAsymmErrors assignment operator

   if(this!=&gr) {
      TGraph::operator=(gr);
      if (!CtorAllocate()) return *this;
      Int_t n = fNpoints*sizeof(Double_t);
      memcpy(fEXlow, gr.fEXlow, n);
      memcpy(fEYlow, gr.fEYlow, n);
      memcpy(fEXhigh, gr.fEXhigh, n);
      memcpy(fEYhigh, gr.fEYhigh, n);
   }
   return *this;
}


//______________________________________________________________________________
TGraphAsymmErrors::TGraphAsymmErrors(Int_t n)
       : TGraph(n)
{
   // TGraphAsymmErrors normal constructor.
   //
   // the arrays are preset to zero

   if (!CtorAllocate()) return;
   FillZero(0, fNpoints);
}


//______________________________________________________________________________
TGraphAsymmErrors::TGraphAsymmErrors(Int_t n, const Float_t *x, const Float_t *y, const Float_t *exl, const Float_t *exh, const Float_t *eyl, const Float_t *eyh)
       : TGraph(n,x,y)
{
   // TGraphAsymmErrors normal constructor.
   //
   // if exl,h or eyl,h are null, the corresponding arrays are preset to zero

   if (!CtorAllocate()) return;

   for (Int_t i=0;i<n;i++) {
      if (exl) fEXlow[i]  = exl[i];
      else     fEXlow[i]  = 0;
      if (exh) fEXhigh[i] = exh[i];
      else     fEXhigh[i] = 0;
      if (eyl) fEYlow[i]  = eyl[i];
      else     fEYlow[i]  = 0;
      if (eyh) fEYhigh[i] = eyh[i];
      else     fEYhigh[i] = 0;
   }
}


//______________________________________________________________________________
TGraphAsymmErrors::TGraphAsymmErrors(Int_t n, const Double_t *x, const Double_t *y, const Double_t *exl, const Double_t *exh, const Double_t *eyl, const Double_t *eyh)
       : TGraph(n,x,y)
{
   // TGraphAsymmErrors normal constructor.
   //
   // if exl,h or eyl,h are null, the corresponding arrays are preset to zero

   if (!CtorAllocate()) return;

   n = fNpoints*sizeof(Double_t);
   if(exl) { memcpy(fEXlow, exl, n);
   } else { memset(fEXlow, 0, n); }
   if(exh) { memcpy(fEXhigh, exh, n);
   } else { memset(fEXhigh, 0, n); }
   if(eyl) { memcpy(fEYlow, eyl, n);
   } else { memset(fEYlow, 0, n); }
   if(eyh) { memcpy(fEYhigh, eyh, n);
   } else { memset(fEYhigh, 0, n); }
}


//______________________________________________________________________________
TGraphAsymmErrors::TGraphAsymmErrors(const TVectorF  &vx, const TVectorF  &vy, const TVectorF  &vexl, const TVectorF  &vexh, const TVectorF  &veyl, const TVectorF  &veyh)
                  :TGraph()
{
   // Constructor with six vectors of floats in input
   // A grapherrors is built with the X coordinates taken from vx and Y coord from vy
   // and the errors from vectors vexl/h and veyl/h.
   // The number of points in the graph is the minimum of number of points
   // in vx and vy.

   fNpoints = TMath::Min(vx.GetNrows(), vy.GetNrows());
   if (!TGraph::CtorAllocate()) return;
   if (!CtorAllocate()) return;
   Int_t ivxlow  = vx.GetLwb();
   Int_t ivylow  = vy.GetLwb();
   Int_t ivexllow = vexl.GetLwb();
   Int_t ivexhlow = vexh.GetLwb();
   Int_t iveyllow = veyl.GetLwb();
   Int_t iveyhlow = veyh.GetLwb();
      for (Int_t i=0;i<fNpoints;i++) {
      fX[i]      = vx(i+ivxlow);
      fY[i]      = vy(i+ivylow);
      fEXlow[i]  = vexl(i+ivexllow);
      fEYlow[i]  = veyl(i+iveyllow);
      fEXhigh[i] = vexh(i+ivexhlow);
      fEYhigh[i] = veyh(i+iveyhlow);
   }
}


//______________________________________________________________________________
TGraphAsymmErrors::TGraphAsymmErrors(const TVectorD &vx, const TVectorD &vy, const TVectorD &vexl, const TVectorD &vexh, const TVectorD &veyl, const TVectorD &veyh)
                  :TGraph()
{
   // Constructor with six vectors of doubles in input
   // A grapherrors is built with the X coordinates taken from vx and Y coord from vy
   // and the errors from vectors vexl/h and veyl/h.
   // The number of points in the graph is the minimum of number of points
   // in vx and vy.

   fNpoints = TMath::Min(vx.GetNrows(), vy.GetNrows());
   if (!TGraph::CtorAllocate()) return;
   if (!CtorAllocate()) return;
   Int_t ivxlow  = vx.GetLwb();
   Int_t ivylow  = vy.GetLwb();
   Int_t ivexllow = vexl.GetLwb();
   Int_t ivexhlow = vexh.GetLwb();
   Int_t iveyllow = veyl.GetLwb();
   Int_t iveyhlow = veyh.GetLwb();
      for (Int_t i=0;i<fNpoints;i++) {
      fX[i]      = vx(i+ivxlow);
      fY[i]      = vy(i+ivylow);
      fEXlow[i]  = vexl(i+ivexllow);
      fEYlow[i]  = veyl(i+iveyllow);
      fEXhigh[i] = vexh(i+ivexhlow);
      fEYhigh[i] = veyh(i+iveyhlow);
   }
}


//______________________________________________________________________________
TGraphAsymmErrors::TGraphAsymmErrors(const TH1 *h)
       : TGraph(h)
{
   // TGraphAsymmErrors constructor importing its parameters from the TH1 object passed as argument
   // the low and high errors are set to the bin error of the histogram.

   if (!CtorAllocate()) return;

   for (Int_t i=0;i<fNpoints;i++) {
      fEXlow[i]  = h->GetBinWidth(i+1)*gStyle->GetErrorX();
      fEXhigh[i] = fEXlow[i];
      fEYlow[i]  = h->GetBinError(i+1);
      fEYhigh[i] = fEYlow[i];
   }
}


//______________________________________________________________________________
TGraphAsymmErrors::TGraphAsymmErrors(const TH1 *pass, const TH1 *total, Option_t *option)
       : TGraph()
{
   // Creates a TGraphAsymmErrors by dividing two input TH1 histograms:
   // pass/total. (see TGraphAsymmErrors::BayesDivide)

   CtorAllocate();
   BayesDivide(pass, total, option);
}


//______________________________________________________________________________
TGraphAsymmErrors::~TGraphAsymmErrors()
{
   // TGraphAsymmErrors default destructor.

   delete [] fEXlow;
   delete [] fEXhigh;
   delete [] fEYlow;
   delete [] fEYhigh;
}


//______________________________________________________________________________
void TGraphAsymmErrors::Apply(TF1 *f)
{
   // apply a function to all data points
   // y = f(x,y)
   //
   // Errors are calculated as eyh = f(x,y+eyh)-f(x,y) and
   // eyl = f(x,y)-f(x,y-eyl)
   //
   // Special treatment has to be applied for the functions where the
   // role of "up" and "down" is reversed.
   // function suggested/implemented by Miroslav Helbich <helbich@mail.desy.de>

   Double_t x,y,exl,exh,eyl,eyh,eyl_new,eyh_new,fxy;

   for (Int_t i=0;i<GetN();i++) {
      GetPoint(i,x,y);
      exl=GetErrorXlow(i);
      exh=GetErrorXhigh(i);
      eyl=GetErrorYlow(i);
      eyh=GetErrorYhigh(i);

      fxy = f->Eval(x,y);
      SetPoint(i,x,fxy);

      // in the case of the functions like y-> -1*y the roles of the
      // upper and lower error bars is reversed
      if (f->Eval(x,y-eyl)<f->Eval(x,y+eyh)) {
         eyl_new = TMath::Abs(fxy - f->Eval(x,y-eyl));
         eyh_new = TMath::Abs(f->Eval(x,y+eyh) - fxy);
      }
      else {
         eyh_new = TMath::Abs(fxy - f->Eval(x,y-eyl));
         eyl_new = TMath::Abs(f->Eval(x,y+eyh) - fxy);
      }

      //error on x doesn't change
      SetPointError(i,exl,exh,eyl_new,eyh_new);
   }
}


//______________________________________________________________________________
void TGraphAsymmErrors::BayesDivide(const TH1 *pass, const TH1 *total, Option_t *option)
{
   // Fills this TGraphAsymmErrors by dividing two input TH1 histograms pass/total.
   //
   // Andy Haas (haas@fnal.gov)
   // University of Washington
   //
   // Method and code directly taken from:
   // Marc Paterno (paterno@fnal.gov)
   // FNAL/CD
   //
   // The input histograms must be filled with weights of 1.
   // By default the function does not check this assertion.
   // if option "w" is specified, the function will fail if the histograms
   // have been filled with weights not equal to 1.
   //
   // The assumption is that the entries in "pass" are a
   // subset of those in "total". That is, we create an "efficiency"
   // graph, where each entry is between 0 and 1, inclusive.
   // The resulting graph can be fit to functions, using standard methods:
   // graph->Fit("erf")... for instance. (You have to define the erf
   // function for yourself for now, sorry.)
   //
   // The points are assigned an x value at the center of each histogram bin.
   // The y values are #pass/#total, between 0 and 1.
   // The x errors span each histogram bin (lowedge->lowedge+width)
   // The y errors are the fun part. :)
   //
   // The y errors are assigned based on applying Bayes theorem.
   // The model is the Binomial distribution, and the "prior" is
   // the flat distribution from 0 to 1.
   // If there is no data in a bin of the total histogram, no information
   // can be obtained for that bin, so no point is made on the graph.
   //
   // The complete method and a beautiful discussion can be found here:
   // http://home.fnal.gov/~paterno/images/effic.pdf
   // And more information is on these pages:
   // http://home.fnal.gov/~paterno/probability/localresources.html
   // A backup of the main document is here:
   // http://www-clued0.fnal.gov/~haas/documents/paterno_effic.pdf
   //
   // A 68.3% Confidence Level is used to assign the errors.
   // Warning! You should understand, the errors reported are the shortest
   // ranges containing 68.3% of the probability distrubution. The errors are
   // not exactly Gaussian! The Minuit fitting routines will assume that
   // the errors are Gaussian. But this is a reasonable approximation.
   // A fit using the full shape of the error distribution for each point
   // would be far more difficult to perform.

   TString opt = option; opt.ToLower();

   Int_t nbins = pass->GetNbinsX();
   if (nbins != total->GetNbinsX()){
      Error("BayesDivide","Histograms must have the same number of X bins");
      return;
   }

   if (opt.Contains("w")) {
      //compare sum of weights with sum of squares of weights
      Double_t stats[10];
      pass->GetStats(stats);
      if (TMath::Abs(stats[0] -stats[1]) > 1e-6) {
      Error("BayesDivide","Pass histogram has not been filled with weights = 1");
      return;
      }
      total->GetStats(stats);
      if (TMath::Abs(stats[0] -stats[1]) > 1e-6) {
      Error("BayesDivide","Total histogram has not been filled with weights = 1");
      return;
      }
   }

   //Set the graph to have a number of points equal to the number of histogram bins
   Set(nbins);

   // Ok, now set the points for each bin
   // (Note: the TH1 bin content is shifted to the right by one:
   //  bin=0 is underflow, bin=nbins+1 is overflow.)

   double mode, low, high; //these will hold the result of the Bayes calculation
   int npoint=0;//this keeps track of the number of points added to the graph
   for (int b=1; b<=nbins; ++b) { // loop through the bins

      int t = (int)total->GetBinContent(b);
      if (!t) continue;  //don't add points for bins with no information

      int p = (int)pass->GetBinContent(b);
      if (p>t) {
         Warning("BayesDivide","Histogram bin %d in pass has more entries than corresponding bin in total! (%d>%d)",b,p,t);
         continue; //we may as well go on...
      }

      //This is the Bayes calculation...
      Efficiency(p,t,0.683,mode,low,high);

      // exclude the poits where result is less < 0
      if (mode < 0) continue;
      //These are the low and high error bars
      low = mode-low;
      high = high-mode;

      //If either of the errors are 0, set them to 1/10 of the other error
      //so that the fitters don't get confused.
      // LM: what is this ??? The scope of this routine is to have lower error zero for eff = 0
      // and uppper error zero for eff=1
      // remove this code
      //if (low==0.0) low=high/10.;
      //if (high==0.0) high=low/10.;
      //if (high+mode > 1) high = 1-mode;

      //Set the point center and its errors
      SetPoint(npoint,pass->GetBinCenter(b),mode);
      SetPointError(npoint,
      pass->GetBinCenter(b)-pass->GetBinLowEdge(b),
      pass->GetBinLowEdge(b)-pass->GetBinCenter(b)+pass->GetBinWidth(b),
      low,high);
      npoint++;//we have added a point to the graph

   }

   Set(npoint);//tell the graph how many points we've really added

   if (opt.Contains("debug")) {
      printf("BayesDivide: made a graph with %d points from %d bins\n",npoint,nbins);
      Print();//The debug prints out what we get for each point
   }

}


//______________________________________________________________________________
double TGraphAsymmErrors::Beta_ab(double a, double b, int k, int N) const
{
   // Calculates the fraction of the area under the
   // curve x^k*(1-x)^(N-k) between x=a and x=b

   if (a == b) return 0;    // don't bother integrating over zero range
   int c1 = k+1;
   int c2 = N-k+1;
   return Ibetai(c1,c2,b)-Ibetai(c1,c2,a);
}


//______________________________________________________________________________
double TGraphAsymmErrors::Ibetai(double a, double b, double x) const
{
   // Calculates the incomplete beta function  I_x(a,b); this is
   // the incomplete beta function divided by the complete beta function

   double bt;
   if (x < 0.0 || x > 1.0) {
      Error("Ibetai","Illegal x in routine Ibetai: x = %g",x);
      return 0;
   }
   if (x == 0.0 || x == 1.0)
      bt=0.0;
   else
      bt=TMath::Exp(TMath::LnGamma(a+b)-TMath::LnGamma(a)-TMath::LnGamma(b)+a*log(x)+b*log(1.0-x));

   if (x < (a+1.0)/(a+b+2.0))
      return bt*TMath::BetaCf(x,a,b)/a;
   else
      return 1.0-bt*TMath::BetaCf(1-x,b,a)/b;
}


//______________________________________________________________________________
double TGraphAsymmErrors::Betai(double a, double b, double x) const
{
   // Calculates the incomplete beta function B_x(a,b), as defined
   // in Gradshteyn and Ryzhik (4th edition) 8.391

   // calculates the complete beta function
   double beta = TMath::Exp(TMath::LnGamma(a)+TMath::LnGamma(b)-TMath::LnGamma(a+b));
   return Ibetai(a,b,x)*beta;
}


//______________________________________________________________________________
double TGraphAsymmErrors::Brent(double ax, double bx, double cx, double tol, double *xmin) const
{
   // Implementation file for the numerical equation solver library.
   // This includes root finding and minimum finding algorithms.
   // Adapted from Numerical Recipes in C, 2nd edition.
   // Translated to C++ by Marc Paterno

   const int    kITMAX = 100;
   const double kCGOLD = 0.3819660;
   const double kZEPS  = 1.0e-10;

   int iter;
   double a,b,d=0.,etemp,fu,fv,fw,fx,p,q,r,tol1,tol2,u,v,w,x,xm;
   double e=0.0;

   a=(ax < cx ? ax : cx);
   b=(ax > cx ? ax : cx);
   x=w=v=bx;
   fw=fv=fx=Interval(x);
   for (iter=1;iter<=kITMAX;iter++) {
      xm=0.5*(a+b);
      tol2=2.0*(tol1=tol*TMath::Abs(x)+kZEPS);
      if (TMath::Abs(x-xm) <= (tol2-0.5*(b-a))) {
         *xmin=x;
         return fx;
      }
      if (TMath::Abs(e) > tol1) {
         r=(x-w)*(fx-fv);
         q=(x-v)*(fx-fw);
         p=(x-v)*q-(x-w)*r;
         q=2.0*(q-r);
         if (q > 0.0) p = -p;
         q=TMath::Abs(q);
         etemp=e;
         e=d;
         if (TMath::Abs(p) >= TMath::Abs(0.5*q*etemp) || p <= q*(a-x) || p >= q*(b-x)) d=kCGOLD*(e=(x >= xm ? a-x : b-x));
         else {
            d=p/q;
            u=x+d;
            if (u-a < tol2 || b-u < tol2) d=TMath::Sign(tol1,xm-x);
         }
      } else {
         d=kCGOLD*(e=(x >= xm ? a-x : b-x));
      }
      u=(TMath::Abs(d) >= tol1 ? x+d : x+TMath::Sign(tol1,d));
      fu=Interval(u);
      if (fu <= fx) {
         if (u >= x) a=x; else b=x;
         v  = w;
         w  = x;
         x  = u;
         fv = fw;
         fw = fx;
         fx = fu;
      } else {
         if (u < x) a=u; else b=u;
         if (fu <= fw || w == x) {
            v=w;
            w=u;
            fv=fw;
            fw=fu;
         } else if (fu <= fv || v == x || v == w) {
            v=u;
            fv=fu;
         }
      }
   }
   Error("Brent","Too many interations");
   *xmin=x;
   return fx;
}


//______________________________________________________________________________
void TGraphAsymmErrors::ComputeRange(Double_t &xmin, Double_t &ymin, Double_t &xmax, Double_t &ymax) const
{
   // Compute Range

   TGraph::ComputeRange(xmin,ymin,xmax,ymax);

   for (Int_t i=0;i<fNpoints;i++) {
      if (fX[i] -fEXlow[i] < xmin) {
         if (gPad && gPad->GetLogx()) {
            if (fEXlow[i] < fX[i]) xmin = fX[i]-fEXlow[i];
            else                   xmin = TMath::Min(xmin,fX[i]/3);
         } else {
            xmin = fX[i]-fEXlow[i];
         }
      }
      if (fX[i] +fEXhigh[i] > xmax) xmax = fX[i]+fEXhigh[i];
      if (fY[i] -fEYlow[i] < ymin) {
         if (gPad && gPad->GetLogy()) {
            if (fEYlow[i] < fY[i]) ymin = fY[i]-fEYlow[i];
            else                   ymin = TMath::Min(ymin,fY[i]/3);
         } else {
            ymin = fY[i]-fEYlow[i];
         }
      }
      if (fY[i] +fEYhigh[i] > ymax) ymax = fY[i]+fEYhigh[i];
   }
}


//______________________________________________________________________________
void TGraphAsymmErrors::CopyAndRelease(Double_t **newarrays,
                                       Int_t ibegin, Int_t iend, Int_t obegin)
{
   // Copy and release.

   CopyPoints(newarrays, ibegin, iend, obegin);
   if (newarrays) {
      delete[] fEXlow;
      fEXlow = newarrays[0];
      delete[] fEXhigh;
      fEXhigh = newarrays[1];
      delete[] fEYlow;
      fEYlow = newarrays[2];
      delete[] fEYhigh;
      fEYhigh = newarrays[3];
      delete[] fX;
      fX = newarrays[4];
      delete[] fY;
      fY = newarrays[5];
      delete[] newarrays;
   }
}


//______________________________________________________________________________
Bool_t TGraphAsymmErrors::CopyPoints(Double_t **arrays,
                                     Int_t ibegin, Int_t iend, Int_t obegin)
{
   // Copy errors from fE*** to arrays[***]
   // or to f*** Copy points.

   if (TGraph::CopyPoints(arrays ? arrays+4 : 0, ibegin, iend, obegin)) {
      Int_t n = (iend - ibegin)*sizeof(Double_t);
      if (arrays) {
         memmove(&arrays[0][obegin], &fEXlow[ibegin], n);
         memmove(&arrays[1][obegin], &fEXhigh[ibegin], n);
         memmove(&arrays[2][obegin], &fEYlow[ibegin], n);
         memmove(&arrays[3][obegin], &fEYhigh[ibegin], n);
      } else {
         memmove(&fEXlow[obegin], &fEXlow[ibegin], n);
         memmove(&fEXhigh[obegin], &fEXhigh[ibegin], n);
         memmove(&fEYlow[obegin], &fEYlow[ibegin], n);
         memmove(&fEYhigh[obegin], &fEYhigh[ibegin], n);
      }
      return kTRUE;
   } else {
      return kFALSE;
   }
}


//______________________________________________________________________________
Bool_t TGraphAsymmErrors::CtorAllocate(void)
{
   // Should be called from ctors after fNpoints has been set

   if (!fNpoints) {
      fEXlow = fEYlow = fEXhigh = fEYhigh = 0;
      return kFALSE;
   }
   fEXlow = new Double_t[fMaxSize];
   fEYlow = new Double_t[fMaxSize];
   fEXhigh = new Double_t[fMaxSize];
   fEYhigh = new Double_t[fMaxSize];
   return kTRUE;
}


//______________________________________________________________________________
void TGraphAsymmErrors::Efficiency(int k, int N, double conflevel,
      double& mode, double& low, double& high) const
{
   // Calculate the shortest central confidence interval containing the required
   // probability content.
   // Interval(low) returns the length of the interval starting at low
   // that contains CONFLEVEL probability. We use Brent's method,
   // except in two special cases: when k=0, or when k=N
   // Main driver routine
   // Author: Marc Paterno

   //If there are no entries, then we know nothing, thus return the prior...
   if (0==N) {
      mode = .5; low = 0.0; high = 1.0;
      return;
   }

   // Calculate the most probable value for the posterior cross section.
   // This is easy, 'cause it is just k/N
   double efficiency = (double)k/N;

   double low_edge;
   double high_edge;

   if (k == 0) {
      low_edge = 0.0;
      high_edge = SearchUpper(low_edge, k, N, conflevel);
   } else if (k == N) {
      high_edge = 1.0;
      low_edge = SearchLower(high_edge, k, N, conflevel);
   } else {
      GLOBAL_k = k;
      GLOBAL_N = N;
      CONFLEVEL = conflevel;
      Brent(0.0, 0.5, 1.0, 1.0e-9, &low_edge);
      high_edge = low_edge + Interval(low_edge);
   }

   // return output
   mode = efficiency;
   low = low_edge;
   high = high_edge;
}


//______________________________________________________________________________
void TGraphAsymmErrors::FillZero(Int_t begin, Int_t end,
                                 Bool_t from_ctor)
{
   // Set zero values for point arrays in the range [begin, end)

   if (!from_ctor) {
      TGraph::FillZero(begin, end, from_ctor);
   }
   Int_t n = (end - begin)*sizeof(Double_t);
   memset(fEXlow + begin, 0, n);
   memset(fEXhigh + begin, 0, n);
   memset(fEYlow + begin, 0, n);
   memset(fEYhigh + begin, 0, n);
}


//______________________________________________________________________________
Double_t TGraphAsymmErrors::GetErrorX(Int_t i) const
{
   // This function is called by GraphFitChisquare.
   // It returns the error along X at point i.

   if (i < 0 || i >= fNpoints) return -1;
   if (!fEXlow && !fEXhigh) return -1;
   Double_t elow=0, ehigh=0;
   if (fEXlow)  elow  = fEXlow[i];
   if (fEXhigh) ehigh = fEXhigh[i];
   return TMath::Sqrt(0.5*(elow*elow + ehigh*ehigh));
}


//______________________________________________________________________________
Double_t TGraphAsymmErrors::GetErrorY(Int_t i) const
{
   // This function is called by GraphFitChisquare.
   // It returns the error along Y at point i.

   if (i < 0 || i >= fNpoints) return -1;
   if (!fEYlow && !fEYhigh) return -1;
   Double_t elow=0, ehigh=0;
   if (fEYlow)  elow  = fEYlow[i];
   if (fEYhigh) ehigh = fEYhigh[i];
   return TMath::Sqrt(0.5*(elow*elow + ehigh*ehigh));
}


//______________________________________________________________________________
Double_t TGraphAsymmErrors::GetErrorXhigh(Int_t i) const
{
   // Get high error on X.

   if (i<0 || i>fNpoints) return -1;
   if (fEXhigh) return fEXhigh[i];
   return -1;
}


//______________________________________________________________________________
Double_t TGraphAsymmErrors::GetErrorXlow(Int_t i) const
{
   // Get low error on X.

   if (i<0 || i>fNpoints) return -1;
   if (fEXlow) return fEXlow[i];
   return -1;
}


//______________________________________________________________________________
Double_t TGraphAsymmErrors::GetErrorYhigh(Int_t i) const
{
   // Get high error on Y.

   if (i<0 || i>fNpoints) return -1;
   if (fEYhigh) return fEYhigh[i];
   return -1;
}


//______________________________________________________________________________
Double_t TGraphAsymmErrors::GetErrorYlow(Int_t i) const
{
   // Get low error on Y.

   if (i<0 || i>fNpoints) return -1;
   if (fEYlow) return fEYlow[i];
   return -1;
}


//______________________________________________________________________________
double TGraphAsymmErrors::Interval(double low) const
{
   // Return the length of the interval starting at low
   // that contains CONFLEVEL of the x^GLOBAL_k*(1-x)^(GLOBAL_N-GLOBAL_k)
   // distribution.
   // If there is no sufficient interval starting at low, we return 2.0

   double high = SearchUpper(low, GLOBAL_k, GLOBAL_N, CONFLEVEL);
   if (high == -1.0) return 2.0; //  so that this won't be the shortest interval
   return (high - low);
}


//______________________________________________________________________________
void TGraphAsymmErrors::Print(Option_t *) const
{
   // Print graph and errors values.

   for (Int_t i=0;i<fNpoints;i++) {
      printf("x[%d]=%g, y[%d]=%g, exl[%d]=%g, exh[%d]=%g, eyl[%d]=%g, eyh[%d]=%g\n"
         ,i,fX[i],i,fY[i],i,fEXlow[i],i,fEXhigh[i],i,fEYlow[i],i,fEYhigh[i]);
   }
}


//______________________________________________________________________________
void TGraphAsymmErrors::SavePrimitive(ostream &out, Option_t *option /*= ""*/)
{
    // Save primitive as a C++ statement(s) on output stream out

   char quote = '"';
   out<<"   "<<endl;
   if (gROOT->ClassSaved(TGraphAsymmErrors::Class())) {
      out<<"   ";
   } else {
      out<<"   TGraphAsymmErrors *";
   }
   out<<"grae = new TGraphAsymmErrors("<<fNpoints<<");"<<endl;
   out<<"   grae->SetName("<<quote<<GetName()<<quote<<");"<<endl;
   out<<"   grae->SetTitle("<<quote<<GetTitle()<<quote<<");"<<endl;

   SaveFillAttributes(out,"grae",0,1001);
   SaveLineAttributes(out,"grae",1,1,1);
   SaveMarkerAttributes(out,"grae",1,1,1);

   for (Int_t i=0;i<fNpoints;i++) {
      out<<"   grae->SetPoint("<<i<<","<<fX[i]<<","<<fY[i]<<");"<<endl;
      out<<"   grae->SetPointError("<<i<<","<<fEXlow[i]<<","<<fEXhigh[i]<<","<<fEYlow[i]<<","<<fEYhigh[i]<<");"<<endl;
   }

   static Int_t frameNumber = 0;
   if (fHistogram) {
      frameNumber++;
      TString hname = fHistogram->GetName();
      hname += frameNumber;
      fHistogram->SetName(hname.Data());
      fHistogram->SavePrimitive(out,"nodraw");
      out<<"   grae->SetHistogram("<<fHistogram->GetName()<<");"<<endl;
      out<<"   "<<endl;
   }

   // save list of functions
   TIter next(fFunctions);
   TObject *obj;
   while ((obj=next())) {
      obj->SavePrimitive(out,"nodraw");
      if (obj->InheritsFrom("TPaveStats")) {
         out<<"   grae->GetListOfFunctions()->Add(ptstats);"<<endl;
         out<<"   ptstats->SetParent(grae->GetListOfFunctions());"<<endl;
      } else {
         out<<"   grae->GetListOfFunctions()->Add("<<obj->GetName()<<");"<<endl;
      }
   }

   const char *l = strstr(option,"multigraph");
   if (l) {
      out<<"   multigraph->Add(grae,"<<quote<<l+10<<quote<<");"<<endl;
   } else {
      out<<"   grae->Draw("<<quote<<option<<quote<<");"<<endl;
   }
}


//______________________________________________________________________________
double TGraphAsymmErrors::SearchLower(double high, int k, int N, double c) const
{
   // Integrates the binomial distribution with
   // parameters k,N, and determines what is the lower edge of the
   // integration region which ends at high, and which contains
   // probability content c. If a lower limit is found, the value is
   // returned. If no solution is found, the -1 is returned.
   // check to see if there is any solution by verifying that the integral down
   // to the minimum lower limit (0) is greater than c

   double integral = Beta_ab(0.0, high, k, N);
   if (integral == c) return 0.0;      // lucky -- this is the solution
   if (integral < c) return -1.0;      // no solution exists
   double too_low = 0.0;               // lower edge estimate
   double too_high = high;
   double test;

   // use a bracket-and-bisect search
   // LM: looping 20 times might be not enough to get an accurate precision.
   // see for example bug https://savannah.cern.ch/bugs/?30246
   // now break loop when difference is less than 1E-15
   // t.b.d: use directly the beta distribution quantile

   for (int loop=0; loop<50; loop++) {
      test = 0.5*(too_high + too_low);
      integral = Beta_ab(test, high, k, N);
      if (integral > c)  too_low = test;
      else too_high = test;
      if ( TMath::Abs(integral - c) <= 1.E-15) break;
   }
   return test;
}


//______________________________________________________________________________
double TGraphAsymmErrors::SearchUpper(double low, int k, int N, double c) const
{
   // Integrates the binomial distribution with
   // parameters k,N, and determines what is the upper edge of the
   // integration region which starts at low which contains probability
   // content c. If an upper limit is found, the value is returned. If no
   // solution is found, -1 is returned.
   // check to see if there is any solution by verifying that the integral up
   // to the maximum upper limit (1) is greater than c

   double integral = Beta_ab(low, 1.0, k, N);
   if (integral == c) return 1.0;    // lucky -- this is the solution
   if (integral < c) return -1.0;    // no solution exists
   double too_high = 1.0;            // upper edge estimate
   double too_low = low;
   double test;

   // use a bracket-and-bisect search
   // LM: looping 20 times might be not enough to get an accurate precision.
   // see for example bug https://savannah.cern.ch/bugs/?30246
   // now break loop when difference is less than 1E-15
   // t.b.d: use directly the beta distribution quantile

   for (int loop=0; loop<50; loop++) {
      test = 0.5*(too_low + too_high);
      integral = Beta_ab(low, test, k, N);
      if (integral > c)  too_high = test;
      else too_low = test;
      if ( TMath::Abs(integral - c) <= 1.E-15) break;
   }
   return test;
}


//______________________________________________________________________________
void TGraphAsymmErrors::SetPointError(Double_t exl, Double_t exh, Double_t eyl, Double_t eyh)
{
   // Set ex and ey values for point pointed by the mouse.

   Int_t px = gPad->GetEventX();
   Int_t py = gPad->GetEventY();

   //localize point to be deleted
   Int_t ipoint = -2;
   Int_t i;
   // start with a small window (in case the mouse is very close to one point)
   for (i=0;i<fNpoints;i++) {
      Int_t dpx = px - gPad->XtoAbsPixel(gPad->XtoPad(fX[i]));
      Int_t dpy = py - gPad->YtoAbsPixel(gPad->YtoPad(fY[i]));
      if (dpx*dpx+dpy*dpy < 25) {ipoint = i; break;}
   }
   if (ipoint == -2) return;

   fEXlow[ipoint]  = exl;
   fEYlow[ipoint]  = eyl;
   fEXhigh[ipoint] = exh;
   fEYhigh[ipoint] = eyh;
   gPad->Modified();
}


//______________________________________________________________________________
void TGraphAsymmErrors::SetPointError(Int_t i, Double_t exl, Double_t exh, Double_t eyl, Double_t eyh)
{
   // Set ex and ey values for point number i.

   if (i < 0) return;
   if (i >= fNpoints) {
   // re-allocate the object
      TGraphAsymmErrors::SetPoint(i,0,0);
   }
   fEXlow[i]  = exl;
   fEYlow[i]  = eyl;
   fEXhigh[i] = exh;
   fEYhigh[i] = eyh;
}


//______________________________________________________________________________
void TGraphAsymmErrors::SetPointEXlow(Int_t i, Double_t exl)
{
   // Set EXlow for point i

   if (i < 0) return;
   if (i >= fNpoints) {
   // re-allocate the object
      TGraphAsymmErrors::SetPoint(i,0,0);
   }
   fEXlow[i]  = exl;
}


//______________________________________________________________________________
void TGraphAsymmErrors::SetPointEXhigh(Int_t i, Double_t exh)
{
   // Set EXhigh for point i

   if (i < 0) return;
   if (i >= fNpoints) {
   // re-allocate the object
      TGraphAsymmErrors::SetPoint(i,0,0);
   }
   fEXhigh[i]  = exh;
}


//______________________________________________________________________________
void TGraphAsymmErrors::SetPointEYlow(Int_t i, Double_t eyl)
{
   // Set EYlow for point i

   if (i < 0) return;
   if (i >= fNpoints) {
   // re-allocate the object
      TGraphAsymmErrors::SetPoint(i,0,0);
   }
   fEYlow[i]  = eyl;
}


//______________________________________________________________________________
void TGraphAsymmErrors::SetPointEYhigh(Int_t i, Double_t eyh)
{
   // Set EYhigh for point i

   if (i < 0) return;
   if (i >= fNpoints) {
   // re-allocate the object
      TGraphAsymmErrors::SetPoint(i,0,0);
   }
   fEYhigh[i]  = eyh;
}


//______________________________________________________________________________
void TGraphAsymmErrors::Streamer(TBuffer &b)
{
   // Stream an object of class TGraphAsymmErrors.

   if (b.IsReading()) {
      UInt_t R__s, R__c;
      Version_t R__v = b.ReadVersion(&R__s, &R__c);
      if (R__v > 2) {
         b.ReadClassBuffer(TGraphAsymmErrors::Class(), this, R__v, R__s, R__c);
         return;
      }
      //====process old versions before automatic schema evolution
      TGraph::Streamer(b);
      fEXlow  = new Double_t[fNpoints];
      fEYlow  = new Double_t[fNpoints];
      fEXhigh = new Double_t[fNpoints];
      fEYhigh = new Double_t[fNpoints];
      if (R__v < 2) {
         Float_t *exlow  = new Float_t[fNpoints];
         Float_t *eylow  = new Float_t[fNpoints];
         Float_t *exhigh = new Float_t[fNpoints];
         Float_t *eyhigh = new Float_t[fNpoints];
         b.ReadFastArray(exlow,fNpoints);
         b.ReadFastArray(eylow,fNpoints);
         b.ReadFastArray(exhigh,fNpoints);
         b.ReadFastArray(eyhigh,fNpoints);
         for (Int_t i=0;i<fNpoints;i++) {
            fEXlow[i]  = exlow[i];
            fEYlow[i]  = eylow[i];
            fEXhigh[i] = exhigh[i];
            fEYhigh[i] = eyhigh[i];
         }
         delete [] eylow;
         delete [] exlow;
         delete [] eyhigh;
         delete [] exhigh;
      } else {
         b.ReadFastArray(fEXlow,fNpoints);
         b.ReadFastArray(fEYlow,fNpoints);
         b.ReadFastArray(fEXhigh,fNpoints);
         b.ReadFastArray(fEYhigh,fNpoints);
      }
      b.CheckByteCount(R__s, R__c, TGraphAsymmErrors::IsA());
      //====end of old versions

   } else {
      b.WriteClassBuffer(TGraphAsymmErrors::Class(),this);
   }
}


//______________________________________________________________________________
void TGraphAsymmErrors::SwapPoints(Int_t pos1, Int_t pos2)
{
   // Swap points.

   SwapValues(fEXlow,  pos1, pos2);
   SwapValues(fEXhigh, pos1, pos2);
   SwapValues(fEYlow,  pos1, pos2);
   SwapValues(fEYhigh, pos1, pos2);
   TGraph::SwapPoints(pos1, pos2);
}
