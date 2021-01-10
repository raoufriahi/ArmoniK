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
#ifndef __EPOLL_WRAPPER_HPP__
#define __EPOLL_WRAPPER_HPP__

#include <sys/epoll.h>
#include <assert.h>


typedef int (*Function_EpollHandler) (int);

#define  MAX_EPOLL_EVENT_NUMBER  1024

class EpollWrapper
{
public:
	EpollWrapper(int iFDSize = 256)
	:m_iEpollWaitingTime(1000)
	{
		EpollCreate(iFDSize);
	}
	~EpollWrapper()
	{
		close(m_iEpollFD);
	}
public:

	void SetHandler_Error(Function_EpollHandler pfError);
	void SetHandler_Read(Function_EpollHandler pfRead);
	void SetHandler_Write(Function_EpollHandler pfWrite);

public:

	int EpollCreate(int iFDSize);
	int EpollWait(int iTimeout);
	bool FDIsSet(int iFD, int iEpollEventNumber, unsigned int &uiEpollEvent);

	int EpollAdd(int iFD);
	int EpollDelete(int iFD);

	int SetNonBlock(int iFD);
    	int SetNagleOff(int iFD);

	inline void SetEpollWaitingTime(int iEpollWaitingTime);

	bool IsErrorEvent(unsigned int uiEvent, int iFD);
	bool IsReadEvent(unsigned int uiEvent);
	bool IsWriteEvent(unsigned int uiEvent);

	int NotifyErrorEvent(int iFD);
	int NotifyReadEvent(int iFD);
	int NotifyWriteEvent(int iFD);
public:
	epoll_event m_lastEpollEvent[MAX_EPOLL_EVENT_NUMBER];

private:

	Function_EpollHandler m_pfError;
	Function_EpollHandler m_pfRead;
	Function_EpollHandler m_pfWrite;

private:

	int m_iEpollFD;
	int m_iEpollEventSize;
	int m_iEpollWaitingTime;
	epoll_event m_stOneEpollEvent;

};

//inline void EpollWrapper::SetEpollEventSize(int iEpollEventSize)
//{
//	m_iEpollEventSize = iEpollEventSize;
//}

inline void EpollWrapper::SetEpollWaitingTime(int iEpollWaitingTime)
{
	m_iEpollWaitingTime = iEpollWaitingTime;
}



#endif
