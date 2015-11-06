BTagValidation
==============

Boosted b-tag validation analysis 

To set up:

Follow the directions to clone cms-btv-pog/RecoBTag-PerformanceMeasurements at https://github.com/cms-btv-pog/RecoBTag-PerformanceMeasurements

After setting up the PerformanceMeasurements package:

cd CMSSW_7_X_X/src      
#Replace X's with the release number you are using

git clone git://github.com/cms-btv-pog/RecoBTag-BTagValidation.git RecoBTag/BTagValidation

scram b -j8

cd RecoBTag/BTagValidation/test/

cmsRun btagvalidation_cfg.py
  
