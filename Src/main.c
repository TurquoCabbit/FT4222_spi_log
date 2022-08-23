#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <main.h>

#include <LibFT4222.h>
#include <ftd2xx.h>
#include <FT4222.h>

void log_init(void) {
#ifdef log_file
    struct stat info;

    if (stat(log_folder, &info) != 0 || !(info.st_mode & S_IFDIR)) {
        mkdir(log_folder);
    }
#endif
}

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------

int main(int argc, char const *argv[]) {
    uint16 read_len;
    char spi_rx_buffer[2048];

    log_init();

    printf("Version : %s\nbuild time: %s %s\n\n", version, __DATE__, __TIME__);

    if (FT4222_init()) {
        system("pause");
        return 0;
    }

    for(;;) {
        FT4222_spi_get_rxSize(&read_len);
        if (read_len) {
            FT4222_spi_read(spi_rx_buffer, read_len);
            printf("%s", spi_rx_buffer);
        }
        read_len = 0;
        memset(spi_rx_buffer, 0, sizeof(spi_rx_buffer));
    }

    return 0;
}

