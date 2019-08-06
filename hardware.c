/**
 * @file harware.c
 * @author Juan J. Rojas
 * @date 10 Nov 2018
 * @brief function definitions for the BDC prototype controller.
 * @par Institution:
 * LaSEINE / CeNT. Kyushu Institute of Technology.
 * @par Mail (after leaving Kyutech):
 * juan.rojas@tec.ac.cr
 * @par Git repository:
 * https://bitbucket.org/juanjorojash/bdc_prototype/src/master
 */

#include "hardware.h"

/**@brief Function to define initialize the system
*/
void initialize()
{
    /** @b GENERAL*/
    CLRWDT(); /// * Clear WDT by calling @p CLRWDT()
    nWPUEN = 0; /// * Allow change of individual WPU
    WPUE3 = 1;      //Enable pull up for MCLR
    /** @b SYSTEM @b CLOCK*/
    /** PLL is always enabled because of configuration bits*/
    OSCCONbits.IRCF = 0b1111; /// * Set clock to 32MHz (with PLL)
    OSCCONbits.SCS = 0b00; /// * Clock determined by FOSC<2:0> in Configuration Words
    OSCCONbits.SPLLEN = 1; /// * Enable PLL. According to Errata this shall not be done in the Configuration Words
    /** @b TIMER 1 for control and measuring loop using interruption*/
    /* Preload TMR1 register pair for 1us overflow*/
    /* T1OSCEN = 1, nT1SYNC = 1, TMR1CS = 0 and TMR1ON = 1*/
    nT1SYNC = 0;     //Synchronized
    T1OSCEN = 0;
    TMR1ON = 0;       //ON
    TMR1GE = 0;      //Dont care about gate
    TMR1CS0 = 0;       
    TMR1CS1 = 0;    //FOSC/4
    T1CKPS0 = 1;
    T1CKPS1 = 1;    // 8 milliseconds
    TMR1H = 0xE1;   //TMR1 Fosc/4= 8Mhz (Tosc= 0.125us)
    TMR1L = 0x83;   //TMR1 counts: 7805 x 0.125us = 0.97562ms
    /** @b ADC*/
    /** ADC INPUTS*///check this after final design
    TRISA0 = 1; /// * RA0
    ANSA0 = 1; /// * RA0 analog
    WPUA0 = 0; /// * RA0 weak pull up deactivated
    TRISA1 = 1; /// * RA1
    ANSA1 = 1; /// * RA1 analog
    WPUA1 = 0; /// * RA1 weak pull up deactivated
    TRISA2 = 1; /// * RA2
    ANSA2 = 1; /// * RA2 analog
    WPUA2 = 0; /// * RA2 weak pull up deactivated
    TRISB0 = 1; /// * RB0
    ANSB0 = 1; /// * RB0 analog
    WPUB0 = 0; /// * RB0 weak pull up deactivated
    TRISB1 = 1; /// * RB1
    ANSB1 = 1; /// * RB1 analog
    WPUB1 = 0; /// * RB1 weak pull up deactivated
    TRISB3 = 1; /// * RB3
    ANSB3 = 1; /// * RB3 analog
    WPUB3 = 0; /// * RB3 weak pull up deactivated
    TRISB4 = 1; /// * RB4
    ANSB4 = 1; /// * RB4 analog
    WPUB4 = 0; /// * RB4 weak pull up deactivated
    /** Configs*/
    ADCON0bits.ADRMD = 0; /// * 12 bits result
    ADCON1bits.ADCS = 0b010; /// * Clock selected as FOSC/32
    ADCON1bits.ADNREF = 0; /// * Connected to Vss
    //THIS IS THE CORRECT //ADCON1bits.ADPREF = 0b01; /// * Connected to Vref+
    ADCON1bits.ADPREF = 0b00; /// * Connected to VDD
    ADCON1bits.ADFM = 1; /// * 2's compliment result
    ADCON2bits.CHSN = 0b1111; /// * Negative differential input as ADNREF
    ADCON0bits.ADON = 1; /// * Turn on the ADC
    /** @b UART*/
    //**Setting I/O pins for UART*/
    TXSEL = 0;      /// * RC6 selected as TX
    RXSEL = 0;      /// * RC7 selected as RX
    //________I/O pins set __________//
    
    /**Initialize SPBRG register for required 
    baud rate and set BRGH for fast baud_rate**/
    SP1BRGH = 0x00; 
    SP1BRGL = 0x8A;    
    BRGH  = 1;  /// * for high baud_rate
    BRG16 = 1;  /// * for 16 bits timer
    //_________End of baud_rate setting_________//
    
    //****Enable Asynchronous serial port*******//
    SYNC  = 0;    /// * Asynchronous
    SPEN  = 1;    /// * Enable serial port pins
    //_____Asynchronous serial port enabled_______//
    //**Lets prepare for transmission & reception**//
    TXEN  = 1;    /// * enable transmission
    CREN  = 1;    /// * enable reception
    //__UART module up and ready for transmission and reception__//
    //**Select 8-bit mode**//  
    TX9   = 0;    /// * 8-bit reception selected
    RX9   = 0;    /// * 8-bit reception mode selected
    //__8-bit mode selected__/
    RCIE = 0; /// * Disable UART reception interrupts
    TXIE = 0; /// * Disable UART transmission interrupts
}

