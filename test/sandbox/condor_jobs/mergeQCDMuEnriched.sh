#!/bin/sh

mkdir  QCDMuEnriched_ptmin-300_70m200_muTag_tau21-04
mkdir  QCDMuEnriched_ptmin-300_70m200_muTag_tau21-05
mkdir  QCDMuEnriched_ptmin-300_70m200_muTag

python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDMuEnriched_ptmin-300_70m200_muTag_tau21-05.txt -o QCDMuEnriched_ptmin-300_70m200_muTag_tau21-05
python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDMuEnriched_ptmin-300_70m200_muTag_tau21-05.txt -o QCDMuEnriched_ptmin-300_70m200_muTag_tau21-04
python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDMuEnriched_ptmin-300_70m200_muTag.txt -o QCDMuEnriched_ptmin-300_70m200_muTag