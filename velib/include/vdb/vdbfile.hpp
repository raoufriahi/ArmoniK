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
#include "vdbtype.hpp"

class VE_LIBRARY_API VdbFile
{
public:
    VdbFile(string & strPath);
    ~VdbFile();
	
	BOOL Open(int flags, int mode);
	BOOL Lseek(int offset, int whence);
	BOOL Close();

	int Read(void * buf, int len);
	int Write(void * buf, int len);
	BOOL Fflush();
	int Size();

private:
    string m_strPath;
	VDB_HANDLE m_handle;
};


