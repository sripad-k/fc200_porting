#ifndef TYPES_FCS_H
#define TYPES_FCS_H

typedef struct
{
    float fcs_mr_eul_rpy_d[3]; // deg
    float fcs_mr_yawrate_d;    // rad/s
    int8_t fcs_mr_yaw_hold;
    float fcs_ca_nu_des[4];   // thrust, Mx, My, Mz
    float fcs_ca_nu_alloc[4]; // thrust, Mx, My, Mz
    float fcs_ca_cerp[3];     // Mx, My, Mz
    float fcs_mr_h_d;
    float fcs_mr_hdot_d;
    float fcs_mr_h_hold;
    float fcs_fw_roll_d;  // deg
    float fcs_fw_pitch_d; // deg
    float fcs_fw_h_d;
    float fcs_fw_cas_d;         // m/s
    float fcs_mr_pit_intg;      // crossfeed
    int8_t fcs_mr_pit_intg_sat; // crossfeed
    float fcs_fw_pit_intg;      // crossfeed
    int8_t fcs_fw_pit_intg_sat; // crossfeed
} fcs_mi_fbctrl_data_t;

#endif // TYPES_FCS_H