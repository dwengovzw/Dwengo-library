
#include <stdio.h>
#include <string.h>
#include "Config/AppConfig.h"

#if defined( CUSTOM_USB_SERIAL )

#include "UsbHdwrSerial.h"

const unsigned char serial[] = USB_HDWR_SERIAL;

#if defined( STRICT_USB_SERIAL )

int main()
{
    int rc = 0;
    int n = strlen(serial);
    
    printf("\n---------- Validating USB Serial Number (Strict) ----------\n\n");

    if (n < 12)
    {
        printf("Error: USB serial number length is less than 12.\n");
        rc |= 1;
    }
    
    if (n > 126)
    {
        printf("Error: USB serial number length is greater than 126.\n");
        rc |= 2;
    }

    for (int k=0; k<n; k++)
    {
        if (serial[k] < '0' || serial[k] > 'F' || (serial[k] > '9' && serial[k] < 'A'))
        {
            printf("Error: USB serial number string contains invalid character(s).\n");
            printf("       Only upper case hexadecimal charcters (0-9,A-F) are allowed.\n");
            rc |= 4;
            break;
        }
    }

    if (rc) printf("       See ValidateUsbHdwrSerial.c for details.\n");

    printf( rc ? "\n**********      USB Serial Number NOT Valid     **********\n\n" :
                   "----------      USB Serial Number is  Valid     ----------\n\n");

    return rc;
}

#else

int main()
{
    int rc = 0;
    int n = strlen(serial);
    
    printf("\n---------- Validating USB Serial Number ----------\n\n");

    if (n < 1)
    {
        printf("Error: USB serial number length is less than 1.\n");
        rc |= 1;
    }
    
    if (n > 126)
    {
        printf("Error: USB serial number length is greater than 126.\n");
        rc |= 2;
    }

    for (int k=0; k<n; k++)
    {
        if (serial[k] < 0x20 || serial[k] > 0x7E)
        {
            printf("Error: USB serial number string contains invalid character(s).\n");
            printf("       Only charcters from 0x20 to 0x7E inclusive are allowed.\n");
            rc |= 4;
            break;
        }
    }

    if (rc) printf("       See ValidateUsbHdwrSerial.c for details.\n");

    printf( rc ? "\n**********  USB Serial Number NOT Valid **********\n\n" :
                   "----------  USB Serial Number is  Valid ----------\n\n");

    return rc;
}

#endif

#else

int main() { return 0; }

#endif
