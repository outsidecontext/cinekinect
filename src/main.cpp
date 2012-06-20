#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
    // uncomment for oooooh
    //window.setGlutDisplayString("rgb alpha double samples depth");
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
    ofRunApp(new testApp());
}
