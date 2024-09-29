#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define TableButton_1 PF0
#define TableButton_2 PF1
#define TableButton_3 PF2
#define TableButton_4 PF3
#define TableButton_5 PF4
#define StopButton PF5

#define MoveForwardButton PF6
#define MoveBackwardButton PF7

#define TableLED_1 PK0
#define TableLED_2 PK1
#define TableLED_3 PK2
#define TableLED_4 PK3
#define TableLED_5 PK4

#define MoveForwarLED PK6
#define MoveBackwardLED PK7

#define ReversPin PA1

#define RailSwitchL_1 PA2
#define RailSwitchR_1 PA3

#define RailSwitchL_2 PA4
#define RailSwitchR_2 PA5

#define RailSwitchL_3 PA6
#define RailSwitchR_3 PA7

#define RailSwitchL_4 PC5
#define RailSwitchR_4 PC6

#define EndWaySensor PC2

#define TableSensor PL0
#define StartPointSensor PL1
#define ShortSircuitPin PL6
#define KitchenSensor PL7

#define SwitchTable_1 PG1
#define SwitchTable_2 PG2
#define SwitchTable_3 PD7
#define SwitchTable_4 PC0
#define SwitchTable_5 PC1

#define Gear_1_Pin PD3
#define Gear_2_Pin PD2

#define PWM_Pin PB5
#define SemaforTable_Pin_1 PB2
#define SemaforTable_Pin_2 PB1
#define SemaforTable_Pin_3 PB0
#define SemaforTable_Pin_4 PD0
#define SemaforTable_Pin_5 PD1

#define SMOOTH_FADE_DOWN_DELAY 200 
#define SMOOTH_FADE_STOP_DELAY int 10000 
#define SLOW_MODE_SPEED 80
#define STOP_PWM_LEVEL 40

int FullThrottlSpeed = 255;

int ShortCircuitWaitingTime = 100;

int ChoisenTableNumber; 

bool IsTrainOnTheTable = false;
bool IsTrainOnTheKitchen = false;

bool IsTrainMoving = false;
bool IsTableChosen = false;
bool IsTrainArrivedToTable = false;

bool previousButtonState[8] = {false};

unsigned long shortCircuitStartTime = 0;
unsigned long shortCircuitDelayTime = 0;
bool shortCircuitDetected = false;
bool shortCircuitDelayCompleted = false;

void SwithOffAllTables()
{
    PORTG &= ~((1 << SwitchTable_1) | (1 << SwitchTable_2));
    PORTD &= ~(1 << SwitchTable_3);
    PORTC &= ~((1 << SwitchTable_4) | (1 << SwitchTable_5));
	PORTB &= ~((1 << SemaforTable_Pin_1)|(1 << SemaforTable_Pin_2)|(1 << SemaforTable_Pin_3));
	PORTD &= ~((1 << SemaforTable_Pin_4)|(1 << SemaforTable_Pin_5));
}

