/**
 * @file hardware.h
 * @author Juan J. Rojas
 * @date 10 Nov 2018
 * @brief Definitions for the BDC prototype controller
 * @par Institution:
 * LaSEINE / CeNT. Kyushu Institute of Technology.
 * @par Mail (after leaving Kyutech):
 * juan.rojas@tec.ac.cr
 * @par Git repository:
 * https://bitbucket.org/juanjorojash/bdc_prototype/src/master
 */

#ifndef HARDWARE_H
    #define HARDWARE_H
    // PIC16F1786 Configuration Bit Settings

    // 'C' source line config statements
    #pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
    #pragma config WDTE = OFF       // Watchdog Timer Disabled (WDT disabled)
    #pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
    #pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)//If this is enabled, the Timer0 module will not work properly.
    #pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
    #pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
    #pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset enabled)
    #pragma config CLKOUTEN = ON    // Clock Out Negative Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
    #pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
    #pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

    // CONFIG2
    #pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
    #pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable bit (Vcap functionality is disabled on RA6.)
    #pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
    #pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
    #pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
    #pragma config LPBOR = OFF      // Low Power Brown-Out Reset Enable Bit (Low power brown-out is disabled)
    #pragma config LVP = OFF        // Low-Voltage Programming Enable (Low-voltage programming disabled)//IF THIS IN ON MCLR is always enabled

    #include <xc.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    #include <stdint.h> // To include uint8_t and uint16_t
    #include <string.h>
    #include <stdbool.h> // Include bool type

#define 	_XTAL_FREQ 				32000000
#define		ERR_MAX					1000
#define		ERR_MIN					-1000
#define 	DC_MIN                  103 // DC = 0.2 MINIMUM
#define 	DC_MAX                  461	// DC = 0.9 MAX
#define     KP                      15 ///< Proportional constant divider 
#define     KI                      35 ///< Integral constant divider 
#define     VREF                    4800                  
#define     sVREF                   (uint16_t) ( ( ( VREF * 4096.0 ) / 5000 ) + 0.5 )
#define     CREF                    2000                  
#define     sCREF                   (uint16_t) ( ( ( CREF * 4096.0 ) / (5000 * 2.5 * 5 ) ) + 0.5 )
#define     VOC                     5400
#define     sVOC                    (uint16_t) ( ( ( VOC * 4096.0 ) / 5000 ) + 0.5 )
#define     VBATMIN                 2500
#define     sVBATMIN                (uint16_t) ( ( ( VBATMIN * 4096.0 ) / 5000 ) + 0.5 )
#define     VBATMAX                 4150
#define     sVBATMAX                (uint16_t) ( ( ( VBATMAX * 4096.0 ) / 5000 ) + 0.5 )


#define     COUNTER                 128
#define     I_PDU_50V               0b01011 //AN11 (RB4)
#define     V_PDU_50V               0b01001 //AN9 (RB3)
#define     I_PDU_33V               0b01010 //AN10 (RB1)
#define     V_PDU_33V               0b01100 //AN12 (RB0)
#define		I_LOAD                  0b00010 //AN2 (RA2) 
#define		V_BUS                   0b00001 //AN1 (RA1)  
#define		I_PV                    0b00000 //AN0 (RA0)  

bool                                mppt = 0;
float                               power = 0;
char                                dir = 0x07; //0x06 increase 0x07 decrease 0x08 mantain
// ADC values
uint16_t                            vpv = 0;
uint16_t                            ipv = 0;
uint16_t                            ilo = 0;
uint16_t                            v50 = 0;
uint16_t                            i50 = 0;
uint16_t                            v33 = 0;
uint16_t                            i33 = 0;
// Acumulators
uint24_t                            vpvac = 0;
uint24_t                            ipvac = 0;
uint24_t                            iloac = 0;
uint24_t                            v50ac = 0;
uint24_t                            i50ac = 0;
uint24_t                            v33ac = 0;
uint24_t                            i33ac = 0;
// Averages
uint16_t                            vpvav = 0;
uint16_t                            ipvav = 0;
uint16_t                            iloav = 0;
uint16_t                            v50av = 0;
uint16_t                            i50av = 0;
uint16_t                            v33av = 0;
uint16_t                            i33av = 0;


bool EMSF = 0;
uint16_t count8 = 8; 
///END OF TEST
bool                                SECF = 0;
uint16_t                            count = COUNTER + 1; ///< Counter that should be cleared every second. Initialized as #COUNTER 
bool                                log_on = 0; ///< Variable to indicate if the log is activated 
int16_t                             second = -1; ///< Seconds counter, resetted after 59 seconds.
uint16_t                            minute = 0; ///< Minutes counter, only manually reset
uint16_t                            capap = 0;
uint16_t                            cvref = 4800;
uint16_t                            ocref = 2000;
uint16_t                            vbusr = 0;    
uint16_t                            ivbusr = 0; 
uint16_t                            iref = 0;
uint16_t                            voc = 0;
uint16_t                            vbatmin = 0;
uint16_t                            vbatmax = 0;
        
void initialize(void);
uint16_t read_ADC(uint16_t channel);
void log_control_hex(void);
void display_value_u(uint16_t value);
void display_value_s(int16_t value);
void calculate_avg(void);
void interrupt_enable(void);
void UART_send_char(char bt);
char UART_get_char(void); 
void UART_send_string(const char* st_pt);
void UART_send_u16(uint16_t number); 
void timing(void);
void PAO(uint16_t pv_voltage, uint16_t pv_current, float* previous_power, char* previous_direction);


#define     LINEBREAK               {UART_send_char(0xA); UART_send_char(0xD);}
#define     HEADER                  {UART_send_char(0x01); UART_send_char(0x02);}
#define     FOOTER                  {UART_send_char(0x02); UART_send_char(0x01);}
#define  	DIRECTION(x)		    {UART_send_char(0x01); UART_send_char(0x03); UART_send_char(x);UART_send_char(0x03); UART_send_char(0x01);}
#define  	START		            {UART_send_char(0x01); UART_send_char(0x03); UART_send_char(0x04); UART_send_char(0x03); UART_send_char(0x01);}
#define  	STOP		            {UART_send_char(0x01); UART_send_char(0x03); UART_send_char(0x05); UART_send_char(0x03); UART_send_char(0x01);}
#define     RESET_TIME()            { minute = 0; second = -1; } ///< Reset timers.

#endif /* HARDWARE_H*/