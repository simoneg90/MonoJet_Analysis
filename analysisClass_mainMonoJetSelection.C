#define analysisClass_cxx
#include "analysisClass.h"
#include <TH2.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <TVector3.h>
#include "TStopwatch.h"

analysisClass::analysisClass(string * inputList, string * cutFile, string * treeName, string * outputFileName, string * cutEfficFile)
  :baseClass(inputList, cutFile, treeName, outputFileName, cutEfficFile)
{
  std::cout << "analysisClass::analysisClass(): begins " << std::endl;

  std::cout << "analysisClass::analysisClass(): ends " << std::endl;
}

analysisClass::~analysisClass()
{
  std::cout << "analysisClass::~analysisClass(): begins " << std::endl;

  std::cout << "analysisClass::~analysisClass(): ends " << std::endl;
}

void analysisClass::Loop()
{
   std::cout << "analysisClass::Loop() begins" <<std::endl;   

   TStopwatch time;
   time.Start(true);
    
   if (fChain == 0) return;
   
   //////////book histos here

   // TH1F *h_nJetFinal = new TH1F ("h_nJetFinal","",10,0,10);
   // h_nJetFinal->Sumw2();      
   // TH1F *h_nVtx = new TH1F ("h_nVtx","",30,0,30);
   // h_nVtx->Sumw2(); 
   // TH1F *h_trueVtx = new TH1F ("h_trueVtx","",40,0,40);
   // h_trueVtx->Sumw2();  
   // TH1F *h_pT1stJet = new TH1F ("h_pT1stJet","",100,0,3000);
   // h_pT1stJet->Sumw2();
   // TH1F *h_pT2ndJet = new TH1F ("h_pT2ndJet","",100,0,3000);
   // h_pT2ndJet->Sumw2();
   // TH1F *h_eta1stJet = new TH1F ("h_eta1stJet","",5,-2.5,2.5);
   // h_eta1stJet->Sumw2();
   // TH1F *h_eta2ndJet = new TH1F ("h_eta2ndJet","",5,-2.5,2.5);
   // h_eta2ndJet->Sumw2();
   // TH1F *h_DijetMass = new TH1F ("h_DijetMass","",600,0,6000);
   // h_DijetMass->Sumw2();
   // TH1F *h_DeltaETAjj = new TH1F ("h_DeltaETAjj","",120,0,3.);
   // h_DeltaETAjj->Sumw2();

   /////////initialize variables

   Long64_t nentries = fChain->GetEntriesFast();

   std::cout<<"++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;
   std::cout << "analysisClass::Loop(): nentries = " << nentries << std::endl;
   std::cout<<"++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;

   double R_isr1, R_isr2, R_ISR, R_ISR_primo;
   double pft1_dphi,pf_dphi;
   int MET_range, isr1_number,isr2_number, vertex_range;
   int met[]={250,500,1000};
   
   isr1_number=isr2_number=vertex_range=0;

   ////// The following ~7 lines have been taken from rootNtupleClass->Loop() /////
   ////// If the root version is updated and rootNtupleClass regenerated,     /////
   ////// these lines may need to be updated.                                 /////    
   Long64_t nbytes = 0, nb = 0;
   //for (Long64_t jentry=0; jentry<nentries;jentry++) {
   for (Long64_t jentry=0; jentry<185000;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     if(jentry < 10 || jentry%1000 == 0) std::cout << "analysisClass::Loop(): jentry = " << jentry << std::endl;   
     // if (Cut(ientry) < 0) continue;

     ////////////////////// User's code starts here ///////////////////////

     ///Stuff to be done for every event
 
     resetCuts();
   
     // if(no_jets_ak4>=2){
     //  if(!(fabs(jetEtaAK4->at(0)) < getPreCutValue1("jetFidRegion") && idTAK4->at(0) == getPreCutValue1("tightJetID"))){
     //    std::cout << " JET 0 FAIL " << jetEtaAK4->at(0) << " JET 0  ID " << idTAK4->at(0) << std::endl;
     //  }
     //  if(!(fabs(jetEtaAK4->at(1)) < getPreCutValue1("jetFidRegion") && idTAK4->at(1) == getPreCutValue1("tightJetID"))){
     //    std::cout << " JET 1 FAIL " << jetEtaAK4->at(1) << " JET 1  ID " << idTAK4->at(1) << std::endl;
     //  }  
     // }

     //== Fill Variables == (on which cuts are applied)

     // fillVariableWithValue("Calo_met",calomet);
    
     fillVariableWithValue("leadingJetpt",signaljetpt);
     fillVariableWithValue("leadingJeteta",signaljeteta);
     fillVariableWithValue("PFMET",pfmet);
     fillVariableWithValue("t1corr",t1pfmet); //type-1 correction for PFMET
     fillVariableWithValue("met_jet1dphi",signaljetmetdphi);     
     

     //== Creating and filling TVectors for DM_particle and jet1,jet2,jet3 particles (DM=gen_level; jet=reco_level) ==

     TLorentzVector dm1, dm2, jet1, jet2, jet3, isr1, isr2, pfmet_vector, pfmett1_vector;
     dm1.SetPtEtaPhiM(l1pt,l1eta,l1phi,10);
     dm2.SetPtEtaPhiM(l2pt,l2eta,l2phi,10);

     pfmet_vector.SetPtEtaPhiM(pfmet,1,pfmetphi,1);
     pfmett1_vector.SetPtEtaPhiM(t1pfmet,1,t1pfmetphi,1);

     jet1.SetPtEtaPhiM(signaljetpt, signaljeteta, signaljetphi, 0);
     jet2.SetPtEtaPhiM(secondjetpt, secondjeteta, secondjetphi, 0);
     jet3.SetPtEtaPhiM(thirdjetpt, thirdjeteta, thirdjetphi, 0);

     isr1.SetPtEtaPhiM(i1pt,i1eta,i1phi,0);
     isr2.SetPtEtaPhiM(i2pt,i2eta,i2phi,0);

     TLorentzVector DM, Jet, ISR, JET;
     DM=dm1+dm2;
     Jet=jet1+jet2;
     ISR=isr1+isr2;
     JET=jet1+jet2+jet3;


     //== Filling histos ==


     // == DM info == (gen_level)

     CreateAndFillUserTH1D("Eta_DM_system",100,-10.,10.,DM.Eta());
     CreateAndFillUserTH1D("Phi_DM_system",30,-3,3,DM.Phi());
     CreateAndFillUserTH1D("Pt_DM_system",1000,-1,2000,DM.Pt());
    // CreateAndFillUserTH1D("DM_jet1Eta",40,-10.,10.,(DM.Eta()-jet1.Eta())/jet1.Eta());
    // CreateAndFillUserTH1D("DM_jet1Phi",40,-5.,5.,(DM.Phi()-jet1.Phi()));
     //CreateAndFillUserTH1D("DM_PF_Eta",50,-10.,10.,(pfmeteta-DM.Eta())/DM.Eta()); PF_Eta non è riportato!!!

     pf_dphi=DM.Phi()-pfmetphi;    //DM_system - PFmet (Phi) :: Gen_level - RECO_level
     pft1_dphi=DM.Phi()-t1pfmetphi;//DM_system - PFmet_t1 (Phi) :: Gen_level - RECO_level

     if(pf_dphi>3.14){
       pf_dphi-=6.28;
     }
     if(pf_dphi<-3.14){
       pf_dphi+=6.28;
     }
     if(pft1_dphi>3.14){
       pft1_dphi-=6.28;
     }
     if(pft1_dphi<-3.14){
       pft1_dphi+=6.28;
     }

     //== PFMET info == (RECO_level)

     //CreateAndFillUserTH1D("PFMET_Eta",100,-10.,10.,pfmeteta);  //Non c'e' (forse non calcolabile...)
     CreateAndFillUserTH1D("Phi_PFMET",30,-3,3,pfmetphi);
     CreateAndFillUserTH1D("Pt_PFMET",1000,-1,2000,pfmet);

     //== PFMET-t1 info == (RECO_level)
          
     //CreateAndFillUserTH1D("PFMETt1_Eta",100,-10.,10.,t1pfmeteta); //Non c'e' (forse non calcolabile...)
     CreateAndFillUserTH1D("Phi_PFMETt1",30,-3,3,t1pfmetphi);
     CreateAndFillUserTH1D("Pt_PFMETt1",1000,-1,2000,t1pfmet);

     //== Filling histos :: Gen_level - RECO_level == (DM part) 

     CreateAndFillUserTH1D("Phi_DM_PF",100,0.,3.2,abs(pf_dphi));
     CreateAndFillUserTH1D("Phi_DM_PFt1",100,0.,3.2,abs(pft1_dphi));
     CreateAndFillUserTH2D("Pt_DM_PF_2D",2000,0.,1000.,200,-2.,20.,DM.Pt(),(pfmet-DM.Pt())/DM.Pt());
     CreateAndFillUserTH1D("Pt_DM_PF_1D", 200, -2.,20, (pfmet-DM.Pt())/DM.Pt());
     CreateAndFillUserTH2D("Pt_DM_PFt1_2D",500,0.,1000.,200,-2.,20.,DM.Pt(),(t1pfmet-DM.Pt())/DM.Pt());
     CreateAndFillUserTH1D("Pt_DM_PFt1_1D", 200, -2.,20, (t1pfmet-DM.Pt())/DM.Pt());

     //== RECO_level - Gen_level components == (DM part)

     CreateAndFillUserTH1D("Pt_DM_PF_ortogonal_tot",120,-600,600,(pfmet*sin(DM.DeltaPhi(pfmet_vector))));  
     CreateAndFillUserTH1D("Pt_DM_PFt1_ortogonal_tot",120,-600,600,(t1pfmet*sin(DM.DeltaPhi(pfmett1_vector))));
     CreateAndFillUserTH1D("Pt_DM_PF_parallel_tot",100,-5,5,(pfmet*(cos(DM.DeltaPhi(pfmet_vector)))-DM.Pt())/DM.Pt());
     CreateAndFillUserTH1D("Pt_DM_PFt1_parallel_tot",100,-5,5,(t1pfmet*cos(DM.DeltaPhi(pfmett1_vector))-DM.Pt())/DM.Pt());
     CreateAndFillUserTH2D("Pt_DM_PF_ortogonal_tot_2D",1000,-1,2000,120,-600,600,DM.Pt(),(pfmet*sin(DM.DeltaPhi(pfmet_vector))));
     CreateAndFillUserTH2D("Pt_DM_PF_parallel_tot_2D",1000,-1,2000,100,-5,5,DM.Pt(),(pfmet*cos(DM.DeltaPhi(pfmet_vector))-DM.Pt())/DM.Pt());
     CreateAndFillUserTH2D("Coseno_PF_parallel_tot_2D",1000,-1,2000,200,-2,2,DM.Pt(),cos(DM.DeltaPhi(pfmet_vector)));
     CreateAndFillUserTH2D("PFMet_par_vs_DM",1000,-1,2000,2000,-2000,2000,DM.Pt(),(pfmet*cos(DM.DeltaPhi(pfmet_vector))));
     
     if(DM.Pt()<met[0]){
       CreateAndFillUserTH1D("Pt_parallel_rangeIn_tot",300,-5,5,(pfmet*(cos(DM.DeltaPhi(pfmet_vector)))-DM.Pt())/DM.Pt());
       CreateAndFillUserTH1D("Pt_t1_parallel_rangeIn_tot",300,-5,5,(t1pfmet*(cos(DM.DeltaPhi(pfmett1_vector)))-DM.Pt())/DM.Pt());
       CreateAndFillUserTH1D("Pt_ortogonal_rangeIn_tot",120,-600,600,(pfmet*sin(DM.DeltaPhi(pfmet_vector))));
       CreateAndFillUserTH1D("Pt_t1_ortogonal_rangeIn_tot",120,-600,600,(t1pfmet*sin(DM.DeltaPhi(pfmett1_vector))));
     }
     for(int y=0;y<3;++y){
       if(DM.Pt()>met[y]&&DM.Pt()<met[y+1]&&y!=2){
         CreateAndFillUserTH1D(Form("Pt_parallel_range%d_tot",y),300,-5,5,(pfmet*(cos(DM.DeltaPhi(pfmet_vector)))-DM.Pt())/DM.Pt());
         CreateAndFillUserTH1D(Form("Pt_ortogonal_range%d_tot",y),120,-600,600,(pfmet*sin(DM.DeltaPhi(pfmet_vector))));
         CreateAndFillUserTH1D(Form("Pt_t1_parallel_range%d_tot",y),300,-5,5,(t1pfmet*(cos(DM.DeltaPhi(pfmett1_vector)))-DM.Pt())/DM.Pt());
         CreateAndFillUserTH1D(Form("Pt_t1_ortogonal_range%d_tot",y),120,-600,600,(t1pfmet*sin(DM.DeltaPhi(pfmett1_vector))));
       }
       if(y==2 && DM.Pt()>met[y]){
         CreateAndFillUserTH1D("Pt_parallel_rangeLast_tot",300,-5,5,(pfmet*(cos(DM.DeltaPhi(pfmet_vector)))-DM.Pt())/DM.Pt());
         CreateAndFillUserTH1D("Pt_ortogonal_rangeLast_tot",120,-600,600,(pfmet*sin(DM.DeltaPhi(pfmet_vector))));
         CreateAndFillUserTH1D("Pt_t1_parallel_rangeLast_tot",300,-5,5,(t1pfmet*(cos(DM.DeltaPhi(pfmett1_vector)))-DM.Pt())/DM.Pt());
         CreateAndFillUserTH1D("Pt_t1_ortogonal_rangeLast_tot",120,-600,600,(t1pfmet*sin(DM.DeltaPhi(pfmett1_vector))));

       }
     }
     if(DM.Pt()>250){
       CreateAndFillUserTH1D("Pt_DM_PF_parallel_tot_prova",100,-5,5,(pfmet*cos(DM.DeltaPhi(pfmet_vector))-DM.Pt())/DM.Pt());
       CreateAndFillUserTH1D("Pt_DM_PFt1_parallel_tot_prova",100,-5,5,(t1pfmet*cos(DM.DeltaPhi(pfmett1_vector))-DM.Pt())/DM.Pt());
       CreateAndFillUserTH1D("Pt_DM_PF_ortogonal_tot_prova",120,-600,600,(pfmet*sin(DM.DeltaPhi(pfmet_vector))));
       CreateAndFillUserTH1D("Pt_DM_PFt1_ortogonal_tot_prova",120,-600,600,(t1pfmet*sin(DM.DeltaPhi(pfmett1_vector))));
       CreateAndFillUserTH2D("Pt_DM_PF_ortogonal_tot_prova_2D",1000,-1,2000,120,-600,600,DM.Pt(),(pfmet*sin(DM.DeltaPhi(pfmet_vector))));
       CreateAndFillUserTH2D("Pt_DM_PF_parallel_tot_prova_2D",1000,-1,2000,100,-5,5,DM.Pt(),(pfmet*cos(DM.DeltaPhi(pfmet_vector))-DM.Pt())/DM.Pt());
     }

     //== Analysis for different number of ISR and different MET_gen bins ==
    
     //=== TEST->DM.PT for events with isr1Pt<70GeV

     if(DM.Pt()<100){ 
       CreateAndFillUserTH1D("ISR1_test",1000,0,2000,i1pt);
       CreateAndFillUserTH1D("ISR2_test",1000,0,2000,i2pt);
     }

     //== TEST ended ==

     if(i1pt>30 && i1eta<2.5 && i1eta>-2.5){   //ISR1 condition
    
       if(i2pt>30 && i2eta<2.5 && i2eta>-2.5){  //ISR2 condition => 2 ISR particles
         ++isr2_number;
         for(int y=0;y<3;++y){  //MET bins
           
           if(DM.Pt()<250){
             CreateAndFillUserTH1D("Pt_parallel_rangeIn_2isr",300,-5,5,(pfmet*(cos(DM.DeltaPhi(pfmet_vector)))-DM.Pt())/DM.Pt());
             CreateAndFillUserTH1D("Pt_ortogonal_rangeIn_2isr",120,-600,600,(pfmet*sin(DM.DeltaPhi(pfmet_vector))));
             CreateAndFillUserTH1D("Pt_t1_parallel_rangeIn_2isr",300,-5,5,(t1pfmet*(cos(DM.DeltaPhi(pfmett1_vector)))-DM.Pt())/DM.Pt());
             CreateAndFillUserTH1D("Pt_t1_ortogonal_rangeIn_2isr",120,-600,600,(t1pfmet*sin(DM.DeltaPhi(pfmett1_vector))));
           }
           if(DM.Pt()>met[y]&&DM.Pt()<met[y+1]&&y!=2){
             CreateAndFillUserTH1D(Form("Pt_parallel_range%d_2isr",y),300,-5,5,(pfmet*(cos(DM.DeltaPhi(pfmet_vector)))-DM.Pt())/DM.Pt());
             CreateAndFillUserTH1D(Form("Pt_ortogonal_range%d_2isr",y),120,-600,600,(pfmet*sin(DM.DeltaPhi(pfmet_vector))));
             CreateAndFillUserTH1D(Form("Pt_t1_parallel_range%d_2isr",y),300,-5,5,(t1pfmet*(cos(DM.DeltaPhi(pfmett1_vector)))-DM.Pt())/DM.Pt());
             CreateAndFillUserTH1D(Form("Pt_t1_ortogonal_range%d_2isr",y),120,-600,600,(t1pfmet*sin(DM.DeltaPhi(pfmett1_vector))));
             
           }
           if(y==2 && DM.Pt()>met[y]){
              
             CreateAndFillUserTH1D("Pt_parallel_rangeLast_2isr",300,-5,5,(pfmet*(cos(DM.DeltaPhi(pfmet_vector)))-DM.Pt())/DM.Pt());
             CreateAndFillUserTH1D("Pt_ortogonal_rangeLast_2isr",120,-600,600,(pfmet*sin(DM.DeltaPhi(pfmet_vector))));
             CreateAndFillUserTH1D("Pt_t1_parallel_rangeLast_2isr",300,-5,5,(t1pfmet*(cos(DM.DeltaPhi(pfmett1_vector)))-DM.Pt())/DM.Pt());
             CreateAndFillUserTH1D("Pt_t1_ortogonal_rangeLast_2isr",120,-600,600,(t1pfmet*sin(DM.DeltaPhi(pfmett1_vector))));
            
           }
       
         }//end loop over MET bins
         //== Analysis for 2 ISR and different N_vertex bins ==
         vertex_range=5;
         if(nvtx>11 && nvtx<19 && DM.Pt()>250){
           CreateAndFillUserTH1D("Pt_parallel_2isr_vertex_11_19",300,-5,5,(pfmet*(cos(DM.DeltaPhi(pfmet_vector)))-DM.Pt())/DM.Pt());
           CreateAndFillUserTH1D("Pt_ortogonal_2isr_vertex_11_19",120,-600,600,(pfmet*sin(DM.DeltaPhi(pfmet_vector))));
           CreateAndFillUserTH1D("Pt_t1_parallel_2isr_vertex_11_19",300,-5,5,(t1pfmet*(cos(DM.DeltaPhi(pfmett1_vector)))-DM.Pt())/DM.Pt());
           CreateAndFillUserTH1D("Pt_t1_ortogonal_2isr_vertex_11_19",120,-600,600,(t1pfmet*sin(DM.DeltaPhi(pfmett1_vector))));
         }
         if(nvtx<11 && DM.Pt()>250){
           CreateAndFillUserTH1D("Pt_parallel_2isr_vertex_11",300,-5,5,(pfmet*(cos(DM.DeltaPhi(pfmet_vector)))-DM.Pt())/DM.Pt());
           CreateAndFillUserTH1D("Pt_ortogonal_2isr_vertex_11",120,-600,600,(pfmet*sin(DM.DeltaPhi(pfmet_vector))));
           CreateAndFillUserTH1D("Pt_t1_parallel_2isr_vertex_11",300,-5,5,(t1pfmet*(cos(DM.DeltaPhi(pfmett1_vector)))-DM.Pt())/DM.Pt());
           CreateAndFillUserTH1D("Pt_t1_ortogonal_2isr_vertex_11",120,-600,600,(t1pfmet*sin(DM.DeltaPhi(pfmett1_vector))));
         }
         if(nvtx>19 && DM.Pt()>250){
           CreateAndFillUserTH1D("Pt_parallel_2isr_vertex_19",300,-5,5,(pfmet*(cos(DM.DeltaPhi(pfmet_vector)))-DM.Pt())/DM.Pt());
           CreateAndFillUserTH1D("Pt_ortogonal_2isr_vertex_19",120,-600,600,(pfmet*sin(DM.DeltaPhi(pfmet_vector))));
           CreateAndFillUserTH1D("Pt_t1_parallel_2isr_vertex_19",300,-5,5,(t1pfmet*(cos(DM.DeltaPhi(pfmett1_vector)))-DM.Pt())/DM.Pt());
           CreateAndFillUserTH1D("Pt_t1_ortogonal_2isr_vertex_19",120,-600,600,(t1pfmet*sin(DM.DeltaPhi(pfmett1_vector))));
         }
    
       }else{  //just 1 ISR particle
         ++isr1_number;
         if(DM.Pt()<250){
           CreateAndFillUserTH1D("Pt_parallel_rangeIn_1isr",300,-5,5,(pfmet*(cos(DM.DeltaPhi(pfmet_vector)))-DM.Pt())/(DM.Pt()));//ricambiato senza sqrt
           CreateAndFillUserTH1D("Pt_ortogonal_rangeIn_1isr",120,-600,600,(pfmet*sin(DM.DeltaPhi(pfmet_vector))));
           CreateAndFillUserTH1D("Pt_t1_parallel_rangeIn_1isr",300,-5,5,(t1pfmet*(cos(DM.DeltaPhi(pfmett1_vector)))-DM.Pt())/DM.Pt());
           CreateAndFillUserTH1D("Pt_t1_ortogonal_rangeIn_1isr",120,-600,600,(t1pfmet*sin(DM.DeltaPhi(pfmett1_vector))));
         }
         for(int y=0;y<3;++y){  //MET bins
             
           if(DM.Pt()>met[y]&&DM.Pt()<met[y+1]&&y!=2){
             CreateAndFillUserTH1D(Form("Pt_parallel_range%d_1isr",y),300,-5,5,(pfmet*(cos(DM.DeltaPhi(pfmet_vector)))-DM.Pt())/(DM.Pt())); //ricambiato senza sqrt
             CreateAndFillUserTH1D(Form("Pt_ortogonal_range%d_1isr",y),120,-600,600,(pfmet*sin(DM.DeltaPhi(pfmet_vector))));
             CreateAndFillUserTH1D(Form("Pt_t1_parallel_range%d_1isr",y),300,-5,5,(t1pfmet*(cos(DM.DeltaPhi(pfmett1_vector)))-DM.Pt())/DM.Pt());
             CreateAndFillUserTH1D(Form("Pt_t1_ortogonal_range%d_1isr",y),120,-600,600,(t1pfmet*sin(DM.DeltaPhi(pfmett1_vector))));
           } 
               
           if(y==2 && DM.Pt()>met[y]){
             CreateAndFillUserTH1D("Pt_parallel_rangeLast_1isr",300,-5,5,(pfmet*(cos(DM.DeltaPhi(pfmet_vector)))-DM.Pt())/(DM.Pt()));//ricambiato senza sqrt
             CreateAndFillUserTH1D("Pt_ortogonal_rangeLast_1isr",120,-600,600,(pfmet*sin(DM.DeltaPhi(pfmet_vector))));
             CreateAndFillUserTH1D("Pt_t1_parallel_rangeLast_1isr",300,-5,5,(t1pfmet*(cos(DM.DeltaPhi(pfmett1_vector)))-DM.Pt())/DM.Pt());
             CreateAndFillUserTH1D("Pt_t1_ortogonal_rangeLast_1isr",120,-600,600,(t1pfmet*sin(DM.DeltaPhi(pfmett1_vector))));
           }
          
         }//end loop over MET bins
         //== Analysis for 1 ISR and different N_vertex bins ==
         vertex_range=5;
         
         if(nvtx>11 && nvtx<19 && DM.Pt()>250){
           CreateAndFillUserTH1D("Pt_parallel_1isr_vertex_11_19",300,-5,5,(pfmet*(cos(DM.DeltaPhi(pfmet_vector)))-DM.Pt())/DM.Pt());
           CreateAndFillUserTH1D("Pt_ortogonal_1isr_vertex_11_19",120,-600,600,(pfmet*sin(DM.DeltaPhi(pfmet_vector))));
           CreateAndFillUserTH1D("Pt_t1_parallel_1isr_vertex_11_19",300,-5,5,(t1pfmet*(cos(DM.DeltaPhi(pfmett1_vector)))-DM.Pt())/DM.Pt());
           CreateAndFillUserTH1D("Pt_t1_ortogonal_1isr_vertex_11_19",120,-600,600,(t1pfmet*sin(DM.DeltaPhi(pfmett1_vector))));
         }
          if(nvtx<11 && DM.Pt()>250){
            CreateAndFillUserTH1D("Pt_parallel_1isr_vertex_11",300,-5,5,(pfmet*(cos(DM.DeltaPhi(pfmet_vector)))-DM.Pt())/DM.Pt());
            CreateAndFillUserTH1D("Pt_ortogonal_1isr_vertex_11",120,-600,600,(pfmet*sin(DM.DeltaPhi(pfmet_vector))));
            CreateAndFillUserTH1D("Pt_t1_parallel_1isr_vertex_11",300,-5,5,(t1pfmet*(cos(DM.DeltaPhi(pfmett1_vector)))-DM.Pt())/DM.Pt());
            CreateAndFillUserTH1D("Pt_t1_ortogonal_1isr_vertex_11",120,-600,600,(t1pfmet*sin(DM.DeltaPhi(pfmett1_vector))));
          } 
          if(nvtx>19 && DM.Pt()>250){
            CreateAndFillUserTH1D("Pt_parallel_1isr_vertex_19",300,-5,5,(pfmet*(cos(DM.DeltaPhi(pfmet_vector)))-DM.Pt())/DM.Pt());
            CreateAndFillUserTH1D("Pt_ortogonal_1isr_vertex_19",120,-600,600,(pfmet*sin(DM.DeltaPhi(pfmet_vector))));
            CreateAndFillUserTH1D("Pt_t1_parallel_1isr_vertex_19",300,-5,5,(t1pfmet*(cos(DM.DeltaPhi(pfmett1_vector)))-DM.Pt())/DM.Pt());
            CreateAndFillUserTH1D("Pt_t1_ortogonal_1isr_vertex_19",120,-600,600,(t1pfmet*sin(DM.DeltaPhi(pfmett1_vector))));
          }
    
     
       }//end if for ISR number
     }//end ISR1 condition
     
     //== Number of vertices ==
     CreateAndFillUserTH1D("N_vertex",31,-.5,30.5,nvtx);

     //CreateAndFillUserTH2D("DM_JetPt_vs_PU",41,-.5,40.5,40,-10,50,puobs,(DM.Pt()-Jet.Pt())/Jet.Pt());
     //CreateAndFillUserTH1D("jet1Eta",100,-10,10,signaljeteta);
     //CreateAndFillUserTH1D("N_jets",40,-1,15,njets);

     //== Initial State Radiation info == (gen_level)
     
     CreateAndFillUserTH1D("Pt_ISR1",1000,.1,2000,i1pt);
     CreateAndFillUserTH1D("Pt_ISR2",1000,.1,2000,i2pt);
     CreateAndFillUserTH1D("Eta_ISR1",100,-10,10,i1eta);
     CreateAndFillUserTH1D("Eta_ISR2",100,-10,10,i2eta);
     CreateAndFillUserTH1D("Phi_ISR1",30,-3,3,i1phi);
     CreateAndFillUserTH1D("Phi_ISR2",30,-3,3,i2phi);

//     R_isr1=sqrt(pow(signaljeteta-i1eta,2)+pow(signaljetphi-i1phi,2));           //Delta_R for leading_jet & isr1
//     R_isr2=sqrt(pow(signaljeteta-i2eta,2)+pow(signaljetphi-i2phi,2));           //Delta_R for leading_jet &isr2
//     R_ISR=sqrt(pow(ISR.Phi()-Jet.Phi(),2)+pow(ISR.Eta()-Jet.Eta(),2));          //Delta_R for 2Jet_system & isr1+isr2 system
//     R_ISR_primo=sqrt(pow(ISR.Phi()-JET.Phi(),2)+pow(ISR.Eta()-JET.Eta(),2));    //Delta_R for 3Jet_system & isr1+isr2 system

     //== JET info == (RECO_level)

     CreateAndFillUserTH1D("Pt_jet1",1000,.1,2000,signaljetpt);
     CreateAndFillUserTH1D("Pt_jet2",1000,.1,2000,secondjetpt);
     CreateAndFillUserTH1D("Eta_jet1",100,-10,10,signaljeteta);
     CreateAndFillUserTH1D("Eta_jet2",100,-10,10,secondjeteta);
     CreateAndFillUserTH1D("Phi_jet1",30,-3,3,(signaljetphi));
     CreateAndFillUserTH1D("Phi_jet2",30,-3,3,secondjetphi);

     //CreateAndFillUserTH2D("pt_recoGen_vs_R",100,-1,10,40,-1,30,R_isr1,(signaljetpt-i1pt)/i1pt);

     ////== Filling histos :: Gen_level - RECO_level == (Jet part)

     double jet_eta[]={signaljeteta,secondjeteta,thirdjeteta};
     double jet_phi[]={signaljetphi,secondjetphi,thirdjetphi};
     double jet_pt[]={signaljetpt,secondjetpt,thirdjetpt};

     double isr_eta[]={i1eta,i2eta};
     double isr_phi[]={i1phi,i2phi};
     double isr_pt[]={i1pt,i2pt};

     double Delta_R,Delta_R_min;
     int jet_index, isr_index;

     for(int i=0;i<2;++i){ //loop over parton generated
       Delta_R_min=99999;
       

       for(int j=0;j<3;++j){  //loop over jet reconstructed
         Delta_R=sqrt(pow(isr_phi[i]-jet_phi[j],2)+pow(isr_eta[i]-jet_eta[j],2));

         if(Delta_R<Delta_R_min){
           Delta_R_min=Delta_R;
           jet_index=j;
           isr_index=i;
 
         }//end if for Delta_R_min

       }//end for over jets

       //== Filling histos :: Gen_level - RECO_level ==

       CreateAndFillUserTH2D(Form("Pt_isr%d_difference",i+1),1000,-1,2000,400,-20,20,isr_pt[i],(jet_pt[jet_index]-isr_pt[i])/isr_pt[i]);
       CreateAndFillUserTH1D(Form("Eta_isr%d_difference",i+1),50,-5,5,jet_eta[jet_index]-isr_eta[i]);
       CreateAndFillUserTH1D(Form("Phi_isr%d_difference",i+1),30,-3,3,jet_phi[jet_index]-isr_phi[i]);
       //== Component analysis == (ortogonal && parallel)
       CreateAndFillUserTH1D(Form("Pt_isr%d_parallel_difference",i+1),40,-2,2,(jet_pt[jet_index]*cos(jet_phi[jet_index])-isr_pt[i]*cos(isr_phi[i]))/isr_pt[i]*cos(isr_phi[i]));
       CreateAndFillUserTH1D(Form("Pt_isr%d_ortogonal_difference",i+1),40,-2,2,(jet_pt[jet_index]*sin(jet_phi[jet_index])-isr_pt[i]*sin(isr_phi[i]))/isr_pt[i]*sin(isr_phi[i]));

     }//end loop over isr particles



     if(signaljetpt>200 && signaljeteta<2.4 && signaljeteta>-2.4){
        CreateAndFillUserTH1D("N_jets_corr",40,-1,15,njets);
     }

     //== Just a try on DM invariant mass == (nothing seen...it's not a peak! The mediator mass is 40 TeV!)
     fillVariableWithValue("DM_invMass",DM.M());

     if((pfmetphi-DM.Phi())/DM.Phi()>-1&&(pfmetphi-DM.Phi())/DM.Phi()<1){

       CreateAndFillUserTH1D("Difference_reco_gen_PhiCorrected", 100, -2.,6, (pfmet-DM.Pt())/DM.Pt());

     }

     if((t1pfmetphi-DM.Phi())/DM.Phi()>-1&&(t1pfmetphi-DM.Phi())/DM.Phi()<1){

       CreateAndFillUserTH1D("Difference_reco_t1_gen_PhiCorrected", 100, -2.,6, (t1pfmet-DM.Pt())/DM.Pt());

     }

     // fillVariableWithValue("lumi",lumi);     
     // fillVariableWithValue("nVtx",nvtx);     
     // fillVariableWithValue("nJet",widejets.size());

     //         fillVariableWithValue( "pT_j2", widejets[1].Pt() );

     //fillVariableWithValue("MET",met);

     // Evaluate cuts (but do not apply them)
     evaluateCuts();
     
     // optional call to fill a skim with the full content of the input roottuple
     //if( passedCut("nJetFinal") ) fillSkimTree();
     
     // optional call to fill a skim with a subset of the variables defined in the cutFile (use flag SAVE)Z
     if( passedAllPreviousCuts("PFMET") && passedCut("PFMET") ) 
       {
	 fillReducedSkimTree();
       }	 

   //CreateAndFillUserTH1D("h_pt_corr", 100, 0, 1000, getVariableValue("pT_j1")*getPreCutValue1("ScaleFactorForJets")); //Questa funzione permette di salvare degli istogrammi diversi dalle variabili inizializzate prima dei tagli
     //CreateAndFillUserTH2D("SignalPt_vs_eta",100,-5.,5.,2000,-100.,1500.,getVariableValue("eta"),getVariableValue("signal_jet"));
     // 	 // ===== Take a look at this =====
     // 	 // //Example on how to investigate quickly the data
     // 	 // if(getVariableValue("mjj")>4000)
     // 	 //   {
     // 	 //     //fast creation and filling of histograms
     // 	 //     CreateAndFillUserTH1D("h_dphijj_mjjgt4000", 100, 0, 3.15, getVariableValue("deltaPHIjj"));
     // 	 //     CreateAndFillUserTH1D("h_htak4_mjjgt4000", 1000, 0, 10000, getVariableValue("HTAK4"));
     // 	 //     CreateAndFillUserTH1D("h_nvtx_mjjgt4000", 31, -0.5, 30.5, getVariableValue("nVtx"));
     // 	 //   }

     //   }

     // ===== Example of mjj spectrum after HLT selection =====
     // if( passedAllPreviousCuts("mjj") )
     //   {
     // 	 if(getVariableValue("passHLT")>0)
     // 	   {
     // 	     //fast creation and filling of histograms
     // 	     CreateAndFillUserTH1D("h_mjj_passHLT", getHistoNBins("mjj"), getHistoMin("mjj"), getHistoMax("mjj"), getVariableValue("mjj"));
     // 	   }
     //   }

     // reject events that did not pass level 0 cuts
     //if( !passedCut("0") ) continue;
     // ......
     
     // reject events that did not pass level 1 cuts
     //if( !passedCut("1") ) continue;
     // ......

     // reject events that did not pass the full cut list
     //if( !passedCut("all") ) continue;
     // ......

     // if( widejets.size() >= 2) {
     //  h_nJetFinal->Fill(widejets.size());
     //  h_DijetMass->Fill(wdijet.M());
     //  h_pT1stJet->Fill(widejets[0].Pt());
     //  h_pT2ndJet->Fill(widejets[1].Pt());
     //  h_eta1stJet->Fill(widejets[0].Eta());
     //  h_eta2ndJet->Fill(widejets[1].Eta());
     // }
     ////////////////////// User's code ends here ///////////////////////

   } // End loop over events

   //////////write histos 

   // h_nVtx->Write();
   // h_trueVtx->Write();
   // h_nJetFinal->Write();
   // h_pT1stJet->Write();
   // h_pT2ndJet->Write();
   // h_DijetMass->Write();
   // h_eta1stJet->Write();
   // h_eta2ndJet->Write();

   // //pT of both jets, to be built using the histograms produced automatically by baseClass
   // TH1F * h_pTJets = new TH1F ("h_pTJets","", getHistoNBins("pT1stJet"), getHistoMin("pT1stJet"), getHistoMax("pT1stJet"));
   // h_pTJets->Add( & getHisto_noCuts_or_skim("pT1stJet") ); // all histos can be retrieved, see other getHisto_xxxx methods in baseClass.h
   // h_pTJets->Add( & getHisto_noCuts_or_skim("pT2ndJet") );
   // //one could also do:  *h_pTJets = getHisto_noCuts_or_skim("pT1stJet") + getHisto_noCuts_or_skim("pT2ndJet");
   // h_pTJets->Write();
   // //one could also do:   const TH1F& h = getHisto_noCuts_or_skim// and use h
   std::cout<<"++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;
   std::cout<<"1 ISR: "<<isr1_number<<" 2 ISR: "<<isr2_number<<std::endl;
   std::cout<<"++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;

   std::cout<<"++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
   std::cout<<"++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;
   std::cout<<"RICORDATI DI CAMBIARE LE MASSE!!"<<std::endl;

   std::cout << "------ TIME ELAPSED DURING ANALYSIS  ----- " << time.RealTime() << " s" <<std::endl;
  
   //Wake up function!!!
   std::cout << '\a';
}
