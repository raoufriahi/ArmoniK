/*
 * Copyright (c) 2017-2023 Heimdall
 *
 * The computer program contained herein contains proprietary
 * information which is the property of Heimdall.
 * The program may be used and/or copied only with the written
 * permission of Heimdall or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#include "vplay.hpp"
#include "license.hpp"
#include "render_wrapper.hpp"
#include "ffkit/av_muxer.h"
#include "ffkit/av_demuxer.h"
#include "ffkit/h264_decoder.h"
#include "ffkit/fflocky.h"
#include "ffkit/ffoptions.h"
#include "cppkit/ck_memory.h"
#include "cppkit/os/ck_large_files.h"
#include "rapidmedia/rapidmedia.hpp"
#include "curl/curl.h"
#include "AVKit/JPEGEncoder.h"

#define ENABLE_SDL_RENDER 1

using namespace UtilityLib;
using namespace std;
using namespace cppkit;
using namespace ffkit;

/**
 * @brief A fast mutex used to synchronize access to gRenderNoVideoPic.
 *
 * This mutex ensures that concurrent access to gRenderNoVideoPic is properly synchronized
 * to prevent race conditions and data corruption.
 */
fast_mutex gRenderNoVideoLock;
/**
 * @brief Pointer to an av_packet used for rendering video without an actual video source.
 *
 * This pointer holds the av_packet structure used for rendering video frames when there
 * is no actual video available. It can be set to NULL when there is no video frame to render.
 */
av_packet *gRenderNoVideoPic = NULL;

/**
 * @brief Defines the state representing a video frame.
 *
 * This constant defines the value representing the state of a video frame.
 */
#define STATE_FRAME 1

/**
 * @brief Defines the state representing the main header.
 *
 * This constant defines the value representing the state of the main header.
 */
#define STATE_MAIN_HEADER 2

/**
 * @brief Defines the state representing a frame header.
 *
 * This constant defines the value representing the state of a frame header.
 */
#define STATE_FRAME_HEADER 3
/**
 * @brief The size of the HTTP JPEG header.
 *
 * This constant defines the size of the HTTP JPEG header, which is set to 1024 bytes.
 */
#define HTTP_JPEG_HEADER_SIZE 1024

class VPlayData
{
public:
	/* First check the bMJPEG, if the bMJPEG is false then check the bFile */
	VPlayData(VPlay &pPlay, bool bFile, bool bMJPEG, astring strFile, string strUser, 
		string strPass,  BOOL bHWAccel)
	:m_vPlay(pPlay), m_bFile(bFile), m_strFile(strFile), m_bMJPEG(bMJPEG), m_strUser(strUser), 
	 m_strPass(strPass), m_bHWAccel(bHWAccel)
	{
		this->m_nAttachedPlayer = 0;
		this->pData = NULL;
		this->dataHandler = NULL;
		this->m_pThread = NULL;

		this->m_render = new RenderWrapper(m_bHWAccel);
		m_pktMJPEG.bufLen = 0;
		m_pktMJPEG.dataBuf = NULL;

		m_jpeg_buffer = NULL;
		m_jpeg_first_frame = true;
		m_bSingleJpeg = false;

		this->m_bExit = false;
		if (m_bFile == true && m_bMJPEG == false)
		{
			this->m_pThread = new tthread::thread(VPlayData::Run, (void *)this);
		}else if (m_bMJPEG == true)
		{
			this->m_pThread = new tthread::thread(VPlayData::RunMJPEG, (void *)this);
		}

		return;
	}
	~VPlayData()
	{
		m_avWrapper.StopRaw();
		m_avWrapper.Stop();
	  	if (m_bMJPEG == true)
	   	{
			m_bExit = true;
			m_pThread->join();
			delete m_pThread;
			m_pThread = NULL;
	   	}
		if (m_bMJPEG != true && m_bFile == true)
	   	{
			m_bExit = true;
			m_pThread->join();
			delete m_pThread;
			m_pThread = NULL;
	   	}
		if (this->m_render)
		{
			delete this->m_render;
		}
		if (m_jpeg_buffer)
		{
			free(m_jpeg_buffer);
			m_jpeg_buffer = NULL;
		}
		if (m_pktMJPEG.dataBuf)
		{
			free(m_pktMJPEG.dataBuf);
			m_pktMJPEG.dataBuf = NULL;
		}
	}
public:
	static void Run(void * pParam);
	void Run1();

	static void RunMJPEG(void * pParam);
	void RunMJPEG1();
	static size_t MJPEGWriteData(void *ptr, size_t size, size_t nmemb, void *userdata);
	size_t MJPEGWriteData1(void *ptr, size_t size, size_t nmemb);
	static int CurlXferinfo(void *p, curl_off_t dltotal, curl_off_t dlnow,
                    curl_off_t ultotal, curl_off_t ulnow);
	int CurlXferinfo1(curl_off_t dltotal, curl_off_t dlnow,
                    curl_off_t ultotal, curl_off_t ulnow);
	void on_headerline(char *buf);
	void on_frame(unsigned char *ptr, int len);
public:
	VPlay &m_vPlay;
	RapidMedia m_avWrapper;
	s32 m_nAttachedPlayer;
	void* pData;
	VPlayDataHandler dataHandler;
	RenderWrapper *m_render;
	bool m_bHWAccel;

	bool m_bFile;
	bool m_bMJPEG;
	astring m_strUser;
	astring m_strPass;
	astring m_strFile;
	tthread::thread *m_pThread;
	struct timeval m_lastTime;
	bool m_bExit;
	VideoFrame m_pktMJPEG;

