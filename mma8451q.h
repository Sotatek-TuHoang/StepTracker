#ifndef __MMA8451Q_H_
#define __MMA8451Q_H_

#include "MKL46Z4.h"
#include "I2C.h"

#define MMA8451Q_I2C_ADDRESS (0x1D << 1)

// register addresses
#define MMA8451Q_REG_STATUS              0x00
#define MMA8451Q_REG_F_STATUS            0x00
#define MMA8451Q_REG_OUT_X_MSB           0x01
#define MMA8451Q_REG_OUT_X_LSB           0x02
#define MMA8451Q_REG_OUT_Y_MSB           0x03
#define MMA8451Q_REG_OUT_Y_LSB           0x04
#define MMA8451Q_REG_OUT_Z_MSB           0x05
#define MMA8451Q_REG_OUT_Z_LSB           0x06

#define MMA8451Q_REG_F_SETUP             0x09
#define MMA8451Q_REG_TRIG_CFG            0x0a
#define MMA8451Q_REG_SYSMOD              0x0b
#define MMA8451Q_REG_INT_SOURCE          0x0c
#define MMA8451Q_REG_WHO_AM_I            0x0d
#define MMA8451Q_REG_XYZ_DATA_CFG        0x0e
#define MMA8451Q_REG_HP_FILTER_CUTOFF    0x0f
#define MMA8451Q_REG_PL_STATUS           0x10
#define MMA8451Q_REG_PL_CFG              0x11
#define MMA8451Q_REG_PL_COUNT            0x12
#define MMA8451Q_REG_PL_BF_ZCOMP         0x13
#define MMA8451Q_REG_PL_THS              0x14
#define MMA8451Q_REG_FF_MT_CFG           0x15
#define MMA8451Q_REG_FF_MT_SRC           0x16
#define MMA8451Q_REG_FF_MT_THS           0x17
#define MMA8451Q_REG_FF_MT_COUNT         0x18

#define MMA8451Q_REG_TRANSIENT_CFG       0x1d
#define MMA8451Q_REG_TRANSIENT_SRC       0x1e
#define MMA8451Q_REG_TRANSIENT_THS       0x1f
#define MMA8451Q_REG_TRANSIENT_COUNT     0x20
#define MMA8451Q_REG_PULSE_CFG           0x21
#define MMA8451Q_REG_PULSE_SRC           0x22
#define MMA8451Q_REG_PULSE_THSX          0x23
#define MMA8451Q_REG_PULSE_THSY          0x24
#define MMA8451Q_REG_PULSE_THSZ          0x25
#define MMA8451Q_REG_PULSE_TMLT          0x26
#define MMA8451Q_REG_PULSE_LTCY          0x27
#define MMA8451Q_REG_PULSE_WIND          0x28
#define MMA8451Q_REG_ASLP_COUNT          0x29
#define MMA8451Q_REG_CTRL1               0x2a
#define MMA8451Q_REG_CTRL2               0x2b
#define MMA8451Q_REG_CTRL3               0x2c
#define MMA8451Q_REG_CTRL4               0x2d
#define MMA8451Q_REG_CTRL5               0x2e
#define MMA8451Q_REG_OFF_X               0x2f
#define MMA8451Q_REG_OFF_Y               0x30
#define MMA8451Q_REG_OFF_Z               0x31

#define SENSORS_GRAVITY_EARTH (9.80665F) /**< Earth's gravity in m/s^2 */

typedef struct mma8451q_data {
	float x_out;
	float y_out;
	float z_out;
} mma8451q_data_t;

static void pause(void);
void mma8451q_init(void);
void mma8451q_set(uint8_t addr, uint8_t data);
signed short mma8451q_read(uint8_t addr);
void mma8451q_calib(void);
void mma8451_get_data(mma8451q_data_t* data);

#endif //__MMA8451Q_H_
