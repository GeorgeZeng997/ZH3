#ifndef __XMOS_H
#define __XMOS_H

#include "stdint.h"

#define XMOS_HEADER     0X55AA
#define XMOS_UPDATE_VERSION    0X01

#define XMOS_DEFAULT_LENGTH 6

#define XMOS_CMD_BOOT   0X00
#define XMOS_CMD_READ_PRODUCT  0X01
#define XMOS_CMD_READ_BOOT_CONFIG   0X02
#define XMOS_CMD_READ_AUDIO_MODE 0X03
#define XMOS_CMD_READ_CLIENT_CONFIG 0X04
#define XMOS_CMD_COMPLETE   0X05

#define XMOS_CMD_SEND_STATUS    0X20
#define XMOS_CMD_SEND_MEDIA_CTRL 0X21
#define XMOS_CMD_SEND_FORMAT    0X22
#define XMOS_CMD_SEND_AUDIO_MODE    0X23
#define XMOS_CMD_SEND_PLAY_VOLUME   0X24
#define XMOS_CMD_SEND_RECORD_VOLUME 0X25

#define XMOS_CMD_SEND_UPDATE_SUCCESS 0X01
#define XMOS_CMD_SEND_UPDATE_FAILURE 0X00
#define XMOS_CMD_SEND_UPDATE_RESTART 0X02

typedef enum
{
    XMOS_CMD_IDX_BOOT,
    XMOS_CMD_IDX_READ_PRODUCT,
    XMOS_CMD_IDX_READ_BOOT_CONFIG,
    XMOS_CMD_IDX_READ_AUDIO_MODE,
    XMOS_CMD_IDX_READ_CLIENT_CONFIG,
    XMOS_CMD_IDX_COMPLETE,

    XMOS_CMD_IDX_SEND_STATUS,
    XMOS_CMD_IDX_SEND_MEDIA_CTRL,
    XMOS_CMD_IDX_SEND_FORMAT,
    XMOS_CMD_IDX_SEND_AUDIO_MODE,
    XMOS_CMD_IDX_SEND_PLAY_VOLUME,
    XMOS_CMD_IDX_SEND_RECORD_VOLUME,

    XMOS_CMD_IDX_UPDATE_PACK_SUCCESS,
    XMOS_CMD_IDX_UPDATE_PACK_FAILURE,
    XMOS_CMD_IDX_UPDATE_PACK_RESTART,
}xmos_cmd_idx_t;

typedef struct 
{
    uint8_t version;
    uint8_t cmd;
    char *content;
}xmos_cmd_t;

typedef struct 
{
    uint8_t reason;
    uint16_t uac_1_vid;
    uint16_t uac_1_pid;
    uint16_t uac_2_vid;
    uint16_t uac_2_pid;
    uint32_t product_crc;
    uint32_t config_crc;
    uint8_t checksum;
}xmos_reboot_t;


typedef struct 
{
    uint16_t uac_1_vid;
    uint16_t uac_1_pid;
    uint16_t uac_2_vid;
    uint16_t uac_2_pid;
    uint8_t manu_name[16];
    uint8_t product_name[16];
    uint8_t serial_num[16];
    uint32_t crc;
}xmos_product_info_t;

typedef struct 
{
    uint8_t audio_mode[5];
    uint16_t mute_time;
    uint8_t mic_volume;
    uint8_t left_channal_volume;
    uint8_t right_channal_volume;
    uint32_t crc;
}xmos_config_info_t;

typedef struct 
{
    uint8_t i2s_mode;
    uint8_t sample_rate;
    uint16_t config;
    uint8_t play_mode;
}xmos_loading_firmware_t;

xmos_reboot_t xmos_reboot;
xmos_product_info_t xmos_product_info;
xmos_config_info_t xmos_config_info;
xmos_loading_firmware_t xmos_loading_firmware;

extern xmos_cmd_t xmos_cmd_array[];

#endif

