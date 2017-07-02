/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"


GPIO_InitTypeDef  GPIO_InitStructure;

#define R1  GPIO_Pin_0
#define G1  GPIO_Pin_1
#define B1  GPIO_Pin_2
#define R2  GPIO_Pin_3
#define G2  GPIO_Pin_4
#define B2  GPIO_Pin_5
#define A   GPIO_Pin_6
#define B   GPIO_Pin_7
#define C   GPIO_Pin_8
#define D   GPIO_Pin_9
#define CLK GPIO_Pin_13
#define LAT GPIO_Pin_14
#define OE  GPIO_Pin_15

#define DELAY_PERIOD 0xF

struct Block {
  uint8_t r[8];
  uint8_t g[8];
  uint8_t b[8];
};

void Delay(__IO uint32_t nCount);
void ClockIn(uint32_t bits);
void PulseOff(uint16_t pin);
void PulseOn(uint16_t pin);
void SetAddr(uint8_t addr);
void Set(uint16_t pin);
void Reset(uint16_t pin);
void SetColour(uint8_t col);
void ColorBox(void);
void EmptyBox(void);


int main(void)
{

  /* GPIOD Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = R1 | G1 | B1 | R2 | G2 | B2 | A | B | C | D | CLK | LAT | OE;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

  uint8_t row;
  row = 0;
  uint8_t i, j;

  struct Block block_arr[16];

  // Initialise all bits to zero
  for (i=0; i<16; i++) {
    for (j=0; j<8; j++) {
      block_arr[i].r[j] = 0;
      block_arr[i].g[j] = 0;
      block_arr[i].b[j] = 0;
    }
  }

  if (block_arr[0].r[3] == 0) ColorBox();

  while (1)
  {

    // EmptyBox();

    ColorBox();
    EmptyBox();
    EmptyBox();
    EmptyBox();
    EmptyBox();
    EmptyBox();
    EmptyBox();
    EmptyBox();


    // ClockIn(64);
    Reset(OE);
    SetAddr(row);
    Set(OE);

    Set(LAT);
    Reset(LAT);



    row++;
    if (row >= 8) row = 1;
  }
}

void ColorBox() {
  uint8_t count;
  for (count=0; count<8; count++) {
    SetColour(count % 8);
    PulseOn(CLK);
  }
}

void EmptyBox() {
  SetColour(0);
  ClockIn(8);
}

void SetAddr(uint8_t addr) {
  if (addr & 0x01) {
    GPIO_SetBits(GPIOA, A);
    GPIO_SetBits(GPIOD, GPIO_Pin_12);
  }
  else {
    GPIO_ResetBits(GPIOA, A);
    GPIO_ResetBits(GPIOD, GPIO_Pin_12);
  }

  if (addr & 0x02) {
    GPIO_SetBits(GPIOA, B);
    GPIO_SetBits(GPIOD, GPIO_Pin_13);
  }
  else {
    GPIO_ResetBits(GPIOA, B);
    GPIO_ResetBits(GPIOD, GPIO_Pin_13);
  }

  if (addr & 0x04) {
    GPIO_SetBits(GPIOA, C);
    GPIO_SetBits(GPIOD, GPIO_Pin_14);
  }
  else {
    GPIO_ResetBits(GPIOA, C);
    GPIO_ResetBits(GPIOD, GPIO_Pin_14);
  }

  if (addr & 0x08) {
    GPIO_SetBits(GPIOA, D);
    GPIO_SetBits(GPIOD, GPIO_Pin_15);
  }
  else {
    GPIO_ResetBits(GPIOA, D);
    GPIO_ResetBits(GPIOD, GPIO_Pin_15);
  }
}

void SetColour(uint8_t colour) {
  GPIO_ResetBits(GPIOA, R1 | G1 | B1 | R2 | G2 | B2);

  if (colour & 0x01) {
    GPIO_SetBits(GPIOA, R1);
  }

  if (colour & 0x02) {
    GPIO_SetBits(GPIOA, G1);
  }

  if (colour & 0x04) {
    GPIO_SetBits(GPIOA, B1);
  }

  if (colour & 0x10) {
    GPIO_SetBits(GPIOA, R2);
  }

  if (colour & 0x20) {
    GPIO_SetBits(GPIOA, G2);
  }

  if (colour & 0x40) {
    GPIO_SetBits(GPIOA, B2);
  }
}

void ClockIn(uint32_t bits) {
  uint32_t j;
  for (j=0; j < bits; j++) {
    GPIO_SetBits(GPIOA, CLK);
    Delay(DELAY_PERIOD);
    GPIO_ResetBits(GPIOA, CLK);
    Delay(DELAY_PERIOD);
  }
}

void Reset(uint16_t pin) {
  GPIO_ResetBits(GPIOA, pin);
}

void Set(uint16_t pin) {
  GPIO_SetBits(GPIOA, pin);
}

void PulseOn(uint16_t pin) {
  GPIO_SetBits(GPIOA, pin);
  Delay(DELAY_PERIOD);
  GPIO_ResetBits(GPIOA, pin);
  Delay(DELAY_PERIOD);
}

void PulseOff(uint16_t pin) {
  GPIO_ResetBits(GPIOA, pin);
  Delay(DELAY_PERIOD);
  GPIO_SetBits(GPIOA, pin);
  Delay(DELAY_PERIOD);
}

void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}
