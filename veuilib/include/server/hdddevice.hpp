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


#include "utility.hpp"
#include "leveldb/db.h"
#include "config/conf.hpp"
#include "debug.hpp"
#include <QtCore/QStorageInfo>
#include <QtCore/QFileInfoList>
#include <QtCore/QDir>

using namespace UtilityLib;

/* This is same with QStorageInfo define */
enum HddDriveType {
	HddUnknownDrive = 0,
	HddInternalDrive,
	HddRemovableDrive,
	HddRemoteDrive,
	HddCdromDrive,
	HddRamDrive
};

inline HddDriveType HddGetDriveType(const string &drive)
{
	return HddInternalDrive;
}



