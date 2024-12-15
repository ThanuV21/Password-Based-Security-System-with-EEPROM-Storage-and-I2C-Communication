typedef unsigned char u8;

void i2c_eeprom_write(unsigned char slaveaddr,unsigned char Wbuffaddr,unsigned char dat)
{
	i2c_start();//start i2c
	i2c_write(slaveaddr<<1);//7bit slave addr
	i2c_write(Wbuffaddr);//send eeprom memory address
	i2c_write(dat);//send the data to be written
	i2c_stop();//stop i2c
	delay_ms(10);//internal operation
}

unsigned char i2c_eeprom_read(unsigned char slaveaddr,unsigned char rbuffaddr)
{
	unsigned char dat;
	i2c_start();//start i2c
	i2c_write(slaveaddr<<1);//7bit slave addr
	i2c_write(rbuffaddr);//send eeprom memory address
	i2c_restart();//restart i2c for read operation
	i2c_write(slaveaddr<<1|1);//send the slave address with read bit LSB=1
	dat=i2c_nack();//read the data byte from eeprom and send nack
	i2c_stop();//stop i2c
	return dat;//internal operation
}

void i2c_eeprom_page_write(u8 slaveaddr,u8 wbuffstartaddr,u8 *p,u8 nbytes)
{
	u8 i;
	i2c_start();
	i2c_write(slaveaddr<<1);
	i2c_write(wbuffstartaddr);
	for(i=0;i<nbytes;i++)
	{
		i2c_write(p[i]);
	}
	i2c_stop();
	delay_ms(50);
}

void i2c_eeprom_seq_read(u8 slaveaddr,u8 rbuffstartaddr,u8 *p,u8 nbytes)
{
	u8 i;
	i2c_start();
	i2c_write(slaveaddr<<1);
	i2c_write(rbuffstartaddr);
	i2c_restart();
	i2c_write(slaveaddr<<1|1);
	for(i=0;i<nbytes-1;i++)
	{
		p[i]=i2c_masterack();
	}
	p[i]=i2c_nack();
	i2c_stop();
}
