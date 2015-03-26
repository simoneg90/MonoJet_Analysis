//This program makes CUT & COUNT Analysis

#include <iostream>
#include <sstream>

#include "utility.cc"
#include "cuts.h"
#include "yield.h"

void analysis_cut(){

  bool isData = false;
  double lumi = isData ? 1.0 : 19.7;
  const char* path="/cmshome/gellisim/MonoJet/MonoJet7XTrees/dmVM10/tree.root"; //path to analyse....maybe better to put it from outside


  std::cout<<jet1ETA<<std::endl;

  std::string test="21";
  std::stringstream str; //string for cuts

  breakLine();
  std::cout<<"Building the string for cuts!!!"<<std::endl;
  breakLine();

  str<< (!isData ? "puwgt * wgt *" : "")<<"(";  //remember to rescale also for Luminosity and K-factor (1.23 for W-events, 1.27 for Z-events)

  std::string jet1_cut= str.str() + "njets==1 && signaljetpt>" + jet1PT + " && abs(signaljeteta)<" + jet1ETA + " && signaljetCHfrac>0.2 && signaljetNHfrac<0.7 && signaljetEMfrac<0.7)";

  std::cout<<"Prova stringa: "<<jet1_cut<<std::endl;

  std::pair<double,double> yld3  = yieldwitherror(path, jet1_cut.c_str(), lumi, 1, "tree/tree");

  std::cout<<yld3.first<<" "<<yld3.second<<std::endl;









}

