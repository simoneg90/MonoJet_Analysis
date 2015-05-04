//This program makes CUT & COUNT Analysis

#include <stdio.h>
//#include <unistd.h> library for sleep function
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <time.h>
#include <string>

#include "TStopwatch.h"

#include "utility.cc"
#include "cuts.h"

#define files 6
#define ncuts 12

void analysis_cut_new(){

  bool isData = false;
  double lumi = isData ? 1.0 : 19.7;
  //int cuts[files][ncuts];
  //std::string *string_cuts = new std::string[ncuts];
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
  //str<< (!isData ? " " : "")<<"(";

  std::string met_cut                   = str.str() + "mumet>" + MET;
  //std::string noiseCleaning_cut         = met_cut + " && njets<=2 && signaljetNHfrac < 0.7 && signaljetEMfrac < 0.7 && signaljetCHfrac > 0.2 && (njets < 2 || (secondjetNHfrac < 0.7 && secondjetEMfrac < 0.9))";
  std::string nuova                     = met_cut + "&& njets<=2";
  std::string noiseCleaning_cut         = met_cut + "&& njets<=2 && signaljetNHfrac < 0.7 && signaljetEMfrac < 0.7 && signaljetCHfrac > 0.2";
  std::string jet1_cut                  = noiseCleaning_cut + " && signaljetpt > " + jet1PT + " && abs(signaljeteta) <" + jet1ETA + "&& (njets<2 ||(secondjetpt>30 && abs(secondjeteta)<4.5))";
  //std::string jet2_cut                  = jet1_cut + "&& njets <= 2"; 
  std::string jet_deltaPHI_cut          = jet1_cut + " && (njets == 1 || abs(jetjetdphi)<" + DeltaPHI + "&&secondjetpt>30 && abs(secondjeteta)<4.5 && secondjetNHfrac < 0.7 && secondjetEMfrac < 0.9)";
  std::string muon_cut                  = jet_deltaPHI_cut + " && nmuons==0";
  std::string elec_cut                  = muon_cut + " && nelectronsnew==0";
  std::string tau_cut                   = elec_cut + " && ntaus==0";
  std::string mumet0_cut                = tau_cut + "&& mumet>250";
  std::string mumet1_cut                = tau_cut + "&& mumet>300";
  std::string mumet2_cut                = tau_cut + "&& mumet>400";
  std::string mumet3_cut                = tau_cut + "&& mumet>500";

  //Yield variables definition
  int yld_noCUT[files],yld_MET[files],yld_noise[files],yld_jet1[files],yld_jet2[files],yld_deltaPhi[files],yld_jet3[files],yld_muon[files],yld_elec[files],yld_tau[files],yld_mumet0[files], yld_mumet1[files],yld_mumet2[files],yld_mumet3[files], yld_nuova[files], yld_noCUT_err[files],yld_MET_err[files],yld_noise_err[files],yld_jet1_err[files],yld_jet2_err[files],yld_deltaPhi_err[files],yld_jet3_err[files],yld_muon_err[files],yld_elec_err[files],yld_tau_err[files],yld_mumet0_err[files], yld_mumet1_err[files],yld_mumet2_err[files],yld_mumet3_err[files], yld_nuova_err[files];
  std::pair<double,double> yld_1,yld_2,yld_3,yld_4,yld_5,yld_6,yld_7,yld_8,yld_9,yld_10,yld_11, yld_12,yld_13,yld_14, yld_new;

  for(int i=0; i<param; ++i){
    if (i==0 || i==2) K_Factor= "1.27";  //1.27
    if(i==1) K_Factor="1.23";  //1.23

    frame("Analysis info");
    std::cout<<"File: "<<path[i]<<std::endl;
    std::cout<<"K-Factor: "<<K_Factor<<std::endl;

    int luminosity= 5;
    //str[i]<<K_Factor[i]<<" * ";
    //str[i]<< (!isData ? " wgt *" : "")<<"("; 
    std::pair<double,double>yld_1    = yieldwitherror(path[i], (K_Factor + " * " + str.str()+"1)").c_str(), luminosity, "tree/tree");
    yld_noCUT[i]                     = yld_1.first;
    yld_noCUT_err[i]                 = yld_1.second;
    std::pair<double,double>yld_2    = yieldwitherror(path[i], (K_Factor + " * " + met_cut + ")").c_str(), luminosity, "tree/tree");
    yld_MET[i]                       = yld_2.first;
    yld_MET_err[i]                   = yld_2.second;
    std::pair<double,double>yld_new  = yieldwitherror(path[i],(K_Factor + " * " + nuova + ")").c_str(), luminosity, "tree/tree");
    yld_nuova[i]                     = yld_new.first;
    yld_nuova_err[i]                 = yld_new.second;
    std::pair<double,double>yld_3    = yieldwitherror(path[i], (K_Factor + " * " + noiseCleaning_cut+ ")").c_str(),luminosity,"tree/tree");
    yld_noise[i]                     = yld_3.first;
    yld_noise_err[i]                 = yld_3.second;
    std::pair<double,double>yld_4    = yieldwitherror(path[i], (K_Factor + " * " + jet1_cut + ")").c_str(), luminosity, "tree/tree");
    yld_jet1[i]                      = yld_4.first;
    yld_jet1_err[i]                  = yld_4.second;
    //yld_jet2[i]     = yield(path[i], (K_Factor + " * " +  + jet2_cut + ")").c_str(),5,"tree/tree");
    std::pair<double,double>yld_5    = yieldwitherror(path[i], (K_Factor + " * " +  jet_deltaPHI_cut + ")").c_str(),luminosity,"tree/tree");
    yld_deltaPhi[i]                  = yld_5.first;
    yld_deltaPhi_err[i]              = yld_5.second;
    //yld_jet3[i]     = yield(path[i], (K_Factor + " * " +  jet_deltaPHI_cut + "&& njets>2)").c_str(),5,"tree/tree");
    std::pair<double,double>yld_6    = yieldwitherror(path[i], (K_Factor + " * " +  muon_cut + ")").c_str(),luminosity,"tree/tree");
    yld_muon[i]                      = yld_6.first;
    yld_muon_err[i]                  = yld_6.second;
    std::pair<double,double>yld_7    = yieldwitherror(path[i], (K_Factor + " * " +  elec_cut + ")").c_str(),luminosity,"tree/tree");
    yld_elec[i]                      = yld_7.first;
    yld_elec_err[i]                  = yld_7.second;
    std::pair<double,double>yld_8    = yieldwitherror(path[i], (K_Factor + " * " +  tau_cut + ")").c_str(),luminosity,"tree/tree");
    yld_tau[i]                       = yld_8.first;
    yld_tau_err[i]                   = yld_8.second;
    std::pair<double,double>yld_9    = yieldwitherror(path[i], (K_Factor + " * " +  mumet0_cut + ")").c_str(),luminosity,"tree/tree");
    yld_mumet0[i]                    = yld_9.first;
    yld_mumet0_err[i]                = yld_9.second;
    std::pair<double,double>yld_10   = yieldwitherror(path[i], (K_Factor + " * " +  mumet1_cut + ")").c_str(),5,"tree/tree");
    yld_mumet1[i]                    = yld_10.first;
    yld_mumet1_err[i]                = yld_10.second;
    std::pair<double,double>yld_11   = yieldwitherror(path[i], (K_Factor + " * " +  mumet2_cut + ")").c_str(),5,"tree/tree");
    yld_mumet2[i]                    = yld_11.first;
    yld_mumet2_err[i]                = yld_11.second;
    std::pair<double,double>yld_12   = yieldwitherror(path[i], (K_Factor + " * " +  mumet3_cut + ")").c_str(),5,"tree/tree");
    yld_mumet3[i]                    = yld_12.first;
    yld_mumet3_err[i]                = yld_12.second;
    K_Factor = "1.";
  }
  frame("Beamer instructions!!!");
  std::cout<<"No cut ";
  for(int i=0; i<param; ++i){
    std::cout<<"&" <<yld_noCUT[i]<<"$\\pm$ "<<yld_noCUT_err[i]<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{E_{MISS}^T>}$"<<MET;
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_MET[i]<<"$\\pm$ "<<yld_MET_err[i]<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"N$\\leq$2";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_nuova[i]<<"$\\pm$ "<<yld_nuova_err[i]<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"Noise Cleaning";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_noise[i]<<"$\\pm$ "<<yld_noise_err[i]<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"Jet requirement ";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_jet1[i]<<"$\\pm$ "<<yld_jet1_err[i]<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  /*std::cout<<"2 Jet requirement "; 
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_jet2[i]<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;*/
  std::cout<<"$\\mathrm{|\\Delta\\Phi|<}$"<<DeltaPHI;
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_deltaPhi[i]<<"$\\pm$ "<<yld_deltaPhi_err[i]<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
 /* std::cout<<"Njets$>$2";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_jet3[i]<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;*/
  std::cout<<"$#$ muons = 0";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_muon[i]<<"$\\pm$ "<<yld_muon_err[i]<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$#$ electrons = 0";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_elec[i]<<"$\\pm$ "<<yld_elec_err[i]<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$#$taus = 0";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_tau[i]<<"$\\pm$ "<<yld_tau_err[i]<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{E_{MISS}^T>250}$";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_mumet0[i]<<"$\\pm$ "<<yld_mumet0_err[i]<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{E_{MISS}^T>300}$";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_mumet1[i]<<"$\\pm$ "<<yld_mumet1_err[i]<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{E_{MISS}^T>}$400";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_mumet2[i]<<"$\\pm$ "<<yld_mumet2_err[i]<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{E_{MISS}^T>}$500";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<yld_mumet3[i]<<"$\\pm$ "<<yld_mumet3_err[i]<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;

  frame("Efficiencies!");
  //Efficienze relative
  std::cout<<"No cut ";
  for(int i=0; i<param; ++i){
    std::cout<<"&" <<"-"<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{E_{MISS}^T>}$"<<MET;
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_MET[i],yld_noCUT[i])<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"N$\\leq$2";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_nuova[i],yld_MET[i])<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"Noise Cleaning";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_noise[i],yld_nuova[i])<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"Jet requirement ";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_jet1[i],yld_noise[i])<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{|\\Delta\\Phi|<}$"<<DeltaPHI;
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_deltaPhi[i],yld_jet1[i])<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"$#$ muons = 0";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_muon[i],yld_deltaPhi[i])<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$#$ electrons = 0";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_elec[i],yld_muon[i])<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$#$taus = 0";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_tau[i],yld_elec[i])<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{E_{MISS}^T>250}$";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_mumet0[i],yld_tau[i])<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{E_{MISS}^T>300}$";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_mumet1[i],yld_mumet0[i])<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{E_{MISS}^T>}$400";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_mumet2[i],yld_mumet1[i])<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{E_{MISS}^T>}$500";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_mumet3[i],yld_mumet2[i])<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;

  frame("Total Efficiencies");

  //Efficienze totali
 
  std::cout<<"No cut ";
  for(int i=0; i<param; ++i){
    std::cout<<"&" <<"-"<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{E_{MISS}^T>}$"<<MET;
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_MET[i],yld_noCUT[i])<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"N$\\leq$2";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_nuova[i],yld_noCUT[i])<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"Noise Cleaning";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_noise[i],yld_noCUT[i])<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"Jet requirement ";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_jet1[i],yld_noCUT[i])<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{|\\Delta\\Phi|<}$"<<DeltaPHI;
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_deltaPhi[i],yld_noCUT[i])<<" ";
  }
  std::cout<<"\\\\\\hline"<<std::endl;
  std::cout<<"$#$ muons = 0";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_muon[i],yld_noCUT[i])<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$#$ electrons = 0";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_elec[i],yld_noCUT[i])<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$#$taus = 0";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_tau[i],yld_noCUT[i])<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{E_{MISS}^T>250}$";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_mumet0[i],yld_noCUT[i])<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{E_{MISS}^T>300}$";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_mumet1[i],yld_noCUT[i])<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{E_{MISS}^T>}$400";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_mumet2[i],yld_noCUT[i])<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;
  std::cout<<"$\\mathrm{E_{MISS}^T>}$500";
  for(int i=0; i<param; ++i){
    std::cout<<"& "<<Ratio(yld_mumet3[i],yld_noCUT[i])<<" ";
  }
  std::cout<<" \\\\\\hline"<<std::endl;


  std::cout<<""<<std::endl;
  std::cout << "------ TIME ELAPSED DURING ANALYSIS  ----- " << time.RealTime() << " s" <<std::endl;
  std::cout<<""<<std::endl;

}

