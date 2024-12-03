/**
 * POGOBOT
 *
 * Copyright © 2022 Sorbonne Université ISIR
 * This file is licensed under the Expat License, sometimes known as the MIT License.
 * Please refer to file LICENCE for details.
**/


/** \file
Pogobot demo short header message.
*/

/* clang-format-ok */

#include <stdio.h>
#include <string.h>
#include "pogobot.h"


#define SIZE_TEST 10

uint8_t message[34] = "robot A abcdefghijklmnopqrstuvwyz";
uint8_t messageAll[38] = "robot A all abcdefghijklmnopqrstuvwyz";
uint8_t messageAllId[41] = "robot A all ID abcdefghijklmnopqrstuvwyz";

time_reference_t mytimer;

void echo_cancel_disable( void );
void echo_cancel_disable( void )
{
  for ( int ir_i = 0; ir_i < IR_RX_COUNT; ir_i++ )
  {
      uint32_t value = IRn_rx_conf_read( ir_i );
      value = ir_rx0_conf_echo_cancel_replace( value, 0 );
      IRn_rx_conf_write( ir_i, value );
  }
}

int main(void) {

    pogobot_init();

    // echo cancel
    echo_cancel_disable();

    printf("init ok\n");

    pogobot_led_setColor( 25, 25, 25 );

    while (1)
    {
        pogobot_infrared_update();

        static int counter = 0;
        static int ir_emitter = 0;

        if ( counter < SIZE_TEST )
        {
          printf( "TRANS one dir: %d, %d ", ir_emitter, sizeof(message) );
          printf( "%s\n", message);
          pogobot_infrared_sendLongMessage_uniSpe( ir_emitter, message, sizeof(message) );
        
          printf( "TRANS all without id: %d ", sizeof(messageAll) );
          printf( "%s\n", messageAll);
          pogobot_infrared_sendLongMessage_omniGen( messageAll, sizeof(messageAll) );

          printf( "TRANS all with id: %d ", sizeof(messageAllId) );
          printf( "%s\n", messageAllId);
          pogobot_infrared_sendLongMessage_omniSpe( messageAllId, sizeof(messageAllId) );

          counter = counter + 1;
        }

        /* read reception fifo buffer */
        if ( pogobot_infrared_message_available() )
        {
            while ( pogobot_infrared_message_available() )
            {
                message_t mr;
                pogobot_infrared_recover_next_message( &mr );

                printf( "RECV: receiver on ir %d, sender %d on ir %d ",
                        mr.header._receiver_ir_index,
                        mr.header._sender_id, mr.header._sender_ir_index );
                printf( "RECV: len %d [%s]\n", mr.header.payload_length,
                        mr.payload );
            }
        }
        

        /*if ( counter == SIZE_TEST )
        {
            uint32_t time_microseconds =
                pogobot_stopwatch_get_elapsed_microseconds( &mytimer );
            printf( "\nSent %u %u bytes in %lu microseconds.\n", SIZE_TEST,
                    message_length_bytes, time_microseconds );
            uint64_t numerator =
                ( (uint64_t)SIZE_TEST * message_length_bytes * 1000000L );
            uint64_t denominator = ( time_microseconds );
            uint32_t ir_speed = numerator / denominator;
            printf( "IR comm speed:  %luB/s\n", ir_speed );

            // end of the test
            while ( 1 )
            {
            }
        }
        else
        {
            counter++;
        }*/

        if ( counter == SIZE_TEST )
        {
            static int once = 1;
            slip_error_counter_s e_counter;

            if (once == 1)
            {
                for (size_t i = 0; i < 4; i++)
                {
                    pogobot_infrared_get_receiver_error_counter(&e_counter, i);
                    printf(" IR %d, crc mismatch : %ld\n", i, e_counter.crc_mismatch_counter);
                }

                once = 0;
            }


        }

        ir_emitter = ( ir_emitter + 1 ) % IR_RX_COUNT;
    }

}
