#include "mma8451q.h"
#include <stdbool.h>

static void pause(void)
{
    int n;
    for(n=0; n<40; n++)
        __asm("nop");
//    for(n=0; n<100; n++);
}

void mma8451q_init(void)
{
	InitI2C(); //Initialize I2C modules done in read/write functions itself
	
	unsigned char reg_val = 0;
    mma8451q_set(MMA8451Q_REG_CTRL2, 0x40); // Reset all registers to POR values
    do  // Wait for the RST bit to clear
    {
       reg_val = mma8451q_read(MMA8451Q_REG_CTRL2) & 0x40;
    }
	while (reg_val);
		
    mma8451q_set(MMA8451Q_REG_XYZ_DATA_CFG, 0x00); // +/-2g range -> 1g = 16384/4 = 4096 counts

    mma8451q_set(MMA8451Q_REG_CTRL2, 0x02);        // High Resolution mode

    mma8451q_set(MMA8451Q_REG_CTRL1, 0x11);        // ODR = 1.56Hz, Reduced noise,
}

void mma8451q_calib(void)
{
	unsigned char reg_val = 0;
	float x_offset, y_offset, z_offset;
	int16_t x_read_14bit, y_read_14bit, z_read_14bit;

    while (!reg_val)           // Wait for a first set of data         
    {
			reg_val = mma8451q_read(MMA8451Q_REG_STATUS) & 0x08;
    }

	x_read_14bit = ((int16_t) (mma8451q_read(MMA8451Q_REG_OUT_X_MSB)<<8 | mma8451q_read(MMA8451Q_REG_OUT_X_LSB))) >> 2;
	y_read_14bit = ((int16_t) (mma8451q_read(MMA8451Q_REG_OUT_Y_MSB)<<8 | mma8451q_read(MMA8451Q_REG_OUT_Y_LSB))) >> 2;
	z_read_14bit = ((int16_t) (mma8451q_read(MMA8451Q_REG_OUT_Z_MSB)<<8 | mma8451q_read(MMA8451Q_REG_OUT_Z_LSB))) >> 2;

    x_offset = x_read_14bit / 8 * (-1) - 1;  					// Compute X-axis offset correction value
    y_offset = y_read_14bit / 8 * (-1) - 1; 					// Compute Y-axis offset correction value
    z_offset = (z_read_14bit - 4096) / 8 * (-1) - 1; 	// Compute Z-axis offset correction value

    mma8451q_set(MMA8451Q_REG_CTRL1, 0x10); // Standby mode to allow writing to the offset registers 

    mma8451q_set(MMA8451Q_REG_OFF_X, x_offset);	// Write offset correction values to X   
    mma8451q_set(MMA8451Q_REG_OFF_Y, y_offset);	// Write offset correction values to Y	
    mma8451q_set(MMA8451Q_REG_OFF_Z, z_offset);	// Write offset correction values to Z
		
		mma8451q_set(MMA8451Q_REG_CTRL1, 0x11);  // ODR = 200Hz, Active mode		
		
}

void mma8451_get_data(mma8451q_data_t* data)
{
	int16_t x_out_14bit, y_out_14bit, z_out_14bit;
		x_out_14bit = ((int16_t) (mma8451q_read(MMA8451Q_REG_OUT_X_MSB)<<8 | mma8451q_read(MMA8451Q_REG_OUT_X_LSB))) >> 2;
		y_out_14bit = ((int16_t) (mma8451q_read(MMA8451Q_REG_OUT_Y_MSB)<<8 | mma8451q_read(MMA8451Q_REG_OUT_Y_LSB))) >> 2;
		z_out_14bit = ((int16_t) (mma8451q_read(MMA8451Q_REG_OUT_Z_MSB)<<8 | mma8451q_read(MMA8451Q_REG_OUT_Z_LSB))) >> 2;

		data->x_out = ((float) x_out_14bit) / 4096;
		data->y_out = ((float) y_out_14bit) / 4096;
		data->z_out = ((float) z_out_14bit) / 4096;
}

void mma8451q_set(uint8_t addr, uint8_t data)
{
	// Master & TX mode
	I2C0->C1 |=  I2C_C1_MST_MASK;
	I2C0->C1 |= I2C_C1_TX_MASK;
	
    WriteToI2C(MMA8451Q_I2C_ADDRESS|I2C_WRITE); //device ID
		I2CWait();
		I2C_Get_Ack();
	
    WriteToI2C(addr); //register address - C2
		I2CWait();
		I2C_Get_Ack();
	
    WriteToI2C(data); //register address - C1
		I2CWait();
		I2C_Get_Ack();
	
		// Slave & RX mode
		I2C0->C1 &= ~I2C_C1_MST_MASK;
		I2C0->C1 &= ~I2C_C1_TX_MASK;
		pause();
}

signed short mma8451q_read(uint8_t addr)
{
		uint8_t result;
			// Master & TX mode
		I2C0->C1  |=  I2C_C1_MST_MASK;
		I2C0->C1 |= I2C_C1_TX_MASK;
    WriteToI2C(MMA8451Q_I2C_ADDRESS|I2C_WRITE); //Device ID
	
		I2CWait();
		I2C_Get_Ack();
	
	  WriteToI2C(addr); //Device ID
		I2CWait();
		I2C_Get_Ack();
	
		I2C0->C1 |= 0x04;
		WriteToI2C(MMA8451Q_I2C_ADDRESS|I2C_READ);
		I2CWait();
		I2C_Get_Ack();
	
		I2C0->C1 &= ~I2C_C1_TX_MASK;
		I2C0->C1 |= I2C_C1_TXAK_MASK;
		result = I2C0->D;
		I2CWait();
		
		// Slave & RX mode
		I2C0->C1  &= ~I2C_C1_MST_MASK;
		I2C0->C1 &= ~I2C_C1_TX_MASK;
		result = I2C0->D;
		pause();
    return result;
}
