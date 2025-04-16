#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>


#define PURR_CONTROL_PID_KI_MAX (8000)

void printi64(int64_t Paue) {
    int32_t first_p, second_p;
    first_p = (uint32_t)(Paue >> 32);
    second_p = (uint32_t)(Paue >> 0);
    if ((first_p == 0xFFFFFFFF) && ((second_p & 0x80000000) == 0)) printf("\r\nE!!\r\n");
    printf("%d", second_p);
}


void check_test(int32_t v1, int32_t v2) {
    if (v1 != v2) printf("ERR!!   %d != %d\r\n", v1, v2);
    else printf("ok\r\n");
}

void check_utest(uint32_t v1, uint32_t v2) {
    if (v1 != v2) printf("ERR!!   %d != %d\r\n", v1, v2);
    else printf("ok\r\n");
}


typedef struct {
    const uint8_t *name;
    uint8_t resolution;
    uint32_t poly;    // or reversed poly, if CRC_REFOUT == true
    uint32_t init;
    bool refin;
    bool refout;
    uint32_t xorout;
    uint32_t check;
} crc_calc_settings_t;


crc_calc_settings_t crc8_settings[] = {
    {
    .name = "CRC-8",
    .resolution = 8,
    .poly = 0x7,
    .init = 0x0,
    .refin = false,
    .refout = false,
    .xorout = 0x0,
    .check = 0xF4
},
{
    .name = "CRC-8/CDMA2000",
    .resolution = 8,
    .poly = 0x9B,
    .init = 0xFF,
    .refin = false,
    .refout = false,
    .xorout = 0x0,
    .check = 0xDA
},
{
    .name = "CRC-8/DARC",
    .resolution = 8,
    .poly = 0x39,
    .init = 0x0,
    .refin = true,
    .refout = true,
    .xorout = 0x0,
    .check = 0x15
},
{
    .name = "CRC-8/DVB-S2",
    .resolution = 8,
    .poly = 0xD5,
    .init = 0x0,
    .refin = false,
    .refout = false,
    .xorout = 0x0,
    .check = 0xBC
},
{
    .name = "CRC-8/EBU",
    .resolution = 8,
    .poly = 0x1D,
    .init = 0xFF,
    .refin = true,
    .refout = true,
    .xorout = 0x0,
    .check = 0x97
},
{
    .name = "CRC-8/I-CODE",
    .resolution = 8,
    .poly = 0x1D,
    .init = 0xFD,
    .refin = false,
    .refout = false,
    .xorout = 0x0,
    .check = 0x7E
},
{
    .name = "CRC-8/ITU",
    .resolution = 8,
    .poly = 0x7,
    .init = 0x0,
    .refin = false,
    .refout = false,
    .xorout = 0x55,
    .check = 0xA1
},
{
    .name = "CRC-8/MAXIM",
    .resolution = 8,
    .poly = 0x31,
    .init = 0x0,
    .refin = true,
    .refout = true,
    .xorout = 0x0,
    .check = 0xA1
},
{
    .name = "CRC-8/ROHC",
    .resolution = 8,
    .poly = 0x7,
    .init = 0xFF,
    .refin = true,
    .refout = true,
    .xorout = 0x0,
    .check = 0xD0
},
{
    .name = "CRC-8/WCDMA",
    .resolution = 8,
    .poly = 0x9B,
    .init = 0x0,
    .refin = true,
    .refout = true,
    .xorout = 0x0,
    .check = 0x25
}
};

