//
// allocates SRAM for, and copies a USB descriptor from PROGMEM
// If the pointer (*sram) is not null, nothing happens.
// This provides a cache type behavior.
//
void __attribute__((noinline)) CacheDescriptor( uint8_t **sram, uint16_t progPtr )
{
    if (*sram) return;
    uint16_t prog = pgm_read_word(progPtr); // read pointer to descriptor from PROGMEM
    if (prog == 0x0000u || prog == 0xffffu) return; // not a valid pointer

    uint8_t cnt = pgm_read_byte( prog );    // 1st byte of descriptor is always length
    *sram = (uint8_t *)malloc(cnt);
    if (! *sram) return;
    uint8_t *dest = *sram;
    while (cnt--) *dest++ = pgm_read_byte( prog++ );
}
//
// overrides the weak function defined in USBCore.cpp, returning the serial number descriptor
// stored in Katiana flash (if it's there).
// If buf is returned null, no descriptor is available,
// otherwise buf points to a string descriptor in SRAM.
// Return value is non-zero if the buffer was malloc'ed and needs to be
// freed by the caller.
//
uint8_t USB_GetCustomSerialNumber(uint8_t **buf)
{
    if (!buf) return 0;
    *buf = 0;
    CacheDescriptor(buf, FLASHEND - 5);
    if (*buf)
    {
        // first byte is length of entire descriptor.
        // that's followed by a type byte (0x03 means uincode string),
        // and that's followed by the unicode data. 
        uint8_t n = (*buf)[0];
        (*buf)[n-2] ^= 0x01; // flip LSB of low byte of last Unicode char in S/N.
    }
    return 1;   // indicates that caller must free memory when finished with it.
}
//
// overrides weak function in USBCore.cpp an returns USB device descriptor from
// bootloader flash (if available). See remarks above for arguments and return value.
//
uint8_t USB_GetCustomDescriptor(uint8_t **buf)
{
    if (!buf) return 0;
    *buf = 0;
    CacheDescriptor(buf, FLASHEND - 7);
    if (*buf)
    {
        // increment PID by one (or could toggle if desired).
        ((DeviceDescriptor *)buf)->idProduct += 1;
    }
    return 1;   // indicates that caller must free memory when finished with it.
}
