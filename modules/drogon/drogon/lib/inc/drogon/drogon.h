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

#include <trantor/net/EventLoop.h>
#include <trantor/net/InetAddress.h>
#include <trantor/utils/Date.h>
#include <trantor/utils/Logger.h>

#include <drogon/CacheMap.h>
#include <drogon/Cookie.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpClient.h>
#include <drogon/IOThreadStorage.h>
#include <drogon/MultiPart.h>
#include <drogon/Session.h>
#include <drogon/UploadFile.h>
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
