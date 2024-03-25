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
#include "veventdb.hpp"
#include "Poco/UUIDGenerator.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/Column.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/SharedPtr.h"
#include "Poco/DateTime.h"
#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/SQLite/Connector.h"
#include "debug.hpp"

/* 
1. The event table 
CREATE TABLE events (id INTEGER PRIMARY KEY, device INTEGER, 
value TEXT, numa INTEGER, numb INTEGER, start DATE, end DATE, type TEXT)
*/

using namespace Poco::Data;
using namespace Poco::Data::Keywords;

using Poco::Data::Session;

VEventDB::VEventDB(string & strPath)
: m_strIndexPath(strPath), m_DB(NULL)
{
	Init();
}
VEventDB::~VEventDB()
{
	DeInit();
}

BOOL VEventDB::CreateTablesIfNecessary()
{
	if (m_DB) {										
		/* events  */
		*m_DB << "CREATE TABLE IF NOT EXISTS events (id INTEGER PRIMARY KEY, device TEXT, "
		"devname TEXT, type TEXT, evttime INTEGER, evttimestr DATE, desc TEXT)", now;
	}
	return TRUE;
}

BOOL VEventDB::Init()
{
	Poco::Path path(m_strIndexPath);
	path.append("idb");
	Poco::File file(path);
	file.createDirectories();
	SQLite::Connector::registerConnector();
	VDC_DEBUG("Create the DB path %s\n", path.toString().c_str());
	
	Poco::Path filePath("eventdb.vi");
	path.append(filePath);
	path.makeFile();
	VDC_DEBUG("Create the DB file %s\n", path.toString().c_str());

	m_DB = new Poco::Data::Session("SQLite", path.toString());
	
    CreateTablesIfNecessary();
	
	return TRUE;
}

BOOL VEventDB::NewEvent(string strDev, string strDevName, string strType, time_t nEvtTime, string strDesc)
{
	XGuard guard(m_cMutex);
	string strEvtTime;
	long recordId;
	strEvtTime = Time2String(nEvtTime);
	*m_DB << "INSERT INTO records VALUES(NULL, :device, :devname, :type, :evttime, :evttimestr, :desc)", 
	use(strDev), use(strDevName), use(strType), 
	use(nEvtTime), use(strEvtTime), use(strDesc), now;	
	
	return true;
}

BOOL VEventDB::DeInit()
{
	if (m_DB){
		delete m_DB;
	}
	return TRUE;
}

