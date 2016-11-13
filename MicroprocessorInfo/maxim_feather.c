#include "mbed.h"
#include "USBSerial.h"

#define MAX14690_I2C_ADDR    0x50
#define BMI_160_I2C_ADDR    0xD0

// Hardware serial port over DAPLink
Serial pc(USBTX, USBRX);

DigitalOut led1(P2_5);
I2C i2cm2(P5_7, P6_0);

// main() runs in its own thread in the OS
// (note the calls to Thread::wait below for delays)
int main()
{
    
    int16_t acc[3];
    int16_t gyr[3];
    int16_t mag[3];
    int time = 0;
    char data[5];
    int c;
    data[0] = 0x14; // I2C address for first register (LDO2 CFG)
    data[1] = 0x00; // Dissable LDO2
    data[2] = 0x19; // Set voltage to 3.3V
    data[3] = 0x00; // Dissable LDO3
    data[4] = 0x19; // Set voltage to 3.3V
    i2cm2.write(MAX14690_I2C_ADDR, data, 5);
    data[1] = 0x02; // Enable LDO2
    data[3] = 0x02; // Enable LDO3
    i2cm2.write(MAX14690_I2C_ADDR, data, 5);
    char ret[23];
    char cmd[2];
    cmd[0] = 0x7E;
    cmd[1] = 0b00010001;
    
    i2cm2.write(BMI_160_I2C_ADDR, cmd,2);
    wait(0.2);
    cmd[0] = 0x7E;
    cmd[1] = 0b00010101;
    
    i2cm2.write(BMI_160_I2C_ADDR, cmd,2);
    wait(0.2);
    cmd[0] = 0x7E;
    cmd[1] = 0b00011001;
    
    i2cm2.write(BMI_160_I2C_ADDR, cmd,2);
    wait(0.2);
    cmd[0] = 0x40;
    cmd[1] = 0b00101100;
    
    
    
    i2cm2.write(BMI_160_I2C_ADDR, cmd,2);
    wait(0.2);
    
    
    cmd[0] = 0x42;
    cmd[1] = 0b00101101;
    
    i2cm2.write(BMI_160_I2C_ADDR, cmd,2);
    wait(0.2);

    // Virtual serial port over USB
    USBSerial serial;
    while(1) {
        
        cmd[0] = 0x04;
        i2cm2.write(BMI_160_I2C_ADDR, cmd,1, true);
        i2cm2.read(BMI_160_I2C_ADDR, ret, 23);
        //c = serial.getc();
        mag[0] = ret[1]<<8 | ret[0];
        mag[1] = ret[3]<<8 | ret[2];
        mag[2] = ret[5]<<8 | ret[4];
        gyr[0] = ret[9]<<8 | ret[8];
        gyr[1] = ret[11]<<8 | ret[10];
        gyr[2] = ret[13]<<8 | ret[12];
        acc[0] = ret[15]<<8 | ret[14];
        acc[1] = ret[17]<<8 | ret[16];
        acc[2] = ret[19]<<8 | ret[18];
        time = ret[20] | ret[21] << 8 | ret[22] << 16;
        serial.printf("%d %d %d %f %f %f %f %f %f %d\r\n",mag[0],mag[1],mag[2],-gyr[0]/16.4,gyr[1]/16.4,-gyr[2]/16.4,acc[0]/16384.0,acc[1]/16384.0,acc[2]/16384.0, time);
        
    }
}
