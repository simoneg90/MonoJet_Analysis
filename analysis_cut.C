//This program makes CUT & COUNT Analysis

#include <iostream>
#include <sstream>

#include "TStopwatch.h"

#include "utility.cc"
#include "cuts.h"

void analysis_cut(){

  bool isData = false;
  double lumi = isData ? 1.0 : 19.7;
  const char* path="/cmshome/gellisim/MonoJet/MonoJet7XTrees/wjets/tree.root"; //path to analyse....maybe better to put it from outside

  TStopwatch time;
  time.Start(true);
  
  std::cout<<jet1ETA<<std::endl;

  std::string test="21";
  std::stringstream str; //string for cuts

  frame("Building the string for cuts!!!");

  str<<K_factor<<" * ";
  str<< (!isData ? "   wgt *" : "")<<"(";  //remember to rescale also for Luminosity and K-factor (1.23 for W-events, 1.27 for Z-events)

  std::string met_cut          = str.str() + "mumet>" + MET;
  std::string jet1_cut         = met_cut + " && signaljetpt>" + jet1PT + " && abs(signaljeteta)<" + jet1ETA + " && signaljetCHfrac>0.2 && signaljetNHfrac<0.7 && signaljetEMfrac<0.7";
  std::string jet2_cut         = jet1_cut + " && secondjetpt>" + jet2PT + " && abs(secondjeteta)<" + jet2ETA + " && secondjetNHfrac<0.7 && secondjetEMfrac<0.9";
  std::string jet_deltaPHI_cut = jet2_cut + " && abs(jetjetdphi)<" + DeltaPHI;
  std::string muon_cut         = jet_deltaPHI_cut + " && nmuons==0";
  std::string elec_cut         = muon_cut + " && nelectrons==0";
  std::string tau_cut          = elec_cut + " && ntaus==0";
  std::string mumet1_cut       = tau_cut + "&& mumet>300";
  std::string mumet2_cut       = tau_cut + "&& mumet>400";
  std::string mumet3_cut       = tau_cut + "&& mumet>500";

  std::cout<<"Prova stringa: "<<jet1_cut<<std::endl;

  std::cout<<"Prova stringa: "<<jet2_cut<<std::endl;

  double yld_noCUT    = yield(path, (str.str()+"1)").c_str(), 5, "tree/tree");
  double yld_MET      = yield(path, (met_cut + ")").c_str(), 5, "tree/tree");
  double yld_jet1     = yield(path, (jet1_cut + ")").c_str(), 5, "tree/tree");
  double yld_jet2     = yield(path, (jet2_cut + ")").c_str(),5,"tree/tree");
  double yld_deltaPhi = yield(path, (jet_deltaPHI_cut + ")").c_str(),5,"tree/tree");
  double yld_jet3     = yield(path, (jet_deltaPHI_cut + "&& njets>2)").c_str(),5,"tree/tree");
  double yld_muon     = yield(path, (muon_cut + ")").c_str(),5,"tree/tree");
  double yld_elec     = yield(path, (elec_cut + ")").c_str(),5,"tree/tree");
  double yld_tau      = yield(path, (tau_cut + ")").c_str(),5,"tree/tree");
  double yld_mumet1   = yield(path, (mumet1_cut + ")").c_str(),5,"tree/tree");
  double yld_mumet2   = yield(path, (mumet2_cut + ")").c_str(),5,"tree/tree");
  double yld_mumet3   = yield(path, (mumet3_cut + ")").c_str(),5,"tree/tree");

  std::cout<<"No cut && "<<yld_noCUT<<std::endl;
  std::cout<<"E_{MISS}^T>"<<MET<<" && "<<yld_MET<<" \\\hline"<<std::endl;
  std::cout<<"1 Jet requirement && "<<yld_jet1<<" \\\hline"<<std::endl;
  std::cout<<"2 Jet requirement && "<<yld_jet2<<" \\\hline"<<std::endl;
  std::cout<<"|Delta\Phi|<"<<DeltaPHI<<" && "<<yld_deltaPhi<<" \\\hline"<<std::endl;
  std::cout<<"Njets>2 &&"<<yld_jet3<<" \\\hline"<<std::endl;
  std::cout<<"\# muons = 0 &&"<<yld_muon<<" \\\hline"<<std::endl;
  std::cout<<"\# electrons = 0 && "<<yld_elec<<" \\\hline"<<std::endl;
  std::cout<<"\#taus = 0 &&"<<yld_tau<<" \\\hline"<<std::endl;
  std::cout<<"E_{MISS}^T>300 && "<<yld_mumet1<<" \\\hline"<<std::endl;
  std::cout<<"E_{MISS}^T>400 && "<<yld_mumet2<<" \\\hline"<<std::endl;
  std::cout<<"E_{MISS}^T>500 && "<<yld_mumet3<<" \\\hline"<<std::endl;

  std::cout<<""<<std::endl;
  std::cout << "------ TIME ELAPSED DURING ANALYSIS  ----- " << time.RealTime() << " s" <<std::endl;
  std::cout<<""<<std::endl;

}

