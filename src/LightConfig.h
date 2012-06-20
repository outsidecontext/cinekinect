//
//  LightConfig.h
//
//  Created by Chris Mullany on 03/02/2012.
//

#pragma once

#include "ofMain.h"

#define LIGHT_TYPE_DIR 0
#define LIGHT_TYPE_SPOT 1
#define LIGHT_TYPE_POINT 2

struct LightConfig {
    bool isEnabled;
    int type;
    string typeLabel;
    ofVec3f pos, target;
    float concentration, cutoff, attenuationConst, attenuationLinear,attenuationQuad;
    ofFloatColor lightColorAmbient, lightColorDiffuse, lightSpecular;
    
    bool		mDIFFUSE;
	bool		mAMBIENT;
	bool		mSPECULAR;
	bool		mEMISSIVE;
	bool		mSHADER;
};