crc_calc_settings_t crc16_settings[] = {
{
    .name = "CRC-16/ARC",
    .resolution = 16,
    .poly = 0x8005,
    .init = 0x0,
    .refin = true,
    .refout = true,
    .xorout = 0x0,
    .check = 0xBB3D
},
{
    .name = "CRC-16/AUG-CCITT",
    .resolution = 16,
    .poly = 0x1021,
    .init = 0x1D0F,
    .refin = false,
    .refout = false,
    .xorout = 0x0,
    .check = 0xE5CC
},
{
    .name = "CRC-16/BUYPASS",
    .resolution = 16,
    .poly = 0x8005,
    .init = 0x0,
    .refin = false,
    .refout = false,
    .xorout = 0x0,
    .check = 0xFEE8
},
{
    .name = "CRC-16/CCITT-FALSE",
    .resolution = 16,
    .poly = 0x1021,
    .init = 0xFFFF,
    .refin = false,
    .refout = false,
    .xorout = 0x0,
    .check = 0x29B1
},
{
    .name = "CRC-16/CDMA2000",
    .resolution = 16,
    .poly = 0xC867,
    .init = 0xFFFF,
    .refin = false,
    .refout = false,
    .xorout = 0x0,
    .check = 0x4C06
},
{
    .name = "CRC-16/DDS-110",
    .resolution = 16,
    .poly = 0x8005,
    .init = 0x800D,
    .refin = false,
    .refout = false,
    .xorout = 0x0,
    .check = 0x9ECF
},
{
    .name = "CRC-16/DECT-R",
    .resolution = 16,
    .poly = 0x589,
    .init = 0x0,
    .refin = false,
    .refout = false,
    .xorout = 0x1,
    .check = 0x7E
},
{
    .name = "CRC-16/DECT-X",
    .resolution = 16,
    .poly = 0x589,
    .init = 0x0,
    .refin = false,
    .refout = false,
    .xorout = 0x0,
    .check = 0x7F
},
{
    .name = "CRC-16/DNP",
    .resolution = 16,
    .poly = 0x3D65,
    .init = 0x0,
    .refin = true,
    .refout = true,
    .xorout = 0xFFFF,
    .check = 0xEA82
},
{
    .name = "CRC-16/EN-13757",
    .resolution = 16,
    .poly = 0x3D65,
    .init = 0x0,
    .refin = false,
    .refout = false,
    .xorout = 0xFFFF,
    .check = 0xC2B7
},
{
    .name = "CRC-16/GENIBUS",
    .resolution = 16,
    .poly = 0x1021,
    .init = 0xFFFF,
    .refin = false,
    .refout = false,
    .xorout = 0xFFFF,
    .check = 0xD64E
},
{
    .name = "CRC-16/MAXIM",
    .resolution = 16,
    .poly = 0x8005,
    .init = 0x0,
    .refin = true,
    .refout = true,
    .xorout = 0xFFFF,
    .check = 0x44C2
},
{
    .name = "CRC-16/MCRF4XX",
    .resolution = 16,
    .poly = 0x1021,
    .init = 0xFFFF,
    .refin = true,
    .refout = true,
    .xorout = 0x0,
    .check = 0x6F91
},
{
    .name = "CRC-16/RIELLO",
    .resolution = 16,
    .poly = 0x1021,
    .init = 0xB2AA,
    .refin = true,
    .refout = true,
    .xorout = 0x0,
    .check = 0x63D0
},
{
    .name = "CRC-16/T10-DIF",
    .resolution = 16,
    .poly = 0x8BB7,
    .init = 0x0,
    .refin = false,
    .refout = false,
    .xorout = 0x0,
    .check = 0xD0DB
},
{
    .name = "CRC-16/TELEDISK",
    .resolution = 16,
    .poly = 0xA097,
    .init = 0x0,
    .refin = false,
    .refout = false,
    .xorout = 0x0,
    .check = 0xFB3
},
{
    .name = "CRC-16/TMS37157",
    .resolution = 16,
    .poly = 0x1021,
    .init = 0x89EC,
    .refin = true,
    .refout = true,
    .xorout = 0x0,
    .check = 0x26B1
},
{
    .name = "CRC-16/USB",
    .resolution = 16,
    .poly = 0x8005,
    .init = 0xFFFF,
    .refin = true,
    .refout = true,
    .xorout = 0xFFFF,
    .check = 0xB4C8
},
{
    .name = "CRC-16/KERMIT",
    .resolution = 16,
    .poly = 0x1021,
    .init = 0x0,
    .refin = true,
    .refout = true,
    .xorout = 0x0,
    .check = 0x2189
},
{
    .name = "CRC-16/MODBUS",
    .resolution = 16,
    .poly = 0x8005,
    .init = 0xFFFF,
    .refin = true,
    .refout = true,
    .xorout = 0x0,
    .check = 0x4B37
},
{
    .name = "CRC-16/X-25",
    .resolution = 16,
    .poly = 0x1021,
    .init = 0xFFFF,
    .refin = true,
    .refout = true,
    .xorout = 0xFFFF,
    .check = 0x906E
},
{
    .name = "CRC-16/XMODEM",
    .resolution = 16,
    .poly = 0x1021,
    .init = 0x0,
    .refin = false,
    .refout = false,
    .xorout = 0x0,
    .check = 0x31C3
}
};

