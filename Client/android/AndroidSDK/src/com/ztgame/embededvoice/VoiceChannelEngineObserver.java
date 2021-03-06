package com.ztgame.embededvoice;

import java.util.List;

import Cmd.Game.cmdNotifyAddVoiceUser;
import Cmd.Game.cmdNotifyCreateResult;
import Cmd.Game.cmdNotifyDelVoiceUser;
import Cmd.Game.cmdNotifyLoginResult;
import Cmd.Game.cmdNotifyTakeMic;
import Cmd.Public.enRoomType;
import Cmd.Public.stMicInfo;
import Cmd.Public.stRoomInfo;
import Cmd.Public.stRoomUserInfo;
import Cmd.Public.stVoiceUserInfo;

public interface VoiceChannelEngineObserver 
{
  void OnLoginResult(cmdNotifyLoginResult loginresult);
  void OnCreateResult(cmdNotifyCreateResult createresult);
  void OnEnterRoomNotify(long roomid,enRoomType roomtype);
  void OnAddVoiceUser(List<stVoiceUserInfo> voiceuser);
  void OnDelVoiceUser(List<stVoiceUserInfo> voiceuser);
  void OnRoomListNotify(List<stRoomInfo> roomlist);
  void OnMicQueueNotify(List<stMicInfo> micinfolists);
  void OnMicTakeNotify(cmdNotifyTakeMic micinfo);
  void OnRoomUsersEnter(List<stRoomUserInfo> roomUsersList,boolean isAll);
  void OnRoomUserLeave(long leaveRoomUser);
  void OnRandChatInvite(long tempid,long roomid,String uniqueid);
  //¼����֪ͨ
  void OnStopVoiceRecording(long size);
  void OnUploadVoiceEnd(String cloudvoiceurl,int duration);
  void OnDlVoiceBegin();
  void OnDlVoiceEnd(boolean isOk,long size);
  void OnRandomRecording(String url);
  
 //mic power 
  void OnResultUpPwder(int result);
  void OnNotifyUpdatePower(long tempid,int power);
  void OnNotifyAssignRet(int ret);
}
