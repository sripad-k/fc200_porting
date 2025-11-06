/****************************************************
 *  da_adc_simtec.h
 *  Created on: 07-Jul-2025 3:53:26 PM
 *  Implementation of the Class da_adc_simtec
 *  Copyright: LODD (c) 2025
 ****************************************************/

#ifndef H_DA_ADC_SIMTEC
#define H_DA_ADC_SIMTEC

#include "uart_interface.h" /* Adjusted path for 'UART_interface' */
#include "types_ads.h"      /* Adjusted path for 'TYPES_ads' */
#include "da_interface.h"

/*
 *   NOTE: ADC-9 or 10 or 10X either one of them should be defined
 *   in the CMakeLists.txt of this domain based on the configured
 *   ADC in the system.
 */

typedef enum
{
    FLAG_VALID = 0,
    FLAG_RANGE_ABOVE = 1,
    FLAG_RANGE_BELLOW = 2,
    FLAG_INVALID_POS = 3,
    FLAG_INVALID_NEG = 4,
    FLAG_INVALID = 5

} da_ads_data_validity_t;

typedef struct
{
    float Data;
    da_ads_data_validity_t Data_Flag;

} da_ads_data_t;

typedef enum
{
    /* ---------- SOH1 Labels -------------*/
    SOH1_QC, /* Impact pressure */
    SOH1_PS, /* Static pressure */
/* Labels available in ADC_9 and ADC_10X only */
#if defined(ADC_9) || defined(ADC_10X)
    SOH1_AOA, /* Angle of attack */
    SOH1_AOS, /* Angle of slideslip */
#endif
    SOH1_CAS,  /* Calibrate airspeed */
    SOH1_TAS,  /* True airspeed */
    SOH1_HP,   /* Pressure altitude (see note) */
    SOH1_MACH, /* Mach Number */
    SOH1_SAT,  /* Static air temperature */
    SOH1_TAT,  /* Total air temperature */
               /* Label Exclusive for ADC_9 */
#ifdef ADC_9
    SOH1_PCB_TEMP, /* PCB Temperature */
#endif
    SOH1_QNH, /* QNH Setting */
              /* ------------------------------------*/

    /* ---------- SOH2 Labels -------------*/
    SOH2_CR, /* Climb rate */
#ifdef ADC_9
    SOH2_PT, /* Total pressure */
#endif
#if defined(ADC_10) || defined(ADC_10X)
    SOH2_CAS_R, /* CAS Rate */
    SOH2_TAS_R, /* TAS Rate */
#endif
    SOH2_HBARO, /* Barometric Altitude (QNH) */
#ifdef ADC_9
    SOH2_IAS, /* Indicated Airspeed */
    SOH2_VMO, /* Maximum Operating Airspeed */
#endif
/* ------------------------------------*/

/* ---------- SOH3 Labels -------------*/
#ifdef ADC_9
    SOH3_SP_QC,  /* Sensor Pressure Qc */
    SOH3_SP_PS,  /* Sensor Pressure Ps */
    SOH3_SP_AOA, /* Sensor Pressure Angle of Attack */
    SOH3_SP_AOS, /* Sensor Pressure Angle of Sideslip */
    SOH3_IAT,    /* Indicated Temperature */
    SOH3_BAT,    /* Base Temperature */
#endif
#ifdef ADC_10X
    SOH3_QCU, /* Uncorrected Impact Pressure */
    SOH3_PSU, /* Uncorrected Static Pressure */
#endif
    SOH3_ST_QC, /* Sensor Temperature Qc */
    SOH3_ST_PS, /* Sensor Temperature Ps */
#ifdef ADC_9
    SOH3_ST_AOA, /* Sensor Temperature Angle of Attack */
    SOH3_ST_AOS, /* Sensor Temperature Angle of Sideslip */
#endif
/* ------------------------------------*/

/* No Labels available under SOH4 in ADC-9, 10 and 10X */

/* ---------- SOH5 Labels -------------*/
#if defined(ADC_10) || defined(ADC_10X)
    SOH5_QC,    /* Impact Pressure */
    SOH5_PS,    /* Static Pressure */
    SOH5_HP,    /* Pressure Altitude */
    SOH5_HBARO, /* Barometric Altitude (QNH) */
    SOH5_CAS,   /* Calibrate Airspeed */
    SOH5_TAS,   /* True Airspeed */
    SOH5_CR,    /* Climb Rate */
#endif
/* ------------------------------------*/

/* ---------- SOH6 Labels -------------*/
#ifdef ADC_9
    SOH6_QC_I,     /* Indicated Impact Pressure */
    SOH6_PS_I,     /* Indicated Static Pressure */
    SOH6_PS_I_AOA, /* Indicated Angle of Attack */
    SOH6_PS_I_AOS, /* Indicated Angle of Sideslip */
    SOH6_PT_I,     /* Indicated Total Pressure */
#endif
    /* ------------------------------------*/
    MAX_ADC_LABEL_COUNT

} da_ads_label_t;

bool da_adc_9_init(void);
void da_adc_9_read_periodic(void);

#endif /*!defined(H_DA_ADC_SIMTEC)*/
