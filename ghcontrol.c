/**Gh control function
 *@brief implementation of code functions
 * @file ghcintrol.c
 */
#include "ghcontrol.h"

/** Function that prints the program header
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param sname string with Operator's name
* @return void
*/
void GhDisplayHeader(const char* sname){
	fprintf(stdout,"%s's CENG153 Greenhouse Controller\n", sname);
}

/** Function that making a random number in range between 0 and "range"
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param range
* @return int
*/
int GhGetRandom(int range){
	return rand()%range;
}

/** Function that create delay beetwen printing lines
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param delay milliseconds
* @return void
*/
void GhDelay(int milliseconds){
	long wait;
	clock_t now, start;

	wait = milliseconds*(CLOCKS_PER_SEC/1000);
	start = clock();
	now = start;
	while((now-start) < wait){
		now = clock();
	}
}

/** Initializing function for targets and header
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param void
* @return void
*/
void GhControllerInit(){
	srand((unsigned) time(NULL));
	GhSetTargets();
	GhDisplayHeader("Mykyta Dymchenko");
}

/** Initializing function for targets and header
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param struct control
* @return void
*/
void GhDisplayControls(controls_s ctrl){fprintf(stdout, "\n Controls\tHeater: %d\tHumidifier: %d", ctrl.heater, ctrl.humidifier);}

/** Function that displays read values
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param readings_s data
* @return void
*/
void GhDisplayReadings(readings_s rdata){
	fprintf(stdout,"\nUnit: %LX %s Readings\tT: %5.1fC\tH: %5.1f%\tP: %6.1fmb \n", ShGetSerial(),ctime(&rdata.rtime), rdata.temperature, rdata.humidity, rdata.pressure);
}

/** Function that display seted points for readings
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param void
* @return void
*/
void GhDisplayTargets(setpoints_s spts){
    fprintf(stdout, " Setpoints\tT: %5.1fC\tH: %5.1f%\t", spts.temperature, spts.humidity);
}

/** Function that display seted points for readings
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param struct setpoint target, readings_s rdata
* @return controls_s
*/
controls_s GhSetControls(setpoints_s target, readings_s rdata){
	controls_s cset = {0};
	cset.heater = (rdata.temperature < target.temperature) ? ON : OFF;
	cset.humidifier = (rdata.humidity < target.humidity) ? ON : OFF;
    return cset;
}

/** Function that set target values
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param void
* @return setpoints_s
*/
setpoints_s GhSetTargets(){
    setpoints_s cpoints = GhRetriveSetpoints("setpoints.dat");

    if(cpoints.temperature == 0 || cpoints.humidity == 0){
        cpoints.temperature = STEMP;
        cpoints.humidity = SHUMID;
        GhSaveSetpoints("setpoints.dat", cpoints);
    }

    return cpoints;
}
/** Function that set a random number to humidity readings
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param void
* @return float
*/
float GhGetHumidity(){
    #if SIMHUMIDITY
        return GhGetRandom(USHUMID-LSHUMID)+LSHUMID;
    #else
        return ShGetHumidity();
    #endif
}

/** Function that set a random number to pressure readings
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param void
* @return float
*/
float GhGetPressure(){
    #if SIMPRESSURE
        return GhGetRandom(USPRESS-LSPRESS)+LSPRESS;
    #else
        return ShGetPressure();
    #endif
}

/** Function that set a random number to temperature readings
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param void
* @return float
*/
float GhGetTemperature(){
    #if SIMTEMPERATURE
        return GhGetRandom(USTEMP-LSTEMP)+LSTEMP;
    #else
        return ShGetTemperature();
    #endif
}

