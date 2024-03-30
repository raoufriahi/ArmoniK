// CPictureAnalyser.h: interface for the CPictureAnalyser class.
//
//////////////////////////////////////////////////////////////////////
#ifndef PICTURE_ANALYSER
#define PICTURE_ANALYSER

/*!
* \file CPictureAnalyser.h
* \brief 
* \date 28.11.12 
* \author gp
* \version 0.1
*/
//

#include "ParamAnalytics.h"

struct AS_Polygone {
	CvPoint polyTab [100] ; 
	int id ; 
	int ptNbr;
	int alarmeType ;
	int minSpeed ; 
	int maxSpeed ; 
	int minArea ;
	int maxArea ;
};

class CPictureAnalyser
{
public:
	CPictureAnalyser(void);
	~CPictureAnalyser(void);

public :
	bool process_pa_(char* cameraKey, IplImage* frame, bool bLastPicture);
	bool load_param(char *configuration_path);
	int get_analysis_result();
	int get_analysis_threshold();
	char* get_filter_type();
	int get_buffer_size();
	char* get_buffer();
	bool get_state();	
	bool re_init();
	bool flush();
	int count;

private:
	bool process_image(IplImage* img, const char* camera_key, int timeDistanceMS, bool last_picture);
	void set_value(const CvArr* srcarr, int value);
	int pixel_counter(const CvArr* srcarr, int value);
	int basic_detector(IplImage* current, IplImage* previous, IplImage* mask);
	int basic_detector_first(IplImage* current, IplImage* previous, IplImage* mask);
	int basic_filtering(IplImage* fg, float& probability);
	float whitepix_density_n_pix(const CvArr* srcarr, int kernel_size);
	float whitepix_density_1_pix(const CvArr* srcarr);
	int make_border_n_pix(IplImage* img, int border_size);
	float small_particule_filtering(IplImage* fg, IplImage* current, int max_blob_superficy);
	int add_perspective(IplImage* fg, IplImage* current, int pc /*perspective coefficient*/);
	void grown_cvRect(CvRect& rect, int p, int h);
	float ratio_bwpixels(IplImage* difference);

	void draw_text(IplImage* img, const char* text, int pos_x, int pos_y, CvScalar color);
	void draw_activity(IplImage* img, int channel);
	void draw_results(IplImage* img, char* cameraKey, int probability, int threshold, bool first_picture);
	void draw_mask(IplImage* img, CvScalar color, bool fill_mask);
	void draw_mask_hard(IplImage* img, CvScalar color, bool fill_mask);
	void trace_bin3(char* cameraKey, char* bin, int sz_bin);

private:
#ifdef TRACE_ALARM
	CTraceImage* obj_traceIm;		//!< module de sauvegardes des alarmes
#endif
	AR_SSR_PARAM_CHANNEL_PRESET m_parameters;
	AR_SSR_PARAM_CHANNEL_PRESET m_parameters2;

	IplImage* m_previousImgRAW;	
	IplImage* m_firstImgRAW;	
	IplImage* m_currentImgRAW;
	IplImage* m_currentImgRAW_first;
	IplImage* m_difference;
	IplImage* m_difference_first;

	IplImage* m_current_gray;
	IplImage* m_previous_gray;
	IplImage* m_mask;
	IplImage* m_mask_hard;
	IplImage* m_current_grayM;
	IplImage* m_previous_grayM;

	char m_sFilterType[64];
	unsigned char* m_lppRgbBuffer;
	unsigned char* m_lppJPEGBuffer;
	float m_analysisResult;
	float m_analysisResult_first;//[BLA] result of comparaison with first image.

	bool m_bFirstCall;
	int m_iframeCounter;

	float m_fAverageActivity;
	float m_fAverageWhitePix_density;

	int m_witdh;
	int m_height;

	int m_alarm_threshold;
	int m_default_threshold;
	int m_view_type; // = 1: champ proche  = 2: champ large
	int m_perspective_coefficient;
	int m_perspective_activation;
};

#endif