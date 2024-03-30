///////////////////////////////////////////////////////////////////////////////
/**
 * @file    CPictureAnalyser.cpp
 *
 * @note    
 *
 * @author 
 *
 * @par     email
 *          
 * @date    
 *
 * @par     
 *            
 *
 * @par     Licences
 *          Copyright (C)
 *
 * @brief   
 *          
 */
////////////////////////////////////////////////////////////////////////////////
#include "include.h"
#include "CPictureAnalyser.h"
#include <iostream>
#include <stdio.h>
using namespace std;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPictureAnalyser::CPictureAnalyser()
{
#ifdef TRACE_ALARM
	obj_traceIm	= NULL; //object visual traces
#endif

	
//#define TRACE_HISTO //sort les resultats intermedaire a l 'écran.
	


	m_analysisResult = 0;
	m_analysisResult_first = 0;
	m_difference = 0;
	m_difference_first = 0;
	m_previous_gray = 0;
	m_current_gray = 0;
	m_previousImgRAW = 0;
	m_firstImgRAW = 0;//[BLA]

	m_currentImgRAW = 0;
	m_currentImgRAW_first = 0;
	m_mask = 0;

	m_current_grayM = 0;
	m_previous_grayM = 0;

	m_iframeCounter	= 0;

	m_fAverageActivity = 0;
	m_fAverageWhitePix_density = 0;

	m_bFirstCall = true;

	m_alarm_threshold = 0;
	m_view_type = 2; // = 1: champ proche  = 2: champ large
	m_default_threshold = 0;
	m_perspective_coefficient = -1;
	m_perspective_activation = -1;


	count=0;
}
CPictureAnalyser::~CPictureAnalyser()
{
	this->flush();
}
 
