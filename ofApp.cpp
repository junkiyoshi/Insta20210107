#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->near_line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
	this->line_list.clear();
	this->near_line.clear();

	for (int i = 0; i < 7; i++) {

		ofMesh line;
		line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
		auto noise_seed = ofRandom(1000);
		//auto v_start = ofGetFrameNum();
		for (int v = 0; v <  360; v += 2) {

			auto p = glm::vec2(300 * cos(v * DEG_TO_RAD), 300 * sin(v * DEG_TO_RAD));
			auto u = ofMap(ofNoise(noise_seed, p.x * 0.002, p.y * 0.002, ofGetFrameNum() * 0.0035), 0, 1, -360, 360);
			line.addVertex(this->make_point(300, 60, u, v));
			if (v > 0) {

				line.addIndex(line.getNumVertices() - 1);
				line.addIndex(line.getNumVertices() - 2);
			}
		}

		line.addIndex(0);
		line.addIndex(line.getNumVertices() - 1);

		this->line_list.push_back(line);
	}


	for (int i = 0; i < this->line_list.size(); i++) {

		for (int k = i + 1; k < this->line_list.size(); k++) {

			for (int m = 0; m < this->line_list[i].getNumVertices(); m++) {

				for (int p = 0; p < this->line_list[k].getNumVertices(); p++) {

					if (glm::distance(this->line_list[i].getVertex(m), this->line_list[k].getVertex(p)) < 30) {

						this->near_line.addVertex(this->line_list[i].getVertex(m));
						this->near_line.addVertex(this->line_list[k].getVertex(p));

						this->near_line.addIndex(this->near_line.getNumVertices() - 1);
						this->near_line.addIndex(this->near_line.getNumVertices() - 2);
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	for (auto& line : this->line_list) {

		line.draw();
	}

	this->near_line.draw();

	for (auto& vertex : this->near_line.getVertices()) {

		ofDrawSphere(vertex, 3.5);
	}

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}