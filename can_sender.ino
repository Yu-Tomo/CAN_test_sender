#define CAN_CS 6
#define CAN_INT 2

#include <CAN.h>
#include <string.h>

void setup() {
  CAN.setPins(CAN_CS, CAN_INT);
  
  Serial.begin(9600);
  while (!Serial);
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
  Serial.println("CAN sender start!");
}
void loop() {
  uint16_t uiCan_id;
  int iByte;
  long lCan_data;
  unsigned char ucSend[4];
  uint8_t ucWork;
  
  while (1)
  {
    uiCan_id = 0;
    iByte = 0;
    lCan_data = -1;
    ucWork = 0;

    for (int i = 0; i < 4; i++)
    {
      ucSend[i] = 0;
    }
    
    Serial.println("Please input CAN_ID : ");
    uiCan_id = SerialInput();
    Serial.print("OK! Your inputing is (DEC) : ");
    Serial.print(uiCan_id);
    Serial.print(" (HEX) : 0x");
    Serial.println(uiCan_id, HEX);

    while (!((lCan_data >= 0 ) && (lCan_data <= 0x7FFFFFFF)))
    {
      Serial.println("Please input your sending data :");
      lCan_data = SerialInput();
    }

    Serial.print("OK! Your inputing is (DEC) : ");
    Serial.println(lCan_data);
    Serial.print("Your inputing is (HEX) : ");
    Serial.print("0x");
    Serial.println(lCan_data, HEX);

    for (int i = 3 ; i >= 0; i--)
    {
      ucWork = lCan_data >> 8 * (3-i);
      ucSend[i] = ucWork;
    }
    Serial.println("Big Endian");
    Serial.println("Sending packet ... ");
    CAN.beginPacket(uiCan_id);
    for (int i = 0; i < 4; i++)
    {
      CAN.write(ucSend[i]);
      Serial.print("0x");
      Serial.println(ucSend[i], HEX);
    }
    CAN.endPacket();
    Serial.println("done");
    delay(1000);
  }
}
long SerialInput(void)
{
  int iByte_cnt = 0;
  long work = 0;
  char ucData[256];
  char dec_hex = 1;         //0以外の時dec
  while (!Serial.available())
  {

  }
  delay(10);
  
  iByte_cnt = Serial.available();
  
  for (int i = 0; i < iByte_cnt; i++)
  {
    ucData[i] = Serial.read();
  }
  
  //改行コードがOSによって異なるため、有効文字の数を数える
  iByte_cnt = String_len(&ucData[0]);
  dec_hex = strncmp(ucData, "0x", 2);

  //入力がHEX
  if (dec_hex == 0)
  {
    Serial.println("HEX ");

    for (int i = 2; i < iByte_cnt; i++)
    {
      work <<= 4;
      if ((ucData[i] >= '0') && (ucData[i] <= '9'))
      {
        //'0'~'9'をHEXに変換
        work |= (ucData[i] - 0x30);
      }
      else if ((ucData[i] >= 'A') && (ucData[i] <= 'F'))
      {
        //'A'~'F'をHEXに変換
        work |= (ucData[i] - 0x37);
      }
      else if ((ucData[i] >= 'a') && (ucData[i] <= 'f'))
      {
        //'a'~'f'をHEXに変換
        work |= (ucData[i] - 0x57);
      }
      else
      {
        //異常終了
        return -1;
      }
    }
  }
  //入力がDEC
  else
  {
    Serial.println("DEC");
    for (int i = 0; i < iByte_cnt; i++)
    {
      work *= 10;
      work += (ucData[i] - 0x30);
    }
  }
  return work;
}
int String_len(char* str)
{
  int iCnt = 0;
  while ((*str >= 0x30 ) && (*str <= 0x7a))
  {
    str++;
    iCnt++;
  }
  return iCnt;
}
