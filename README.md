BTagValidation
==============

Boosted b-tag validation analysis 

To set up:

Follow the directions to clone cms-btv-pog/RecoBTag-PerformanceMeasurements at https://github.com/cms-btv-pog/RecoBTag-PerformanceMeasurements

cd CMSSW_7_4_7/src

git clone git://github.com/EJDomi/BTagValidation.git RecoBTag/BTagValidation

scram b -j8

cd RecoBTag/BTagValidation

cmsRun btagvalidation_cfg.py
  
