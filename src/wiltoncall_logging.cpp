/*
 * Copyright 2017, alex at staticlibs.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * File:   wiltoncall_logging.cpp
 * Author: alex
 *
 * Created on January 10, 2017, 6:01 PM
 */

#include <cstdint>
#include <string>

#include "staticlib/io.hpp"
#include "staticlib/json.hpp"
#include "staticlib/support.hpp"
#include "staticlib/utils.hpp"

#include "wilton/wilton_logging.h"

#include "wilton/support/buffer.hpp"
#include "wilton/support/exception.hpp"
#include "wilton/support/registrar.hpp"

namespace wilton {
namespace logging {

support::buffer initialize(sl::io::span<const char> data) {
    char* err = wilton_logger_initialize(data.data(), data.size_int());
    if (nullptr != err) support::throw_wilton_error(err, TRACEMSG(std::string(err)));
    return support::make_null_buffer();
}

support::buffer log(sl::io::span<const char> data) {
    // json parse
    auto json = sl::json::load(data);
    auto rlevel = std::ref(sl::utils::empty_string());
    auto rlogger = std::ref(sl::utils::empty_string());
    auto rmessage = std::ref(sl::utils::empty_string());
    for (const sl::json::field& fi : json.as_object()) {
        auto& name = fi.name();
        if ("level" == name) {
            rlevel = fi.as_string_nonempty_or_throw(name);
        } else if ("logger" == name) {
            rlogger = fi.as_string_nonempty_or_throw(name);
        } else if ("message" == name) {
            rmessage = fi.as_string();
        } else {
            throw support::exception(TRACEMSG("Unknown data field: [" + name + "]"));
        }
    }
    if (rlevel.get().empty()) throw support::exception(TRACEMSG(
            "Required parameter 'level' not specified"));
    if (rlogger.get().empty()) throw support::exception(TRACEMSG(
            "Required parameter 'logger' not specified"));
    const std::string& level = rlevel.get();
    const std::string& logger = rlogger.get();
    const std::string& message = rmessage.get();
    // call wilton
    char* err = wilton_logger_log(level.c_str(), static_cast<int>(level.length()),
            logger.c_str(), static_cast<int>(logger.length()),
            message.c_str(), static_cast<int>(message.length()));
    if (nullptr != err) support::throw_wilton_error(err, TRACEMSG(std::string(err)));
    return support::make_null_buffer();
}

support::buffer is_level_enabled(sl::io::span<const char> data) {
    // parse json
    auto json = sl::json::load(data);
    auto rlevel = std::ref(sl::utils::empty_string());
    auto rlogger = std::ref(sl::utils::empty_string());
    for (const sl::json::field& fi : json.as_object()) {
        auto& name = fi.name();
        if ("level" == name) {
            rlevel = fi.as_string_nonempty_or_throw(name);
        } else if ("logger" == name) {
            rlogger = fi.as_string_nonempty_or_throw(name);
        } else {
            throw support::exception(TRACEMSG("Unknown data field: [" + name + "]"));
        }
    }
    if (rlevel.get().empty()) throw support::exception(TRACEMSG(
            "Required parameter 'level' not specified"));
    if (rlogger.get().empty()) throw support::exception(TRACEMSG(
            "Required parameter 'logger' not specified"));
    const std::string& level = rlevel.get();
    const std::string& logger = rlogger.get();
    // call wilton
    int out = 0;
    char* err = wilton_logger_is_level_enabled(logger.c_str(), static_cast<int>(logger.length()),
            level.c_str(), static_cast<int>(level.length()), std::addressof(out));
    if (nullptr != err) support::throw_wilton_error(err, TRACEMSG(std::string(err)));
    return support::make_json_buffer({
        { "enabled", out != 0 }
    });
}

support::buffer shutdown(sl::io::span<const char>) {
    // call wilton
    char* err = wilton_logger_shutdown();
    if (nullptr != err) support::throw_wilton_error(err, TRACEMSG(std::string(err)));
    return support::make_null_buffer();
}

} // namespace
}

extern "C" char* wilton_module_init() {
    try {
        wilton::support::register_wiltoncall("logging_initialize", wilton::logging::initialize);
        wilton::support::register_wiltoncall("logging_log", wilton::logging::log);
        wilton::support::register_wiltoncall("logging_is_level_enabled", wilton::logging::is_level_enabled);
        wilton::support::register_wiltoncall("logging_shutdown", wilton::logging::shutdown);
        return nullptr;
    } catch (const std::exception& e) {
        return wilton::support::alloc_copy(TRACEMSG(e.what() + "\nException raised"));
    }
}
