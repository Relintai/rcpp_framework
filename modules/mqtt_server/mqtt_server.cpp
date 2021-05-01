#include "mqtt_server.h"

void MQTTServer::initialize() {
	evloop = event_base_new();

	if (!evloop) {
		printf("Could not initialize libevent\n");
		return;
	}

	std::memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	evutil_inet_pton(sin.sin_family, bind_address.c_str(), &sin.sin_addr);
	sin.sin_port = htons(port);

	listener = evconnlistener_new_bind(evloop, MQTTServer::listener_cb,
                    (void *)this,
                    LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
                    (struct sockaddr *)&sin, sizeof(sin));

	if (!listener) {
		std::cerr << "Could not create listener!\n";
		return;
	}
}

void MQTTServer::listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data) {

	MQTTServer *server = static_cast<MQTTServer *>(user_data);

	struct bufferevent *bev;

	bev = bufferevent_socket_new(server->evloop, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev) {
		std::cerr << "Error constructing bufferevent!\n";
		event_base_loopbreak(server->evloop);
		return;
	}

	server->session_manager->accept_connection(bev);
}

void MQTTServer::loop_once() {
	event_base_dispatch(evloop);
}

MQTTServer::MQTTServer() {
	bind_address = "0";
	port = 1883;

    session_manager = new SessionManager();

	evloop = nullptr;
	listener = nullptr;
}

MQTTServer::~MQTTServer() {
    if (event_base_loopexit(evloop, NULL)) {
        std::cerr << "failed to exit event loop\n";
    }

	evconnlistener_free(listener);
	event_base_free(evloop);

    delete session_manager;
}
