#include <Arduino.h>
#include <Wire.h>

#define I2C_ADDR        0x34

#define ADC_BAT_ADDR                  0x00
#define MOTOR_TYPE_ADDR               0x14 // (set encoder motor type)
#define MOTOR_ENCODER_POLARITY_ADDR   0x15 // (set the encoder direction polarity)
// (If the motor speed is completely uncontrollable, either it rotates at the fastest speed or stops. You can reset the value of this address)
// (range 0 or 1, default 0)
#define MOTOR_FIXED_PWM_ADDR      0x1F //（-100~100）(Fixed PWM control belongs to open loop control. It ranges from -100 to 100)   
#define MOTOR_FIXED_SPEED_ADDR    0x33 // (Fixed speed control belongs to closed loop control)
// (Unit: pulse count per 10 milliseconds, range (depending on the specific encoder motor, affected by the number of encoding lines, voltage size, load size, etc., generally around ±50))

#define MOTOR_ENCODER_TOTAL_ADDR  0x3C // (total pulse value of 4 encoder motors)
// (If the pulse count per revolution of the motor is known as U, and the diameter of the wheel is known as D, then the distance traveled by each wheel can be obtained through pulse counting)
// (For example, if the total pulse count of motor 1 is P, then the distance traveled is (P/U) * (3.14159*D))
// (For different motors, you can test the pulse count per revolution U by manually rotating 10 times and reading the pulse count, and then take the average value to get)


// (motor type specific value)
#define MOTOR_TYPE_WITHOUT_ENCODER        0 // (Motor without encoder, 44 pulses per magnetic ring rotation, reduction ratio: 90, default)
#define MOTOR_TYPE_TT                     1 // (TT encoder motor)
#define MOTOR_TYPE_N20                    2 // (N20 encoder motor)
#define MOTOR_TYPE_JGB37_520_12V_110RPM   3 // (44 pulses per magnetic ring rotation, reduction ratio: 90, default)

u8 data[20];// (used to temporarily store data read by I2C)

uint8_t count = 0;

bool WireWriteByte(uint8_t val)
{
    Wire.beginTransmission(I2C_ADDR);
    Wire.write(val);
    if( Wire.endTransmission() != 0 ) {
        return false;
    }
    return true;
}
// (write data to the address (reg: address  val: data content  len: data length))
bool WireWriteDataArray(  uint8_t reg,uint8_t *val,unsigned int len)
{
    unsigned int i;

    Wire.beginTransmission(I2C_ADDR);
    Wire.write(reg);
    for(i = 0; i < len; i++) {
        Wire.write(val[i]);
    }
    if( Wire.endTransmission() != 0 ) {
        return false;
    }

    return true;
}
// (read data from the address (reg: address  val: data content))
bool WireReadDataByte(uint8_t reg, uint8_t &val)
{
    if (!WireWriteByte(reg)) {
        return false;
    }
    
    Wire.requestFrom(I2C_ADDR, 1);
    while (Wire.available()) {
        val = Wire.read();
    }
    
    return true;
}
// (read the data of the specified length from the address (reg: address val: data content len: data length))
int WireReadDataArray(   uint8_t reg, uint8_t *val, unsigned int len)
{
    unsigned char i = 0;
    
    /* Indicate which register we want to read from */
    if (!WireWriteByte(reg)) {
        return -1;
    }
    
    /* Read block data */
    Wire.requestFrom(I2C_ADDR, len);
    while (Wire.available()) {
        if (i >= len) {
            return -1;
        }
        val[i] = Wire.read();
        i++;
    }
    
    return i;
}


int serial_putc( char c, struct __file * )
{
  Serial.write( c );
  return c;
}
void printf_begin(void)
{
  fdevopen( &serial_putc, 0 );
}

uint8_t MotorType = MOTOR_TYPE_JGB37_520_12V_110RPM; // (set motor type)
uint8_t MotorEncoderPolarity = 0; 


void Initialize()
{
//  Wire.begin();
  delay(200);
  WireWriteDataArray(MOTOR_TYPE_ADDR,&MotorType,1);
  delay(5);
  WireWriteDataArray(MOTOR_ENCODER_POLARITY_ADDR,&MotorEncoderPolarity,1);
}

void SetMotorSpeed(int left, int right)
{
  int8_t p1[4]={left,right,0,0};

  WireWriteDataArray(MOTOR_FIXED_SPEED_ADDR,p1,4); 
}

void GetEncoderCounts(long& left, long& right)
{
  int32_t encoders[4];
  WireReadDataArray(MOTOR_ENCODER_TOTAL_ADDR,(uint8_t*)encoders,16); // (read accumulated motor rotation value)

  left = encoders[0];
  right = encoders[1];
}

void ResetEncoders()
{
  int8_t encoderReset[16]={0};
  WireWriteDataArray(MOTOR_ENCODER_TOTAL_ADDR,encoderReset,16);
}
