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
#ifndef __VDB_FILE_HPP__
#define __VDB_FILE_HPP__
#include "utility.hpp"
#include "vdbtype.hpp"

class VE_LIBRARY_API VdbFile
{
public:
    VdbFile(astring & strPath);
    ~VdbFile();
	
public:
	BOOL Open(s32 flags, s32 mode);
	BOOL Lseek(s32 offset, s32 whence);
	
	
public:
	BOOL Close();
	s32 Read(void * buf, s32 len);
	s32 Write(void * buf, s32 len);
	BOOL Fflush();
	s32 Size();

public:


private:
    astring m_strPath;
	VDB_HANDLE m_handle;
};

#endif /* __VDB_FILE_HPP__ */
