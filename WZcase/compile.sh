#!/bin/zsh

export RIVET_ANALYSIS_PATH=$PWD/build
export RIVET_REF_PATH=$PWD

## compile ##
rivet-build build/RivetAnalyses.so *.cc

