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
void fal_test(void)
{
#define test_length    128
#define test_Partition "test"

    uint8_t buf[test_length];
    uint8_t find_partion = 0;
    fal_show_part_table();
    find_partion = fal_partition_find(test_Partition) != NULL;
    if (find_partion) {
        printf("\r\nfal_partition_find(%s) succeed\r\n", test_Partition);

        printf("origin \r\n");
        fal_partition_read(fal_partition_find(test_Partition), 0, buf, test_length);
        for (uint8_t i = 0; i < test_length; i++) {
            printf("0x%02x,", buf[i]);
            if ((i + 1) % 16 == 0) printf("\r\n");
        }

        // erase operation
        fal_partition_erase(fal_partition_find(test_Partition), 0, test_length);
        fal_partition_read(fal_partition_find(test_Partition), 0, buf, test_length);
        printf("\r\nerased \r\n");
        for (uint8_t i = 0; i < test_length; i++) {
            printf("0x%02x,", buf[i]);
            if ((i + 1) % 16 == 0) printf("\r\n");
        }
        printf("\r\n");

        for (uint8_t i = 0; i < test_length; i++) {
            buf[i] = 0x10 + i;
        }

        fal_partition_write(fal_partition_find(test_Partition), 0, buf, test_length);
        // it wont check whether succeed
        printf("\r\nwrite \r\n");
        for (uint8_t i = 0; i < test_length; i++) {
            printf("0x%02x,", buf[i]);
            if ((i + 1) % 16 == 0) printf("\r\n");
        }
        printf("\r\nresult \r\n");
        fal_partition_read(fal_partition_find(test_Partition), 0, buf, test_length);
        for (uint8_t i = 0; i < test_length; i++) {
            printf("0x%02x,", buf[i]);
            if ((i + 1) % 16 == 0) printf("\r\n");
        }
        printf("\r\n");
    } else {
        printf("\r\nfal_partition_find(%s) failed\r\n", test_Partition);
    }
}