#include <Wire.h>

const int MPU_ADDR = 0x68; // I2C Tarayıcıda bulduğumuz adres

// LED Pinleri
const int ledSistem = 8;  // Yeşil LED
const int ledTehlike = 9; // Kırmızı LED

// Titreşim eşik değeri (Normal duruş yaklaşık 9.8'dir)
float esikDegeri = 13.0; 

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  // LED'leri ayarla
  pinMode(ledSistem, OUTPUT);
  pinMode(ledTehlike, OUTPUT);
  digitalWrite(ledSistem, LOW);
  digitalWrite(ledTehlike, LOW);

  // MPU6050'yi Uyandırma İşlemi (Güç yönetimi register'ına 0 yazarak)
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // 0 yazarak sensörü uyandır
  Wire.endTransmission(true);

  // Sistem hazır, Yeşil LED'i yak
  digitalWrite(ledSistem, HIGH); 
  Serial.println("SISTEM_HAZIR");
}

void loop() {
  // İvme verilerinin tutulduğu adrese git (0x3B)
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  
  // 6 byte veri iste (X, Y ve Z eksenleri için 2'şer byte)
  Wire.requestFrom(MPU_ADDR, 6, true); 

  // Ham verileri oku ve birleştir
  int16_t hamX = Wire.read() << 8 | Wire.read();
  int16_t hamY = Wire.read() << 8 | Wire.read();
  int16_t hamZ = Wire.read() << 8 | Wire.read();

  // Ham verileri m/s² cinsinden standart ivmeye çevir (±2g aralığı için)
  float x = (hamX / 16384.0) * 9.81;
  float y = (hamY / 16384.0) * 9.81;
  float z = (hamZ / 16384.0) * 9.81;

  // İvme vektörünün büyüklüğünü hesapla
  float toplamIvme = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

  // Eğer hesaplanan ivme eşik değerini geçerse
  if (toplamIvme > esikDegeri) {
    // Kırmızı LED'i yak
    digitalWrite(ledTehlike, HIGH);
    
    // Python'a mesaj gönder
    Serial.println("KILITLE");
    
    // 3 saniye kilitli kal ve sinyal gönderme
    delay(3000); 
    
    // Kırmızı LED'i geri söndür
    digitalWrite(ledTehlike, LOW);
  }
  
  delay(50);
}