#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  flyEncoder,     sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  wheelEncoder,   sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port3,           fly1,          tmotorVex393_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port4,           fly2,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           wheel1,        tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port9,           wheel2,        tmotorVex393HighSpeed_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

const float margin = .05;

int defaultPower = 100;
float targetSpeed = 2.0;

typedef union
{
	struct
	{
		float speed;
		tSensors encoder;
		TTimers timer;
	};

	tMotor motors[2];
} flywheel;

void findWheelSpeed(flywheel &flywheel)
{
	flywheel.speed = SensorValue[flywheel.encoder] / time1(flywheel.timer);
	SensorValue[flywheel.encoder] = 0;
	clearTimer(flywheel.timer);
}

void setWheelPower(flywheel &flywheel)
{
	findWheelSpeed(flywheel);

	if (flywheel.speed < targetSpeed * (1 - margin))
	{
		motor[flywheel.motors[0]] = 127;
		motor[flywheel.motors[1]] = 127;
	}
	else if (flywheel.speed > targetSpeed * (1 + margin))
	{
		motor[flywheel.motors[0]] = 0;
		motor[flywheel.motors[1]] = 0;
	}
	else
	{
		motor[flywheel.motors[0]] = defaultPower;
		motor[flywheel.motors[1]] = defaultPower;
	}
}

task main()
{
	//first flywheel
	flywheel fly;
	fly.speed = 0.0;
	fly.encoder = flyEncoder;
	fly.timer = T1;
	tMotor flyMotors[2] = {fly1, fly2};
	fly.motors = flyMotors;

	//second flywheel
	flywheel wheel;
	wheel.speed = 0.0;
	wheel.encoder = wheelEncoder;
	wheel.timer = T2;
	tMotor wheelMotors[2] = {wheel1, wheel2};
	wheel.motors = wheelMotors;

while(true)
{
	setWheelPower(fly);
	setWheelPower(wheel);
}
}
