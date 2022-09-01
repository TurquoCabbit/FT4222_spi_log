#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>

#include <main.h>
#include <cwm_log.h>

#include <LibFT4222.h>
#include <ftd2xx.h>
#include <FT4222.h>

static void sig_handler(int);
//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------

int main(int argc, char const *argv[]) {
    
    signal(SIGINT, sig_handler);
    uint16 read_len, sizeOfRead;
    char spi_rx_buffer[65536];
    memset(spi_rx_buffer, 0, sizeof(spi_rx_buffer));

    cwm_log_init();

    CWM_OS_dbgPrintf("Version : %s\nbuild time: %s %s\n\n", version, __DATE__, __TIME__);

    if (FT4222_init()) {
        system("pause");
        return 0;
    }

    for(;;) {
        read_len = 0;
        FT4222_spi_get_rxSize(&read_len);
        if (read_len) {
            FT4222_spi_read(spi_rx_buffer, read_len, &sizeOfRead);
            CWM_OS_dbgOutput(spi_rx_buffer);
            memset(spi_rx_buffer, 0, sizeOfRead);
        }
    }

    return 0;
}

static void sig_handler(int sig) {
    switch(sig) {
        case SIGINT:
            cwm_log_close();
            exit(0);
            break;
    }
}