/*==============================================================================
 Project: Intro-3-Loops                 Activity: mirobo.tech/ubmp4-intro-3
 Date:    February 13, 2023
 
 This introductory programming activity for the mirobo.tech UBMP4 demonstrates
 the use of both while- and for-loop structures to change the brightness of an
 LED using PWM (Pulse-Width Modulation).
 
 Additional program analysis and programming activities expand the use of loops
 to create tones of different pitches, and audio frequency sweeps.
==============================================================================*/

#include    "xc.h"              // Microchip XC8 compiler include file
#include    "stdint.h"          // Include integer definitions
#include    "stdbool.h"         // Include Boolean (true/false) definitions

#include    "UBMP420.h"         // Include UBMP4.2 constants and functions

// TODO Set linker ROM ranges to 'default,-0-7FF' under "Memory model" pull-down.
// TODO Set linker code offset to '800' under "Additional options" pull-down.

// Program variable definitions
unsigned char tOnLED3 = 127;    // LE3D on-time/brightness value
unsigned char pwmPeriod;        // Counter for PWM generation loop
unsigned int period = 460;      // Sound period for program analysis activities

int main(void)
{
    OSC_config();               // Configure internal oscillator for 48 MHz
    UBMP4_config();             // Configure on-board UBMP4 I/O devices
	
    while(1)
	{
        // Decrease brightness
        if(SW2 == 0)
        {
            tOnLED3 -= 1;
        }

        // Increase brightness
        if(SW3 == 0)
        {
            tOnLED3 += 1;
        }
        
        // PWM LED3 brightness
        pwmPeriod = 255;
        while(pwmPeriod != 0)
        {
            if(tOnLED3 == pwmPeriod)
            {
                LED3 = 1;
            }
            pwmPeriod --;
            __delay_us(20);
        }
        LED3 = 0;
        
        // Activate bootloader if SW1 is pressed.
        if(SW1 == 0)
        {
            RESET();
        }
    }
}