crc_calc_settings_t crc32_settings[] = {
{
    .name = "CRC-32/Zlib",
    .resolution = 32,
    .poly = 0x4C11DB7,
    .init = 0xFFFFFFFF,
    .refin = true,
    .refout = true,
    .xorout = 0xFFFFFFFF,
    .check = 0xCBF43926
},
{
    .name = "CRC-32/BZIP2",
    .resolution = 32,
    .poly = 0x4C11DB7,
    .init = 0xFFFFFFFF,
    .refin = false,
    .refout = false,
    .xorout = 0xFFFFFFFF,
    .check = 0xFC891918
},
{
    .name = "CRC-32C",
    .resolution = 32,
    .poly = 0x1EDC6F41,
    .init = 0xFFFFFFFF,
    .refin = true,
    .refout = true,
    .xorout = 0xFFFFFFFF,
    .check = 0xE3069283
},
{
    .name = "CRC-32D",
    .resolution = 32,
    .poly = 0xA833982B,
    .init = 0xFFFFFFFF,
    .refin = true,
    .refout = true,
    .xorout = 0xFFFFFFFF,
    .check = 0x87315576
},
{
    .name = "CRC-32/MPEG-2",
    .resolution = 32,
    .poly = 0x4C11DB7,
    .init = 0xFFFFFFFF,
    .refin = false,
    .refout = false,
    .xorout = 0x0,
    .check = 0x376E6E7
},
{
    .name = "CRC-32/POSIX",
    .resolution = 32,
    .poly = 0x4C11DB7,
    .init = 0x0,
    .refin = false,
    .refout = false,
    .xorout = 0xFFFFFFFF,
    .check = 0x765E7680
},
{
    .name = "CRC-32Q",
    .resolution = 32,
    .poly = 0x814141AB,
    .init = 0x0,
    .refin = false,
    .refout = false,
    .xorout = 0x0,
    .check = 0x3010BF7F
},
{
    .name = "CRC-32/JAMCRC",
    .resolution = 32,
    .poly = 0x4C11DB7,
    .init = 0xFFFFFFFF,
    .refin = true,
    .refout = true,
    .xorout = 0x0,
    .check = 0x340BC6D9
},
{
    .name = "CRC-32/XFER",
    .resolution = 32,
    .poly = 0xAF,
    .init = 0x0,
    .refin = false,
    .refout = false,
    .xorout = 0x0,
    .check = 0xBD0BE338
}
};




