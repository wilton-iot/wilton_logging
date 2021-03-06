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
 * File:   logger_config.hpp
 * Author: alex
 *
 * Created on May 10, 2016, 8:01 PM
 */

#ifndef WILTON_LOGGING_LOGGER_CONFIG_HPP
#define WILTON_LOGGING_LOGGER_CONFIG_HPP

#include <string>

#include "staticlib/config.hpp"
#include "staticlib/json.hpp"

#include "wilton/support/exception.hpp"

namespace wilton {
namespace logging {

class logger_config {
public:
    std::string name = "";
    std::string level = "";

    logger_config(const logger_config&) = delete;

    logger_config& operator=(const logger_config&) = delete;

    logger_config(logger_config&& other) :
    name(std::move(other.name)),
    level(std::move(other.level)) { }

    logger_config& operator=(logger_config&& other) {
        this->name = std::move(other.name);
        this->level = std::move(other.level);
        return *this;
    }

    logger_config() { }

    logger_config(const sl::json::value& json) {
        for (const sl::json::field& fi : json.as_object()) {
            auto& fname = fi.name();
            if ("name" == fname) {
                this->name = fi.as_string_nonempty_or_throw(name);
            } else if ("level" == fname) {
                this->level = fi.as_string_nonempty_or_throw(name);
            } else {
                throw support::exception(TRACEMSG("Unknown 'logging.loggers' field: [" + fname + "]"));
            }
        }
        if (0 == name.length()) throw support::exception(TRACEMSG(
                "Invalid 'logging.loggers.name' field: []"));
        if (0 == level.length()) throw support::exception(TRACEMSG(
                "Invalid 'logging.loggers.level' field: []"));
    }
    
    logger_config(const sl::json::field& fi) {
        this->name = fi.name();
        if (0 == name.length()) throw support::exception(TRACEMSG(
                "Invalid 'logging.loggers.name' field: []"));
        this->level = fi.as_string_nonempty_or_throw(this->name);
    }

    sl::json::value to_json() const {
        return {
            {"name", name},
            {"level", level}
        };
    }
};

} // namepspace
}

#endif /* WILTON_LOGGING_LOGGER_CONFIG_HPP */

