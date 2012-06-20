#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

#include "PaperMesh.h"
#include "LightConfig.h"

#define LIGHT_COUNT 3

#define DRAW_POINTS 0
#define DRAW_WIREFRAME 1
#define DRAW_FILL 2
#define DRAW_COUNT 3


class testApp : public ofBaseApp {
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void drawPointCloud();
    void draw2d();
    void draw3d(bool drawGrey = false);
    void drawWithSSAO();
    void drawWithBokeh();
    void startLights();
    void endLights();
	
	void keyPressed (int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	ofxKinect kinect;
	
	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
	ofxCvContourFinder contourFinder;
	
    ofFbo meshGreyFbo;
    ofFbo meshColourFbo;
    
    
	bool bThreshWithOpenCV;
	bool bDrawPointCloud;
	int nearThreshold;
	int farThreshold;
	int angle;
	
    // used for viewing 3d stuff
	ofEasyCam easyCam;
    
    // shaderz
    ofShader bokehShader;
    ofShader ssaoShader;
    
    // dof/bokeh
    float focus, blurclamp, bias;
    
    // SSAO
    float camerarangex;
    float camerarangey;
    float aoCap;
    float aoMultiplier;
    float depthTolerance;
    float aorange;
    float readDepthVal;
    
    // params
    bool doDrawTextures, doDraw3d, doDraw3dWithShader, doDrawWithSSAO;
    int drawMode;
    float thresholdZ;
    ofFloatColor bgColour;
    
    // lights
    bool areLightsEnabled;
    ofLight lights[LIGHT_COUNT];
    LightConfig lightConfigs[LIGHT_COUNT];
    
    
    PaperMesh mesh;
};
