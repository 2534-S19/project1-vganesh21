
#include <ti/devices/msp432p4xx/driverlib/driverlib.h> 	//Drivers for the board
#include <ti/grlib/grlib.h>  				//  Graphics Library for the LCD Screen
#include "LcdDriver/Crystalfontz128x128_ST7735.h"  	//  Library used for displaying on the LCD Screen

// include the colors file
#include "colors.h"

//////////////Definitions/////////////////////////////////////

// When a button is pressed, it is grounded (logic 0)
#define PRESSED 0

// The bitmask for left Launchpad Board button, also known as Button1 or Launchpad S1
#define LEFT_BUTTON BIT1    // P1.1

#define LEFT_LED BIT0       // P1.0

// TODO: Define the bitmask for all the Launchpad's and BoosterPack's buttons and LEDs

#define RIGHT_BUTTON BIT4   // P1.4
#define S1_BUTTON BIT1	    // P5.1
#define S2_BUTTON BIT5	    // P3.5
#define BSTRLED_RED BIT6    // P2.6
#define BSTRLED_GRN BIT4   // P2.4
#define BSTRLED_BLU BIT6    // P5.6

#define LAUNCHLED_RED BIT0    // P2.0
#define LAUNCHLED_GRN BIT1    // P2.1
#define LAUNCHLED_BLU BIT2    // P2.2

////////////Function Declarations/////////////////////////////

// This function initializes everything including the setup of the project
void initialize(Graphics_Context *g_sContext, drawn_color_t *pick);

// These functions initialize the graphics
void InitGraphics(Graphics_Context *g_sContext);
void InitFonts();

// This function initializes all the needed GPIO pins
void InitGPIO();

// This function displays the texts needed for the project on the LCD screen
void DisplayFunction(Graphics_Context *g_sContext, drawn_color_t *pick);

// Function for matching the displayed colors
bool colorsMatched(drawn_color_t *pick);

// This function is used to Read the Button input on the LaunchPad and the BoosterXL.
// From the input combinations, you must produce correct output for the LED1 on the Launchpad and RGB LED on the BoosterXL
void BoosterLEDColors(drawn_color_t *pick);

// This function is used to produce the output for the LED2 on the Launchpad
void LED2Colors(color_selected_t picked);




/////////////The Function Definitions////////////////////////////

// Main function is the applicator's entry point
int main(void) {

    //Initialize structures
    drawn_color_t pick;
    Graphics_Context g_sContext;
    initialize(&g_sContext, &pick);

    // Program event loop
    while (1) {

	// LCD printing
	DisplayFunction(&g_sContext, &pick);

	// Lighting up LED2 of Launchpad
	LED2Colors(pick.picked);

	// Reading the status of the buttons, lighting up the BoosterPack LED
	BoosterLEDColors(&pick);

    }
}

// TODO: You need to complete this function
// This function lights up the LED2 on Launchpad based on the chosen color.
// For example, if the "Red" is picked, then LED2_RED should be turned on and the other two LED2s should be off.
// If the Purple is picked, the LED2_BLUE and LED2_RED should be on and the LED2_GREEN should be off.
void LED2Colors(color_selected_t picked) {


    switch(picked)
    {

	case Red:
	    P2OUT = P2OUT |  LAUNCHLED_RED;
	    P2OUT = P2OUT & ~LAUNCHLED_GRN;
	    P2OUT = P2OUT & ~LAUNCHLED_BLU;
	    break;

	case Blue:
	    P2OUT = P2OUT & ~LAUNCHLED_RED;
	    P2OUT = P2OUT & ~LAUNCHLED_GRN;
	    P2OUT = P2OUT |  LAUNCHLED_BLU;
	    break;

	case Green:
	    P2OUT = P2OUT & ~LAUNCHLED_RED;
	    P2OUT = P2OUT |  LAUNCHLED_GRN;
	    P2OUT = P2OUT & ~LAUNCHLED_BLU;
	    break;

	case Purple:
	    P2OUT = P2OUT |  LAUNCHLED_RED;
	    P2OUT = P2OUT & ~LAUNCHLED_GRN;
	    P2OUT = P2OUT |  LAUNCHLED_BLU;
	    break;

	case Orange:
	    P2OUT = P2OUT |  LAUNCHLED_RED;
	    P2OUT = P2OUT |  LAUNCHLED_GRN;
	    P2OUT = P2OUT & ~LAUNCHLED_BLU;
	    break;

	case White:
	    P2OUT = P2OUT |  LAUNCHLED_RED;
	    P2OUT = P2OUT |  LAUNCHLED_GRN;
	    P2OUT = P2OUT |  LAUNCHLED_BLU;
	    break;

	default:
	    P2OUT = P2OUT & ~LAUNCHLED_RED;
	    P2OUT = P2OUT & ~LAUNCHLED_GRN;
	    P2OUT = P2OUT & ~LAUNCHLED_BLU;
	    // turn off all LED2s
    }
}

