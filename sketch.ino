
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int Pir = 8;
int Buzzer = 9;
int Led = 10;

Servo myservo;
bool doorOpen = false;
unsigned long lastMovementTime = 0;
const unsigned long movementTimeout = 5000;

// Inisialisasi LCD (alamat 0x27 untuk I2C, 16 kolom, 2 baris)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  myservo.attach(6);
  pinMode(Pir, INPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Led, OUTPUT);

  myservo.write(90); // posisi awal netral
  digitalWrite(Buzzer, LOW);
  digitalWrite(Led, LOW);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Door Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  int statusPir = digitalRead(Pir);
  unsigned long now = millis();

  if (statusPir == HIGH) {
    lcd.setCursor(0, 0);
    lcd.print("Gerakan Terdeteksi ");
    lcd.setCursor(0, 1);
    lcd.print("Pintu Terbuka     ");
    Serial.println("Gerakan terdeteksi");

    digitalWrite(Led, HIGH);       // LED nyala
    tone(Buzzer, 1000);            // Buzzer bunyi (1000Hz)
    delay(200);                    // Bunyi selama 200ms
    noTone(Buzzer);                // Matikan buzzer

    lastMovementTime = now;

    if (!doorOpen) {
      myservo.write(0); // buka pintu
      doorOpen = true;
    }
  } else {
    if (doorOpen && (now - lastMovementTime >= movementTimeout)) {
      myservo.write(90); // tutup pintu
      lcd.setCursor(0, 0);
      lcd.print("Tidak Ada Gerakan");
      lcd.setCursor(0, 1);
      lcd.print("Pintu Tertutup   ");
      Serial.println("Pintu tertutup");

      digitalWrite(Led, LOW); // LED mati
      doorOpen = false;
    }

    if (!doorOpen) {
      lcd.setCursor(0, 0);
      lcd.print("Menunggu Gerakan ");
      lcd.setCursor(0, 1);
      lcd.print("Pintu Tertutup   ");
    }
  }

  delay(100);
}