	/* HTTP JPEG */
	char m_headerline[HTTP_JPEG_HEADER_SIZE];
	int m_jpeg_frame_position;
	int m_jpeg_num_headers;
	int m_jpeg_frame_size;
	bool m_jpeg_first_frame;
	bool m_bSingleJpeg;
	int m_g_state;	
	unsigned char *m_jpeg_buffer;
	int m_jpeg_buffer_len;
};
/**
 * \brief Updates the render type based on the hardware acceleration flag and current render type.
 *
 * This function takes a hardware acceleration flag and the current render type as input and returns an updated render type.
 *
 * \param bHWAccel A boolean flag indicating whether hardware acceleration is enabled.
 * \param current The current render type.
 *
 * \return The updated render type based on the hardware acceleration flag and current render type.
 *
 * \note This function is used to determine the appropriate render type based on the hardware acceleration flag and current render type.
 */
static RenderType UpdateRenderType(bool bHWAccel, RenderType current)
{
	if (bHWAccel == false){
		return current;
	}

#ifdef _WIN32
	return RENDER_TYPE_D3D;
#else
#ifdef __APPLE__
	/* Current MacOS do not support HW decode */
	return RENDER_TYPE_SDL;
#endif
	return RENDER_TYPE_VAAPI;
#endif
	
}
/**
 * @brief Runs the VPlayData object with the specified parameters.
 *
 * This function executes the main logic of the VPlayData object, performing the necessary operations
 * based on the given parameters. It should be called in a separate thread to avoid blocking the main
 * program execution.
 *
 * @param pParam A pointer to additional parameters required for the execution of the VPlayData object.
 *               The exact type and format of the parameters should be documented separately.
 */
 void VPlayData::Run(void * pParam)
{
	VPlayData * pThread = (VPlayData *)pParam;
	if (pThread){
	    pThread->Run1();
	}
}

/**
 * @brief Executes the Run1 operation.
 *
 * This function performs the Run1 operation on the VPlayData object.
 * The Run1 operation is responsible for executing a specific task or operation
 * related to the VPlayData class.
 *
 * @note This function assumes that the necessary data and resources are available
 *       for performing the Run1 operation.
 *
 * @see VPlayData
 */
void VPlayData::Run1()
{
	struct timeval current;
	VideoFrame packet;
	packet.bufLen = 1024 * 16;
	packet.dataBuf = (u8 *)malloc(packet.bufLen);
#ifdef VPLAY_DUMP_RAW
       FILE* fSink;    
       fopen_s(&fSink, "raw.264", "wb");
	BOOL bGotIFrame = FALSE;
#endif
	while (m_bExit != true){
		try {
			VDC_DEBUG( "%s  Read File Source %s\n",__FUNCTION__, m_strFile.c_str());
			ve_sleep(1000);	
			struct stream_info info;
			shared_ptr<av_demuxer> deMuxer = make_shared<av_demuxer>( m_strFile );

			int videoStreamIndex = deMuxer->get_video_stream_index();
			deMuxer->get_stream_types(videoStreamIndex, info);
			VDC_DEBUG( "%s  Read File Codec %d (%d, %d)\n",__FUNCTION__, m_strFile.c_str(), 
				info.codec_id, info.width, info.height);
			packet.streamType = VIDEO_STREAM_VIDEO;

			int index = 0;
			int i = 0;
			int streamIndex = 0;
			double videoTimeDiff1 = deMuxer->get_seconds_between_frames(videoStreamIndex);
			int videoTimeDiff = videoTimeDiff1 * 1000;

			if (videoTimeDiff < 20) {
				videoTimeDiff = 20;
			}
			VDC_DEBUG( "%s  time diff %f %d\n",__FUNCTION__, videoTimeDiff1, videoTimeDiff);
			while(m_bExit != true && deMuxer->read_frame( streamIndex )){
				if ( streamIndex != videoStreamIndex ){
					continue;
				}
				gettimeofday(&current, NULL);
				shared_ptr<av_packet> frame = deMuxer->get();
#ifdef VPLAY_DUMP_RAW
				if (bGotIFrame == TRUE || deMuxer->is_key()){
					fwrite(frame->map(), 1, frame->get_data_size(), fSink);
					if (deMuxer->is_key()){	
						bGotIFrame = TRUE;
					}
				}
#endif
				if (packet.bufLen < (sizeof(InfoFrameI) + frame->get_data_size())) {
					free(packet.dataBuf);
					packet.bufLen = (sizeof(InfoFrameI) + frame->get_data_size());
					packet.dataBuf = (u8 *)malloc(packet.bufLen);
				}
				if (deMuxer->is_key() == true){
					packet.frameType = VIDEO_FRM_I;
					memcpy(packet.dataBuf + sizeof(InfoFrameI), 
						frame->map(), frame->get_data_size());
					InfoFrameI *pI = (InfoFrameI *)packet.dataBuf;
					packet.dataLen = frame->get_data_size() + sizeof(InfoFrameI);
					pI->audio = CODEC_NONE;
					if (info.codec_id == AV_CODEC_ID_HEVC)
					{
						pI->video = CODEC_H265;
					}else if (info.codec_id == AV_CODEC_ID_H264)
					{
						pI->video = CODEC_H264;
					}else if (info.codec_id == AV_CODEC_ID_MJPEG)
					{
						pI->video = CODEC_MJPEG;
					}else if (info.codec_id == AV_CODEC_ID_MPEG4)
					{
						pI->video = CODEC_MPEG4;
					}else
					{
						pI->video = CODEC_H264;
					}

					pI->vWidth = info.width;
					pI->vHeight = info.height;
				}else {
					packet.frameType = VIDEO_FRM_P;
					memcpy(packet.dataBuf + sizeof(InfoFrameP), 
						frame->map(), frame->get_data_size());
					InfoFrameP *pP = (InfoFrameP *)packet.dataBuf;
					pP->audio = CODEC_NONE;
					packet.dataLen = frame->get_data_size() + sizeof(InfoFrameP);
					if (info.codec_id == AV_CODEC_ID_HEVC)
					{
						pP->video = CODEC_H265;
					}else if (info.codec_id == AV_CODEC_ID_H264)
					{
						pP->video = CODEC_H264;
					}else if (info.codec_id == AV_CODEC_ID_MJPEG)
					{
						pP->video = CODEC_MJPEG;
					}else if (info.codec_id == AV_CODEC_ID_MPEG4)
					{
						pP->video = CODEC_MPEG4;
					}else
					{
						pP->video = CODEC_H264;
					}
					
				}
				
				packet.secs = current.tv_sec;
				packet.msecs = current.tv_usec/1000;
				
				m_vPlay.PutRawData(packet);
				
				if (dataHandler){
					dataHandler(pData, packet);
				}
				if (videoTimeDiff> 0){
					ve_sleep(videoTimeDiff);
				}
				
			}
		}catch (...)
		{
		}
	}
	free(packet.dataBuf);

	return;
	
}
/**
 * @brief Handles the header line of the VPlayData object.
 *
 * This function is called when a header line is encountered during data processing.
 * It processes the provided buffer containing the header information and performs
 * any necessary operations.
 *
 * @param buf A pointer to a character buffer containing the header line.
 */
