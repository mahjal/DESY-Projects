rm pythia.out
mkdir /tmp/$(whoami)
rm /tmp/$(whoami)/myhepmc.fifo

export RIVET_ANALYSIS_PATH=/afs/desy.de/user/j/jung/public/summerstudents/2022/rivet
export RIVET_DATA_PATH=/afs/desy.de/user/j/jung/public/summerstudents/2022/rivet

export HEPMCOUT=/tmp/$(whoami)/myhepmc.fifo
mkfifo /tmp/$(whoami)/myhepmc.fifo
export PYSEED=12344
nice ./main31 &
#rivet -a myanalysis $HEPMCOUT> rivet.out
rivet -a DELPHI_1991_I324035 $HEPMCOUT > rivet.out
#rivet -a CMS_2016_I1487277 $HEPMCOUT > rivet.out
#rivet -a MC_PRINTEVENT  $HEPMCOUT > rivet.out
