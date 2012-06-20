#include "testApp.h"
#include "ofxSimpleGuiToo.h"
#include "ofFbo.h"


//--------------------------------------------------------------
void testApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	
    // enable depth->rgb image calibration
	kinect.setRegistration(true);
    
	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	kinect.open();
    
    ofDisableArbTex();
    
    ssaoShader.load("shaders/ssao2DRect.vert", "shaders/ssao2DRect.frag");
    bokehShader.load("shaders/bokeh.vert", "shaders/bokeh.frag");
    
    meshGreyFbo.allocate(ofGetWidth(), ofGetHeight());
    meshColourFbo.allocate(ofGetWidth(), ofGetHeight());
	
	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
    
    doDrawTextures = true;
    doDraw3d = false;
    doDraw3dWithShader = false;
    doDrawWithSSAO = false;
    drawMode = DRAW_WIREFRAME;
    thresholdZ = 1000;
    bgColour = ofFloatColor(0,0,0,0);
	
	nearThreshold = 230;
	farThreshold = 70;
	bThreshWithOpenCV = true;
	
	ofSetFrameRate(60);
	
	// set tile to zero
	angle = 0;
	kinect.setCameraTiltAngle(angle);
	
	
	bDrawPointCloud = false;
    
    // init mesh settings
    mesh.columnCount = 320;
    mesh.rowCount = 240;
    mesh.setup(640, 480);
    
    ofEnableSmoothing();
    
    
    // init openGL lights
    ofSetGlobalAmbientColor(ofColor(0,0,0,1));
    ofSetSmoothLighting(true);
    for (int i = 0; i < LIGHT_COUNT; i++) {
        lightConfigs[i].isEnabled = false;
        lightConfigs[i].type = LIGHT_TYPE_DIR;
        lightConfigs[i].pos = ofVec3f(0,0,0);
        lightConfigs[i].target = ofVec3f(0,0,0);
        lightConfigs[i].concentration = 0;
        lightConfigs[i].cutoff = 0;
        lightConfigs[i].attenuationConst = 0;
        lightConfigs[i].lightColorAmbient = ofFloatColor(0,0,0);
        lightConfigs[i].lightColorDiffuse = ofFloatColor(1,1,1);
        lightConfigs[i].lightSpecular = ofFloatColor(1,1,1);
    }
    
    
    
    //
    // GUI
    //
    gui.setDefaultKeys(true);
    
    gui.addToggle("doDrawTextures", doDrawTextures);
    gui.addToggle("doDraw3d", doDraw3d);
    gui.addToggle("doDraw3dWithShader", doDraw3dWithShader);
    string titles[] = {"point", "wireframe", "fill"};
    gui.addComboBox("drawMode", drawMode, DRAW_COUNT, titles);
    gui.addSlider("thresholdZ", thresholdZ, 0, 2000);
    gui.addColorPicker("bgColour", &bgColour.r);
    gui.addToggle("lights enabled", areLightsEnabled);
    gui.addSlider("focus", focus, 0, 10);
    gui.addSlider("blurclamp", blurclamp, 0, 100);
    gui.addSlider("bias", bias, 0, 100);
    
    
    gui.addTitle("SSAO Setting");
    gui.addToggle("doDrawWithSSAO", doDrawWithSSAO);
	gui.addSlider("camerarangex", camerarangex, 0, 1000);
	gui.addSlider("camerarangey", camerarangey, 0, 1000);
	gui.addSlider("aoCap", aoCap, 0.0, 10.0);
	gui.addSlider("aoMultiplier", aoMultiplier, 0.0, 20000.0);
	gui.addSlider("depthTolerance", depthTolerance, 0.000, 0.002);
	gui.addSlider("aorange", aorange, 0.0, 2.0);
	gui.addSlider("readDepthVal", readDepthVal, 0.0, 20.0);
    	
	camerarangex = 6113.28;
	camerarangey = 4121.09;
	aoCap = 1.8795;
	aoMultiplier = 1523.5625;
	depthTolerance = 0.0001130;
	aorange = 0.285156;
	readDepthVal = 2.0;
    
    
    //lights
    string LightTypeLabels[] = {"directional","spot","point"};
    for (int i = 0; i < LIGHT_COUNT; i++) {
        gui.addPage( ofToString(i) + " light" );
        gui.addToggle( ofToString(i) + " light enabled", lightConfigs[i].isEnabled);
        gui.addComboBox(ofToString(i) + " light type", lightConfigs[i].type, 3,  LightTypeLabels);
        gui.addSlider( ofToString(i) + " pos x", lightConfigs[i].pos.x, -2000, 2000);
        gui.addSlider( ofToString(i) + " pos y", lightConfigs[i].pos.y, -2000, 2000);
        gui.addSlider( ofToString(i) + " pos z", lightConfigs[i].pos.z, -2000, 2000);
        gui.addSlider( ofToString(i) + " target x", lightConfigs[i].target.x, -1000, 1000);
        gui.addSlider( ofToString(i) + " target y", lightConfigs[i].target.y, -1000, 1000);
        gui.addSlider( ofToString(i) + " target z", lightConfigs[i].target.z, -1000, 1000);
        gui.addSlider( ofToString(i) + " conc", lightConfigs[i].concentration, 0, 100);
        gui.addSlider( ofToString(i) + " cutoff", lightConfigs[i].cutoff, 0, 100);
        gui.addSlider( ofToString(i) + " attenuation const", lightConfigs[i].attenuationConst, 0, 2);
        gui.addColorPicker( ofToString(i) + " colour ambient", &lightConfigs[i].lightColorAmbient.r);
        gui.addColorPicker( ofToString(i) + " colour diffuse", &lightConfigs[i].lightColorDiffuse.r);
        gui.addColorPicker( ofToString(i) + " colour specular", &lightConfigs[i].lightSpecular.r);
    }
    
    gui.loadFromXML();
}