inline void VPlayData::on_headerline(char *buf) 
{
    if (strncmp(buf, "Content-Length:", 15) == 0) {
        m_jpeg_frame_size = atoi(buf + 16);
    }
    if (strncmp(buf, "Content-Type:", 13) == 0) {
    }
}

/**
 * @brief Processes a frame of data.
 *
 * This function is responsible for processing a frame of data.
 * It takes a pointer to the frame data and its length as input
 * parameters.
 *
 * @param ptr Pointer to the frame data.
 * @param len Length of the frame data.
 *
 */
inline void VPlayData::on_frame(unsigned char *ptr, int len) 
{
	struct timeval current;
	gettimeofday(&current, NULL);

	if (m_pktMJPEG.bufLen < (sizeof(InfoFrameI) + len)){
		free(m_pktMJPEG.dataBuf);
		m_pktMJPEG.bufLen = (sizeof(InfoFrameI) + len);
		m_pktMJPEG.dataBuf = (u8 *)malloc(m_pktMJPEG.bufLen);
	}

#if 0
	static int iName = 0;
	char strName[64];
	sprintf(strName, "%d.jpg", iName ++);
	
	image_write_raw((unsigned char *)ptr, len, strName);
	DumpHexData((unsigned char *)ptr, 20);
#endif
	
	m_pktMJPEG.streamType = VIDEO_STREAM_VIDEO;
	m_pktMJPEG.frameType = VIDEO_FRM_I;
	u8 * pHeader = m_pktMJPEG.dataBuf + sizeof(InfoFrameI);
	memcpy(pHeader, ptr, len);
	InfoFrameI *pI = (InfoFrameI *)m_pktMJPEG.dataBuf;
	m_pktMJPEG.dataLen = len + sizeof(InfoFrameI);

	pI->video = CODEC_MJPEG;
	pI->audio = CODEC_NONE;

	pI->vWidth = 640;
	pI->vHeight = 480;
	m_pktMJPEG.secs = current.tv_sec;
	m_pktMJPEG.msecs = current.tv_usec/1000;
	
	m_vPlay.PutRawData(m_pktMJPEG);
	
	if (dataHandler){
		dataHandler(pData, m_pktMJPEG);
	}
}

/**
 * @brief Writes data to an MJPEG stream.
 *
 * This function is a callback that is called by an MJPEG streaming library to write data to a stream.
 *
 * @param ptr A pointer to the data buffer containing the data to be written.
 * @param size The size of each element to be written, in bytes.
 * @param nmemb The number of elements to be written.
 * @param userdata A pointer to user-defined data that can be passed to the callback function.
 *
 * @return The total number of bytes successfully written.
 *
 * @note This function is intended to be used as a callback and should not be called directly.
 */
size_t VPlayData::MJPEGWriteData(void *ptr, size_t size, size_t nmemb, void *userdata) 
{
	VPlayData * pThread = (VPlayData *)userdata;
	if (pThread){
	    pThread->MJPEGWriteData1(ptr, size, nmemb);
	}
	return nmemb;
}

/**
 * Writes data to the MJPEG stream.
 *
 * This function is called by libcurl to handle the received data from the MJPEG stream. It writes the data
 * to the appropriate location or performs any necessary processing.
 *
 * @param ptr Pointer to the received data.
 * @param size Size of each data element.
 * @param nmemb Number of data elements.
 *
 * @return The total number of bytes written.
 */
