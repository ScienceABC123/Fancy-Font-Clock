#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <wiringPi.h>
//#include "options.h"

// Defines

#define	WHITE 0x00FFFFFF
#define BLACK 0x00000000
#define	DK_GRAY	0x001F1F1F
#define	MED_GRAY 0x006F6F6F
#define	LT_GRAY 0x00BFBFBF

#define RED 0x00FF0000
#define ORANGE 0x00FF8B28
#define YELLOW 0x00FFEB2A
#define GREEN 0x0000FF00
#define BLUE 0x000000FF

// Simple ARM Graphics Library functions

unsigned int SAGL_Init(char *a, unsigned int b, unsigned int c);								// SAGL_Init(fb_addr, h_size, v_size)

unsigned int ScreenClear(unsigned int a);														// ScreenClear(color)

unsigned int SelectColors(unsigned int a, unsigned int b);										// SelectColors(f_color, b_color)

unsigned int SelectFont(unsigned int *a);														// SelectFont(font_addr)
unsigned int GetFontHeight(void);																// GetFontHeight(void)
unsigned int GetStringLength(char *a);															// GetStringLength(string_ptr)
unsigned int PrintStringLeft(char *a, unsigned int b, unsigned int c);							// PrintStringLeft(string_ptr, x_pos, y_pos)
unsigned int PrintStringRight(char *a, unsigned int b, unsigned int c);							// PrintStringRight(string_ptr, x_pos, y_pos)
unsigned int PrintStringCenter(char *a, unsigned int b, unsigned int c);						// PrintStringCenter(string_ptr, x_pos, y_pos)

unsigned int DrawRectOutline(unsigned int a, unsigned int b, unsigned int c, unsigned int d);	// DrawRectOutline(x1_pos, y1_pos, x2_pos, y2_pos)
unsigned int DrawRectFilled(unsigned int a, unsigned int b, unsigned int c, unsigned int d);	// DrawRectFilled(x1_pos, y1_pos, x2_pos, y2_pos)

unsigned int DrawCircleOutline(unsigned int a, unsigned int b, unsigned int c);					// DrawCircleOutline(x_pos, y_pos, radius)
unsigned int DrawCircleFilled(unsigned int a, unsigned int b, unsigned int c);					// DrawCircleFilled(x_pos, y_pos, radius)

unsigned int DrawLine(unsigned int a, unsigned int b, unsigned int c, unsigned int d);			// DrawLine(x1_pos, y1_pos, x2_pos, y2_pos)

unsigned int DisplayBMP(unsigned int *a, unsigned int b, unsigned int c);						// DisplayBMP(BMP_ptr, x_pos, y_pos)

unsigned int GetPixelColor(unsigned int a, unsigned int b);										// GetPixelColor(x_pos, y_pos)
unsigned int SetPixelColor(unsigned int a, unsigned int b, unsigned int c);						// SetPixelColor(color, x_pos, y_pos)

// Fonts

extern unsigned int FontArial32;
extern unsigned int FontArial64;
extern unsigned int FontArial153;
extern unsigned int FontCentury36;
extern unsigned int FontCentury238;

// BMPs

extern unsigned int Moon_0_BMP;
extern unsigned int Moon_1_BMP;
extern unsigned int Moon_2_BMP;
extern unsigned int Moon_3_BMP;
extern unsigned int Moon_4_BMP;
extern unsigned int Moon_5_BMP;
extern unsigned int Moon_6_BMP;
extern unsigned int Moon_7_BMP;
extern unsigned int Moon_8_BMP;
extern unsigned int Moon_9_BMP;
extern unsigned int Moon_10_BMP;
extern unsigned int Moon_11_BMP;
extern unsigned int Moon_12_BMP;
extern unsigned int Moon_13_BMP;
extern unsigned int Moon_14_BMP;
extern unsigned int Moon_15_BMP;
extern unsigned int Moon_16_BMP;
extern unsigned int Moon_17_BMP;
extern unsigned int Moon_18_BMP;
extern unsigned int Moon_19_BMP;
extern unsigned int Moon_20_BMP;
extern unsigned int Moon_21_BMP;
extern unsigned int Moon_22_BMP;
extern unsigned int Moon_23_BMP;
extern unsigned int Moon_24_BMP;
extern unsigned int Moon_25_BMP;
extern unsigned int Moon_26_BMP;
extern unsigned int Moon_27_BMP;
extern unsigned int Moon_28_BMP;
extern unsigned int Moon_29_BMP;

