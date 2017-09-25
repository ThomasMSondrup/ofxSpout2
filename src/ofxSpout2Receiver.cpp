#pragma once

#include "ofxSpout2Receiver.h"

using namespace ofxSpout2;

Receiver::Receiver() {
	;
}

void Receiver::setup() {
	mReceiver = new SpoutReceiver;
	mbInitialized = false;
	SenderName[0] = 0;
	//mReceiver->SetCPUmode(false);
	//mReceiver->SetBufferMode(false);
	//mReceiver->SetMemoryShareMode(false);
	//mReceiver->SetShareMode(0);
	//mReceiver->spout.interop.UseDX9(false);
	//mReceiver->spout.interop.OpenDirectX11();
}

void Receiver::setup(string Sendername)
{
	setup();
	strcpy(SenderName, Sendername.c_str());
}


void Receiver::showSenders() {
	mReceiver->SelectSenderPanel();
}

void Receiver::updateTexture() {
	if (!mbInitialized) {
		unsigned int width, height;
		//Here we are creating the receiver. If the SenderName[0]==0 - then we are using the active selection mode - else the passive mode when creating the receiver
		if (mReceiver->CreateReceiver(SenderName, width, height, SenderName[0]==0)) {
			mTexture.allocate(width, height, GL_RGBA);
			mbInitialized = true;
			return;
		}
		else {
			ofLogWarning("ofxSpout", "No sender detected");
		}
	}

	else { // mbInitialized
		assert(mTexture.isAllocated() && "Texture not allocated but receiver initialized!");
		unsigned int preWidth = mTexture.getWidth();
		unsigned int preHeight = mTexture.getHeight();

		unsigned int width = preWidth;
		unsigned int height = preHeight;

		if (mReceiver->ReceiveTexture(SenderName, width, height, mTexture.getTextureData().textureID, mTexture.getTextureData().textureTarget)) {
			if (width != preWidth || height != preHeight) { // in case of size change, reallocate
				mTexture.allocate(width, height, GL_RGBA);
				return;
			}
		}
		else {
			// receiving failed
			mReceiver->ReleaseReceiver();
			mbInitialized = false;
		}
	}
}

void Receiver::exit() {
	if(mReceiver != NULL && mbInitialized)
		mReceiver->ReleaseReceiver();
}