//Questo programma contiene funzioni utili all'esecuzione di analisi

#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "TH1D.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TCanvas.h"

//Questa funzione serve per creare spazio tra una stampa ed un'altra
void breakLine(){

  std::cout<<""<<std::endl;
  std::cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;
  std::cout<<""<<std::endl;

}

//Questa funzione implementa le caratteristiche di un istogramma TH1D
void TH1D_config(TH1D *histo, const char* title, const char* title_x, const char* title_y, int color){

  std::string x="";
  x+=title_x;

  if(strcmp(x.c_str(),"Eta")==0)x="#eta";
  if(strcmp(x.c_str(),"Phi")==0)x="#Phi";
  if(strcmp(x.c_str(),"Pt")==0)x="Pt [GeV]";
  histo->SetTitle(title);
  histo->GetXaxis()->SetTitle(x.c_str());
  histo->GetYaxis()->SetTitle(title_y);
  histo->SetLineColor(color);
  histo->SetMarkerColor(color);

}

//Questa funzione implementa le caratteristiche di un istogramma TH1F
void TH1F_config(TH1F *histo, const char* title, const char* title_x, const char* title_y, int color){
 
  std::string x="";

  if(strcmp(x.c_str(),"Eta")==0)x="#eta";
  if(strcmp(x.c_str(),"Phi")==0)x="#Phi";
  if(strcmp(x.c_str(),"Pt")==0)x="Pt [GeV]";
  histo->SetTitle(title);
  histo->GetXaxis()->SetTitle(x.c_str());
  histo->GetYaxis()->SetTitle(title_y);
  histo->SetLineColor(color);
  histo->SetMarkerColor(color);
  
}

//Questa funzione implementa le caratteristiche di un istogramma TH2D
void TH2D_config(TH2D *histo, const char* title, const char* title_x, const char* title_y, int color){
 
  histo->SetTitle(title);
  histo->GetXaxis()->SetTitle(title_x);
  histo->GetYaxis()->SetTitle(title_y);
  histo->SetLineColor(color);
  histo->SetMarkerColor(color);

}

//Questa funzione implementa le caratteristiche di un istogramma TH2F
void TH2F_config(TH2F *histo, const char* title, const char* title_x, const char* title_y, int color){
    
  histo->SetTitle(title);
  histo->GetXaxis()->SetTitle(title_x);
  histo->GetYaxis()->SetTitle(title_y);
  histo->SetLineColor(color);
  histo->SetMarkerColor(color);

}

//Questa funzione analizza gli istogrammi all'interno di un file.root e li scrive su un file di output
void LS_config(std::string rootFile, std::string outfile){

  std::cout<<"Removing old config file: "<<outfile.c_str()<<std::endl;
  system(Form("rm -rf %s",outfile.c_str()));
  std::cout<<"Root File analyzed: "<<rootFile.c_str()<<std::endl;
  system(Form("./ls_script.sh %s | tee %s",rootFile.c_str(),outfile.c_str()));

}
