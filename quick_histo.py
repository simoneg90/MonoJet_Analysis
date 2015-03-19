#!usr/bin/python
import sys
import os

from array import *

import ntpath

import optparse

#=== Cut definitions ===
pfmetCut1                      = 1000
pfmetCut                       = 10000
mumetCut                       = 200
signaljetNHfracCut             = 0.7  #NOT TO BE CHANGED
signaljetEMfracCut             = 0.7  #NOT TO BE CHANGED
signaljetCHfracCut             = 0.2  #NOT TO BE CHANGED
njetsCut                       = 2
secondjetNHfracCut             = 0.7  #NOT TO BE CHANGED
secondjetEMfracCut             = 0.9  #NOT TO BE CHANGED
signaljetptCut1                = 1000
signaljetptCut                 = 10000
signaljetetaCut                = 2.4
jetjetdphiCut                  = 2.5
nmuonsCut                      = 0
nelectronsCut                  = 0
ntausCut                       = 0


print("Starting the comparison program")

#=== Defining Options ===
#=== List of Options ===
#  var-> variable you want to plot
#  outputDir-> where you want to put your histo
#  etaCut, ptCut-> cuts over |eta|, pfmet
#  inputFile-> input tree file or input list

parser = optparse.OptionParser()
parser.add_option("-v","--var",action="store",type="string",dest="var",default='signaljetpt')
parser.add_option("-o","--outputDir",action="store",type="string",default="./",dest="outputDir")
parser.add_option("-E","--etaCut",action="store", type="float", default='7.',dest="etaCut")
parser.add_option("-P","--ptCut",action="store",type="float",default='0',dest="ptCut")
parser.add_option("-i","--inputFile",action="store", type="string", dest="inputFile")
parser.add_option("-s","--signalFile", action="store", type="string", dest="signalFile")

(options, args) = parser.parse_args()
print("Variable %s" % (options.var))
print("Output directory: %s" % options.outputDir)
print("Cut on eta: +- %f" % (options.etaCut))
print("Cut on pfmet: %f" % (options.ptCut))
print("File in input: %s" % (options.inputFile))
print("Signal file in input: %s" % (options.signalFile))

#=== Associating variables to entries ===

var = options.var
outputDir = options.outputDir
etaCut = options.etaCut
ptCut = options.ptCut
inputFile = options.inputFile
signalFile = options.signalFile

os.system('mkdir -p %s'%options.outputDir)

#=== Start reading the input file list for background ===

lines = [line.strip() for line in open(inputFile)]
files = []
signal_file = []

i=0
for line in lines:
  files.append(line)
  print("File: %s" % (files[i]))
  i+=1

#signals = [signal.strip() for signal in open(signalFile)]

j=0
#for signal in signals:
#  signal_file.append(signal)
#  print("Signal file: %s" % (signal_file[j]))
#  j+=1

#=== Opening ROOT ===

import ROOT
from ROOT import TFile, TH1F, TH1D, THStack, TCanvas, TMath, gROOT, gPad, TLegend

#=== Starting reading each file ===
string= 'signaljeteta < ' + str(etaCut) + ' && signaljeteta > -' + str(etaCut) + ' && pfmet > ' + str(ptCut)
print("test: %s" % (string))

#=== Creating Legend ===
leg=TLegend(0.7,0.7,0.9,0.9)

#=== First step: get scale factors for different backgrounds  ===
integral_back=[]
integral_back_sum=0
counter=0
for file in files:
  back = TFile.Open(file)
  print("Scale factor study for: %s" % (back.GetName()))
  histo_back=TH1F("histo_back","histo_back",3000,0,2000)
  tree=back.Get('tree/tree')
  tree.Project(histo_back.GetName(),var)
  print("Histogram events: %d for file: %s" % (histo_back.Integral(),back.GetName()))
  integral_back_sum+=histo_back.Integral()
  integral_back.append(histo_back.Integral())
  counter+=1

signal=TFile.Open(signalFile)
print("Scale factor study for: %s" % (signal.GetName()))
histo_sig=TH1F("histo_sig","histo_sig",3000,0,2000)
tree=signal.Get('tree/tree')
tree.Project(histo_sig.GetName(),var)
integral_signal=histo_sig.Integral()

tot_scale=integral_back_sum/integral_signal
print("Signal events: %d Background events: %d" % (integral_signal,integral_back_sum))
print("Total scale factor= %f" % (tot_scale))