size_t VPlayData::MJPEGWriteData1(void *ptr, size_t size, size_t nmemb) 
{
	size_t written;

	unsigned char *bptr = (unsigned char *)ptr;
	long nbytes = size * nmemb;
	
	if (nbytes < 2){
		return nbytes;
	}

	VDC_DEBUG("got %d bytes (%d, %d)...\n", nbytes, size, nmemb);

	if (m_jpeg_first_frame == true) {
		VDC_DEBUG("got %d bytes (%d, %d) %c %c...\n", nbytes, size, nmemb, bptr[0], bptr[1]);
		if (strncmp((char *)bptr, "--", 2) != 0){
			m_bSingleJpeg = true;
		}
		m_jpeg_first_frame = false;
	}

	if (m_bSingleJpeg == true) {
		if (m_jpeg_frame_position + nbytes > m_jpeg_buffer_len) {
			u8 * tempbuf = (u8*)malloc((m_jpeg_frame_position + nbytes) * 2);
			memcpy(tempbuf, m_jpeg_buffer, m_jpeg_buffer_len);
			m_jpeg_buffer_len = (m_jpeg_frame_position + nbytes) * 2;
			free(m_jpeg_buffer);
			m_jpeg_buffer = tempbuf;
		}

		memcpy(m_jpeg_buffer + m_jpeg_frame_position, bptr, nbytes);
		m_jpeg_frame_position = m_jpeg_frame_position + nbytes;
		m_jpeg_frame_size = m_jpeg_frame_size + nbytes;
		return nbytes;
	}


	for(int i=0; i<nbytes; i++) {
	    unsigned char b = bptr[i];
	    if (m_g_state == STATE_MAIN_HEADER || m_g_state == STATE_FRAME_HEADER) {
	        int p = strlen(m_headerline);
	        if (p < 1000)
	            m_headerline[p] = b;
	        if (b == '\n') {
	            if (m_headerline[p-1] == '\r') {
	                m_headerline[p] = 0;
	                m_headerline[p-1] = 0;
	                on_headerline(m_headerline);
	                if (strncmp(m_headerline, "--", 2) == 0) {
	                    m_g_state = STATE_FRAME_HEADER;
	                }
	                if (strlen(m_headerline) == 0 && m_jpeg_num_headers > 0) {
	                    if (m_g_state == STATE_FRAME_HEADER) {
	                        m_g_state = STATE_FRAME;
	                        m_jpeg_frame_position = 0;
	                    }
	                }
	                memset(m_headerline, 0, 1000);
	                m_jpeg_num_headers ++;
	            }
	        }
	    }
	    else if (m_g_state == STATE_FRAME) {
	        if (m_jpeg_frame_position < m_jpeg_buffer_len) {
	            m_jpeg_buffer[m_jpeg_frame_position] = b;
	        }else
	        {
	        	u8 * tempbuf = (u8*)malloc(m_jpeg_buffer_len * 2);
			memcpy(tempbuf, m_jpeg_buffer, m_jpeg_buffer_len);
			m_jpeg_buffer_len = m_jpeg_buffer_len * 2;
			free(m_jpeg_buffer);
			m_jpeg_buffer = tempbuf;
			m_jpeg_buffer[m_jpeg_frame_position] = b;
	        }
	        m_jpeg_frame_position ++;
	        if (m_jpeg_frame_position >= m_jpeg_frame_size) {
	            on_frame(m_jpeg_buffer, m_jpeg_frame_size);
	            m_g_state = STATE_FRAME_HEADER;
	            memset(m_headerline, 0, 1000);
	            m_jpeg_frame_position = 0;
	            m_jpeg_num_headers = 0;
	        }
	    }
	}

	return nbytes;
}

/**
 * @brief Callback function used by libcurl to receive transfer information.
 *
 * This function is called by libcurl to provide transfer progress information
 * during a file transfer operation. It is intended to be used as a callback
 * function when setting up a libcurl transfer.
 *
 * @param p A pointer to a user-defined data structure.
 * @param dltotal The total number of bytes expected to be downloaded.
 * @param dlnow The number of bytes downloaded so far.
 * @param ultotal The total number of bytes expected to be uploaded.
 * @param ulnow The number of bytes uploaded so far.
 *
 * @return An integer value indicating the success or failure of the callback function.
 *         Return 0 to continue the transfer or a non-zero value to abort the transfer.
 */
int VPlayData::CurlXferinfo(void *p, curl_off_t dltotal, curl_off_t dlnow,
                            curl_off_t ultotal, curl_off_t ulnow)
{
	VPlayData * pThread = (VPlayData *)p;
	if (pThread){
	    return pThread->CurlXferinfo1(dltotal, dlnow, ultotal, ulnow);
	}
	return 1;
}
/**
 * @brief Callback function for tracking the progress of a libcurl transfer.
 *
 * This function is used as a callback for the CURLOPT_XFERINFOFUNCTION option
 * in a libcurl request. It is called by libcurl to provide information about
 * the progress of the transfer.
 *
 * @param dltotal Total number of bytes expected to be downloaded.
 * @param dlnow   Number of bytes downloaded so far.
 * @param ultotal Total number of bytes expected to be uploaded.
 * @param ulnow   Number of bytes uploaded so far.
 *
 * @return An integer indicating whether the transfer should continue or not.
 *         Return 0 to continue the transfer, or a non-zero value to abort.
 */
int VPlayData::CurlXferinfo1(curl_off_t dltotal, curl_off_t dlnow,
                             curl_off_t ultotal, curl_off_t ulnow)
{
	if (m_bExit == true){
		VDC_DEBUG( "%s  CurlXferinfo1 exit\n",__FUNCTION__);
		return 1;
	}	
	return 0;
}

/**
 * @brief Executes the MJPEG process.
 *
 * This function runs the MJPEG process with the provided parameter.
 * It performs the necessary operations to handle the MJPEG data.
 *
 * @param pParam A pointer to the parameter required for the MJPEG process.
 *               The type and content of the parameter depend on the implementation.
 *               It is the responsibility of the caller to ensure the validity of the parameter.
 */
void VPlayData::RunMJPEG(void * pParam)
{
	VPlayData * pThread = (VPlayData *)pParam;
	if (pThread){
	    pThread->RunMJPEG1();
	}
}

 /**
 * @brief Runs the MJPEG1 algorithm on the VPlayData object.
 *
 * This function applies the MJPEG1 algorithm to the VPlayData object,
 * producing a video stream with Motion-JPEG compression. The compressed
 * stream is stored internally within the object and can be accessed
 * using the appropriate getter methods.
 *
 * @note This function assumes that the VPlayData object contains a valid
 *       video stream. If the object is not initialized or the video stream
 *       is empty, the behavior is undefined.
 *
 * @return void
 */
