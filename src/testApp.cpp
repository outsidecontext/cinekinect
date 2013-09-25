#include "testApp.h"


void testApp::setup(){
    // enable depth->rgb image calibration
	kinect.setRegistration(true);
	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	kinect.open();
    
    ofPlanePrimitive plane(640, 480, 320, 240);
    mesh = plane.getMesh();
    mesh.setColorForIndices(0, mesh.getNumIndices(), ofColor(255,255,255));
    
    // Setup post-processing chain
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<SSAOPass>()->setEnabled(false);
    post.createPass<BloomPass>()->setEnabled(false);
    dof = post.createPass<DofPass>();
    dof->setEnabled(false);
    post.createPass<ToonPass>()->setEnabled(false);
    post.createPass<NoiseWarpPass>()->setEnabled(false);
    post.createPass<PixelatePass>()->setEnabled(false);
    post.createPass<EdgePass>()->setEnabled(false);
    post.createPass<VerticalTiltShifPass>()->setEnabled(false);
    post.createPass<GodRaysPass>()->setEnabled(false);
    
    //light.setPosition(1000, 1000, 2000);
}


void testApp::update(){
    if (!kinect.isConnected()) return;
    kinect.update();
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
        updateMesh();
    }
}

void testApp::updateMesh(){
    float thresholdZ = 2000;
    float lastZ = thresholdZ;
    ofVec3f meshOffset = ofVec3f(320,240,0);
    
    for (int i=0; i<mesh.getNumVertices(); i++) {
        ofVec3f vertex = mesh.getVertex(i);
        ofVec3f kinectVertex = vertex + meshOffset;
        float z = kinect.getDistanceAt(kinectVertex.x, kinectVertex.y);
        if (z > thresholdZ || z <= 0) {
            mesh.setColor(i, ofColor(0, 0, 0, 0));
        } else {
            vertex.z = ofLerp(vertex.z, z, 1);
            //mesh.setColor(i, kinect.getColorAt(kinectVertex.x, kinectVertex.y));
            mesh.setColor(i, ofColor(230,180,10));
            mesh.setVertex(i, vertex);
        }
    }
}

void testApp::draw(){
    if (!kinect.isConnected()) return;
    
    ofBackgroundGradient(ofColor(64), ofColor(0));
    
	// copy enable part of gl state
    glPushAttrib(GL_ENABLE_BIT);
    
    // setup gl state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //light.enable();
    
    // begin scene to post process
    post.begin(cam);
    ofPushMatrix();
    // the projected points are 'upside down' and 'backwards'
    ofScale(1, -1, -1);
    ofTranslate(0, 0, -1000);
	//ofEnableDepthTest();
    
    debugMesh(&mesh);
    if(ofGetKeyPressed('w')) mesh.drawWireframe();
    else if(ofGetKeyPressed('p')) mesh.drawVertices();
    else mesh.draw();
    //ofDrawBox(20);
    //ofDrawSphere(40, 40, 25);
    //light.draw();
	
    //light.disable();
    //ofDisableLighting();
	ofDisableDepthTest();
    ofPopMatrix();
	// end scene and draw
    post.end();
    // set gl state back to original
    glPopAttrib();
    
    //kinect.draw(0, 0, 640/8, 320/8);
	
    // draw help
    ofSetColor(0, 255, 255);
    ofDrawBitmapString("Number keys toggle effects, mouse rotates scene", 10, 20);
    for (unsigned i = 0; i < post.size(); ++i)
    {
        if (post[i]->getEnabled()) ofSetColor(0, 255, 255);
        else ofSetColor(255, 0, 0);
        ostringstream oss;
        oss << i << ": " << post[i]->getName() << (post[i]->getEnabled()?" (on)":" (off)");
        ofDrawBitmapString(oss.str(), 10, 20 * (i + 2));
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

void testApp::debugMesh(ofMesh* mesh){
    // normals debug
    if(ofGetKeyPressed('n')){
        //ofDisableLighting();
        vector<ofVec3f> n = mesh->getNormals();
        vector<ofVec3f> v = mesh->getVertices();
        float normalLength = 5.;
        ofSetColor(255);
        for(int i=0; i < n.size() ;i++){
            ofLine(v[i].x,v[i].y,v[i].z,
                   v[i].x+n[i].x*normalLength,v[i].y+n[i].y*normalLength,v[i].z+n[i].z*normalLength);
            ofLine(.98*v[i].x,.98*v[i].y,.98*v[i].z,
                   .98*v[i].x+n[i].x*normalLength*.2,.98*v[i].y+n[i].y*normalLength*.2,.98*v[i].z+n[i].z*normalLength*.2);
            ofLine(.98*v[i].x+n[i].x*normalLength*.2,.98*v[i].y+n[i].y*normalLength*.2,.98*v[i].z+n[i].z*normalLength*.2,
                   v[i].x+n[i].x*normalLength*.2,v[i].y+n[i].y*normalLength*.2,v[i].z+n[i].z*normalLength*.2);
        }
        //ofEnableLighting();
    }
}

void testApp::keyPressed(int key){
    unsigned idx = key - '0';
    if (idx < post.size()) post[idx]->setEnabled(!post[idx]->getEnabled());
    if (key == OF_KEY_UP) {
        dof->setFocus(dof->getFocus()+.01);
    }
    else if (key == OF_KEY_DOWN) {
        dof->setFocus(dof->getFocus()-.01);
    }
    else if (key == OF_KEY_RIGHT) {
        dof->setAperture(dof->getAperture()+.01);
    }
    else if (key == OF_KEY_LEFT) {
        dof->setAperture(dof->getAperture()-.01);
    }
}


void testApp::keyReleased(int key){

}


void testApp::mouseMoved(int x, int y ){

}


void testApp::mouseDragged(int x, int y, int button){

}


void testApp::mousePressed(int x, int y, int button){

}


void testApp::mouseReleased(int x, int y, int button){

}


void testApp::windowResized(int w, int h){

}


void testApp::gotMessage(ofMessage msg){

}


void testApp::dragEvent(ofDragInfo dragInfo){ 

}
