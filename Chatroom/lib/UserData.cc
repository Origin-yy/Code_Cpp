#include "UserData.hpp"
#include <string>

string GetNowTime(){
    time_t nowtime;
    struct tm* p;
    time(&nowtime);
    p = localtime(&nowtime);
    string time = to_string(p->tm_mon) + "-" + to_string(p->tm_mday) + "-" + 
                  to_string(p->tm_hour) + "-" + to_string(p->tm_min);

    return time;
}
void UserData::add_message_queue(int& uid){
    queue<message> *newQueue = new queue<message>;
    map_uid_queue.insert(make_pair(uid,*newQueue));
}
void UserData::add_message(int& senderUid, string msg, int uid_queue){
    message newMessage(senderUid,GetNowTime(),msg);
    map_uid_queue.find(uid_queue)->second.push(newMessage);
}