/*
 * hmc5883.h
 *
 *  Created on: Sep 26, 2012
 *      Author: nemo
 */

#ifndef HMC5883_H_
#define HMC5883_H_

#include <Arduino.h>
#include <inttypes.h>
#include <Wire.h>

struct PART{
	uint8_t LSB;
	uint8_t MSB;
};

union DATA_REG{
	int16_t value;
	PART	part;
};

struct DATA_REGS{
	DATA_REG X;
	DATA_REG Y;
	DATA_REG Z;
};

class HMC5883{
public:
	HMC5883(void);

	void begin(uint8_t , uint8_t , uint8_t , uint8_t, uint8_t );

	void getConfReg(void);
	void getModeReg(void);
	void getDataReg(DATA_REGS *);

	uint8_t getStatusReg(uint8_t);
	bool isDataReady(void);

	void getIdentReg(uint8_t *);
	uint8_t getReg(void);

	void setMode(uint8_t);
	uint8_t readReg(uint8_t);

private:
	void	writeReg(uint8_t, uint8_t);

	TwoWire Wire;

	struct{
		uint8_t A;
		uint8_t B;
	}ConfReg;
	uint8_t ModeReg;
	DATA_REGS DataReg;
	uint8_t	StatusReg;
	struct{
		uint8_t A;
		uint8_t B;
		uint8_t C;
	}IdentReg;

	uint8_t address_counter;
};



#endif /* HMC5883_H_ */

/*
 *    !3.3V
 *    ! !DRDY
 *    ! ! !SDA
 *    ! ! ! !SCL
 *    ! ! ! ! !GND
 *    ! ! ! ! ! !Vcc+5V
 *  +-------------+
 *  ! o o o o o o !
 *  !             !
 *  !             !
 *  ! <-O         !
 *  !   |         !
 *  !   v         !
 *  !             !
 *  ! ()       () !
 *  +-------------+
 *
 */
