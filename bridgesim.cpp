#include "bridgesim.h"


void Bridgesim::simulate(map<string,Bridge> &bridgemap,map<string,vector<string>> &portmap,int trace){

    int time=0;                                                 
    bool change=false;                                       //to indicate change in any bridge configurations
    queue<pair<string,vector<string>>> msg_to_process;          //{port , msg}
    set<pair<string,vector<string>>> buffer;                   // a buffer to maintain the messages between t -> t+1

    while(true){              
        change=false;

        for(auto u: bridgemap){                      //queuing  generate msg for root bridges
            if(u.first==u.second.get_root_id())
            {
                vector<string> msg=u.second.create_config_msg();
                for(auto s: u.second.get_designated_port()){
                    msg_to_process.push(make_pair(s,msg));
                    if(trace==1)
                    cout<<time<<" "<<"s "<<u.first<<" ( "<<msg[0]<<" "<<stoi(msg[1])<<" "<<msg[2]<<" ) "<<s<<"\n";
                }                                                       //added the port in the trace for reference      
            }

        }
       
        time++;     

        while(!msg_to_process.empty()){                                
            string port=msg_to_process.front().first;
            vector<string> message=msg_to_process.front().second;
            msg_to_process.pop();

            for(auto connections: portmap[port]){           //sending msg from a port to all its connnected bridges

                if(connections==message[2]) continue;       //dont send the msg to same bridge

                string ans=to_string(time)+" r "+connections+" ( "+message[0]+" "+message[1]+" "+message[2]+" ) "+port;
                int outcome=bridgemap[connections].recieve(port,message);   

                if(outcome==0||outcome==1){
                    if(trace==1)
                    cout<<ans<<"\n";                    //it should be printed only if the receiving port is not null port
                   
                    vector<string> msg= bridgemap[connections].create_config_msg();
                    set<string> dp=bridgemap[connections].get_designated_port();

                    for(auto u: dp){                    //queuing the forward msg from current bridges of designated ports
                        if(u!=port){
                        buffer.insert(make_pair(u,msg));
                        }
                    }

                    if(outcome==1){         //outcome is 1 if status of any port changes else 0 
                    change=true;
                    }
                }
            }
        }
        if(change==false)       //at steady state, there should be no change in bridge config
        break;


        //transferring msg from buffer to main queue
        for(auto it_buffer: buffer){

            Bridge sender_bridge=bridgemap[it_buffer.second[2]];

            if(sender_bridge.get_root_id()==it_buffer.second[2])        //this are the forwarded msg, hence root bridges should not send them
            continue;

            set<string> sender_null_port=sender_bridge.get_null_port();

            auto it=sender_null_port.find(it_buffer.first);
            string root=sender_bridge.get_root_port();   

            if(it==sender_null_port.end()&&(root!=it_buffer.first)){                 //transfer only if the sending port is not the null port
                msg_to_process.push(it_buffer);
                if(trace==1){
                string a=to_string(time)+" s "+ it_buffer.second[2] + " ( "+it_buffer.second[0] + " " +it_buffer.second[1]+" "+it_buffer.second[2]+" ) "+it_buffer.first;
                cout<<a<<"\n";
                }
            }
        }
        buffer.clear();
    }
}
