#!/bin/sh

# mkdir  QCDincl_ptmin-300_70m200_CSVIVF-09_tau21-05
# mkdir  QCDincl_ptmin-300_70m200_CSVIVF-08_tau21-05
# mkdir  QCDincl_ptmin-300_70m200_CSVIVF-07_tau21-05
# mkdir  QCDincl_ptmin-300_70m200_CSVIVF-06_tau21-05
# mkdir  QCDincl_ptmin-300_70m200_CSVIVF-06_tau21-04
mkdir  QCDincl_ptmin-300_70m200_CSVIVF-06
# mkdir  QCDincl_ptmin-300_70m200_CSVIVF-05_tau21-05
# mkdir  QCDincl_ptmin-300_70m200_CSVIVF-04_tau21-05


mkdir  QCDincl_ptmin-300_70m200_muTag_tau21-05


# python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDincl_ptmin-300_70m200_CSVIVF-09_tau21-05.txt -o QCDincl_ptmin-300_70m200_CSVIVF-09_tau21-05
# python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDincl_ptmin-300_70m200_CSVIVF-08_tau21-05.txt -o QCDincl_ptmin-300_70m200_CSVIVF-08_tau21-05
# python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDincl_ptmin-300_70m200_CSVIVF-07_tau21-05.txt -o QCDincl_ptmin-300_70m200_CSVIVF-07_tau21-05
# python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDincl_ptmin-300_70m200_CSVIVF-06_tau21-05.txt -o QCDincl_ptmin-300_70m200_CSVIVF-06_tau21-05
# python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDincl_ptmin-300_70m200_CSVIVF-06_tau21-04.txt -o QCDincl_ptmin-300_70m200_CSVIVF-06_tau21-04
python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDincl_ptmin-300_70m200_CSVIVF-06.txt -o QCDincl_ptmin-300_70m200_CSVIVF-06
# python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDincl_ptmin-300_70m200_CSVIVF-05_tau21-05.txt -o QCDincl_ptmin-300_70m200_CSVIVF-05_tau21-05
# python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDincl_ptmin-300_70m200_CSVIVF-04_tau21-05.txt -o QCDincl_ptmin-300_70m200_CSVIVF-04_tau21-05

python ../../mergeDatasets.py -w /eos/uscms/store/user/rsyarif/HbbTagVal/EventSelectionStudies/ -d datasetListForMerging_QCDincl_ptmin-300_70m200_muTag_tau21-05.txt -o QCDincl_ptmin-300_70m200_muTag_tau21-05
