#ifndef MQTT_SERVER_H
#define MQTT_SERVER_H

#include <cstdio>
#include <string>
#include <vector>
#include <thread>

#include "./mqtt_broker/src/broker_session.h"
#include "./mqtt_broker/src/session_manager.h"

#include <event2/listener.h>

#include <getopt.h>

#include <csignal>
#include <cstring>

class MQTTServer {

public:
	static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *addr, int socklen, void *arg);

	void add_local_session(const std::string &filter, void (*func)(const std::string &client_id, const std::vector<uint8_t> &data, void *obj), void* obj);

	void initialize();
	void run_async();

	MQTTServer();
	~MQTTServer();

	std::thread *_thread;

	SessionManager *session_manager;
	std::string bind_address;
	uint16_t port;

	struct event_base *evloop;
	struct evconnlistener *listener;
	struct sockaddr_in sin;
};

#endif