// TODO: You need to complete this function
// This function achieves the following: It checks the status of the Launchpad and the BoosterPack buttons and
// lights up the Left LED and the BoosterPack RGB LED accordingly to Table 1 of the project description. In addition,
// it updates the LEDColor member of the input argument, namely pick.
// For example, if the Left Launchpad button is pressed, the RGB on BoosterPack should light up with Red,
// the Left LED on Launchpad should be off and last but not least, pick->LEDColor should be updated to be "Red".
// Note that the picked color for an invalid button combination should be "none" that is defined in the enum of colors.
void BoosterLEDColors(drawn_color_t *pick) {

    if(((P5IN & S1_BUTTON) == PRESSED) && ((P3IN & S2_BUTTON) == PRESSED))
    {
    	P2OUT = P2OUT |  BSTRLED_RED;
    	P2OUT = P2OUT |  BSTRLED_GRN;
    	P5OUT = P5OUT |  BSTRLED_BLU;

        P1OUT = P1OUT & ~LEFT_LED;
        pick->LEDColor = White;
    }
    else if(((P1IN & LEFT_BUTTON) == PRESSED) && ((P1IN & RIGHT_BUTTON) == PRESSED))
    {
    	P2OUT = P2OUT |  BSTRLED_RED;
    	P2OUT = P2OUT & ~BSTRLED_GRN;
    	P5OUT = P5OUT |  BSTRLED_BLU;

    	P1OUT = P1OUT & ~LEFT_LED;
    	pick->LEDColor = Purple;
    }
    else if(((P1IN & LEFT_BUTTON) == PRESSED))
    {
	P2OUT = P2OUT |  BSTRLED_RED;
	P2OUT = P2OUT & ~BSTRLED_GRN;
	P5OUT = P5OUT & ~BSTRLED_BLU;

	P1OUT = P1OUT & ~LEFT_LED;
	pick->LEDColor = Red;
    }
    else if(((P1IN & RIGHT_BUTTON) == PRESSED))
    {
	P2OUT = P2OUT & ~BSTRLED_RED;
	P2OUT = P2OUT & ~BSTRLED_GRN;
	P5OUT = P5OUT |  BSTRLED_BLU;

	P1OUT = P1OUT & ~LEFT_LED;
	pick->LEDColor = Blue;
    }
    else if(((P5IN & S1_BUTTON) == PRESSED))
    {
	P2OUT = P2OUT & ~BSTRLED_RED;
	P2OUT = P2OUT |  BSTRLED_GRN;
	P5OUT = P5OUT & ~BSTRLED_BLU;

	P1OUT = P1OUT & ~LEFT_LED;
	pick->LEDColor = Green;
    }
    else if(((P3IN & S2_BUTTON) == PRESSED))
    {
	P2OUT = P2OUT |  BSTRLED_RED;
	P2OUT = P2OUT |  BSTRLED_GRN;
	P5OUT = P5OUT & ~BSTRLED_BLU;

    	P1OUT = P1OUT & ~LEFT_LED;
    	pick->LEDColor = Orange;
    }
    else
    {
	P2OUT = P2OUT & ~BSTRLED_RED;
	P2OUT = P2OUT & ~BSTRLED_GRN;
	P5OUT = P5OUT & ~BSTRLED_BLU;

	P1OUT = P1OUT |  LEFT_LED;
	pick->LEDColor = none;
    }

}

