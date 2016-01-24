{
	gROOT->ProcessLine(".L makeComparisonBackground.C");

	bool Reweight = true;
	option="330";
	isLog=0;
	makeComparisonBackground("BDTG_SV_bfromg", -1, 1, "Final Discriminator",2,option, isLog, Reweight);
	makeComparisonBackground("jetNTracks_bfromg", 0, 40, "Number of tracks",1,option, isLog, Reweight);
	makeComparisonBackground("nSV_bfromg", 0, 10, "Number of SV",1,option, isLog, Reweight);
	makeComparisonBackground("tau1_vertexEnergyRatio_bfromg", 0, 4, "SV energy ratio (1)",2,option, isLog, Reweight);
	makeComparisonBackground("tau1_vertexMass_bfromg", 0, 50, "SV Mass (1)",2,option, isLog, Reweight);
	makeComparisonBackground("tau2_vertexEnergyRatio_bfromg", 0, 4, "SV energy ratio (1)",2,option, isLog, Reweight);
	makeComparisonBackground("tau2_vertexMass_bfromg", 0, 50, "SV Mass (1)",2,option, isLog, Reweight);
	makeComparisonBackground("pt_all_bfromg", 0, 1500, "p_{T}",2,option, isLog, Reweight);
	makeComparisonBackground("mass_bfromg", 0, 400, "Mass",2,option, isLog, Reweight);
	makeComparisonBackground("prunedMass_bfromg", 0, 400, "Mass_{Pruned}",2,option, isLog, Reweight);
	makeComparisonBackground("softdropMass_bfromg", 0, 400, "Mass_{SoftDrop}",2,option, isLog, Reweight);

	isLog=1;
	makeComparisonBackground("BDTG_SV_bfromg", -1, 1, "Final Discriminator",2,option, isLog, Reweight);
	makeComparisonBackground("jetNTracks_bfromg", 0, 40, "Number of tracks",1,option, isLog, Reweight);
	makeComparisonBackground("nSV_bfromg", 0, 10, "Number of SV",1,option, isLog, Reweight);
	makeComparisonBackground("tau1_vertexEnergyRatio_bfromg", 0, 4, "SV energy ratio (1)",2,option, isLog, Reweight);
	makeComparisonBackground("tau1_vertexMass_bfromg", 0, 50, "SV Mass (1)",2,option, isLog, Reweight);
	makeComparisonBackground("tau2_vertexEnergyRatio_bfromg", 0, 4, "SV energy ratio (1)",2,option, isLog, Reweight);
	makeComparisonBackground("tau2_vertexMass_bfromg", 0, 50, "SV Mass (1)",2,option, isLog, Reweight);
	makeComparisonBackground("pt_all_bfromg", 0, 1500, "p_{T}",2,option, isLog, Reweight);
	makeComparisonBackground("mass_bfromg", 0, 400, "Mass",2,option, isLog, Reweight);
	makeComparisonBackground("prunedMass_bfromg", 0, 400, "Mass_{Pruned}",2,option, isLog, Reweight);
	makeComparisonBackground("softdropMass_bfromg", 0, 400, "Mass_{SoftDrop}",2,option, isLog, Reweight);
	
// 	option="450";
// 	isLog=0;
// 	makeComparisonBackground("BDTG_SV_bfromg", -1, 1, "Final Discriminator",2,option, isLog, Reweight);
// 	makeComparisonBackground("jetNTracks_bfromg", 0, 40, "Number of tracks",1,option, isLog, Reweight);
// 	makeComparisonBackground("nSV_bfromg", 0, 10, "Number of SV",1,option, isLog, Reweight);
// 	makeComparisonBackground("tau1_vertexEnergyRatio_bfromg", 0, 4, "SV energy ratio (1)",2,option, isLog, Reweight);
// 	makeComparisonBackground("tau1_vertexMass_bfromg", 0, 50, "SV Mass (1)",2,option, isLog, Reweight);
// 	makeComparisonBackground("tau2_vertexEnergyRatio_bfromg", 0, 4, "SV energy ratio (1)",2,option, isLog, Reweight);
// 	makeComparisonBackground("tau2_vertexMass_bfromg", 0, 50, "SV Mass (1)",2,option, isLog, Reweight);
// 	makeComparisonBackground("pt_all_bfromg", 0, 1500, "p_{T}",2,option, isLog, Reweight);
// 	makeComparisonBackground("mass_bfromg", 0, 400, "Mass",2,option, isLog, Reweight);
// 	makeComparisonBackground("prunedMass_bfromg", 0, 400, "Mass_{Pruned}",2,option, isLog, Reweight);
// 	makeComparisonBackground("softdropMass_bfromg", 0, 400, "Mass_{SoftDrop}",2,option, isLog, Reweight);
// 
// 	isLog=1;
// 	makeComparisonBackground("BDTG_SV_bfromg", -1, 1, "Final Discriminator",2,option, isLog, Reweight);
// 	makeComparisonBackground("jetNTracks_bfromg", 0, 40, "Number of tracks",1,option, isLog, Reweight);
// 	makeComparisonBackground("nSV_bfromg", 0, 10, "Number of SV",1,option, isLog, Reweight);
// 	makeComparisonBackground("tau1_vertexEnergyRatio_bfromg", 0, 4, "SV energy ratio (1)",2,option, isLog, Reweight);
// 	makeComparisonBackground("tau1_vertexMass_bfromg", 0, 50, "SV Mass (1)",2,option, isLog, Reweight);
// 	makeComparisonBackground("tau2_vertexEnergyRatio_bfromg", 0, 4, "SV energy ratio (1)",2,option, isLog, Reweight);
// 	makeComparisonBackground("tau2_vertexMass_bfromg", 0, 50, "SV Mass (1)",2,option, isLog, Reweight);
// 	makeComparisonBackground("pt_all_bfromg", 0, 1500, "p_{T}",2,option, isLog, Reweight);
// 	makeComparisonBackground("mass_bfromg", 0, 400, "Mass",2,option, isLog, Reweight);
// 	makeComparisonBackground("prunedMass_bfromg", 0, 400, "Mass_{Pruned}",2,option, isLog, Reweight);
// 	makeComparisonBackground("softdropMass_bfromg", 0, 400, "Mass_{SoftDrop}",2,option, isLog, Reweight);

	gROOT->ProcessLine(".q");

} 
