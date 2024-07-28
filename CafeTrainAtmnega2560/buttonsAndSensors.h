
#ifndef BUTTONSANDSENSORS_H_
#define BUTTONSANDSENSORS_H_

#define TableButton_1 PF0
#define TableButton_2 PF1
#define TableButton_3 PF2
#define TableButton_4 PF3
#define TableButton_5 PF4

#define StopButton PF5

#define MoveForwardButton PF6
#define MoveBackwardButton PF7

#define EndWaySensor PC2

#define TableSensor PL0

#define StartPointSensor PL1

#define ShortSircuitPin PL6

#define KitchenSensor PL7

extern bool previousButtonState[8];

#endif /* BUTTONSANDSENSORS_H_ */