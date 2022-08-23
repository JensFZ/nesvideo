#include <memory>
#include "olc2C02.h"
#include "Cartridge.h"

olc2C02::olc2C02()
{

    // NES Palettenfarben festlegen
    palScreen[0x00] = olc::Pixel(84, 84, 84);
    palScreen[0x01] = olc::Pixel(0, 30, 116);
    palScreen[0x02] = olc::Pixel(8, 16, 144);
    palScreen[0x03] = olc::Pixel(48, 0, 136);
    palScreen[0x04] = olc::Pixel(68, 0, 100);
    palScreen[0x05] = olc::Pixel(92, 0, 48);
    palScreen[0x06] = olc::Pixel(84, 4, 0);
    palScreen[0x07] = olc::Pixel(60, 24, 0);
    palScreen[0x08] = olc::Pixel(32, 42, 0);
    palScreen[0x09] = olc::Pixel(8, 58, 0);
    palScreen[0x0A] = olc::Pixel(0, 64, 0);
    palScreen[0x0B] = olc::Pixel(0, 60, 0);
    palScreen[0x0C] = olc::Pixel(0, 50, 60);
    palScreen[0x0D] = olc::Pixel(0, 0, 0);
    palScreen[0x0E] = olc::Pixel(0, 0, 0);
    palScreen[0x0F] = olc::Pixel(0, 0, 0);

    palScreen[0x10] = olc::Pixel(152, 150, 152);
    palScreen[0x11] = olc::Pixel(8, 76, 196);
    palScreen[0x12] = olc::Pixel(48, 50, 236);
    palScreen[0x13] = olc::Pixel(92, 30, 228);
    palScreen[0x14] = olc::Pixel(136, 20, 176);
    palScreen[0x15] = olc::Pixel(160, 20, 100);
    palScreen[0x16] = olc::Pixel(152, 34, 32);
    palScreen[0x17] = olc::Pixel(120, 60, 0);
    palScreen[0x18] = olc::Pixel(84, 90, 0);
    palScreen[0x19] = olc::Pixel(40, 114, 0);
    palScreen[0x1A] = olc::Pixel(8, 124, 0);
    palScreen[0x1B] = olc::Pixel(0, 118, 40);
    palScreen[0x1C] = olc::Pixel(0, 102, 120);
    palScreen[0x1D] = olc::Pixel(0, 0, 0);
    palScreen[0x1E] = olc::Pixel(0, 0, 0);
    palScreen[0x1F] = olc::Pixel(0, 0, 0);

    palScreen[0x20] = olc::Pixel(236, 238, 236);
    palScreen[0x21] = olc::Pixel(76, 154, 236);
    palScreen[0x22] = olc::Pixel(120, 124, 236);
    palScreen[0x23] = olc::Pixel(176, 98, 236);
    palScreen[0x24] = olc::Pixel(228, 84, 236);
    palScreen[0x25] = olc::Pixel(236, 88, 180);
    palScreen[0x26] = olc::Pixel(236, 106, 100);
    palScreen[0x27] = olc::Pixel(212, 136, 32);
    palScreen[0x28] = olc::Pixel(160, 170, 0);
    palScreen[0x29] = olc::Pixel(116, 196, 0);
    palScreen[0x2A] = olc::Pixel(76, 208, 32);
    palScreen[0x2B] = olc::Pixel(56, 204, 108);
    palScreen[0x2C] = olc::Pixel(56, 180, 204);
    palScreen[0x2D] = olc::Pixel(60, 60, 60);
    palScreen[0x2E] = olc::Pixel(0, 0, 0);
    palScreen[0x2F] = olc::Pixel(0, 0, 0);

    palScreen[0x30] = olc::Pixel(236, 238, 236);
    palScreen[0x31] = olc::Pixel(168, 204, 236);
    palScreen[0x32] = olc::Pixel(188, 188, 236);
    palScreen[0x33] = olc::Pixel(212, 178, 236);
    palScreen[0x34] = olc::Pixel(236, 174, 236);
    palScreen[0x35] = olc::Pixel(236, 174, 212);
    palScreen[0x36] = olc::Pixel(236, 180, 176);
    palScreen[0x37] = olc::Pixel(228, 196, 144);
    palScreen[0x38] = olc::Pixel(204, 210, 120);
    palScreen[0x39] = olc::Pixel(180, 222, 120);
    palScreen[0x3A] = olc::Pixel(168, 226, 144);
    palScreen[0x3B] = olc::Pixel(152, 226, 180);
    palScreen[0x3C] = olc::Pixel(160, 214, 228);
    palScreen[0x3D] = olc::Pixel(160, 162, 160);
    palScreen[0x3E] = olc::Pixel(0, 0, 0);
    palScreen[0x3F] = olc::Pixel(0, 0, 0);
}

