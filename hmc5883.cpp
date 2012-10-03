/*
 * hmc5883.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: nemo
 */

#include "hmc5883.h"

#define HMC5883_BASE_ADDRESS	0x1E
#define	CONF_REG_A_ADDRESS		0x00
#define	CONF_REG_B_ADDRESS		0x01
#define	MODE_REG_ADDRESS		0x02
#define	DATA_REGS_BASE_ADDRESS	0x03
#define	STATUS_REG_ADDRESS		0x09
#define	IDENT_REGS_BASE_ADDRESS	0x0A

#define	CONTINUE_MEASUREMENT	0
#define	SINGLE_MEASUREMENT		1
#define	IDLE					2

#define RDY_BIT					0
#define	LOCK_BIT				1
#define	FULL_REG				2

#define DFLT	255

#define WRITE	0
#define READ	1


HMC5883 :: HMC5883(void){

}

void HMC5883 :: begin(uint8_t num_of_samples, uint8_t output_rate, uint8_t meas_conf, uint8_t gain_conf, uint8_t mode){

	Wire.begin();

	if(num_of_samples != DFLT){
		ConfReg.A &= 0x1F;
		ConfReg.A |= ( ( num_of_samples & 0x3 ) << 5 );
	}
	if(output_rate != DFLT){
		ConfReg.A &= 0x63;
		ConfReg.A |= ( (    output_rate & 0x7 ) << 2 );
	}
	if(meas_conf != DFLT){
		ConfReg.A &= 0x7C;
		ConfReg.A |= ( (      meas_conf & 0x3 ) << 0 );
	}
	if(gain_conf != DFLT){
		ConfReg.B &= 0x00;
		ConfReg.B |= ( (      gain_conf & 0x7 ) << 5 );
	}
	if(mode != DFLT){
		ModeReg   &= 0x00;
		ModeReg   |= ( (           mode & 0x3 ) << 0 );
	}

	this->writeReg(CONF_REG_A_ADDRESS, ConfReg.A);
	this->writeReg(CONF_REG_B_ADDRESS, ConfReg.B);
	this->writeReg(MODE_REG_ADDRESS,   ModeReg);
}

uint8_t HMC5883 :: getStatusReg(uint8_t bit_num){
	uint8_t tmp;
    Wire.beginTransmission(HMC5883_BASE_ADDRESS);
    Wire.write(STATUS_REG_ADDRESS);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)HMC5883_BASE_ADDRESS, (uint8_t)1, (uint8_t)true);
    tmp = Wire.read();
    Wire.endTransmission(true);

    if(bit_num < 2){
    	return (tmp >> bit_num)&0x1;
    }
    else{
    	return tmp & 0x3;
    }
}

bool HMC5883 :: isDataReady(void){
	uint8_t tmp;
    Wire.beginTransmission(HMC5883_BASE_ADDRESS);
    Wire.write(STATUS_REG_ADDRESS);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)HMC5883_BASE_ADDRESS, (uint8_t)1, (uint8_t)true);
    tmp = Wire.read();
    Wire.endTransmission(true);

   	return tmp & 0x1;
}

/*
void HMC5883 :: getIdentReg(uint8_t *dst){
    Wire.beginTransmission(HMC5883_BASE_ADDRESS);
    Wire.write(IDENT_REGS_BASE_ADDRESS);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)HMC5883_BASE_ADDRESS, (uint8_t)3, (uint8_t)true);
    dst[0] = Wire.read();
    dst[1] = Wire.read();
    dst[2] = Wire.read();
    Wire.endTransmission(true);
}
*/

void HMC5883 :: getDataReg(DATA_REGS *dst){
    Wire.beginTransmission(HMC5883_BASE_ADDRESS);
    Wire.write(DATA_REGS_BASE_ADDRESS);
    Wire.endTransmission();
    Wire.beginTransmission(HMC5883_BASE_ADDRESS);
    Wire.requestFrom((uint8_t)HMC5883_BASE_ADDRESS, (uint8_t)6);

    if(Wire.available() == 6){
    	dst->X.part.MSB = Wire.read();
    	dst->X.part.LSB = Wire.read();
    	dst->Z.part.MSB = Wire.read();
    	dst->Z.part.LSB = Wire.read();
    	dst->Y.part.MSB = Wire.read();
    	dst->Y.part.LSB = Wire.read();
    }

    Wire.endTransmission();
}

void HMC5883 :: writeReg(uint8_t addr, uint8_t value){
    Wire.beginTransmission(HMC5883_BASE_ADDRESS);
    Wire.write(addr);
    Wire.write(value);
    Wire.endTransmission(true);
}

void HMC5883 :: setMode(uint8_t mode){
	ModeReg   &= 0x00;
	ModeReg   |= (mode & 0x3);

	this->writeReg(MODE_REG_ADDRESS,   ModeReg);
}

uint8_t HMC5883 :: readReg(uint8_t addr){
    Wire.beginTransmission(HMC5883_BASE_ADDRESS);
    Wire.write(addr);
    Wire.endTransmission();
    Wire.beginTransmission(HMC5883_BASE_ADDRESS);
    Wire.requestFrom((uint8_t)HMC5883_BASE_ADDRESS, (uint8_t)1);
    return Wire.read();
}

