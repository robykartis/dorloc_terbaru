#include <SoftwareSerial.h>
SoftwareSerial BTserial(3, 2); // rx-tx

int relay = 7;
char c = 0; // kontainer
bool isDelayActive = false; // penanda apakah relay sedang dalam proses penundaan
unsigned long delayStartMillis = 0; // waktu mulai penundaan
unsigned long delayDuration = 5000; // durasi penundaan dalam milidetik (5 detik)
unsigned long ignoreStartMillis = 0; // waktu mulai mengabaikan sinyal
unsigned long ignoreDuration = 2000; // durasi pengabaian sinyal dalam milidetik (2 detik)

void setup() {
  BTserial.begin(9600);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
}

void loop() {
  if (BTserial.available()) { // kode Bluetooth
    c = BTserial.read();
    Serial.write(c);

    if (c == '1' && !isDelayActive && !isIgnoringSignal()) { // jika nilai kontainer adalah 1 dan tidak ada penundaan aktif atau sedang mengabaikan sinyal
      isDelayActive = true; // tandai penundaan aktif
      delayStartMillis = millis(); // catat waktu mulai penundaan
      digitalWrite(relay, LOW);
    }
  }

  if (isDelayActive) { // cek apakah ada penundaan aktif
    unsigned long currentMillis = millis(); // mendapatkan waktu sekarang
    if (currentMillis - delayStartMillis >= delayDuration) { // cek jika waktu penundaan telah berlalu
      isDelayActive = false; // tandai penundaan selesai
      ignoreStartMillis = currentMillis; // catat waktu mulai mengabaikan sinyal
      digitalWrite(relay, HIGH);
    }
  }

  if (isIgnoringSignal()) { // cek apakah sedang mengabaikan sinyal
    unsigned long currentMillis = millis(); // mendapatkan waktu sekarang
    if (currentMillis - ignoreStartMillis >= ignoreDuration) { // cek jika waktu pengabaian sinyal telah berlalu
      clearIgnoreSignal(); // selesai mengabaikan sinyal
    }
  }
}

bool isIgnoringSignal() {
  return (ignoreStartMillis > 0); // cek apakah sedang dalam proses mengabaikan sinyal
}

void clearIgnoreSignal() {
  ignoreStartMillis = 0; // menghentikan proses mengabaikan sinyal
}
