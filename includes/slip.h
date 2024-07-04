/**
 * POGOBOT
 *
 * Copyright © 2022 Sorbonne Université ISIR
 * This file is licensed under the Expat License, sometimes known as the MIT License.
 * Please refer to file LICENCE for details.
**/


/*
The MIT License (MIT)

Copyright (c) 2017 Marcin Borowicz <marcinbor85@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 */

/* clang-format-ok */

#ifndef SLIP_H
#define SLIP_H

#include <stdint.h>

//#define DEBUG_SLIP 1

#define SLIP_SPECIAL_BYTE_END 0xC0
#define SLIP_SPECIAL_BYTE_ESC 0xDB

#define SLIP_ESCAPED_BYTE_END 0xDC
#define SLIP_ESCAPED_BYTE_ESC 0xDD

typedef enum
{
    SLIP_STATE_NORMAL = 0x00,
    SLIP_STATE_ESCAPED
} slip_state_t;

typedef enum
{
    SLIP_NO_ERROR = 0x00,
    SLIP_ERROR_BUFFER_OVERFLOW,
    SLIP_ERROR_UNKNOWN_ESCAPED_BYTE,
    SLIP_ERROR_CRC_MISMATCH
} slip_error_t;

typedef struct 
{
    uint32_t overflow_counter;
    uint32_t unknown_escaped_byte_counter;
    uint32_t crc_mismatch_counter;
} slip_error_counter_s;

typedef struct
{
    uint32_t crc_seed;
    uint8_t ( *write_byte )( uint8_t byte );
} slip_send_descriptor_s;

typedef struct
{
    slip_state_t state;
    uint32_t size;
    uint8_t *buf;
    uint32_t buf_size;
    uint32_t crc;
    void *tag;
    void ( *recv_message )( uint8_t *data, uint32_t size, void *tag );
    uint32_t crc_seed;
    slip_error_counter_s error_counter;
} slip_receive_state_s;

slip_error_t
slip_send_init( const slip_send_descriptor_s *const slip_send_descriptor );

slip_error_t
slip_receive_init( slip_receive_state_s *const slip_receive_state );

slip_error_t slip_decode_received_byte( slip_receive_state_s *slip,
                                        uint8_t byte );
slip_error_t slip_send_message( const slip_send_descriptor_s *const slipdesc,
                                uint8_t *data, uint32_t size );

void slip_dump_state( slip_receive_state_s *slip, char *msg );

void slip_get_error_counter( slip_receive_state_s *slip, slip_error_counter_s *counter_struct );

void slip_reset_error_counter( slip_receive_state_s *slip );

#endif /* SLIP_H */
