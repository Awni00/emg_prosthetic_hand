#include <Servo.h>

const int FING_SERVO_PIN = 5;
const int THUMB_SERVO_PIN = 6;

Servo fing_servo;
Servo thumb_servo;

int delay_time = 3; //milliseconds
char inputControl = '\0';


void setup()
{
  // set up servos
  fing_servo.attach(FING_SERVO_PIN);
  thumb_servo.attach(THUMB_SERVO_PIN);

  // begin serial connection and send confirmation
  Serial.begin(9600);
  Serial.println("ready!");

}

void loop()
{
  if(Serial.available())
  {
    // read command byte from neurol
    inputControl = Serial.read();

    if (inputControl == '1') moveServos(fing_servo, thumb_servo, 180, delay_time);
    else if (inputControl == '0') moveServos(fing_servo, thumb_servo, 0, delay_time);

    Serial.println(inputControl); // send back the received byte

    inputControl = '\0';
  }

}


void moveServo(Servo servo, int desired_pos, int delay_time)
// move servo to desired position
{
  int init_pos = servo.read();
  if (desired_pos > init_pos)
  {
    for (int pos = init_pos; pos < desired_pos; pos+=1)
    {
      servo.write(pos);
      delay(delay_time);
    }
  } else
  {
    for (int pos = init_pos; pos > desired_pos; pos -= 1)
    {
      servo.write(pos);
      delay(delay_time);
    }
  }
}


void moveServos(Servo servo1, Servo servo2, int desired_pos, int delay_time)
// moves two servos in sync. Assumes same position.
{
  int init_pos = servo1.read();
  if (desired_pos > init_pos)
  {
    for (int pos = init_pos; pos < desired_pos; pos+=1)
    {
      servo1.write(pos);
      servo2.write(pos);
      delay(delay_time);
    }
  } else
  {
    for (int pos = init_pos; pos > desired_pos; pos -= 1)
    {
      servo1.write(pos);
      servo2.write(pos);
      delay(delay_time);
    }
  }
}
