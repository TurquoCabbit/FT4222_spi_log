#ifndef __FT222_H__
#define __FT222_H__

int FT4222_init(void);
int FT4222_unint_and_close(void);
int FT4222_i2c_read(uint8_t, uint8_t, uint8_t *, uint16_t);
int FT4222_i2c_write(uint8_t, uint8_t, uint8_t *, uint16_t);
int FT4222_spi_get_rxSize(uint16 *);



#endif //__FT222_H__