int main(int argc, char* argv[]) {

	int fbfd = 0;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	char *fbp = 0;

	time_t curtime;
	struct tm *current_time;

	int	second;
	int second_last = 60;
	int time_update_flag;

	int gmt_hour_24;
	int gmt_minute;
	int gmt_day_of_week;
	int gmt_day_of_week_last = 8;

	int loc_hour_24;
	int loc_hour_12;
	int loc_minute;
	int loc_time_zone;
	int loc_time_zone_last = 13;
	int loc_day_of_week;
	int loc_day_of_week_last = 8;
	int loc_day_update_flag;

	double julian_date;
	double julian_date_now;

	int sunrise;
	int sunset;
	int civil_twilight_am;
	int civil_twilight_pm;
	int nautical_twilight_am;
	int nautical_twilight_pm;

	float lunar_day;
	unsigned int *bmp_adr;

	int string_length_last = 1000;

	int display_mode;

	int auto_dim_night;
	int day_bright;
	int night_bright;
	int loc_minute_of_day;

	float latitude;
	float longitude;

	float a;
	float b;
	float c;
	float aa;
	float ab;
	float ac;
	float ad;
	float ae;
	float af;
	float ag;
	float ah;
	float ai;
	float aj;
	float ak;
	float al;
	float am;
	float an;

	int i;
	int j;
	int k;
	int l;
	int m;
	int n;

	char str1[80];
	char str2[40];

	FILE *file0;

	int save_bmp_flag = 1;

	// Note: Using "sudo nano /home/pi/.bashrc" to autostart on boot

/*******************************************************************************
*
* Initialization
*
*******************************************************************************/

	// Use system call to clear the screen

	strcpy(str1, "clear");
	system(str1);

	// Initialize the graphics screen

	printf("Mapping video frame buffer to user memory, ");

	// Open the file for reading and writing

	fbfd = open("/dev/fb0", O_RDWR);
	if (!fbfd) {
		printf("FAILED!\n");
		return(-1);
	}

	// Get fixed screen information

	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		printf("FAILED!\n");
		return(-1);
	}

	// Get variable screen information

	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("FAILED!\n");
		return(-1);
	}

	// Map framebuffer to user memory

	fbp = (char*)mmap(0,
		finfo.smem_len,
		PROT_READ | PROT_WRITE,
		MAP_SHARED,
		fbfd, 0);

	if ((int)fbp == -1) {
		printf("FAILED!\n");
		return(-1);
	} else {
		printf("successful!\n");
	}

	// Display screen resolution and color depth

	printf("Screen resolution: %dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel );

	// Check to make sure frame buffer is 32bpp

	if (vinfo.bits_per_pixel != 32) {
		printf("Video frame buffer is NOT setup for 32bpp. Fix it before using the Simple ARM Graphics library!\n");
		return(-1);
	}

	// Initialize WiringPi

	printf("\nInitializing WiringPi, ");
	i = wiringPiSetupGpio();
	if (i != 0) {
		printf("FAILED! Error code: %d\n", i);
		return(-1);
	} else {
		printf("successful!\n");
	}

	// Use system call to turn the cursor off

	strcpy(str1, "setterm -cursor off");
	system(str1);

	// Sleep for 3 seconds

	sleep(3);

	// Initialize the Simple ARM Graphics library

	SAGL_Init(fbp, vinfo.xres, vinfo.yres);

	// Clear the screen to black

	ScreenClear(BLACK);

	// Display software version

	SelectFont(&FontArial32);
	SelectColors(WHITE, BLACK);
	PrintStringLeft("Fancy Font Clock (v2.0)", 0, 0);

	// Set default options

	display_mode = 0;
	day_bright = 200;
	night_bright = 1;
	auto_dim_night = 0;
	latitude = 0;
	longitude = 0;

	// Read options from "options.h"

	FILE *fptr = fopen("options.h", "r");
	while (fscanf(fptr, "%s %s %f", str1, str2, &a) != EOF) {
		if ((strcmp(str1, "#define") == 0) && (strcmp(str2, "DISPLAY_MODE") == 0)) {
			if ((a >= 0) && (a <= 5)) {
				display_mode = a;
			}
		} else if ((strcmp(str1, "#define") == 0) && (strcmp(str2, "DAY_BRIGHT") == 0)) {
			if ((a >= 1) && (a <= 200)) {
				day_bright = a;
			}
		} else if ((strcmp(str1, "#define") == 0) && (strcmp(str2, "NIGHT_BRIGHT") == 0)) {
			if ((a >= 1) && (a <= day_bright)) {
				night_bright = a;
			}
		} else if ((strcmp(str1, "#define") == 0) && (strcmp(str2, "AUTO_DIM_NIGHT") == 0)) {
			if (a != 0) {
				a = 1;
			}
			auto_dim_night = a;
		} else if ((strcmp(str1, "#define") == 0) && (strcmp(str2, "LATITUDE") == 0)) {
			if ((a >= -90) && (a <= 90)) {
				latitude = a;
			}
		} else if ((strcmp(str1, "#define") == 0) && (strcmp(str2, "LONGITUDE") == 0)) {
			if ((a >= -180) && (a <= 180)) {
				longitude = a;
			}
		}
	}
	fclose (fptr);

	// Display options being used

	i = GetFontHeight() * 1.25;
	sprintf(str1, "display mode = %d", display_mode);
	PrintStringLeft(str1, 0, (2 * i));
	a = day_bright;
	a = a / 2;
	sprintf(str1, "day brightness = %d (%.1f%%)", day_bright, a);
	a = night_bright;
	a = a / 2;
	PrintStringLeft(str1, 0, (3 * i));
	sprintf(str1, "night brightness = %d (%.1f%%)", night_bright, a);
	PrintStringLeft(str1, 0, (4 * i));
	if (auto_dim_night != 0) {
		sprintf(str2, "enabled");
	} else {
		sprintf(str2, "disabled");
	}
	sprintf(str1, "auto-dim at night = %d (%s)", auto_dim_night, str2);
	PrintStringLeft(str1, 0, (5 * i));
	sprintf(str1, "latitude = %.5f", latitude);
	PrintStringLeft(str1, 0, (6 * i));
	sprintf(str1, "longitude = %.5f", longitude);
	PrintStringLeft(str1, 0, (7 * i));

	// Setup PWM for controlling LCD brightness

	pinMode(18, PWM_OUTPUT);		// Set GPIO 18 for PWM output
	pwmSetMode(PWM_MODE_MS);		// Base frequency for mark:space mode seems to be 19.2MHz
	pwmSetClock(5);					// Divide base frequency by 5 to get 3.84MHz for PWM counter
	pwmSetRange(200);				// Set PWM cycle counter for 200 clocks (0-100% duty cycle, in 0.5% increments) this sets the PWM frequency to 19.2KHz
	pwmWrite(18, day_bright);		// Set daytime brightness

	// Sleep for 7 seconds

	sleep(7);

/*******************************************************************************
*
* Main Loop
*
*******************************************************************************/

	while (1) {

		// Update current time

		curtime = time(NULL);

		// Get current GMT date and time info

		current_time = gmtime(&curtime);

		// Get second

		strftime(str1, 40, "%S", current_time);
		second = strtof(str1, NULL);

		// Check for new second

		if (second != second_last) {

			// Save new second as last second, and set time update flag

			second_last = second;
			time_update_flag = 1;

			// Get GMT hour (24 hour format)

			strftime(str1, 40, "%k", current_time);
			gmt_hour_24 = strtof(str1, NULL);

			// Get GMT minute

			strftime(str1, 40, "%M", current_time);
			gmt_minute = strtof(str1, NULL);

			// Get GMT day of week

			strftime(str1, 30, "%u", current_time);
			gmt_day_of_week = strtof(str1, NULL);

			// Check for new GMT day

			if (gmt_day_of_week != gmt_day_of_week_last) {

				// Save new GMT day as last GMT day

				gmt_day_of_week_last = gmt_day_of_week;

				// Get GMT year

				strftime(str1, 30, "%Y", current_time);
				a = strtof(str1, NULL);									// Y

				// Get GMT month

				strftime(str1, 30, "%-m", current_time);
				b = strtof(str1, NULL);									// M

				// Get GMT day

				strftime(str1, 30, "%-d", current_time);
				c = strtof(str1, NULL);									// D

				// Calculate the Julian date
				// source: https://quasar.as.utexas.edu/BillInfo/JulianDatesG.html

				if (b <= 2) {
					a = a - 1;
					b = b + 12;
				}
				j = a / 100;											// A
				k = j / 4;												// B
				l = 2 - j + k;											// C
				a = 365.25 * (a + 4716);
				m = a;													// E
				a = 30.6001 * (b + 1);
				n = a;													// F
				julian_date = l + c + m + n - 1524.5;					// Julian date

				// Partial calculations for Sunrise, Sunset, and twilight times
				// source: NOAA Solar Calculations spreadsheet

				aa = (julian_date - 2451545.0) / 36525.0;													// Julian century
				ab = fmodf(280.46646 + aa * (36000.76983 + aa * 0.0003032), 360.0);							// Geom Mean Long Sun
				ac = 357.52911 + aa * (35999.05029 - 0.0001537 * aa);										// Geom Mean Anom Sun
				ad = 0.016708634 - aa * (0.000042037 + 0.0000001267 * aa);									// Eccent Earth Orbit
				ae = sinf(ac * (M_PI / 180.0)) * (1.914602 - aa * (0.004817 + 0.000014 * aa)) + sinf(2.0 * ac * (M_PI / 180.0)) * (0.019993 - 0.000101 * aa) + sinf(3.0 * ac * (M_PI / 180.0)) * 0.000289;		// SunEq of Ctr
				af = ab + ae;																				// Sun True Long
				ag = af - 0.00569 - 0.00478 * sinf((125.04 - 1934.136 * aa) * (M_PI / 180.0));				// Sun App Long
				ah = 23.0 + (26.0 + ((21.448 - aa * (46.815 + aa * (0.00059 - aa * 0.001813)))) / 60.0) / 60.0;		// Mean Obliq Ecliptic
				ai = ah + 0.00256 * cosf(125.04 - 1934.136 * aa * (M_PI / 180.0));							// Obliq Corr
				aj = asinf(sinf(ai * (M_PI / 180.0)) * sinf(ag * (M_PI / 180.0))) * (180.0 / M_PI);			// Sun Declin
				ak = tanf((ai / 2.0) * (M_PI / 180.0)) * tanf((ai / 2.0) * (M_PI / 180.0));					// vary
				al = 4.0 * (((ak * sinf(2.0 * ab * (M_PI / 180.0))) - (2.0 * ad * sinf(ac * (M_PI / 180.0))) + (4.0 * ad * ak * sinf(ac * (M_PI / 180.0)) * cosf(2.0 * (ab * (M_PI / 180.0)))) - (0.5 * ak * ak * sinf(4.0 * (ab * (M_PI / 180.0)))) - (1.25 * ad * ad * sinf(2.0 * (ac * (M_PI / 180.0))))) * (180.0 / M_PI));		// Eq of Time
			}

			// Get current local date and time info

			current_time = localtime(&curtime);

			// Get local hour (24 hour format)

			strftime(str1, 40, "%k", current_time);
			loc_hour_24 = strtof(str1, NULL);

			// Get local hour (12 hour format)

			strftime(str1, 40, "%l", current_time);
			loc_hour_12 = strtof(str1, NULL);

			// Get local minute

			strftime(str1, 40, "%M", current_time);
			loc_minute = strtof(str1, NULL);

			// Get local time zone

			strftime(str1, 30, "%z", current_time);
			loc_time_zone = strtof(str1, NULL) / 100;

			// Check for local time zone change (spring forward, fall back)

			if (loc_time_zone != loc_time_zone_last) {

				// Save new local time zone as last and set local day update flag

				loc_time_zone_last = loc_time_zone;
				loc_day_update_flag = 1;
			}

			// Get local day of week

			strftime(str1, 30, "%u", current_time);
			loc_day_of_week = strtof(str1, NULL);

			// Check for new local day

			if (loc_day_of_week != loc_day_of_week_last) {

				// Save new local day as last local day, and set local day update flag

				loc_day_of_week_last = loc_day_of_week;
				loc_day_update_flag = 1;
			}

			// Check for local day update flag

			if (loc_day_update_flag != 0) {

				// Final calculations for Sunrise, Sunset, and twilight times

				am = (720.0 - 4.0 * longitude - al + loc_time_zone * 60.0) / 1440.0;						// Solar Noon
				an = (acosf(cosf(90.833 * (M_PI / 180.0)) / (cosf(latitude * (M_PI / 180.0)) * cosf(aj * (M_PI / 180.0))) - tanf(latitude * (M_PI / 180.0)) * tanf(aj * (M_PI / 180.0)))) * (180.0 / M_PI);		// HA Sunrise
				a = am - an * 4.0 / 1440.0;																	// Sunrise
				sunrise = (a * 1440.0) + 0.5;																// Sunrise in minutes (rounded up/down)
				a = am + an * 4.0 / 1440.0;																	// Sunset
				sunset = (a * 1440.0) + 0.5;																// Sunset in minutes (rounded up/down)
				an = (acosf(cosf(96.0 * (M_PI / 180.0)) / (cosf(latitude * (M_PI / 180.0)) * cosf(aj * (M_PI / 180.0))) - tanf(latitude * (M_PI / 180.0)) * tanf(aj * (M_PI / 180.0)))) * (180.0 / M_PI);		// HA Civil Twilight
				a = am - an * 4.0 / 1440.0;																	// Civil Twilight in the morning
				civil_twilight_am = (a * 1440.0) + 0.5;														// Civil Twilight in the morning in minutes (rounded up/down)
				a = am + an * 4.0 / 1440.0;																	// Civil Twilight in the evening
				civil_twilight_pm = (a * 1440.0) + 0.5;														// Civil Twilight in the evening in minutes (rounded up/down)
				an = (acosf(cosf(102.0 * (M_PI / 180.0)) / (cosf(latitude * (M_PI / 180.0)) * cosf(aj * (M_PI / 180.0))) - tanf(latitude * (M_PI / 180.0)) * tanf(aj * (M_PI / 180.0)))) * (180.0 / M_PI);		// HA Nautical Twilight
				a = am - an * 4.0 / 1440.0;																	// Nautical Twilight in the morning
				nautical_twilight_am = (a * 1440.0) + 0.5;													// Nautical Twilight in the morning in minutes (rounded up/down)
				a = am + an * 4.0 / 1440.0;																	// Nautical Twilight in the evening
				nautical_twilight_pm = (a * 1440.0) + 0.5;													// Nautical Twilight in the evening in minutes (rounded up/down)
			}
		}

		// Update display per display mode

		// Note: current_time must be set to current local date and time info before updating the display

		switch(display_mode) {

			case 0:		// Century - 12H time only

				if (((time_update_flag != 0) && (second == 0)) || (loc_day_update_flag != 0)) {
					SelectFont(&FontCentury238);
					sprintf(str1, "%-d:%02d", loc_hour_12, loc_minute);			// ex. "11:45"
					i = GetStringLength(str1);
					if (i < string_length_last) {
						ScreenClear(BLACK);
					}
					string_length_last = i;
					SelectColors(WHITE, BLACK);
					PrintStringCenter(str1, 399, 49);
				}

				break;

			case 1:		// Century - 12H time, with day on top and date on bottom

				if (((time_update_flag != 0) && (second == 0)) || (loc_day_update_flag != 0)) {
					SelectFont(&FontCentury238);
					sprintf(str1, "%-d:%02d", loc_hour_12, loc_minute);			// ex. "11:45"
					i = GetStringLength(str1);
					if ((i < string_length_last) || (loc_day_update_flag != 0)) {
						ScreenClear(BLACK);
						loc_day_update_flag = 1;
					}
					string_length_last = i;
					SelectColors(WHITE, BLACK);
					PrintStringCenter(str1, 399, 49);
					SelectFont(&FontCentury36);									// Note: Have to always do the top line as the time update cuts off lower case descenders
					strftime(str1, 40, "%A", current_time);						// ex. "Sunday"
					SelectColors(WHITE, BLACK);
					PrintStringCenter(str1, 399, 0);							// Print top line
					if (loc_day_update_flag != 0) {
						strftime(str1, 40, "%B %-d, %Y", current_time);			// ex. "November 27, 2022"
						SelectColors(WHITE, BLACK);
						PrintStringCenter(str1, 399, 422);						// Print bottom line
					}
				}

				break;

			case 2:		// Arial, 12H time only

				if (((time_update_flag != 0) && (second == 0)) || (loc_day_update_flag != 0)) {
					SelectFont(&FontArial153);
					sprintf(str1, "%-d:%02d", loc_hour_12, loc_minute);			// ex. "11:45"
					if (loc_hour_24 <= 11) {
						strcat(str1, "am");										// ex. "11:45am"
					} else {
						strcat(str1, "pm");
					}
					i = GetStringLength(str1);
					if (i < string_length_last) {
						ScreenClear(BLACK);
					}
					string_length_last = i;
					SelectColors(WHITE, BLACK);
					PrintStringCenter(str1, 399, 166);
				}

				break;

			case 3:		// Arial - 12H time, with day and date on bottom

				if (((time_update_flag != 0) && (second == 0)) || (loc_day_update_flag != 0)) {
					SelectFont(&FontArial153);
					sprintf(str1, "%-d:%02d", loc_hour_12, loc_minute);			// ex. "11:45"
					if (loc_hour_24 <= 11) {
						strcat(str1, "am");										// ex. "11:45am"
					} else {
						strcat(str1, "pm");
					}
					i = GetStringLength(str1);
					if (i < string_length_last) {
						ScreenClear(BLACK);
						loc_day_update_flag = 1;
					}
					string_length_last = i;
					SelectColors(WHITE, BLACK);
					PrintStringCenter(str1, 399, 166);
					if (loc_day_update_flag != 0) {
						SelectFont(&FontArial64);
						strftime(str1, 40, "%a, %b %-d, %Y", current_time);		// ex. "Sun, Nov 27, 2022"
						SelectColors(WHITE, BLACK);
						PrintStringCenter(str1, 399, 396);						// Print bottom line
					}
				}

				break;

			case 4:		// Arial - 12H time, with day on top and date on bottom

				if (((time_update_flag != 0) && (second == 0)) || (loc_day_update_flag != 0)) {
					SelectFont(&FontArial153);
					sprintf(str1, "%-d:%02d", loc_hour_12, loc_minute);			// ex. "11:45"
					if (loc_hour_24 <= 11) {
						strcat(str1, "am");										// ex. "11:45am"
					} else {
						strcat(str1, "pm");
					}
					i = GetStringLength(str1);
					if (i < string_length_last) {
						ScreenClear(BLACK);
						loc_day_update_flag = 1;
					}
					string_length_last = i;
					SelectColors(WHITE, BLACK);
					PrintStringCenter(str1, 399, 166);
					if (loc_day_update_flag != 0) {
						SelectFont(&FontArial64);
						strftime(str1, 40, "%A", current_time);					// ex. "Sunday"
						SelectColors(WHITE, BLACK);
						PrintStringCenter(str1, 399, 0);						// Print top line
						strftime(str1, 40, "%B %-d, %Y", current_time);			// ex. "November 27, 2022"
						SelectColors(WHITE, BLACK);
						PrintStringCenter(str1, 399, 396);						// Print bottom line
					}
				}

				break;

			case 5:		// Astronomical

				if (time_update_flag != 0) {
					SelectFont(&FontArial32);
					SelectColors(WHITE, BLACK);

					// Check for local day update flag

					if (loc_day_update_flag != 0) {

						ScreenClear(BLACK);

						// Display various labels

						PrintStringLeft("Julian date:", 0, 0);
						PrintStringLeft("GMT time:", 0, 50);
						PrintStringLeft("Local time/zone:", 0, 100);
						PrintStringLeft("Lunar day:", 0, 400);

						// Display latitude

						a = fabsf(latitude);
						i = a;													// Degrees
						a = 60 * (a - i);
						j = a;													// Minutes
						a = 60 * (a - j);										// Seconds
						sprintf(str1, "Lat:%3d\xF8%02d'%02.1f\"", i, j, a);
						if (latitude >= 0) {
							strcat(str1, "N");
						} else {
							strcat(str1, "S");
						}
						PrintStringLeft(str1, 0, 150);

						// Display longitude

						a = fabsf(longitude);;
						i = a;													// Degrees
						a = 60 * (a - i);
						j = a;													// Minutes
						a = 60 * (a - j);										// Seconds
						sprintf(str1, "Long:%3d\xF8%02d'%02.1f\"", i, j, a);
						if (longitude >= 0) {
							strcat(str1, "E");
						} else {
							strcat(str1, "W");
						}
						PrintStringLeft(str1, 400, 150);

						// Display sunrise and sunset times

						PrintStringLeft("Sunrise/Sunset:", 25, 200);

						i = sunrise / 60;
						j = sunrise - (i * 60);
						if (i <= 11) {
							if (i == 0) {
								i = 12;
							}
						} else {
							if (i > 12) {
								i = i - 12;
							}
						}
						sprintf(str1, " %2d:%02d", i, j);
						if (sunrise < 720) {
							strcat(str1, "am");
						} else {
							strcat(str1, "pm");
						}
						PrintStringLeft(str1, 400, 200);

						i = sunset / 60;
						j = sunset - (i * 60);
						if (i <= 11) {
							if (i == 0) {
								i = 12;
							}
						} else {
							if (i > 12) {
								i = i - 12;
							}
						}
						sprintf(str1, " %2d:%02d", i, j);
						if (sunset < 720) {
							strcat(str1, "am");
						} else {
							strcat(str1, "pm");
						}
						PrintStringLeft(str1, 600, 200);

						// Display civil twilight times

						PrintStringLeft("Civil Twilight:", 25, 250);

						i = civil_twilight_am / 60;
						j = civil_twilight_am - (i * 60);
						if (i <= 11) {
							if (i == 0) {
								i = 12;
							}
						} else {
							if (i > 12) {
								i = i - 12;
							}
						}
						sprintf(str1, " %2d:%02d", i, j);
						if (civil_twilight_am < 720) {
							strcat(str1, "am");
						} else {
							strcat(str1, "pm");
						}
						PrintStringLeft(str1, 400, 250);

						i = civil_twilight_pm / 60;
						j = civil_twilight_pm - (i * 60);
						if (i <= 11) {
							if (i == 0) {
								i = 12;
							}
						} else {
							if (i > 12) {
								i = i - 12;
							}
						}
						sprintf(str1, " %2d:%02d", i, j);
						if (civil_twilight_pm < 720) {
							strcat(str1, "am");
						} else {
							strcat(str1, "pm");
						}
						PrintStringLeft(str1, 600, 250);

						// Display nautical twilight times

						PrintStringLeft("Nautical Twilight:", 25, 300);

						i = nautical_twilight_am / 60;
						j = nautical_twilight_am - (i * 60);
						if (i <= 11) {
							if (i == 0) {
								i = 12;
							}
						} else {
							if (i > 12) {
								i = i - 12;
							}
						}
						sprintf(str1, " %2d:%02d", i, j);
						if (nautical_twilight_am < 720) {
							strcat(str1, "am");
						} else {
							strcat(str1, "pm");
						}
						PrintStringLeft(str1, 400, 300);

						i = nautical_twilight_pm / 60;
						j = nautical_twilight_pm - (i * 60);
						if (i <= 11) {
							if (i == 0) {
								i = 12;
							}
						} else {
							if (i > 12) {
								i = i - 12;
							}
						}
						sprintf(str1, " %2d:%02d", i, j);
						if (nautical_twilight_pm < 720) {
							strcat(str1, "am");
						} else {
							strcat(str1, "pm");
						}
						PrintStringLeft(str1, 600, 300);
					}

					// Calculate Julian date for current GMT time

					julian_date_now = julian_date + ((gmt_hour_24 + (gmt_minute / 60.0) + (second / 3600.0)) / 24.0);			// Julian date at current GMT time

					// Display current Julian date

					sprintf(str1, "%.6lf", julian_date_now);
					PrintStringLeft(str1, 400, 0);

					// Display GMT time

					if (gmt_hour_24 < 10) {
						sprintf(str1, " %2d:%02d:%02d", gmt_hour_24, gmt_minute, second);
					} else {
						sprintf(str1, "%2d:%02d:%02d", gmt_hour_24, gmt_minute, second);
					}
					PrintStringLeft(str1, 400, 50);

					// Display local time and time zone

					if (loc_hour_24 < 10) {
						sprintf(str1, " %2d:%02d:%02d  %d", loc_hour_24, loc_minute, second, loc_time_zone);
					} else {
						sprintf(str1, "%2d:%02d:%02d  %d", loc_hour_24, loc_minute, second, loc_time_zone);
					}
					PrintStringLeft(str1, 400, 100);

					// Calculate the lunar day (i.e. number of days since the last new moon)
					// source: https://www.subsystems.us/uploads/9/8/9/4/98948044/moonphase.pdf
					// modified to use the April 8, 2024 18:21:00 GMT total solar eclipse

					lunar_day = fmod((julian_date_now - 2460409.264583), 29.530589);				// 2460409.264583 = new moon on April 8, 2024 18:21:00 GMT)

					// Display lunar day

					if (lunar_day < 10) {
						sprintf(str1, " %2.6f ", lunar_day);
					} else {
						sprintf(str1, "%2.6f ", lunar_day);
					}
					PrintStringLeft(str1, 400, 400);

					// Calculate moon BMP to display

					a = (lunar_day * (30 / 29.530589)) + 0.5;
					if (a >= 30) {
						a = a - 30;
					}
					i = a;											// i = moon BMP to display (0 - 29)
					j = &Moon_1_BMP - &Moon_0_BMP;					// j = size of each moon BMP
					bmp_adr = &Moon_0_BMP + (i * j);				// Calculate address of moon BMP to display
					DisplayBMP(bmp_adr, 650, 384);					// Display moon BMP
				}

				break;

		}

		// Check for minute update

		if ((second == 0) || (loc_day_update_flag != 0)) {

			// Check for automatically dim at night

			if (auto_dim_night != 0 ) {

				// Compute minute of the local day

				loc_minute_of_day = (loc_hour_24 * 60) + loc_minute;

				// Calculate brightness level

				if (loc_minute_of_day <= nautical_twilight_am) {
					i = night_bright;
				} else if ((loc_minute_of_day > nautical_twilight_am) && (loc_minute_of_day < sunrise)) {
					a = loc_minute_of_day - nautical_twilight_am;
					b = sunrise - nautical_twilight_am;
					c = day_bright - night_bright;
					i = ((a / b) * c) + night_bright;
				} else if ((loc_minute_of_day >= sunrise) && (loc_minute_of_day <= sunset)) {
					i = day_bright;
				} else if ((loc_minute_of_day > sunset) && (loc_minute_of_day < nautical_twilight_pm)) {
					a = loc_minute_of_day - sunset;
					b = nautical_twilight_pm - sunset;
					c = day_bright - night_bright;
					i = day_bright - ((a / b) * c);
				} else {
					i = night_bright;
				}

				// Set brightness level

				pwmWrite(18, i);
			}
		}

		// Clear the flags

		time_update_flag = 0;
		loc_day_update_flag = 0;

		// Optionally create a BMP from the current display screen

		if(save_bmp_flag == 1) {

			// Open the file for writing

			file0 = fopen("print_screen.bmp", "wb");

			// Create the BMP file header (14 bytes)

			i = 0x42;											// "B"
			fputc(i, file0);
			i = 0x4D;											// "M"
			fputc(i, file0);
			i = 14 + 40 + (vinfo.yres * vinfo.xres * 3);		// File size in bytes
			j = i & 0xFF;
			fputc(j, file0);
			j = (i >> 8) & 0xFF;
			fputc(j, file0);
			j = (i >> 16) & 0xFF;
			fputc(j, file0);
			j = (i >> 24) & 0xFF;
			fputc(j, file0);
			i = 0;												// Reserved #1 (half word)
			fputc(i, file0);
			fputc(i, file0);
			i = 0;												// Reserved #2 (half word)
			fputc(i, file0);
			fputc(i, file0);
			i = 14 + 40;										// Offset to pixel data
			j = i & 0xFF;
			fputc(j, file0);
			j = (i >> 8) & 0xFF;
			fputc(j, file0);
			j = (i >> 16) & 0xFF;
			fputc(j, file0);
			j = (i >> 24) & 0xFF;
			fputc(j, file0);

			// Create Image header (40 bytes)

			i = 40;												// Image header size
			j = i & 0xFF;
			fputc(j, file0);
			j = (i >> 8) & 0xFF;
			fputc(j, file0);
			j = (i >> 16) & 0xFF;
			fputc(j, file0);
			j = (i >> 24) & 0xFF;
			fputc(j, file0);
			i = vinfo.xres;										// Image width in pixels
			j = i & 0xFF;
			fputc(j, file0);
			j = (i >> 8) & 0xFF;
			fputc(j, file0);
			j = (i >> 16) & 0xFF;
			fputc(j, file0);
			j = (i >> 24) & 0xFF;
			fputc(j, file0);
			i = vinfo.yres;										// Image height in pixels
			j = i & 0xFF;
			fputc(j, file0);
			j = (i >> 8) & 0xFF;
			fputc(j, file0);
			j = (i >> 16) & 0xFF;
			fputc(j, file0);
			j = (i >> 24) & 0xFF;
			fputc(j, file0);
			i = 1;												// Number of planes
			j = i & 0xFF;
			fputc(j, file0);
			j = (i >> 8) & 0xFF;
			fputc(j, file0);
			i = 24;												// bits per pixel
			j = i & 0xFF;
			fputc(j, file0);
			j = (i >> 8) & 0xFF;
			fputc(j, file0);
			i = 0;												// Compression type (0 = uncompressed)
			j = i & 0xFF;
			fputc(j, file0);
			j = (i >> 8) & 0xFF;
			fputc(j, file0);
			j = (i >> 16) & 0xFF;
			fputc(j, file0);
			j = (i >> 24) & 0xFF;
			fputc(j, file0);
			i = vinfo.yres * vinfo.xres * 3;					// Image size in bytes
			j = i & 0xFF;
			fputc(j, file0);
			j = (i >> 8) & 0xFF;
			fputc(j, file0);
			j = (i >> 16) & 0xFF;
			fputc(j, file0);
			j = (i >> 24) & 0xFF;
			fputc(j, file0);
			i = 0;												// Prefered horizontal resolution in pixels per meter (0 = none)
			j = i & 0xFF;
			fputc(j, file0);
			j = (i >> 8) & 0xFF;
			fputc(j, file0);
			j = (i >> 16) & 0xFF;
			fputc(j, file0);
			j = (i >> 24) & 0xFF;
			fputc(j, file0);
			i = 0;												// Prefered vertical resolution in pixels per meter (0 = none)
			j = i & 0xFF;
			fputc(j, file0);
			j = (i >> 8) & 0xFF;
			fputc(j, file0);
			j = (i >> 16) & 0xFF;
			fputc(j, file0);
			j = (i >> 24) & 0xFF;
			fputc(j, file0);
			i = 0;												// Number of color map entries (0 = none, or not used)
			j = i & 0xFF;
			fputc(j, file0);
			j = (i >> 8) & 0xFF;
			fputc(j, file0);
			j = (i >> 16) & 0xFF;
			fputc(j, file0);
			j = (i >> 24) & 0xFF;
			fputc(j, file0);
			i = 0;												// Number of significant colors (0 = none, or not used)
			j = i & 0xFF;
			fputc(j, file0);
			j = (i >> 8) & 0xFF;
			fputc(j, file0);
			j = (i >> 16) & 0xFF;
			fputc(j, file0);
			j = (i >> 24) & 0xFF;
			fputc(j, file0);

			// Create pixel data from screen

			for (l = (vinfo.yres - 1); l >= 0; l--) {
				for (k = 0; k < vinfo.xres; k++) {
					i = GetPixelColor(k, l);
					j = i & 0xFF;								// Get red value
					fputc(j, file0);
					j = (i >> 8) & 0xFF;						// Get green value
					fputc(j, file0);
					j = (i >> 16) & 0xFF;						// Get blue value
					fputc(j, file0);
				}
			}

			// Close the BMP file

			fclose(file0);

			// Clear save BMP flag

			save_bmp_flag = 0;

		}
	}

   return 0;
}