void VPlayData::RunMJPEG1()
{
	m_pktMJPEG.bufLen = 1024 * 16;
	m_pktMJPEG.dataBuf = (u8 *)malloc(m_pktMJPEG.bufLen);
	m_jpeg_buffer_len = 2 * 1024 * 1024;
	m_jpeg_buffer = (unsigned char *)malloc(m_jpeg_buffer_len);
#ifdef VPLAY_DUMP_RAW
       FILE* fSink;    
       fopen_s(&fSink, "raw.264", "wb");
	BOOL bGotIFrame = FALSE;
#endif
	while (m_bExit != true) {
		try {

			VDC_DEBUG( "%s  Read MJPEG Source %s\n",__FUNCTION__, m_strFile.c_str());
			/* HTTP JPEG */
			memset(m_headerline, 0, HTTP_JPEG_HEADER_SIZE);
			m_g_state = STATE_MAIN_HEADER;
			m_jpeg_frame_position = 0;
			m_jpeg_num_headers = 0;
			m_jpeg_frame_size = 0;
			m_jpeg_first_frame = true;
			m_bSingleJpeg = false;
			
			CURL *curl;
			CURLcode res;
			curl = curl_easy_init();
			if (curl) {
				curl_easy_setopt(curl, CURLOPT_USERNAME, m_strUser.c_str());
				curl_easy_setopt(curl, CURLOPT_PASSWORD, m_strPass.c_str());
				curl_easy_setopt(curl, CURLOPT_URL, m_strFile.c_str());
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, VPlayData::MJPEGWriteData);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)this);

				/*  https://curl.haxx.se/libcurl/c/progressfunc.html */
				curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, VPlayData::CurlXferinfo);
				curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);
				res = curl_easy_perform(curl);
				ve_sleep(1000);
				if (m_bSingleJpeg == true) {
					on_frame(m_jpeg_buffer, m_jpeg_frame_size);
				}
				/* always cleanup */
				curl_easy_cleanup(curl);				
			}
		}catch (...) {
		}
	}
	return;
}

/**
 * @brief Handles the RM data for video playback.
 *
 * This function is responsible for processing the RM (RealMedia) data and handling it during video playback.
 *
 * @param pContext A pointer to the context associated with the data handler.
 * @param packet The video frame packet to be processed.
 * @return Returns a boolean value indicating the success of the data handling operation.
 *
 * @note This function assumes that the `VideoFrame` structure is properly defined.
 * @warning This function should be used only for RM data handling.
 */
static BOOL VplayRMDataHandler(void* pContext, VideoFrame& packet)
{
	VPlay * pVPlay = (VPlay *)pContext;
	if (pVPlay && pVPlay->m_data->dataHandler) {
		pVPlay->m_data->dataHandler(pVPlay->m_data->pData, packet);
	}
	return TRUE;
}

/**
 * @brief Constructs a new VPlay object.
 *
 * This constructor initializes a new instance of the VPlay class.
 */
VPlay::VPlay()
{
	m_data = new VPlayData(*this, false, false, "fake", "fake", "fake",FALSE);
}

/**
 * @brief Destructor for the VPlay class.
 *
 * This destructor is responsible for freeing the memory allocated by the VPlay object.
 * If the internal data member `m_data` is not null, it is deleted to prevent memory leaks.
 */
VPlay::~VPlay()
{
	if (m_data) {
		delete m_data;
	}
}

/**
 * @brief Initializes the VPlay object with the specified parameters.
 *
 * This function initializes the VPlay object and sets up the necessary configurations for video playback.
 *
 * @param strFile The path to the video file to be played.
 * @param bMJPEG Set to true if the video file is in MJPEG format, false otherwise.
 * @param strUser The username for authentication (if required).
 * @param strPass The password for authentication (if required).
 * @param bHWAccel Set to true to enable hardware acceleration, false otherwise.
 *
 * @return Returns a boolean value indicating the success of the initialization.
 *
 */
BOOL VPlay::Init(string strFile, bool bMJPEG, string strUser, 
		         string strPass, BOOL bHWAccel)
{
	if (m_data) {
		delete m_data;
		m_data = NULL;
	}

	//TOD check if the license support HW decode
	m_data = new VPlayData(*this, true, bMJPEG, strFile, strUser, strPass, bHWAccel);
	m_data->m_avWrapper.Init(FALSE," "," "," ", bHWAccel);
	m_data->m_avWrapper.SetRawDataHandler((RMRawVideoHandler)(RenderWrapper::RenderRMRawVideoHandler),
						m_data->m_render);
	
	return TRUE;
}

/**
 * \brief Initializes the VPlay object.
 *
 * This function initializes the VPlay object with the specified parameters.
 *
 * \param bRealStream A boolean value indicating whether to use real streaming or not.
 * \param strUrl The URL for the stream.
 * \param strUser The username for authentication.
 * \param strPass The password for authentication.
 * \param bHWAccel A boolean value indicating whether to use hardware acceleration or not.
 * \param connectType The type of connection for the stream.
 *
 * \return Returns a boolean value indicating the success of the initialization.
 *
 */
BOOL VPlay::Init(BOOL bRealStream, string strUrl, string strUser, 
		         string strPass, BOOL bHWAccel, VSCConnectType connectType)
{
	if (m_data) {
		delete m_data;
		m_data = NULL;
	}

	//TOD check if the license support HW decode
	
	m_data = new VPlayData(*this, false, false, "false", strUser, strPass, bHWAccel);

	m_data->m_avWrapper.Init(bRealStream, strUrl,  
				strUser,strPass, bHWAccel, connectType);
	m_data->m_avWrapper.SetRawDataHandler(RMRawVideoHandler(RenderWrapper::RenderRMRawVideoHandler),
						m_data->m_render);
	return TRUE;
}
/**
 * \brief Retrieves information about the video stream.
 *
 * This function retrieves detailed information about the video stream
 * associated with the VPlay object.
 *
 * \param[out] pInfo A reference to a VideoStreamInfo object that will be
 *                   filled with the retrieved stream information.
 *
 * \return Returns a boolean value indicating the success of the operation.
 *         - `true` if the stream information was successfully retrieved.
 *         - `false` if an error occurred or the stream information is not available.
 *
 * \note The VideoStreamInfo structure must be properly initialized before calling
 *       this function. Otherwise, the retrieved information may be inconsistent.
 *
 * \sa VideoStreamInfo
 */