void crc_create_table(uint8_t data_resolution, crc_calc_settings_t *crc_settings, const void *crc_table, uint32_t *crc_table_size) {
    uint32_t crc_bit;
    uint32_t i;
    uint32_t poly;
    uint32_t table_size;
    uint32_t table_index;
    uint32_t table_value;


    if (crc_settings->refout) {
        // Poly mirrored
        poly = 0;
        for (i = 0; i < crc_settings->resolution; i++) {
            poly = poly << 1;
            poly |= (crc_settings->poly >> i) & 0x1;
        }
    }
    else {
        poly = crc_settings->poly;
    }


    table_size = (1 << data_resolution);


    if (crc_settings->refin) {
        for (table_index = 0; table_index < table_size; table_index++) {
            table_value = table_index;
            for (crc_bit = 0; crc_bit < data_resolution; crc_bit++) {
                if (table_value & 0x01) {
                    table_value = (table_value >> 1) ^ poly;
                }
                else {
                    table_value = (table_value >> 1);
                }
            }

            if (crc_settings->resolution == 8) ((uint8_t*)crc_table)[table_index] = (uint8_t)table_value;
            else if (crc_settings->resolution == 16) ((uint16_t*)crc_table)[table_index] = (uint16_t)table_value;
            else if (crc_settings->resolution == 32) ((uint32_t*)crc_table)[table_index] = (uint32_t)table_value;
        }
    }
    else {
        for (table_index = 0; table_index < table_size; table_index++) {
            if (data_resolution == 8) table_value = table_index << (crc_settings->resolution - 8);
            else if (data_resolution == 16) table_value = table_index << (crc_settings->resolution - 16);
            else if (data_resolution == 32) table_value = table_index;

            for (crc_bit = 0; crc_bit < data_resolution; crc_bit++) {
                if (table_value & (1 << (crc_settings->resolution - 1))) {
                    table_value = (table_value << 1) ^ poly;
                }
                else {
                    table_value = (table_value << 1);
                }
            }

            if (crc_settings->resolution == 8) ((uint8_t*)crc_table)[table_index] = (uint8_t)table_value;
            else if (crc_settings->resolution == 16) ((uint16_t*)crc_table)[table_index] = (uint16_t)table_value;
            else if (crc_settings->resolution == 32) ((uint32_t*)crc_table)[table_index] = (uint32_t)table_value;
        }
    }

    if (crc_table_size != NULL) *crc_table_size = table_size;
}


uint32_t crc_continue_clac_by_table(uint32_t init, const void *data, uint8_t data_resolution, uint32_t data_qty, crc_calc_settings_t *crc_settings, const void *crc_table) {
    uint32_t crc;
    uint32_t data_index;
    uint32_t data_conv;
    uint32_t i;
    uint32_t init_old;
    uint32_t table_index;


    if (crc_settings->refout) {
        // Init mirrored
        init_old = init;
        init = 0;
        for (i = 0; i < crc_settings->resolution; i++) {
            init = init << 1;
            init |= (init_old >> i) & 0x1;
        }
    }


    crc = init;
    if (crc_settings->refin) {
        for (data_index = 0; data_index < data_qty; data_index++) {
            if (data_resolution == 8) data_conv = ((uint8_t*)data)[data_index];
            else if (data_resolution == 16) data_conv = ((uint16_t*)data)[data_index];
            else if (data_resolution == 32) data_conv = ((uint32_t*)data)[data_index];

            table_index = crc ^ data_conv;
            table_index &= ((1 << data_resolution) - 1);
            crc >>= data_resolution;

            if (crc_settings->resolution == 8) crc ^= ((uint8_t*)crc_table)[table_index];
            else if (crc_settings->resolution == 16) crc ^= ((uint16_t*)crc_table)[table_index];
            else if (crc_settings->resolution == 32) crc ^= ((uint32_t*)crc_table)[table_index];
        }
    }
    else {
        for (data_index = 0; data_index < data_qty; data_index++) {
            if (data_resolution == 8) data_conv = ((uint8_t*)data)[data_index];
            else if (data_resolution == 16) data_conv = ((uint16_t*)data)[data_index];
            else if (data_resolution == 32) data_conv = ((uint32_t*)data)[data_index];

            table_index = (crc >> (crc_settings->resolution - data_resolution)) ^ data_conv;
            table_index &= ((1 << data_resolution) - 1);
            crc <<= data_resolution;

            if (crc_settings->resolution == 8) crc ^= ((uint8_t*)crc_table)[table_index];
            else if (crc_settings->resolution == 16) crc ^= ((uint16_t*)crc_table)[table_index];
            else if (crc_settings->resolution == 32) crc ^= ((uint32_t*)crc_table)[table_index];
        }
    }

    crc ^= crc_settings->xorout;
    return crc;
}


