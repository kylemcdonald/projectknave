
#include "FerryBuilding.h"



// ------------------------------------ setup
void FerryBuilding::setupBuilding() {
	
	bSaved		= true;
	bFirstPoint = true;
	bEnable		= false;
	
	
	building.loadImage("buildingRefrences/building.jpg");
	
	mask.loadImage("buildingRefrences/mask_half.png");
	
	// load the building from xml
	if(xmlSaver.loadFile("buildingRefrences/building.xml")) {
		
		int numTags = xmlSaver.getNumTags("building:shape");
		if(numTags > 0) {
			
			xmlSaver.pushTag("building");
			
			int numShapes = xmlSaver.getNumTags("shape:pnt");
			for(int i=0; i<numShapes; i++) {
				
				xmlSaver.pushTag("shape", i);
				shapes.push_back(FerryShape());
				int numPnts = xmlSaver.getNumTags("pnt");
				for(int j=0; j<numPnts; j++) {
					
					float x = xmlSaver.getValue("pnt:x", 0.0, j);
					float y = xmlSaver.getValue("pnt:y", 0.0, j);
					shapes.back().pnts.push_back(ofPoint(x,y,0));
				}
				
				xmlSaver.popTag();
				
			}			
			xmlSaver.popTag();
		}
		
	}
	
	
}

// ------------------------------------ save building
void FerryBuilding::saveBuilding() {
	
	xmlSaver.clear();
	
	xmlSaver.addTag("building");
	xmlSaver.pushTag("building");
	for(int i=0; i<shapes.size(); i++) {
		xmlSaver.addTag("shape");
		xmlSaver.pushTag("shape", i);
		for(int j=0; j<shapes[i].pnts.size(); j++) {
			int ptnum = xmlSaver.addTag("pnt");
			xmlSaver.setValue("pnt:x", shapes[i].pnts[j].x, ptnum);
			xmlSaver.setValue("pnt:y", shapes[i].pnts[j].y, ptnum);
		}
		xmlSaver.popTag();
	}
	xmlSaver.popTag();
	xmlSaver.saveFile("buildingRefrences/building.xml");
	bSaved = true;
	printf("--- building saved ---\n");
}

// ------------------------------------ draw
void FerryBuilding::draw() {
	
	ofSetColor(0xffffff);
	//mask.draw(0, 0);	
	building.draw(0, 0);
}

// ------------------------------------ Draw the building contour
void FerryBuilding::drawContour() {
	
	glLineWidth(2.0);
	for(int i=0; i<shapes.size(); i++) {
		ofNoFill();
		ofSetColor(255, 0, 255);		
		ofBeginShape();
		for(int j=0; j<shapes[i].pnts.size(); j++) {
			ofVertex(shapes[i].pnts[j].x, shapes[i].pnts[j].y);
		}
		ofEndShape(false);
	}
	glLineWidth(1.0);
	
}

// ------------------------------------ clear
void FerryBuilding::clear() {
	for(int i=0; i<shapes.size(); i++) {
		shapes[i].pnts.clear();
	}
	shapes.clear();
}

// ------------------------------------ mouse/keyboard
void FerryBuilding::keyPressed(int key) {
	if(bEnable) {
		
		if(key == ' ') { 
			bSaved		= false;
			bFirstPoint = false;
			shapes.push_back(FerryShape());
		}
		if(key == 'c') clear();
		if(key == 's') saveBuilding();
		
		// undo the last click
		if(key == 'z') {
			if(shapes.size() > 0) {
				// no points left in shape then erase shape
				if(shapes.back().pnts.size() == 0) {
					int lastShape = shapes.size();
					shapes.erase(shapes.begin() + lastShape);
				} else {
					int lastPnt = shapes.back().pnts.size();
					shapes.back().pnts.erase(shapes.back().pnts.begin() + lastPnt);	
				}
			}
		}
	}
	
	// need to turn me on first
	if(key == 'e') bEnable = !bEnable;
	
}


void FerryBuilding::mousePressed(int x, int y, int button) {
	if(bEnable) {
		
		bSaved = false;
		
		// we need one to start
		if(shapes.size() == 0 || bFirstPoint) {
			shapes.push_back(FerryShape());
			shapes.back().pnts.push_back(ofPoint(x,y,0));
			bFirstPoint = false;
		}
		
		shapes.back().pnts.push_back(ofPoint(x,y,0));
	}
	
}


