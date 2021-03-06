#pragma once

class FernStyle {
public:
	ofxVec2f
		branchAngle,
		branchVariation,
		trunkAngle,
		trunkVariation,
		leftBranchAngle,
		rightBranchAngle,
		leftTrunkAngle,
		rightTrunkAngle,
		leafVariation;

	float
		branchScaling,
		trunkScaling,
		minLength,
		leafScale,
		branchWidth,
		leafGrowth,
		overallGrowth;

	float baseLength;
};
