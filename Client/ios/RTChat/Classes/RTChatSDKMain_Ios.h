//
//  RTChatSDKMain.h
//  RTChat
//
//  Created by wang3140@hotmail.com on 14-7-29.
//  Copyright (c) 2014年 RTChatTeam. All rights reserved.
//

#ifndef __RTChat__RTChatSDKMain__
#define __RTChat__RTChatSDKMain__

#include <iostream>
#include <map>
#include "proto/public.pb.h"
#include "proto/game.pb.h"
#include "RTChatCommonTypes.h"
#include "NetProcess/HttpProcess.h"
#include "webrtc/system_wrappers/interface/thread_wrapper.h"

using namespace webrtc;

namespace rtchatsdk {
    
class NetDataManager;
class MediaSample;
class RTChatBuffStream;
class RTChatBuffStreamPool;
class Public;

typedef std::function<void (SdkResponseCmd cmdType, SdkErrorCode error, const char* dataPtr, uint64_t dataSize)> pMsgCallFunc;

class RTChatSDKMain {
public:
    typedef std::map<int64_t, Cmd::stRoomInfo> RoomInfoMap;
    
public:
    RTChatSDKMain();
    virtual ~RTChatSDKMain();
    
    static RTChatSDKMain& sharedInstance();
    
    /*******************需要暴露给用户的接口开始**********************/
    
    //sdk初始化，只能调用一次(主线程)
    SdkErrorCode initSDK(const std::string& appid, const std::string& key, const char* uniqueid = NULL);
    
    //注册消息回调(主线程)
    void registerMsgCallback(const pMsgCallFunc& func);
    
    //激活SDK(主线程)
    void activateSDK();
    
    //当应用最小化时需要调用这个，清理数据(主线程)
    void deActivateSDK();
    
    //获取SDK当前操作状态，用户发起操作前可以检测一下状态判断可否继续
    SdkOpState getSdkState();
    
    /// 申请房间列表(主线程)
    SdkErrorCode requestRoomList();
    
    /// 创建房间(主线程)
    SdkErrorCode createRoom(enRoomType roomType, enRoomReason reason);
    
    /// 加入房间(主线程)
    SdkErrorCode joinRoom(uint64_t roomid);
    
    /// 离开房间(主线程)
    SdkErrorCode leaveRoom();
    
    /// 加入麦序(主线程)
    SdkErrorCode requestInsertMicQueue();
    
    /// 离开麦序(主线程)
    SdkErrorCode leaveMicQueue();
    
    //是否接收随机聊天，临时增加的接口(主线程)
    void returnRandChatRes(bool isAccept, uint64_t srctempid);
    
    /// 设置本人Mac静音(主线程)
    void setMuteSelf(bool isMute);
    
    /// 开始录制麦克风数据(主线程)
    bool startRecordVoice();
    
    /// 停止录制麦克风数据(主线程)
    bool stopRecordVoice();
    
    /// 取消当前录音
    bool cancelRecordedVoice();
    
    /// 开始播放录制数据(主线程)
    bool startPlayLocalVoice(const char* voiceUrl);
    
    /// 停止播放数据(主线程)
    bool stopPlayLocalVoice();
    
    /// 请求更改排麦房权限(主线程)
    bool requestUpdatePower(uint64_t othertempid, enPowerType powertype);
    
    /// 分配麦(主线程)
    bool requestAssignMic(uint64_t othertempid);
    
    /// 请求和某人聊天(主线程)
    bool requestTalkToOther(const char* uniquegameaccid);
    
    ///开始语音识别
    bool startVoiceToText();
    
    ///停止语音识别
    bool stopVoiceToText();
    
    /// 获取当前地理位置信息
    bool startGetCurrentCoordinate();
    
    /*******************需要暴露给用户的接口结束**********************/
    
    /// 请求逻辑服务器地址(工作线程)
    SdkErrorCode requestLogicInfo();
    
    /// 请求登录(工作线程)
    SdkErrorCode requestLogin(const char* uniqueid = NULL);
    
    /// 收到网络线程消息
    void onRecvMsg(char* data, int len);
    
    /// 获取当前的输入mic静音状态
    bool getMuteSelf();
    
    ///注意。这个接口不应该暴露给上层应用使用
    void set_SdkOpState(SdkOpState state) {
        pthread_mutex_lock(&_mutexLock);
        _sdkOpState = state;
        pthread_mutex_unlock(&_mutexLock);
    };
    
    uint64_t get_sdkTempID() {
        return _sdkTempID;
    };
    
    //http请求回调函数
    void httpRequestCallBack(HttpDirection direction, const char* ptr, int size);
    
    /// 底层音量等级通知
    void voiceLevelNotify(float power);
    
    /// 底层播放结束通知
    void onVoicePlayOver();
    
    /// 语音听写底层回调
    void onReceiveVoiceTextResult(const std::string& text);
    
protected:
    //线程循环调用
    static bool Run(ThreadObj obj);
    
    bool Process();
    
    /// 尝试重连服务器
    void tryConnectServer();
    
    //语音引擎连接语音房间
    void connectVoiceRoom(const std::string& ip, unsigned int port);
    
    /// 随机进入一个房间（测试用接口）
    void randomJoinRoom();
    
    ///打开控制连接
    void openControlConnection();
    
    ///关闭控制连接
    void closeControlConnection();
    
    ///打开网关服务器连接
    void openGateWayConnection();
    
    //上传录制的语音数据
    void uploadVoiceData();
    
    //上传录制的语音数据
    void uploadVoiceData(const char *data, unsigned long datasize, unsigned int duration);
    
    //调用底层引擎播放流
    void playVoiceStream(RTChatBuffStream* instream);
    
    //录音超过最大时间回调
    void recordTimeExceed(int time);
    
    /// 请求发送漂流瓶(测试用接口)
    bool requestRandPlay(const std::string& url);
    
private:
    //刷新房间列表信息
    void refreshRoomInfoMap(const Cmd::cmdNotifyRoomList& protomsg);
    
    //初始化互斥锁
    void initMutexLock();
    
private:
    ThreadWrapper*      _workThread;        //主工作线程
    NetDataManager*     _netDataManager;    //数据管理器
    MediaSample*        _mediaSample;       //音频管理器
    
private:
    std::string         _appid;
    std::string         _appkey;
    std::string         _uniqueid;
    std::string         _token;
    std::string         _gateWayIP;       //网关服务器IP
    uint32_t            _gateWayPort;     //网关服务器PORT
    
    uint64_t            _sdkTempID;     //服务器下发的sdk唯一标识符
    
    uint64_t            _currentRoomID; //当前进入的房间ID
    
    bool                _isMute;
    
    RoomInfoMap         _roomInfoMap;   //保存房间列表
    
    SdkOpState          _sdkOpState;    //保存SDK操作状态
    
    pthread_mutex_t     _mutexLock;      //读写锁
    
    pMsgCallFunc        _func;          //回调函数
    
    RTChatBuffStream*       _buffStream;    //录音缓冲区
    RTChatBuffStreamPool*   _playBuffPool;  //播放缓冲池
    const char*             _downloadingfileurl; //临时存放下载中的文件名
    
    bool                _isrecording;       //录音标志
    time_t              _recordstarttime;   //录音开始时间
    time_t              _recordduration;    //录音持续时间
    Public*             _public;            //公共工具类
};
    
}

#endif /* defined(__RTChat__RTChatSDKMain__) */
