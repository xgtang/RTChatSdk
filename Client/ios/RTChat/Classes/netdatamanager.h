//
//  netdatamanager.h
//  RTChat
//
//  Created by wang3140@hotmail.com on 14-7-28.
//  Copyright (c) 2014年 RTChatTeam. All rights reserved.
//

#ifndef __RTChat__netdatamanager__
#define __RTChat__netdatamanager__

#include <iostream>
#include "WebSocket.h"
#include "MediaSample.h"
#include "TimeCounter.h"

namespace rtchatsdk {
    
class NetDataManager : public WebSocket::Delegate {
public:
    NetDataManager();
    virtual ~NetDataManager();
    
    void init(const std::string& controlserver);
    
    /// 开启网络工作线程
    void activity();
    
    /// 停止网络工作线程
    void deactive();
    
    /// 发送客户端消息给服务器
    void sendClientMsg(const unsigned char* msg, unsigned int len);
    
    ///销毁websocket
    void destroyWebSocket();
    
    //获取底层websocket状态
    WebSocket::State getWebSocketState();
    
    /// 重置重连次数
    void resetRetryCount();
    
    /// 获得重连次数
    int getRetryCount();
    
    bool Process();
    
    virtual void onOpen(WebSocket* ws);
    virtual void onMessage(WebSocket* ws, const WebSocket::Data& data);
    virtual void onClose(WebSocket* ws);
    virtual void onError(WebSocket* ws, const WebSocket::ErrorCode& error);
    
protected:
    //超时回调
    void connnectionTimeOut(int period);
    
private:
    //连接控制服务器
    void connectControlServer();
    
    //发送心跳消息
    void sendHelloMsg();
    
    //是否达到最大重连次数
    bool haveReachMaxRetryCount();
    
private:
    bool                _haveInited;
    bool                _needCloseConnection;
    pthread_mutex_t     _mutexlock;
    WebSocket*          _socket;
    std::string         _controlServerStr;
    int                 _retrycount;    //重连次数
    unsigned char*      _cryptobuffer;  //加密缓冲区
    unsigned char*      _decryptobuffer;    //解密缓冲区
};
    
}

#endif /* defined(__RTChat__netdatamanager__) */
