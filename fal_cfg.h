/*
 * @Author: Shuai Wang 277668922@qq.com
 * @Date: 2023-12-25 14:15:48
 * @LastEditors: Shuai Wang 277668922@qq.com
 * @LastEditTime: 2024-03-18 14:51:37
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

#define NOR_FLASH_DEV_NAME "norflash0"

/* ===================== Flash device Configuration ========================= */
extern const struct fal_flash_dev stm32f4_onchip_flash;
extern struct fal_flash_dev nor_flash0;

#define BSCIC_CONFIG_NAME "BasicConfig"
// &stm32f4_onchip_flash,
/* flash device table */
#define FAL_FLASH_DEV_TABLE \
    {                       \
        &nor_flash0,        \
    }
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG
/* partition table */
#define FAL_PART_TABLE                                                                                   \
    {                                                                                                    \
        {FAL_PART_MAGIC_WORD, "Reserved", NOR_FLASH_DEV_NAME, 0, 16 * 1024, 0},                          \
            {FAL_PART_MAGIC_WORD, "ConfigsPart", NOR_FLASH_DEV_NAME, 16 * 1024, 32 * 1024, 0},           \
            {FAL_PART_MAGIC_WORD, "FontsConfigPart", NOR_FLASH_DEV_NAME, 48 * 1024, 32 * 1024, 0},       \
            {FAL_PART_MAGIC_WORD, "PatientPart", NOR_FLASH_DEV_NAME, 80 * 1024, 32 * 1024, 0},           \
            {FAL_PART_MAGIC_WORD, "StatisticPart", NOR_FLASH_DEV_NAME, 112 * 1024, 64 * 1024, 0},        \
            {FAL_PART_MAGIC_WORD, "Fonts", NOR_FLASH_DEV_NAME, 520 * 1024, 1024 * 1024, 0},              \
            {FAL_PART_MAGIC_WORD, BSCIC_CONFIG_NAME, NOR_FLASH_DEV_NAME, 2 * 1024 * 1024, 16 * 1024, 0}, \
    }

#endif /* FAL_PART_HAS_TABLE_CFG */

#endif /* _FAL_CFG_H_ */
