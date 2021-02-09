#pragma once
#include <string>
#include "proton/variant.hpp"
#include "enet/include/enet.h"
#include "world.h"

class server {

   private:
    ENetHost* m_proxy_server;
    ENetHost* m_real_server;
    ENetPeer* m_server_peer;
  
    
    void handle_outgoing();
    void handle_incoming();
    bool connect();
    void disconnect(bool reset);
   public:
       ENetPeer* m_gt_peer;

       struct Item {
           uint16_t id;
           uint8_t count;
           uint8_t type;
       };
       static std::vector<server::Item> inventory;

       void disconnectsr(bool reset);

    int m_user = 0;
    int m_token = 0;
    std::string m_server = "213.179.209.168";
    int m_port = 17198;
    std::string serverz = m_server;
    int portz = m_port;
    int m_proxyport = 17191;
    std::string ipserver = "127.0.0.1";
    std::string create = "0.0.0.0";

    world m_world;
    bool start();
    void quit();
    bool setup_client();
    void redirect_server(variantlist_t& varlist);
    void send(bool client, int32_t type, uint8_t* data, int32_t len);
    void send(bool client, variantlist_t& list, int32_t netid = -1, int32_t delay = 0);
    void send(bool client, std::string packet, int32_t type = 2);
    void SendPacketRaw(int a1, void* packetData, size_t packetDataSize, void* a4, ENetPeer* peer, int packetFlag);

    void poll();
};
extern server* g_server;