void AdjustWay(int ChosenTable)
{
	SwithOffAllTables();
	
	int DelayBWRailSwitch = 1000;
	
	IsTableChosen = true;
	
	switch (ChosenTable)
	{
		case 1:
		PORTA |= (1 << RailSwitchL_1);
		_delay_ms(DelayBWRailSwitch);
		PORTA &= ~ (1 << RailSwitchL_1);
		
		PORTG |= (1 << SwitchTable_1);
				
		break;
		
		case 2:
		PORTA |= (1 << RailSwitchR_1);
		_delay_ms(DelayBWRailSwitch);
		PORTA &= ~ (1 << RailSwitchR_1);
		
		PORTA |= (1 << RailSwitchL_2);
		_delay_ms(DelayBWRailSwitch);
		PORTA &= ~ (1 << RailSwitchL_2);
		
		PORTG |= (1 << SwitchTable_2);
		
		//PORTB |= (1 << SemaforTable_Pin_2);
		break;
		
		case 3:
		PORTA |= (1 << RailSwitchR_1);
		_delay_ms(DelayBWRailSwitch);
		PORTA &= ~ (1 << RailSwitchR_1);
		
		PORTA |= (1 << RailSwitchR_2);
		_delay_ms(DelayBWRailSwitch);
		PORTA &= ~ (1 << RailSwitchR_2);
		
		PORTA |= (1 << RailSwitchL_3);
		_delay_ms(DelayBWRailSwitch);
		PORTA &= ~ (1 << RailSwitchL_3);
		
		PORTD |= (1 << SwitchTable_3);
		
		//PORTB |= (1 << SemaforTable_Pin_3);
		break;
		
		case 4:
		PORTA |= (1 << RailSwitchR_1);
		_delay_ms(DelayBWRailSwitch);
		PORTA &= ~ (1 << RailSwitchR_1);
		
		PORTA |= (1 << RailSwitchR_2);
		_delay_ms(DelayBWRailSwitch);
		PORTA &= ~ (1 << RailSwitchR_2);
		
		PORTA |= (1 << RailSwitchR_3);
		_delay_ms(DelayBWRailSwitch);
		PORTA &= ~ (1 << RailSwitchR_3);
		
		PORTC |= (1 << RailSwitchL_4);
		_delay_ms(DelayBWRailSwitch);
		PORTC &= ~ (1 << RailSwitchL_4);
		
		PORTC |= (1 << SwitchTable_4);
		
		//PORTD |= (1 << SwitchTable_4);
		break;
		
		case 5:
		PORTA |= (1 << RailSwitchR_1);
		_delay_ms(DelayBWRailSwitch);
		PORTA &= ~ (1 << RailSwitchR_1);
		
		PORTA |= (1 << RailSwitchR_2);
		_delay_ms(DelayBWRailSwitch);
		PORTA &= ~ (1 << RailSwitchR_2);

		PORTA |= (1 << RailSwitchR_3);
		_delay_ms(DelayBWRailSwitch);
		PORTA &= ~ (1 << RailSwitchR_3);

		PORTC |= (1 << RailSwitchR_4);
		_delay_ms(DelayBWRailSwitch);
		PORTC &= ~ (1 << RailSwitchR_4);
		
		PORTC |= (1 << SwitchTable_5);
		
		//PORTD |= (1 << SwitchTable_5);
		
		break;
		
		default:
		break;
	}
}

void TurnOnButtonLED(int ChosenTable)
{
    PORTK &= ~((1 << TableLED_1) | (1 << TableLED_2) | (1 << TableLED_3) | (1 << TableLED_4) | (1 << TableLED_5));

    if (ChosenTable != 0)
    {
        PORTK |= (1 << (ChosenTable - 1));
    }
}

void SetLEDMove (bool forwardLED, bool backwardLED)
{
    PORTK &= ~((1 << MoveBackwardLED) | (1 << MoveForwarLED));
    
    if (forwardLED == 1)
    {
        PORTK|= (1 << MoveForwarLED);
    }
    if (backwardLED == 1)
    {
        PORTK|= (1 << MoveBackwardLED);
    }
}

void StopTrain()
{	
	OCR1A = 0;
	IsTrainMoving = false;
	PORTD &= ~((1 << Gear_1_Pin) | (1 << Gear_2_Pin) | (1 << SemaforTable_Pin_4) | (1 << SemaforTable_Pin_5));  
	PORTB &= ~((1 << SemaforTable_Pin_1) | (1 << SemaforTable_Pin_2) | (1 << SemaforTable_Pin_3));
	PORTA &= ~(1 << ReversPin);
	SetLEDMove(false, false);
	shortCircuitDelayTime = 0;
}


void SetTrainSpeed(int Speed)
{
	OCR1A = Speed;
}


void SetPMWControlMode()
{
	PORTD |= (1 << Gear_1_Pin);
	PORTD |= (1 << Gear_2_Pin);	
}