olc2C02::~olc2C02()
{
}

uint8_t olc2C02::cpuRead(uint16_t addr, bool rdonly)
{
    uint8_t data = 0x00;

    switch (addr) {
        case 0x0000: // Control
            break;
        case 0x0001: // Mask
            break;
        case 0x0002: // Status
            data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F); // nur oberen 3 Bit des Statusregisters sind relevant, rest wird mit Noise aus dem ppu_data_buffer gefüllt
            status.vertical_blank = 0;
            address_latch = 0;
            break;
        case 0x0003: // OAM Adresse
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // PPU Address
            break;
        case 0x0007: // PPU Data
            data = ppu_data_buffer;
            ppu_data_buffer = ppuRead(vram_addr.reg);

            if (vram_addr.reg > 0x3F00) data = ppu_data_buffer;
            vram_addr.reg += (control.increment_mode ? 32 : 1); // Wenn incement_Mode -> Vertikal befüllen, wenn nicht Horizontal
            break;
    }

    return data;
}

void olc2C02::cpuWrite(uint16_t addr, uint8_t data)
{
    switch (addr) {
        case 0x0000: // Control
            control.reg = data;
            tram_addr.nametable_x = control.nametable_x;
            tram_addr.nametable_y = control.nametable_y;
            break;
        case 0x0001: // Mask
            mask.reg = data;
            break;
        case 0x0002: // Status
            //Status ist RO
            break;
        case 0x0003: // OAM Adresse
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            if (address_latch == 0) {
                fine_x = data & 0x07;
                tram_addr.coarse_x = data >> 3;
                address_latch = 1;
            } else {
                tram_addr.fine_y = data & 0x07;
                tram_addr.coarse_y = data >> 3;

                address_latch = 0;
            }
            break;
        case 0x0006: // PPU Address
            if (address_latch == 0) {
                tram_addr.reg = (tram_addr.reg & 0x00FF) | (data << 8);
                address_latch = 1;
            } else {
                tram_addr.reg = (tram_addr.reg & 0xFF00) | data;
                vram_addr = tram_addr;
                address_latch = 0;
            }
            break;
        case 0x0007: // PPU Data
            ppuWrite(vram_addr.reg, data);
            vram_addr.reg += (control.increment_mode ? 32 : 1); // Wenn incement_Mode -> Vertikal befüllen, wenn nicht Horizontal
            break;
    }
}

uint8_t olc2C02::ppuRead(uint16_t addr, bool rdonly)
{
    uint8_t data = 0x00;
    addr &= 0x3FFF; // Alles größer 0x3FFF wird ignoriert;

    // Cartridge ansprechen
    if (cart->ppuRead(addr, data)) {

    } else if (addr >= 0x0000 && addr <= 0x1FFF) { // Paletten Tables
        data = tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];
    } else if (addr >= 0x2000 && addr <= 0x3EFF) { // Nametables
        addr &= 0x0FFF;

        if (cart->mirror == Cartridge::MIRROR::VERTICAL)
        {
            // Vertical
            if (addr >= 0x0000 && addr <= 0x03FF)
                data = tblName[0][addr & 0x03FF];
            if (addr >= 0x0400 && addr <= 0x07FF)
                data = tblName[1][addr & 0x03FF];
            if (addr >= 0x0800 && addr <= 0x0BFF)
                data = tblName[0][addr & 0x03FF];
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                data = tblName[1][addr & 0x03FF];
        }
        else if (cart->mirror == Cartridge::MIRROR::HORIZONTAL)
        {
            // Horizontal
            if (addr >= 0x0000 && addr <= 0x03FF)
                data = tblName[0][addr & 0x03FF];
            if (addr >= 0x0400 && addr <= 0x07FF)
                data = tblName[0][addr & 0x03FF];
            if (addr >= 0x0800 && addr <= 0x0BFF)
                data = tblName[1][addr & 0x03FF];
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                data = tblName[1][addr & 0x03FF];
        }
    } else if (addr >= 0x3F00 && addr <= 0x3FFF) { // Paletten index
        addr &= 0x001F;
        if (addr == 0x0010) addr = 0x0000;
        if (addr == 0x0014) addr = 0x0004;
        if (addr == 0x0018) addr = 0x0008;
        if (addr == 0x001C) addr = 0x000C;

        data = tblPalette[addr];
    }

    return data;
}

