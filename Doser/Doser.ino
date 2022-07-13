int Total_Print_Volume = 401; //mililiters
long Total_Print_Time = 15000; //seconds
int Number_Of_Doses = 10; //times pump is turned on
float Dose_Flow_Rate = 0.5; //ml/s
bool default_dir = true; //flip what direction "forward" is

float dose_volume = Total_Print_Volume / Number_Of_Doses;
float dose_time = dose_volume / Dose_Flow_Rate;
float off_time = Total_Print_Time - (Number_Of_Doses*dose_time);

long timer = 0;
long pulse = 0;
int doses_count = 0;

byte state = 0;// 0 = off, 1 = manual, 2 = dosing off time, 3 = dosing

byte start = 13; //pin for start button
byte estop = 12; //pin for estop button
byte forward = 11; //pin for forward button
byte backward = 10; //pin for backward button
byte manual = 9; //pin for manual light
byte dosing = 8; //pin for dosing light
byte off = 7; //pin for off light
byte mstep = 5; //pin for mstep motor
byte dir = 6; //pin for direction motor
byte enable = 4; //pin for enable motor

void setup()
{
  pinMode(start, INPUT);
  pinMode(estop, INPUT);
  pinMode(forward, INPUT);
  pinMode(backward, INPUT);
  pinMode(manual, OUTPUT);
  pinMode(dosing, OUTPUT);
  pinMode(off, OUTPUT);
  pinMode(mstep, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(enable, OUTPUT);
}

void loop() {
  if (state = 0) { //off
    digitalWrite(manual, LOW);
    digitalWrite(dosing, LOW);
    digitalWrite(off, HIGH);
    if (digitalRead(start)) {
      state = 2;
      timer = 0;
      doses_count = 0;
      digitalWrite(enable, HIGH);
    }
    if (digitalRead(forward)) {
      state = 1;
      digitalWrite(enable, HIGH);
    }
    if (digitalRead(backward)) {
      state = 1;
      digitalWrite(enable, HIGH);
    }
  }
  else if (state = 1) { //manual
    digitalWrite(manual, HIGH);
    digitalWrite(dosing, LOW);
    digitalWrite(off, LOW);
    if (digitalRead(estop)) {
      state = 0;
      digitalWrite(enable, LOW);
      return;
    }
    if (digitalRead(forward)) {
      digitalWrite(dir, default_dir);
      do {
        digitalWrite(mstep, HIGH);
        delay(10);
        digitalWrite(mstep, LOW);
        delay(10);
      } while (digitalRead(forward));
    }
    if (digitalRead(backward)) {
      digitalWrite(dir, default_dir);
      do {
        digitalWrite(mstep, HIGH);
        delay(10);
        digitalWrite(mstep, LOW);
        delay(10);
      } while (digitalRead(backward));
    }
  }
  else if (state = 2) { //dosing off time
    digitalWrite(manual, LOW);
    digitalWrite(dosing, HIGH);
    digitalWrite(off, HIGH);
    if (digitalRead(estop)) {
      state = 0;
      digitalWrite(enable, LOW);
      return;
    }
    if (timer <= off_time) {
      timer++;
      delay(1000);
    }
    else {
      state = 3;
      timer = 0;
    }

  }
  if (state = 3) { //dosing
    digitalWrite(manual, LOW);
    digitalWrite(dosing, HIGH);
    digitalWrite(off, LOW);

    if (timer <= dose_time) {
      timer++;
      digitalWrite(dir, default_dir);
      do { //based on 10milisec pules on mstepper
        digitalWrite(mstep, HIGH);
        delay(10);
        digitalWrite(mstep, LOW);
        delay(10);
        pulse++;

        if (digitalRead(estop)) {
          state = 0;
          digitalWrite(enable, LOW);
          return;
        }
      } while (pulse < 100);
    }
    else {
      state = 2;
      timer = 0;
      doses_count++;
      if (doses_count == Number_Of_Doses) {
        state = 0;
      }
    }
  }
  delay(100);
}