uint32_t crc_continue_clac(uint32_t init, const void *data, uint8_t data_resolution, uint32_t data_qty, crc_calc_settings_t *crc_settings) {
    uint32_t crc;
    uint32_t data_index, crc_bit;
    uint32_t data_conv;
    uint32_t i;
    uint32_t poly;
    uint32_t init_old;


    if (crc_settings->refout) {
        // Poly mirrored
        poly = 0;
        for (i = 0; i < crc_settings->resolution; i++) {
            poly = poly << 1;
            poly |= (crc_settings->poly >> i) & 0x1;
        }

        // Init mirrored
        init_old = init;
        init = 0;
        for (i = 0; i < crc_settings->resolution; i++) {
            init = init << 1;
            init |= (init_old >> i) & 0x1;
        }
    }
    else {
        poly = crc_settings->poly;
    }


    crc = init;
    if (crc_settings->refin) {
        for (data_index = 0; data_index < data_qty; data_index++) {

            if (data_resolution == 8) data_conv = ((uint8_t*)data)[data_index];
            else if (data_resolution == 16) data_conv = ((uint16_t*)data)[data_index];
            else if (data_resolution == 32) data_conv = ((uint32_t*)data)[data_index];

            crc ^= data_conv;
            for (crc_bit = 0; crc_bit < data_resolution; crc_bit++) {
                if (crc & 0x01) {
                    crc = (crc >> 1) ^ poly;
                }
                else {
                    crc = (crc >> 1);
                }
            }
        }
    }
    else {
        /*
        for (data_index = 0; data_index < data_qty; data_index++) {
            //if (data_resolution == 8) data_conv = ((uint8_t*)data)[data_index] << (crc_settings->resolution - 8);
            //else if (data_resolution == 16) data_conv = ((uint16_t*)data)[data_index] << (crc_settings->resolution - 16);
            //else if (data_resolution == 32) data_conv = ((uint32_t*)data)[data_index];
            if (data_resolution == 8) data_conv = ((uint8_t*)data)[data_index];
            else if (data_resolution == 16) data_conv = ((uint16_t*)data)[data_index];
            else if (data_resolution == 32) data_conv = ((uint32_t*)data)[data_index];

            table_index = crc ^ data_conv;
            table_index &= ((1 << data_resolution) - 1);
            crc <<= data_resolution;

            if (crc_settings->resolution == 8) crc ^= ((uint8_t*)crc_table)[table_index];
            else if (crc_settings->resolution == 16) crc ^= ((uint16_t*)crc_table)[table_index];
            else if (crc_settings->resolution == 32) crc ^= ((uint32_t*)crc_table)[table_index];
        }*/


        for (data_index = 0; data_index < data_qty; data_index++) {
            if (data_resolution == 8) data_conv = ((uint8_t*)data)[data_index] << (crc_settings->resolution - 8);
            else if (data_resolution == 16) data_conv = ((uint16_t*)data)[data_index] << (crc_settings->resolution - 16);
            else if (data_resolution == 32) data_conv = ((uint32_t*)data)[data_index];

            crc ^= data_conv;
            for (crc_bit = 0; crc_bit < data_resolution; crc_bit++) {
                if (crc & (1 << (crc_settings->resolution - 1))) {
                    crc = (crc << 1) ^ poly;
                }
                else {
                    crc = (crc << 1);
                }
            }
        }
    }

    crc ^= crc_settings->xorout;
    return crc;
}




