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

	void on_connect(int rc) { printf("on_connect\n"); }
	void on_connect_with_flags(int rc, int flags) { printf("on_connect_with_flags\n"); }
	void on_disconnect(int rc) { printf("on_disconnect\n"); }
	void on_publish(int mid) { printf("on_publish\n"); }
	void on_message(const struct mosquitto_message *message) { printf("on_message\n"); }
	void on_subscribe(int mid, int qos_count, const int *granted_qos) { printf("on_subscribe\n"); }
	void on_unsubscribe(int mid) { printf("on_unsubscribe\n"); }
	void on_log(int level, const char *str) { printf("on_log\n"); }
	void on_error() { printf("on_error\n"); }

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