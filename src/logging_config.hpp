/* 
 * File:   logging_config.hpp
 * Author: alex
 *
 * Created on May 10, 2016, 5:12 PM
 */

#ifndef WILTON_LOGGING_LOGGING_CONFIG_HPP
#define WILTON_LOGGING_LOGGING_CONFIG_HPP

#include <string>
#include <vector>

#include "staticlib/ranges.hpp"
#include "staticlib/json.hpp"

#include "wilton/support/exception.hpp"

#include "appender_config.hpp"
#include "logger_config.hpp"

namespace wilton {
namespace logging {

class logging_config {
public:
    std::vector<appender_config> appenders;
    std::vector<logger_config> loggers;

    logging_config(const logging_config&) = delete;

    logging_config& operator=(const logging_config&) = delete;

    logging_config(logging_config&& other) :
    appenders(std::move(other.appenders)),
    loggers(std::move(other.loggers)) { }

    logging_config& operator=(logging_config&& other) {
        this->appenders = std::move(other.appenders);
        this->loggers = std::move(other.loggers);
        return *this;
    }

    logging_config() { }

    logging_config(const sl::json::value& json) {
        for (const sl::json::field& fi : json.as_object()) {
            auto& name = fi.name();
            if ("appenders" == name) {
                for (const sl::json::value& ap : fi.as_array_or_throw(name)) {
                    auto ja = appender_config(ap);
                    appenders.emplace_back(std::move(ja));
                }
            } else if ("loggers" == name) {
                if (sl::json::type::array == fi.json_type()) {
                    for (const sl::json::value& lo : fi.as_array()) {
                        auto jl = logger_config(lo);
                        loggers.emplace_back(std::move(jl));
                    }
                } else if (sl::json::type::object == fi.json_type()) {
                    for (const sl::json::field& lo : fi.as_object()) {
                        auto jl = logger_config(lo);
                        loggers.emplace_back(std::move(jl));
                    }
                } else {
                    throw support::exception(TRACEMSG("Invalid 'logging.loggers' value," +
                            " type: [" + sl::json::stringify_json_type(fi.json_type()) + "]"));
                }
            } else {
                throw support::exception(TRACEMSG("Unknown 'logging' field: [" + name + "]"));
            }
        }
    }

    sl::json::value to_json() const {
        return {
            {"appenders", [this](){
                auto ra = sl::ranges::transform(appenders, [](const appender_config & el) {
                    return el.to_json();
                });
                return ra.to_vector();
            }()},
            {"loggers", [this](){
                auto ra = sl::ranges::transform(loggers, [](const logger_config & el) {
                    return el.to_json();
                });
                return ra.to_vector();
            }()}
        };
    }
};

} // namespace
}

#endif /* WILTON_LOGGING_LOGGING_CONFIG_HPP */