/* Learn More -- Program Analysis Activities
 * 
 * 1. The main part of the program contains the 'while(1)' loop, and its
 *    brackets can contain a conditional statement. What condition does the '1'
 *    represent? (Hint: Think about the Boolean variables from the 
 *    'Intro-2-Variables activity.) How many times will this outer while loop
 *    run?
 * 
 * 2. There is a second inner while loop inside the main, outer loop, beginning
 *    with the statement: while(pwmPeriod != 0)
 * 
 *    What condition is being evaluated inside this while statement's
 *    brackets? How many times will the contents of this inner loop run?
 * 
 * 3. What condition is being evaluated by the if statement inside the PWM loop?
 *    What happens when the if condition is true?
 * 
 * 4. Pressing the up or down buttons (SW3 and SW2) will increase or decrease
 *    the brightness of LED D3 using PWM (Pulse-Width Modulation). How many 
 *    different brightnesses can the LED have? What would the step size of one
 *    brightness level change be if it were to be expressed as a percentage?
 * 
 * 5. The while loop needs three statements to perform its function. First, the
 *    assignment statement 'pwmPeriod = 255;' sets the pwmPeriod variable. Next,
 *    the condition 'while(pwmPeriod != 0)' runs the contents of the loop code
 *    while the condition is true. Unlike if statements, in which program flow
 *    continues past the contents of the if structure, the while loop causes
 *    the condition to be re-evaluated after executing the contents within its
 *    braces. To ensure that the while loop does actually end at some point,
 *    the variable in the while condition has to be modified inside the loop.
 *    The assignment statement 'pwmPeriod --;' does this. The loop continues
 *    while the condition is true, and exits when the condition is false, 
 *    running the code below the loop structure.
 * 
 *    A 'for' loop is an alternative to a while loop, and incorporates the
 *    assignment of the loop variable, the loop conditional check, and variable
 *    modification into a single statement as shown in the example code. Replace
 *    the entire while structure in the code with the following for structure:
        
        // PWM LED3 brightness
        for(unsigned char pwmPeriod = 255; pwmPeriod != 0; pwmPeriod --)
        {
            if(tOnLED3 == pwmPeriod)
            {
                LED3 = 1;
            }
            __delay_us(20);
        }
        LED3 = 0;
        
 *    What is an advantage of using a for loop instead of a while loop?
 * 
 * 6. The 'for' loop, above, redefines the pwmPeriod variable in the 
 *    initialization statement: 'for(unsigned char pwmPeriod = 255; ...'
 * 
 *    Even though they share the same name, this instance of the PWMperiod
 *    variable will be local to the for loop. It won't be accessible outside
 *    of the loop, nor will it alter the value of the existing pwmPeriod
 *    variable. For these reasons, it's a bad idea to re-use variable names
 *    inside loops due to the confusion of having two different variables
 *    with the same name. (It's being done here to prove a point!)
 * 
 *    Is this really the case? Let's prove that the global pwmPeriod variable
 *    is not affected by the local pwmPeriod variable inside the for loop. Add
 *    this line above the for loop:
 
        pwmPeriod = 128;

 *    Next, add the following lines after the for loop:
        
        if(pwmPeriod == 128)
        {
            LED4 = 1;
        }
        else
        {
            LED4 = 0;
        }
        
 *    Compile and run the code. When the program runs, the pwmPeriod variable
 *    inside the for loop will count down from 255 to 0, and should be 0 when
 *    the loop finishes. Is LED D4 lit? If it is, what must the value of the
 *    global pwmPeriod variable be?
 * 
 *    Can the global pwmPeriod variable definition at the top of now be removed
 *    since the pwmPeriod variable is being defined in the for loop? Try it!
 * 
 * 7. Add this code below the PWM loop to generate a tone:
                
        // Change pitch
        if(SW4 == 0)
        {
            period -= 1;
        }
        
        if(SW5 == 0)
        {
            period += 1;
        }
        
        // Make a tone
        for(unsigned char cycles = 50; cycles != 0; cycles--)
        {
            BEEPER = !BEEPER;
            for(unsigned int p = period; p != 0; p--);
        }

 *    The code that makes a tone contains two nested for loops. The outer loop 
 *    repeatedly causes the BEEPER pin to toggle before the inner loop runs.
 *    The inner loop is an empty loop, shown by its trailing semicolon ';'. It
 *    contains no code, so nothing will be repeated, but exists only to add a
 *    time delay to our program. The higher the number of loops, the more
 *    instruction cycles it will take to decrement the loop counter variable
 *    to zero, increasing the time delay until the next cycle.
 * 
 *    What numeric type is the period variable? How large a number can this
 *    the period variable hold?
 * 
 * 8. Why is the value of period copied to the local variable p inside the inner
 *    for loop? What would happen if the actual period variable was decremented
 *    instead? (Hint: practice thinking like a computer.)
 * 
 * Programming Activities
 * 
 * 1. Pressing and holding SW2 or SW3 causes the brightness of LED D3 to
 *    repeatedly cycle through its entire brightness range because the tOnLED3
 *    variable is allowed to either overflow, or underflow. Modify the code so
 *    that pressing and holding SW2 will dim the LED until it is off and then
 *    keep if off, and pressing and holding SW3 will brighten the LED and
 *    maintain it at its maximum brightness.
 * 
 * 2. Modify your program to control the brightness of two LEDs simultaneously.
 *    Add code to control LED D4 using SW4 and SW5 while still using SW3 and
 *    SW2 to control LED D3. (Hint: To ensure each LED can reach maximum
 *    brightness -- or 100% PWM on-time -- you'll have to perform the PWM
 *    functions for both LEDs in the same loop. You can see the resulting PWM
 *    wave if you have access to an oscilloscope. If not, to ensure your LEDs
 *    reach full brightness, light LED2 and LED5 at the start of your program
 *    and compare their brightness to LEDs D3 and D4.
 * 
 * 3. Rather than having lights suddenly turn on at full brightness, or motors
 *    turn on at full power, create a program that uses a for loop and your PWM
 *    code to make a 'soft-start' program that slowly increases the PWM on-time
 *    when you press a button. Can you make it turn off in a similar way?
 * 
 * 4. Make a program that creates a repeated, visual 'pulse', brightening and
 *    dimming one or more LEDs continuously.
 * 
 * 5. Make a 'chirp' or 'pew-pew' sound effect by sweeping through a range of
 *    frequencies when a button is pressed.
 */
