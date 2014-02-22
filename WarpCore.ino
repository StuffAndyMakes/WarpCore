#define RING_COUNT 6
#define HEARTBEAT_PIN 18
#define MS_BETWEEN_UPDATES 17 // roughly 60 frames a second
#define MAX_MS_BETWEEN_UPDATES 25.0
#define DECAY_AMOUNT 8 // number subtracted from ring value each frame
#define POT_PIN 14

int updatesEnergized = 17; // how many updates the ring stays at full energy
unsigned long  nextUpdate = millis(); // start right away with an update
int ringPin[RING_COUNT] = { 3, 6, 10, 9, 4, 2 }; // pins assigned to rings
int ringValue[RING_COUNT] = { 0, 0, 0, 0, 0, 0 };
int currentRing = 0;

void setup() {
  pinMode( HEARTBEAT_PIN, OUTPUT );
  digitalWrite( HEARTBEAT_PIN, LOW );
  pinMode( POT_PIN, INPUT );
  for( int n = 0; n < RING_COUNT; n++ ) {
    pinMode( ringPin[n], OUTPUT );
    analogWrite( ringPin[n], 0 );
  }
  pinMode( 14, INPUT );
}

void loop() {

  if( millis() >= nextUpdate ) {

    // turn on heartbeat LED
    digitalWrite( HEARTBEAT_PIN, HIGH );

    // loop through rings to update them
    for( int n = 0; n < RING_COUNT; n++ ) {

      // current ring
      if( n == currentRing ) {
        // value in currentRing is number of updates left before moving on to next ring
        ringValue[n]++;
        if( ringValue[n] == updatesEnergized ) {
          ringValue[n] = 192; // it will degrade from this value
          analogWrite( ringPin[n], ringValue[n] );
          currentRing++;
          if( currentRing == RING_COUNT ) {
            currentRing = 0;
          }
          ringValue[currentRing] = 0; // reset new current ring
          analogWrite( ringPin[currentRing], 255 ); // turn on new current ring
        }

      // other rings
      } else {
        // this is normal ring, so degrade its energy
        ringValue[n] = ringValue[n] - DECAY_AMOUNT;
        if( ringValue[n] < 0 ) ringValue[n] = 0;
        analogWrite( ringPin[n], ringValue[n] );
      }

    }

    // setup time trigger for next update
    nextUpdate = millis() + round( analogRead( POT_PIN ) / 1023.0 * MAX_MS_BETWEEN_UPDATES ); //MS_BETWEEN_UPDATES;
    // blink the heartbeat LED on and off every update
    digitalWrite( HEARTBEAT_PIN, LOW );

  } // end of if( millis() >= nextUpdate )

} // end of loop()
