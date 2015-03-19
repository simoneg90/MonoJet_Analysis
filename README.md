# MonoJet_Analysis
This repository contains the initial analysis for MonoJet events. (just the main programs)

#Analysis usage
./main config/inputListExample.txt config/cutFile_mainMonoJetSelection.txt tree/tree output/tree10 output/cutEfficiencyFile

#quick_histo script usage
python quick_histo.py -i 'background_list.txt' -o 'output_folder' -E 10 -v "signaljetpt" -s 'signal_file.root'
