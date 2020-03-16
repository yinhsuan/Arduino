#include <PS2X_lib.h>
//按著的時候可以驅動另一顆=>change

PS2X ps2x; // 建立PS2控制器的類別實體
//pressure
int fsrData;
const int fsrPin = A0;
int error = 0; 
byte type = 0;
byte vibrate = 0;
unsigned long times = 0;
unsigned long tmp_times = 0;
bool lockMode = false; //true => lock; false => unlock
bool changeMotor = true;

void setup(){
  
 //motorOne pin
 pinMode(7,OUTPUT); //DIR-
 pinMode(6,OUTPUT); //PUL-
 //motorTwo pin
 pinMode(9,OUTPUT); //DIR-
 pinMode(8,OUTPUT); //PUL-
 //motorThree pin
 pinMode(11,OUTPUT); //DIR-
 pinMode(10,OUTPUT); //PUL-
 //motorFour pin
 pinMode(13,OUTPUT); //DIR-
 pinMode(12,OUTPUT); //PUL-
 
 Serial.begin(57600);
// 控制器接腳設置並驗證是否有錯誤:  GamePad(時脈腳位, 命令腳位, 選取腳位, 資料腳位, 是否支援類比按鍵, 是否支援震動) 
 error = ps2x.config_gamepad(2,3,4,5, true, 0);
 
 if(error == 0) { // 如果控制器連接沒有問題，就顯示底下的訊息。
   Serial.println("Found Controller, configured successful");
   Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
   Serial.println("holding L1 or R1 will print out the analog stick values.");
   Serial.println("Go to www.billporter.info for updates and to report bugs.");
 }
   
  else if(error == 1) // 找不到控制器，顯示底下的錯誤訊息。
   Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)  // 發現控制器，但不接受命令，請參閱程式作者網站的除錯說明。
   Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
   
  else if(error == 3) // 控制器拒絕進入類比感測壓力模式，或許是此控制器不支援的緣故。
   Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
   
   type = ps2x.readType();  // 偵測控制器器的類型
     switch(type) {
       case 0:
        Serial.println("Unknown Controller type");   // 未知的控制器類型
       break;
       case 1:
        Serial.println("DualShock Controller Found");  // 發現DualShock控制器
       break;
       case 2:
         Serial.println("GuitarHero Controller Found");  // 發現吉他英雄控制器
       break;
     }
}

