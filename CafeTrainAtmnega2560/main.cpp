#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h> // �������� ������������ ���� ��� ������������� ���� ������ bool

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
#define MoveBackwardLED PK7 // ���������� �������� ���������� �� MoveBackwardLED

#define ReversPin PA1

#define RailSwitchL_1 PA2
#define RailSwitchR_1 PA3

#define RailSwitchL_2 PA4
#define RailSwitchR_2 PA5

#define RailSwitchL_3 PA6
#define RailSwitchR_3 PA7

#define RailSwitchL_4 PC5
#define RailSwitchR_4 PC6

#define RailSwitchL_5 PC4
#define RailSwitchR_5 PC3

#define TableSensor_1 PL0 // D49
#define TableSensor_2 PL1 // D48
#define TableSensor_3 PL2 // D47
#define TableSensor_4 PL3 // D46
#define TableSensor_5 PL4 // D45

#define Gear_1_Pin PD3
#define Gear_2_Pin PD2
#define Gear_3_Pin PD1
#define Gear_4_Pin PD0

bool IsTrainMoving = false;
bool IsTableChosen = false;

void AdjustWay(int ChosenTable)
{
	int DelayBWRailSwitch = 1000;
	
	IsTableChosen = true;
	
	switch (ChosenTable) 
	{
		case 1:

		PORTA |= (1 << RailSwitchL_1);
		_delay_ms(DelayBWRailSwitch);
		PORTA &= ~ (1 << RailSwitchL_1);
		

		break;
		
		case 2:

		_delay_ms(DelayBWRailSwitch);
		PORTA &= ~ (1 << RailSwitchR_1);
	
		PORTA |= (1 << RailSwitchL_2);
		_delay_ms(DelayBWRailSwitch);
		PORTA &= ~ (1 << RailSwitchL_2);
		
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
	

		break;
		
		case 5:

		PORTA |= (1 << RailSwitchR_1);
		_delay_ms(DelayBWRailSwitch);
		PORTA &= ~ (1 << RailSwitchR_1);
	
		PORTA | (1 << RailSwitchR_2);
		_delay_ms(DelayBWRailSwitch);
		PORTA &= ~ (1 << RailSwitchR_2);
	
		PORTA |= (1 << RailSwitchR_3);
		_delay_ms(DelayBWRailSwitch);
		PORTA &= ~ (1 << RailSwitchR_3);
	
		PORTC |= (1 << RailSwitchR_4);
		_delay_ms(DelayBWRailSwitch);
		PORTC &= ~ (1 << RailSwitchR_4);
	
		PORTC |=  (1 << RailSwitchL_5);
		_delay_ms(DelayBWRailSwitch);
		PORTC &= ~ (1 << RailSwitchL_5);

		break;
		
		default:
	
		break;
	}
}


void TurnOnButtonLED(int ChosenTable)
{
	// ��������� ��� ����������
	PORTK &= ~((1 << TableLED_1) | (1 << TableLED_2) | (1 << TableLED_3) | (1 << TableLED_4) | (1 << TableLED_5)); // | (1 << TableLED_6));
	// �������� ��������� ���������
	PORTK |= (1 << (ChosenTable - 1));
}


void StopTrain()
{
	PORTD &=~ ((1 << Gear_1_Pin) | (1 << Gear_2_Pin) | (1 << Gear_3_Pin) | (1 << Gear_4_Pin));	
	PORTA &=~ (1 << ReversPin);
}



void MakeTrainMove() 
{	
	PORTD |= (1 << Gear_1_Pin);
	_delay_ms(500);
	PORTD |= (1 << Gear_2_Pin);
	_delay_ms(500);
	PORTD |= (1 << Gear_3_Pin);
	_delay_ms(500);
	PORTD |= (1 << Gear_4_Pin);	
}

void MoveTrain(bool direction)
{	

	if (direction == 1 && PINA & (1 << ReversPin) )
	{
		StopTrain();
		PORTA &=~ (1 << ReversPin);

	}
	
	if (direction == 0 && !(PINA & (1 << ReversPin)))
	{	
		StopTrain();	
		PORTA |= (1<< ReversPin);
	}

	MakeTrainMove();
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
	

int main(void)
{
	// PF ��������� ������ ������ ������������ ������ ��� ����
	DDRF = 0x00; // ��������� ���� ����� ����� PF � 0 (�����)
	//��������� PB � �������� ������ ������
	DDRB &= ~(1 << StopButton); // | (1 << MoveTrainButton));

	// ��������� ������ PK ��� ������� ������ ��������� ���������� �����
	DDRK = 0xFF; // ��������� ���� ����� ����� PK � 1 (������)
	PORTK = 0x00; // ��������� ��������� ������� ����� PK (���������)

	// ��������� ������ ������� ��� �������
	DDRA |= (1 << RailSwitchL_1) | (1 << RailSwitchR_1) | (1 << RailSwitchL_2) | (1 << RailSwitchR_2) | (1 << RailSwitchL_3) | (1 << RailSwitchR_3) | (1 << ReversPin);
	DDRC |= (1 << RailSwitchL_4) | (1 << RailSwitchR_4) | (1 << RailSwitchL_5) | (1 << RailSwitchR_5); // | (1 << RailSwitchL_6) | (1 << RailSwitchR_6);
	DDRD |= (1 << Gear_1_Pin) | (1 << Gear_2_Pin) | (1 << Gear_3_Pin) | (1 << Gear_4_Pin);
	DDRL &= ~ ((1 << TableSensor_1) | (1 << TableSensor_2) | (1<< TableSensor_3) | (1 << TableSensor_4) | (1 << TableSensor_5));


	while (1)
	{
		if (IsTrainMoving == false)
		{
			// ��������� ������� ������ � ��������� ��������������� ��������
			if (!(PINF & (1 << TableButton_1)))
			{
				TurnOnButtonLED(1); // ��������� ���������� ��� �����
				AdjustWay(1);       // �������� ��������� �� ���� ��� ����� 1
			}

			if (!(PINF & (1 << TableButton_2)))
			{
				TurnOnButtonLED(2); // ��������� ���������� ��� �����
				AdjustWay(2);       // �������� ��������� �� ���� ��� ����� 1
			}

			if (!(PINF & (1 << TableButton_3)))
			{
				TurnOnButtonLED(3); // ��������� ���������� ��� ����������
				AdjustWay(3);       // �������� ��������� �� ���� ��� ����� 1
			}

			if (!(PINF & (1 << TableButton_4)))
			{
				TurnOnButtonLED(4); // ��������� ���������� ��� �����
				AdjustWay(4);       // �������� ��������� �� ���� ��� ����� 1
			}

			if (!(PINF & (1 << TableButton_5)))
			{
				TurnOnButtonLED(5); // ��������� ���������� ��� �����
				AdjustWay(5);       // �������� ��������� �� ���� ��� ����� 1
			}	
		}

		if (!(PINF & (1 << MoveForwardButton)) && IsTableChosen == true)
		{
			if (IsTrainMoving == false)
			{
				SetLEDMove(1,0);
				MoveTrain(1);
				IsTrainMoving = true;
			}
		}

		if (!(PINF & (1 << MoveBackwardButton)) && IsTableChosen == true)
		{
			if (IsTrainMoving == false)
			{		
			SetLEDMove(0,1);
			MoveTrain(0);
			IsTrainMoving = true;
			}
		}


		if (!(PINF & (1 << StopButton)))
		{
			SetLEDMove(0,0);
			StopTrain();
			IsTrainMoving = false; // ������������� �����
		}
		
		_delay_ms(50);
		
	}

	return 0;
}
