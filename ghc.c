/** @brief Serial: 9a0f8a27
 *  @brief main function with source code
 *  @file lab06.c
 */

#include "ghcontrol.h"

/**The main function in which functions are called to perfom certain operations
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param void
* @return int
*/
int main(void){

    struct fb_t *fb = ShInit(fb);
	int tempc, humidc, logged;
	readings_s creadings;
	setpoints_s cpoint;
	controls_s ctrl;
	GhControllerInit();
	alarmlimits_s alimits = {0};
	alarm_s *arecord;
    alimits = GhSetAlarmLimits();

    arecord = (alarm_s *) calloc(1, sizeof(alarm_s));
    if(!arecord){
        printf("\nCannot allocate memory\n");
        return EXIT_FAILURE;
    }

	for(;;){
		creadings=GhGetReadings();
		logged = GhLogData("ghdata.txt", creadings);
		cpoint = GhSetTargets();
		ctrl=GhSetControls(cpoint, creadings);
		arecord = GhSetAlarms(arecord, alimits, creadings);
		GhDisplayAll(creadings, cpoint, fb);
		GhDisplayReadings(creadings);
		GhDisplayTargets(cpoint);
		GhDisplayControls(ctrl);
		GhDisplayAlarm(arecord);
		GhDelay(GHUPDATE);
	}
	return EXIT_SUCCESS;
}

