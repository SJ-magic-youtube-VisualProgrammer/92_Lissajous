/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include <ofMain.h>
#include "fft.h"
#include <ofxVboSet.h>
#include "sj_common.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class LISSAJOUS{
private:
	const float A;
	const float B;
	
	float a = 1;
	float b = 1;
	
	float theta_a = 0;
	float theta_b = 3.14 / 180.0;
	
public:
	LISSAJOUS(float _A, float _B) : A(_A), B(_B) { }
	
	void set(float _a, float _theta_a, float _b, float _theta_b){
		a = _a;
		b = _b;
		theta_a = _theta_a;
		theta_b = _theta_b;
	}
	
	float get(float& x, float& y, float t){
		float dt = (float)ofGetElapsedTimeMillis() / 1000.0;
		
		float theta_ofs_a = Gui_Global->Lissajous_theta_rot_speed * 3.1415 / 180.0 * dt;
		float theta_ofs_b = Gui_Global->Lissajous_theta_rot_speed * 3.1415 / 180.0 * dt - 90.0 * 3.1415 / 180.0;
		
		x = A * cos(a * t + theta_a + theta_ofs_a);
		y = B * cos(b * t + theta_b + theta_ofs_b);
		
		/*
		x = A * cos(100 * t + 0);
		y = B * cos(200 * t + 1.57075);
		*/
	}
};

/**************************************************
**************************************************/
class VISUALIZE{
private:
	/****************************************
	****************************************/
	enum{
		NUM_POINTS_LISSAJOUS = 640,
		// NUM_POINTS_LISSAJOUS = 64,
	};
	
	ofx__VBO_SET VboSet__FFT;
	ofx__VBO_SET VboSet__Lissajous;
	
	const float t_step_Lissajous = 0.01;
	
	int Li_GainId_a = 2;
	int Li_GainId_b = 100;
	
	float t_Last = 0;
	
	ofTrueTypeFont font;
	
	bool b_HoriLine = false;
	
	ofFbo fbo_out;
	
	/********************
	********************/
	LISSAJOUS Lissajous;
	
	/****************************************
	****************************************/
	void Refresh_vboVerts__FFT(const FFT& fft);
	void Refresh_vboColor__FFT(const FFT& fft);
	void Refresh_vboVerts__Lissajous(const FFT& fft);
	void Refresh_vboColor__Lissajous(const FFT& fft);
	
	void ClearFbo(ofFbo& fbo);
	
	void draw_FFT(const FFT& fft, ofFbo& fbo);
	void draw_Lissajous(const FFT& fft, ofFbo& fbo);
	
public:
	VISUALIZE();
	~VISUALIZE();
	void setup(const FFT& fft);
	void update(const FFT& fft);
	void draw(const FFT& fft);
	void toggle_HoriLine();
};

