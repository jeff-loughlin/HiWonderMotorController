This is an Arduino library for HiWonder motor controller.  This is the unit that comes with their tank-tread kit, and a few other kits they sell.  Their documentation is horrendous, so I've taken some of their sample code and placed it here for reference.  I didn't write this code, I just took their sample code, cleaned it up a bit and made an Arduino library out of it, and added a couple of very basic helper functions to make it easy to set the motor speeds and read the counters:

SetMotorSpeed(int left, int right)
  Set the speeds for the left and right motors -100 to 100 (0 to stop)

GetEncoderCounts(int left, int right)
  Read the encoder counts for the left and right motors

ResetEncoders()
  Reset both encoder counters to 0


Just create a directory called HiWonderMotorController in the Arduino libraries directory (Dcouments/Arduino/libraries on Windows), and copy the HoWonderMotorController.cpp and HiWonderMotorController.h files into that directory.  I'm assuming their code is public domain, since I didn't see anything indicating otherwise in their nearly useless documentation.  I don't anticipate them having a problem with me repackaging it this way, since it just makes their product easier to use.  No guarantees from me or them as to the usefulness or uselessness of this code, of course.  It works for me.


Wiring the controller is pretty straightforward.  Use the 4-pin connector on the controller board and wire GND to GND on the Arduino, SDA and SCL to SDA and SCL on the arduino (leave the 5V pin open).  Then power the controller and the motors from a separate source (6-12V) through the + and - pins on the controller board.  This will power both the controller and the motors.  Then power the Arduino as normal, either through the usb port, the external power connector, or a direct 5V power source.  
