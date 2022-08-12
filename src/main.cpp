#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

const int RED_PIN = 11;   // led đỏ
const int GREEN_PIN = 10; // led xanh lá
const int BLUE_PIN = 9;   // led xanh dương
const int LIGHT_PIN = 2;
const int OUT_PIN = 8;
int l;
int state;

int DELAY_TIME = 500;

TaskHandle_t task1_handle = NULL;

void displayAllBasicColors()
{
  // Tắt toàn bộ các led - cái này dễ mà ha

  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);

  delay(DELAY_TIME);

  // Chỉ bật led đỏ

  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);

  delay(DELAY_TIME);

  // Chỉ bật led xanh lá

  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, LOW);

  delay(DELAY_TIME);

  // Chỉ bật led xanh dương

  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, HIGH);

  delay(DELAY_TIME);

  // Bật màu vàng bắng cách bật led đỏ và xanh

  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, LOW);

  delay(DELAY_TIME);

  // Xanh lam (Cyan) bằng cách bật led xanh lá và xanh dương

  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, HIGH);

  delay(DELAY_TIME);

  // Tím (đỏ xanh dương)

  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, HIGH);

  delay(DELAY_TIME);

  // Màu trắng (tất cả các led)
  // Mình không hiểu nổi vụ con công tô màu cho con quạ :3, đáng lẻ phải ra màu trắng chứ, mà thế quái nào lại ra màu đen :3, chắc do con công pha màu kém quá :D

  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, HIGH);

  delay(DELAY_TIME);
}

void showRGB(int color)
{
  int redPWM;
  int greenPWM;
  int bluePWM;

  if (color <= 255) // phân vùng 1
  {
    redPWM = 255 - color; // red đi từ sáng về tắt
    greenPWM = color;     // green đi từ tắt thành sáng
    bluePWM = 0;          // blue luôn tắt
  }
  else if (color <= 511) // phân vùng 2
  {
    redPWM = 0;                     // đỏ luôn tắt
    greenPWM = 255 - (color - 256); // green đi từ sáng về tắt
    bluePWM = (color - 256);        // blue đi từ tắt thành sáng
  }
  else // color >= 512       // phân vùng 3
  {
    redPWM = (color - 512);        // red tắt rồi lại sáng
    greenPWM = 0;                  // green luôn tắt nhé
    bluePWM = 255 - (color - 512); // blue sáng rồi lại tắt
  }

  // rồi xuất xung ra và chơi thôi :3

  analogWrite(RED_PIN, redPWM);
  analogWrite(BLUE_PIN, bluePWM);
  analogWrite(GREEN_PIN, greenPWM);
}

void showSpectrum()
{
  for (int i = 0; i < 768; i++)
  {
    showRGB(i); // Call RGBspectrum() with our new x
    //delay(10);  // Delay 10ms
    vTaskDelay(1);
  }
}

void TaskLed(void *pvParameters) // This is a task.
{
  (void)pvParameters;

  for (;;)
  {
    displayAllBasicColors();
    showSpectrum();
  }
}

void TaskMain(void *pvParameters) // This is a task.
{
  (void)pvParameters;

  for (;;)
  {
    l = digitalRead(LIGHT_PIN);
    if (!l)
    {
      digitalWrite(OUT_PIN, HIGH);
      // Serial.println("yes");
      state = 0;
    }
    else
    {
      digitalWrite(OUT_PIN, LOW);
      state = 1;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(9600);

  pinMode(LIGHT_PIN, INPUT);
  pinMode(OUT_PIN, OUTPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  xTaskCreate(TaskLed, "Led", 128, NULL, 1, &task1_handle);
  xTaskCreate(TaskMain, "Main", 128, NULL, 3, NULL);
}

void loop()
{
  if (state)
  {
    vTaskResume(task1_handle);
  }
  else
  {
    vTaskSuspend(task1_handle);
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
  }
}
