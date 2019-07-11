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
    /** @b SYSTEM @b CLOCK*/
    /** PLL is always enabled because of configuration bits*/
    OSCCONbits.IRCF = 0b1111; /// * Set clock to 32MHz (with PLL)
    OSCCONbits.SCS = 0b00; /// * Clock determined by FOSC<2:0> in Configuration Words
    OSCCONbits.SPLLEN = 1; /// * Enable PLL. According to Errata this shall not be done in the Configuration Words
    /** @b ENABLE @b OUPUT*/
    // //PORTC doesn't have ANSELC
    TRISA4 = 0; /// * Set RA4 as output. Driver enable (DR_EN)
    ANSA4 = 0; /// * RA4 as digital
    WPUA4 = 0; /// * Weak pull up deactivated
    RA4 = 0; /// * Start disabled
    /** @b TIMER 1 for control and measuring loop using interruption*/
    /* Preload TMR1 register pair for 1us overflow*/
    /* T1OSCEN = 1, nT1SYNC = 1, TMR1CS = 0 and TMR1ON = 1*/
    nT1SYNC = 0;     //Synchronized
    T1OSCEN = 0;
    TMR1ON = 0;       //ON
    TMR1GE = 0;      //Dont care about gate
    TMR1CS0 = 0;       
    TMR1CS1 = 0;    //FOSC/4
    T1CKPS0 = 0;
    T1CKPS1 = 0;
    TMR1H = 0xE1;//TMR1 Fosc/4= 8Mhz (Tosc= 0.125us)
    TMR1L = 0x83;//TMR1 counts: 7805 x 0.125us = 0.97562ms
    /** @b PSMC/PWM @b SETTINGS*/
    /** Programmable switch mode control (PSMC)*/
    PSMC1CON = 0x00; /// * Clear PSMC1 configuration to start
    PSMC1MDL = 0x00; /// * No modulation
    PSMC1CLK = 0x01; /// * Driven by 64MHz PLL system clock
    PSMC1PRH = 0x01; /// * Set period high register to 0x01
    PSMC1PRL = 0xFF; /// * Set period low register to 0xFF
    /** 511 + 1 clock cycles for period that is 8us (125KHz)*/
    /** This set the PWM with 9 bit of resolution*/
    /** Duty cycle*/
    PSMC1DCH = 0x00;                    // * Set duty cycle high register to 0x00   
    PSMC1DCL = 0x00;                    // * Set duty cycle low register to 0x00
    /* Duty cycle starts in 0 */  
    /** Phase or rising event*/
    PSMC1PHH = 0x00;                    /// * Rising event starts from the beginning
    PSMC1PHL = 0x00;                    /// * Rising event starts from the beginning
    P1STRA = 1;            /// * Single PWM activated in PSMC1A (RC0)
    P1POLA = 0;            /// * Active high (RC0)
    //P1OEC = 1;             /// * PSMC activated in PSMC1C (RC2)
    P1PRST = 1;            /// * Period event occurs when PSMC1TMR = PSMC1PR
    P1PHST = 1;            /// * Rising edge event occurs when PSMC1TMR = PSMC1PH
    P1DCST = 1;            /// * Falling edge event occurs when PSMC1TMR = PSMC1DC
    PSMC1CON = 0x80;                    /// * Enable|Load Buffer|Dead band disabled|Single PWM
    //PSMC1TIE = 1;                       //Enable interrupts for Time Based 
    WPUC0 = 0;                          /// * Disable WPU for RC0.
    TRISC0 = 1;                         /// * Set RC0 as input for now
    /** @b ADC*/
    /** ADC INPUTS*///check this after final design
    TRISA0 = 1; /// * RA0, IS_BAT
    ANSA0 = 1; /// * RA0 analog
    WPUA0 = 0; /// * Weak pull up deactivated
    TRISA1 = 1; /// * RA1, VS_BAT
    ANSA1 = 1; /// * RA1 analog
    WPUA1 = 0; /// * RA1 weak pull up deactivated
    TRISA2 = 1; /// * RA2, VS_BUS
    ANSA2 = 1; /// * RA2 analog
    WPUA2 = 0; /// * RA2 weak pull up deactivated
    /** Configs*/
    ADCON0bits.ADRMD = 0; /// * 12 bits result
    ADCON1bits.ADCS = 0b010; /// * Clock selected as FOSC/32
    ADCON1bits.ADNREF = 0; /// * Connected to Vss
    ADCON1bits.ADPREF = 0b01; /// * Connected to Vref+
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

/**@brief This function is the PI control loop
*/
void control_loop()
{   
    pid(vbus, vref);  /// * The #pid() function is called with @p feedback = #v and @p setpoint = #vref
    set_DC(); /// The duty cycle is set by calling the #set_DC() function
}
/**@brief This function defines the PI controller
*  @param   feedback average of measured values for the control variable
*  @param   setpoint desire controlled output for the variable
*/

