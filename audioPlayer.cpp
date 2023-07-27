#include "mbed.h"
#include "audioPlayer.h"


I2S i2s2(PC_3, PB_13, PB_12);

Mail<audiobuffer_msg_mail_t, 4> audio_msg_mail_box;


typedef struct {
    uint16_t* pbuf;
    int bulk_size_bytes;
    int audio_freq;
    int cur_buf_idx_words;
    int total_size_bytes;
    bool repeated;
} audio_info_struct_t;

static audio_info_struct_t _audio_info;
static Thread AudioPlayer_Thread;

static void i2s_callback(int narg);
void AudioPlayerManager(void);

/// @brief Initialize Audio Player
/// @param  
/// @return 
int AudioPlayer_Init( void )
{
    int ret = -1;

    memset((char*)&_audio_info, 0, sizeof(audio_info_struct_t));

    i2s2.abort_all_transfers();
    i2s2.clear_transfer_buffer();

    ret = (int)AudioPlayer_Thread.start(callback(AudioPlayerManager));

    return ret;
}

/// @brief 
/// @param bulk_size_in_bytes 
/// @param audio_freq 
/// @param repeated 
void AudioPlayer_SetConfig(int bulk_size_in_bytes, int audio_freq, bool repeated)
{
    _audio_info.bulk_size_bytes = bulk_size_in_bytes;
    _audio_info.audio_freq = audio_freq;
    _audio_info.repeated = repeated;
}

/// @brief start audio-player
/// @param bufptr 
/// @param total_size_in_bytes 
/// @return number of data to be sent to I2S buffer. -1 = failed.
int AudioPlayer_Start(uint16_t* bufptr, int total_size_in_bytes)
{
    int ret = -1;

    if (bufptr != NULL)
    {
        _audio_info.pbuf = bufptr;
        _audio_info.cur_buf_idx_words = 0;
        _audio_info.total_size_bytes = total_size_in_bytes;

        int txSize_in_bytes = (total_size_in_bytes >= _audio_info.bulk_size_bytes) ? _audio_info.bulk_size_bytes:_audio_info.total_size_bytes;

        i2s2.abort_all_transfers();
        i2s2.clear_transfer_buffer();

        i2s2.protocol(PHILIPS);
        i2s2.format(16, 16, 0);
	    i2s2.mode(MASTER_TX, false);
	    i2s2.audio_frequency(_audio_info.audio_freq);

	    ret = (i2s2.transfer<uint16_t>(_audio_info.pbuf, txSize_in_bytes, NULL, 0, &i2s_callback, I2S_EVENTS));

        if (ret < 0)
        {
            printf("Error(%d)!\r\n", ret);
        }
        else
        {
            _audio_info.cur_buf_idx_words = _audio_info.cur_buf_idx_words + txSize_in_bytes / 2;
            ret = txSize_in_bytes;
        }
    }

    return ret;
}

/// @brief Refill a buffer
/// @param  
/// @return number of data to be sent to I2S buffer. -1 = failed.
int AudioPlayer_RefillBuffer( void )
{
    int txSize_in_bytes = 0;
    int ret = -1;

    if (_audio_info.pbuf != NULL)
    {
        if (_audio_info.cur_buf_idx_words < (_audio_info.total_size_bytes / 2))
        {
            int remain_bytes = _audio_info.total_size_bytes - _audio_info.cur_buf_idx_words * 2;

            txSize_in_bytes = (remain_bytes >= _audio_info.bulk_size_bytes) ? _audio_info.bulk_size_bytes:remain_bytes;

	        ret = i2s2.transfer<uint16_t>(_audio_info.pbuf + _audio_info.cur_buf_idx_words , txSize_in_bytes, NULL, 0, &i2s_callback, I2S_EVENTS);

            if (ret < 0)
            {
                printf("Error(%d)!\r\n", ret);
                ret = -1;
            }
            else
            {
                _audio_info.cur_buf_idx_words = _audio_info.cur_buf_idx_words + txSize_in_bytes / 2;
                ret = txSize_in_bytes;
            }            
        }
        else
        {
            ret = 0;
        }
    }
    else
    {
        ret = -2;
    }

    return ret;
}

/// @brief Stop Audio Player
/// @param  
void AudioPlayer_Stop( void )
{
    i2s2.abort_all_transfers();
}

static void i2s_callback(int narg)
{
	audiobuffer_msg_mail_t* mail;

	mail = (audiobuffer_msg_mail_t*)audio_msg_mail_box.alloc();

	if (mail != NULL)
	{
        if (narg & I2S_EVENT_TX_HALF_COMPLETE)
        {
            mail->msgType = HALF_EMPTY;
            audio_msg_mail_box.put(mail);
        }
        else if (narg & I2S_EVENT_TX_COMPLETE)
        {
            mail->msgType = FULL_EMPTY;
            audio_msg_mail_box.put(mail);
        }
	}    
}


void AudioPlayerManager(void)
{
	osEvent evt;
    int bufIdx = 0;

	audiobuffer_msg_mail_t* mail;

	while (true)
    {
		evt = audio_msg_mail_box.get(10);

		if (evt.status == osEventMail)
		{
			mail = (audiobuffer_msg_mail_t*)evt.value.p;
			switch (mail->msgType)
			{
				case HALF_EMPTY:
					break;
				case FULL_EMPTY:
                {
                    int ret = AudioPlayer_RefillBuffer();

                    if (ret == 0 && _audio_info.repeated == true)
                    {
                        int ret_aud = AudioPlayer_Start(_audio_info.pbuf, _audio_info.total_size_bytes);
                    }
                    break;
                }
            }
			audio_msg_mail_box.free(mail);		
        }
        
        i2s2.i2s_bh_queue.dispatch(10);
    }    
}