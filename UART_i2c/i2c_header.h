//pins
#define SCL_EN 0x00000010
#define SDA_EN 0x00000040
//speed
#define CCLK 60000000
#define PCLK CCLK/4
#define I2C_speed 100000
#define LOADVAL ((PCLK/I2C_speed)/2)
//bit defines for I2CONSET sfr
#define AA_BIT 2
#define SI_BIT 3
#define STO_BIT 4
#define STA_BIT 5
#define I2EN_BIT 6

void i2c_init(void)
{
	PINSEL0|=SCL_EN|SDA_EN;
	I2SCLL=LOADVAL;
	I2SCLH=LOADVAL;
	I2CONSET=1<<I2EN_BIT; //enable i2c
}

void i2c_start(void)
{
	I2CONSET=1<<STA_BIT; //start
	while(((I2CONSET>>SI_BIT)&1)==0); //wait for si bit
	I2CONCLR=1<<STA_BIT; //clear start bit
}

void i2c_restart(void)
{
	I2CONSET=1<<STA_BIT;//start
	I2CONCLR=1<<SI_BIT;//clear si bit
	while(((I2CONSET>>SI_BIT)&1)==0);//wait
	I2CONCLR=1<<STA_BIT;//stop
}

void i2c_write(unsigned char dat)
{
	I2DAT=dat;
	I2CONCLR=1<<SI_BIT;
	while(((I2CONSET>>SI_BIT)&1)==0);
}

void i2c_stop(void)
{
	I2CONSET=1<<STO_BIT;
	I2CONCLR=1<<SI_BIT;
}

unsigned char i2c_nack(void)
{
	I2CONSET=0x00;
	I2CONCLR=1<<SI_BIT;
	while(((I2CONSET>>SI_BIT)&1)==0);
	return I2DAT;
}

unsigned char i2c_masterack(void)
{
	I2CONSET=0x04;
	I2CONCLR=1<<SI_BIT;
	while(((I2CONSET>>SI_BIT)&1)==0);
	I2CONCLR=0x04;
	return I2DAT;
}
