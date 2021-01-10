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
#include "vpoco.hpp"

#include "Poco/Notification.h"
#include "Poco/NotificationQueue.h"
#include "Poco/ThreadPool.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Poco/Random.h"
#include "Poco/AutoPtr.h"
#include "Poco/Net/HTTPCredentials.h" 


using Poco::NotificationCenter;
using Poco::NotificationQueue;
using Poco::Notification;
using Poco::AutoPtr;

static bool VHTTPClient(astring strHost, astring strPath, astring strUser, astring strPasswd, astring strBody
							astring &pResponse, const std::string& method)
{
	try {
		Poco::Net::HTTPCredentials creds(strUser, strPasswd);
		
		Poco::Net::HTTPClientSession session(strHost);
		Poco::Net::HTTPRequest request(method, strPath, HTTPMessage::HTTP_1_1);
		session.sendRequest(request);
		Poco::Net::HTTPResponse response;
		std::istream& firstResponseStream = session.receiveResponse(response);
		std::stringstream firstStrStream;
		Poco::StreamCopier::copyStream(firstResponseStream, firstStrStream);
		
		if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
		{
			creds.authenticate(request, response);
			session.sendRequest(request);
		}
		
		std::istream& bodyStream = session.receiveResponse(response);
		if (response.getStatus() != Poco::Net::HTTPResponse::HTTP_OK)
		{
			return false;
		}
		
		Poco::StreamCopier::copyToString(bodyStream, pResponse);
		
	}catch (Poco::Exception& e) {
		std::cout << e.displayText() << std::endl;
		return false;
	}
	
	return true;
}

bool VHttpClientGet(astring strHost, astring strPath, astring strUser, astring strPasswd, 
							astring &pResponse)
{
	astring strBody = "dummy";
	return VHTTPClient(strHost, strPath, strUser, strPasswd, strBody, pResponse, HTTPRequest::HTTP_GET);
}
bool VHttpClientPost(astring strHost, astring strPath, astring strUser, astring strPasswd, astring strBody
							astring &pResponse)
{
	return VHTTPClient(strHost, strPath, strUser, strPasswd, strBody, pResponse, HTTPRequest::HTTP_POST);
}

