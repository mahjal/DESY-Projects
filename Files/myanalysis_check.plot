File Edit Options Buffers Tools Help                                                      
BEGIN PLOT /myanalysis_check/eta
Title=Charged Particle Multiplicity (Pseudorapidity)
XLabel=$\eta$
YLabel=$1/N dn/d\eta$
# + any additional plot settings you might like, see make-plots documentation
LogY=0
END PLOT
BEGIN PLOT /myanalysis_check/rap
Title=Charged Particle Multiplicity (Rapidity)
XLabel=$y$
YLabel=$1/N dn/dy$
# + any additional plot settings you might like, see make-plots documentation
LogY=0
END PLOT

# ... add more histograms as you need them ...


