/*
 * @Author: Shuai Wang 277668922@qq.com
 * @Date: 2023-11-30 15:51:51
 * @LastEditors: Shuai Wang 277668922@qq.com
 * @LastEditTime: 2024-01-03 16:43:10
 * @FilePath: \stm32f407zet6\APP\FAL\fal_flash_stm32f2_port.c
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
#include "stm32f4xx.h"


/* RT-Thread error code definitions */
#define RT_EOK                   0  /**< There is no error */
#define RT_ERROR                 1  /**< A generic error happens */
#define RT_ETIMEOUT              2  /**< Timed out */
#define RT_EFULL                 3  /**< The resource is full */
#define RT_EEMPTY                4  /**< The resource is empty */
#define RT_ENOMEM                5  /**< No memory */
#define RT_ENOSYS                6  /**< No system */
#define RT_EBUSY                 7  /**< Busy */
#define RT_EIO                   8  /**< IO error */
#define RT_EINTR                 9  /**< Interrupted system call */
#define RT_EINVAL                10 /**< Invalid argument */

#define ROM_START                ((uint32_t)0x08000000)
#define ROM_SIZE                 (1024 * 1024)
#define ROM_END                  ((uint32_t)(ROM_START + ROM_SIZE))

#define RAM_START                (0x20000000)
#define RAM_SIZE                 (192 * 1024)
#define RAM_END                  (RAM_START + RAM_SIZE)

#define STM32_FLASH_START_ADRESS ROM_START
#define STM32_FLASH_SIZE         ROM_SIZE
#define STM32_FLASH_END_ADDRESS  ROM_END

typedef signed char rt_int8_t;      /**<  8bit integer type */
typedef signed short rt_int16_t;    /**< 16bit integer type */
typedef signed int rt_int32_t;      /**< 32bit integer type */
typedef unsigned char rt_uint8_t;   /**<  8bit unsigned integer type */
typedef unsigned short rt_uint16_t; /**< 16bit unsigned integer type */
typedef unsigned int rt_uint32_t;   /**< 32bit unsigned integer type */

typedef int rt_bool_t;            /**< boolean type */
typedef long rt_base_t;           /**< Nbit CPU related date type */
typedef unsigned long rt_ubase_t; /**< Nbit unsigned CPU related data type */

typedef rt_base_t rt_err_t;    /**< Type for error number */
typedef rt_uint32_t rt_time_t; /**< Type for time stamp */
typedef rt_uint32_t rt_tick_t; /**< Type for tick count */
typedef rt_base_t rt_flag_t;   /**< Type for flags */
typedef rt_ubase_t rt_size_t;  /**< Type for size number */
typedef rt_ubase_t rt_dev_t;   /**< Type for device */
typedef rt_base_t rt_off_t;    /**< Type for offset */

/**
 * @brief  Gets the sector of a given address
 * @param  None
 * @retval The sector of a given address
 */
static rt_uint32_t GetSector(rt_uint32_t Address)
{
    return stmflash_get_flash_sector(Address);
}

/**
 * Read data from flash.
 * @note This operation's units is word.
 *
 * @param addr flash address
 * @param buf buffer to store read data
 * @param size read bytes size
 *
 * @return result
 */
int stm32_flash_read(rt_uint32_t addr, rt_uint8_t *buf, size_t size)
{
    size_t i;

    if ((addr + size) > STM32_FLASH_END_ADDRESS) {
        // LOG_E("read outrange flash size! addr is (0x%p)", (void*)(addr + size));
        return -1;
    }

    for (i = 0; i < size; i++, buf++, addr++) {
        *buf = *(rt_uint8_t *)addr;
    }

    return size;
}

/**
 * Write data to flash.
 * @note This operation's units is word.
 * @note This operation must after erase. @see flash_erase.
 *
 * @param addr flash address
 * @param buf the write data buffer
 * @param size write bytes size
 *
 * @return result
 */