void SlowMode(int Stage)
{
	if (!IsTrainMoving)
	{
		return;
	}

	int borderSpeed = 95;
	int slowSpeed =145;
		
	SetPMWControlMode();

	if (Stage == 1)
	{
		
		for (int i = slowSpeed; i>= borderSpeed; i--)
		{
			SetTrainSpeed(i);
			_delay_ms(100);
			
			if (i == borderSpeed)
				{
					break;
				}							
		}
	}
	
	if (Stage == 2)
	{
			for (int i = borderSpeed; i>= 80; i--)
			{
				SetTrainSpeed(i);
				_delay_ms(1000);				
			}			

			IsTrainMoving = false;
			StopTrain();	
	}
}


void SoftStart()
{
	if (IsTrainMoving)
	{
		return; // ?????? ?? ?????????? ???????, ???? ????? ???????? ????? ?? ??????? 
	}

	SetPMWControlMode();
	
	for (uint8_t i = 0; i <= 255; i++)
	{
		if (!(PINC & (1 << EndWaySensor)) && !(PINA & (1 << ReversPin)))
		{
			
			break;
		}
			
        if (!(PINL & (1 << StartPointSensor)) && PINA & (1 << ReversPin))
        {
			break;
        }

        if (!(PINF & (1 << StopButton)))
        {
            StopTrain();
			OCR1A = 0;
			break;
        }

		OCR1A = i;  // Set PWM value
		_delay_ms(SMOOTH_FADE_DOWN_DELAY);  // Delay for smooth ramp-up

		if (i >= 250)
		{			
			PORTD &= ~(1 << Gear_2_Pin);
			break;  // Stop the function when OCR1A reaches 250
		}
	}

	OCR1A = 0;
}

void MoveTrain(bool direction)
{
	
	if (IsTrainMoving)
	{
		return; // Exit the loop if the train stops moving
	}
	
	 if (IsTrainOnTheTable && direction == 1) // ???? ????? ????????? ? ?? ????? ?????? ?? ???????? ??????
	 {
		 return;
	 }
	 
	 if (IsTrainOnTheKitchen && direction == 0) // ???? ????? ?? ?? ????? ?????? ???????? ?????
	 {
		 return; 
	 }	 
	
	switch (ChoisenTableNumber)
	{
		case 1:
		PORTB |= (1 << SemaforTable_Pin_1);
		break;
		case 2:
		PORTB |= (1 << SemaforTable_Pin_2);
		break;
		case 3:
		PORTB |= (1 << SemaforTable_Pin_3);
		break;
		case 4:
		PORTD |= (1 << SemaforTable_Pin_4);
		break;
		case 5:
		PORTD |= (1 << SemaforTable_Pin_5);
		break;
		default:
		break;
	}
	
    if (direction == 1 && PINA & (1 << ReversPin))
    {
		StopTrain();
		PORTA &=~ (1 << ReversPin);
		//PORTB |= (1 << SemaforTable_Pin_1);
		SetLEDMove(1,0);
    }

    if (direction == 0 && !(PINA & (1 << ReversPin)))
    {
        StopTrain();
		SetLEDMove(0,1);
        PORTA |= (1 << ReversPin);

    }

	SoftStart();
	
	IsTrainMoving = true;
	
	
}


bool isButtonPressed(uint8_t buttonPin, bool *previousState)
{
    if (!(PINF & (1 << buttonPin)))
    {
        if (!(*previousState))
        {
            *previousState = true;
            _delay_ms(50); // ?????????? ????????
            return true;
        }
    }
    else
    {
        *previousState = false;
    }
    return false;
}

