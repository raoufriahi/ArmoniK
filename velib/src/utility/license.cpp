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
#include "license.hpp"

BOOL SetIc(string lic)
{
	return TRUE;
}
BOOL IsIced()
{
	return TRUE;
}
BOOL LicSetLicense(astring &strLicense)
{
	return TRUE;
}
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
bool LicRecordRef()
{
	return TRUE;
}
bool LicRecordUnRef()
{
	return TRUE;
}
bool LicPlayRef()
{
	return TRUE;
}
bool LicPlayUnRef()
{
	return TRUE;
}
