#include <Wire.h>
#define KIRI 11
#define KANAN 12

int rpmL = 0, rpmR = 0; //penampung data yang dikirimkan oleh slave kiri dan kanan (RPM)
int pwm_left = 0;
int pwm_right = 0;
String data = "";

void setup() {
 Wire.begin();
 Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0){
    data = Serial.readString();
    Serial.println("menunggu data");
    Serial.println(data);
    String values[2];
    int index = 0;
    int startIndex = 0;
    while (index < 2) {
      int commaIndex = data.indexOf(',', startIndex);
      if (commaIndex != -1) {
        values[index] = data.substring(startIndex, commaIndex);
        startIndex = commaIndex + 1;
        index++;
      } else {
        values[index] = data.substring(startIndex);
        break;
      }
    }

    // Convert the values to integers
    int lpwm = values[0].toInt();
    int rpwm = values[1].toInt();
    // Send the PWM values to the slave devices
    transmit_data(lpwm, rpwm);}
    read_rpm();
    Serial.print(rpmL);
    Serial.print("\t"); 
    Serial.print(rpmR);
    Serial.println("  ");
    

}

void transmit_data(int lpwm, int rpwm){ // fungsi yang digunakan mengirim data ke slave
  //kirim data ke slave kiri
  Wire.beginTransmission(KIRI);
  Wire.write((byte)(lpwm >> 8));
  Wire.write((byte)(lpwm));
  Wire.endTransmission();

  //kirim data ke slave kanan
  Wire.beginTransmission(KANAN);
  Wire.write((byte)(rpwm >> 8));
  Wire.write((byte)(rpwm));
  Wire.endTransmission();
   
}

void read_rpm(){ //fungsi yang digunkaan untuk meminta data ke slave
  rpmL = 0; rpmR = 0;
  //meminta data RPM dari slave kiri
  Wire.requestFrom(KIRI, 2);
  rpmL |= (Wire.read() << 8);
  rpmL |= Wire.read();
  //meminta dara RPM dari slave kanan
  Wire.requestFrom(KANAN, 2);
  rpmR |= (Wire.read() << 8);
  rpmR |= Wire.read();  
}
