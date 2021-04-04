/*
  系统名称：语音控制机器人
  version2.0 稳定版
  功能介绍：直行，停止，转弯，躺下-升起
  作者：李朝辉
  时间：2021/3/22
*/

#include <ld3320.h>

//舵机参数
#include<Servo.h>
Servo myservo_down, myservo_up;
int servoPin_down = 6, servoPin_up = 10;

//电机参数
int input1 = 3, input3 = 5;
//语音模块
VoiceRecognition Voice;

int pos = 5;

void setup() {
  //采样率读取
  Serial.begin(9600);
  Serial.print("Uart start!");
  //设置舵机引脚
  myservo_down.attach(servoPin_down);
  myservo_up.attach(servoPin_up);
  delay(3000);
  //设置电机引脚
  pinMode(input1, OUTPUT);
  pinMode(input3, OUTPUT);
  //初始语音设置
  def_sound();
  //引脚初始化
  init_motorPin();
}

void loop() {
//   put your main code here, to run repeatedly:
    rec_sound();
//  serialText();

}

//初始化引脚
void init_motorPin() {
  digitalWrite(input1, LOW);
  digitalWrite(input3, LOW);
}

//舵机模块
void lie_down() { // 先背后脚
  Serial.println("  lie_down");
  for (int temp = pos; temp < 90; ++temp) {
    myservo_down.write(temp);
    delay(40);
  }
  delay(1000);
  for (; pos < 90; ++pos) {
    myservo_up.write(90 - pos);
    delay(40);
  }
  delay(500);
}
void sit_down() { // 先脚后背
  Serial.println("  sit_down");
  for (int temp = pos; temp > 0; --temp) {
    myservo_down.write(temp);
    delay(40);
  }
  delay(1000);
  for (; pos > 0; --pos) {
    myservo_up.write(90 - pos);
    delay(40);
  }
  delay(500);
}


//电机模块
void go_forward() {
  Serial.println("  go_forward");
  analogWrite(input1, 255);
  analogWrite(input3, 255);
//  delay(3000);
//  init_motorPin();
}

void now_stop() {
  Serial.println("  now_stop");
  init_motorPin();
//  delay(500);
}

void turn_left() {
  Serial.println("  turn_left");
  analogWrite(input1, 255);
  analogWrite(input3, 0);
  delay(5000);
  init_motorPin();
}
void turn_right() {
  Serial.println("  turn_right");
  analogWrite(input1, 0);
  analogWrite(input3, 255);
  delay(5000);
  init_motorPin();
}

//语音模块
void def_sound() {
  Voice.init();                               //初始化VoiceRecognition模块
    Voice.addCommand("qian jin", 0);             //添加指令，参数（指令内容，指令标签（可重复））
  //  Voice.addCommand("zou", 0);
    Voice.addCommand("ting", 1);
  //  Voice.addCommand("ting xia", 1);
  //  Voice.addCommand("ting ting ting", 1);
    Voice.addCommand("zuo zhuan", 2);
    Voice.addCommand("you zhuan", 3);
  Voice.addCommand("tang xia", 4);
  Voice.addCommand("sheng qi", 5);
  Voice.start();//开始识别
}

void rec_sound() {
  switch (Voice.read()) {
    case 0:
      go_forward();
      Serial.println("s  go_forward");
      break;
    case 1:
      now_stop();
      Serial.println("s  now_stop");
      break;
    case 2:
      turn_left();
      Serial.println("s  turn_left");
      break;
    case 3:
      turn_right();
      Serial.println("s  turn_right");
      break;
    case 4:
      Serial.println("s  lie_down");
      lie_down();
      break;
    case 5:
      Serial.println("s  sit_down");
      sit_down();
      break;
    default:
      break;
  }
}

void serialText() {
  if (Serial.available()) {
    char ch = Serial.read();
    switch (ch) {
      case 'l':
        delay(5000);
        lie_down();
        delay(2000);
        sit_down();
        break;
      case 's':
        sit_down();
        break;
      case 'f':
        go_forward();
        break;
      case 'b':
        now_stop();
        break;
      case 'z':
        turn_left();
        break;
      case 'y':
        turn_right();
        break;
      default :
        break;
    }
  }
  delay(1000);
}