//////////////////////////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////////////////////////
bool CPictureAnalyser::process_pa_(char* cameraKey, IplImage* frame, bool bLastPicture = true)
{	
	if(m_firstImgRAW == 0) //[BLA]
		m_firstImgRAW = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);

	if(m_previousImgRAW == 0)
		m_previousImgRAW = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);

	if(m_currentImgRAW == 0)
		m_currentImgRAW = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);

	if(m_currentImgRAW_first == 0) //[BLA]
		m_currentImgRAW_first = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);

	if(m_mask == 0)
		m_mask = cvCreateImage(cvGetSize(frame), 8, 1);
	
	if(frame->imageSize != m_previousImgRAW->imageSize)
	{
		this->flush();
		if(m_previousImgRAW == 0)
			m_previousImgRAW = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);	
		if(m_currentImgRAW == 0)
			m_currentImgRAW = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);			
		if(m_mask == 0)
			m_mask = cvCreateImage(cvGetSize(frame), 8, 1);

		if(m_firstImgRAW == 0) //[BLA]
		m_firstImgRAW = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels); //[BLA]
		
		if(m_currentImgRAW_first == 0) //[BLA]
			m_currentImgRAW_first = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);	


	}
	
	cvCopy(frame, m_currentImgRAW);
	cvCopy(frame, m_currentImgRAW_first);


	//set mask
	if(m_mask != NULL)
	{
		set_value(m_mask, 255);
		//draw mask on mask buffer
		draw_mask(m_mask, CV_RGB(0, 0, 0), true);
		////draw mask on current frame
		//draw_mask(frame, CV_RGB(0, 0, 0), false);
	}

	// --- sauvegarde image vierge
	#ifdef TRACE_ALARM
	if(obj_traceIm == NULL)
		obj_traceIm = new CTraceImage(rep_trace_ssr);
	obj_traceIm->set_folder(cameraKey);
	//obj_traceIm->trace_image(m_currentImgRAW, cameraKey);
	#endif

	m_iframeCounter++;

	if(m_bFirstCall)
	{
		//save previous image
		memcpy(m_previousImgRAW->imageData, m_currentImgRAW->imageData, frame->imageSize);

		memcpy(m_firstImgRAW->imageData, m_currentImgRAW->imageData, frame->imageSize);//[BLA]
		//display info
		//char txt[256];
		//sprintf(txt,"Frame %d", m_iframeCounter);
		//draw_text(frame, txt, 5, frame->height - 5, CV_RGB(128, 128, 128));
		m_bFirstCall = false;
		
		//display activity considering threshold
		draw_results(frame, cameraKey, (int)m_analysisResult, m_alarm_threshold, true);
	}
	else
	{
		#ifdef _DEBUG
		if(m_mask != 0)
		{
			cvResizeWindow("m_mask", m_mask->width, m_mask->height);
			cvNamedWindow("m_mask", 0);
			cvShowImage("m_mask", m_mask);
			cvWaitKey(1);

		}
		#endif

	

		//process analysis
		
		//Comparaison between first and current: m_difference_first is computed
		//m_analysisResult_first = (float) basic_detector_first(m_currentImgRAW_first,m_firstImgRAW, m_mask); 
		
		//Comparaison between previous and current: m_difference is computed
		m_analysisResult = (float) basic_detector(m_currentImgRAW, m_previousImgRAW, m_mask);

		//Max is computed at this step :
		basic_filtering(m_difference, m_analysisResult);//intermediare result is computed.
		//basic_filtering(m_difference_first, m_analysisResult);//second max is done.

		//[BLA]
		#ifdef TRACE_HISTO
			cvResizeWindow("image diff with previous/current method", m_difference->width, m_difference->height);
			cvNamedWindow("image diff with previous/current method", 0);
			cvShowImage("image diff with previous/current method",m_difference);
		
			cvResizeWindow("image diff with first/current method", m_difference_first->width, m_difference_first->height);
			cvNamedWindow("image diff with first/current method", 0);
			cvShowImage("image diff with first/current method",m_difference_first);
		#endif

		//this->small_particule_filtering(m_difference, frame, 150);//devra prendre en compte le filtrage selon les dimensions (cm) des blobs
		//this->make_border_n_pix(m_difference, 25);
		
		//[BLA]
		//if(m_perspective_activation > 0 &&
		//	m_perspective_coefficient > 0)
		//	this->add_perspective(m_difference, m_currentImgRAW, m_perspective_coefficient);


		//save previous image
		memcpy(m_previousImgRAW->imageData, frame->imageData, frame->imageSize);	
		
		//display activity considering threshold 
		draw_results(frame, cameraKey, (int)m_analysisResult, m_alarm_threshold, false);

		// --- write graphical traces
		#ifdef TRACE_ALARM
		/*if(obj_traceIm != NULL)
		{	
			if(m_current_grayM != 0)
				obj_traceIm->trace_image_processed(m_current_grayM, cameraKey);
			if(m_previous_grayM != 0)
				obj_traceIm->trace_image_processed(m_previous_grayM, cameraKey);
		}*/
		#endif
	}

	//draw mask on current frame
	draw_mask(frame, CV_RGB(0, 0, 0), false);

	#ifdef _DEBUG
	char trace[256];
	sprintf(trace, "numero image: %d	probabilite: %d", m_iframeCounter, m_analysisResult); 
	tracer_info(trace);
	#endif

	#ifdef _DEBUG
	if(m_difference != 0)
	{
		cvResizeWindow("current", m_difference->width, m_difference->height);
		cvNamedWindow("current", 0);
		cvShowImage("current", m_difference);
		cvWaitKey(1);
	}
	#endif

	// --- write graphical traces
	#ifdef TRACE_ALARM
	if(obj_traceIm != NULL)
	{	
		//obj_traceIm->trace_image_processed(m_mask, cameraKey);	
		obj_traceIm->trace_image_processed(frame, cameraKey);	
	}
	#endif

	// --- sauvegarde des paramètres
	#ifdef TRACE_ALARM
	if(obj_traceIm != NULL)
	{
		obj_traceIm->trace_bin(cameraKey, (char*)&m_parameters, sizeof(AR_SSR_PARAM_CHANNEL_PRESET));
		obj_traceIm->trace_ini("FILM_ANALYSIS", "default_threshold", m_default_threshold);
		obj_traceIm->trace_ini("FILM_ANALYSIS", "alarm_threshold", m_alarm_threshold);
		obj_traceIm->trace_ini("FILM_ANALYSIS", "perspective_coefficient", m_perspective_coefficient);
	}
	#endif

	// --- sauvegarde de la probabilité + seuil de détection
	#ifdef TRACE_ALARM 
	if(obj_traceIm != NULL)
		obj_traceIm->trace_results(cameraKey, m_analysisResult, m_alarm_threshold);
	#endif

	return true;
}
bool CPictureAnalyser::load_param(char *configuration_path)
{//chargement des paramètres

	//current camera key ini
	char FileName[256];
	sprintf(FileName,"%s\\film_analysis_options.ini", configuration_path);
	m_default_threshold = GetPrivateProfileInt("FILM_ANALYSIS", "default_threshold", -1, FileName);
	m_perspective_coefficient = GetPrivateProfileInt("FILM_ANALYSIS", "view_type", -1, FileName);

	//general ini
	char FileNameAnChannel[128];
	sprintf(FileNameAnChannel,"%s\\ini\\AnChannel.ini", rep_config);
	m_perspective_activation = GetPrivateProfileInt("GENERAL", "perspective_activation", 0, FileNameAnChannel);

	if(m_default_threshold == 0)
	{// seuil fixé par caméra
		m_alarm_threshold = GetPrivateProfileInt("FILM_ANALYSIS", "alarm_theshold", 0, FileName);
	}
	else /*if(m_default_threshold == 1 || m_default_threshold == -1)*/
	{//seuil par défaut d'un serveur d'analyse
		m_alarm_threshold = GetPrivateProfileInt("GENERAL", "threshold", 0, FileNameAnChannel);
	}

	char trace[256];
	sprintf(trace, "load_param: configuration_path = %s", configuration_path); 
	tracer_info(trace);
	sprintf(trace, "load_param: film_analysis_options.ini	alarm_threshold: %d view_type: %d", m_alarm_threshold, m_view_type); 
	tracer_info(trace);

	//load bin
	sprintf(FileName,"%s\\FAEngineParameters.bin", configuration_path);
	memset(&m_parameters, -1, sizeof(AR_SSR_PARAM_CHANNEL_PRESET));
	int size_buffer = GetTailleFichier(FileName);

	if(size_buffer > 0)
	{	
		char param[63000];	
		ChargeFichierEnMemoire(FileName, (char*)param, size_buffer);
		memcpy(&m_parameters, param, sizeof(AR_SSR_PARAM_CHANNEL_PRESET));

		char trace[256];
		sprintf(trace, "load_param: FAEngineParameters.bin	nbMask = %d", m_parameters.NbRulePolyMask); 
		tracer_info(trace);

		return true;
	}
	else
		return false;
}
bool CPictureAnalyser::get_state()
{
	return m_bFirstCall;
}
int CPictureAnalyser::get_analysis_result()
{
	return (int)m_analysisResult;
}
int CPictureAnalyser::get_analysis_threshold()
{
	return (int)m_alarm_threshold;
}
char* CPictureAnalyser::get_filter_type()
{
	return m_sFilterType;
}
bool CPictureAnalyser::flush()
{
	if(m_firstImgRAW != 0)
		cvReleaseImage(&m_firstImgRAW);	
	if(m_currentImgRAW_first != 0) //[BLA]
		cvReleaseImage(&m_currentImgRAW_first);//[BLA]
	if(m_previousImgRAW != 0)
		cvReleaseImage(&m_previousImgRAW);	
	if(m_currentImgRAW != 0)
		cvReleaseImage(&m_currentImgRAW);
	if(m_difference != 0)
		cvReleaseImage(&m_difference);
	if(m_difference_first != 0)
		cvReleaseImage(&m_difference_first);
	if(m_current_gray != 0)
		cvReleaseImage(&m_current_gray);
	if(m_previous_gray != 0)
		cvReleaseImage(&m_previous_gray);
	if(m_mask != 0)
		cvReleaseImage(&m_mask);
	if(m_current_grayM != 0)
		cvReleaseImage(&m_current_grayM);
	if(m_previous_grayM != 0)
		cvReleaseImage(&m_previous_grayM);
	if(m_mask != 0)
		cvReleaseImage(&m_mask);

	m_previousImgRAW = 0;
	m_firstImgRAW=0;
	m_difference = 0;
	m_difference_first = 0;
	m_previous_gray = 0;
	m_current_gray = 0;
	m_mask = 0;
	m_current_grayM = 0;
	m_previous_grayM = 0;

	return true;
}
bool CPictureAnalyser::re_init()
{
	m_analysisResult = 0;
	m_analysisResult_first = 0;
	m_iframeCounter	= 0;
	m_bFirstCall = true;
	m_fAverageWhitePix_density = 0;
	m_fAverageActivity = 0;

	#ifdef TRACE_ALARM
	if(obj_traceIm != NULL)
		obj_traceIm->init_trace(rep_trace_ssr);
	#endif

	return true;
}

