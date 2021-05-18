/**
 * A simple script to keep track of a 2 digit score. This assumes that there are two switches that act as score incrementer
 * and decrementer. It also assumes you have the numbers drawn up in a snake pattern with indexes as so:
 * 
 * 0  -> 1  ->  2         ---> 15 -> 16 -> 17
 *              |        |                  |
 *              v        |                  v
 * 5  <- 4  <-  3        |     20 <- 19 <- 18  
 * |                     |     |
 * v                     |     v
 * 6  -> 7  ->  8        |     21 -> 22 -> 23
 *              |        |                  |
 *              v        |                  v
 * 11 <- 10 <-  9        |     26 <- 25 <- 24
 * |                     |     |
 * v                     |     v
 * 12 -> 13 -> 14 -------      27 -> 28 -> 29
 */

//ws2812b/ws2811
#include <FastLED.h>
#define LED_PIN        4
#define LEDS_PER_DIGIT 15
#define NUM_LEDS       30
CRGB leds[NUM_LEDS];
CRGB emptyColor = CRGB(0, 0, 0);

// define what each digit looks like, remember we are in a serpentine pattern
boolean zero[LEDS_PER_DIGIT] = {
  true, true, true,
  true, false, true,
  true, false, true,
  true, false, true,
  true, true, true
};

boolean one[LEDS_PER_DIGIT] = {
  true, false, false,
  false, false, true,
  true, false, false,
  false, false, true,
  true, false, false
};

boolean two[LEDS_PER_DIGIT] = {
  true, true, true,
  false, false, true,
  true, true, true,
  true, false, false,
  true, true, true
};

boolean three[LEDS_PER_DIGIT] = {
  true, true, true,
  false, false, true,
  true, true, true,
  false, false, true,
  true, true, true
};

boolean four[LEDS_PER_DIGIT] = {
  true, false, true,
  true, false, true,
  true, true, true,
  false, false, true,
  true, false, false
};

boolean five[LEDS_PER_DIGIT] = {
  true, true, true,
  true, false, false,
  true, true, true,
  false, false, true,
  true, true, true
};

boolean six[LEDS_PER_DIGIT] = {
  true, true, true,
  true, false, false,
  true, true, true,
  true, false, true,
  true, true, true
};

boolean seven[LEDS_PER_DIGIT] = {
  true, true, true,
  false, false, true,
  true, false, false,
  false, false, true,
  true, false, false
};

boolean eight[LEDS_PER_DIGIT] = {
  true, true, true,
  true, false, true,
  true, true, true,
  true, false, true,
  true, true, true
};

boolean nine[LEDS_PER_DIGIT] = {
  true, true, true,
  true, false, true,
  true, true, true,
  false, false, true,
  true, false, false
};

// Score Params
#define INC_PIN   12
#define DEC_PIN   14
#define MAX_SCORE 21

int score = 0;
int delayBetweenScoreChange = 200;

void setup() {
  Serial.begin(9600);
  
  pinMode(INC_PIN, INPUT);
  pinMode(DEC_PIN, INPUT);

  // init fast led
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  clearLeds();
  renderScore();
}

void loop() {
  int prevScore = score;
  decrementScoreIfNeeded();
  incrementScoreIfNeeded();
  if(prevScore != score) {
    renderScore();
  }
}

void decrementScoreIfNeeded(){
  boolean shouldDecrement = digitalRead(DEC_PIN) == HIGH;
  if(shouldDecrement) {
    //reset to max score less than 0
    score -= (score > 0) ? 1 : -MAX_SCORE;
    delay(delayBetweenScoreChange);
  }
}

void incrementScoreIfNeeded(){
  boolean shouldIncrement = digitalRead(INC_PIN) == HIGH;
  if(shouldIncrement){
    //reset score at MAX_SCORE
    score += (score < MAX_SCORE) ? 1 : -MAX_SCORE;
    delay(delayBetweenScoreChange);
  }
}

void renderScore(){
  boolean *firstDigit = getDigit(score/10);
  boolean *secondDigit = getDigit(score%10);
  drawDigit(firstDigit, false);
  drawDigit(secondDigit, true);
  FastLED.show();
}

void drawDigit(boolean *digit, boolean isSecondDigit) {
  // offset is needed for first digit since it shows up on further indexes
  int offset = isSecondDigit ? 0 : LEDS_PER_DIGIT;
  for(int i = 0; i < LEDS_PER_DIGIT; i++) {
    int index = i + offset;
    if(digit[i]) {
      leds[index].setHue(150 + score);
    }else{
      leds[index] = emptyColor;
    }
  }
}

boolean * getDigit(int digit) {
  switch(digit){
    case 0:
      return zero;
    case 1:
      return one;
    case 2:
      return two;
    case 3:
      return three;
    case 4:
      return four;
    case 5:
      return five;
    case 6:
      return six;
    case 7:
      return seven;
    case 8:
      return eight;
    case 9:
      return nine;
    default:
      return zero;
  }
}

void clearLeds() {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = emptyColor;
  }
}