/**@brief This function defines the PI controller
*  @param   feedback average of measured values for the control variable
*  @param   setpoint desire controlled output for the variable
*/
void pid(uint16_t feedback, uint16_t setpoint)
{ 
int16_t     er = 0; /// * Define @p er for calculating the error
int16_t     pi = 0; /// * Define @p pi for storing the PI compesator value
int16_t     prop = 0;
int16_t     inte = 0;
    er = (int16_t) (setpoint - feedback); /// * Calculate the error by substracting the @p feedback from the @p setpoint and store it in @p er
    if(er > ERR_MAX) er = ERR_MAX; /// * Make sure error is never above #ERR_MAX
    if(er < ERR_MIN) er = ERR_MIN; /// * Make sure error is never below #ERR_MIN
    prop = er / KP; /// * Calculate #proportional component of compensator
	intacum += (int24_t) (er); /// * Calculate #integral component of compensator
    inte = (int16_t) (intacum /  ((int24_t) KI * COUNTER));
    pi = prop + inte; /// * Sum them up and store in @p pi*/
    if ((uint16_t)((int16_t)dc + pi) >= DC_MAX){ /// * Make sure duty cycle is never above #DC_MAX
        dc = DC_MAX;
    }else if ((uint16_t)((int16_t)dc + pi) <= DC_MIN){ /// * Make sure duty cycle is never below #DC_MIN
        dc = DC_MIN;
    }else{
        dc = (uint16_t)((int16_t)dc + pi); /// * Store the new value of the duty cycle with operation @code dc = dc + pi @endcode
    }   
}

/**@brief This function sets the desired duty cycle
*/
void set_DC()
{
/// This function can set the duty cycle from 0x0 to 0x1FF
    PSMC1DCL = dc & 0x00FF; /// * Lower 8 bits of #dc are stored in @p PSMC1DCL
    PSMC1DCH = (dc >> 8) & 0x01; /// * Higher 1 bit of #dc are stored in @p PSMC1DCH
    PSMC1CONbits.PSMC1LD = 1; /// * Set the load register. This will load all the setting as once*/
}

/**@brief This function takes care of printing the test data using the UART
*/
void log_control()
{
/**The code in this function is only executed if the #log_on variable is set*/
/**This function takes care of sending the logging data in pieces to avoid disturbing the control loop. 
This problem can be avoided with the use of interruptions for the control loop; however this was not implemented
and could be considered as some future improvement IT IS IMPLEMENTED NOW*/  
vbusp = (uint16_t) ( ( ( vbusp * 5000.0 ) / 4096 ) + 0.5 );
vbatp = (uint16_t) ( ( ( vbatp * 5000.0 ) / 4096 ) + 0.5 );
ibatp = (uint16_t) ( ( ( ibatp * 2.5 * 5000 ) / 4096 ) + 0.5 ); 
if ( ibatp > 0 ) capap += (uint16_t) ( ibatp / 360 ) + 0.5; /// * Divide #iprom between 3600 and multiplied by 10 add it to #qprom to integrate the current over time
    
    if (log_on)
    {
                LINEBREAK;
                display_value_u(minute);
                UART_send_char(':'); /// * Send a colons character
                if (second < 10) UART_send_char('0'); /// * If #second is smaller than 10 send a '0'
                display_value_u((uint16_t) second);
                UART_send_char(','); /// * Send a comma character
                UART_send_string((char *) "vbus:"); /// * Send a 'C'
                display_value_u(vbusp);
                UART_send_char(','); /// * Send a comma character
                UART_send_string((char *) "vbat:"); /// * Send an 'I'
                display_value_u(vbatp);
                UART_send_char(','); /// * Send a comma character
                UART_send_string((char *) "ibat:"); /// * Send an 'I'
                display_value_u(ibatp);
                UART_send_char(','); /// * Send a comma character
                UART_send_string((char *) "cbat:"); /// * Send a 'Q'
                //display_value_u((uint16_t) (dc * 1.933125));
                display_value_u(capap);
                UART_send_char('<'); /// * Send a '<'
    }
    if (!log_on) RESET_TIME(); /// If #log_on is cleared, call #RESET_TIME()
}


/**@brief This function read the ADC and store the data in the coresponding variable
*/
uint16_t read_ADC(uint16_t channel)
{
    uint16_t ad_res = 0;
    __delay_us(10);
    ADCON0bits.CHS = channel;
    __delay_us(10);
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
            vbusa = 0;
            vbata = 0;
            ibata = 0;
            break;
        case 0: /// If #count = 0
            vbusp = ((vbusa >> 10) + ((vbusa >> 9) & 0x01)); /// * This is equivalent to vbusa / 1024 = vbusa / 2^10      
            vbatp = ((vbata >> 10) + ((vbata >> 9) & 0x01)); /// * This is equivalent to vbata / 1024 = vbata / 2^10          
            ibatp = ((ibata >> 10) + ((ibata >> 9) & 0x01)); /// * This is equivalent to ibata / 1024 = ibata / 2^10   
            break;
        default: /// If #count is not any of the previous cases then
            vbusa += (uint24_t) vbus; /// * Accumulate #vbus in #vbusa
            vbata += (uint24_t) vbat; /// * Accumulate #vbat in #vbata
            ibata += (uint24_t) ibat; /// * Accumulate #ibat in #ibata
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
void UART_send_string(char* st_pt)
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
    UART_send_string(&buffer[0]); /// * Send @p buffer using #UART_send_string()
}