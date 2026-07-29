#pragma once

#include <common.h>

typedef struct{
    u8 entry[4];
    u8 logo[0x30];

    char title[16];
    u16 new_lic_codes;
    u8 sgb_flag;
    u8 cart_type;
    u8 rom_size;
    u8 ram_size;
    u8 dest_code;
    u8 old_lic_codes;
    u8 Mask_rom_ver;
    u8 checksum;
    u16 global_checksum;


}rom_header;


bool cart_loader(char *cart);