/**@brief This function takes care of printing the test data using the UART
*/
void log_control_hex()
{
/**The code in this function is only executed if the #log_on variable is set*/
/**This function takes care of sending the logging data in pieces to avoid disturbing the control loop. 
This problem can be avoided with the use of interruptions for the control loop; however this was not implemented
and could be considered as some future improvement IT IS IMPLEMENTED NOW*/  
vpvav = (uint16_t) ( ( ( vpvav * 5935.0 ) / 4096 ) + 0.5 ); //If I install an 845omh with a 158 ohm will be 5935.91 
ipvav = (uint16_t) ( ( ( ipvav * 2.5 * 5000 ) / 4096 ) + 0.5 ); 
iloav = (uint16_t) ( ( ( iloav * 2.5 * 5000 ) / 4096 ) + 0.5 );
v50av = (uint16_t) ( ( ( v50av * 5000.0 ) / 4096 ) + 0.5 );
i50av = (uint16_t) ( ( ( i50av * 5000.0 ) / 4096 ) + 0.5 );
v33av = (uint16_t) ( ( ( v33av * 5000.0 ) / 4096 ) + 0.5 );
i33av = (uint16_t) ( ( ( i33av * 5000.0 ) / 4096 ) + 0.5 );
    if (log_on)
    {

                HEADER;
                //From here there are 20 bytes
                UART_send_u16(minute);
                UART_send_u16((uint16_t)second);
                UART_send_u16(vpvav);
                UART_send_u16(ipvav);
                UART_send_u16(iloav);
                UART_send_u16(v50av);
                UART_send_u16(i50av);
                UART_send_u16(v33av);
                UART_send_u16(i33av);
                FOOTER;
    }
    if (!log_on) RESET_TIME(); /// If #log_on is cleared, call #RESET_TIME()
}


/**@brief This function read the ADC and store the data in the coresponding variable
*/
uint16_t read_ADC(uint16_t channel)
{
    uint16_t ad_res = 0;
    ADCON0bits.CHS = channel;
    __delay_us(20);
    GO_nDONE = 1;
    while(GO_nDONE);
    ad_res = (uint16_t)((ADRESL & 0xFF)|((ADRESH << 8) & 0xF00));
    return ad_res;
}

/**@brief This function control the timing
*/
void timing()
{
    if(!count) /// If #count is other than zero, then
    {
        SECF = 1;
        count = COUNTER + 1; /// * Make #count equal to #COUNTER
        if(second < 59) second++; /// * If #second is smaller than 59 then increase it
        else{second = 0; minute++;} /// * Else, make #second zero and increase #minute
    }else /// Else,
    {
        count--; /// * Decrease it
    }
}

