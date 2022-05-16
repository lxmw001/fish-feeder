// CONNECTIONS:
// DS1302 CLK/SCLK --> 5
// DS1302 DAT/IO --> 4
// DS1302 RST/CE --> 2
// DS1302 VCC --> 3.3v - 5v
// DS1302 GND --> GND

// RTC libraies
#include <ThreeWire.h>
#include <RtcDS1302.h>

#include <Servo.h>
Servo servofeeder;
ThreeWire myWire(4, 5, 2); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

#define countof(a) (sizeof(a) / sizeof(a[0]))

int minPosition = 30; //30
int maxPosition = 130; //130
// 28 only to tests
// 28 to test feed for one week
int times = 2; // replace to 1 by the moment
int servoSpeed = 50;

int hours[4] = {6, 10, 14, 18};

void setup() {
  // put your setup code here, to run once:
  setupServo();
  setupRtc();
}

void setupServo() {
  servofeeder.attach(3);
  servofeeder.write(minPosition);
  Serial.begin(9600);
}

void setupRtc() {
  Serial.begin(57600);
  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);

  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);
  Serial.println();

  if (!Rtc.IsDateTimeValid())
  {
    // Common Causes:
    //    1) first time you ran and the device wasn't running yet
    //    2) the battery on the device is low or even missing

    Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }

  if (Rtc.GetIsWriteProtected())
  {
    Serial.println("RTC was write protected, enabling writing now");
    Rtc.SetIsWriteProtected(false);
  }

  if (!Rtc.GetIsRunning())
  {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  Serial.println("RTC now");
  printDateTime(now);
  if (now < compiled)
  {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  }
  else if (now > compiled)
  {
    Serial.println("RTC is newer than compile time. (this is expected)");
  }
  else if (now == compiled)
  {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }
}

void loop() {
  checkTimeAndFeed();
  delay(10000); // 10 seconds
}

void checkTimeAndFeed() {
  RtcDateTime now = Rtc.GetDateTime();
  int hour = now.Hour();
  int minute = now.Minute();
  int second = now.Second();



  for (byte i = 0; i < 4; i = i + 1) {
    int feedHour = hours[i];
    if (hour == feedHour && minute == 0 && (second > 20 && second < 40)) {
      //    if (hour == 23 && (second > 20 && second < 40)) {
      Serial.print("hour: ");
      Serial.println(hour);
      Serial.print("minute: ");
      Serial.println(minute);
      Serial.print("second: ");
      Serial.println(second);
      Serial.println("feeeeeeddd");
      feed();
    }
  }
}

void feed() {
  for (int i = 0; i < times; i++) {
    goUp();
    goDown();
    delay(1000);
  }
}

void goUp() {
  for (int i = minPosition; i <= maxPosition; i += 10) {
    servofeeder.write(i);
    delay(servoSpeed);
  }
}

void goDown() {
  for (int i = maxPosition; i >= minPosition; i -= 10) {
    servofeeder.write(i);
    delay(servoSpeed);
  }
}

int hoursToMiliseconds(int hours) {
  return hours * 1000 * 60 * 60;
}

void printDateTime(const RtcDateTime& dt) {
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Month(),
             dt.Day(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );
  Serial.print(datestring);
}
