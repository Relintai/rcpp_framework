/**
 *
 *  drogon.h
 *  An Tao
 *
 *  Copyright 2018, An Tao.  All rights reserved.
 *  https://github.com/an-tao/drogon
 *  Use of this source code is governed by a MIT license
 *  that can be found in the License file.
 *
 *  Drogon
 *
 */

#pragma once

#include "core/loops/event_loop.h"
#include "core/net/inet_address.h"
#include "core/math/date.h"
#include "core/log/logger.h"

#include <http/CacheMap.h>
#include <http/Cookie.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpClient.h>
#include <drogon/IOThreadStorage.h>
#include <http/MultiPart.h>
#include <http/Session.h>
#include <http/UploadFile.h>
#include <drogon/plugins/AccessLogger.h>
#include <drogon/plugins/Plugin.h>
#include <drogon/plugins/SecureSSLRedirector.h>
#include <drogon/utils/Utilities.h>

/**
 * @mainpage
 * ### Overview
 * Drogon is a C++14/17-based HTTP application framework. Drogon can be used to
 * easily build various types of web application server programs using C++.
 */
