//USER-----
int Total_Print_Volume = 1; //mililiters
long Total_Print_Time = 10; //seconds
int Number_Of_Doses = 2; //times pump is turned on
float Dose_Flow_Rate = .45; //ml/s
bool default_dir = true; //flip what direction "forward" is
//----------


float dose_volume = (float)Total_Print_Volume / Number_Of_Doses;
long dose_time = floor(dose_volume / Dose_Flow_Rate);
long off_time = floor(((float)Total_Print_Time - ((float)Number_Of_Doses*dose_time)) / Number_Of_Doses);

long timer = 0;
long pulse = 0;
int doses_count = 0;

byte state = 0;// 0 = off, 1 = manual, 2 = dosing off time, 3 = dosing

byte start = 3; //pin for start button
byte estop = 2; //pin for estop button
byte forward = 5; //pin for forward button
byte backward = 4; //pin for backward button
byte manual = 8; //pin for manual light
byte dosing = 6; //pin for dosing light
byte off = 7; //pin for off light
byte mstep = 9; //pin for mstep motor
byte dir = 10; //pin for direction motor
byte enable = 11; //pin for enable motor

void setup()
{
  pinMode(start, INPUT_PULLUP);
  pinMode(estop, INPUT_PULLUP);
  pinMode(forward, INPUT_PULLUP);
  pinMode(backward, INPUT_PULLUP);
  pinMode(manual, OUTPUT);
  pinMode(dosing, OUTPUT);
  pinMode(off, OUTPUT);
  pinMode(mstep, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(enable, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println(off_time);
  if (state == 0) { //off
    digitalWrite(manual, LOW);
    digitalWrite(dosing, LOW);
    digitalWrite(off, HIGH);
    digitalWrite(enable, HIGH);
    if (!digitalRead(start)) {
      state = 2;
      timer = 0;
      doses_count = 0;
    }
    if (!digitalRead(forward)) {
      state = 1;
    }
    if (!digitalRead(backward)) {
      state = 1;
    }
  }
  else if (state == 1) { //manual
    digitalWrite(manual, HIGH);
    digitalWrite(dosing, LOW);
    digitalWrite(off, LOW);
    digitalWrite(enable, LOW);
    if (!digitalRead(estop)) {
      state = 0;
      return;
    }
    if (!digitalRead(forward)) {
      digitalWrite(dir, default_dir);
      do {
        digitalWrite(mstep, HIGH);
        delay(1);
        digitalWrite(mstep, LOW);
        delay(1);
      } while (!digitalRead(forward));
    }
    if (!digitalRead(backward)) {
      digitalWrite(dir, !default_dir);
      do {
        digitalWrite(mstep, HIGH);
        delay(1);
        digitalWrite(mstep, LOW);
        delay(1);
      } while (!digitalRead(backward));
    }
  }

  else if (state == 2) { //dosing off time
    digitalWrite(manual, LOW);
    digitalWrite(dosing, HIGH);
    digitalWrite(off, HIGH);
    digitalWrite(enable, HIGH);
    if (!digitalRead(estop)) {
      state = 0;
      return;
    }
    if (timer < off_time) {
      timer++;
      delay(1000);
    }
    else {
      state = 3;
      timer = 0;
    }

  }
  if (state == 3) { //dosing
    digitalWrite(manual, LOW);
    digitalWrite(dosing, HIGH);
    digitalWrite(off, LOW);
    digitalWrite(enable, LOW);
    digitalWrite(dir, default_dir);
    if (timer < dose_time) {
      timer++;
      do { //based on 10milisec pules on mstepper
        digitalWrite(mstep, HIGH);
        delay(1);
        digitalWrite(mstep, LOW);
        delay(1);
        pulse++;

        if (!digitalRead(estop)) {
          state = 0;
          return;
        }
      } while (pulse < 1000);
    }
    else {
      state = 2;
      timer = 0;
      doses_count++;
      if (doses_count == Number_Of_Doses) {
        state = 0;
      }
      pulse = 0;

    }
  }
  delay(100);
}