void testApp::update() {
    
    ofBackground(bgColour.r*255, bgColour.g*255, bgColour.b*255);
	
	kinect.update();
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
        
        if (doDraw3dWithShader || doDrawWithSSAO) {
            // depth texture
            meshGreyFbo.begin();
            ofClear(0, 0, 0, 0);
            draw3d(true);
            meshGreyFbo.end();
            // colour texture
            meshColourFbo.begin();
            ofClear(0, 0, 0, 0);
            draw3d(false);
            meshColourFbo.end();
        }
        
	}
    
}


void testApp::draw() {
    
	if (doDraw3d) draw3d();
    if (doDrawWithSSAO) drawWithSSAO();
    if (doDraw3dWithShader) drawWithBokeh();
    if (doDrawTextures) draw2d();
    //meshGreyFbo.draw(0, 0);
    
    gui.draw();
}


void testApp::draw2d(){
    // draw 2d kinect images/textures
    kinect.drawDepth(10, 10, 400, 300);
    kinect.draw(420, 10, 400, 300);
    //grayImage.draw(10, 320, 400, 300);
    //contourFinder.draw(10, 320, 400, 300);
}


void testApp::draw3d(bool drawGrey){
    
    easyCam.begin();
    startLights();
    
    ofPushMatrix();
    
    // the projected points are 'upside down' and 'backwards' 
    ofScale(1, 1, -1);
    ofTranslate(0, 0, -1000);
    
    glEnable(GL_DEPTH_TEST);
    //ofEnableAlphaBlending();
    
    
    // update mesh vertices
    float lastZ = thresholdZ;
    float grey;
    for(int i=0; i<mesh.mesh.getNumVertices(); i++) {
        ofVec3f mV = mesh.mesh.getVertex(i);
        mV.z = kinect.getDistanceAt(mV.x, mV.y);
        
        if (mV.z > thresholdZ || mV.z <= 0) {
            mV.z = lastZ;
            if (drawGrey) {
                mesh.mesh.setColor( i, ofColor(0,0,0,255) );
            } else {
                mesh.mesh.setColor(i, ofFloatColor(bgColour.r, bgColour.g, bgColour.b, bgColour.a));
            }
        } else {
            lastZ = mV.z;
            if (drawGrey) {
                grey = ofMap(mV.z, 600, thresholdZ, 255, 0, true);
                mesh.mesh.setColor( i, ofColor(grey,grey,grey,255) );
            } else {
                mesh.mesh.setColor(i, kinect.getColorAt(mV.x, mV.y));
            }
        }
        mesh.mesh.setVertex(i, mV);
    }
    
    // update mesh draw mode
    if (drawMode == DRAW_POINTS){
        mesh.doDrawPoints = true;
        mesh.doDrawWireframe = false;
        mesh.doDrawPlane = false;
    } else if (drawMode == DRAW_WIREFRAME){
        mesh.doDrawPoints = false;
        mesh.doDrawWireframe = true;
        mesh.doDrawPlane = false;
    } else if (drawMode == DRAW_FILL){
        mesh.doDrawPoints = false;
        mesh.doDrawWireframe = false;
        mesh.doDrawPlane = true;
    }
    
    if (drawGrey) {
        mesh.doDrawPoints = false;
        mesh.doDrawWireframe = false;
        mesh.doDrawPlane = true;
    }
    
    // draw mesh
    mesh.draw();
    
    
    //ofDisableAlphaBlending();
    glDisable(GL_DEPTH_TEST);
    
    ofPopMatrix();
    
    endLights();
    easyCam.end();
}


