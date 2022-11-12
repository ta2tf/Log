

#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


#include "log.h"


static vprintf_like_t s_log_print_func = &vprintf;



char* log_system_timestamp(void)
{
	static char ticktxt[16];
	uint32_t tick;

	memset(ticktxt,0,16);
	tick = HAL_GetTick();
	sprintf(ticktxt,"%d",tick);
	return ticktxt;
}




void log_writev(log_level_t level,
                   const char *tag,
                   const char *format,
                   va_list args)
{

    (*s_log_print_func)(format, args);
}


void log_write(log_level_t level,
                   const char *tag,
                   const char *format, ...)
{
    va_list list;
    va_start(list, format);
    log_writev(level, tag, format, list);
    va_end(list);
}


//print number of bytes per line for log_buffer_char and log_buffer_hex
#define BYTES_PER_LINE 16

void log_buffer_hex_internal(const char *tag, const void *buffer, uint16_t buff_len,
                                 log_level_t log_level)
{
    if (buff_len == 0) {
        return;
    }
    char temp_buffer[BYTES_PER_LINE + 3]; //for not-byte-accessible memory
    char hex_buffer[3 * BYTES_PER_LINE + 1];
    const char *ptr_line;
    int bytes_cur_line;

    do {
        if (buff_len > BYTES_PER_LINE) {
            bytes_cur_line = BYTES_PER_LINE;
        } else {
            bytes_cur_line = buff_len;
        }

            ptr_line = buffer;


        for (int i = 0; i < bytes_cur_line; i ++) {
            sprintf(hex_buffer + 3 * i, "%02x ", ptr_line[i]);
        }
        LOG_LEVEL(log_level, tag, "%s", hex_buffer);
        buffer += bytes_cur_line;
        buff_len -= bytes_cur_line;
    } while (buff_len);
}

void log_buffer_char_internal(const char *tag, const void *buffer, uint16_t buff_len,
                                  log_level_t log_level)
{
    if (buff_len == 0) {
        return;
    }
    char temp_buffer[BYTES_PER_LINE + 3]; //for not-byte-accessible memory
    char char_buffer[BYTES_PER_LINE + 1];
    const char *ptr_line;
    int bytes_cur_line;

    do {
        if (buff_len > BYTES_PER_LINE) {
            bytes_cur_line = BYTES_PER_LINE;
        } else {
            bytes_cur_line = buff_len;
        }

            ptr_line = buffer;


        for (int i = 0; i < bytes_cur_line; i ++) {
            sprintf(char_buffer + i, "%c", ptr_line[i]);
        }
        LOG_LEVEL(log_level, tag, "%s", char_buffer);
        buffer += bytes_cur_line;
        buff_len -= bytes_cur_line;
    } while (buff_len);
}

void log_buffer_hexdump_internal(const char *tag, const void *buffer, uint16_t buff_len, log_level_t log_level)
{

    if (buff_len == 0) {
        return;
    }
    char temp_buffer[BYTES_PER_LINE + 3]; //for not-byte-accessible memory
    const char *ptr_line;
    //format: field[length]
    // ADDR[10]+"   "+DATA_HEX[8*3]+" "+DATA_HEX[8*3]+"  |"+DATA_CHAR[8]+"|"
    char hd_buffer[10 + 3 + BYTES_PER_LINE * 3 + 3 + BYTES_PER_LINE + 1 + 1];
    char *ptr_hd;
    int bytes_cur_line;

    do {
        if (buff_len > BYTES_PER_LINE) {
            bytes_cur_line = BYTES_PER_LINE;
        } else {
            bytes_cur_line = buff_len;
        }

            ptr_line = buffer;

        ptr_hd = hd_buffer;

        ptr_hd += sprintf(ptr_hd, "%p ", buffer);
        for (int i = 0; i < BYTES_PER_LINE; i ++) {
            if ((i & 7) == 0) {
                ptr_hd += sprintf(ptr_hd, " ");
            }
            if (i < bytes_cur_line) {
                ptr_hd += sprintf(ptr_hd, " %02x", ptr_line[i]);
            } else {
                ptr_hd += sprintf(ptr_hd, "   ");
            }
        }
        ptr_hd += sprintf(ptr_hd, "  |");
        for (int i = 0; i < bytes_cur_line; i ++) {
            if (isprint((int)ptr_line[i])) {
                ptr_hd += sprintf(ptr_hd, "%c", ptr_line[i]);
            } else {
                ptr_hd += sprintf(ptr_hd, ".");
            }
        }
        ptr_hd += sprintf(ptr_hd, "|");

        LOG_LEVEL(log_level, tag, "%s", hd_buffer);
        buffer += bytes_cur_line;
        buff_len -= bytes_cur_line;
    } while (buff_len);
}
