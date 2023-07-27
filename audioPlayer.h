
#ifndef DRVL_AUDIO_PLAYER_H
#define	DRVL_AUDIO_PLAYER_H

#include "I2S.h"

#ifdef __cplusplus
extern "C" {
#endif

#define     BULK_SIZE           (0xFFFC * 2)
#define     HALF_BULK_SIZE      (64)
//#define     AUDIO_FREQ          44100UL
#define     AUDIO_FREQ          32000UL
#define     AUDIO_BUFFER_SIZE   (1024 * 32)

#define     I2S_EVENTS          (I2S_EVENT_TX_COMPLETE /*| I2S_EVENT_TX_HALF_COMPLETE */)

typedef enum
{
	HALF_EMPTY = 0,
	FULL_EMPTY
} bufferemptiness_msg_type_e;

typedef struct {
	bufferemptiness_msg_type_e msgType;
} audiobuffer_msg_mail_t;


int AudioPlayer_Init( void );
void AudioPlayer_SetConfig(int bulk_size_in_bytes, int audio_freq, bool repeated);
int AudioPlayer_Start(uint16_t* bufptr, int total_size_in_bytes);


#ifdef __cplusplus
}
#endif

#endif