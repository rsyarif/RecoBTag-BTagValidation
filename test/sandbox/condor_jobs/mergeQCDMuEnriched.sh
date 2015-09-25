#!/bin/sh

#mkdir  QCDMuEnriched_ptmin-300_70m200_muTag_tau21-05
#mkdir  QCDMuEnriched_ptmin-300_70m200_muTag_tau21-04
#mkdir  QCDMuEnriched_ptmin-300_70m200_muTag

# mkdir  QCDMuEnriched_ptmin-300_70m200_singleMuTag_tau21-05

mkdir  QCDMuEnriched_ptmin-300_70m200_muTag_tau21-05_muPt7
mkdir  QCDMuEnriched_ptmin-300_70m200_singleMuTag_tau21-05_muPt7

mkdir  QCDMuEnriched_ptmin-300_70m200_muTag_tau21-05_muPt10
mkdir  QCDMuEnriched_ptmin-300_70m200_singleMuTag_tau21-05_muPt10

mkdir  QCDMuEnriched_ptmin-100_doubleMuTag
mkdir  QCDMuEnriched_ptmin-100_singleMuTag

#python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDMuEnriched_ptmin-300_70m200_muTag_tau21-05.txt -o QCDMuEnriched_ptmin-300_70m200_muTag_tau21-05
#python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDMuEnriched_ptmin-300_70m200_muTag_tau21-04.txt -o QCDMuEnriched_ptmin-300_70m200_muTag_tau21-04
#python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDMuEnriched_ptmin-300_70m200_muTag.txt -o QCDMuEnriched_ptmin-300_70m200_muTag
#python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDMuEnriched_ptmin-300_70m200_singleMuTag_tau21-05.txt -o QCDMuEnriched_ptmin-300_70m200_singleMuTag_tau21-05

python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDMuEnriched_ptmin-300_70m200_muTag_tau21-05_muPt7.txt -o QCDMuEnriched_ptmin-300_70m200_muTag_tau21-05_muPt7
python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDMuEnriched_ptmin-300_70m200_muTag_tau21-05_muPt10.txt -o QCDMuEnriched_ptmin-300_70m200_muTag_tau21-05_muPt10

python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDMuEnriched_ptmin-300_70m200_singleMuTag_tau21-05_muPt7.txt -o QCDMuEnriched_ptmin-300_70m200_singleMuTag_tau21-05_muPt7
python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDMuEnriched_ptmin-300_70m200_singleMuTag_tau21-05_muPt10.txt -o QCDMuEnriched_ptmin-300_70m200_singleMuTag_tau21-05_muPt10

python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDMuEnriched_ptmin-100_doubleMuTag.txt -o QCDMuEnriched_ptmin-100_doubleMuTag
python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDMuEnriched_ptmin-100_singleMuTag.txt -o QCDMuEnriched_ptmin-100_singleMuTag