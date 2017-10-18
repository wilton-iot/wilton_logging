/* 
 * File:   wilton_db.h
 * Author: alex
 *
 * Created on June 10, 2017, 1:23 PM
 */

#ifndef WILTON_LOGGING_H
#define WILTON_LOGGING_H

#include "wilton/wilton.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
    "logging": {
        "appenders": [{
            "appenderType": "NULL | CONSOLE | FILE | DAILY_ROLLING_FILE",
            "filePath": "path/to/log/file",
            "layout": "%d{%Y-%m-%d %H:%M:%S,%q} [%-5p %-5.5t %-20.20c] %m%n",
            "thresholdLevel": "TRACE | DEBUG | INFO | WARN | ERROR | FATAL"
        }, ... ],
        "loggers": [{
            "name": "my.logger.name",
            "level": "TRACE | DEBUG | INFO | WARN | ERROR | FATAL"
        }, ...]
    }
 */
char* wilton_logger_initialize(
        const char* conf_json,
        int conf_json_len);

char* wilton_logger_log(
        const char* level_name,
        int level_name_len,
        const char* logger_name,
        int logger_name_len,
        const char* message,
        int message_len);

char* wilton_logger_is_level_enabled(
        const char* logger_name,
        int logger_name_len,
        const char* level_name,
        int level_name_len,
        int* res_out
);

char* wilton_logger_shutdown();


#ifdef __cplusplus
}
#endif

#endif /* WILTON_LOGGING_H */

