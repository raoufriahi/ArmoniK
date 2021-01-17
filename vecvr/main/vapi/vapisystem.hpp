
#pragma once
#include "link/linkstream.hpp"
#include "CivetServer.h"
#include "server/factory.hpp"
#include "cppkit/ck_string.h"
#include "cppkit/os/ck_sprintf.h"
#include "rapidmedia/rapidmedia.hpp"
#include "vplay.hpp"
#include <google/protobuf/util/json_util.h>


#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define VAPI_LOGIN_SYSTEM "/api/v1/login"
#define VAPI_KEEPALIVE_SYSTEM "/api/v1/Keepalive"
#define VAPI_LOGOUT_SYSTEM "/api/v1/Logout"
#define VAPI_GET_SYSTEM_INFO "/api/v1/GetSystemInfo"
#define VAPI_GET_CODEC_INFO "/api/v1/GetCodecInfo"
#define VAPI_GET_RUN_INFO "/api/v1/GetRunInfo"
#define VAPI_GET_DEVICE_SUMMARY "/api/v1/GetDeviceSummary"
#define VAPI_GET_VOLUMES_SYSTEM "/api/v1/GetVolumes"

class WebAPISystemHandler : public CivetHandler
{
public:

        WebAPISystemHandler(Factory &pFactory)
                :m_pFactory(pFactory)
        {

        }

    bool
        handleGet(CivetServer *server, struct mg_connection *conn){

            const struct mg_request_info *request_info = mg_get_request_info(conn);
            if(request_info == NULL && request_info->request_uri) {

                return false;
            }

            if(strcmp(VAPI_LOGIN_SYSTEM,request_info->request_uri) == 0){
                return ProcessLoginReq(server,conn);
            } else if (strcmp(VAPI_KEEPALIVE_SYSTEM,request_info->request_uri) == 0){
                // TODO
            } else if (strcmp(VAPI_LOGOUT_SYSTEM,request_info->request_uri) == 0){
                // TODO
            } else if (strcmp(VAPI_GET_SYSTEM_INFO,request_info->request_uri) == 0){
                /* /api/v1/GetSystemInfo?session=xxxxxxxx */

            } else if (strcmp(VAPI_GET_CODEC_INFO,request_info->request_uri) == 0){
                // TODO
            } else if (strcmp(VAPI_GET_RUN_INFO,request_info->request_uri) == 0){
                // TODO
            } else if (strcmp(VAPI_GET_DEVICE_SUMMARY,request_info->request_uri) == 0){
                // TODO
            } else if (strcmp(VAPI_GET_VOLUMES_SYSTEM,request_info->request_uri) == 0){
                // TODO
            }

        return true;
    }


    bool
        handlePost(CivetServer *server, struct mg_connection *conn)
        {
            const struct mg_request_info *request_info = mg_get_request_info(conn);
            if(request_info == NULL && request_info->request_uri) {

                return false;
            }

            if(strcmp(VAPI_LOGIN_SYSTEM,request_info->request_uri) == 0){
                // TODO
            } else if (strcmp(VAPI_KEEPALIVE_SYSTEM,request_info->request_uri) == 0){
                // TODO
            } else if (strcmp(VAPI_LOGOUT_SYSTEM,request_info->request_uri) == 0){
                // TODO
            } else if (strcmp(VAPI_GET_SYSTEM_INFO,request_info->request_uri) == 0){
                // TODO
            } else if (strcmp(VAPI_GET_CODEC_INFO,request_info->request_uri) == 0){
                // TODO
            } else if (strcmp(VAPI_GET_RUN_INFO,request_info->request_uri) == 0){
                // TODO
            } else if (strcmp(VAPI_GET_DEVICE_SUMMARY,request_info->request_uri) == 0){
                // TODO
            } else if (strcmp(VAPI_GET_VOLUMES_SYSTEM,request_info->request_uri) == 0){
                // TODO
            }

            return true;
        }

private:
    void LoginPost(struct mg_connection *conn){

    }

    bool ProcessLoginReq( CivetServer *server,
                             struct mg_connection *conn){
          /* /api/v1/Login?user=admin&password=xxxxxxxx */
        /*{
            "bStatus":true,
            "strSession":"2e4de605-b218-455f-a7e2-8f395267cb46",
            "nTimeout":600
        }*/
       Link::LinkCmd cmdResp;
       LinkLoginResp *resp = new LinkLoginResp;
       resp->set_strnonce("2e4de605-b218-455f-a7e2-8f395267cb46");
       resp->set_bretnonce(true);

       cmdResp.set_allocated_loginresp(resp);

       SendRespMsg(cmdResp, server, conn);
       return true;
    }

    bool SendRespMsg(Link::LinkCmd &resp, CivetServer *server,
                            struct mg_connection *conn) {

            string strMsg;
            ::google::protobuf::util::Status status =
                    ::google::protobuf::util::MessageToJsonString(resp, &strMsg);

            if(status.ok()){

                mg_printf(conn,
                          "HTTP/1.1 200 OK\r\nContent-Type: "
                          "application/json\r\n"
                          "Content-Length: %d\r\n\r\n", strMsg.length());
                mg_printf(conn, strMsg.c_str());
            }else {

                mg_printf(conn,
                          "HTTP/1.1 200 OK\r\nContent-Type: "
                          "text/plain\r\nConnection: close\r\n\r\n");
                mg_printf(conn, "unknown problem!\n");

            }
            return true;
    }

    bool
    handleAll(const char *method,
                  CivetServer *server,
                  struct mg_connection *conn)
        {

            const struct mg_request_info *request_info = mg_get_request_info(conn);


                mg_printf(conn,
                                  "HTTP/1.1 200 OK\r\nContent-Type: "
                                  "text/plain\r\nConnection: close\r\n\r\n");
               mg_printf(conn, "Method! : %s \n", request_info->request_uri); //conn->request_info->local_uri
                return true;
        }

private:
        Factory &m_pFactory;

};
