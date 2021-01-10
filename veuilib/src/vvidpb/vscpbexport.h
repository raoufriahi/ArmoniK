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
#ifndef __VSC_PB_EXPORT_H__
#define __VSC_PB_EXPORT_H__

#include "QDateTime"
#include "vtaskmgr.hpp"
#include "pbwrapper.hpp"

#include "utility.hpp"
#include "debug.hpp"
#include "cppkit/ck_string.h"
#include "cppkit/ck_memory.h"
#include "cppkit/ck_command_queue.h"
#include "ffkit/av_muxer.h"
#include "client/storsyncinf.hpp"
#include "client/storstream.hpp"

#include "Poco/Path.h"

using namespace std;
using namespace cppkit;
using namespace ffkit;
using namespace Poco;


class VSCPbExportTask : public VTaskItem
{
	Q_OBJECT
public:
	VSCPbExportTask(astring strExportPath, astring strCamName,
					VidStor &pStor, astring strId, u32 nPlaytime, 
					u32 nEndTime);
	~VSCPbExportTask();
public:
	static void PBCallbackHandler(VideoFrame& frame,  void * pParam);
	void PBCallbackHandler1(VideoFrame& frame);
	
public:
	void run();
public:
	virtual int GetPercentage();
	virtual bool HasOpenResult();
	virtual bool OpenResult();
	virtual bool StopTask();
	virtual bool IsEol();
	virtual std::string GetTaskName();

private:

	StorStream *m_StorStream;
	VidStor m_pStor;
	astring m_strId;
	astring m_strExportPath;
	astring m_strCamName;
	u32 m_nPlaytime; 
	u32 m_nEndTime;
	QDateTime m_qStartTime;
	QDateTime m_qEndTime;
	u32 m_currentTime;
	BOOL m_isEol;
	astring m_fileName;

	codec_options m_options;
	av_muxer *m_pMuxer;
	InfoFrameI m_infoData;
	BOOL m_bGotInfoData;
	bool m_bQuit;

	XMutex m_cMutex;
};

#endif // __VSC_PB_EXPORT_H__
