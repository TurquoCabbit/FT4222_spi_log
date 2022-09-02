#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <ftd2xx.h>
#include <LibFT4222.h>

#include <FT4222.h>
#include <cwm_log.h>

static FT_DEVICE_LIST_INFO_NODE g_FT4222Dev;
static FT_HANDLE ftHandle = NULL;

//------------------------------------------------------------------------------
static int ListFtUsbDevices(void) {
    FT_STATUS ftStatus = 0;
    DWORD numOfDevices = 0;
    ftStatus = FT_CreateDeviceInfoList(&numOfDevices);

    for(DWORD iDev=0; iDev<numOfDevices; ++iDev)
    {
        FT_DEVICE_LIST_INFO_NODE devInfo;
        memset(&devInfo, 0, sizeof(devInfo));

        ftStatus = FT_GetDeviceInfoDetail(iDev, &devInfo.Flags, &devInfo.Type,
                                        &devInfo.ID, &devInfo.LocId,
                                        devInfo.SerialNumber,
                                        devInfo.Description,
                                        &devInfo.ftHandle);

        if (FT_OK == ftStatus)
        {
            const char * desc = devInfo.Description;
            if (!strcmp(desc, "FT4222") || !strcmp(desc, "FT4222 A")) {
                g_FT4222Dev = devInfo;
                return FT_OK;
            }
        }
    }
    return FT_DEVICE_NOT_FOUND;
}

static int FT4222_device_init(void) {
    
    if (ListFtUsbDevices()) {
        CWM_OS_dbgPrintf("Device not found!\n");
        printf("Device not found!\n");
        return FT_DEVICE_NOT_FOUND;
    }

    CWM_OS_dbgPrintf("Open Device\n");
    CWM_OS_dbgPrintf("  Flags= 0x%x, (%s), (%s)\n", g_FT4222Dev.Flags, (g_FT4222Dev.Flags & 0x01) ? "DEVICE_OPEN" : "DEVICE_CLOSE",\
                                                             (g_FT4222Dev.Flags & 0x02) ? "High-speed USB" : "Full-speed USB");
    CWM_OS_dbgPrintf("  Type= 0x%x\n",        g_FT4222Dev.Type);
    CWM_OS_dbgPrintf("  ID= 0x%x\n",          g_FT4222Dev.ID);
    CWM_OS_dbgPrintf("  LocId= 0x%x\n",       g_FT4222Dev.LocId);
    CWM_OS_dbgPrintf("  SerialNumber= %s\n",  g_FT4222Dev.SerialNumber);
    CWM_OS_dbgPrintf("  Description= %s\n",   g_FT4222Dev.Description);
    CWM_OS_dbgPrintf("  ftHandle= %#x\n",    g_FT4222Dev.ftHandle);

    FT_STATUS ftStatus;
    ftStatus = FT_OpenEx((PVOID)g_FT4222Dev.LocId, FT_OPEN_BY_LOCATION, &ftHandle);
    if (FT_OK != ftStatus) {
        CWM_OS_dbgPrintf("Open a FT4222 device failed!\n");
        return FT_DEVICE_NOT_OPENED;
    }
    return FT_OK;
}

static int FT4222_i2c_init(void) {    
    CWM_OS_dbgPrintf("\n\n");
    CWM_OS_dbgPrintf("Init FT4222 as I2C master\n");
    FT_STATUS ftStatus;
    ftStatus = FT4222_I2CMaster_Init(ftHandle, 400);
    if (FT_OK != ftStatus) {
        CWM_OS_dbgPrintf("Init FT4222 as I2C master device failed!\n");
        return ftStatus;
    }
    return FT_OK;
}

static int FT4222_spi_init(void) {    
    CWM_OS_dbgPrintf("\n\n");
    CWM_OS_dbgPrintf("Init FT4222 as SPI slave\n");
    FT_STATUS ftStatus;
    ftStatus = FT4222_SPISlave_InitEx(ftHandle, SPI_SLAVE_NO_PROTOCOL);
    if (FT_OK != ftStatus) {
        CWM_OS_dbgPrintf("Init FT4222 as SPI slave device failed!\n");
        return ftStatus;
    }
    return FT4222_SPISlave_SetMode(ftHandle, CLK_IDLE_HIGH, CLK_TRAILING);
}

/****************************************************************************************/

int FT4222_init(void) {
    int ret;

    ret = FT4222_device_init();
    if (ret)
        return ret;
    return FT4222_spi_init();
}

int FT4222_unint_and_close(void) {
    int ret;
    CWM_OS_dbgPrintf("UnInitialize FT4222\n");
    ret = FT4222_UnInitialize(ftHandle);
    if (ret)
        return ret;
    CWM_OS_dbgPrintf("Close FT device\n");
    return FT_Close(ftHandle);
}

int FT4222_i2c_read(uint8_t slaveAddr, uint8_t reg, uint8_t *data, uint16_t len) {
    uint16_t sizeTransferred = 0;
    int ret;

    // slaveAddr >>= 1;
    
	ret = FT4222_I2CMaster_WriteEx(ftHandle, slaveAddr, START, &reg, 1, &sizeTransferred);
    if (ret) {
        return ret;
    }

    return FT4222_I2CMaster_ReadEx(ftHandle, slaveAddr, Repeated_START | STOP, data, len, &sizeTransferred);
}


int FT4222_i2c_write(uint8_t slaveAddr, uint8_t reg, uint8_t *data, uint16_t len) {
    uint16_t sizeTransferred = 0;
    int ret;

    // slaveAddr >>= 1;
    
	ret = FT4222_I2CMaster_WriteEx(ftHandle, slaveAddr, START, &reg, 1, &sizeTransferred);
    if (ret) {
        return ret;
    }

	return FT4222_I2CMaster_WriteEx(ftHandle, slaveAddr, STOP | STOP, data, len, &sizeTransferred);
}

int FT4222_spi_get_rxSize(uint16 *rxSize) {
    uint16 size;
    int ret;

    size = 0;

    ret = FT4222_SPISlave_GetRxStatus(ftHandle, &size);

    if (ret)
        return ret;
    
    *rxSize = size;

    return FT_OK;
}


int FT4222_spi_read(char *pBuffer, uint16 len, uint16 *sizeOfRead) {

    *sizeOfRead = 0;
    
    return FT4222_SPISlave_Read(ftHandle, (uint8_t *)pBuffer, len, sizeOfRead);
}