void testApp::drawWithSSAO(){
    
    ssaoShader.begin();
    
	glActiveTexture(GL_TEXTURE0);
    meshGreyFbo.getTextureReference().bind();
	glActiveTexture(GL_TEXTURE1);
    meshColourFbo.getTextureReference().bind();
    
    bokehShader.setUniform1i("texture0", 0);
    bokehShader.setUniform1i("texture1", 1);
    ssaoShader.setUniform2f("screensize", ofGetWidth(), ofGetHeight());
    ssaoShader.setUniform2f("camerarange", camerarangex, camerarangey);
    ssaoShader.setUniform1f("aoCap", aoCap);
    ssaoShader.setUniform1f("aoMultiplier", aoMultiplier);
    ssaoShader.setUniform1f("depthTolerance", depthTolerance);
    ssaoShader.setUniform1f("aorange", aorange);
    ssaoShader.setUniform1f("readDepthVal", readDepthVal);
    
    float w = meshGreyFbo.getWidth();
    float h = meshGreyFbo.getHeight();
    
	// draw quads 
	glBegin(GL_QUADS);  
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, h);
	glVertex3f(0, h, 0);  
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	glVertex3f(0, 0, 0);  
	glMultiTexCoord2f(GL_TEXTURE0, w, 0.0f);
	glVertex3f(w, 0, 0);  
	glMultiTexCoord2f(GL_TEXTURE0, w, h);
	glVertex3f(w, h, 0);  
	glEnd();
    
	// unbind the textures
	glActiveTexture(GL_TEXTURE0);
    meshGreyFbo.getTextureReference().unbind();
	glActiveTexture(GL_TEXTURE1);
    meshColourFbo.getTextureReference().unbind();
    
    ssaoShader.end();
    
}