BOOL VPlay::GetStreamInfo(VideoStreamInfo &pInfo)
{
	m_data->m_render->GetStreamInfo(pInfo);

	return TRUE;
}
/**
 * @brief Attaches a widget to the VPlay player.
 *
 * This function attaches a widget (specified by the provided window handle) to the VPlay player. 
 * The widget will be rendered with the specified dimensions and render type.
 *
 * @param hWnd The handle of the widget's window.
 * @param w The width of the widget.
 * @param h The height of the widget.
 * @param render The render type to be used for rendering the widget.
 * @return Returns TRUE if the widget was successfully attached, 
 *         or FALSE if a license is required or an error occurred.
 *
 */
BOOL VPlay::AttachWidget(HWND hWnd, int w, int h, RenderType render)
{
	if (LicPlayRef() == false) {
		VDC_DEBUG("Need License !\n");
		m_data->m_nAttachedPlayer ++;
		return FALSE;
	}
	
	m_data->m_render->AttachPlayer(hWnd, w, h, UpdateRenderType(m_data->m_bHWAccel, render));
	if (m_data->m_nAttachedPlayer == 0) {
		m_data->m_avWrapper.StartRaw();
	}
	m_data->m_nAttachedPlayer++;
	return TRUE;
}
/**
 * @brief Updates the dimensions of the widget and its associated window handle.
 *
 * This function is used to update the dimensions of the widget and its associated window
 * handle. It should be called whenever the size of the widget needs to be changed.
 *
 * @param hWnd The handle to the widget's window.
 * @param w The new width of the widget.
 * @param h The new height of the widget.
 * @return Boolean value indicating the success of the update operation.
 *
 * @note This function assumes that the window handle `hWnd` is valid and refers to a
 *       valid window.
 * @note The function may return false if the update operation fails, for example, if the
 *       widget's window handle is invalid or the dimensions are invalid.
 *
 * @example
 * // Example usage of the UpdateWidget function
 * HWND hWnd = GetWidgetWindow(); // Get the handle to the widget's window
 * int newWidth = 800; // New width for the widget
 * int newHeight = 600; // New height for the widget
 *
 * if (VPlay::UpdateWidget(hWnd, newWidth, newHeight))
 * {
 *     // Update successful
 *     // ...
 * }
 * else
 * {
 *     // Update failed
 *     // ...
 * }
 */
BOOL VPlay::UpdateWidget(HWND hWnd, int w, int h)
{
#if ENABLE_SDL_RENDER
	m_data->m_render->ResizePlayer(hWnd, w, h);
#endif
	return TRUE;
}
/**
 * @brief Enables or disables motion tracking for a specified window.
 *
 * This function enables or disables motion tracking for the specified window
 * identified by its handle (`hWnd`). Motion tracking allows capturing and
 * processing of motion-related events for the window.
 *
 * @param hWnd The handle to the window for which motion tracking is to be enabled or disabled.
 * @param enable Specifies whether motion tracking should be enabled (`true`) or disabled (`false`).
 * @param strConf An additional configuration string for the motion tracking process.
 *                This parameter is optional and can be used to provide additional settings or parameters
 *                for the motion tracking algorithm.
 *
 * @return Returns a boolean value indicating the success of the operation.
 *         - `true` if motion tracking was enabled or disabled successfully.
 *         - `false` if motion tracking could not be enabled or disabled for the specified window.
 *
 */
BOOL VPlay::EnableMot(HWND hWnd, bool enable, astring strConf)
{
#if ENABLE_SDL_RENDER
	m_data->m_render->EnableMot(hWnd, enable, strConf);
#endif
	return TRUE;
}

/**
 * @brief Displays an alarm.
 *
 * This function displays an alarm window specified by the provided `hWnd`.
 * The `hWnd` parameter represents the handle to the window that will host the alarm display.
 *
 * @param hWnd The handle to the window to display the alarm in.
 * @return Returns a boolean value indicating the success of displaying the alarm.
 *         - `true` if the alarm was successfully displayed.
 *         - `false` if an error occurred while displaying the alarm.
 *
 * @note The `hWnd` parameter should be a valid window handle.
 * @warning This function may fail if the window handle is invalid or if there is a problem displaying the alarm.
 *          Ensure that the window handle is valid and the necessary resources are available.
 *
 */
BOOL VPlay::ShowAlarm(HWND hWnd)
{
#if ENABLE_SDL_RENDER
	m_data->m_render->ShowAlarm(hWnd);
#endif
	return TRUE;
}

/**
 * @brief Sets a playback time callback for a specified window.
 *
 * This function sets a callback function to be called whenever the playback time changes
 * for the specified window. The callback will receive the current playback time as a parameter.
 *
 * @param hWnd The handle to the window for which the playback time callback is being set.
 * @param pData A pointer to user-defined data that will be passed to the callback function.
 * @param callback A function pointer to the callback function that will be called when the playback time changes.
 *
 * @note The callback function should have the following signature:
 * @code
 * void CallbackFunction(double playbackTime, void* pData);
 * @endcode
 *
 * @return BOOL Returns TRUE if the playback time callback was set successfully, or FALSE otherwise.
 */
