#pragma once
#include "utility/type.hpp"
#include "config/linkproto.pb.h"
#include "config/restsystem.pb.h"
#include "link/linkstream.hpp"
#include "CivetServer.h"
#include "server/factory.hpp"
#include "cppkit/ck_string.h"
#include "cppkit/os/ck_sprintf.h"
#include "rapidmedia/rapidmedia.hpp"
#include "vplay.hpp"
#include <google/protobuf/util/json_util.h>



#include "CivetServer.h"
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
                :m_pFactory(pFactory){

        UUIDGenerator uuidCreator;
        m_seesionId  = uuidCreator.createRandom().toString();
        }
    bool
        handleGet(CivetServer *server, struct mg_connection *conn){

            const struct mg_request_info *request_info = mg_get_request_info(conn);
            if(request_info == NULL && request_info->request_uri == NULL) {

                return false;
            }

            if(!mg_strcasecmp(VAPI_LOGIN_SYSTEM,request_info->request_uri)){
                return ProcessLoginReq(server,conn);
            } else if (!mg_strcasecmp(VAPI_KEEPALIVE_SYSTEM,request_info->request_uri)){
                // TODO
            } else if (!mg_strcasecmp(VAPI_LOGOUT_SYSTEM,request_info->request_uri)){
                // TODO
            } else if (!mg_strcasecmp(VAPI_GET_SYSTEM_INFO,request_info->request_uri)){
                /* /api/v1/GetSystemInfo?session=xxxxxxxx */

            } else if (!mg_strcasecmp(VAPI_GET_CODEC_INFO,request_info->request_uri)){
                // TODO
            } else if (!mg_strcasecmp(VAPI_GET_RUN_INFO,request_info->request_uri)){
                // TODO
            } else if (!mg_strcasecmp(VAPI_GET_DEVICE_SUMMARY,request_info->request_uri)){
                // TODO
            } else if (!mg_strcasecmp(VAPI_GET_VOLUMES_SYSTEM,request_info->request_uri)){
                // TODO
            }

        return true;
    }


    bool
        handlePost(CivetServer *server, struct mg_connection *conn)
        {
            const struct mg_request_info *request_info = mg_get_request_info(conn);
            if(request_info == NULL && request_info->request_uri == NULL) {

                return false;
            }

            if(!mg_strcasecmp(VAPI_LOGIN_SYSTEM,request_info->request_uri)){
                // TODO
            } else if (!mg_strcasecmp(VAPI_KEEPALIVE_SYSTEM,request_info->request_uri)){
                // TODO
            } else if (!mg_strcasecmp(VAPI_LOGOUT_SYSTEM,request_info->request_uri)){
                // TODO
            } else if (!mg_strcasecmp(VAPI_GET_SYSTEM_INFO,request_info->request_uri)){
                // TODO
            } else if (!mg_strcasecmp(VAPI_GET_CODEC_INFO,request_info->request_uri)){
                // TODO
            } else if (!mg_strcasecmp(VAPI_GET_RUN_INFO,request_info->request_uri)){
                // TODO
            } else if (!mg_strcasecmp(VAPI_GET_DEVICE_SUMMARY,request_info->request_uri)){
                // TODO
            } else if (!mg_strcasecmp(VAPI_GET_VOLUMES_SYSTEM,request_info->request_uri)){
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
        string strUser = "";
        string strPassword = "";
        astring realPasswd = "admin";
        SimpleCrypt crypt;
        XMD5 md5Check;
        RestLogin cmdResp ;

        if (CivetServer::getParam(conn, "user", strUser)  == false ||
            CivetServer::getParam(conn, "password", strPassword)  == false ) {
             SendRespMsg( NULL, true,conn);
             return true;
        }

        // here has error QString strDePasswd = m_pFactory.GetAdminPasswd(realPasswd);
        m_pFactory.GetAdminPasswd(realPasswd);
        //realPasswd = crypt.decryptToString(realPasswd).toStdString();
        realPasswd = "admin";
        /* calc the md5 and compare */
        string pass = m_seesionId + realPasswd;
        md5Check.Update((const uint8_t*)(pass.c_str()), pass.length());
        md5Check.Finalize();

        if (md5Check.GetAsString().c_str() == "admin")
        {
            cmdResp.set_strsession("");
            cmdResp.set_bstatus(false);
            cmdResp.set_ntimeout(600);;

        }else {

            cmdResp.set_strsession(m_seesionId);
            cmdResp.set_bstatus(true);
            cmdResp.set_ntimeout(600);
        }

       string strMsg;
       ::google::protobuf::util::Status status =
               ::google::protobuf::util::MessageToJsonString(cmdResp, &strMsg);
       SendRespMsg( strMsg, status.ok(),conn);

       return true;
    }

    bool SendRespMsg(string strMsg, bool status,
                            struct mg_connection *conn) {
            if(status){
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
        astring m_seesionId;
};
