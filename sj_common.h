/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include <stdio.h>

#include <ofMain.h>
#include <ofxGui.h>

/************************************************************
************************************************************/
#define ERROR_MSG(); printf("Error in %s:%d\n", __FILE__, __LINE__);

/************************************************************
************************************************************/
enum{
	BUF_SIZE_S = 500,
	BUF_SIZE_M = 1000,
	BUF_SIZE_L = 6000,
	BUF_SIZE_LL = 100000,
	UDP_BUF_SIZE = 100000,
};

enum{
	AUDIO_BUF_SIZE = 512,
	
	AUDIO_OUT_CHS = 2,
	AUDIO_IN_CHS = 2,
	AUDIO_BUFFERS = 2,			// 使われてないっぽい
	AUDIO_SAMPLERATE = 44100,
};

enum{
	NUM_HISTORY = 60,
};
	


/************************************************************
************************************************************/

/**************************************************
**************************************************/
class GUI_GLOBAL{
private:
	/****************************************
	****************************************/
	
public:
	/****************************************
	****************************************/
	void setup(string GuiName, string FileName = "gui.xml", float x = 10, float y = 10);
	
	ofxGuiGroup Group_FFT;
		ofxFloatSlider Gain;
		ofxFloatSlider k_smooth;
		ofxFloatSlider dt_smooth_2;
		ofxToggle b_Window;
		
	ofxGuiGroup Group_Lissajous;
		ofxFloatSlider Li_GainId_a;
		ofxFloatSlider Li_GainId_b;
		ofxFloatSlider Lissajous_Scale;
		ofxFloatSlider Lissajous_PointSize;
		ofxFloatSlider Lissajous_LineWidth;
		ofxFloatSlider Lissajous_theta_rot_speed;
		ofxColorSlider col_Lissajous;
		ofxToggle b_Lissajous_Line;
		
	ofxGuiGroup Group_ArtSin;
		ofxFloatSlider ArtSin_Band_min;
		ofxFloatSlider ArtSin_Band_max;
		ofxFloatSlider ArtSin_PhaseMap_k;
		ofxToggle b_ArtSin_abs;
		ofxToggle b_Window_artSin;
		ofxColorSlider col_ArtSin;
		
	ofxPanel gui;
	
	bool b_Disp = false;
};

/************************************************************
************************************************************/
double LPF(double LastVal, double CurrentVal, double Alpha_dt, double dt);
double LPF(double LastVal, double CurrentVal, double Alpha);
double sj_max(double a, double b);

bool checkIf_ContentsExist(char* ret, char* buf);
void Align_StringOfData(string& s);
void print_separatoin();

/************************************************************
************************************************************/
extern GUI_GLOBAL* Gui_Global;

extern FILE* fp_Log;

extern int GPIO_0;
extern int GPIO_1;


/************************************************************
************************************************************/

