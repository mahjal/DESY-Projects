#!/bin/zsh

export RIVET_ANALYSIS_PATH=$PWD/build
export RIVET_REF_PATH=$PWD
export RIVET_DATA_PATH=$PWD

### CMS_2021_I1949191 analysis ###                                                                                                                                                                          
rivet-mkhtml WZTo3LNu_13TeV-powheg-pythia8.yoda:'Title=POWHEG (NLO, CP5)':'ErrorBands=1':'ErrorBandColor=red':'ErrorBandOpacity=0.2' \
             WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8.yoda:'Title=aMC@NLO (NLO, CP5)':'ErrorBands=1':'ErrorBandColor=blue':'ErrorBandOpacity=0.2' \
             WZTo3LNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.yoda:'Title=aMC@NLO (NLO, CUETP8M1)':'ErrorBands=1':'ErrorBandColor=green':'ErrorBandOpacity=0.2' \
             --no-weights --font times -o CMS_2021_I1949191_HTML


### CMS_2021_I1992937 analysis ###
#yodamerge -o CMS_2021_I1992937_YODA/DYJetsToLL_TuneCP5_13TeV-amcatnloFXFX-pythia8.yoda \
#             CMS_2021_I1992937_YODA/DYJetsToLL_0J_TuneCP5_13TeV-amcatnloFXFX-pythia8.yoda \
#             CMS_2021_I1992937_YODA/DYJetsToLL_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8.yoda \
#             CMS_2021_I1992937_YODA/DYJetsToLL_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8.yoda
             
#rivet-mkhtml CMS_2021_I1992937_YODA/DYJetsToLL_TuneCP5_13TeV-amcatnloFXFX-pythia8.yoda:'Title=aMC@NLO (NLO, CP5)':'ErrorBands=1':'ErrorBandColor=red':'ErrorBandOpacity=0.2' \
#             CMS_2021_I1992937_YODA/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.yoda:'Title=aMC@NLO (LO, CP5)':'ErrorBands=1':'ErrorBandColor=blue':'ErrorBandOpacity=0.2' \
#             CMS_2021_I1992937_YODA/DY01234jets_13TeV-sherpa.yoda:'Title=Sherpa':'ErrorBands=1':'ErrorBandColor=green':'ErrorBandOpacity=0.2' \
#             --no-weights --font times -o CMS_2021_I1992937_HTML

