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
#ifndef SDL_RENDER_H
#define SDL_RENDER_H

#include "video_render.h"
#include "utility.hpp"
#include "debug.hpp"
#include "ffmpeg_scale.hpp"
#include "SDL.h"
#include "vplay/vsmotalgo.hpp"
#include "vvideodraw.hpp"

#ifdef WIN32
#define USING_LIBYUV 1
#endif
using namespace UtilityLib;

class sdl_render : public video_render
{
public:
	virtual bool init_render(void* ctx, int w, int h, int pix_fmt);

	virtual bool render_one_frame(RenderFrame* data, int pix_fmt);

	virtual void re_size(int width, int height);

	virtual void aspect_ratio(int srcw, int srch, bool enable_aspect);

	virtual void destory_render();
	
	virtual bool use_overlay();

	virtual void enable_mot(bool enable, astring strConf);
	
	virtual void show_alarm();
	
public:
	bool Close();
	bool Init(int width, int height);
	sdl_render();
	~sdl_render();
private:
    fast_mutex m_Lock;
#ifndef USING_LIBYUV
	FFmpegScale m_Scale;
#endif
	void * m_hWnd;
	bool m_bInited; 
	SDL_Window *m_SdlWin;
	SDL_Renderer *m_SdlRender;
	SDL_Texture *m_pTex;
	HWND m_hwnd;
	int m_image_width;
	int m_image_height;

	bool m_keep_aspect;

	float m_window_aspect;	
	int m_pix_fmt;
	char * m_buf;
	int m_bufSize;

	char * m_buf2;
	int m_bufSize2;

	bool m_motEnable;
	int m_alarmTime;
	bool m_alarmEnable;
	VSMotAlgo *m_MotAlgo;
	//VVideoDraw m_videoDraw;
};

#endif // SDL_RENDER_H
