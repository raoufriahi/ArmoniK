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
#include "license.h"

/**
 * Sets the license.
 *
 * \param lic The license to be set.
 * \return True if the license was set successfully, false otherwise.
 */
BOOL SetIc(string lic)
{
	return TRUE;
}

/**
 * Checks if the license is valid.
 *
 * \return True if the license is valid, false otherwise.
 */
BOOL IsIced()
{
	return TRUE;
}
/**
 * Sets the license using the provided license string.
 *
 * \param strLicense A reference to the license string.
 * \return True if the license was set successfully, false otherwise.
 */
BOOL LicSetLicense(astring &strLicense)
{
	return TRUE;
}

/**
 * Retrieves information about the license.
 *
 * \param strHostId A reference to the host ID.
 * \param ch A reference to the license channel.
 * \param type A reference to the license type.
 * \param startTime A reference to the license start time.
 * \param expireTime A reference to the license expiration time.
 * \return True if the license information was retrieved successfully, false otherwise.
 */
BOOL LicGetLicenseInfo(astring &strHostId, int &ch, astring &type, 
                      astring &startTime, astring &expireTime)
{
	strHostId = "HEIMDALL::COMMUNITY";
	ch = 512;
	type = "Community";
	startTime = std::to_string((s64)(time(NULL)));
	expireTime = std::to_string((s64)(time(NULL)));
	return TRUE;
}

/**
 * Increments the license reference count for recording.
 *
 * \return True if the reference count was incremented successfully, false otherwise.
 */
bool LicRecordRef()
{
	return TRUE;
}
/**
 * Decrements the license reference count for recording.
 *
 * \return True if the reference count was decremented successfully, false otherwise.
 */
bool LicRecordUnRef()
{
	return TRUE;
}
/**
 * Increments the license reference count for playback.
 *
 * \return True if the reference count was incremented successfully, false otherwise.
 */
bool LicPlayRef()
{
	return TRUE;
}
/**
 * Decrements the license reference count for playback.
 *
 * \return True if the reference count was decremented successfully, false otherwise.
 */
bool LicPlayUnRef()
{
	return TRUE;
}
