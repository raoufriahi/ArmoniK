/*
 * Copyright (c) 2017-2023 UbVideo
 *
 * The computer program contained herein contains proprietary
 * information which is the property of UbVideo.
 * The program may be used and/or copied only with the written
 * permission of UbVideo or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#pragma once

class VwsAPI : public CivetWebSocketHandler
{
public:
	VwsAPI(Factory &pFactory)
		:m_pFactory(pFactory)
	{

	}
	bool handleConnection(CivetServer *server,
	                              const struct mg_connection *conn)
				{
					return true;
				}
								  
								  
								  
	bool handleData(CivetServer *server,
	                        struct mg_connection *conn,
	                        int bits,
	                        char *data,
	                        size_t data_len)
							{
								VDC_DEBUG("%s %s %d %d\n", __FUNCTION__, __FILE__, __LINE__, data_len);
								mg_websocket_write(conn, WEBSOCKET_OPCODE_TEXT, "data1", 5);
								return true;
							}
	void handleClose(CivetServer *server,
	                         const struct mg_connection *conn)
							 {
								 VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
								 return;
							 }
	void handleReadyState(CivetServer *server,
	                              struct mg_connection *conn)
	{
		VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);			  
	}								  
private:
	Factory &m_pFactory;
};
