//This program makes CUT & COUNT Analysis

#include <stdio.h>
//#include <unistd.h> library for sleep function
#include <iostream>
#include <iomanip>
#include <sstream>
#include <time.h>

#include "TStopwatch.h"

#include "utility.cc"
#include "cuts.h"

#define files 6

void analysis_cut(){

  bool isData = false;
  double lumi = isData ? 1.0 : 19.7;
  std::string choice;
  int param;
  const char* path[files];
  path[0] = "/cmshome/gellisim/MonoJet/MonoJet7XTrees/znunu/tree.root"; //path to analyse....maybe better to put it from outside
  path[1] = "/cmshome/gellisim/MonoJet/MonoJet7XTrees/wjets/tree.root";
  path[2] = "/cmshome/gellisim/MonoJet/MonoJet7XTrees/zjets/tree.root";
  path[3] = "/cmshome/gellisim/MonoJet/MonoJet7XTrees/ttbar/tree.root";
  path[4] = "/cmshome/gellisim/MonoJet/MonoJet7XTrees/qcd/tree.root";
  path[5] = "/cmshome/gellisim/MonoJet/MonoJet7XTrees/dmVM10/tree.root";

  TStopwatch time;
  time.Start(true);
  frame("Choose the dataset to use");
  std::cout<<"Do you want to use only the first path? [y/n]   ";
  std::cin>>choice;

  while(strcmp(choice.c_str(), "y")!=0 && strcmp(choice.c_str(),"n")!=0){

    std::cout<<"wrong answer! Try again!"<<std::endl;
    std::cout<<"Do you want to use only the first path? [y/n]"<<std::endl;
  }

  if(strcmp(choice.c_str(),"y")==0){
    std::cout<<"Ok, I'll check only the first dataset!"<<std::endl;
    param=1;
  }else{
    std::cout<<"Ok, I'll check all the datasets!"<<std::endl;
    param=files;
  }

  std::stringstream str;  //string for cuts

  frame("Building the string for cuts!!!");

  std::string K_Factor = "1.";
  //str<<K_factor<<" * ";
  str<< (!isData ? " wgt *" : "")<<"(";  //remember to rescale also for Luminosity and K-factor (1.23 for W-events, 1.27 for Z-events)

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

  //Yield variables definition
  int yld_noCUT[5],yld_MET[5],yld_jet1[5],yld_jet2[5],yld_deltaPhi[5],yld_jet3[5],yld_muon[5],yld_elec[5],yld_tau[5],yld_mumet1[5],yld_mumet2[5],yld_mumet3[5];

  for(int i=0; i<param; ++i){
    if (i==0 || i==2) K_Factor= "1.27";
    if(i==1) K_Factor="1.23";

    frame("Analysis info");
    std::cout<<"File: "<<path[i]<<std::endl;
    std::cout<<"K-Factor: "<<K_Factor<<std::endl;

    //str[i]<<K_Factor[i]<<" * ";
    //str[i]<< (!isData ? " wgt *" : "")<<"("; 
    yld_noCUT[i]    = yield(path[i], (K_Factor + " * " + str.str()+"1)").c_str(), 5, "tree/tree");
    yld_MET[i]      = yield(path[i], (K_Factor + " * " + met_cut + ")").c_str(), 5, "tree/tree");
    yld_jet1[i]     = yield(path[i], (K_Factor + " * " + jet1_cut + ")").c_str(), 5, "tree/tree");
    yld_jet2[i]     = yield(path[i], (K_Factor + " * " +  + jet2_cut + ")").c_str(),5,"tree/tree");
    yld_deltaPhi[i] = yield(path[i], (K_Factor + " * " +  jet_deltaPHI_cut + ")").c_str(),5,"tree/tree");
    yld_jet3[i]     = yield(path[i], (K_Factor + " * " +  jet_deltaPHI_cut + "&& njets>2)").c_str(),5,"tree/tree");
    yld_muon[i]     = yield(path[i], (K_Factor + " * " +  muon_cut + ")").c_str(),5,"tree/tree");
    yld_elec[i]     = yield(path[i], (K_Factor + " * " +  elec_cut + ")").c_str(),5,"tree/tree");
    yld_tau[i]      = yield(path[i], (K_Factor + " * " +  tau_cut + ")").c_str(),5,"tree/tree");
    yld_mumet1[i]   = yield(path[i], (K_Factor + " * " +  mumet1_cut + ")").c_str(),5,"tree/tree");
    yld_mumet2[i]   = yield(path[i], (K_Factor + " * " +  mumet2_cut + ")").c_str(),5,"tree/tree");
    yld_mumet3[i]   = yield(path[i], (K_Factor + " * " +  mumet3_cut + ")").c_str(),5,"tree/tree");
    K_Factor = "1.";
  }
  frame("Beamer instructions!!!");
  std::cout<<"No cut ";
  for(int i=0; i<param; ++i){
    std::cout<<"&" <<yld_noCUT[i]<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{E_{MISS}^T>}$"<<MET;
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_MET[i]<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"1 Jet requirement ";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_jet1[i]<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"2 Jet requirement "; 
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_jet2[i]<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{|\\Delta\\Phi|<}$"<<DeltaPHI;
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_deltaPhi[i]<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"Njets$>$2";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_jet3[i]<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$#$ muons = 0";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_muon[i]<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$#$ electrons = 0";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_elec[i]<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$#$taus = 0";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_tau[i]<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{E_{MISS}^T>300}$";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_mumet1[i]<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{E_{MISS}^T>}$400";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_mumet2[i]<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{E_{MISS}^T>}$500";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_mumet3[i]<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;

  std::cout<<""<<std::endl;
  std::cout << "------ TIME ELAPSED DURING ANALYSIS  ----- " << time.RealTime() << " s" <<std::endl;
  std::cout<<""<<std::endl;

}