void loop(){
  //pressure
  fsrData = analogRead(fsrPin); // 將讀取到的數值轉為 0~1023 之間的數字
  if (fsrData > 800) //預設pressure超過800 => stop
    lockMode = true;
  else if (fsrData <= 800) //預設pressure超過800 => stop
    lockMode = true;

   if (lockMode == false) {    
   
    ps2x.read_gamepad(false, vibrate);      // 讀取控制器並且命令大的震動馬達以"vibrate"變數值的速度旋轉
     
     if (ps2x.NewButtonState())          // 若「按下」或「放開」任何按鈕
    {
      if(ps2x.Button(PSB_L3))
        Serial.println("L3 pressed");
        if(ps2x.Button(PSB_R3))
         Serial.println("R3 pressed");
        if(ps2x.Button(PSB_L2))
         Serial.println("L2 pressed");
        if(ps2x.Button(PSB_R2))
         Serial.println("R2 pressed");
        if(ps2x.Button(PSB_GREEN)) {   // 若被按下的是三角按鍵
         Serial.println("Triangle pressed"); 
         digitalWrite(6,LOW); 
         delayMicroseconds(100);
        }
    } 


    if(ps2x.ButtonPressed(PSB_RED)) {            // 若「按下」圈圈按鍵
         Serial.println("Circle just pressed");
         digitalWrite(6,HIGH); 
         delayMicroseconds(100);
    }
         
    if(ps2x.ButtonReleased(PSB_PINK))           // 若「放開」方塊按鍵
         Serial.println("Square just released");     
    
    if(ps2x.NewButtonState(PSB_BLUE))            // 若「按下」或「放開」叉叉按鍵
         Serial.println("X just changed");    

    //按下 向下鍵 後操作鎖死
    //if(ps2x.Button(PSB_PAD_DOWN))
    //  unlockMode == ~unlockMode;
    //按下 叉叉鍵 後切換馬達
    if(ps2x.Button(PSB_BLUE))
      changeMotor = ~changeMotor;
      
    //if (unlockMode == true) {
      if (140 <= ps2x.Analog(PSS_RY) || ps2x.Analog(PSS_RY) <= 120)
        if(ps2x.Button(PSB_R1))
        {  
            // 按下 (ps2x.Button(PSB_R1) + 類比搖桿 => 右邊邊搖桿控制單顆馬達前進後退 //具兩段速控制
              //切換兩顆馬達操控 //motorOne & motorTwo
              if (65 <= ps2x.Analog(PSS_RY) && ps2x.Analog(PSS_RY) <= 190) {
                if (ps2x.Analog(PSS_RY) < 128) {
                  digitalWrite(7,LOW);
                } else {
                  digitalWrite(7,HIGH);
                }
                digitalWrite(6,HIGH); 
                delayMicroseconds(100);
                digitalWrite(6,LOW); 
              } else {
                if (ps2x.Analog(PSS_RY) < 128) {
                  digitalWrite(7,LOW);
                  //Serial.println("Hello");
                } else {
                  digitalWrite(7,HIGH);
                }
                for(int i =0;i<8;i++){
                digitalWrite(6,HIGH); 
                delayMicroseconds(100);
                digitalWrite(6,LOW);
                delayMicroseconds(2000);
                }
             }
        } else {
              if (65 <= ps2x.Analog(PSS_RY) && ps2x.Analog(PSS_RY) <= 190) {
                if (ps2x.Analog(PSS_RY) < 128) {
                  digitalWrite(9,LOW);
                } else {
                  digitalWrite(9,HIGH);
                }
                digitalWrite(8,HIGH); 
                delayMicroseconds(100);
                digitalWrite(8,LOW); 
              } else {
                if (ps2x.Analog(PSS_RY) < 128) {
                  digitalWrite(9,LOW);
                } else {
                  digitalWrite(9,HIGH);
                }
                for(int i =0;i<8;i++){
                digitalWrite(8,HIGH); 
                delayMicroseconds(100);
                digitalWrite(8,LOW);
                delayMicroseconds(2000);
                }
              }
           }


           
      //按下 Button(PSB_L1)+ 類比搖桿 => 同時控制兩顆馬達運轉

      if (140 <= ps2x.Analog(PSS_LY) || ps2x.Analog(PSS_LY) <= 120)
          if (65 <= ps2x.Analog(PSS_LY) && ps2x.Analog(PSS_LY) <= 190) {
            if (ps2x.Analog(PSS_LY) < 128) {
              digitalWrite(11,LOW);
              digitalWrite(13,LOW);
            } else {
              digitalWrite(11,HIGH);
              digitalWrite(13,HIGH);
            }
            digitalWrite(10,HIGH); 
            delayMicroseconds(100);
            digitalWrite(10,LOW);

            digitalWrite(12,HIGH); 
            delayMicroseconds(100);
            digitalWrite(12,LOW);
             
          } else {
            if (ps2x.Analog(PSS_LY) < 128) {
              digitalWrite(11,LOW);
              digitalWrite(13,LOW);
            } else {
              digitalWrite(11,HIGH);
              digitalWrite(13,HIGH);
            }
            for(int i =0;i<8;i++){
            digitalWrite(10,HIGH); 
            delayMicroseconds(100);
            digitalWrite(10,LOW);
            delayMicroseconds(2000);

            digitalWrite(12,HIGH); 
            delayMicroseconds(100);
            digitalWrite(12,LOW);
            delayMicroseconds(2000);
            }
            //delayMicroseconds(100);
          }
         
  delay(10);
  }
}
