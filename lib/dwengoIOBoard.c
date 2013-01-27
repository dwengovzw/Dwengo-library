#include "dwengoI2C.h"
#include "dwengoIOBoard.h"

void initIOBoard(void) {
    unsigned char address;
    
    initI2C();
    
    for (address=0; address<4; address++) {
        // Configure PCA9555
        write2I2C(IO_ADDR|(address<<1), 3 /* output1 */, 0);
        write2I2C(IO_ADDR|(address<<1), 7 /* config1 */, 0 /* output */);
        write2I2C(IO_ADDR|(address<<1), 4 /* polarity0 */, 0xff /* invert all inputs */);
    }
}

unsigned char readInputs_addr(unsigned char address) {
    if (address < 4) {
        write1I2C(IO_ADDR|(address<<1), 0 /* input0 */);
        return read1I2C(IO_ADDR);
    }else{
        return 0;
    }
}

void setOutputs_addr(unsigned char address, unsigned char out) {
    if (address < 4) {
        write2I2C(IO_ADDR|(address<<1), 3 /* output1 */, out);
    }
}

unsigned char readInputs() {
    return readInputs_addr(0);
}

void setOutputs(unsigned char out) {
    setOutputs_addr(0, out);
}