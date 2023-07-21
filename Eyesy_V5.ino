/** 
 * This example demonstrates the use of MIDI Control Change potentiometers that
 * can be used for changing effect parameters, volumes, pan and balance
 * controls, etc. It can control almost any knob in your DAW software. 
 * This example shows how to define multiple potentiometers using an array.
 *
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, Teensy 3.x
 * 
 * Connections
 * -----------
 * 
 * - A0: wiper of the first potentiometer
 * - A1: wiper of the second potentiometer
 * - ...
 * - A5: wiper of the sixth potentiometer
 * 
 * Connect the left terminal of the potentiometers to ground, and the right one
 * to V<sub>CC</sub>.
 * 
 * Behavior
 * --------
 * 
 * - When you turn a potentiometer, you should receive MIDI Control Change
 *   events, with a value between 0 and 127.
 * - The analog inputs are filtered, so there shouldn't be any noise on the 
 *   position. If there is, check your wiring, and make sure that the resistance
 *   of the potentiometers isn't too high (10 kΩ is ideal).
 * 
 * Mapping
 * -------
 * 
 * Select the Arduino as a custom MIDI controller in your DAW, and use the 
 * MIDI learn option to assign the potentiometers to a function.
 * 
 * Written by PieterP, 2019-08-13  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library
BEGIN_CS_NAMESPACE
 
/*
 * @brief   A class for momentary buttons and switches that send MIDI events.
 *
 * The button is debounced, and the internal pull-up resistor is enabled.
 *
 * @see     NoteButton
 * @see     Button
 */
class MyCCButton : public MIDIOutputElement {
 public:
  /*
   * @brief   Create a new MyCCButton object on the given pin, with the 
   *          given address and velocity.
   * 
   * @param   pin
   *          The digital input pin to read from.  
   *          The internal pull-up resistor will be enabled.
   * @param   address
   *          The MIDI address to send to.
   * @param   velocity
   *          The MIDI note velocity [0, 127].
   */
  MyCCButton(pin_t pin, MIDIAddress address, uint8_t velocity)
    : button(pin), address(address), velocity(velocity) {}
 
 public:
  // Initialize: enable the pull-up resistor for the button
  // This method is called once by `Control_Surface.begin()`.
  void begin() final override { button.begin(); }
 
  // Update: read the button and send MIDI messages when appropriate.
  // This method is called continuously by `Control_Surface.loop()`.
  void update() final override {
    AH::Button::State state = button.update();        // Read the button
    if (state == AH::Button::Falling) {               // if pressed
      Control_Surface.sendCC(address, velocity);  //   → note on
    } 
  }
 
 private:
  AH::Button button;
  const MIDIAddress address;
  uint8_t velocity;
};
 
END_CS_NAMESPACE
 
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //


// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

// Instantiate an array of CCPotentiometer objects
CCPotentiometer potentiometers[] {
  {A0, 25},
  {A1, 24},
  {A2, 23}, //Element Color
  {A3, 22}, //Background Color
  {A10, 21}, 
};

MyCCButton buttons[] {
  {2, {29, CHANNEL_1}, 1}, 
  {3, {29, CHANNEL_1}, 127}, 
  {7, {26, CHANNEL_1},1}, 
  {9, {26, CHANNEL_1},127},                  
                 };

CCButton button[] {
  {4, 27, 1}, // Save/Delete
  {5, 31, 127}, // Display Info
  {6, 32, 127}, // Send Trigger
  {8, 28, 127}, //Persist
                  };
  
void setup() {
 // Use the Mackie Control protocol for sending relative MIDI CC messages.
  RelativeCCSender::setMode(MACKIE_CONTROL_RELATIVE);

  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface
}