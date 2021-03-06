//
//  RTChatBuffStream.cpp
//  RTChat
//
//  Created by wang3140@hotmail.com on 14-8-26.
//  Copyright (c) 2014年 RTChatTeam. All rights reserved.
//

#include "RTChatBuffStream.h"
#include "public.h"

namespace rtchatsdk {

RTChatBuffStream::RTChatBuffStream(int maxbuffsize) :
_curWriteSize(0),
_curReadSize(0)
{
    _buffVec.resize(maxbuffsize);
}

RTChatBuffStream::~RTChatBuffStream()
{
    _buffVec.clear();
}

bool RTChatBuffStream::Write(const void *buf, int len)
{
    if (len <= _buffVec.size() - _curWriteSize) {
        bcopy(buf, &_buffVec[_curWriteSize], len);
        _curWriteSize += len;
//        Public::sdklog("当前已写入缓冲区%d字节", _curWriteSize);
        return true;
    }
    else {
        return false;
    }
}

int RTChatBuffStream::Read(void *buf, int len)
{
    if (len < (_curWriteSize - _curReadSize)) {
        bcopy(&_buffVec[_curReadSize], buf, len);
        _curReadSize += len;
        return len;
    }
    else {
        bcopy(&_buffVec[_curReadSize], buf, (_curWriteSize - _curReadSize));
        _curReadSize += (_curWriteSize - _curReadSize);
        return _curWriteSize - _curReadSize;
    }
}

//int RTChatBuffStream::Rewind()
//{
//    _curReadSize = 0;
//    _curWriteSize = 0;
//    return 0;
//}

const RTChatBuffStream::BuffVec& RTChatBuffStream::getBuffVec()
{
    return _buffVec;
}

///获得实际数据长度
int RTChatBuffStream::get_datasize()
{
    return _curWriteSize;
}

//重置BUFF等待写入
void RTChatBuffStream::resetBuffStream()
{
    _curReadSize = 0;
    _curWriteSize = 0;
}

//复位读
void RTChatBuffStream::resetReadPosition()
{
    _curReadSize = 0;
}
    
}