/** Function that transfer all readings values into readings array
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param void
* @return readings_s
*/
readings_s GhGetReadings(){
	readings_s now = {0};

	now.rtime = time(NULL);
	now.temperature = GhGetTemperature();
	now.humidity = GhGetHumidity();
	now.pressure = GhGetPressure();
	return now;
}
/** Function that open file to write reading data
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param char*, readings_s
* @return int
*/
int GhLogData(char *fname, readings_s ghdata){
    FILE *fp;
    char ltime[CTIMESTRSZ];

    fp = fopen(fname, "a");
    if(!fp){
        fprintf(stdout, "\nCan't open file, data not retrived!\n");
        return 0;
    }
    strcpy(ltime, ctime(&ghdata.rtime));
    ltime[3] = ',', ltime[7] = ',', ltime[10] = ',', ltime[19] = ',';
    fprintf(fp, "\n%.24s,%5.1lf,%5.1lf,%6.1lf", ltime, ghdata.temperature, ghdata.humidity, ghdata.pressure);
    fclose(fp);
    return 1;
}
/** Function that open file to save setpoint into it
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param char*, setpoints_s
* @return int
*/
int GhSaveSetpoints(char *fname, setpoints_s spts){
    FILE *fp;
    fp = fopen(fname, "w");
    if(!fp){
        fprintf(stdout, "\nCan't open file, data not retrived!\n");
        return 0;
    }
    fwrite(&spts, sizeof(setpoints_s), 1, fp);
    fclose(fp);
    return 1;
}
/** Function that open file retrive data from file
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param char*
* @return setpoints_s
*/
setpoints_s GhRetriveSetpoints(char *fname){
    setpoints_s spts = {0.0};
    FILE *fp;
    fp = fopen(fname, "r");
    if(!fp){
        fprintf(stdout, "\nCan't open file, data not retrived!\n");
        return spts;
    }
    fread(&spts, sizeof(setpoints_s), 1, fp);
    fclose(fp);
    return spts;
}
/** Function that light vertical pixer of reading values
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param int, COLOR_SENSEHAT, uint_, struct fb_t
* @return int
*/
int GhSetVerticalBar(int bar, COLOR_SENSEHAT pxc, uint8_t value, struct fb_t *fb){
    if(value > 7) value =7;
    if(!((bar >=0 && bar <8)&&(value >= 0 && value <8))) return EXIT_FAILURE;
    for(int i = 0; i <= value; i++) ShLightPixel(i, bar, pxc, fb);
    for(int i = value+1; i != 8; i++) ShLightPixel(i, bar, BLACK, fb);
    return EXIT_SUCCESS;
}
/** Function that reassing values to light pixels
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param readings_s, setpoints_s, struct fb_t
* @return void
*/
void GhDisplayAll(readings_s rd, setpoints_s sd, struct fb_t *fb){
    int rv,sv,avh,avl;
    COLOR_SENSEHAT pxc;
    ShWipeScreen(BLACK, fb);

    rv = (8.0*(((rd.temperature-LSTEMP)/(USTEMP-LSTEMP))+0.05))-1.0;
    GhSetVerticalBar(TBAR, GREEN, rv, fb);

    rv = (8.0*(((rd.humidity-LSHUMID)/(USHUMID-LSHUMID))+0.05))-1.0;
    GhSetVerticalBar(HBAR, GREEN, rv, fb);

    rv = (8.0*(((rd.pressure-LSPRESS)/(USPRESS-LSPRESS))+0.05))-1.0;
    GhSetVerticalBar(PBAR, GREEN, rv, fb);

    sv = (8.0*(((sd.temperature-LSTEMP)/(USTEMP-LSTEMP))+0.05))-1.0;
    ShLightPixel(sv, TBAR, MAGENTA, fb);

    sv = (8.0*(((sd.humidity-LSHUMID)/(USHUMID-LSHUMID))+0.05))-1.0;
    ShLightPixel(sv, HBAR, MAGENTA, fb);

}
/** Function that assing value for limits which is used for alarm
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param void
* @return alarmlimits_s
*/
alarmlimits_s GhSetAlarmLimits(){
    alarmlimits_s calarm = {0};

    calarm.hight = UPPERATEMP;
    calarm.highh = UPPERAHUMID;
    calarm.highp = UPPERAPRESS;

    calarm.lowt = LOWERATEMP;
    calarm.lowh = LOWERAHUMID;
    calarm.lowp = LOWERAPRESS;

    return calarm;
}
/** Function that assing array of readings values for alarm
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param alarm_s, alarmlimits_s, readings_s
* @return void
*/
alarm_s * GhSetAlarms(alarm_s *head, alarmlimits_s alarmpt, readings_s rdata){

    if(rdata.temperature >= alarmpt.hight)
        GhSetOneAlarm(HTEMP, rdata.rtime, rdata.temperature, head);
    else
        head = GhClearOneAlarm(HTEMP, head);


    if(rdata.temperature < alarmpt.lowt)
        GhSetOneAlarm(LTEMP, rdata.rtime, rdata.temperature, head);
    else
        head = GhClearOneAlarm(LTEMP, head);


    if(rdata.humidity >= alarmpt.highh)
        GhSetOneAlarm(HHUMID, rdata.rtime, rdata.humidity, head);
    else
        head = GhClearOneAlarm(HHUMID, head);


    if(rdata.humidity < alarmpt.lowh)
        GhSetOneAlarm(LHUMID, rdata.rtime, rdata.humidity, head);
    else
        head = GhClearOneAlarm(LHUMID, head);


    if(rdata.pressure >= alarmpt.highp)
        GhSetOneAlarm(HPRESS, rdata.rtime, rdata.pressure, head);
    else
        head = GhClearOneAlarm(HPRESS, head);


    if(rdata.pressure < alarmpt.lowp)
        GhSetOneAlarm(LPRESS, rdata.rtime, rdata.pressure, head);
    else
        head = GhClearOneAlarm(LPRESS, head);
    return head;
}
/** Function that print alarm messege if values below or above the limit
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param alarm_s
* @return void
*/
void GhDisplayAlarm(alarm_s *head){

    alarm_s *cur = head;
    fprintf(stdout, "\n\nAlarms\n");

    const char alarmnames[NALARMS][ALARMNMSZ] = {
        "No Alarm",
        "High Temperature",
        "Low Temperature",
        "High Humidity",
        "Low Humidity",
        "High Pressure",
        "Low Pressure"
    };

    while(cur){
        if(cur->code != NOALARM)
            fprintf(stdout, "%s Alarm %s", alarmnames[cur->code], ctime(&(cur->atime)));
        cur = cur->next;
    }
}
/** Function that seting and adding alarm object to the list
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param alarm_s
* @return void
*/
int GhSetOneAlarm(alarm_e code, time_t atime, double value, alarm_s* head){
    alarm_s *last, *cur = head;
    if(cur->code != 0){
        while(cur){
            if(cur->code == code) return 0;
            last = cur;
            cur = cur->next;
        }
    }
    else{
        cur->code = code, cur->value = value, cur->atime = atime, cur->next = NULL;
        return 0;
    }
    cur = calloc(1, sizeof(alarm_s));
    last->next = cur;
    cur->code = code, cur->value = value, cur->atime = atime, cur->next = NULL;
    return 1;
}
/** Function that clear alarm object if readings from sensor changed
* @version ceng153, Serial: 9a0f8a27
* @author Mykyta Dymchenko
* @since 2024-02-17
* @param alarm_s
* @return void
*/
alarm_s * GhClearOneAlarm(alarm_e code, alarm_s *head){
     alarm_s *cur = head, *last = head;
     if((cur->code == code) && !(cur->next)){
        cur->code = NOALARM;
        return head;
     }
     if((cur->code == code) && (cur->next)){
        cur->next = head;
        free(cur);
        return head;
     }
     while(cur){
        if(cur->code == code){
            last->next = cur->next;
            free(cur);
            return head;
        }
        last = cur;
        cur= cur->next;
     }
     return head;
}

