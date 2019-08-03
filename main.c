/**
 * @file main.c
 * @author Juan J. Rojas
 * @date 10 Nov 2018
 * @brief main loop for the BDC prototype controller or converter.
 * @par Institution:
 * LaSEINE / CeNT. Kyushu Institute of Technology.
 * @par Mail (after leaving Kyutech):
 * juan.rojas@tec.ac.cr
 * @par Git repository:
 * https://bitbucket.org/juanjorojash/bdc_prototype/src/master
 */

#include "hardware.h"

/**@brief This is the main function of the program.
*/


void main(void)
{
    initialize(); /// * Call the #initialize() function
    __delay_ms(10);
    interrupt_enable();
    voc = sVOC; 
    ivbusr = sVREF;
    vbusr = sVREF;
    vbatmin = sVBATMIN;
    vbatmax = sVBATMAX;
    iref = sCREF;
    log_on = 1;
    while(1)
    {           
        if (SECF) /// * The following tasks are executed every second:
        {     
            SECF = 0;
            log_control_hex();
        }       
	}
}

/**@brief This is the interruption service function. It will stop the process if an @b ESC or a @b "n" is pressed. 
*/
void __interrupt() ISR(void) 
{
    char recep = 0; /// Define and initialize @p recep variable to store the received character
    
    if(TMR1IF)
    {
        TMR1H = 0xE1; //TMR1 Fosc/4= 8Mhz (Tosc= 0.125us)
        TMR1L = 0x83; //TMR1 counts: 7805 x 0.125us x 8 = 7805us
        TMR1IF = 0; //Clear timer1 interrupt flag
        vpv = read_ADC(V_BUS);
        ipv = read_ADC(I_PV);
        ipv = abs(ipv - 2048);
        if (mppt){
            PAO(vpv, ipv, &power, &dir);
            DIRECTION(dir);
        }
        ilo = read_ADC(I_LOAD);
        ilo = abs(ilo - 2048);
        v50 = read_ADC(V_PDU_50V);
        i50 = read_ADC(I_PDU_50V);
        v33 = read_ADC(V_PDU_33V);
        i33 = read_ADC(I_PDU_33V);
        calculate_avg(); /// * Then, averages for the 250 values available each second are calculated by calling the #calculate_avg() function
        timing(); /// * Timing control is executed by calling the #timing() function         
    }
    
    if(RCIF)/// If the UART reception flag is set then:
    {
        if(RC1STAbits.OERR) /// * Check for errors and clear them
        {
            RC1STAbits.CREN = 0;  
            RC1STAbits.CREN = 1; 
        }
        while(RCIF) recep = RC1REG; /// * Empty the reception buffer and assign its contents to the variable @p recep
        switch (recep)
        {
        case 0x63: /// * If @p recep received a @b "c", then:
            STOP;
            break;
        case 0x73: /// * If @p recep received an @b "s", then:
            RESET_TIME();
            mppt=1;
            START;
            break;
        case 0x69: /// * If @p recep received an @b "i", then:
            DIRECTION(0x06);
            break;
        case 0x64: /// * If @p recep received an @b "d", then:
            DIRECTION(0x07);
            break;
        case 0x72: /// * If @p recep received an @b "r", then:
            DIRECTION(0x09);
            break;
        default:
            recep = 0;
        }
    }  
   
}