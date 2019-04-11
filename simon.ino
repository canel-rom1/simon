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
#include "ESP8266TrueRandom.h"

/* DEBUG MODE */
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
  LED1 = 1,
  LED2,
  LED3,
  LED4
};
const int nb_led = 4;

int seq_leds[SEQ_SIZE];
int i_seq;

/* PROTOTYPE */
void addLED2SEQ(int*, int);
int checkSEQ(int, int*, int*);
int initInterruptSW(int button);
//int initInterruptSW(int button, void *buttonISR);
void ledON(int);
void ledsON(int);
int randomLED(void);
int readButtons(void);
int readSEQ(int*);
void playSEQ(int*);
void signal(int, int);
void validate(void);

void buttonISR(void);

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
  //pinMode(SW_PIN4, INPUT);

  initInterruptSW(SW_PIN4);

  randomSeed(analogRead(0));
}


/********/
/* MAIN */
/********/
void loop(void)
{
  i_seq = 1;

/*
#ifdef DEBUG
  Serial.println("START GAME");
#endif//DEBUG
  validate();

  for(int i=1 ; i < SEQ_SIZE ; i++)
  {
  #ifdef DEBUG
    Serial.print("SEQUENCE N°");
    Serial.println(i);
  #endif//DEBUG
    addLED2SEQ(seq_leds, i);
    playSEQ(seq_leds);
    delay(100);
    signal(1, 50);
    delay(100);
  }
*/

delay(5000);
Serial.println("Disable Interrupt");
//noInterrupts();
detachInterrupt(digitalPinToInterrupt(SW_PIN4));
delay(5000);
digitalRead(SW_PIN4);
interrupts();
Serial.println("Enable Interrupt");
  initInterruptSW(SW_PIN4);

#ifdef DEBUG
  Serial.println("END GAME");
#endif//DEBUG
}


/****************/
/* INPUT/OUTPUT */
/****************/
int initInterruptSW(int button)
//int initInterruptSW(int button, void *buttonISR_)
{
  pinMode(button, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button), buttonISR, RISING);
  //attachInterrupt(digitalPinToInterrupt(button), buttonISR_, RISING);
}

void enableInterrupts()
{
  initInterruptSW(SW_PIN1);
  initInterruptSW(SW_PIN2);
  initInterruptSW(SW_PIN3);
  initInterruptSW(SW_PIN4);
}

void disableInterrupts()
{
  detachInterrupt(digitalPinToInterrupt(SW_PIN1));
  detachInterrupt(digitalPinToInterrupt(SW_PIN2));
  detachInterrupt(digitalPinToInterrupt(SW_PIN3));
  detachInterrupt(digitalPinToInterrupt(SW_PIN4));
}

void buttonISR(void)
{
  Serial.println("interrupt");
}

void ledON(int led)
{
  digitalWrite(led, HIGH);
  delay(DELAYON);
  digitalWrite(led, LOW);
  delay(DELAYON);
}

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
void addLED2SEQ(int *sequence, int i)
{
  sequence[i-1] = randomLED();
#ifdef DEBUG
  Serial.print("Ajouter à la chaine: ");
  Serial.println(sequence[i-1]);
#endif//DEBUG
}

void playSEQ(int *sequence)
{
#ifdef DEBUG
  Serial.print("LEDs number:");
#endif//DEBUG
  for(int i=0 ; sequence[i] != 0 ; i++)
  {
  #ifdef DEBUG
    Serial.print(" ");
    Serial.print(sequence[i]);
  #endif//DEBUG
    ledsON(sequence[i]);
  }
#ifdef DEBUG
  Serial.println();
#endif//DEBUG
}

int readSEQ(int *seq)
{
  int ret = 0;
  int seq_tmp[SEQ_SIZE];
  int i_tmp = 0;

  disableInterrupts();
  for(int i=0 ; i < i_seq ; i++)
  {
    seq_tmp[i] = readButtons();

    if(checkSEQ(i_tmp, seq, seq_tmp) == 1)
      ret=1;
  }
  enableInterrupts();

  return ret;
}

int checkSEQ(int nb, int *seq1, int *seq2)
{
  if(seq1 == NULL || seq2 == NULL)
  {
  #ifdef DEBUG
    Serial.println("[checkSEQ]: NULL Pointers");
  #endif//DEBUG
    return -1;
  }

  for(int i=0 ; i<nb ; i++)
  {
    if(seq1[i] != seq2[i])
    {
    #ifdef DEBUG
      Serial.println("Bad sequence");
    #endif//DEBUG
      return 1;
    }
  }
  return 0;
}

int printSEQ(int *sequence)
{
  if(sequence == NULL)
  {
  #ifdef DEBUG
    Serial.println("[printSEQ]: NULL Pointers");
  #endif//DEBUG
    return -1;
  }

  //for(int i=0 ; 0 )
}

int recordSEQ(int nb, int *seq1, int *seq2)
{
  if(seq1 == NULL || seq2 == NULL)
  {
  #ifdef DEBUG
    Serial.println("[recordSEQ]: NULL Pointers");
  #endif//DEBUG
    return -1;
  }
  
  for(int i=0 ; i<nb ; i++)
  {
    seq1[i] = seq2[i];
  }
  return 0;
}


/**********/
/* OTHERS */
/**********/
int randomLED(void)
{
  return ESP8266TrueRandom.random(1, nb_led+1);
}

void signal(int nb, int delay_)
{
  for(int i=0 ; i < nb ; i++)
  {
    digitalWrite(LED_PIN1, HIGH);
    digitalWrite(LED_PIN2, HIGH);
    digitalWrite(LED_PIN3, HIGH);
    digitalWrite(LED_PIN4, HIGH);
    delay(delay_);
    digitalWrite(LED_PIN1, LOW);
    digitalWrite(LED_PIN2, LOW);
    digitalWrite(LED_PIN3, LOW);
    digitalWrite(LED_PIN4, LOW);
    delay(delay_);
  }
}

void validate(void)
{
#ifdef DEBUG
  Serial.println("Appuyer sur le bouton.");
#endif//DEBUG
  while(digitalRead(SW_PIN1) == 0)//Améliorer la vitesse de lecture
  {
    /* Faire clignoter les LEDs avant de commencer la partie */
    signal(1, DELAYON);
  }
}


// vim: ft=arduino tw=80 et ts=2 sw=2
