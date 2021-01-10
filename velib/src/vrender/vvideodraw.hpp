/*
 * Copyright (c) 2017-2018 Heimdall
 *
 * The computer program contained herein contains proprietary
 * information which is the property of Heimdall.
 * The program may be used and/or copied only with the written
 * permission of Heimdall or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#ifndef __V_VIDEO_DRAW_HPP__
#define __V_VIDEO_DRAW_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "utility/videotype.hpp"
#include "video_render.h"

extern "C"
{
#include "libavfilter/avfiltergraph.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
#include "libavutil/avutil.h"
#include "libavutil/imgutils.h"
};

using namespace UtilityLib;

class VVideoDraw
{
public:

	VVideoDraw();
	~VVideoDraw();
public:
	bool ProcessFrame(RenderFrame& frame, RenderFrame& outFrame);
	bool ReleaseFrame();
	bool Init(int w, int h);
	bool UnInit();
private:
	int m_width; 
	int m_height;
	int m_format;
	astring m_strText;
	bool m_bInit;

	AVFrame *m_frame_in;
	AVFrame *m_frame_out;
	unsigned char *m_frame_buffer_in;
	unsigned char *m_frame_buffer_out;

	AVFilterContext *m_buffersink_ctx;
	AVFilterContext *m_buffersrc_ctx;
	AVFilterGraph *m_filter_graph;
	AVFilter *m_buffersrc;
	AVFilter *m_buffersink;
	AVFilterInOut *m_outputs;
	AVFilterInOut *m_inputs;
	AVBufferSinkParams *m_buffersink_params;
	FILE *fp_out;
	
	
	
};
#endif /* __V_VIDEO_DRAW_HPP__ */
