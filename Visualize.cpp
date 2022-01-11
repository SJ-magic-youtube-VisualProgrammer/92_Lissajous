/************************************************************
************************************************************/
#include "Visualize.h"

/************************************************************
************************************************************/

/******************************
******************************/
VISUALIZE::VISUALIZE()
: Lissajous(500/2, 500/2)
{
	font.load("font/RictyDiminished-Regular.ttf", 10, true, true, true);
}

/******************************
******************************/
VISUALIZE::~VISUALIZE()
{
}

/******************************
******************************/
void VISUALIZE::setup(const FFT& fft)
{
	/********************
	********************/
	print_separatoin();
	printf("> setup : Visualize\n");
	fflush(stdout);
	
	/********************
	********************/
	fbo_out.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	ClearFbo(fbo_out);
	
	VboSet__FFT.setup(AUDIO_BUF_SIZE/2 * 4);
	Refresh_vboVerts__FFT(fft);
	Refresh_vboColor__FFT(fft);
	VboSet__FFT.update_vertex_color();
	
	VboSet__Lissajous.setup(NUM_POINTS_LISSAJOUS);
	Refresh_vboVerts__Lissajous(fft);
	Refresh_vboColor__Lissajous(fft);
	VboSet__Lissajous.update_vertex_color();
}

/******************************
******************************/
void VISUALIZE::ClearFbo(ofFbo& fbo)
{
	fbo.begin();
		ofClear(0, 0, 0, 0);
	fbo.end();
}

/******************************
1--2
|  |
0--3
******************************/
void VISUALIZE::Refresh_vboVerts__Lissajous(const FFT& fft)
{
	/********************
	********************/
	float a, b, theta_a, theta_b;
	
	a = fft.get_Gain2( int(Gui_Global->Li_GainId_a) ) * Gui_Global->Lissajous_Scale;
	theta_a = fft.get_phase_rad_madeFromGain( int(Gui_Global->Li_GainId_a) );
	// theta_a = fft.get_phase_rad( int(Gui_Global->Li_GainId_a) );
	
	b = fft.get_Gain2( int(Gui_Global->Li_GainId_b) ) * Gui_Global->Lissajous_Scale;
	theta_b = fft.get_phase_rad_madeFromGain( int(Gui_Global->Li_GainId_b) );
	// theta_b = fft.get_phase_rad( int(Gui_Global->Li_GainId_b) );
	
	/********************
	********************/
	Lissajous.set(a, theta_a, b, theta_b);
	
	/********************
	********************/
	float t = 0;
	for(int i = 0; i < NUM_POINTS_LISSAJOUS; i++){
		float x, y;
		
		Lissajous.get(x, y, t);
		
		VboSet__Lissajous.set_vboVerts( i, x, y);
		
		t += t_step_Lissajous;
	}
}

/******************************
******************************/
void VISUALIZE::Refresh_vboColor__Lissajous(const FFT& fft)
{
	VboSet__Lissajous.set_singleColor(Gui_Global->col_Lissajous);
}

/******************************
1--2
|  |
0--3
******************************/
void VISUALIZE::Refresh_vboVerts__FFT(const FFT& fft)
{
	const ofVec2f ofs(0, 700);
	const int width_of_bar = 3;
	const int space_per_bar = 5;
	const int BarHeight = 100;
	
	for (int i = 0; i < fft.get_sizeof_GainArray(); i++) {
		VboSet__FFT.set_vboVerts( i * 4 + 0, ofs.x + i * space_per_bar,					ofs.y );
		VboSet__FFT.set_vboVerts( i * 4 + 1, ofs.x + i * space_per_bar,					ofs.y - fft.get_Gain(i) * BarHeight );
		VboSet__FFT.set_vboVerts( i * 4 + 2, ofs.x + i * space_per_bar + width_of_bar,	ofs.y - fft.get_Gain(i) * BarHeight );
		VboSet__FFT.set_vboVerts( i * 4 + 3, ofs.x + i * space_per_bar + width_of_bar,	ofs.y );
	}
}

