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

#include "utility/debug.hpp"
#include "leveldb/db.h"
#include "config/conf.hpp"
#include "config/vidconf.pb.h"


class VE_LIBRARY_API ClientConfDB
{
public:
    ClientConfDB()  { }
    ~ClientConfDB() { }

    bool Open(string & pPath);

	bool GetCmnParam(string &strKey, string &strParam);
	bool SetCmnParam(string &strKey, string &strParam);
	bool GetLicense(string &strLicense);
	bool SetLicense(string &strLicense);

	/* Client */
	bool GetClientConf(VidClientConf &pData);
	bool SetClientConf(VidClientConf &pData);

	/* Stor */
	bool FindStor(string strStorId);
	bool DeleteStor(string strStorId);
	bool AddStor(VidStor &pStor);
	bool GetStorConf(string strId, VidStor &pStor);

	/* Stor All */
	bool GetStorListConf(VidStorList &pData);
	bool UpdateStorListConf(VidStorList &pData);

	/* View */
	bool FindView(string strViewId);
	bool DeleteView(string strViewId);
	bool AddView(VidView &pView);
	bool GetViewConf(string strId, VidView &pView);

	/* View All */
	bool GetViewListConf(VidViewList &pData);
	bool UpdateViewListConf(VidViewList &pData);

	/* Tour */
	bool FindTour(string strTourId);
	bool DeleteTour(string strTourId);
	bool AddTour(VidTour &pTour);
	bool GetTourConf(string strId, VidTour &pTour);
	/* Group All */
	bool GetTourListConf(VidTourList &pData);
	bool UpdateTourListConf(VidTourList &pData);

	/* Group */
	bool FindGroup(string strGroupId);
	bool DeleteGroup(string strGroupId);
	bool AddGroup(VidGroup &pGroup);
	bool GetGroupConf(string strId, VidGroup &pGroup);
	/* Group All */
	bool GetGroupListConf(VidGroupList &pData);
	bool UpdateGroupListConf(VidGroupList &pData);

	/* Emap */
	bool FindEmap(string strEmapId);
	bool DeleteEmap(string strEmapId);
	bool AddEmap(VidEmap &pEmap);
	bool GetEmapConf(string strId, VidEmap &pEmap);

	/* Emap All */
	bool GetEmapListConf(VidEmapList &pData);
	bool UpdateEmapListConf(VidEmapList &pData);

private:
    std::mutex m_cMutex;
    leveldb::DB* m_pDb;
    leveldb::Options m_Options;
};

typedef ClientConfDB* LPClientConfDB;