void olc2C02::ppuWrite(uint16_t addr, uint8_t data)
{
    addr &= 0x3FFF; // Alles größer 0x3FFF wird ignoriert;
    // Cartridge ansprechen
    if (cart->ppuWrite(addr, data)) {

    } else if (addr >= 0x0000 && addr <= 0x1FFF) { // Paletten Tables
        tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
    } else if (addr >= 0x2000 && addr <= 0x3EFF) { // Nametables
        addr &= 0x0FFF;
        if (cart->mirror == Cartridge::MIRROR::VERTICAL)
        {
            // Vertical
            if (addr >= 0x0000 && addr <= 0x03FF)
                tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x0400 && addr <= 0x07FF)
                tblName[1][addr & 0x03FF] = data;
            if (addr >= 0x0800 && addr <= 0x0BFF)
                tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                tblName[1][addr & 0x03FF] = data;
        }
        else if (cart->mirror == Cartridge::MIRROR::HORIZONTAL)
        {
            // Horizontal
            if (addr >= 0x0000 && addr <= 0x03FF)
                tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x0400 && addr <= 0x07FF)
                tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x0800 && addr <= 0x0BFF)
                tblName[1][addr & 0x03FF] = data;
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                tblName[1][addr & 0x03FF] = data;
        }
    } else if (addr >= 0x3F00 && addr <= 0x3FFF) { // Paletten index
        addr &= 0x001F;
        if (addr == 0x0010) addr = 0x0000;
        if (addr == 0x0014) addr = 0x0004;
        if (addr == 0x0018) addr = 0x0008;
        if (addr == 0x001C) addr = 0x000C;

        tblPalette[addr] = data;
    }

}

void olc2C02::ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
    this->cart = cartridge; // Cartridge verbinden
}

