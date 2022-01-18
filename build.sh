g++ -std=c++17 -Ofast -pthread -o proxy_linux proxy/Naga/callbacks.c proxy/Naga/compress.c proxy/Naga/host.c proxy/Naga/list.c proxy/Naga/packet.c proxy/Naga/peer.c proxy/Naga/protocol.c proxy/Naga/unix.c proxy/Naga/win32.c proxy/events.cpp proxy/gt.cpp proxy/http.cpp proxy/proxy.cpp proxy/sandbird/sandbird.c proxy/server.cpp proxy/utils.cpp

