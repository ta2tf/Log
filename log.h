
#ifndef __ESP_LOG_H__
#define __ESP_LOG_H__

#include <stdint.h>
#include <stdarg.h>



/*
   to use this library
   -include log.h 
   -define a TAG
   -use LOGI or LOGV or LOGE or LOGD or LOGW where to need log 
   -configure parameter
     CONFIG_LOG_LEVEL                
     CONFIG_LOG_COLORS            

   USAGE:
     const char *TAG="OLA:";

	LOGI(TAG,"mola-----------I-------------------%d",10);
	LOGV(TAG,"mola-----------V-------------------");
	LOGE(TAG,"mola-----------E------------------");
	LOGD(TAG,"mola-----------D------------------");
	LOGW(TAG,"mola-----------W------------------");

	char buff[255];
	memset(buff,0,255);
	sprintf(buff,"%s","nolila");


	LOG_BUFFER_HEXDUMP("OLA",buff,64,LOG_ERROR);
	LOG_BUFFER_HEX("OLA",buff,64);
	LOG_BUFFER_CHAR("OLA",buff,64);

 OUTPUTS:

I [4] OLA:: mola-----------I-------------------10
V [9] OLA:: mola-----------V-------------------
E [14] OLA:: mola-----------E------------------
D [19] OLA:: mola-----------D------------------
W [25] OLA:: mola-----------W------------------


E [30] OLA: 0x2407fee8   6e 6f 6c 69 6c 61 00 00  00 00 00 00 00 00 00 00  |nolila..........|
E [39] OLA: 0x2407fef8   00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
E [48] OLA: 0x2407ff08   00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
E [57] OLA: 0x2407ff18   00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|

I [66] OLA: 6e 6f 6c 69 6c 61 00 00 00 00 00 00 00 00 00 00
I [72] OLA: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
I [79] OLA: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
I [85] OLA: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

I [91] OLA: nolila
I [94] OLA:
I [96] OLA:
I [98] OLA:


 */



typedef int (*vprintf_like_t)(const char *, va_list);


typedef enum {
    LOG_NONE,       /*!< No log output */
    LOG_ERROR,      /*!< Critical errors, software module can not recover on its own */
    LOG_WARN,       /*!< Error conditions from which recovery measures have been taken */
    LOG_INFO,       /*!< Information messages which describe normal flow of events */
    LOG_DEBUG,      /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    LOG_VERBOSE     /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
} log_level_t;

#define LOG_COLOR_ENABLE  1
#define LOG_COLOR_DISABLE 0

// User Definitions
#define CONFIG_LOG_LEVEL   LOG_VERBOSE             // set Log level to be outputs
#define CONFIG_LOG_COLORS  LOG_COLOR_ENABLE        // color terminal usage



/**
 * @brief Write message into the log
 *
 * This function is not intended to be used directly. Instead, use one of
 * LOGE, LOGW, LOGI, LOGD, LOGV macros.
 *
 * This function or these macros should not be used from an interrupt.
 */
 void log_write(log_level_t level, const char* tag, const char* format, ...) __attribute__ ((format (printf, 3, 4)));



/** @cond */

#if CONFIG_LOG_COLORS
#define LOG_COLOR_BLACK   "30"
#define LOG_COLOR_RED     "31"
#define LOG_COLOR_GREEN   "32"
#define LOG_COLOR_BROWN   "33"
#define LOG_COLOR_BLUE    "34"
#define LOG_COLOR_PURPLE  "35"
#define LOG_COLOR_CYAN    "36"
#define LOG_COLOR(COLOR)  "\033[0;" COLOR "m"
#define LOG_BOLD(COLOR)   "\033[1;" COLOR "m"
#define LOG_RESET_COLOR   "\033[0m"
#define LOG_COLOR_E       LOG_COLOR(LOG_COLOR_RED)
#define LOG_COLOR_W       LOG_COLOR(LOG_COLOR_BROWN)
#define LOG_COLOR_I       LOG_COLOR(LOG_COLOR_GREEN)
#define LOG_COLOR_D       LOG_COLOR(LOG_COLOR_PURPLE)
#define LOG_COLOR_V       LOG_COLOR(LOG_COLOR_BLUE)
#else //CONFIG_LOG_COLORS
#define LOG_COLOR_E
#define LOG_COLOR_W
#define LOG_COLOR_I
#define LOG_COLOR_D
#define LOG_COLOR_V
#define LOG_RESET_COLOR
#endif //CONFIG_LOG_COLORS


#define LOG_SYSTEM_TIME_FORMAT(letter, format)  LOG_COLOR_ ## letter #letter " [%s] %s: " format LOG_RESET_COLOR "\n"



