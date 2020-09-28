/* WARNING: Auto generated file. Do not modify. */
#ifndef __ECU_define__
#define __ECU_define__

#define CMD2_TASK_DATA_STANDBY 0x17
#define CMD2_TASK_DATA_CONNECT 0x10
#define CMD2_TASK_DATA_DISCONNECT 0x18
#define CMD2_TASK_DATA_PARAMETER 0x14
#define CMD2_TASK_DATA_CONFIGURATION 0x16
#define CMD2_TASK_DATA_REMOTE_LEARNING 0x19
#define CMD2_TASK_DATA_FAULTS 0x13
#define CMD2_TASK_DATA_DOWNLOAD 0x20
#define CMD2_TASK_DATA_CATEGORY 0xcc
#define CMD2_TASK_DATA_ZIR_ACTUATOR 0x21
#define CMD2_TASK_DATA_ACTUATOR 0x15
#define CMD2_TASK_DATA_SUMMARY 0x11
#define CMD2_TASK_DATA_IDENTIFICATION 0x12
#define CMD2_TASK_DATA_ت 0x1

#define STANDBY 0
#define CONNECT 1
#define DISCONNECT 2
#define FAULT_ERASE 3
#define PARAM_34711 4 /* Battery Voltage */
#define PARAM_34717 5 /* Throttle Position Voltage */
#define PARAM_34721 6 /* Coolant Temperature */
#define PARAM_34725 7 /* Coolant temperature voltage */
#define PARAM_34729 8 /* Manifold Air Temperature */
#define PARAM_34732 9 /* Manifold Air Temperature Voltage Sensor */
#define PARAM_34735 10 /* Manifold Pressure */
#define PARAM_34738 11 /* Manifold Pressure Voltage Sensor */
#define PARAM_34741 12 /* vehicle speed */
#define PARAM_34745 13 /* Coil charging Timing */
#define PARAM_34772 14 /* Idle Adaptation */
#define PARAM_34775 15 /* Stepper Motor */
#define PARAM_34778 16 /* Ignition Advance */
#define PARAM_34781 17 /* Upstream O2 Sensor Voltage */
#define PARAM_34785 18 /* Upstream O2 Sensor Heating */
#define PARAM_34789 19 /* Idle */
#define PARAM_34792 20 /* Canister Valve Openning */
#define PARAM_34795 21 /* Mean O2 Correct */
#define PARAM_34798 22 /* Mileage */
#define PARAM_34801 23 /* Engine State */
#define PARAM_34811 24 /* Fan Relay */
#define PARAM_34880 25 /* Fuel pump status */
#define PARAM_34904 26 /* Fuel Cut */
#define PARAM_34912 27 /* Richness Regulation Status */
#define PARAM_34928 28 /* Gear */
#define PARAM_34940 29 /* Run Time With VSS Error */
#define PARAM_34943 30 /* Vehicle Speed Sensor Error */
#define PARAM_34946 31 /* Additive Correct Mixture Adaptive */
#define PARAM_34949 32 /* Multiple Correct Mixture Adaptive */
#define PARAM_34952 33 /* Coolong Fan Voltage */
#define PARAM_34955 34 /* Air Condition Switch */
#define PARAM_34964 35 /* Idle correction A/C on */
#define PARAM_35040 36 /* A/C Compressor Relay */
#define PARAM_35045 37 /* MAF Correct Altitude */
#define PARAM_35055 38 /* Switch Status */
#define PARAM_35071 39 /* Knock */
#define PARAM_35128 40 /* Throttle Position Angle */
#define PARAM_35145 41 /* Mass Air Flow */
#define PARAM_35191 42 /* Engine Speed */
#define PARAM_35194 43 /* Evaporator temp */
#define PARAM_35207 44 /* AC Pressure switch */
#define PARAM_35213 45 /* Air Condition Thermostat  Switch */
#define PARAM_35218 46 /* Injection Time */
#define PARAM_35235 47 /* Start Angle of Injection */
#define PARAM_35239 48 /* Fuel shot off pattern */
#define PARAM_35244 49 /* Idle speed actuator opening */
#define PARAM_35249 50 /* Idle speed regulation */
#define PARAM_35297 51 /* Canister loading */
#define PARAM_35301 52 /* Target of stepper motor position */
#define PARAM_36639 53 /* Software Version */
#define PARAM_36653 54 /* Calibration Number */
#define PARAM_320838 55 /* Number of Faults */
#define MFAULTS_34689 56 /*Faults*/
#define MACTUATOR_34691 57 /*Actuators*/
#define MZIR_ACTUATOR_36486 58 /*Diagnostic Lamp*/
#define MZIR_ACTUATOR_36491 59 /*Fuel Pump Relay*/
#define MZIR_ACTUATOR_36500 60 /*A/C Compressor Relay*/
#define MZIR_ACTUATOR_36506 61 /*Low Speed Fan Relay*/
#define MZIR_ACTUATOR_36511 62 /*High Speed Fan Relay*/
#define MZIR_ACTUATOR_36519 63 /*Main Relay*/
#define MZIR_ACTUATOR_36533 64 /*Canister*/
#define MZIR_ACTUATOR_36541 65 /*Stepper Motor*/
#define MZIR_ACTUATOR_36555 66 /*Coil 1-4*/
#define MZIR_ACTUATOR_36560 67 /*Coil 2-3*/
#define MZIR_ACTUATOR_36597 68 /*Immobilizer Light*/
#define MZIR_ACTUATOR_36605 69 /*Hot Lamp*/
#define MACTUATOR_63596 70 /*Reset Adaptive Values*/
#define MZIR_ACTUATOR_151715 71 /*Injector 1 - Engine Off*/
#define MZIR_ACTUATOR_151719 72 /*Injector 1 - Engine On*/
#define MZIR_ACTUATOR_151724 73 /*Injector 2 - Engine Off*/
#define MZIR_ACTUATOR_151729 74 /*Injector 2 - Engine On*/
#define MZIR_ACTUATOR_151734 75 /*Injector 3 - Engine Off*/
#define MZIR_ACTUATOR_151739 76 /*Injector 3 - Engine On*/
#define MZIR_ACTUATOR_151744 77 /*Injector 1 - Engine Off*/
#define MZIR_ACTUATOR_151749 78 /*Injector 1 - Engine On*/
#define MZIR_ACTUATOR_157385 79 /*Reset All Adaptive Values*/
#define MZIR_ACTUATOR_157387 80 /*Altitude Compensation Via MAF*/
#define MZIR_ACTUATOR_157390 81 /*knock Adaptation*/
#define MZIR_ACTUATOR_157393 82 /*Throttle Position Sensor Adaptation*/
#define MZIR_ACTUATOR_157396 83 /*Injection Time Adaptitation*/
#define MZIR_ACTUATOR_157399 84 /*Idle Speed Adaptation*/
#define MZIR_ACTUATOR_157402 85 /*Segment Adaptation For Engine Roughness*/
#define MZIR_ACTUATOR_157405 86 /*Engine Roughness Adaptation For Catalyst Heating*/
#define MZIR_ACTUATOR_157408 87 /*CPS Adaptive*/

