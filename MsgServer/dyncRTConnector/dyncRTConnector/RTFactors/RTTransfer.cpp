//
//  RTTransfer.cpp
//  dyncRTConnector
//
//  Created by hp on 12/1/15.
//  Copyright (c) 2015 hp. All rights reserved.
//

#include "RTTransfer.h"
#include "http_common.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <sstream>

RTTransfer::RTTransfer()
{

}

RTTransfer::~RTTransfer()
{

}

int RTTransfer::DoProcessData(const char *pData, int nLen)
{
    memset((void*)&m_msg, 0, sizeof(TRANSFERMSG));
    std::string str(pData, nLen), err;
    m_msg.GetMsg(str, err);
    if (err.length() > 0) {
        LE("%s err:%s\n", __FUNCTION__, err.c_str());
        return -1;
    }
    {
        if (m_msg._action == TRANSFERACTION::req) {
            TRANSFERMSG ack_msg;
            ack_msg._action = TRANSFERACTION::req_ack;
            ack_msg._fmodule = TRANSFERMODULE::mconnector;
            ack_msg._type   = m_msg._type;
            ack_msg._trans_seq = m_msg._trans_seq;
            ack_msg._trans_seq_ack = m_msg._trans_seq + 1;
            ack_msg._valid = m_msg._valid;
            ack_msg._content = "";
            const std::string s = ack_msg.ToJson();
            OnTransfer(s);
        } else if (m_msg._action == TRANSFERACTION::req_ack) {
            if (m_msg._trans_seq + 1 == m_msg._trans_seq_ack) {
                // ack ok
                return nLen;
            } else {
                // ack failer
                LE("msg ack failed, seq:%d, seq_ack:%d\n",m_msg._trans_seq, m_msg._trans_seq_ack);
                return -1;
            }
        } else if (m_msg._action == TRANSFERACTION::resp) {
            TRANSFERMSG ack_msg;
            ack_msg._action = TRANSFERACTION::resp_ack;
            ack_msg._fmodule = TRANSFERMODULE::mconnector;
            ack_msg._type   = m_msg._type;
            ack_msg._trans_seq = m_msg._trans_seq;
            ack_msg._trans_seq_ack = m_msg._trans_seq + 1;
            ack_msg._valid = m_msg._valid;
            ack_msg._content = "";
            const std::string s = ack_msg.ToJson();
            OnTransfer(s);
        } else if (m_msg._action == TRANSFERACTION::resp_ack) {
            if (m_msg._trans_seq + 1 == m_msg._trans_seq_ack) {
                // ack ok
                return nLen;
            } else {
                // ack failer
                LE("msg ack failed, seq:%d, seq_ack:%d\n",m_msg._trans_seq, m_msg._trans_seq_ack);
                return -1;
            }
        } else {
            LE("msg action error!!!!!\n");
        }
    }
    {
        if (m_msg._type == TRANSFERTYPE::conn) {
            OnTypeConn(m_msg._fmodule, m_msg._content);
        } else if  (m_msg._type == TRANSFERTYPE::trans) {
            OnTypeTrans(m_msg._fmodule, m_msg._content);
        } else if (m_msg._type == TRANSFERTYPE::queue) {
            OnTypeQueue(m_msg._fmodule, m_msg._content);
        } else if (m_msg._type == TRANSFERTYPE::dispatch) {
            OnTypeDispatch(m_msg._fmodule, m_msg._content);
        } else if (m_msg._type == TRANSFERTYPE::push) {
            OnTypePush(m_msg._fmodule, m_msg._content);
        } else {
            LE("invalid type::%d", m_msg._type);
        }
    }
    return nLen;
}

int RTTransfer::ProcessData(const char* pData, int nLen)
{
    int parsed = 0;
    int ll = 0;
    while (parsed < nLen)
    {
        const char* pMsg = pData + parsed;
        int offset = 0;
        if (*(pMsg+offset) == '$') {
            offset += 1;
            char l[4] = {0};
            memcpy(l, pMsg+offset, 4);
            offset += 4;
            ll = (int)strtol(l, NULL, 10);
            if (ll>0 && ll <= nLen) {
                int nlen = DoProcessData((char *)(pMsg+offset), ll);
                LI("Transfer DoProcessData msg nlen:%d, ll:%d\n", nlen, ll);
                if (nlen < 0) { // the message length may be 0
                    break;
                } else { // nlen < 0
                    assert(nlen == ll);
                    offset += ll;
                    parsed += offset;
                }
            } else { // ll>0 && ll <= nLen
                LE("Get Msg Len Error!!!\n");
            }
        }
    }
    return parsed;
}
