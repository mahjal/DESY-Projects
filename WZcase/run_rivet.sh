#!/bin/zsh

export RIVET_ANALYSIS_PATH=$PWD
export RIVET_REF_PATH=$PWD
export RIVET_DATA_PATH=$PWD

# WZ analysis
rivet -a CMS_2021_I1949191 -o CMS_2021_I1949191_MC.yoda -n 100000 /afs/desy.de/user/s/school78/www/Samples/WZTo3LNu_13TeV-powheg-pythia8/WZTo3LNu_13TeV-powheg-pythia8_0.hepmc.gz

# Z+b analysis
#rivet -a CMS_2021_I1992937 -o CMS_2021_I1992937_MC_0.yoda -n 100000 /afs/desy.de/user/s/school78/www/Samples/DYJetsToLL_0J_TuneCP5_13TeV-amcatnloFXFX-pythia8/*.hepmc.gz
#rivet -a CMS_2021_I1992937 -o CMS_2021_I1992937_MC_1.yoda -n 100000 /afs/desy.de/user/s/school78/www/Samples/DYJetsToLL_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8/*.hepmc.gz
#rivet -a CMS_2021_I1992937 -o CMS_2021_I1992937_MC_2.yoda -n 100000 /afs/desy.de/user/s/school78/www/Samples/DYJetsToLL_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8/*.hepmc.gz
#yodamerge -o CMS_2021_I1992937_MC.yoda CMS_2021_I1992937_MC_*.yoda
#rm CMS_2021_I1992937_MC_*.yoda
