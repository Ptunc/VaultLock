import serial
import os
import time

arduino_port = 'COM7' # BURAYI KENDİ PORTUNLA DEĞİŞTİR
baud_rate = 9600

try:
    ser = serial.Serial(arduino_port, baud_rate)
    print(f"{arduino_port} üzerinden dinleniyor... Bekleniyor.")
    
    while True:
        if ser.in_waiting > 0:
            mesaj = ser.readline().decode('utf-8').strip()
            
            # Arduino'nun ne söylediğini her halükarda ekranda görelim
            print(f"Arduino Diyor ki: {mesaj}") 
            
            if mesaj == "KILITLE":
                print(">>> TEHLİKE ALGILANDI! EKRAN KİLİTLENİYOR <<<")
                os.system("rundll32.exe user32.dll,LockWorkStation")
                time.sleep(3)
                
except serial.SerialException:
    print(f"HATA: {arduino_port} portu açılamadı! Başka bir program portu kullanıyor olabilir.")
except KeyboardInterrupt:
    print("\nProgram kapatıldı.")
    if 'ser' in locals():
        ser.close()