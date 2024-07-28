#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "config.h"
#include "buttonsAndSensors.h"
#include "LEDs.h"
#include "railSwitches.h"
#include "trainConfig.h"

int FullThrottlSpeed = 255;

int ShortCircuitWaitingTime = 100;

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
	PORTD &= ~((1 << Gear_1_Pin) | (1 << Gear_2_Pin) | (1 << Gear_3_Pin) | (1 << Gear_4_Pin));
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

void CheckItruption()
{
	
}
void SoftStart()
{
	if (IsTrainMoving)
	{
		return; // Защита от повторного нажатия, если поезд движется выйти из функции 
	}

	SetPMWControlMode();

	for (uint8_t i = 0; i <= FullThrottlSpeed; i++)
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

		if (i >= FullThrottlSpeed)
		{			
			PORTD &= ~(1 << Gear_2_Pin);
			break;  // Stop the function when OCR1A reaches 250
		}

		OCR1A = i;  // Set PWM value
		_delay_ms(SMOOTH_FADE_DOWN_DELAY);  // Delay for smooth ramp-up
	}

	OCR1A = 0;
}


void MoveTrain(bool direction)
{
	if (IsTrainMoving)
	{
		return; // Exit the loop if the train stops moving
	}
	
	 if (IsTrainOnTheTable && direction == 1) // Если поезд находится в на столе запрет на движение вперед
	 {
		 return;
	 }
	 
	 if (IsTrainOnTheKitchen && direction == 0) // Если поезд на на кухне запрет движения назад
	 {
		 return; 
	 }	 
		
    if (direction == 1 && PINA & (1 << ReversPin))
    {
		StopTrain();
        PORTA &=~ (1 << ReversPin);
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
            _delay_ms(50); // Дебаунсинг задержка
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
    DDRK = 0xFF;
    PORTK = 0x00;
    DDRA |= (1 << RailSwitchL_1) | (1 << RailSwitchR_1) | (1 << RailSwitchL_2) | (1 << RailSwitchR_2) | (1 << RailSwitchL_3) | (1 << RailSwitchR_3) | (1 << ReversPin);
    DDRC |= (1 << RailSwitchL_4) | (1 << RailSwitchR_4) | (1 << SwitchTable_4) | (1 << SwitchTable_5);
    DDRD |= (1 << Gear_1_Pin) | (1 << Gear_2_Pin) | (1 << Gear_3_Pin) | (1 << Gear_4_Pin) | (1 << SwitchTable_3);
    DDRG |= (1 << SwitchTable_1) | (1 << SwitchTable_2);
    DDRC &= ~(1 << EndWaySensor);
    DDRL &= ~((1 << TableSensor) | (1 << KitchenSensor) | (1 << ShortSircuitPin));
	
    // Настройка пина для ШИМ
    DDRB |= (1 << PB5);  // Устанавливаем PB5 (соответствует цифровому пину 11) как выход

    // Настройка таймера для ШИМ
    // Используем Таймер 1: 16-битный таймер, частота 31.25 kHz
    TCCR1A = (1 << COM1A1) | (1 << WGM10);  // Fast PWM mode, 8-bit
    TCCR1B = (1 << WGM12) | (1 << CS10);    // Без предделителя, Fast PWM	

    while (1)
    {

        if (IsTrainMoving == false && IsTrainOnTheTable == false)
        {
            if (isButtonPressed(TableButton_1, &previousButtonState[0]))
            {
                TurnOnButtonLED(1);
                AdjustWay(1);
            }

            if (isButtonPressed(TableButton_2, &previousButtonState[1]))
            {
                TurnOnButtonLED(2);
                AdjustWay(2);
            }

            if (isButtonPressed(TableButton_3, &previousButtonState[2]))
            {
                TurnOnButtonLED(3);
                AdjustWay(3);
            }

            if (isButtonPressed(TableButton_4, &previousButtonState[3]))
            {
                TurnOnButtonLED(4);
                AdjustWay(4);
            }

            if (isButtonPressed(TableButton_5, &previousButtonState[4]))
            {
                TurnOnButtonLED(5);
                AdjustWay(5);
            }
        }
        
        if (!(PINF & (1 << MoveForwardButton)) && IsTableChosen == true)
        {
            SetLEDMove(1,0);
			MoveTrain(1);
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
		
        // Проверка состояния пина ShortSircuitPin
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
