#! /bin/bash

if test -z "${_CONDOR_SCRATCH_DIR}"; then
    export _CONDOR_SCRATCH_DIR=.
fi

EXDIR=/home/t3-ku/erichjs/work/BTV/CMSSW_7_4_5/src/RecoBTag/BTagValidation/test
cd $EXDIR

eval `scramv1 runtime -sh`

cd ${_CONDOR_SCRATCH_DIR}
cmsRun ${EXDIR}/btagvalidation_cfg.py useJetProbaTree=True usePrunedSubjets=True outFilename=${EXDIR}/ValPlotFiles/TTJets/50ns/allFatJets.root maxEvents=-1
