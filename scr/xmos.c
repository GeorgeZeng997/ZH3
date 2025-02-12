#include "config.h"

xmos_reboot_t xmos_reboot={0};
xmos_product_info_t xmos_product_info={0};
xmos_config_info_t xmos_config_info={0};
xmos_loading_firmware_t xmos_loading_firmware={0};

xmos_cmd_t xmos_cmd_array[] = {
    {0x03, XMOS_CMD_BOOT, ""},
    {0x03, XMOS_CMD_READ_PRODUCT, ""},
    {0x03, XMOS_CMD_READ_BOOT_CONFIG, ""},
    {0x03, XMOS_CMD_READ_AUDIO_MODE, ""},
    {0x03, XMOS_CMD_READ_CLIENT_CONFIG, ""},
    {0x03, XMOS_CMD_COMPLETE, ""},
    {0x03, XMOS_CMD_SEND_STATUS, ""},
    {0x03, XMOS_CMD_SEND_MEDIA_CTRL, ""},
    {0x03, XMOS_CMD_SEND_FORMAT, ""},
    {0x03, XMOS_CMD_SEND_AUDIO_MODE, ""},
    {0x03, XMOS_CMD_SEND_PLAY_VOLUME, ""},
    {0x03, XMOS_CMD_SEND_RECORD_VOLUME, ""},
    {0x01, XMOS_CMD_SEND_UPDATE_SUCCESS, ""},
    {0x01, XMOS_CMD_SEND_UPDATE_FAILURE, ""},
    {0x01, XMOS_CMD_SEND_UPDATE_RESTART, ""},
};
