#include "bridge.h"
#include "bridgesim.h"
#include<set>
#include<string>

using namespace std;

vector<string> process_string(string s){
    vector<string> a;
    size_t pos=s.find(": ");

    a.push_back(s.substr(0,pos));           //getting Bridge_Id
    s.erase(0,pos+2);                       //removing the Bridge ID

    int l=s.length();
    if(s[l-1]=='\n')    s.erase(l-1,1);

    for(int i=0;i<l;i++){
        if(s[i]!=' '&&s[i]!='\n'){
            a.push_back(s.substr(i,1));         //adding each port to vector 'a'
        }
    }
    return a;
}

map<string,Bridge> bridgemap;              //Bridge_id -> Bridge structure
map<string,vector<string>> portmap;        //Port  -> Bridge_id (Connected bridges to that port)

int main(){
    int trace,total_bridges;

    cin>>trace>>total_bridges;
    cin.ignore();               //accounting for extra "\n" because of cin

    for(int i=1;i<=total_bridges;i++){
        string s;
        getline(cin,s);

        vector<string> bridge_port= process_string(s);

        string bridge_id=bridge_port[0];
        Bridge bridge= Bridge(bridge_id,bridge_port);   //constructor of Bridge Class

        bridgemap[bridge_id]=bridge;

        for(int i=1;i<bridge_port.size();i++){
            if(portmap[bridge_port[i]].size()!=0)
                portmap[bridge_port[i]].push_back(bridge_id);
            else
                portmap[bridge_port[i]]={bridge_id};
        }
    }

    Bridgesim B1;
    B1.simulate(bridgemap,portmap,trace);       //simulating STP

    for(auto u: bridgemap){                     //printing final config of the bridge network

        set<string> ans;
        set<string> dp=u.second.get_designated_port();
        set<string> np=u.second.get_null_port();

        cout<<u.first<<": ";                    //Bridge_ID
        string root=u.second.get_root_port();

        if(root!="NULL")
        ans.insert(root+"-RP");

        for(auto it_dp: dp){
            ans.insert(it_dp+"-DP");
        }
        for(auto it_np: np){
            ans.insert(it_np+"-NP");
        }
        for(auto s: ans){
            cout<<s<<" ";
        }
        cout<<"\n";
    }
    return 0;
}