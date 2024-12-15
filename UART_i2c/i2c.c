/*
#include "uart.h"
#include <string.h>
#include "i2c_header.h"
#include "eeprom_header.h"

#define EEPROM_SA 0x50
#define SW1 14
#define SW2 15
#define SW3 16

unsigned char set[10];
unsigned char reset[10] = "0000";
unsigned char check[10];


// Function to set the password
void set_password(void) {
    uart0_tx_str("Enter the password to be set:\r\n");
    uart0_rx_str(set, sizeof(set));
	uart0_tx_str(set);
	uart0_tx_str("\r\n");
    i2c_eeprom_page_write(EEPROM_SA, 0x00, set, 5);
    uart0_tx_str("The Password is set!!!\r\n");
}

// Function to check the password
void check_password(void) {
    uart0_tx_str("Enter the password to check:\r\n");
    uart0_rx_str(check, sizeof(check));
	uart0_tx_str(check);
	uart0_tx_str("\r\n");
    i2c_eeprom_seq_read(EEPROM_SA, 0x00, set, 5);

    if (strcmp((const char *)set, (const char *)check) == 0) {
        uart0_tx_str("The password is correct!!!\r\n");
        uart0_tx_str("Door Unlocked\r\n");
    } else {
        uart0_tx_str("The password is incorrect!!!\r\n");
    }
}

// Function to reset the password
void reset_password(void) {
    uart0_tx_str("Enter the old password:\r\n");
    uart0_rx_str(check, sizeof(check));
	uart0_tx_str(check);
	uart0_tx_str("\r\n");
    i2c_eeprom_seq_read(EEPROM_SA, 0x00, set, 5);

    if (strcmp((const char *)set, (const char *)check) == 0) {
        i2c_eeprom_page_write(EEPROM_SA, 0x00, reset, 5);
        uart0_tx_str("The password is successfully reset!!!\r\n");
        uart0_tx_str("Please set the new password!!!\r\n");
    } else {
        uart0_tx_str("The password is incorrect!!!\r\n");
    }
}

// Main function
int main() {
    int flag = 1;
    LCD_init();
    uart0_config();
    i2c_init();
    uart0_tx_str("1. Set the password  2. Reset the password  3. Check the password\r\n");

    while (1) {
        if (((IOPIN0 >> SW1) & 1) == 0) {
            delay_ms(150); // Debounce
            if (((IOPIN0 >> SW1) & 1) == 0) { // Confirm button press
                if (flag) {
                    uart_clear_buffer(); // Clear residual data before input
                    set_password();
                    flag = 0;
                } else {
                    uart0_tx_str("Password is already set!!!\r\n");
                }
            }
        }

        if (((IOPIN0 >> SW2) & 1) == 0) {
            delay_ms(150); // Debounce
            if (((IOPIN0 >> SW2) & 1) == 0) {
                if (!flag) {
                    uart_clear_buffer(); // Clear residual data before input
                    reset_password();
                    flag = 1;
                } else {
                    uart0_tx_str("Set the password first!!!\r\n");
                }
            }
        }

        if (((IOPIN0 >> SW3) & 1) == 0) {
            delay_ms(150); // Debounce
            if (((IOPIN0 >> SW3) & 1) == 0) {
                uart_clear_buffer(); // Clear residual data before input
				if(flag==1)
				{
					uart0_tx_str("Set the password first!!!\r\n");
				}
				else
                check_password();
            }
        }
    }
}
*/
#include "uart.h"
#include <string.h>
#include "i2c_header.h"
#include "eeprom_header.h"

#define EEPROM_SA 0x50
#define SW1 14
#define SW2 15
#define SW3 16

unsigned char set[10];
unsigned char reset[10] = "0000";
unsigned char check[10];

// Function to set the password
void set_password(void) {
    uart0_tx_str("Enter the password to be set:\r\n");
    uart0_rx_str(set, sizeof(set));
    uart0_tx_str(set);
    uart0_tx_str("\r\n");
    i2c_eeprom_page_write(EEPROM_SA, 0x00, set, 5);
    uart0_tx_str("The Password is set!!!\r\n");
}

// Function to check the password
void check_password(void) {
    uart0_tx_str("Enter the password to check:\r\n");
    uart0_rx_str(check, sizeof(check));
    uart0_tx_str(check);
    uart0_tx_str("\r\n");
    i2c_eeprom_seq_read(EEPROM_SA, 0x00, set, 5);

    if (strcmp((const char *)set, (const char *)check) == 0) {
        uart0_tx_str("The password is correct!!!\r\n");
        uart0_tx_str("Door Unlocked\r\n");
    } else {
        uart0_tx_str("The password is incorrect!!!\r\n");
    }
}

// Function to reset the password
void reset_password(void) {
    uart0_tx_str("Enter the old password:\r\n");
    uart0_rx_str(check, sizeof(check));
    uart0_tx_str(check);
    uart0_tx_str("\r\n");
    i2c_eeprom_seq_read(EEPROM_SA, 0x00, set, 5);

    if (strcmp((const char *)set, (const char *)check) == 0) {
        i2c_eeprom_page_write(EEPROM_SA, 0x00, reset, 5);
        uart0_tx_str("The password is successfully reset!!!\r\n");
        uart0_tx_str("Please set the new password!!!\r\n");
        set_password(); // Prompt to set the new password immediately
    } else {
        uart0_tx_str("The password is incorrect!!!\r\n");
    }
}

// Main function
int main() {
    int password_set_flag = 0;
    LCD_init();
    uart0_config();
    i2c_init();
    uart0_tx_str("1. Set the password  2. Reset the password  3. Check the password\r\n");

    while (1) {
        if (((IOPIN0 >> SW1) & 1) == 0) {
            delay_ms(150); // Debounce
            if (((IOPIN0 >> SW1) & 1) == 0) { // Confirm button press
                if (!password_set_flag) {
                    uart_clear_buffer(); // Clear residual data before input
                    set_password();
                    password_set_flag = 1;
                } else {
                    uart0_tx_str("Password is already set!!!\r\n");
                }
            }
        }

        if (((IOPIN0 >> SW2) & 1) == 0) {
            delay_ms(150); // Debounce
            if (((IOPIN0 >> SW2) & 1) == 0) {
                if (password_set_flag) {
                    uart_clear_buffer(); // Clear residual data before input
                    reset_password();
                    password_set_flag = 0; // Password needs to be set again after reset
                } else {
                    uart0_tx_str("Set the password first!!!\r\n");
                }
            }
        }

        if (((IOPIN0 >> SW3) & 1) == 0) {
            delay_ms(150); // Debounce
            if (((IOPIN0 >> SW3) & 1) == 0) {
                uart_clear_buffer(); // Clear residual data before input
                if (password_set_flag) {
                    check_password();
                } else {
                    uart0_tx_str("Set the password first!!!\r\n");
                }
            }
        }
    }
}