//////////////////////////////////////////////////////////////////////
// detector
//////////////////////////////////////////////////////////////////////
int CPictureAnalyser::basic_detector(IplImage* current, IplImage* previous, IplImage* mask)
{
	// --- be sure we have a valid buffer

	if( (current->width > 0) &&
	    (current->height > 0) && 
		(current->imageSize > 0)) 
	{
		if(m_difference != NULL)
		{
			if( current->width != m_difference->width	|| 
				current->height != m_difference->height )
			{
				if(m_difference != 0)
					cvReleaseImage(&m_difference);
				if(m_current_gray != 0)
					cvReleaseImage(&m_current_gray);
				if(m_previous_gray != 0)
					cvReleaseImage(&m_previous_gray);
				if(m_current_grayM != 0)
					cvReleaseImage(&m_current_grayM);
				if(m_previous_grayM != 0)
					cvReleaseImage(&m_previous_grayM);
				m_difference = 0;
				m_current_gray = 0;
				m_previous_gray = 0;
				m_current_grayM = 0;
				m_previous_grayM = 0;
			}
		}

		if(m_difference == 0)
			m_difference = cvCreateImage(cvGetSize(current), 8, 1);
		if(m_current_gray == 0)
			m_current_gray = cvCreateImage(cvGetSize(current), 8, 1);
		if(m_previous_gray == 0)
			m_previous_gray = cvCreateImage(cvGetSize(current), 8, 1);
		if(m_current_grayM == 0)
			m_current_grayM = cvCreateImage(cvGetSize(current), 8, 1);
		if(m_previous_grayM == 0)
			m_previous_grayM = cvCreateImage(cvGetSize(current), 8, 1);
			
		//cvSmooth(current, current, CV_MEDIAN);
		cvSmooth(current, current, CV_BLUR);
		cvSmooth(previous, previous, CV_BLUR);

		cvCvtColor(current, m_current_gray, CV_RGB2GRAY);   //BLA: CV_BGR2GRAY
		cvCvtColor(previous, m_previous_gray, CV_RGB2GRAY); //BLA: CV_BGR2GRAY


		#ifdef _DEBUG
			if(m_current_gray != 0)
			{
				cvResizeWindow("m_current_gray", m_current_gray->width, m_current_gray->height);
				cvNamedWindow("m_current_gray", 0);
				cvShowImage("m_current_gray", m_current_gray);
				cvWaitKey(1);
			}
			#endif
		//load mask
		
		
		if(mask!=NULL)	
		{
			
			cvMin(mask, m_current_gray, m_current_grayM);
			cvMin(mask, m_previous_gray, m_previous_grayM);
			cvAbsDiff(m_current_grayM, m_previous_grayM, m_difference);


			


			#ifdef _DEBUG
			if(m_current_grayM != 0)
			{
				cvResizeWindow("m_current_grayM", m_current_grayM->width, m_current_grayM->height);
				cvNamedWindow("m_current_grayM", 0);
				cvShowImage("m_current_grayM", m_current_grayM);
				cvWaitKey(1);
			}
			#endif
			
			
		
		}
		else
		cvAbsDiff(m_current_gray, m_previous_gray, m_difference);
		
		#ifdef TRACE_HISTO
		  cvResizeWindow("m_difference", m_difference->width, m_difference->height);
		  cvNamedWindow("Trace Diff images", 0);
		  cvShowImage("Trace Diff images",m_difference);
		  cvWaitKey(0); 

		  
		  //TODO Compute histogramme.
		  Mat src;
		  const Mat dst=Mat(m_difference,FALSE);
		  int histSize = 256;
		 
		  float range[] = { 0, 256 } ;
		  const float* histRange = { range };

		  MatND b_hist, g_hist, r_hist;
		  bool uniform = true; bool accumulate = false;

		  /// Compute the histograms:
		  int channels[] = {0};
		  calcHist(&dst, 1, channels, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
//		  calcHist(&dst, 1, channels, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
//		  calcHist(&dst, 1, channels, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

		  // Draw the histograms for B, G and R
          int hist_w = 640; int hist_h = 480;
		  int bin_w = cvRound( (double) hist_w/histSize );

		  Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

		  /// Normalize the result to [ 0, histImage.rows ]
		  normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
//		  normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
//		  normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

		  /// Draw for each channel

		  
		  for( int i = 1; i < histSize; i++ )
		  {
			line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
//			line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
//                       Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
//                       Scalar( 0, 255, 0), 2, 8, 0  );
//			line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
//                       Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
//                       Scalar( 0, 0, 255), 2, 8, 0  );
		  }

		  /// Display
		  namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
		  imshow("calcHist Demo", histImage );
		  cvWaitKey(0); 


		  
	    #endif


		
		cvThreshold(m_difference, m_difference, 20, 255,CV_THRESH_BINARY);//[BLA]40//CV_THRESH_OTSU);//BINARY);
		cvErode(m_difference, m_difference, 0, 0);//gp was 2
		cvDilate(m_difference, m_difference, 0, 0);//gp was 2


			


	}

	return 0;
}



int CPictureAnalyser::basic_detector_first(IplImage* current, IplImage* previous, IplImage* mask)
{
	// --- be sure we have a valid buffer

	if( (current->width > 0) &&
	    (current->height > 0) && 
		(current->imageSize > 0)) 
	{
		if(m_difference_first != NULL)
		{
			if( current->width != m_difference_first->width	|| 
				current->height != m_difference_first->height )
			{
				if(m_difference_first != 0)
					cvReleaseImage(&m_difference_first);
				if(m_current_gray != 0)
					cvReleaseImage(&m_current_gray);
				if(m_previous_gray != 0)
					cvReleaseImage(&m_previous_gray);
				if(m_current_grayM != 0)
					cvReleaseImage(&m_current_grayM);
				if(m_previous_grayM != 0)
					cvReleaseImage(&m_previous_grayM);
				m_difference_first = 0;
				m_current_gray = 0;
				m_previous_gray = 0;
				m_current_grayM = 0;
				m_previous_grayM = 0;
			}
		}

		if(m_difference_first == 0)
			m_difference_first = cvCreateImage(cvGetSize(current), 8, 1);
		if(m_current_gray == 0)
			m_current_gray = cvCreateImage(cvGetSize(current), 8, 1);
		if(m_previous_gray == 0)
			m_previous_gray = cvCreateImage(cvGetSize(current), 8, 1);
		if(m_current_grayM == 0)
			m_current_grayM = cvCreateImage(cvGetSize(current), 8, 1);
		if(m_previous_grayM == 0)
			m_previous_grayM = cvCreateImage(cvGetSize(current), 8, 1);
			
		//cvSmooth(current, current, CV_MEDIAN);
		cvSmooth(current, current, CV_BLUR);
		cvSmooth(previous, previous, CV_BLUR);

		//cvCvtColor(current, m_current_gray, CV_RGB2GRAY);
		//cvCvtColor(previous, m_previous_gray, CV_RGB2GRAY);
		cvCvtColor(current, m_current_gray, CV_RGB2GRAY);
		cvCvtColor(previous, m_previous_gray, CV_RGB2GRAY);

		//load mask
		
		

		if(mask!=NULL)	
		{
			
			cvMin(mask, m_current_gray, m_current_grayM);
			cvMin(mask, m_previous_gray, m_previous_grayM);
			cvAbsDiff(m_current_grayM, m_previous_grayM, m_difference_first);


			


			#ifdef _DEBUG
			if(m_current_grayM != 0)
			{
				cvResizeWindow("m_current_grayM", m_current_grayM->width, m_current_grayM->height);
				cvNamedWindow("m_current_grayM", 0);
				cvShowImage("m_current_grayM", m_current_grayM);
				cvWaitKey(1);
			}
			#endif
			
			
		
		}
		else
		cvAbsDiff(m_current_gray, m_previous_gray, m_difference_first);
		cvThreshold(m_difference_first, m_difference_first, 20, 255,CV_THRESH_BINARY);//40//CV_THRESH_OTSU);//BINARY);//40
		cvErode(m_difference_first, m_difference_first, 0, 0);//gp was 2
		cvDilate(m_difference_first, m_difference_first, 0, 0);//gp was 2


			


	}

	return 0;
}






//////////////////////////////////////////////////////////////////////
// filters
//////////////////////////////////////////////////////////////////////
int CPictureAnalyser::basic_filtering(IplImage* fg, float& probability)
{
	//process filters
	/*m_fAverageActivity = (m_fAverageActivity + this->ratio_bwpixels(fg)) / 2.0;
	m_fAverageActivity = __max(m_fAverageActivity, this->ratio_bwpixels(fg));
	m_fAverageWhitePix_density = m_fAverageActivity;*/

	//OK
	//m_fAverageWhitePix_density = (m_fAverageWhitePix_density + this->whitepix_density_n_pix(fg, 3)) / 2.0;	
	m_fAverageWhitePix_density = __max(m_fAverageWhitePix_density, this->whitepix_density_n_pix(fg, 3));
	
	//m_fAverageWhitePix_density *= .80;

	strcpy(m_sFilterType, "");

	//if(m_fAverageActivity < 0.5)//mouvement trop faible
	//{
	//	strcpy(m_sFilterType, "low activity");
	//	probability = 0;
	//	return 0;
	//}
	if(m_fAverageActivity > 30.0 )//mouvement caméra
	{
		strcpy(m_sFilterType, "too noisy");
		probability = -1;
		return 0;
	}
	//if(m_fAverageWhitePix_density < 50.0)//densité trop faible
	//{		
	//	strcpy(m_sFilterType, "");
	//}	

	probability = m_fAverageWhitePix_density;

	return 0;
}
void CPictureAnalyser::set_value( const CvArr* srcarr, int value )
{ 
    CvMat srcstub, *src = (CvMat*)srcarr;
    CvSize ssize;
	src = cvGetMat( srcarr, &srcstub );
	ssize = cvSize(src->width, src->height);
  
	for(int y = 0; y < ssize.height; y++, src->data.ptr += src->step )
	{
		for(int x = 0 ; x < ssize.width; x++ )
			src->data.ptr[x] = value;
	}
}
int CPictureAnalyser::pixel_counter( const CvArr* srcarr, int value )
{  
    CvMat srcstub, *src = (CvMat*)srcarr;
    CvSize ssize;
	src = cvGetMat( srcarr, &srcstub );
	ssize = cvSize(src->width, src->height);
	int sum = 0;
  
	for(int y = 0; y < ssize.height; y++, src->data.ptr += src->step )
	{
		for(int x = 0 ; x < ssize.width; x++ )
		{
			if(src->data.ptr[x] == value)
				sum ++;
		}
	}
	return sum;
}
int CPictureAnalyser::make_border_n_pix(IplImage* img, int border_size)
{
	CvSize ssize = cvGetSize(img);
	char* ptr = img->imageData;
	int color = 0;//black
	for(int y = 0; y < ssize.height; y++, ptr += img->widthStep )
	{
		for(int x = 0 ; x < ssize.width; x++ )
		{
			if(	x > border_size	&&
				x < ssize.width - border_size)
				continue;
				
			if(	y > border_size	&& 
				y < ssize.height - border_size)
				continue;
				
			ptr[x] = color;
		}
	}

	return 0;
}
int CPictureAnalyser::add_perspective(IplImage* fg, IplImage* current, int pc)
{
	CvSeq* contour = 0;	
	CvMemStorage* storage = cvCreateMemStorage(0);
	IplImage* differenceContour	= cvCloneImage(fg);
		
	cvFindContours(		differenceContour, 
						storage, 
						&contour, 
						sizeof(CvContour), 
						CV_RETR_CCOMP, 
						CV_CHAIN_APPROX_SIMPLE );
	char trace[256];
	
	//sprintf(trace, "load_param: TEST, process"); 
	for(int i=0; contour != 0; contour = contour->h_next)
	{	
		//char trace_b[256];
		//sprintf(trace_b, "load_param: process %d", i); 
		//tracer_info(trace_b);
		CvRect bndRect = cvBoundingRect(contour, 1);
		cvResetImageROI(fg);
		cvSetImageROI(fg, bndRect);
		CvRect original = cvGetImageROI(fg);
		this->grown_cvRect(bndRect, pc, fg->height);
		CvRect augmented = bndRect;

		cvRectangle(	current,
						cvPoint(original.x, original.y),
						cvPoint(original.x + original.width, original.y + original.height),
						CV_RGB(0, 255, 0), 1, 8, 0);

		cvRectangle(	current,
						cvPoint(augmented.x, augmented.y),
						cvPoint(augmented.x + augmented.width, augmented.y + augmented.height),
						CV_RGB(255, 0, 0), 1, 8, 0);
	
		IplImage* ipl_orignal = cvCreateImage(cvSize(original.width, original.height), 8, 1);
		IplImage* ipl_augmented = cvCreateImage(cvSize(augmented.width, augmented.height), 8, 1);
		cvCopy(fg, ipl_orignal, NULL);
		cvResize(ipl_orignal, ipl_augmented, CV_INTER_CUBIC);
		
		cvSetImageROI(fg, augmented);
		if(ipl_augmented->width == fg->roi->width &&
			ipl_augmented->height == fg->roi->height)
			cvCopy(ipl_augmented, fg, NULL);

		cvReleaseImage(&ipl_orignal);
		cvReleaseImage(&ipl_augmented);
    }

	cvResetImageROI(fg);

#ifdef _DEBUG
	cvNamedWindow("fg", 0);
	cvShowImage("fg", fg);
	//cvSaveImage("C:\\im.jpg", fg);
	cvNamedWindow("current", 0);
	cvShowImage("current", current);
	//cvSaveImage("C:\\im_.jpg", current);
	//cvWaitKey(0);
#endif

	cvReleaseImage(&differenceContour);
	cvReleaseMemStorage(&storage);
	//cvResizeWindow("fg_activity", activity->width, activity->height);
	//cvNamedWindow("fg_activity", 0);
	//cvShowImage("fg_activity", activity);
	//cvWaitKey(1);
	return 1;
}
void CPictureAnalyser::grown_cvRect(CvRect& rect, int p, int h)
{	
	float p_c_y = (((float)((float)(h - rect.y)) / (float)h) * (float)(p - 1)) + 1; 
	rect.x -= (int)((p_c_y - 1) * 0.5 * rect.width);
	rect.y -= (int)((p_c_y - 1) * 0.5 * rect.height);
	rect.width *= (int)(p_c_y);
	rect.height *= (int)(p_c_y);
	return;
}

float CPictureAnalyser::whitepix_density_n_pix(const CvArr* srcarr, int kernel_size)
{
	float fwhitepix_density = 0;
	CvMat srcstub, *src = (CvMat*)srcarr;
	CvSize ssize;
	src = cvGetMat( srcarr, &srcstub );
	ssize = cvSize(src->width, src->height);
	int sum_white_neighbour = 0;
	int sum_white_pixels = 0;
	int color = 255;//white
	int coeff;//[BLA]
  
	src->data.ptr += kernel_size * src->step;
	for(int y = kernel_size; y < ssize.height - kernel_size; y++, src->data.ptr += src->step )
	{
		for(int x = kernel_size ; x < ssize.width - kernel_size; x++ )
		{
			if(src->data.ptr[x] == color)
			{
				sum_white_pixels++;

				// fenetre autour du pixel blanc
				for(int k = -kernel_size; k < kernel_size + 1; k++)
				{
					uchar* cur_row = src->data.ptr + (k * src->step);

					for(int l = -kernel_size; l < kernel_size + 1; l++)
					{
						if(k == 0 && l == 0)//central pixel
							continue;

						if(cur_row[x-l] == color)
						{
							if(m_perspective_activation > 0 &&	m_perspective_coefficient > 0)
							{
							  coeff=1+(ssize.height-y)*(m_perspective_coefficient-1)/ssize.height;//[BLA]
							  for (int i=0;i<coeff;i++) {//[BLA]
							    
								  sum_white_neighbour++;
							  }
							}
							else {
							  sum_white_neighbour++;
							}
						}
					}
				}
			}
		}
	}

	int nb_pixels_in_kernel = ((2 * kernel_size + 1) * (2 * kernel_size + 1)) - 1;
	
	if(nb_pixels_in_kernel * sum_white_pixels > 0)
		fwhitepix_density = ((float)sum_white_neighbour / (float)(src->width * src->height)) * (float)100.0;

	float proba_extended = 100 - 100 * exp(- 5*fwhitepix_density / 100);//The coefficient 13 correspond to saturate the model.
	return proba_extended;

}
//float CPictureAnalyser::whitepix_density_n_pix(const CvArr* srcarr, int kernel_size)
//{
//	float fwhitepix_density = 0;
//	CvMat srcstub, *src = (CvMat*)srcarr;
//	CvSize ssize;
//	src = cvGetMat( srcarr, &srcstub );
//	ssize = cvSize(src->width, src->height);
//	int sum_white_neighbour = 0;
//	int sum_white_pixels = 0;
//	int color = 255;//white
//  
//	src->data.ptr += kernel_size * src->step;
//	for(int y = kernel_size; y < ssize.height - kernel_size; y++, src->data.ptr += src->step )
//	{
//		for(int x = kernel_size ; x < ssize.width - kernel_size; x++ )
//		{
//			if(src->data.ptr[x] == color)
//			{
//				sum_white_pixels++;
//
//				// fenetre autour du pixel blanc
//				for(int k = -kernel_size; k < kernel_size + 1; k++)
//				{
//					uchar* cur_row = src->data.ptr + (k * src->step);
//
//					for(int l = -kernel_size; l < kernel_size + 1; l++)
//					{
//						if(k == 0 && l == 0)//central pixel
//							continue;
//
//						if(cur_row[x-l] == color)
//						{
//							//src->data.ptr[l] = 125;
//							sum_white_neighbour++;
//						}
//					}
//				}
//			}
//		}
//	}
//
//	int nb_pixels_in_kernel = ((2 * kernel_size + 1) * (2 * kernel_size + 1)) - 1;
//	
//	if(nb_pixels_in_kernel * sum_white_pixels > 0)
//		fwhitepix_density = ((float)sum_white_neighbour / ((float)nb_pixels_in_kernel * (float)sum_white_pixels)) * (float)100.0;
//
//	return fwhitepix_density;
//}
float CPictureAnalyser::whitepix_density_1_pix(const CvArr* srcarr)
{
	float fwhitepix_density = 0;

	CvMat srcstub, *src = (CvMat*)srcarr;
	CvSize ssize;
	src = cvGetMat( srcarr, &srcstub );
	ssize = cvSize(src->width, src->height);
	int sum_white_neighbour = 1;
	int sum_white_pixels = 1;
	int color = 255;//white
  
	src->data.ptr += src->step;
	for(int y = 1; y < ssize.height - 1; y++, src->data.ptr += src->step )
	{
		for(int x = 1 ; x < ssize.width - 1; x++ )
		{
			if(src->data.ptr[x] == color)
			{
				sum_white_pixels++;

				/*	000
					0X0
					000		*/
				/*if(src->data.ptr[x] == color) //central pixel
					sum_white_neighbour ++;*/
				/*	000
					X00
					000		*/
				if(src->data.ptr[x-1] == color)
					sum_white_neighbour ++;
				/*	000
					00X
					000		*/
				if(src->data.ptr[x+1] == color)
					sum_white_neighbour ++;


				uchar* prev_row = src->data.ptr - src->step;
			
				/*	0X0
					000
					000		*/
				if(prev_row[x] == color)
					sum_white_neighbour ++;
				/*	X00
					000
					000		*/
				if(prev_row[x-1] == color)
					sum_white_neighbour ++;
				/*	00X
					000
					000		*/
				if(prev_row[x+1] == color)
					sum_white_neighbour ++;


				uchar* next_row = src->data.ptr + src->step;
			
				/*	000
					000
					0X0		*/
				if(next_row[x] == color)
					sum_white_neighbour ++;
				/*	000
					000
					X00		*/
				if(next_row[x-1] == color)
					sum_white_neighbour ++;
				/*	000
					000
					00X		*/
				if(next_row[x+1] == color)
					sum_white_neighbour ++;

			}
		}
	}

	fwhitepix_density = ((float)sum_white_neighbour / ((float)8.0 * sum_white_pixels)) * (float)100.0;

	return fwhitepix_density;
}
float CPictureAnalyser::small_particule_filtering(IplImage* fg, IplImage* current, int max_blob_superficy)
{
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contour = 0;	
	IplImage* differenceContour	= cvCloneImage(fg);
		
	cvFindContours(		differenceContour, 
						storage, 
						&contour, 
						sizeof(CvContour), 
						CV_RETR_CCOMP, 
						CV_CHAIN_APPROX_SIMPLE );

	cvReleaseImage(&differenceContour);

	//cvResizeWindow("fg", fg->width, fg->height);
	//cvNamedWindow("fg", 0);
	//cvShowImage("fg", fg);
	//cvWaitKey(0);

	float nbPixelTotal = (float)(fg->width * fg->height);
	for(int i=0; contour != 0; contour = contour->h_next)
	{	
		CvRect bndRect = cvBoundingRect(contour, 1);		
		float normalizedBlobSuperficy = (bndRect.width * bndRect.height) / nbPixelTotal;
			
		if(bndRect.width * bndRect.height < max_blob_superficy)
		{
			cvRectangle(	fg, 
							cvPoint(bndRect.x, bndRect.y), 
							cvPoint(bndRect.x + bndRect.width, bndRect.y + bndRect.height),
							CV_RGB(0, 0, 0), CV_FILLED, 8, 0); //fill in black

			cvRectangle(	current, 
							cvPoint(bndRect.x, bndRect.y), 
							cvPoint(bndRect.x + bndRect.width, bndRect.y + bndRect.height),
							CV_RGB(0, 255, 0), 1, 8, 0);				
		}
    }

	cvReleaseMemStorage(&storage);

	//cvResizeWindow("fg_filtered", fg->width, fg->height);
	//cvNamedWindow("fg_filtered", 0);
	//cvShowImage("fg_filtered", fg);
	//cvWaitKey(0);

	return 1.0;
}
float CPictureAnalyser::ratio_bwpixels(IplImage* difference)
{
	//ratio white & black pixels
	float whitePix = 100 - ( (float)(100 * pixel_counter(difference, 0)) / (float)(difference->width * difference->height) );
	return whitePix;
}

//////////////////////////////////////////////////////////////////////
// drawing
//////////////////////////////////////////////////////////////////////
void CPictureAnalyser::draw_text(IplImage* img, const char* text, int pos_x, int pos_y, CvScalar color)
{
	CvFont font;    
	cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0, 1, CV_AA );

	CvPoint pText; 
	pText.x = pos_x; 
	pText.y = pos_y;

	char txt[64];
	sprintf(txt,"%s", text);
	
	//get text size
	CvSize text_size;
	int baseline;
	cvGetTextSize(txt , &font, &text_size, &baseline);

    //draw a white box                       
	CvPoint pos1 = cvPoint(pos_x, pos_y+2);
	CvPoint pos2 = cvPoint(pos_x+text_size.width,pos_y-text_size.height);
	cvRectangle(img, pos1, pos2, CV_RGB(255, 255, 255), -1, 8, 0);

	cvPutText(img, txt, pText, &font, color);

	return;
}
void CPictureAnalyser::draw_results(IplImage* img, char* cameraKey, int probability, int threshold, bool first_picture)
{
	if(first_picture == false)
	{
		if(probability > threshold)	
			draw_activity(img, 2);
		else
			draw_activity(img, 0);
	}

	char txt[64];		
	//sprintf(txt,"Frame %d", m_iframeCounter);
	//draw_text(img, txt, 5, img->height - 5, CV_RGB(128, 128, 128));

	//sprintf(txt,"%s", cameraKey);
	//draw_text(img, txt, 5, img->height - 25, CV_RGB(128, 128, 128));

	if(first_picture == false)
	{	
		sprintf(txt,"Probability %d", probability);
		if(probability > threshold)	
			draw_text(img, txt, img->width - 125, img->height - 5, CV_RGB(255, 0, 0));
		else
			draw_text(img, txt, img->width - 125, img->height - 5, CV_RGB(0, 0, 255));

		if(strcmp(m_sFilterType, "") != 0)
		{
			sprintf(txt,"Filter: %s", m_sFilterType);
			draw_text(img, txt, img->width - 125, img->height - 25, CV_RGB(0, 0, 255));
		}
	}

	//draw_mask(img, CV_RGB(0, 255, 0), false);

	return;
}
void CPictureAnalyser::draw_activity(IplImage* img, int channel)
{
	if(m_difference != 0)
	{
		for(int i=0;i<img->width; i++)
		{
			for(int j=0;j<img->height;j++)
			{
				CvScalar px = cvGet2D(m_difference, j, i);

				if(px.val[0] > 0)
				{
					CvScalar px2 = cvGet2D(img, j, i);
					px2.val[channel] = 255;
					cvSet2D(img, j, i, px2);
				}
			}
		}
	}

	return;
}
void CPictureAnalyser::draw_mask(IplImage* img, CvScalar color, bool fill_mask)
{//set binary mask

	for(int i = 0 ; i<AR_SSR_MAX_POLYMASK; i++)
	{
		if(m_parameters.tabPolyMask[i].Id == -1) continue;

		AS_Polygone TABPoly;

		for(int j = 0 ; j<m_parameters.tabPolyMask[i].NbOfPoint; j++)
		{	
			float pos_x = (float)(m_parameters.tabPolyMask[i].CoordX[j] * img->width) / 32000;
			float pos_y = (float)(m_parameters.tabPolyMask[i].CoordY[j] * img->height) / 32000;
			
			if(pos_x > img->width)	pos_x = (float)img->width;	
			if(pos_y > img->height) pos_y = (float)img->height;
			if(pos_x < 0) pos_x = 0;
			if(pos_y < 0) pos_y = 0;
			
			TABPoly.polyTab[j]	= cvPoint((int)pos_x,(int)pos_y);
		}
		
		if(m_parameters.tabPolyMask[i].NbOfPoint > 0)
		{
			CvPoint *ptr = TABPoly.polyTab;

			if(fill_mask)
				cvFillPoly(img, &ptr, &m_parameters.tabPolyMask[i].NbOfPoint, 1, color, CV_AA);
			else
				cvPolyLine(img, &ptr, &m_parameters.tabPolyMask[i].NbOfPoint, 1, 1, color, 1, 8, 0);                   
		}
	}

	return;
}