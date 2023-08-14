#include "bridge.h"

Bridge::Bridge(){

}   
Bridge::Bridge(string bridge_id,vector<string> a){
        id=bridge_id;
        for(int i=1;i<a.size();i++)
        designated_port.insert(a[i]);
        root_port="NULL";
        dist=0;
        root_id=bridge_id;
        sender_id=bridge_id;
        //null ports
}

//Getter Methods
set<string> Bridge::get_designated_port(){
    return designated_port;
}
string Bridge::get_root_id(){
    return root_id;
}
string Bridge::get_root_port(){
    return root_port;
}
set<string> Bridge::get_null_port(){
    return null_port;
}

vector<string> Bridge::create_config_msg(){         //msg 0-> root bridge, msg 1-> distance, msg 2-> sender bridge (id)
    vector<string> config_msg(3);
    config_msg[0]=root_id;
    config_msg[1]=to_string(dist);
    config_msg[2]=id;
    return config_msg;
}

void Bridge::change_root_port(string port){
    if(root_port!=port){
        if(root_port!="NULL")
        designated_port.insert(root_port);      //previous root port-> designated port
        root_port=port;
        auto it=designated_port.find(port);     //removing the new root port from designated port
        if(it!=designated_port.end())
        {
            designated_port.erase(it);
        }
    }
}
void Bridge::change_root_port2(string port){
    if(root_port!=port){
        if(root_port!="NULL")
        null_port.insert(root_port);            //previous root port-> null port
        root_port=port;
        auto it=designated_port.find(port);     //removing the new root port from dp
        if(it!=designated_port.end())
        {
            designated_port.erase(it);
        }
    }
}
void Bridge::check_hanging_bridge(){            //extra condition to avoid any hanging bridges in the final config
    if(designated_port.size()==0&&root_port!="NULL")
    {
        null_port.insert(root_port);
        root_port="NULL";
    }
}
int Bridge::recieve(string port, vector<string> msg){  //return 0 if no change, -1 if invalid else 1
    //port must not be a null port
    int change=0;
    for(auto u: designated_port){
        if(u==port)
        change=1;
    }
    if(root_port==port) change=1;
    
    if(change==0)
    return -1;

    int msg_dist=stoi(msg[1]);
    if(msg[0]<root_id){               
        dist=1+msg_dist;
        root_id=msg[0];
        sender_id=msg[2];
        
        //update root port
        change_root_port(port);
        check_hanging_bridge();
        return 1;
    }
    else if(msg[0]==root_id){          
            if(msg_dist+1<dist){            
                dist=1+msg_dist;
                sender_id=msg[2];
                change_root_port2(port);
                check_hanging_bridge();
                return 1;
            }
        else if(msg_dist+1==dist){
            if(msg[2]<sender_id){
                sender_id=msg[2];
                change_root_port2(port);
                check_hanging_bridge();
                return 1;
            }
            else if(msg[2]>=sender_id){
                auto it=designated_port.find(port);
                if(it!=designated_port.end()){
                    designated_port.erase(it);
                    null_port.insert(port);
                    check_hanging_bridge();
                    return 1;
                }
            }
            return 0;
        }
        else if(msg_dist==dist){
            if(id>msg[2]){
                auto it=designated_port.find(port);
                if(it!=designated_port.end()){
                    designated_port.erase(it);
                    null_port.insert(port);
                    check_hanging_bridge();
                    return 1;
                }
            }
        }
    }
    return 0;
}