#=== Filling background histos ===
new_counter=0
histograms = []
ii=0
#color=[ ROOT.kGreen, ROOT.kRed, ROOT.kBlue, ROOT.kOrange, ROOT.kYellow, ROOT.kViolet]
color=[ROOT.kOrange, ROOT.kSpring, ROOT.kPink, ROOT.kViolet, ROOT.kAzure, ROOT.kTeal]
for file in files:
  scale_value=integral_back_sum
  f = TFile.Open(file)
  print("File under study: %s" % (f.GetName()))
  histo_weight= TH1F("histo_weight","histo_weight",100,0.,1.)
  histo= TH1F("histo","histo", 50,0,2000)
  tree = f.Get('tree/tree')
  #tree.Project(histo_weight.GetName(),'weight')
  #tree.Project(histo.GetName(),var, 'signaljeteta < ' + str(etaCut) + ' && signaljeteta > -' + str(etaCut) + ' && pfmet > ' + str(ptCut))
  tree.Project(histo.GetName(),var,'pfmet>'+str(pfmetCut1)+'&&pfmet<'+str(pfmetCut) +'&& mumet>'+str(mumetCut)+' && signaljetNHfrac<'+str(signaljetNHfracCut)+' && signaljetEMfrac<'+str(signaljetEMfracCut)+' && signaljetCHfrac>'+str(signaljetCHfracCut)+' && (njets<'+str(njetsCut)+' || (secondjetNHfrac<'+str(secondjetNHfracCut)+' && secondjetEMfrac<'+str(secondjetEMfracCut)+'))&& signaljetpt>'+str(signaljetptCut1)+'&&signaljetpt<'+str(signaljetptCut)+' && abs(signaljeteta)<'+str(signaljetetaCut)+' && njets<='+str(njetsCut)+' && (njets==1 || abs(jetjetdphi)<'+str(jetjetdphiCut)+') && nmuons == '+str(nmuonsCut)+' && nelectrons == '+str(nelectronsCut)+' && ntaus == '+str(ntausCut))
  print("Histogram events: %d for file: %s" % (histo.Integral(),f.GetName()))
  for i in range(0,counter):
    if i is not ii:
      scale_value-=integral_back[i]
      print("sono nell'if")
    print("sono nel for")
  histo.SetLineColor(color[ii]-9)
  histo.SetMarkerColor(color[ii]-9)
  histo.SetFillColor(color[ii]-6)
  #histo.Scale(integral_signal/integral_back_sum)
  #histo.Scale(histo_weight.GetMean())
  new_counter+=histo.GetEntries()
  string=f.GetName()
  leg.AddEntry(histo,string[41:-10],"lp")
  histo.Rebin()
  histograms.append(histo)
  ii+=1

#=== Filling signal histos ===
histograms_signal = []
sig = TFile.Open(signalFile)
print("Signal file under study: %s" % (sig.GetName()))
histo_signal= TH1F("histo_signal","histo_signal", 50,0,2000)
tree = sig.Get('tree/tree')
#tree.Project(histo_signal.GetName(),var, 'signaljeteta < ' + str(etaCut) + ' && signaljeteta > -' + str(etaCut) + ' && pfmet > ' + str(ptCut))
tree.Project(histo_signal.GetName(),var,'pfmet>'+str(pfmetCut1)+'&&pfmet<'+str(pfmetCut) +'&& mumet>'+str(mumetCut)+' && signaljetNHfrac<'+str(signaljetNHfracCut)+' && signaljetEMfrac<'+str(signaljetEMfracCut) +' && signaljetCHfrac>'+str(signaljetCHfracCut)+' && (njets<'+str(njetsCut)+' || (secondjetNHfrac<'+str(secondjetNHfracCut)+' && secondjetEMfrac<'+str(secondjetEMfracCut)+'))&& signaljetpt>'+str(signaljetptCut1)+'&&signaljetpt<'+str(signaljetptCut)+' && abs(signaljeteta)<'+str(signaljetetaCut)+' && njets<='+str(njetsCut)+' && (njets==1 || abs(jetjetdphi)<'+str(jetjetdphiCut)+') && nmuons ==     '+str(nmuonsCut)+' && nelectrons == '+str(nelectronsCut)+' && ntaus == '+str(ntausCut))
print("Histogram events: %d for file: %s" % (histo_signal.Integral(),sig.GetName()))
histo_signal.SetLineColor(1)
histo_signal.SetMarkerColor(1)
string =sig.GetName()
leg.AddEntry(histo_signal,string[51:-5]+' GeV',"lp")
leg.SetFillColor(0)
#histo_signal.Scale(integral_back_sum/integral_signal)
#histo_signal.Scale(new_counter/histo_signal.GetEntries())
histo_signal.Rebin()
histo_signal.SetStats(0)
histograms_signal.append(histo_signal)

#=== Creating output file ===

outFile = TFile(outputDir+"/histo_signal_"+var+".root", "recreate")
outFile.cd()

hs = THStack("hs", "hs")
print("Processing %d files" % (len(files)))
for i in range(0,len(files)):
  hs.Add(histograms[i])

#hs.Add(histo_signal)

hs.Write()
histo.Write()
histo_signal.Write()
c1 = TCanvas("c1","c1",900,600)
c1.SetLogy()
c1.cd()
histo_signal.Draw()
hs.Draw("SAME")
#histo_signal.Draw("SAME")
leg.Draw("SAME")
#gPad.RedrawAxis()
c1.Write()
c1.Close()
print("Attenzione!!! Risultati dei tagli!!!")
print("Numero di eventi di segnale: %d" % (histo_signal.GetEntries()) )
print("Numero di eventi totali di background: %d" %(new_counter))
print("BC syntax:")
print("%d/sqrt(%d)" %(histo_signal.GetEntries(),new_counter))
outFile.Close()
