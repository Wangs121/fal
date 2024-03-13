/*
 * @Author: Shuai Wang 277668922@qq.com
 * @Date: 2023-12-25 14:15:48
 * @LastEditors: Shuai Wang 277668922@qq.com
 * @LastEditTime: 2024-01-04 09:16:07
 * @FilePath: \stm32f407zet6\APP\FAL\fal_flash_sfud_port.c
 * @Description: 
 */
/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-01-26     armink       the first version
 */

#include <fal.h>
#include <sfud.h>
#include "sfud_cfg.h"

#define FAL_USING_SFUD_PORT
#define RT_USING_SFUD
#ifdef FAL_USING_SFUD_PORT



static int init(void);
static int read(long offset, uint8_t *buf, size_t size);
static int write(long offset, const uint8_t *buf, size_t size);
static int erase(long offset, size_t size);

static sfud_flash_t sfud_dev = NULL;
struct fal_flash_dev nor_flash0 =
{
    .name       = NOR_FLASH_DEV_NAME,
    .addr       = 0,
    .len        = 16 * 1024 * 1024,
    .blk_size   = 4096,
    .ops        = {init, read, write, erase},
    .write_gran = 1
};

static int init(void)
{

#ifdef RT_USING_SFUD
    
    // if (sfud_init() == SFUD_SUCCESS) {
    //     // sfud_demo();
    // }
#ifdef SFUD_USING_SFDP
    sfud_dev = sfud_get_device(SFUD_W25Q128_DEVICE_INDEX);
#else
    extern sfud_flash sfud_norflash0;
    sfud_dev = &sfud_norflash0;
#endif
#else
    /* bare metal platform */
    extern sfud_flash flash_table[];
    sfud_dev = &flash_table[0];
#endif

    if (NULL == sfud_dev)
    {
        return -1;
    }

    /* update the flash chip information */
    nor_flash0.blk_size = sfud_dev->chip.erase_gran;
    nor_flash0.len = sfud_dev->chip.capacity;

    return 0;
}

static int read(long offset, uint8_t *buf, size_t size)
{
    assert(sfud_dev);
    assert(sfud_dev->init_ok);
    sfud_read(sfud_dev, nor_flash0.addr + offset, size, buf);

    return size;
}

static int write(long offset, const uint8_t *buf, size_t size)
{
    assert(sfud_dev);
    assert(sfud_dev->init_ok);
    if (sfud_write(sfud_dev, nor_flash0.addr + offset, size, buf) != SFUD_SUCCESS)
    {
        return -1;
    }

    return size;
}

static int erase(long offset, size_t size)
{
    assert(sfud_dev);
    assert(sfud_dev->init_ok);
    if (sfud_erase(sfud_dev, nor_flash0.addr + offset, size) != SFUD_SUCCESS)
    {
        return -1;
    }

    return size;
}
#endif /* FAL_USING_SFUD_PORT */

