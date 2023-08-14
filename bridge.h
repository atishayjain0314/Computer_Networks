#ifndef BRIDGE_HPP
#define BRIDGE_HPP
#include<map>
#include<set>
#include<string>
#include<vector>

using namespace std;

class Bridge{
    private:
    //data members
    string id;
    int dist;
    string root_port,root_id,sender_id;
    set<string> designated_port,null_port;

    //member methods
    void change_root_port(string port);

    void change_root_port2(string port);

    void check_hanging_bridge();

    public:
    Bridge();

    Bridge(string bridge_id,vector<string> a);

    vector<string> create_config_msg();       

    int recieve(string port, vector<string> msg);  
    //Getter methods

    string get_root_id();

    string get_root_port();

    set<string> get_null_port();

    set<string> get_designated_port();
      
};
#endif