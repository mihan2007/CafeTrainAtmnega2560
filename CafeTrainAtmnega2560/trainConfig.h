
#ifndef TRAINCONFIG_H_
#define TRAINCONFIG_H_

#define ReversPin PA1

#define Gear_1_Pin PD3
#define Gear_2_Pin PD2
#define Gear_3_Pin PD1
#define Gear_4_Pin PD0

extern int FullThrottlSpeed;

extern bool IsTrainOnTheTable;
extern bool IsTrainOnTheKitchen;

extern bool IsTrainMoving;
extern bool IsTableChosen;
extern bool IsTrainArrivedToTable;

extern int ShortCircuitWaitingTime;
extern unsigned long shortCircuitStartTime;
extern unsigned long shortCircuitDelayTime;
extern bool shortCircuitDetected;
extern bool shortCircuitDelayCompleted;


#endif /* TRAINCONFIG_H_ */