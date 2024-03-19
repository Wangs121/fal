/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-17     armink       the first version
 */

#include <fal.h>

static uint8_t init_ok = 0;

/**
 * FAL (Flash Abstraction Layer) initialization.
 * It will initialize all flash device and all flash partition.
 *
 * @return >= 0: partitions total number
 */
int fal_init(void)
{
    extern int fal_flash_init(void);
    extern int fal_partition_init(void);

    int result;

    /* initialize all flash device on FAL flash table */
    result = fal_flash_init();

    if (result < 0) {
        goto __exit;
    }

    /* initialize all flash partition on FAL partition table */
    result = fal_partition_init();
    // fal_show_part_table();
__exit:

    if ((result > 0) && (!init_ok))
    {
        init_ok = 1;
        log_i("RT-Thread Flash Abstraction Layer initialize success.");
    }
    else if(result <= 0)
    {
        init_ok = 0;
        log_e("RT-Thread Flash Abstraction Layer initialize failed.");
    }

    return result;
}

/**
 * Check if the FAL is initialized successfully
 *
 * @return 0: not init or init failed; 1: init success
 */
int fal_init_check(void)
{
    return init_ok;
}

/**
 * @description: 清空所有扇区
 * @return {*}
 */
void fal_erase_all_partitions(void)
{
    size_t len = 0;
    const struct fal_partition *partions = fal_get_partition_table(&len);
    for(uint16_t i = 0; i < len; i++) {
        fal_partition_erase_all(&partions[i]);
    }
}