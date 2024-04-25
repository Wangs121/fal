/*
 * @Author: Shuai Wang 277668922@qq.com
 * @Date: 2023-12-25 14:15:48
 * @LastEditors: Shuai Wang 277668922@qq.com
 * @LastEditTime: 2024-04-25 16:28:36
 * @FilePath: \stm32f407zet6\Library\fal\fal_cfg.h
 * @Description:
 */
/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-17     armink       the first version
 */

#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#define FAL_PART_HAS_TABLE_CFG

#define ONCHIP_FLASH_DEV_NAME "stm32_onchip"
#define NOR_FLASH_DEV_NAME "norflash0"

/* ===================== Flash device Configuration ========================= */
extern const struct fal_flash_dev stm32f4_onchip_flash;
extern struct fal_flash_dev nor_flash0;

// &stm32f4_onchip_flash,
/* flash device table */
#define FAL_FLASH_DEV_TABLE        \
    {                              \
        &nor_flash0,               \
            &stm32f4_onchip_flash, \
    }
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG
/* partition table */
#define FAL_PART_TABLE                                                                                              \
    {                                                                                                               \
        {FAL_PART_MAGIC_WORD, "Bootloader", ONCHIP_FLASH_DEV_NAME, 0, 0x8000, 0},                                 \
            {FAL_PART_MAGIC_WORD, "App", ONCHIP_FLASH_DEV_NAME, 0x8000, 0xF8000, 0},                                    \
            {FAL_PART_MAGIC_WORD, "Nor_Boot", NOR_FLASH_DEV_NAME, 0, 8 * 1024, 0},                              \
            {FAL_PART_MAGIC_WORD, "Nor_Info", NOR_FLASH_DEV_NAME, 8 * 1024, 8 * 1024, 0},                       \
            {FAL_PART_MAGIC_WORD, "Nor_Patient", NOR_FLASH_DEV_NAME, 16 * 1024, 64 * 1024, 0},                   \
            {FAL_PART_MAGIC_WORD, "Nor_Statistic", NOR_FLASH_DEV_NAME, 80 * 1024, 64 * 1024, 0},                    \
            {FAL_PART_MAGIC_WORD, "Nor_Reserved", NOR_FLASH_DEV_NAME, 144 * 1024, 880 * 1024 + 7 * 1024 * 1024, 0}, \
            {FAL_PART_MAGIC_WORD, "Nor_Font", NOR_FLASH_DEV_NAME, 8 * 1024 * 1024, 6 * 1024 * 1024, 0},             \
            {FAL_PART_MAGIC_WORD, "Nor_APP", NOR_FLASH_DEV_NAME, 14 * 1024 * 1024, 2 * 1024 * 1024, 0},             \
    }

#endif /* FAL_PART_HAS_TABLE_CFG */

#endif /* _FAL_CFG_H_ */
