#ifndef __SPI_HELPER__
#define __SPI_HELPER__

//define actions for spi
#define XVBAT                              0x01
#define XRPM                               0x02
#define XOSCOPE                            0x03
#define XGET_BOOT_STATUS                   0x04
#define XSET_THRESHOLD                     0x05
#define XBOOT_ENTER                        0xE0
#define XBOOT_ERASE_ALL_FLASH              0xE1  //64k code + 4k app table
#define XBOOT_ERASE_64K_FLASH              0xE2
#define XBOOT_ERASE_APP_TABLE_FLASH        0xE3
#define XBOOT_SEND_ADDRESS                 0xE4
#define XBOOT_SEND_128FLASH_bytes          0xE5
#define XBOOT_END_MODE                     0xFE

//
#define XMEGA_MODE_NORMAL                  0x10
#define XMEGA_MODE_BOOT                    0x11

void slave_enable(void);
void slave_disable(void);
void SPI_initialize_internal(void);
void spi1_send_frame(unsigned char action, unsigned char param1, unsigned char param2);
//void spi_enter_boot_mode();
unsigned char spi1_recieve_frame(void);
unsigned char get_boot_status(void);
unsigned char set_comparator_threshold(unsigned char scale_factor);
unsigned char write_flash(unsigned char *boot_code_buffer, unsigned int length);
unsigned char program_application_using_bootloader(unsigned char *boot_code_buffer,unsigned int length);
//unsigned char program_in_bootmode_from_file(void)

#endif  