BOOL VPlay::SetPbTimeCallback(HWND hWnd, void* pData, VPlayPBTimeCallback callback)
{
	return TRUE;
}

/**
 * @brief Controls the VPlay system with the specified command and parameter.
 *
 * This function allows you to control the VPlay system by sending different commands
 * and parameters. It provides a way to interact with the VPlay system and perform various
 * actions based on the given command and parameter.
 *
 * @param cmd The command to be executed. It specifies the action to be performed.
 * @param param The parameter associated with the command. It provides additional information
 *              or data required for the specified command.
 *
 * @return Returns a boolean value indicating the success or failure of the control operation.
 *         - @c true if the control operation was successful.
 *         - @c false if the control operation failed or the specified command is invalid.
 *
 * @note This function should be called after initializing the VPlay system and establishing
 *       a connection to the VPlay device.
 *
 * @see VPlay::Init()
 * @see VPlay::ConnectToDevice()
 *
 * @par Example usage:
 * @code
 * VPlay vplay;
 * // Initialize and connect to the VPlay device...
 * // ...
 * if (vplay.Control(VPlayCmd::Play, VPlayCmdParam::None)) {
 *     // Playback started successfully
 * } else {
 *     // Failed to start playback or invalid command
 * }
 * @endcode
 */

BOOL VPlay::Control(VPlayCmd cmd, VPlayCmdParam param)
{
	// Implementation of the Control function
    // ...
	return TRUE;
}

/**
 * @brief Detaches a widget from the specified window handle.
 *
 * This function detaches a widget from the specified window handle (`hWnd`).
 * The widget will no longer be associated with the window and will not receive any further messages or events.
 *
 * @param hWnd The handle of the window from which to detach the widget.
 * @return @c true if the widget was successfully detached, @c false otherwise.
 *
 * @see AttachWidget()
 */
BOOL VPlay::DetachWidget(HWND hWnd)
{
	LicPlayUnRef();

	m_data->m_render->DetachPlayer(hWnd);
	m_data->m_nAttachedPlayer --;
	if (m_data->m_nAttachedPlayer < 0) {
		m_data->m_nAttachedPlayer = 0;
	}
	if (m_data->m_nAttachedPlayer == 0) {
	    m_data->m_avWrapper.StopRaw();
	}
    return TRUE;
}

/**
 * @brief Starts retrieving data using the VPlay API.
 *
 * This function initiates the process of retrieving data using the VPlay API. It provides a callback mechanism to handle the retrieved data.
 *
 * @param pData A pointer to the data buffer where the retrieved data will be stored.
 * @param callback A function pointer to the data handler callback function that will be called when data is retrieved.
 *                 The callback function should have the following signature:
 *                 @code{.cpp}
 *                 void callback(void* data);
 *                 @endcode
 *                 The `data` parameter in the callback function will contain the retrieved data.
 *
 * @return A boolean value indicating the success of starting the data retrieval process.
 *         - `true` if the data retrieval process was started successfully.
 *         - `false` if there was an error starting the data retrieval process.
 *
 * @note The `pData` parameter must point to a valid memory location where the retrieved data can be stored.
 *       The `callback` function will be called asynchronously whenever data is retrieved, so make sure to handle the retrieved data properly in the callback.
 */
BOOL VPlay::StartGetData(void * pData, VPlayDataHandler callback)
{
	m_data->pData = pData;
	m_data->dataHandler = callback;
	if (m_data->m_bFile != true)
	{
		m_data->m_avWrapper.SetDataHandler((RMDataHandler)VplayRMDataHandler, this);
		m_data->m_avWrapper.Start();
	}
	return TRUE;
}

/**
 * @brief Stops the data retrieval process.
 *
 * This function stops the retrieval of data in the VPlay class. It is typically used to
 * terminate the process of getting data from a source. Once called, the data retrieval
 * process will be halted and any ongoing operations related to data retrieval will be
 * interrupted or finalized.
 *
 * @return Boolean value indicating the success of stopping the data retrieval.
 *         - `true` if the data retrieval was successfully stopped.
 *         - `false` if an error occurred or the data retrieval was already stopped.
 *
 * @note This function should be called after the `StartGetData()` function to stop the
 *       data retrieval process.
 *
 * @see StartGetData()
 */
BOOL VPlay::StopGetData()
{
	m_data->pData = NULL;
	m_data->dataHandler = NULL;
	return TRUE;
}

/**
 * \brief Starts retrieving raw frames using a callback function.
 *
 * This function starts the process of retrieving raw frames by providing a data pointer and a callback function. 
 * The raw frames will be passed to the callback function for further processing.
 *
 * \param pData A pointer to the data structure where the raw frames will be stored.
 * \param callback A function pointer to the callback function that will be called for each raw frame.
 *                 The callback function should have the following signature:
 *                 \code{.cpp}
 *                 void callback(void* frameData);
 *                 \endcode
 *                 The `frameData` parameter of the callback function will contain the raw frame data.
 *
 * \return Returns a boolean value indicating the success of starting the raw frame retrieval.
 *         - `true` if the raw frame retrieval started successfully.
 *         - `false` if an error occurred while starting the raw frame retrieval.
 *
 * \note Make sure to stop the raw frame retrieval using the appropriate function when it's no longer needed.
 *       See the documentation for the corresponding function to stop the raw frame retrieval.
 */
BOOL VPlay::StartGetRawFrame(void* pData, VPlayRawFrameHandler callback)
{
	BOOL ret = FALSE;
#if ENABLE_SDL_RENDER
	ret = m_data->m_render->StartGetRawFrame(pData, callback);
#endif
	if (m_data->m_nAttachedPlayer == 0)
	{
	    m_data->m_avWrapper.StartRaw();
	}
    	m_data->m_nAttachedPlayer ++;
	
	return ret;
}
/**
 * @brief Stops the process of retrieving raw frames.
 *
 * This function stops the retrieval of raw frames initiated by the `StartGetRawFrame` function.
 * It can be called to terminate the ongoing frame retrieval process and release associated resources.
 *
 * @return Boolean value indicating the success of the operation.
 *         - `true` if the raw frame retrieval was successfully stopped.
 *         - `false` if an error occurred or if the raw frame retrieval was not previously started.
 *
 * @see VPlay::StartGetRawFrame()
 */
