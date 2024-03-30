// CMotiondetection.cpp: implementation of the CMotiondetection class.
//
//////////////////////////////////////////////////////////////////////
#include "CMotiondetection.h"
#define DRAW_ON //pour dessiner et retourner une image de debbugage
#define VISU_ON //pour afficher cette image

CMotiondetection::CMotiondetection()
{	
	m_reloadParam			= true; //call sensorCamFixeInit() again if set to true
	m_height				= 0;    //image height
	m_width					= 0;    //image width

	m_src_					= NULL; //IPL image 8 bits
	m_src24b				= NULL; //IPL image 24 bits
	m_maskImg				= NULL; //IPL image 8 bits
	m_srcWithMask			= NULL; //IPL image 8 bits
	m_srcTmp_				= NULL; //IPL image
																					
	obj_pa					= NULL;
    m_dataSizeFilter        = 0;
	m_timeLastAlarm			= -10000;
	m_dataFilter1			= 0;
	m_dataSpeedFilter		= 0;

	m_idAlarm = -1;
}
CMotiondetection::~CMotiondetection()
{
}

bool
CMotiondetection::md_OnNewContext(
		char *sz_configuration_path,
		uint32_t ui_channel_id)
{
	// chargement des parametres
	if(obj_pa == NULL) obj_pa = new CPictureAnalyser();
	if(obj_pa != NULL) {
		// "FA_OnNewContext_:	configuration_path: %s	channel_id: %d", configuration_path, channel_id
		obj_pa->re_init();
		return obj_pa->load_param(sz_configuration_path);
	}
	return false;
}
bool
CMotiondetection::md_PutImage(
		unsigned char *p_buffer_24bits,
		uint32_t ui_width,
		uint32_t ui_height,
		unsigned char *p_buffer_out,
		long* p_buf_out_size,
		char* sz_key)
{
	if(	p_buffer_24bits == NULL		||
		ui_width<0					||
		ui_height<0			)
	{
		return false;
	}	
	
	if(obj_pa == NULL)
		return false;
	//reload parameters
	if(	m_reloadParam				||
		ui_width != m_src24b->width	||
		ui_height != m_src24b->height)
		fa_init_(width,height);
	//fill color buffers
	if(!fa_fillRGB_(p_buffer_24bits, FORMAT_RGB24, ui_width, ui_height)){
		return false;
	}
	/**************************************************************************************/
	#ifndef _DEBUG
	cvFlip(m_src24b, m_src24b, 0);
	#endif
	obj_pa->process_pa_(sz_key , m_src24b, true);
	sprintf(m_cameraKey, sz_key, 256);
	#ifdef DRAW_ON
	if(p_buffer_out != 0)
	{
#ifdef VA_SYS_WIN32
		memcpy(bufferOut, m_src24b->imageData, m_src24b->imageSize);
		*bufferOut = m_src24b->imageSize;
#endif
	}
	#endif
	return false;
}

bool
CMotiondetection::md_GetAlarmInfo(
		char* p_return)
{//renvoie des informations sur l'alarme en cours
	
	AR_SSR_EVT_DESCRIPTION* evt = (AR_SSR_EVT_DESCRIPTION*)p_return;
	memset(evt, 0, sizeof(AR_SSR_EVT_DESCRIPTION));
	evt->analysis_result = obj_pa->get_analysis_result();
	evt->analysis_threshold = obj_pa->get_analysis_threshold();
	strcpy(evt->filter_type, obj_pa->get_filter_type());
	return true;
}


void
CMotiondetection::md_init(
		uint32_t ui_width,
		uint32_t ui_height)
{
	m_width	= ui_width;
	m_height = ui_height;

	if(m_src_ != NULL)		
		cvReleaseImage(&m_src_);
	if(m_src24b != NULL)	
		cvReleaseImage(&m_src24b);
	if(m_maskImg != NULL)	
		cvReleaseImage(&m_maskImg);
	if(m_srcWithMask != NULL)	
		cvReleaseImage(&m_srcWithMask);

	m_src_ = NULL;
	m_src24b = NULL;
	m_maskImg = NULL;
	m_srcWithMask = NULL;

	// --- initialize all buffers
	if(m_src_ == NULL)		
		m_src_		= cvCreateImage(cvSize(m_width, m_height),8,1);
	if(m_src24b == NULL)	
		m_src24b	= cvCreateImage(cvSize(m_width, m_height),8,3);	
	if(m_maskImg == NULL)	
		m_maskImg	= cvCreateImage(cvSize(m_width, m_height),8,1);
	if(m_srcWithMask == NULL)	
		m_srcWithMask	= cvCreateImage(cvSize(m_width, m_height),8,1);

	//setMask(m_channel, m_prepo);
	m_reloadParam = false;
}

bool
CMotiondetection::md_fillRGB(
		unsigned char *p_bufferY,
		TI_PICTURE_FORMAT format,
		uint32_t ui_width,
		uint32_t ui_height)
{
	if(	format == FORMAT_RGB24 && 
		ui_width*ui_height*3 == m_src24b->imageSize){
		memcpy(m_src24b->imageData, p_bufferY, ui_width*ui_height*3);
		cvCvtColor(m_src24b,m_src_,CV_RGB2GRAY);
		return true;
	}
	return false;
}

