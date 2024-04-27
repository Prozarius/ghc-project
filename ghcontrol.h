/** @drief Gh control constants, structures, function prototype
 * @brief Constants, structures, function prototypes for Gh control code
 * @file ghcontrol.h
 */
#ifndef GHCONTROL_H
#define GHCONTROL_H

//Includes
#include "led2472g.h"
#include "hts221.h"
#include "lps25h.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Constants
#define GHUPDATE 2000
#define SENSORS 3
#define TEMPERATURE 0
#define HUMIDITY 1
#define PRESSURE 2
#define SIMULATE 1
#define USTEMP 50
#define LSTEMP -10
#define USHUMID 100
#define LSHUMID 0
#define USPRESS 1016
#define LSPRESS 975
#define STEMP 25.0
#define SHUMID 55.0
#define ON 1
#define OFF 0
#define SIMTEMPERATURE 0
#define SIMHUMIDITY 0
#define SIMPRESSURE 0
#define CTIMESTRSZ 25
#define SENSEHAT 1
#define NUMBARS 8
#define NUMPTS 8.0
#define TBAR 7
#define HBAR 5
#define PBAR 3
#define NALARMS 7
#define LOWERATEMP 10
#define UPPERATEMP 30
#define LOWERAHUMID 25
#define UPPERAHUMID 70
#define LOWERAPRESS 985
#define UPPERAPRESS 1016
#define ALARMNMSZ 18

//numerated types
typedef enum { NOALARM, HTEMP, LTEMP, HHUMID, LHUMID, HPRESS, LPRESS} alarm_e;

//Structures



typedef struct readings{
    time_t rtime;
    float temperature;
    float humidity;
    float pressure;
}readings_s;

typedef struct setpoints{
    float temperature;
    float humidity;
}setpoints_s;

typedef struct controls{
    int heater;
    int humidifier;
}controls_s;

typedef struct alarmlimits{
    float hight;
    float lowt;
    float highh;
    float lowh;
    float highp;
    float lowp;
}alarmlimits_s;

typedef struct alarms{
    alarm_e code;
    time_t atime;
    float value;
    struct alarms *next;
}alarm_s;

///@cond INTERNAL
//Function Prototypes
int GhGetRandom(int range);
float GhGetHumidity();
float GhGetPressure();
float GhGetTemperature();
readings_s GhGetReadings();

void GhDelay(int milliseconds);
void GhControllerInit();

void GhDisplayHeader(const char* sname);
void GhDisplayControls(controls_s ctrl);
void GhDisplayReadings(readings_s rdata);
void GhDisplayTargets();
void GhDisplayAll(readings_s rd, setpoints_s sd, struct fb_t *fb);
void GhDisplayAlarm(alarm_s *head);

int GhLogData(char *fname, readings_s ghdata);

int GhSaveSetpoints(char *fname, setpoints_s spts);

setpoints_s GhRetriveSetpoints(char *fname);

controls_s GhSetControls(setpoints_s target, readings_s rdata);
setpoints_s GhSetTargets();
int GhSetVerticalBar(int bar, COLOR_SENSEHAT pxc, uint8_t value, struct fb_t *fb);
alarmlimits_s GhSetAlarmLimits();
alarm_s * GhSetAlarms(alarm_s *head, alarmlimits_s alarmpt, readings_s rdata);
int GhSetOneAlarm(alarm_e code, time_t atime, double value, alarm_s *head);
alarm_s * GhClearOneAlarm(alarm_e code, alarm_s *head);
///@endcond
#endif
