#ifndef BUTTONS_H
#define BUTTONS_H

#define JOY_LR 0x04		//ADC channel 1, for Left-Right Joystick
#define JOY_DU 0x05		//ADC channel 2, for Down-Up Joystick
#define SLIDER_R 0x06	//ADC channel 3, for Right Slider
#define SLIDER_L 0x07	//ADC channel 4, for Left Slider

/****************************************************************************
Structures for Joystick, sliders and buttons data
****************************************************************************/
typedef struct {
	uint8_t XX;
	uint8_t YY;
	uint8_t XX_init;
	uint8_t YY_init;
} joy_position_t;

typedef struct {
	uint8_t left;
	uint8_t right;
} slider_position_t;

typedef struct {
	uint8_t left;
	uint8_t right;
} buttons_value_t;

typedef enum JoyDir{JOY_LEFT, JOY_RIGHT, JOY_UP, JOY_DOWN, JOY_NEUTRAL} joy_direction_t;	//Enum with all joystick directions


/****************************************************************************
Joystick initialization
****************************************************************************/
void buttons_init(void);


/****************************************************************************
Reads the current value of the joystick coordinates 
Updates joystick coordinates with the average of the 4 previous values
****************************************************************************/
void buttons_update_joy_coord();


/****************************************************************************
Get function to transform and return the current joystick coordinates
****************************************************************************/
joy_position_t buttons_get_joy_coord();


/****************************************************************************
Get function to calculate and return the current joystick direction by
passing cordinates to the function
****************************************************************************/
joy_direction_t buttons_get_joy_direction(int16_t X_coord, int16_t Y_coord);


/****************************************************************************
Reads the current value of both sliders
****************************************************************************/
void buttons_update_slider_positions();


/****************************************************************************
Get function to transform and return the current sliders values
****************************************************************************/
slider_position_t buttons_get_slider_positions();

#endif // BUTTON_H