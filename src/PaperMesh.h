//
//  PaperMesh.h
//
//  Created by Chris Mullany on 21/10/2011.
//

#pragma once

#include "ofMain.h"
#include "ofImage.h"
#include "ofVboMesh.h"

#define MAX_MESH_POINTS 2000


//
// PaperMesh
// Creates a 3d mesh whose vertices can be manipulated to create a folded paper effect
// a texture is mapped into the mesh
//
class PaperMesh {
	
public:
	
	// Fields ////////////////////////////////////////////////
    
    float progress, deformChance;
    
    // layout and dimensions
    float columnCount, rowCount, width, height;
    int pointsCount;
    ofVec3f startPoints [MAX_MESH_POINTS];
    ofVec3f targetPoints [MAX_MESH_POINTS];
    
    // states
    bool doExpand, doContract, doDrawWireframe, doDrawTexture, doDrawPoints, doDrawPlane;
    
    // mesh transform
    ofVec3f rot;
    
    // pos
    ofVec3f pos, targetPos;
    
    // mesh
    ofTexture* texture;
    ofVboMesh mesh;
    ofVec3f meshPos;
    bool doGenerateNewMesh;
    float maxZDeform;
    
	// Constructor / Destructor //////////////////////////////
    PaperMesh();
    ~PaperMesh();
    
	// Public Methods ////////////////////////////////////////
	void setup();
    void setup(float width, float height);
	void setupGUI();
	void update();
	void draw();
    
    void drawMesh();
    void setupMesh();
	
	// Handlers //////////////////////////////////////////////
	
	
private:
	
	// Fields ////////////////////////////////////////////////
	// Private Methods ///////////////////////////////////////
	
};
