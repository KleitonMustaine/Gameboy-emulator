#include <bus.h>
#include <cart.h>

//0x0000	3FFF	16 KiB ROM bank 00	From cartridge, usually a fixed bank
//0x4000	7FFF	16 KiB ROM Bank 01–NN	From cartridge, switchable bank via mapper (if any)
//0x8000	9FFF	8 KiB Video RAM (VRAM)	In CGB mode, switchable bank 0/1
//0xA000	BFFF	8 KiB External RAM	From cartridge, switchable bank if any
//0xC000	CFFF	4 KiB Work RAM (WRAM)	
//0xD000	DFFF	4 KiB Work RAM (WRAM)	In CGB mode, switchable bank 1–7
//0xE000	FDFF	Echo RAM (mirror of C000–DDFF)	Nintendo says use of this area is prohibited.
//0xFE00	FE9F	Object attribute memory (OAM)	
//0xFEA0	FEFF	Not Usable	Nintendo says use of this area is prohibited.
//0xFF00	FF7F	I/O Registers	
//0xFF80	FFFE	High RAM (HRAM)	
//0xFFFF	FFFF	Interrupt Enable register (IE)

static const char *bus_region(u16 address){
    //ROM Data
    if (address < 0x4000) return cart_read(address);
    if (address < 0x8000) return cart_read(address);
    //Char/Map Data
    if (address < 0xA000) return cart_read(address);
    if (address < 0xC000) return cart_read(address);
    //WRA< (Work RAM)
    if (address < 0xE000) return cart_read(address);
    //Echo RAM
    if (address < 0xFE00) return cart_read(address);
    if (address < 0xFEA0) return cart_read(address);
    if (address < 0xFF00) return cart_read(address);
    if (address < 0xFF80) return cart_read(address);
    if (address < 0xFFFF) return "HRAM";
    return "IE register";
}

#define BUS_TRACE_FMT "%04X: %7s (%02X)       -> %s\n"

u8 bus_read(u16 address){
    if(address < 0x8000){
        return cart_read(address);
    }
    u8 value = 0xFF; 
    printf(BUS_TRACE_FMT, address, "RD", value, bus_region(address));
    return value;
}

void bus_write(u16 address, u8 value){
    if(address < 0x8000){
        //ROM DATA
        cart_write(address, value);
        return;
    }
    printf(BUS_TRACE_FMT, address, "WR", value, bus_region(address));
}

u16 bus_read16(u16 address){
    u16 lo = bus_read(address);
    u16 hi = bus_read(address + 1);

    return lo | (hi << 8);
}

void bus_write16(u16 address, u16 value){
    bus_write(address, value & 0xFF);
    bus_write(address + 1, (value >> 8) & 0xFF);
}