/*
 * Copyright (c) 2017-2023 Heimdall
 *
 * The computer program contained herein contains proprietary
 * information which is the property of Heimdall.
 * The program may be used and/or copied only with the written
 * permission of Heimdall or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#ifndef __V_LICENSE_HPP__
#define __V_LICENSE_HPP__
#include "utility.hpp"
#include "Poco/Environment.h"

using Poco::Environment;

/**
 * Sets the license.
 *
 * \param lic The license to be set.
 * \return True if the license was set successfully, false otherwise.
 */
BOOL SetIc(string lic);

/**
 * Checks if the license is valid.
 *
 * \return True if the license is valid, false otherwise.
 */
BOOL IsIced();

/**
 * Sets the license using the provided license string.
 *
 * \param strLicense A reference to the license string.
 * \return True if the license was set successfully, false otherwise.
 */
BOOL LicSetLicense(astring &strLicense);

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
                      astring &startTime, astring &expireTime);

/**
 * Increments the license reference count for recording.
 *
 * \return True if the reference count was incremented successfully, false otherwise.
 */
bool LicRecordRef();

/**
 * Decrements the license reference count for recording.
 *
 * \return True if the reference count was decremented successfully, false otherwise.
 */
bool LicRecordUnRef();

/**
 * Increments the license reference count for playback.
 *
 * \return True if the reference count was incremented successfully, false otherwise.
 */
bool LicPlayRef();

/**
 * Decrements the license reference count for playback.
 *
 * \return True if the reference count was decremented successfully, false otherwise.
 */
bool LicPlayUnRef();

#endif /* __V_LICENSE_HPP__ */
