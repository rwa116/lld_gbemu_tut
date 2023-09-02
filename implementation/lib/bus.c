#include <bus.h>
#include <cart.h>
#include <ram.h>
#include <cpu.h>

// 0000	3FFF	16 KiB ROM bank 00	From cartridge, usually a fixed bank
// 4000	7FFF	16 KiB ROM Bank 01~NN	From cartridge, switchable bank via mapper (if any)
// 8000	9FFF	8 KiB Video RAM (VRAM)	In CGB mode, switchable bank 0/1
// A000	BFFF	8 KiB External RAM	From cartridge, switchable bank if any
// C000	CFFF	4 KiB Work RAM (WRAM)	
// D000	DFFF	4 KiB Work RAM (WRAM)	In CGB mode, switchable bank 1~7
// E000	FDFF	Mirror of C000~DDFF (ECHO RAM)	Nintendo says use of this area is prohibited.
// FE00	FE9F	Object attribute memory (OAM)	
// FEA0	FEFF	Not Usable	Nintendo says use of this area is prohibited
// FF00	FF7F	I/O Registers	
// FF80	FFFE	High RAM (HRAM)	
// FFFF	FFFF	Interrupt Enable register (IE)

u8 bus_read(u16 address) {
    if(address < 0x8000) {
        //ROM Data
        return cart_read(address);
    }
    else if(address < 0xA000) {
        //Char/map data
        //TODO ...
        printf("UNSUPPORTED bus read(%04X)\n", address);
        NO_IMPL
    }
    else if(address < 0xC000) {
        //Cartridge RAM
        return cart_read(address);
    }
    else if(address < 0xE000) {
        //Working RAM
        return wram_read(address);
    }
    else if(address < 0xFE00) {
        //Reserved echo ram
        return 0;
    }
    else if(address < 0xFEA0) {
        //OAM
        //TODO ...
        printf("UNSUPPORTED bus read(%04X)\n", address);
        return 0; //NO_IMPL
    }
    else if(address < 0xFF00) {
        //Reserved
        return 0x0;
    }
    else if(address < 0xFF80) {
        //IO Registers
        //TODO ...
        printf("UNSUPPORTED bus read(%04X)\n", address);
        return 0x0; //NO_IMPL
    }
    else if(address == 0xFFFF) {
        //CPU Enable Register
        return cpu_get_ie_register();
    }

    return hram_read(address); //0xFF80 - 0xFFFE
}

void bus_write(u16 address, u8 value) {
    if(address < 0x8000) {
        //ROM Data
        cart_write(address, value);
    } 
    else if(address < 0xA000) {
        //Char/map data
        //TODO ...
        printf("UNSUPPORTED bus write(%04X)\n", address);
        //NO_IMPL
    }
    else if(address < 0xC000) {
        //Cartridge RAM
        cart_write(address, value);
    }
    else if(address < 0xE000) {
        //Working RAM
        wram_write(address, value);
    }
    else if(address < 0xFE00) {
        //Reserved echo ram
    }
    else if(address < 0xFEA0) {
        //OAM
        //TODO ...
        printf("UNSUPPORTED bus write(%04X)\n", address);
        //NO_IMPL
    }
    else if(address < 0xFF00) {
        //Reserved
    }
    else if(address < 0xFF80) {
        //IO Registers
        //TODO ...
        printf("UNSUPPORTED bus write(%04X)\n", address);
        //NO_IMPL
    }
    else if(address == 0xFFFF) {
        //CPU Enable Register
        cpu_set_ie_register(value);
    }
    else {
        hram_write(address, value);
    }
}

u16 bus_read16(u16 address) {
    u16 lo = bus_read(address);
    u16 hi = bus_read(address + 1);

    return lo | (hi << 8);
}

void bus_write16(u16 address, u16 value) {
    bus_write(address + 1, (value >> 8) & 0xFF);
    bus_write(address, value & 0xFF);
}