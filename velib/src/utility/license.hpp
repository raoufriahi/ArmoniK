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
#ifndef __V_LICENSE_HPP__
#define __V_LICENSE_HPP__
#include "utility.hpp"
#include "Poco/Environment.h"

using Poco::Environment;

BOOL SetIc(string lic);
BOOL IsIced();
BOOL LicSetLicense(astring &strLicense);
BOOL LicGetLicenseInfo(astring &strHostId, int &ch, astring &type, 
				astring &startTime, astring &expireTime);
bool LicRecordRef();
bool LicRecordUnRef();
bool LicPlayRef();
bool LicPlayUnRef();
#endif /* __V_LICENSE_HPP__ */
