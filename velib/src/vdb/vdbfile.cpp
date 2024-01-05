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
#include "vdbfile.hpp"

VdbFile::VdbFile(string & strPath)
{
	m_strPath = strPath;
	Open(O_RDWR, 0);
}
VdbFile::~VdbFile()
{
	Fflush();
	Close();
}

BOOL VdbFile::Close()
{
	close(m_handle);
	return TRUE;
}
int VdbFile::Read(void * buf, int len)
{
	return read(m_handle, buf, len);
}
int VdbFile::Write(void * buf, int len)
{
	return write(m_handle, buf, len);
}
BOOL VdbFile::Fflush()
{
	fsync(m_handle);	
	return TRUE;
}


int VdbFile::Size()
{
    struct stat buf;

    if (fstat(m_handle, &buf) == -1)
            return FALSE;
    return (buf.st_size);
}

BOOL VdbFile::Open(int flags, int mode)
{
	if ((flags & O_CREAT) != O_CREAT){
		m_handle = open(m_strPath.c_str(), flags, mode);
		if (m_handle < 0){
			return FALSE;
		}
		return TRUE;
	}

	m_handle = open(m_strPath.c_str(), flags, mode);
	if (m_handle < 0){
		return FALSE;
	}
	return TRUE;
}

BOOL VdbFile::Lseek(int offset, int whence)
{
   return (lseek(m_handle, offset, whence) < 0) ? FALSE:TRUE;
}