#define LOG_LEVEL(level, tag, format, ...) do {                     \
        if (level==LOG_ERROR )          { log_write(LOG_ERROR,      tag, LOG_SYSTEM_TIME_FORMAT(E, format), log_system_timestamp(), tag, ##__VA_ARGS__); } \
        else if (level==LOG_WARN )      { log_write(LOG_WARN,       tag, LOG_SYSTEM_TIME_FORMAT(W, format), log_system_timestamp(), tag, ##__VA_ARGS__); } \
        else if (level==LOG_DEBUG )     { log_write(LOG_DEBUG,      tag, LOG_SYSTEM_TIME_FORMAT(D, format), log_system_timestamp(), tag, ##__VA_ARGS__); } \
        else if (level==LOG_VERBOSE )   { log_write(LOG_VERBOSE,    tag, LOG_SYSTEM_TIME_FORMAT(V, format), log_system_timestamp(), tag, ##__VA_ARGS__); } \
        else                            { log_write(LOG_INFO,       tag, LOG_SYSTEM_TIME_FORMAT(I, format), log_system_timestamp(), tag, ##__VA_ARGS__); } \
    } while(0)



/** runtime macro to output logs at a specified level. Also check the level with ``LOG_LOCAL_LEVEL``.
 *
 * @see ``printf``, ``LOG_LEVEL``
 */
#define LOG_LEVEL_LOCAL(level, tag, format, ...) do {               \
        if ( CONFIG_LOG_LEVEL >= level ) LOG_LEVEL(level, tag, format, ##__VA_ARGS__); \
    } while(0)



#define LOGE( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_ERROR,   tag, format, ##__VA_ARGS__)
#define LOGW( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_WARN,    tag, format, ##__VA_ARGS__)
#define LOGI( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_INFO,    tag, format, ##__VA_ARGS__)
#define LOGD( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_DEBUG,   tag, format, ##__VA_ARGS__)
#define LOGV( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_VERBOSE, tag, format, ##__VA_ARGS__)



/**
 * @brief Log a buffer of hex bytes at specified level, separated into 16 bytes each line.
 *
 * @param  tag      description tag
 * @param  buffer   Pointer to the buffer array
 * @param  buff_len length of buffer in bytes
 * @param  level    level of the log
 *
 */
#define LOG_BUFFER_HEX_LEVEL( tag, buffer, buff_len, level ) \
    do {\
        if ( CONFIG_LOG_LEVEL >= (level) ) { \
            log_buffer_hex_internal( tag, buffer, buff_len, level ); \
        } \
    } while(0)

/**
 * @brief Log a buffer of characters at specified level, separated into 16 bytes each line. Buffer should contain only printable characters.
 *
 * @param  tag      description tag
 * @param  buffer   Pointer to the buffer array
 * @param  buff_len length of buffer in bytes
 * @param  level    level of the log
 *
 */
#define LOG_BUFFER_CHAR_LEVEL( tag, buffer, buff_len, level ) \
    do {\
        if ( CONFIG_LOG_LEVEL >= (level) ) { \
            log_buffer_char_internal( tag, buffer, buff_len, level ); \
        } \
    } while(0)

/**
 * @brief Dump a buffer to the log at specified level.
 *
 * The dump log shows just like the one below:
 *
 *      W (195) log_example: 0x3ffb4280   45 53 50 33 32 20 69 73  20 67 72 65 61 74 2c 20  |ESP32 is great, |
 *      W (195) log_example: 0x3ffb4290   77 6f 72 6b 69 6e 67 20  61 6c 6f 6e 67 20 77 69  |working along wi|
 *      W (205) log_example: 0x3ffb42a0   74 68 20 74 68 65 20 49  44 46 2e 00              |th the IDF..|
 *
 * It is highly recommend to use terminals with over 102 text width.
 *
 * @param tag description tag
 * @param buffer Pointer to the buffer array
 * @param buff_len length of buffer in bytes
 * @param level level of the log
 */
#define LOG_BUFFER_HEXDUMP( tag, buffer, buff_len, level ) \
    do { \
        if ( CONFIG_LOG_LEVEL >= (level) ) { \
            log_buffer_hexdump_internal( tag, buffer, buff_len, level); \
        } \
    } while(0)

/**
 * @brief Log a buffer of hex bytes at Info level
 *
 * @param  tag      description tag
 * @param  buffer   Pointer to the buffer array
 * @param  buff_len length of buffer in bytes
 *
 * @see ``log_buffer_hex_level``
 *
 */
#define LOG_BUFFER_HEX(tag, buffer, buff_len) \
    do { \
        if (CONFIG_LOG_LEVEL >= LOG_INFO) { \
            LOG_BUFFER_HEX_LEVEL( tag, buffer, buff_len, LOG_INFO ); \
        }\
    } while(0)

/**
 * @brief Log a buffer of characters at Info level. Buffer should contain only printable characters.
 *
 * @param  tag      description tag
 * @param  buffer   Pointer to the buffer array
 * @param  buff_len length of buffer in bytes
 *
 * @see ``log_buffer_char_level``
 *
 */
#define LOG_BUFFER_CHAR(tag, buffer, buff_len) \
    do { \
        if (CONFIG_LOG_LEVEL >= LOG_INFO) { \
            LOG_BUFFER_CHAR_LEVEL( tag, buffer, buff_len, LOG_INFO ); \
        }\
    } while(0)

#endif /* __LOG_H__ */