void testApp::drawWithBokeh(){
    
    bokehShader.begin();
    
	// bind textures
	glActiveTexture(GL_TEXTURE0);
    meshColourFbo.getTextureReference().bind();
	glActiveTexture(GL_TEXTURE1);
    meshGreyFbo.getTextureReference().bind();
    
    bokehShader.setUniform1i("tex0", 0);
    bokehShader.setUniform1i("tex1", 1);
    bokehShader.setUniform1f("blurclamp", blurclamp);
    bokehShader.setUniform1f("bias", bias);
    bokehShader.setUniform1f("focus", focus);
    
    float w = meshGreyFbo.getWidth();
    float h = meshGreyFbo.getHeight();
    
	// draw quads 
	glBegin(GL_QUADS);  
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, h);
	glVertex3f(0, h, 0);  
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	glVertex3f(0, 0, 0);  
	glMultiTexCoord2f(GL_TEXTURE0, w, 0.0f);
	glVertex3f(w, 0, 0);  
	glMultiTexCoord2f(GL_TEXTURE0, w, h);
	glVertex3f(w, h, 0);  
	glEnd();
    
	// unbind the textures
	//glActiveTexture(GL_TEXTURE1);
    meshGreyFbo.getTextureReference().unbind();
	glActiveTexture(GL_TEXTURE0);
    meshColourFbo.getTextureReference().unbind();
    
	bokehShader.end();
    
}

void testApp::drawPointCloud() {
	int w = 640;
	int h = 480;
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_POINTS);
	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			if(kinect.getDistanceAt(x, y) > 0) {
				mesh.addColor(kinect.getColorAt(x,y));
				mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
			}
		}
	}
    //mesh.setupIndicesAuto();
	glPointSize(3);
	ofPushMatrix();
	// the projected points are 'upside down' and 'backwards' 
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -1000); // center the points a bit
	glEnable(GL_DEPTH_TEST);
	mesh.drawVertices();
    mesh.draw();
	glDisable(GL_DEPTH_TEST);
	ofPopMatrix();
}

void testApp::startLights(){
    //
    // lights
    if (areLightsEnabled) {
        ofEnableLighting();
        //glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        for (int i = 0; i < LIGHT_COUNT; i++) {
            if (lightConfigs[i].isEnabled) {
                lights[i].enable();
                // Set light type
                switch (lightConfigs[i].type) {
                    case LIGHT_TYPE_DIR:
                        lights[i].setDirectional();
                        break;
                    case LIGHT_TYPE_SPOT:
                        lights[i].setSpotlight();
                        lights[i].lookAt(lightConfigs[i].target);
                        lights[i].setSpotConcentration(lightConfigs[i].concentration);
                        lights[i].setSpotlightCutOff(lightConfigs[i].cutoff);
                        lights[i].setAttenuation(lightConfigs[i].attenuationConst, 0, 0);
                        break;
                    case LIGHT_TYPE_POINT:
                        lights[i].setPointLight();
                        break;
                    default:
                        lights[i].setDirectional();
                        break;
                }
                // set pos and colours
                lights[i].setPosition(lightConfigs[i].pos);
                lights[i].setAmbientColor(lightConfigs[i].lightColorAmbient);
                lights[i].setDiffuseColor(lightConfigs[i].lightColorDiffuse);
                lights[i].setSpecularColor(lightConfigs[i].lightSpecular);
            } else {
                // light is disabled
                lights[i].disable();
            }
        }
    } else {
        ofDisableLighting();
    }
}

void testApp::endLights(){
    if (areLightsEnabled) {
        ofDisableLighting();
    }
}


void testApp::exit() {
	kinect.setCameraTiltAngle(0);
	kinect.close();
}


void testApp::keyPressed (int key) {
	switch (key) {
		case ' ':
			bThreshWithOpenCV = !bThreshWithOpenCV;
			break;
			
		case'p':
			bDrawPointCloud = !bDrawPointCloud;
			break;
			
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
			
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
			
		case 'o':
			kinect.setCameraTiltAngle(angle); // go back to prev tilt
			kinect.open();
			break;
			
		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;
			
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
	}
}


void testApp::mouseDragged(int x, int y, int button){}


void testApp::mousePressed(int x, int y, int button){}


void testApp::mouseReleased(int x, int y, int button){}


void testApp::windowResized(int w, int h){}