/**@brief This function calculate the averages
*/
void calculate_avg()
{
    switch(count)
    {
        case COUNTER + 1: /// If #count = #COUNTER
            vpvac = 0;
            ipvac = 0;
            iloac = 0;
            v50ac = 0;
            i50ac = 0;
            v33ac = 0;
            i33ac = 0;
            break;
        case 0: /// If #count = 0
            vpvav = ((vpvac >> 7) + ((vpvac >> 6) & 0x01));
            ipvav = ((ipvac >> 7) + ((ipvac >> 6) & 0x01));
            iloav = ((iloac >> 7) + ((iloac >> 6) & 0x01));
            v50av = ((v50ac >> 7) + ((v50ac >> 6) & 0x01));
            i50av = ((i50ac >> 7) + ((i50ac >> 6) & 0x01));
            v33av = ((v33ac >> 7) + ((v33ac >> 6) & 0x01));
            i33av = ((i33ac >> 7) + ((i33ac >> 6) & 0x01));
            break;
        default: /// If #count is not any of the previous cases then
            vpvac += (uint24_t) vpv;
            ipvac += (uint24_t) ipv;
            iloac += (uint24_t) ilo;
            v50ac += (uint24_t) v50;
            i50ac += (uint24_t) i50;
            v33ac += (uint24_t) v33;
            i33ac += (uint24_t) i33;            
    }  
}

void interrupt_enable()
{
    char clear_buffer = 0; /// * Define the variable @p clear_buffer, used to empty the UART buffer
    while(RCIF){
        clear_buffer = RC1REG; /// * Clear the reception buffer and store it in @p clear_buffer
    }
    RCIE = 1; /// * Enable UART reception interrupts
    TXIE = 0; /// * Disable UART transmission interrupts
    TMR1IE = 1;   //enable T1 interrupt
    PEIE = 1;       //enable peripherals interrupts
    GIE = 1;        //enable global interrupts
    count = COUNTER + 1; /// The timing counter #count will be initialized to zero, to start a full control loop cycle
    TMR1IF = 0; //Clear timer1 interrupt flag
    TMR1ON = 1;    //turn on timer 
}


/**@brief This function send one byte of data to UART
* @param bt character to be send
*/
void UART_send_char(char bt)  
{
    while(0 == TXIF)
    {
    }/// * Hold the program until the transmission buffer is free
    TX1REG = bt; /// * Load the transmission buffer with @p bt
}
/**@brief This function receive one byte of data from UART
* @return RC1REG reception register
*/
char UART_get_char()
{
    if(OERR) /// If there is error
    {
        CREN = 0; /// * Clear the error
        CREN = 1; /// * Restart
    }    
    while(!RCIF);  /// Hold the program until the reception buffer is free   
    return RC1REG; /// Receive the value and return it
}
/**@brief This function send a string using UART
* @param st_pt pointer to string to be send
*/
void UART_send_string(const char* st_pt)
{
    while(*st_pt) /// While there is a byte to send
        UART_send_char(*st_pt++); /// * Send it using #UART_send_char() and then increase the pointer possition
}
///**@brief This function convert a number to string and then send it using UART
//* @param value integer to be send
//*/
void display_value_u(uint16_t value)
{   
    char buffer[6]; /// * Define @p buffer to used it for store character storage
    utoa(buffer,value,10);  /// * Convert @p value into a string and store it in @p buffer
    UART_send_string(buffer); /// * Send @p buffer using #UART_send_string()
}
///**@brief This function convert a number to string and then send it using UART
//* @param value integer to be send
//*/
void display_value_s(int16_t value)
{   
    char buffer[7]; /// * Define @p buffer to used it for store character storage
    itoa(buffer,value,10);  /// * Convert @p value into a string and store it in @p buffer
    UART_send_string(buffer); /// * Send @p buffer using #UART_send_string()
}

/**@brief This function send a 16-bit number to UART
* @param number 16-bit number to be send
*/
void UART_send_u16(uint16_t number)  
{
    while(0 == TXIF)
    {
    }/// * Hold the program until the transmission buffer is free
    TX1REG = (number >> 8) & 0xFF; /// * Load the transmission buffer with @p bt
    while(0 == TXIF)
    {
    }/// * Hold the program until the transmission buffer is free
    TX1REG = number & 0x00FF; /// * Load the transmission buffer with @p bt
}

void PAO(uint16_t pv_voltage, uint16_t pv_current, float* power, char* direction)
{
    float new_power;
    new_power =  ((float)pv_voltage * pv_current);
    if (new_power < *power) 
    {
        switch (*direction)
        {
            case 0x06:
                *direction = 0x07;
                break;
            case 0x07:
                *direction = 0x06; //If the direction was decreasing or maintaining it should increase
                break;
        }
    }
    *power = new_power;
}