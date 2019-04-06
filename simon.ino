/*
Project: SIMON
File: simon.ino
Version: 0.1
Create by: Rom1 <rom1@canel.ch> - CANEL - https://www.canel.ch
Date: 06/04/19
License: GNU GENERAL PUBLIC LICENSE v3
Language: Arduino (C/C++)
Description: Jeu du SIMON, un jeu de mémoire
*/
#define DEBUG

/* PINOUTS */
#define LED_PIN1 D0
#define LED_PIN2 D1
#define LED_PIN3 D2
#define LED_PIN4 D3

#define SW_PIN1 D5
#define SW_PIN2 D6
#define SW_PIN3 D7
#define SW_PIN4 D8

#define DELAYON 500
#define SEQ_SIZE 128

enum{
  LED1 = 0,
  LED2,
  LED3,
  LED4
};
const int nb_led = 4;

int seq_leds[SEQ_SIZE];

/* PROTOTYPE */
void ledON(int);
void ledsON(int);
int randomLED(void);
int readButtons(void);
void readSEQ(int*);
void writeSEQ(int*);


/*********/
/* SETUP */
/*********/
void setup(void)
{
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println();
  Serial.println("DEBUG mode for SIMON game");
#endif//DEBUG

  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);
  pinMode(LED_PIN4, OUTPUT);

  pinMode(SW_PIN1, INPUT);
  pinMode(SW_PIN2, INPUT);
  pinMode(SW_PIN3, INPUT);
  pinMode(SW_PIN4, INPUT);

  randomSeed(analogRead(0));
}


/********/
/* MAIN */
/********/
void loop(void)
{
#ifdef DEBUG
  Serial.println("Write SEQ");
#endif//DEBUG
  writeSEQ(seq_leds);
#ifdef DEBUG
  Serial.println("Read SEQ");
#endif//DEBUG
  readSEQ(seq_leds);

  while(1)
  {
    Serial.println("FIN");
    delay(1000);
  }
}


/****************/
/* INPUT/OUTPUT */
/****************/
void ledsON(int led)
{
  switch(led)
  {
    case LED1:
      ledON(LED_PIN1);
      break;
    case LED2:
      ledON(LED_PIN2);
      break;
    case LED3:
      ledON(LED_PIN3);
      break;
    case LED4:
      ledON(LED_PIN4);
      break;
  }
}

void ledON(int led)
{
  digitalWrite(led, HIGH);
  delay(DELAYON);
  digitalWrite(led, LOW);
  delay(DELAYON);
}

int readButtons(void)
{
  while(1)
  {
    if(digitalRead(SW_PIN1))
    {
      delay(300);
      return LED1;
    }
    if(digitalRead(SW_PIN2))
    {
      delay(300);
      return LED2;
    }
    if(digitalRead(SW_PIN3))
    {
      delay(300);
      return LED3;
    }
    if(digitalRead(SW_PIN4))
    {
      delay(300);
      return LED4;
    }
    delay(10);
  }
}


/************/
/* SEQUENCE */
/************/
void readSEQ(int *sequence)
{
  for(int i=0 ; i < 10 ; i++)
  {
  #ifdef DEBUG
    Serial.print("read LED: ");
    Serial.println(sequence[i]);
  #endif//DEBUG
    ledsON(sequence[i]);
  }
}

void writeSEQ(int *sequence)
{
  for(int i=0 ; i < 10 ; i++)
  {
    sequence[i] = randomLED();
  #ifdef DEBUG
    Serial.print("wite LED: ");
    Serial.println(sequence[i]);
  #endif//DEBUG
  }
}


/**********/
/* OTHERS */
/**********/
int randomLED(void)
{
  return random(4);
}


// vim: ft=arduino tw=80 et ts=2 sw=2
