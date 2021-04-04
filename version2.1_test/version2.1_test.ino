/*
  version 2.1
  基本功能 + 初始化角度读取，避免初始化角度时的不平稳
  实现：eeprom存储角度的值
*/


#include <ld3320.h>
#include<EEPROM.h>

//舵机参数
#include<Servo.h>
Servo myservo_down, myservo_up;
int servoPin_down = 6, servoPin_up = 10;

//电机参数
int input1 = 3;
int input3 = 5;

//语音模块
VoiceRecognition Voice;

//超参数定义
int pos;

void setup() {
  //采样率读取
  Serial.begin(9600);
  Serial.print("Uart start!");
  
  //设置舵机引脚
  myservo_down.attach(servoPin_down);
  myservo_up.attach(servoPin_up);
  
  //设置电机引脚
  pinMode(input1, OUTPUT);
  pinMode(input3, OUTPUT);
  init_motorPin();
  
  //初始化角度读取
  pos = read_angle();
  //初始语音设置
  def_sound();
}

void loop() {
  // put your main code here, to run repeatedly:
  rec_sound();
}

//初始化电机引脚
void init_motorPin() {
  digitalWrite(input1, LOW);
  digitalWrite(input3, LOW);
}

//舵机模块
void lie_down() {
  for (; pos < 90; ++pos) {
    myservo_down.write(pos);
    myservo_up.write(90 - pos);
    delay(40);
  }
  delay(3000);
}
void sit_down() {
  for (; pos > 0; --pos) {
    myservo_down.write(pos);
    myservo_up.write(90 - pos);
    delay(40);
  }
  delay(3000);
}


//电机模块
void go_forward() {
  analogWrite(input1, 150);
  analogWrite(input3, 150);
  delay(3000);
  init_motorPin();
}
void now_stop() {
  init_motorPin();
}
void turn_left() {
  analogWrite(input1, 255);
  analogWrite(input3, 0);
  delay(5000);
  init_motorPin();
}
void turn_right() {
  analogWrite(input1, 0);
  analogWrite(input3, 255);
  delay(5000);
  init_motorPin();
}

//语音模块
void def_sound() {
  Voice.init();                               //初始化VoiceRecognition模块
  Voice.addCommand("qian", 0);             //添加指令，参数（指令内容，指令标签（可重复））
  Voice.addCommand("ting", 1);
  Voice.addCommand("ting ting", 1);
  Voice.addCommand("zuo zhuan", 2);
  Voice.addCommand("zuo", 2);
  Voice.addCommand("you zhuan", 3);
  Voice.addCommand("you", 3);
  Voice.addCommand("tang xia", 4);
//  Voice.addCommand("tang", 4);
  Voice.addCommand("sheng qi", 5);
//  Voice.addCommand("sheng", 5);
  Voice.start();//开始识别
}

void rec_sound() {
  switch (Voice.read()) {
    case 0:
      go_forward();
      Serial.print("  go_forward");
      break;
    case 1:
      now_stop();
      Serial.print("  now_stop");
      break;
    case 2:
      turn_left();
      Serial.print("  turn_left");
      break;
    case 3:
      turn_right();
      Serial.print("  turn_right");
      break;
    case 4:
      Serial.print("  lie_down");
      lie_down();
      record_angle();
      break;
    case 5:
      Serial.print("  sit_down");
      sit_down();
      record_angle();
      break;
    default:
      break;
  }
}

int read_angle() {
  pos = int(EEPROM.read(0X7F));
  return pos;
}

void record_angle() {
  EEPROM.write(0X7F, byte(pos));
}
