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
 * File:   wilton_logger.hpp
 * Author: alex
 *
 * Created on May 10, 2016, 4:49 PM
 */

#ifndef WILTON_LOGGING_WILTON_LOGGER_HPP
#define WILTON_LOGGING_WILTON_LOGGER_HPP

#include <string>

#include "staticlib/pimpl.hpp"

#include "wilton/support/exception.hpp"

#include "logging_config.hpp"

namespace wilton {
namespace logging {

class wilton_logger : public sl::pimpl::object {
protected:
    /**
     * implementation class
     */
    class impl;
public:
    /**
     * PIMPL-specific constructor
     * 
     * @param pimpl impl object
     */
    PIMPL_CONSTRUCTOR(wilton_logger)

    static void log(const std::string& level_name, const std::string& logger_name, const std::string& message);
    
    static void apply_config(const logging_config& config);
    
    static bool is_enabled_for_level(const std::string& logger_name, const std::string& level_name);
    
    static void shutdown();
};

} // namespace
}

#endif /* WILTON_LOGGING_WILTON_LOGGER_HPP */