void olc2C02::clock()
{

    auto IncrementScrollX = [&]()
    {
        if (mask.render_background || mask.render_sprites) // Nur ausführen, wenn auch was gerendet werden soll
        {
            // Eine Nametable ist 32x30 tiles groß. Wenn wir an das ende einer Nametable Zeile kommen, müssen wir in die nächste Nametable befüllen
            if (vram_addr.coarse_x == 31)
            {
                // Zurück zu 0
                vram_addr.coarse_x = 0;
                // Nächste Nametable
                vram_addr.nametable_x = ~vram_addr.nametable_x;
            }
            else
            {
                // Sind noch in der richtigen Nametable -> einfach +1
                vram_addr.coarse_x++;
            }
        }
    };

    // ==============================================================================
    // Increment the background tile "pointer" one scanline vertically
    auto IncrementScrollY = [&]()
    {
        // Incrementing vertically is more complicated. The visible nametable
        // is 32x30 tiles, but in memory there is enough room for 32x32 tiles.
        // The bottom two rows of tiles are in fact not tiles at all, they
        // contain the "attribute" information for the entire table. This is
        // information that describes which palettes are used for different 
        // regions of the nametable.

        // In addition, the NES doesnt scroll vertically in chunks of 8 pixels
        // i.e. the height of a tile, it can perform fine scrolling by using
        // the fine_y component of the register. This means an increment in Y
        // first adjusts the fine offset, but may need to adjust the whole
        // row offset, since fine_y is a value 0 to 7, and a row is 8 pixels high

        // Ony if rendering is enabled
        if (mask.render_background || mask.render_sprites)
        {
            // If possible, just increment the fine y offset
            if (vram_addr.fine_y < 7)
            {
                vram_addr.fine_y++;
            }
            else
            {
                // If we have gone beyond the height of a row, we need to
                // increment the row, potentially wrapping into neighbouring
                // vertical nametables. Dont forget however, the bottom two rows
                // do not contain tile information. The coarse y offset is used
                // to identify which row of the nametable we want, and the fine
                // y offset is the specific "scanline"

                // Reset fine y offset
                vram_addr.fine_y = 0;

                // Check if we need to swap vertical nametable targets
                if (vram_addr.coarse_y == 29)
                {
                    // We do, so reset coarse y offset
                    vram_addr.coarse_y = 0;
                    // And flip the target nametable bit
                    vram_addr.nametable_y = ~vram_addr.nametable_y;
                }
                else if (vram_addr.coarse_y == 31)
                {
                    // In case the pointer is in the attribute memory, we
                    // just wrap around the current nametable
                    vram_addr.coarse_y = 0;
                }
                else
                {
                    // None of the above boundary/wrapping conditions apply
                    // so just increment the coarse y offset
                    vram_addr.coarse_y++;
                }
            }
        }
    };

    // ==============================================================================
    // Transfer the temporarily stored horizontal nametable access information
    // into the "pointer". Note that fine x scrolling is not part of the "pointer"
    // addressing mechanism
    auto TransferAddressX = [&]()
    {
        // Ony if rendering is enabled
        if (mask.render_background || mask.render_sprites)
        {
            vram_addr.nametable_x = tram_addr.nametable_x;
            vram_addr.coarse_x = tram_addr.coarse_x;
        }
    };

    // ==============================================================================
    // Transfer the temporarily stored vertical nametable access information
    // into the "pointer". Note that fine y scrolling is part of the "pointer"
    // addressing mechanism
    auto TransferAddressY = [&]()
    {
        // Ony if rendering is enabled
        if (mask.render_background || mask.render_sprites)
        {
            vram_addr.fine_y = tram_addr.fine_y;
            vram_addr.nametable_y = tram_addr.nametable_y;
            vram_addr.coarse_y = tram_addr.coarse_y;
        }
    };

    auto LoadBackgroundShifters = [&]() {
        bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
        bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;

        bg_shifter_attrib_lo = (bg_shifter_attrib_lo & 0xFF00) | ((bg_next_tile_attrib & 0b01) ? 0xFF : 0x00);
        bg_shifter_attrib_hi = (bg_shifter_attrib_hi & 0xFF00) | ((bg_next_tile_attrib & 0b10) ? 0xFF : 0x00);
    };

    auto UpdateShifters = [&]() {
        if (mask.render_background) {
            bg_shifter_pattern_lo <<= 1;
            bg_shifter_pattern_hi <<= 1;

            bg_shifter_attrib_lo <<= 1;
            bg_shifter_attrib_hi <<= 1;
        }
    };

    if (scanline >= -1 && scanline < 240) {

        if (scanline == 0 && cycle == 0)
        {
            // "Odd Frame" cycle skip
            cycle = 1;
        }

        if (scanline == -1 && cycle == 1) {
            // Wenn "Kathodenstrahl" links oben -> Vertical_Blank = false
            status.vertical_blank = 0;
        }

        if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338)) {

            UpdateShifters();

            // Das alles ist sehr schwer zu lesen. bildet aber im großen und ganzen folgende Grafik nach: https://www.nesdev.org/w/images/default/4/4f/Ppu.svg
            switch ((cycle - 1) % 8) {
                case 0:
                    LoadBackgroundShifters();

                    bg_next_tile_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));
                    break;
                case 2:
                    bg_next_tile_attrib = ppuRead(0x23C0 | (vram_addr.nametable_y << 11)
                        | (vram_addr.nametable_x << 10)
                        | ((vram_addr.coarse_y >> 2) << 3)
                        | (vram_addr.coarse_x >> 2));

                    if (vram_addr.coarse_y & 0x02) bg_next_tile_attrib >>= 4;
                    if (vram_addr.coarse_x & 0x02) bg_next_tile_attrib >>= 2;
                    bg_next_tile_attrib &= 0x03;

                    break;
                case 4:
                    bg_next_tile_lsb = ppuRead((control.pattern_background << 12)
                        + ((uint16_t)bg_next_tile_id << 4)
                        + (vram_addr.fine_y) + 0);
                    break;
                case 6:
                    bg_next_tile_msb = ppuRead((control.pattern_background << 12)
                        + ((uint16_t)bg_next_tile_id << 4)
                        + (vram_addr.fine_y) + 8);

                    break;
                case 7:
                    break;
            }
        }

        if (cycle == 256) {

        }

    }

    if (scanline == 241 && cycle == 1) {
        // Wenn "Kathodenstrahl" rechts unten -> Vertical_Blank = true
        status.vertical_blank = 1;
        if (control.enable_nmi) {
            nmi = true;
        }
    }


    uint8_t bg_pixel = 0x00;
    uint8_t bg_palette = 0x00;

    if (mask.render_background) {
        uint16_t bit_mux = 0x8000 >> fine_x;
        
        uint8_t p0_pixel = (bg_shifter_pattern_lo & bit_mux) > 0;
        uint8_t p1_pixel = (bg_shifter_pattern_hi & bit_mux) > 0;

        bg_pixel = (p1_pixel << 1) | p0_pixel;

        uint8_t bg_pal0 = (bg_shifter_attrib_lo & bit_mux) > 0;
        uint8_t bg_pal1 = (bg_shifter_attrib_hi & bit_mux) > 0;
        bg_palette = (bg_pal1 << 1) | bg_pal0;  


    }



    // etwas rauschen zeichnen
    sprScreen.SetPixel(cycle - 1, scanline, GetColorFromPaletteRam(bg_palette, bg_pixel));

    cycle++;
    if (cycle >= 341) {
        cycle = 0;
        scanline++;
        if (scanline >= 261) {
            scanline = -1;
            frame_complete = true;
        }
    }
}

