/*
 * Copyright (c) 2017-2024 UbVideo
 *
 * The computer program contained herein contains proprietary
 * information which is the property of UbVideo.
 * The program may be used and/or copied only with the written
 * permission of UbVideo or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#pragma once

#include "pbsession.hpp"


/* pb callback */
typedef void ( * PBCallbackFunctionPtr)(VideoFrame& frame,  void * pParam);
typedef void ( * PBTimeCallbackFunctionPtr)(unsigned int currTime, bool bPause, void * pParam);


class VDB;
class PlaybackWrapperImpl;
class VE_LIBRARY_API PlaybackWrapper
{
public:
    PlaybackWrapper(VDB &pVdb, string deviceId, unsigned int nPlaytime, 
				PBCallbackFunctionPtr callback, 
				BOOL eolLoop, void * pParam);
    ~PlaybackWrapper();

	BOOL SeekToTime(unsigned int seekTime);
	BOOL StartPlay();
	BOOL PausePlay();/* same with pause */
	BOOL QuitPlay();/* End of play */
	BOOL SetSpeed(float fSpeed);
	BOOL SetDirection(BOOL bForward);
	BOOL SetTimeCallback(PBTimeCallbackFunctionPtr callback, void * pParam);
	/* main loop for playback */
	void run();
	
private:
	PlaybackWrapperImpl *m_pImpl;
};

