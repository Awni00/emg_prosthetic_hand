#include <Servo.h>

const int FING_SERVO_PIN = 5;
const int THUMB_SERVO_PIN = 6;

const int thumb_close_pos = 25; //  open hand
const int thumb_open_pos = 135; // closed hand
const int fing_close_pos = 170; // fing_open
const int fing_open_pos = 45; // fing_close

Servo fing_servo;
Servo thumb_servo;

int delay_time = 3; //milliseconds
char inputControl = '\1';


void setup()
{
  // set up servos
  fing_servo.attach(FING_SERVO_PIN);
  thumb_servo.attach(THUMB_SERVO_PIN);

  // begin serial connection and send confirmation
  Serial.begin(9600);
  Serial.println("ready!");

  OpenHand(fing_servo, thumb_servo);

}

void loop()
{
  if(Serial.available())
  {
    // read command byte from neurol
    inputControl = Serial.read();

    if (inputControl == '0') CloseHand(fing_servo, thumb_servo);
    else if (inputControl == '1') OpenHand(fing_servo, thumb_servo);

    Serial.println(inputControl); // send back the received byte

    inputControl = '\0';
  }

}

void CloseHand(Servo fing_servo, Servo thumb_servo)
// closes hand
{
  MoveServos(fing_servo, thumb_servo, fing_close_pos, thumb_close_pos, delay_time);
}

void OpenHand(Servo fing_servo, Servo thumb_servo)
// opens
{
  MoveServos(fing_servo, thumb_servo, fing_open_pos, thumb_open_pos, delay_time);
}


void MoveServo(Servo servo, int desired_pos, int delay_time)
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


void MoveServos(Servo servo1, Servo servo2, int desired_pos1, int desired_pos2, int delay_time)
// moves two servos in sync. Assumes same position.
{
  int pos1 = servo1.read();
  int pos2 = servo2.read();
  int increment1 = (pos1 < desired_pos1) ? 1 : -1;
  int increment2 = (pos2 < desired_pos2) ? 1 : -1;

  while (pos1 != desired_pos1 || pos2 != desired_pos2)
  {
    if (pos1 != desired_pos1)
    {
      pos1 += increment1;
      servo1.write(pos1);
    }
    if (pos2 != desired_pos2)
    {
      pos2 += increment2;
      servo2.write(pos2);
    }

    delay(delay_time);
  }

    char *string = (char*)malloc(30 * sizeof(char));
    sprintf(string, "1: %d (%d), 2: %d (%d)", pos1, desired_pos1, pos2, desired_pos2);

    Serial.println(string);

}
