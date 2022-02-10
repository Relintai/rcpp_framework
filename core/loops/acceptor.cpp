
// This file is originally from Trantor - Acceptor.cc

// Copyright (c) 2016-2021, Tao An.  All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//   * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//   * Neither the name of Tao An nor the names of other contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "acceptor.h"

#ifndef O_CLOEXEC
#define O_CLOEXEC O_NOINHERIT
#endif

Acceptor::Acceptor(EventLoop *loop,
		const InetAddress &addr,
		bool reUseAddr,
		bool reUsePort)

		:
#ifndef _WIN32
		idleFd_(::open("/dev/null", O_RDONLY | O_CLOEXEC)),
#endif
		sock_(
				Socket::createNonblockingSocketOrDie(addr.getSockAddr()->sa_family)),
		addr_(addr),
		loop_(loop),
		acceptChannel_(loop, sock_.fd()) {
	sock_.setReuseAddr(reUseAddr);
	sock_.setReusePort(reUsePort);
	sock_.bindAddress(addr_);
	acceptChannel_.setReadCallback(std::bind(&Acceptor::readCallback, this));
	if (addr_.toPort() == 0) {
		addr_ = InetAddress{ Socket::getLocalAddr(sock_.fd()) };
	}
}
Acceptor::~Acceptor() {
	acceptChannel_.disableAll();
	acceptChannel_.remove();
#ifndef _WIN32
	::close(idleFd_);
#endif
}
void Acceptor::listen() {
	loop_->assertInLoopThread();
	sock_.listen();
	acceptChannel_.enableReading();
}

void Acceptor::readCallback() {
	InetAddress peer;
	int newsock = sock_.accept(&peer);
	if (newsock >= 0) {
		if (newConnectionCallback_) {
			newConnectionCallback_(newsock, peer);
		} else {
#ifndef _WIN32
			::close(newsock);
#else
			closesocket(newsock);
#endif
		}
	} else {
		LOG_SYSERR << "Accpetor::readCallback";
// Read the section named "The special problem of
// accept()ing when you can't" in libev's doc.
// By Marc Lehmann, author of libev.
/// errno is thread safe
#ifndef _WIN32
		if (errno == EMFILE) {
			::close(idleFd_);
			idleFd_ = sock_.accept(&peer);
			::close(idleFd_);
			idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
		}
#endif
	}
}
