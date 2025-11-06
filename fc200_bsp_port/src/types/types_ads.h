#ifndef H_TYPES_ADS
#define H_TYPES_ADS

#include "type.h"

typedef enum
{
    /* ---------- SOH1 Labels -------------*/
    QC, /* Impact pressure */
    PS, /* Static pressure */
/* Labels available in ADC_9 and ADC_10X only */
#if defined(ADC_9) || defined(ADC_10X)
    AOA, /* Angle of attack */
    AOS, /* Angle of slideslip */
#endif
    CAS,  /* Calibrate airspeed */
    TAS,  /* True airspeed */
    HP,   /* Pressure altitude (see note) */
    MACH, /* Mach Number */
    SAT,  /* Static air temperature */
    TAT,  /* Total air temperature */
          /* Label Exclusive for ADC_9 */
#ifdef ADC_9
    PCB_TEMP, /* PCB Temperature */
#endif
    QNH, /* QNH Setting */
         /* ------------------------------------*/

    /* ---------- SOH2 Labels -------------*/
    CR, /* Climb rate */
#ifdef ADC_9
    PT, /* Total pressure */
#endif
#if defined(ADC_10) || defined(ADC_10X)
    CAS_R, /* CAS Rate */
    TAS_R, /* TAS Rate */
#endif
    HBARO, /* Barometric Altitude (QNH) */
#ifdef ADC_9
    IAS, /* Indicated Airspeed */
    VMO, /* Maximum Operating Airspeed */
#endif
/* ------------------------------------*/

/* ---------- SOH3 Labels -------------*/
#ifdef ADC_9
    SP_QC,  /* Sensor Pressure Qc */
    SP_PS,  /* Sensor Pressure Ps */
    SP_AOA, /* Sensor Pressure Angle of Attack */
    SP_AOS, /* Sensor Pressure Angle of Sideslip */
    IAT,    /* Indicated Temperature */
    BAT,    /* Base Temperature */
#endif
#ifdef ADC_10X
    QCU, /* Uncorrected Impact Pressure */
    PSU, /* Uncorrected Static Pressure */
#endif
    ST_QC, /* Sensor Temperature Qc */
    ST_PS, /* Sensor Temperature Ps */
#ifdef ADC_9
    ST_AOA, /* Sensor Temperature Angle of Attack */
    ST_AOS, /* Sensor Temperature Angle of Sideslip */
#endif
/* ------------------------------------*/

/* No Labels available under SOH4 in ADC-9, 10 and 10X */

/* ---------- SOH5 Labels -------------*/
#if defined(ADC_10) || defined(ADC_10X)
    QC,    /* Impact Pressure */
    PS,    /* Static Pressure */
    HP,    /* Pressure Altitude */
    HBARO, /* Barometric Altitude (QNH) */
    CAS,   /* Calibrate Airspeed */
    TAS,   /* True Airspeed */
    CR,    /* Climb Rate */
#endif
/* ------------------------------------*/

/* ---------- SOH6 Labels -------------*/
#ifdef ADC_9
    QC_I,     /* Indicated Impact Pressure */
    PS_I,     /* Indicated Static Pressure */
    PS_I_AOA, /* Indicated Angle of Attack */
    PS_I_AOS, /* Indicated Angle of Sideslip */
    PT_I,     /* Indicated Total Pressure */
#endif
} adc_data_type_t;

typedef struct
{
    float cas;             /* Calibrated Airspeed in m/s */
    float aoa;             /* Angle of Attack in degrees */
    float aos;             /* Angle of Sideslip in degrees */
    float alt_baro_amsl;   /* Barometric Altitude in meters above mean sea level */
    bool cas_invalid;      /* Flag to indicate if the calibrated airspeed is invalid */
    bool aoa_invalid;      /* Flag to indicate if the angle of attack is invalid */
    bool aos_invalid;      /* Flag to indicate if the angle of sideslip is invalid */
    bool alt_baro_invalid; /* Flag to indicate if the barometric altitude is invalid */
} std_ads_data_t;

#endif /* TYPES_ADS_H */