#define MSUMMARY_34677_IDX 0 /*Summary*/
#define MIDENTIFICATION_34679_IDX 1 /*identification*/
#define MPARAMETER_34681_IDX 3 /*Parameters*/
#define MPARAMETER_34683_IDX 11 /*All Parameters*/
#define MPARAMETER_34685_IDX 14 /*Dynamic Parameters*/
#define MPARAMETER_34687_IDX 15 /*Static Parameters*/
#define MFAULTS_34689_IDX 2 /*Faults*/
#define MACTUATOR_34691_IDX 4 /*Actuators*/
#define MZIR_ACTUATOR_36486_IDX 7 /*Diagnostic Lamp*/
#define MZIR_ACTUATOR_36491_IDX 8 /*Fuel Pump Relay*/
#define MZIR_ACTUATOR_36500_IDX 13 /*A/C Compressor Relay*/
#define MZIR_ACTUATOR_36506_IDX 16 /*Low Speed Fan Relay*/
#define MZIR_ACTUATOR_36511_IDX 18 /*High Speed Fan Relay*/
#define MZIR_ACTUATOR_36519_IDX 21 /*Main Relay*/
#define MZIR_ACTUATOR_36533_IDX 22 /*Canister*/
#define MZIR_ACTUATOR_36541_IDX 25 /*Stepper Motor*/
#define MZIR_ACTUATOR_36555_IDX 26 /*Coil 1-4*/
#define MZIR_ACTUATOR_36560_IDX 27 /*Coil 2-3*/
#define MZIR_ACTUATOR_36597_IDX 28 /*Immobilizer Light*/
#define MZIR_ACTUATOR_36605_IDX 29 /*Hot Lamp*/
#define MACTUATOR_63596_IDX 5 /*Reset Adaptive Values*/
#define MZIR_ACTUATOR_151715_IDX 30 /*Injector 1 - Engine Off*/
#define MZIR_ACTUATOR_151719_IDX 31 /*Injector 1 - Engine On*/
#define MZIR_ACTUATOR_151724_IDX 32 /*Injector 2 - Engine Off*/
#define MZIR_ACTUATOR_151729_IDX 33 /*Injector 2 - Engine On*/
#define MZIR_ACTUATOR_151734_IDX 34 /*Injector 3 - Engine Off*/
#define MZIR_ACTUATOR_151739_IDX 35 /*Injector 3 - Engine On*/
#define MZIR_ACTUATOR_151744_IDX 36 /*Injector 1 - Engine Off*/
#define MZIR_ACTUATOR_151749_IDX 37 /*Injector 1 - Engine On*/
#define MZIR_ACTUATOR_157385_IDX 6 /*Reset All Adaptive Values*/
#define MZIR_ACTUATOR_157387_IDX 9 /*Altitude Compensation Via MAF*/
#define MZIR_ACTUATOR_157390_IDX 10 /*knock Adaptation*/
#define MZIR_ACTUATOR_157393_IDX 12 /*Throttle Position Sensor Adaptation*/
#define MZIR_ACTUATOR_157396_IDX 17 /*Injection Time Adaptitation*/
#define MZIR_ACTUATOR_157399_IDX 19 /*Idle Speed Adaptation*/
#define MZIR_ACTUATOR_157402_IDX 20 /*Segment Adaptation For Engine Roughness*/
#define MZIR_ACTUATOR_157405_IDX 23 /*Engine Roughness Adaptation For Catalyst Heating*/
#define MZIR_ACTUATOR_157408_IDX 24 /*CPS Adaptive*/

#endif
/* WARNING: Auto generated file. Do not modify. */
