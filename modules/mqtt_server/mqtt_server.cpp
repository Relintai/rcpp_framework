#include "mqtt_server.h"

void MQTTServer::add_local_session(const std::string &filter, void (*func)(const std::string &client_id, const std::vector<uint8_t> &data, void *obj), void* obj) {
    session_manager->add_local_session(filter, func, obj);
}

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

void MQTTServer::run_async() {
    if (_thread) {
        printf("MQTTServer::run_async Error! A thread is already runnig!\n");
        return;
    }

    _thread = new std::thread([this]() { event_base_dispatch(this->evloop); });
}

MQTTServer::MQTTServer() {
	bind_address = "0";
	port = 1883;
	_thread = nullptr;

	session_manager = new SessionManager();

	evloop = nullptr;
	listener = nullptr;
}

MQTTServer::~MQTTServer() {
    //this first, as evloop runs in _thread
	if (evloop && event_base_loopexit(evloop, NULL)) {
		std::cout << "failed to exit event loop\n";
	}

	if (_thread) {
		_thread->join();
		delete _thread;
	}

	if (listener)
		evconnlistener_free(listener);

	if (evloop)
		event_base_free(evloop);

	delete session_manager;
}