int main(void)
{
    DDRF = 0x00;
    DDRB &= ~(1 << StopButton);
	DDRB |= (1 << PWM_Pin);  // ????????????? PB5 (????????????? ????????? ???? 11) ??? ?????
	DDRB |= (1 << SemaforTable_Pin_1) | (1 << SemaforTable_Pin_2) | (1 << SemaforTable_Pin_3);
	
    DDRK = 0xFF;
    PORTK = 0x00;
    DDRA |= (1 << RailSwitchL_1) | (1 << RailSwitchR_1) | (1 << RailSwitchL_2) | (1 << RailSwitchR_2) | (1 << RailSwitchL_3) | (1 << RailSwitchR_3) | (1 << ReversPin);
    DDRC |= (1 << RailSwitchL_4) | (1 << RailSwitchR_4) | (1 << SwitchTable_4) | (1 << SwitchTable_5);
    DDRD |= (1 << Gear_1_Pin) | (1 << Gear_2_Pin) | (1 << SemaforTable_Pin_4) | (1 << SemaforTable_Pin_5) | (1 << SwitchTable_3);
    DDRG |= (1 << SwitchTable_1) | (1 << SwitchTable_2);
    DDRC &= ~(1 << EndWaySensor);
    DDRL &= ~((1 << TableSensor) | (1 << KitchenSensor) | (1 << ShortSircuitPin));
	
    // ????????? ??????? ??? ???
    // ?????????? ?????? 1: 16-?????? ??????, ??????? 31.25 kHz
    TCCR1A = (1 << COM1A1) | (1 << WGM10);  // Fast PWM mode, 8-bit
    TCCR1B = (1 << WGM12) | (1 << CS10);    // ??? ????????????, Fast PWM	

    while (1)
    {

        if (IsTrainMoving == false && IsTrainOnTheTable == false)
        {
            if (isButtonPressed(TableButton_1, &previousButtonState[0]))
            {
                ChoisenTableNumber = 1;
				TurnOnButtonLED(1);
                AdjustWay(1);
            }

            if (isButtonPressed(TableButton_2, &previousButtonState[1]))
            {
                ChoisenTableNumber = 2;
				TurnOnButtonLED(2);
                AdjustWay(2);
            }

            if (isButtonPressed(TableButton_3, &previousButtonState[2]))
            {
                ChoisenTableNumber = 3;
				TurnOnButtonLED(3);
                AdjustWay(3);
            }

            if (isButtonPressed(TableButton_4, &previousButtonState[3]))
            {
                ChoisenTableNumber = 4;
				TurnOnButtonLED(4);
                AdjustWay(4);
            }

            if (isButtonPressed(TableButton_5, &previousButtonState[4]))
            {
                ChoisenTableNumber = 5;
				TurnOnButtonLED(5);
				AdjustWay(5);
            }
        }
        
        if (!(PINF & (1 << MoveForwardButton)) && IsTableChosen == true)
        {
			//AdjustWay(ChoisenTableNumber); // Вызов функции для установки пути
			SetLEDMove(1, 0); // Установка светодиодов направления
			MoveTrain(1); // Запуск движения вперед
			IsTrainOnTheKitchen = false;
		
        }
        
        if (!(PINF & (1 << MoveBackwardButton)) && IsTableChosen == true)
        {

			MoveTrain(0);
			IsTrainOnTheTable = false;
        }

        if (!(PINF & (1 << StopButton)))
        {
            StopTrain();
        }
        
        if (!(PINC & (1 << EndWaySensor)) && !(PINA & (1 << ReversPin)))
        {
			SlowMode(2);
			IsTrainOnTheTable = true;
			StopTrain();
        }
        
        if (!(PINL & (1 << TableSensor)) && !(PINA & (1 << ReversPin)))
        {
            SlowMode(1);
        }
        
        if (!(PINL & (1 << KitchenSensor)) && PINA & (1 << ReversPin))
        {
            SlowMode(1);
        }
        
        if (!(PINL & (1 << StartPointSensor)) && PINA & (1 << ReversPin))
        {
			SlowMode(2);
		    StopTrain();
			TurnOnButtonLED(0);
			IsTableChosen = false;
			IsTrainOnTheKitchen = true;
        }
		
        // ???????? ????????? ???? ShortSircuitPin
		/*if (!(PINL & (1 << ShortSircuitPin))) 
		{		
			if (IsTrainMoving && ++shortCircuitDelayTime >= 1000) 
			{
				StopTrain();
			}
		}
		else 
		{
			shortCircuitDelayTime = 0; 		
		}*/

        _delay_ms(50);
	}

    return 0;
}