// TODO: Complete this function such that all the BoosterPack and Launchpad connected pins are configured properly.
void InitGPIO() {

    P1DIR |= LEFT_LED;

    P2DIR |= LAUNCHLED_RED;
    P2DIR |= LAUNCHLED_GRN;
    P2DIR |= LAUNCHLED_BLU;

    P2DIR |= BSTRLED_RED;
    P2DIR |= BSTRLED_GRN;
    P5DIR |= BSTRLED_BLU;

    P1DIR |= LEFT_BUTTON;
    P1REN |= LEFT_BUTTON;
    P1OUT |= LEFT_BUTTON;

    P1DIR |= RIGHT_BUTTON;
    P1REN |= RIGHT_BUTTON;
    P1OUT |= RIGHT_BUTTON;

    P5DIR |= S1_BUTTON;
    P5REN |= S1_BUTTON;
    P5OUT |= S1_BUTTON;

    P3DIR |= S2_BUTTON;
    P3REN |= S2_BUTTON;
    P3OUT |= S2_BUTTON;
}

// Initialization part is always device dependent and therefore does not change much with HAL
void initialize(Graphics_Context *g_sContext, drawn_color_t *pick) {

    // step 1: Stop watchdog timer
    // We do this at the beginning of all our programs for now. Later we learn more about it.
    WDT_A_hold(WDT_A_BASE);

    InitGPIO();


    // No Need for Students to modify Code beyond this point to finish the project
    // Initial Color Pick
    initColor(pick);

    // Initialize the graphics
    InitGraphics(g_sContext);
    Graphics_drawString(g_sContext, pick->colorText, -1, 10, 60, true);
    Graphics_drawString(g_sContext, pick->buttonText, -1, 10, 100, true);
}

// Initializes the LCD screen graphics
void InitGraphics(Graphics_Context *g_sContext) {

    Graphics_initContext(g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_YELLOW);
    Graphics_setBackgroundColor(g_sContext, GRAPHICS_COLOR_BLUE);
    Graphics_setFont(g_sContext, &g_sFontCmtt16);

    InitFonts();
    Graphics_setFont(g_sContext, &g_sFontCmss18b);
    Graphics_clearDisplay(g_sContext);
    Graphics_drawString(g_sContext, "Color to match: ", -1, 5, 30, true);
    Graphics_drawString(g_sContext, "Press: ", -1, 5, 80, true);
}

//Initialize screen fonts
void InitFonts() {
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
}

// Function takes care of Matching  colors and displaying color word text
void DisplayFunction(Graphics_Context *g_sContext, drawn_color_t *pick) {
    // If the colors have matched move on move this section into a function of it's own
    if(colorsMatched(pick)) {
	SelectColor(pick);
	// print inside here
	Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_BLUE);
	Graphics_Rectangle R1;
	R1.xMax = 128;
	R1.yMax = 80;
	R1.xMin = 0;
	R1.yMin = 50;
	Graphics_fillRectangle(g_sContext, &R1);
	Graphics_Rectangle R2;
	R2.xMax = 128;
	R2.yMax = 100;
	R2.xMin = 0;
	R2.yMin = 120;
	Graphics_fillRectangle(g_sContext, &R2);
	Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_YELLOW);
	Graphics_drawString(g_sContext, pick->colorText, -1, 10, 60, true);
	Graphics_drawString(g_sContext, pick->buttonText, -1, 10, 100, true);
    }
}

// Function determines if the randomly color displayed on the LCD  matches the color generated by the LCD
bool colorsMatched(drawn_color_t *pick) {

    if(pick->picked == pick->LEDColor) {
	return true;
    }
    else {
	return false;
    }
}