/******************************
******************************/
void VISUALIZE::Refresh_vboColor__FFT(const FFT& fft)
{
	ofColor col;
	
	for (int i = 0; i < fft.get_sizeof_GainArray(); i++) {
		if ( i == Li_GainId_a || i == Li_GainId_b )	col = ofColor( 0, 255, 0 , 200);
		else										col = ofColor( 0, 100, 255, 200 );
		
		VboSet__FFT.setColor_perShape(4, i, col); // setColor_perShape(int NumPerShape, int id, const ofColor& color);
	}
	// VboSet__Particle.set_singleColor(Gui_Global->col_particle);
}

/******************************
******************************/
void VISUALIZE::toggle_HoriLine()
{
	b_HoriLine = !b_HoriLine;
}


/******************************
******************************/
void VISUALIZE::update(const FFT& fft)
{
	/********************
	********************/
	float now = ofGetElapsedTimef();
	float dt = now - t_Last;
	dt = ofClamp( dt, 0.0, 0.1 );	
	t_Last = now;
	
	Li_GainId_a = (int)Gui_Global->Li_GainId_a;
	Li_GainId_b = (int)Gui_Global->Li_GainId_b;

	/********************
	********************/
	{
		Refresh_vboVerts__FFT(fft);
		VboSet__FFT.update_vertex();
		
		Refresh_vboColor__FFT(fft);
		VboSet__FFT.update_color();
	}
	
	/********************
	********************/
	{
		Refresh_vboVerts__Lissajous(fft);
		VboSet__Lissajous.update_vertex();
		
		Refresh_vboColor__Lissajous(fft);
		VboSet__Lissajous.update_color();
	}
}

/******************************
******************************/
void VISUALIZE::draw(const FFT& fft)
{
	/********************
	********************/
	ofEnableAlphaBlending();
	// ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing(); // OF_BLENDMODE_ADDとofEnableSmoothingは同時に有効化できない
	
	/********************
	********************/
	ClearFbo(fbo_out);
	draw_FFT(fft, fbo_out);
	draw_Lissajous(fft, fbo_out);
	
	/********************
	設定 戻す
	********************/
	ofDisableDepthTest(); // need this to draw gui properly.
	
	/********************
	********************/
	fbo_out.draw(0, 0, ofGetWidth(), ofGetHeight());
}

/******************************
******************************/
void VISUALIZE::draw_Lissajous(const FFT& fft, ofFbo& fbo)
{
	fbo.begin();
		ofDisableDepthTest();
		
		ofPushMatrix();
			ofSetColor(255);
			ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
			
			if(Gui_Global->b_Lissajous_Line){
				glLineWidth(Gui_Global->Lissajous_LineWidth);
				VboSet__Lissajous.draw(GL_LINE_STRIP);
			}else{
				glPointSize(Gui_Global->Lissajous_PointSize);
				VboSet__Lissajous.draw(GL_POINTS);
			}
		ofPopMatrix();
	fbo.end();
}

/******************************
******************************/
void VISUALIZE::draw_FFT(const FFT& fft, ofFbo& fbo)
{
	fbo.begin();
		ofDisableDepthTest();
		
		//Draw background rect for spectrum
		const int line_per = 5;
		const int BarHeight = 100;
		const int width_of_bar = 3;
		const int space_per_bar = 5;
		
		// 座布団 : spectrum
		ofSetColor(255, 255, 255, 20);
		ofFill();
		ofDrawRectangle( 0, 700, fbo.getWidth(), -BarHeight );
		
		// 縦 line
		char buf[256];
		ofSetColor(255, 255, 255, 180);
		for(int i = 0; i * space_per_bar * line_per < fbo.getWidth(); i++){
			int _x = i * space_per_bar * line_per; // i 本目のlineのx座標
			ofDrawLine(_x, 700, _x, 700 - BarHeight );
			
			sprintf( buf, "%d", i * line_per);
			font.drawString(buf, _x, 700 - BarHeight);
		}
		
		// 横 line
		if(b_HoriLine){
			ofSetColor(255, 0, 0, 200);
			ofDrawLine(0,ofGetAppPtr()-> mouseY, fbo.getWidth(), ofGetAppPtr()->mouseY);	// ofApp でないので、mouseYは使えず、ofGetAppPtr()->mouseY とする必要がある。
		}
		
		ofSetColor(255);
		// glPointSize(Gui_Global->particleSize);
		VboSet__FFT.draw(GL_QUADS);
	fbo.end();
}


