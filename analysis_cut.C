//This program makes CUT & COUNT Analysis

#include <iostream>
#include <sstream>

#include "utility.cc"
#include "cuts.h"

void analysis_cut(){

  bool isData = false;
  double lumi = isData ? 1.0 : 19.7;
  const char* path="/cmshome/gellisim/MonoJet/MonoJet7XTrees/znunu/tree.root"; //path to analyse....maybe better to put it from outside


  std::cout<<jet1ETA<<std::endl;

  std::string test="21";
  std::stringstream str; //string for cuts

  frame("Building the string for cuts!!!");

  str<<K_factor<<" * ";
  str<< (!isData ? "   wgt *" : "")<<"(";  //remember to rescale also for Luminosity and K-factor (1.23 for W-events, 1.27 for Z-events)

  std::string met_cut = str.str() + "mumet>" + MET + ")";
  std::cout<<met_cut<<std::endl;
  std::string jet_cut = met_cut + "&& njets<=2";
  std::cout<<jet_cut<<std::endl;
  std::string jet1_cut = str.str() + "signaljetpt>" + jet1PT + " && abs(signaljeteta)<" + jet1ETA + " && signaljetCHfrac>0.2 && signaljetNHfrac<0.7 && signaljetEMfrac<0.7";

  std::cout<<"Prova stringa: "<<jet1_cut<<std::endl;


  std::string jet2_cut = jet1_cut + " && secondjetpt>" + jet2PT + "&& abs(secondjeteta)<" + jet2ETA + " && secondjetNHfrac<0.7 && secondjetEMfrac<0.9";

  std::cout<<"Prova stringa: "<<jet2_cut<<std::endl;

  double yld_noCUT= yield(path, (str.str()+"1)").c_str(), lumi, "tree/tree");
  double yld_MET= yield(path, met_cut.c_str(), 5, "tree/tree");
  double yld_NJETS=yield(path, jet_cut.c_str(), 5, "tree/tree");
  double yld_jet1= yield(path, (jet1_cut + ")").c_str(), 5, "tree/tree");
  double yld_jet2= yield(path, (jet2_cut + "&& njets==2)").c_str(),5,"tree/tree");
  double yld_jet3= yield(path, (jet2_cut + "&& njets==3)").c_str(),5,"tree/tree");

  std::cout<<yld_noCUT<<std::endl;
  std::cout<<"E_{MISS}^T>"<<MET<<" && "<<yld_MET<<" \\\hline"<<std::endl;
  std::cout<<yld_jet1<<std::endl;
  std::cout<<yld_jet2<<std::endl;
  std::cout<<yld_jet3<<std::endl;
  std::cout<<yld_NJETS<<std::endl;


}

