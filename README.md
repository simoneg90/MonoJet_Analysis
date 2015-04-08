# MonoJet_Analysis
This repository contains the initial analysis for MonoJet events. (just the main programs)

#Analysis usage
./main config/inputListExample.txt config/cutFile_mainMonoJetSelection.txt tree/tree output/tree10 output/cutEfficiencyFile

#quick_histo script usage
python quick_histo.py -i 'background_list.txt' -o 'output_folder' -E 10 -v "signaljetpt" -s 'signal_file.root'

#analysis_cut usage
In root, just launch the program: .x analysis_cut.C <br />
The file analysed are already in the program (further developing to give the file from outside...)
