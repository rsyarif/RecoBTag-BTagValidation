
./condor_submit_list.sh --> this calls createAndSubmitJobs_FNALcondor.py many times, and submits condor jobs saved in this folder

./checkkError_ALLfolders.sh --> calls checkError_rizki.py many times for all condor job folders, checks is output/*.stderr has anything. NOT 100% accurate.

./mergeCondorOutput.sh --> hadd s condor output root files, saves in /eos/ area

./mergeQCDincl.sh --> calls ../../mergeDatasets.py many times and uses datasetListForMerging_QCDincl* in this folder

---> Get Final QCD histos





