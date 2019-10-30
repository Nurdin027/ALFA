#include <LiquidCrystal_I2C.h>
#include <JatayuIO.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

String apikey = "646ce982-de9b-11e9-8801-e4066d151e57";
char* ssid = "Alfa_WIFI"; //RPL hotspot
char* password = "atikan12345";
String TelID = "947494561";
int cahaya;
bool buka;

JatayuIO jatayu(apikey, ssid, password);

void setup(){ 
  //LCD
  lcd.begin();
  lcd.backlight();
  
  Serial.begin(9600);
  jatayu.begin();
  pinMode(D2,OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(D4, INPUT_PULLUP);
  digitalWrite(D2, HIGH);
  digitalWrite(D8, HIGH);
  buka = false;
}



void loop(){
  
  cahaya = analogRead(A0);
  Serial.println(cahaya);
  if(cahaya >=700){
     digitalWrite(D2, LOW);
  }else{
  digitalWrite(D2, HIGH);
  }  


 buka = digitalRead(D4);
 if (!buka){
  digitalWrite(D8,LOW);
  Serial.println("Tombol di tekan");
  delay(10000);
  buka = false;
  digitalWrite(D8,HIGH);
 }
 
 String dataV1 = jatayu.GetData("V1");
 String dari = jatayu.GetData("V2");
 String saha = jatayu.GetData("V3");
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Smart Office");
 lcd.setCursor(0,1);
 lcd.print("SMKN Lemahsugih");
 
 if(dataV1 == "0"){
  digitalWrite(D8, HIGH);
  noTone(D5);
  Serial.println("Pintu Terkunci");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SMKN LEMAHSUGIH");
  lcd.setCursor(0,1);
  lcd.print("Pintu terkunci");
  
 }
 else if(dataV1 == "1"){
  Serial.println("nyala");
  digitalWrite(D8,LOW);
  tone(D5,523,3000);
  Serial.println("Pintu Terbuka");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Akses diterima");
  lcd.setCursor(0,1);
  lcd.print("Kunci Terbuka");
  
  if(dari == "CAM"){
    String lewat = "Camera";
    jatayu.SendTelegram(TelID,"Pintu dibuka oleh " + saha + " lewat "+ lewat);
    jatayu.SetData("V2", "OFF");
    Serial.println("Pintu dibuka oleh " + saha + " lewat "+ lewat);
  lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Selamat Datang");
    lcd.setCursor(0,1);
    lcd.print(saha);
  }else if(dari == "APK"){
    String lewat = "Android";
    jatayu.SendTelegram(TelID,"Pintu dibuka oleh " + saha + " lewat "+ lewat);
    jatayu.SetData("V2", "OFF");
    Serial.println("Pintu dibuka oleh " + saha + " lewat "+ lewat);
  lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Selamat Datang");
    lcd.setCursor(0,1);
    lcd.print(saha);
  }else{
    String lewat = "perangkat tidak dikenal";
    jatayu.SendTelegram(TelID,"Peringatan! Pintu dibuka lewat "+ lewat);
    Serial.println("Peringatan! Pintu dibuka lewat "+ lewat);
  }
  
  delay(5000);
  jatayu.SetData("V1", "0");
 }
 delay(1000);
}