int stm32_flash_write(rt_uint32_t addr, const rt_uint8_t *buf, size_t size)
{
    rt_err_t result          = RT_EOK;
    rt_uint32_t end_addr     = addr + size;
    rt_uint32_t written_size = 0;
    rt_uint32_t write_size   = 0;

    if ((end_addr) > STM32_FLASH_END_ADDRESS) {
        // LOG_E("write outrange flash size! addr is (0x%p)", (void*)(addr + size));
        return -RT_EINVAL;
    }

    if (size < 1) {
        return -RT_EINVAL;
    }

    HAL_FLASH_Unlock();

    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

    while (written_size < size) {
        if (((addr + written_size) % 4 == 0) && (size - written_size >= 4)) {
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr + written_size, *((rt_uint32_t *)(buf + written_size))) == HAL_OK) {
                if (*(rt_uint32_t *)(addr + written_size) != *(rt_uint32_t *)(buf + written_size)) {
                    result = -RT_ERROR;
                    break;
                }
            } else {
                result = -RT_ERROR;
                break;
            }
            write_size = 4;
        } else if (((addr + written_size) % 2 == 0) && (size - written_size >= 2)) {
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr + written_size, *((rt_uint16_t *)(buf + written_size))) == HAL_OK) {
                if (*(rt_uint16_t *)(addr + written_size) != *(rt_uint16_t *)(buf + written_size)) {
                    result = -RT_ERROR;
                    break;
                }
            } else {
                result = -RT_ERROR;
                break;
            }
            write_size = 2;
        } else {
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr + written_size, *((rt_uint8_t *)(buf + written_size))) == HAL_OK) {
                if (*(rt_uint8_t *)(addr + written_size) != *(rt_uint8_t *)(buf + written_size)) {
                    result = -RT_ERROR;
                    break;
                }
            } else {
                result = -RT_ERROR;
                break;
            }
            write_size = 1;
        }

        written_size += write_size;
    }

    HAL_FLASH_Lock();

    if (result != RT_EOK) {
        return result;
    }

    return size;
}

/**
 * Erase data on flash.
 * @note This operation is irreversible.
 * @note This operation's units is different which on many chips.
 *
 * @param addr flash address
 * @param size erase bytes size
 *
 * @return result
 */
int stm32_flash_erase(rt_uint32_t addr, size_t size)
{
    rt_err_t result         = RT_EOK;
    rt_uint32_t FirstSector = 0, NbOfSectors = 0;
    rt_uint32_t SECTORError = 0;

    if ((addr + size) > STM32_FLASH_END_ADDRESS) {
        // LOG_E("ERROR: erase outrange flash size! addr is (0x%p)\n", (void*)(addr + size));
        return -RT_EINVAL;
    }

    if (size < 1) {
        return -RT_EINVAL;
    }

    /*Variable used for Erase procedure*/
    FLASH_EraseInitTypeDef EraseInitStruct;

    /* Unlock the Flash to enable the flash control register access */
    HAL_FLASH_Unlock();

    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

    /* Get the 1st sector to erase */
    FirstSector = GetSector(addr);
    /* Get the number of sector to erase from 1st sector*/
    NbOfSectors = GetSector(addr + size - 1) - FirstSector + 1;
    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase    = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector       = FirstSector;
    EraseInitStruct.NbSectors    = NbOfSectors;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, (uint32_t *)&SECTORError) != HAL_OK) {
        result = -RT_ERROR;
        goto __exit;
    }

__exit:
    HAL_FLASH_Lock();

    if (result != RT_EOK) {
        return result;
    }

    // LOG_D("erase done: addr (0x%p), size %d", (void*)addr, size);
    return size;
}
static int init(void)
{
    /* do nothing now */
}

static int read(long offset, uint8_t *buf, size_t size)
{
    // stmflash_read(stm32f4_onchip_flash.addr + offset);
    return stm32_flash_read(stm32f4_onchip_flash.addr + offset, buf, size);
}

static int write(long offset, const uint8_t *buf, size_t size)
{

    return stm32_flash_write(stm32f4_onchip_flash.addr + offset, buf, size);
}

static int erase(long offset, size_t size)
{

    return stm32_flash_erase(stm32f4_onchip_flash.addr + offset, size);
}

const struct fal_flash_dev stm32f4_onchip_flash =
    {
        .name       = "stm32_onchip",
        .addr       = 0x08000000,
        .len        = 1024 * 1024,
        .blk_size   = 128 * 1024,
        .ops        = {init, read, write, erase},
        .write_gran = 8};
