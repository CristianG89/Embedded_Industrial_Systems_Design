# Embedded_Industrial_Systems_Design
Ping-pong system that pushes a ball with a solenoid, being controlled by a joystick.
The system is composed by two AVR microprocessors (AT162 and a Arduino) sharing data with CAN commun. protocol:
- AT162 communicates with an OLED board, external RAM and ADC ICs by using the parallel port.
- Arduino communicates with a motor by I2C, moves the handler with a PID closed loop and activates a solenoid.

Implementation done in C. The IDE used was a text editor under Linux OS.

Lecture from master "Electronic Systems Design" in NTNU Trondheim.