int main() {
    uint8_t data[] = "123456789";
    uint32_t crc, i;

/*
    // refout = true, refin = true
    crc_calc_settings_t crc_settings_1 = {
        .resolution = 16,
        .poly = 0x8005,
        .init = 0xFFFF,
        .refin = true,
        .refout = true,
        .xorout = 0
    };
    // refout = false, refin = false
    crc_calc_settings_t crc_settings_2 = {
        .resolution = 16,
        .poly = 0xA097,
        .init = 0,
        .refin = false,
        .refout = false,
        .xorout = 0
    };


    uint16_t crc16;

    crc16 = crc_continue_clac(crc_settings_1.init, data, 8, 9, &crc_settings_1);
    printf("CRC16_1 = 0x%04X\r\n", crc16);
    crc16 = crc_continue_clac(crc_settings_2.init, data, 8, 9, &crc_settings_2);
    printf("CRC16_2 = 0x%04X\r\n", crc16);

    return 0;*/


/*
    for (i = 0; i < (sizeof(crc_settings) / sizeof(crc_calc_settings_t)); i++) {
        crc = crc_continue_clac(crc_settings[i].init, data, 8, 9, &crc_settings[i]);
        if (crc_settings[i].resolution < 32) crc &= ((1 << crc_settings[i].resolution) - 1);
        printf("%s    ", crc_settings[i].name);
        check_utest(crc, crc_settings[i].check);
    }
*/


/*
    // refout = true, refin = true
    crc_calc_settings_t crc_settings_1 = {
        .resolution = 16,
        .poly = 0x8005,
        .init = 0xFFFF,
        .refin = true,
        .refout = true,
        .xorout = 0
    };
    // refout = false, refin = false
    crc_calc_settings_t crc_settings_2 = {
        .resolution = 16,
        .poly = 0x1021,
        .init = 0,
        .refin = false,
        .refout = false,
        .xorout = 0
    };
    uint16_t crc_table[256];
    crc_create_table(8, &crc_settings_1, (void*)crc_table, NULL);
    crc = crc_continue_clac_by_table(crc_settings_1.init, data, 8, 9, &crc_settings_1, (void*)crc_table);
    printf("CRC16 = 0x%04X\r\n", crc);

    crc_create_table(8, &crc_settings_2, (void*)crc_table, NULL);
    crc = crc_continue_clac_by_table(crc_settings_2.init, data, 8, 9, &crc_settings_2, (void*)crc_table);
    printf("CRC16 = 0x%04X\r\n", crc);
*/


    uint8_t crc8_table[256];
    for (i = 0; i < (sizeof(crc8_settings) / sizeof(crc_calc_settings_t)); i++) {
        crc_create_table(8, &crc8_settings[i], (void*)crc8_table, NULL);
        crc = crc_continue_clac_by_table(crc8_settings[i].init, data, 8, 9, &crc8_settings[i], (void*)crc8_table);
        if (crc8_settings[i].resolution < 32) crc &= ((1 << crc8_settings[i].resolution) - 1);
        printf("%s %d %d    ", crc8_settings[i].name, crc8_settings[i].refin, crc8_settings[i].refout);
        check_utest(crc, crc8_settings[i].check);
    }
    uint16_t crc16_table[256];
    for (i = 0; i < (sizeof(crc16_settings) / sizeof(crc_calc_settings_t)); i++) {
        crc_create_table(8, &crc16_settings[i], (void*)crc16_table, NULL);
        crc = crc_continue_clac_by_table(crc16_settings[i].init, data, 8, 9, &crc16_settings[i], (void*)crc16_table);
        if (crc16_settings[i].resolution < 32) crc &= ((1 << crc16_settings[i].resolution) - 1);
        printf("%s %d %d    ", crc16_settings[i].name, crc16_settings[i].refin, crc16_settings[i].refout);
        check_utest(crc, crc16_settings[i].check);
    }
    uint32_t crc32_table[256];
    for (i = 0; i < (sizeof(crc32_settings) / sizeof(crc_calc_settings_t)); i++) {
        crc_create_table(8, &crc32_settings[i], (void*)crc32_table, NULL);
        crc = crc_continue_clac_by_table(crc32_settings[i].init, data, 8, 9, &crc32_settings[i], (void*)crc32_table);
        if (crc32_settings[i].resolution < 32) crc &= ((1 << crc32_settings[i].resolution) - 1);
        printf("%s %d %d    ", crc32_settings[i].name, crc32_settings[i].refin, crc32_settings[i].refout);
        check_utest(crc, crc32_settings[i].check);
    }

    return 0;
}