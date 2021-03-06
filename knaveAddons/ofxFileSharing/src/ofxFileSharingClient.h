#pragma once

#include <fstream>
#include "ofxNetwork.h"
#include "ofxFilePacket.h"

template <class T>
class ofxFileSharingClient {
protected:
	string filename;
	ofxTCPClient tcpClient;
	T packet;
	long lastAttempt;
	int reconnectTime;
	bool verbose;
public:
	ofxFileSharingClient() {
		reconnectTime = 500;
		verbose = true;
		lastAttempt = ofGetElapsedTimeMillis();
	}
	void setup(string filename, string ip, int port) {
		if(verbose)
			cout << "ofxFileSharingClient: connecting to " << ip << ":" << port << "/" << filename << endl;
		this->filename = filename;
		tcpClient.setup(ip, port);
	}
	bool update() {
		if(tcpClient.isConnected()) {
			if(tcpClient.receive() == "new") {
				ifstream file(filename.c_str(), ios::in | ios::binary);
				if(!file.is_open()) {
					if(verbose)
						cout << "ofxFileSharingClient: couldn't open shared file '" << filename << "'" << endl;
				} else {
					T curPacket;
					file >> curPacket;
					packet = curPacket;
					file.close();
				}
				return true;
			}
		} else {
			long curTime = ofGetElapsedTimeMillis();
			if(curTime - lastAttempt > reconnectTime) {
				if(verbose)
					cout << "ofxFileSharingClient: reconnecting to " << tcpClient.getIP() << ":" << tcpClient.getPort() << endl;
				tcpClient.setup(tcpClient.getIP(), tcpClient.getPort());
				lastAttempt = curTime;
			}
		}
		return false;
	}
	T& getPacket() {
		return packet;
	}
};
