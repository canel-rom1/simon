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

#define DELAYON 500               // Temps de clignotement dans LEDS [ms]
#define DELAYSEQ 1500             // Délais entre la fin de la saisie et la prochaine séquence [ms]
#define SEQ_SIZE 128              // Taille du temps contenant la séquence

enum{
  LED1 = 1,
  LED2,
  LED3,
  LED4
};
const int nb_led = 4;

/* PROTOTYPE */
void addLED2SEQ(int*, int);
int initInterruptSW(int button);
//int initInterruptSW(int button, void *buttonISR);
int ledNum2Pin(int led);
void ledOff(int led);
void ledOn(int led);
void ledOnFor(int led, int delayOn);
int randomLED(void);
int readButtons(void);
int readSEQ(int*,int);
void playSEQ(int*, int);
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
  int i = 1;
  int seq_leds[SEQ_SIZE];

#ifdef DEBUG
  Serial.println("START GAME");
#endif//DEBUG
  validate();

  while(true)
  {
  #ifdef DEBUG
    Serial.print("SEQUENCE N°");
    Serial.println(i);
  #endif//DEBUG
    addLED2SEQ(seq_leds, i);
    playSEQ(seq_leds, i);
    if(readSEQ(seq_leds, i))
      break;
    i++;
    delay(DELAYSEQ);
  }

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

int ledNum2Pin(int led)
{
  switch(led)
  {
    case LED1:
      return LED_PIN1;
    case LED2:
      return LED_PIN2;
    case LED3:
      return LED_PIN3;
    case LED4:
      return LED_PIN4;
    default:
      return -1;
  }
}

void ledOff(int led)
{
  digitalWrite(ledNum2Pin(led), LOW);
}

void ledOn(int led)
{
  digitalWrite(ledNum2Pin(led), HIGH);
}

void ledOnFor(int led, int delayON)
{
  ledOn(led);
  delay(delayON);
  ledOff(led);
}

int ledBlink(int led, int delayBlink)
{
  ledOn(led);
  delay(delayBlink);
  ledOff(led);
  delay(delayBlink);
}

int readButtons(void)
{
  while(1)
  {
    if(digitalRead(SW_PIN1))
    {
      ledOnFor(LED1, 300);
      return LED1;
    }
    if(digitalRead(SW_PIN2))
    {
      ledOnFor(LED2, 300);
      return LED2;
    }
    if(digitalRead(SW_PIN3))
    {
      ledOnFor(LED3, 300);
      return LED3;
    }
    if(digitalRead(SW_PIN4))
    {
      ledOnFor(LED4, 300);
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

void playSEQ(int *sequence, int nb_seq)
{
#ifdef DEBUG
  Serial.print("LEDs number:");
#endif//DEBUG
  for(int i=0 ; i < nb_seq ; i++)
  {
  #ifdef DEBUG
    Serial.print(" ");
    Serial.print(sequence[i]);
  #endif//DEBUG
    ledBlink(sequence[i], DELAYON);
  }
#ifdef DEBUG
  Serial.println();
#endif//DEBUG
}

int readSEQ(int *seq, int nb_seq)
{
  disableInterrupts();
  /* Joue la séquance */
  for(int i=0 ; i < nb_seq ; i++)
  {
    /* Lit les buttons et compare dans la séquance */
    if(seq[i] != readButtons())
    {
    #ifdef DEBUG
      Serial.print("Séquence KO: ");
      Serial.println(i+1);
    #endif//DEBUG
      return 1;
    }
  }
  enableInterrupts();

#ifdef DEBUG
  Serial.println("Séquence OK");
#endif//DEBUG
  return 0;
}


/**********/
/* OTHERS */
/**********/
int randomLED(void)
{
  return ESP8266TrueRandom.random(1, nb_led+1);
}

void signal(int nb, int delayBlink)
{
  for(int i=0 ; i < nb ; i++)
  {
    ledOn(LED1);
    ledOn(LED2);
    ledOn(LED3);
    ledOn(LED4);
    delay(delayBlink);
    ledOff(LED1);
    ledOff(LED2);
    ledOff(LED3);
    ledOff(LED4);
    delay(delayBlink);
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
