#include <Servo.h>
#include <LiquidCrystal_I2C.h>

Servo myServo;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // 初始化LCD屏，0x27是默认的I2C地址，16表示每行16个字符，2表示2行（根据实际情况调整）

int irSensorPin = 2;  // 人体红外传感器引脚
int soundSensorPin = A0;  // 声音传感器引脚
int buzzerPin =8;  // 蜂鸣器引脚
int ledPin = 13;  // LED灯引脚
int servoPin = 9;  // 舵机引脚

// 定义生日快乐歌的音符频率数组
int melody[] = {
  262, 294, 330, 262, 262, 294, 330, 262,
  330, 349, 392, 330, 349, 392, 392, 440,
  392, 349, 330, 262, 392, 440, 392, 349,
  330, 262, 294, 196, 262, 294, 196, 0
};

// 定义每个音符的持续时间数组（以毫秒为单位）
int noteDurations[] = {
  250, 250, 250, 250, 250, 250, 250, 250,
  250, 250, 250, 250, 250, 250, 500, 250,
  250, 250, 250, 250, 500, 250, 250, 250,
  250, 250, 250, 500, 250, 250, 500, 0
};

void setup() {
  // 初始化传感器引脚为输入模式
  pinMode(irSensorPin, INPUT);
  pinMode(soundSensorPin, INPUT);

  // 初始化输出设备引脚为输出模式
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  myServo.attach(servoPin);

  // 初始化LCD屏
  lcd.init();
  lcd.backlight();

  // 初始化串口通信（可选，用于调试）
  Serial.begin(9600);
  myServo.write(0);
}

void loop() {
  // 检测人体红外传感器
  if (digitalRead(irSensorPin) == HIGH) {
    // 播放生日快乐歌
    playMelody();
    // 点亮LED灯
    digitalWrite(ledPin, HIGH);
    // 打开盒子（控制舵机旋转）
    openBox();
    // 在LED屏上显示祝福语
    displayMessageOnLcd("Happy Birthday");
  }

  // 检测声音传感器
  int soundValue = analogRead(soundSensorPin);
  if (soundValue > 500) {  // 设定声音阈值，可根据实际情况调整
    // 熄灭LED灯
    digitalWrite(ledPin, LOW);
    // 停止播放声音
    noTone(buzzerPin);
  }
}

// 播放生日快乐歌的函数
void playMelody() {
  for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
    int noteDuration = noteDurations[i];
    if (melody[i] == 0) {
      delay(noteDuration);
    } else {
      tone(buzzerPin, melody[i], noteDuration);
      delay(noteDuration);
      noTone(buzzerPin);
      delay(50);  // 音符之间的短暂停顿
    }
  }
}

// 打开盒子的函数（控制舵机旋转）
void openBox() {
  myServo.write(90);  // 假设90度是打开位置，可根据实际情况调整
  delay(1000);  // 等待盒子完全打开
}

// 在LED屏上显示消息的函数
void displayMessageOnLcd(String message) {
  lcd.clear();  // 清屏
  lcd.setCursor(0, 0);  // 设置光标位置为第一行第一列
  lcd.print(message);
}