olc::Pixel& olc2C02::GetColorFromPaletteRam(uint8_t palette, uint8_t pixel)
{
    // Palettenmemory geht bei 0x3F00 los.
    return palScreen[ppuRead(0x3F00 + (palette << 2) + pixel) & 0x3F];
}

olc::Sprite& olc2C02::GetScreen()
{
    return sprScreen;
}

olc::Sprite& olc2C02::GetNameTable(uint8_t i)
{
    return sprNameTable[i];
}

olc::Sprite& olc2C02::GetPatternTable(uint8_t i, uint8_t palette)
{

    for (uint16_t nTileY = 0; nTileY < 16; nTileY++) {
        for (uint16_t nTileX = 0; nTileX < 16; nTileX++) {
            uint16_t nOffset = nTileY * 256 + nTileX * 16; // 2D koordinate in eine lineare Angabe

            for (uint16_t row = 0; row < 8; row++) {

                uint8_t tile_lsb = ppuRead(i * 0x1000 + nOffset + row + 0x0000);
                uint8_t tile_msb = ppuRead(i * 0x1000 + nOffset + row + 0x0008);

                for (uint16_t col = 0; col < 8; col++) {
                    // We can get the index value by simply adding the bits together
                    // but we're only interested in the lsb of the row words because...
                    uint8_t pixel = (tile_lsb & 0x01) + (tile_msb & 0x01);

                    // ...we will shift the row words 1 bit right for each column of
                    // the character.
                    tile_lsb >>= 1; tile_msb >>= 1;

                    // Now we know the location and NES pixel value for a specific location
                    // in the pattern table, we can translate that to a screen colour, and an
                    // (x,y) location in the sprite
                    sprPatternTable[i].SetPixel
                    (
                        nTileX * 8 + (7 - col), // Because we are using the lsb of the row word first
                        // we are effectively reading the row from right
                        // to left, so we need to draw the row "backwards"
                        nTileY * 8 + row,
                        GetColorFromPaletteRam(palette, pixel)
                    );
                }
            }
        }
    }

    return sprPatternTable[i];
}
