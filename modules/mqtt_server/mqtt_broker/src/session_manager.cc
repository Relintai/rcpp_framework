/**
 * @file session_manager.cc
 */

#include "session_manager.h"
#include "broker_session.h"
#include "topic.h"

#include <algorithm>
#include <memory>

void SessionManager::accept_connection(struct bufferevent *bev) {

	auto session = std::unique_ptr<BrokerSession>(new BrokerSession(bev, *this));
	sessions.push_back(std::move(session));
}

std::list<std::unique_ptr<BrokerSession> >::iterator SessionManager::find_session(const std::string &client_id) {

	return find_if(sessions.begin(), sessions.end(), [&client_id](const std::unique_ptr<BrokerSession> &s) {
		return (!s->client_id.empty() and (s->client_id == client_id));
	});
}

void SessionManager::erase_session(const std::string &client_id) {
	sessions.erase(std::remove_if(sessions.begin(), sessions.end(), [&client_id](std::unique_ptr<BrokerSession> &s) {
		return (!s->client_id.empty() and (s->client_id == client_id));
	}),
			sessions.end());
}

void SessionManager::erase_session(const BrokerSession *session) {
	sessions.erase(std::remove_if(sessions.begin(), sessions.end(), [session](std::unique_ptr<BrokerSession> &s) {
		return s.get() == session;
	}),
			sessions.end());
}

void SessionManager::handle_publish(const PublishPacket &packet) {
	for (auto &session : sessions) {
		for (auto &subscription : session->subscriptions) {
			if (topic_match(subscription.topic_filter, TopicName(packet.topic_name))) {
				session->forward_packet(packet);
			}
		}
	}
}

void SessionManager::handle_local_publish(const std::string &client_id, const PublishPacket &packet) {
	for (size_t i = 0; i < local_sessions.size(); ++i) {
        LocalSession &l = local_sessions[i];

		if (topic_match(l.filter, TopicName(packet.topic_name))) {
			l.func(client_id, packet.message_data, l.obj);
		}
	}
}