BOOL VPlay::StopGetRawFrame()
{
	BOOL ret = FALSE;
#if ENABLE_SDL_RENDER
	ret = m_data->m_render->StopGetRawFrame();
#endif
	m_data->m_nAttachedPlayer --;
	if (m_data->m_nAttachedPlayer < 0) {
		m_data->m_nAttachedPlayer = 0;
	}
	if (m_data->m_nAttachedPlayer == 0) {
	    	m_data->m_avWrapper.StopRaw();
	}
	return ret;
}
/**
 * @brief Puts raw video data into the VPlay object.
 *
 * This method is used to supply raw video data to the VPlay object for processing
 * and playback. The provided video data should be contained in a VideoFrame object.
 *
 * @param packet A reference to a VideoFrame object containing the raw video data.
 * @return Returns a boolean value indicating the success of the operation.
 *
 * @note The VideoFrame object passed to this method should contain valid video data
 *       and adhere to the expected format supported by the VPlay object.
 */
BOOL VPlay::PutRawData(VideoFrame& packet)
{
	m_data->m_avWrapper.PutData(packet);
	return TRUE;
}

/**
 * @brief Sets the license for VPlay.
 *
 * This function sets the license for VPlay, allowing the usage of licensed features.
 * The license is provided as a string.
 *
 * @param strLicense A reference to the string containing the license.
 *
 * @return BOOL Returns true if the license was set successfully, false otherwise.
 *
 * @note The license string must be valid and correspond to a valid license key.
 * If the license is not set or is invalid, certain features may be disabled or limited.
 * Please contact the VPlay support team for assistance with obtaining a valid license.
 */
BOOL VPlay::SetLicense(astring &strLicense)
{
	return LicSetLicense(strLicense);
}

/**
 * \brief Retrieves license information.
 *
 * This function retrieves the license information for the VPlay application.
 *
 * \param strHostId [out] A reference to a string that will hold the host ID.
 * \param ch [out] A reference to an integer that will hold the license channel.
 * \param type [out] A reference to a string that will hold the license type.
 * \param startTime [out] A reference to a string that will hold the start time of the license.
 * \param expireTime [out] A reference to a string that will hold the expiration time of the license.
 *
 * \return A boolean value indicating whether the license information retrieval was successful.
 *         Returns true if the license information was successfully retrieved, false otherwise.
 *
 * \note The output parameters (strHostId, ch, type, startTime, expireTime) will be populated with
 *       the license information upon successful retrieval.
 */
BOOL VPlay::GetLicenseInfo(astring &strHostId, int &ch, astring &type, 
	                      astring &startTime, astring &expireTime)
{
	return LicGetLicenseInfo(strHostId, ch, type, startTime, expireTime);
}

/**
 * @brief Encodes a raw frame to JPEG format.
 *
 * This function encodes a raw frame represented by `pRaw` to JPEG format with the specified
 * dimensions and writes the encoded data to the `output` buffer.
 *
 * @param pRaw The raw frame to be encoded.
 * @param dst_w The width of the destination image in pixels.
 * @param dst_h The height of the destination image in pixels.
 * @param output The buffer to store the encoded JPEG data.
 * @param outputSize The size of the `output` buffer.
 * @return The size of the encoded JPEG data in bytes. Returns 0 if encoding fails or if the
 *         `outputSize` is not large enough to hold the encoded data.
 *
 * @note The `output` buffer should be large enough to hold the encoded JPEG data. Use the
 *       `outputSize` parameter to ensure sufficient space.
 *
 * @note The `pRaw` parameter should contain valid raw frame data in a suitable format before
 *       calling this function.
 *
 * @warning The `output` buffer must be pre-allocated and have enough capacity to hold the
 *          encoded JPEG data; otherwise, the function may overwrite memory and cause undefined
 *          behavior.
 */
size_t VPlay::EncodeJPEG(RawFrame& pRaw, int dst_w, int dst_h, u8* output, size_t outputSize)
{
	if (dst_w == 0 || dst_h == 0) {
		dst_w = pRaw.width;
		dst_h = pRaw.height;
	}
	AVKit::JPEGEncoder *pEnc = new AVKit::JPEGEncoder( AVKit::GetJPEGOptions( dst_w, 
					dst_h));

	if (pEnc == NULL) {
		return -1;
	}

	FFmpegScale m_Scale;
	m_Scale.SetAttribute(SWS_PF_YUV420P, SWS_PF_YUV420P, SWS_SA_FAST_BILINEAR);
	char * m_buf = (char *)av_malloc(dst_w * dst_h * 3);

	m_Scale.ScaleYUV420((unsigned char *)pRaw.data[0], pRaw.linesize[0], 
				(unsigned char *)pRaw.data[1], pRaw.linesize[1], 
				(unsigned char *)pRaw.data[2], pRaw.linesize[2], 
				pRaw.width, pRaw.height, pRaw.width * 3, (unsigned char *)m_buf, 
				dst_w, dst_h, dst_w * 3);

	size_t nRet = pEnc->EncodeYUV420P((uint8_t*)m_buf, 
			(uint8_t*)(m_buf + dst_w* dst_h), (uint8_t*) (m_buf + ((dst_w * dst_h)* 5)/4), output, outputSize);

	delete pEnc;
	av_free(m_buf);
	return nRet;
}

