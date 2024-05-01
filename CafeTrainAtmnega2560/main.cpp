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
#define MoveBackwardLED PK7 

#define ReversPin PA1	  //D23

#define RailSwitchL_1 PA2 //D24
#define RailSwitchR_1 PA3 //D25

#define RailSwitchL_2 PA4 //D26
#define RailSwitchR_2 PA5 //D27

#define RailSwitchL_3 PA6 //D228
#define RailSwitchR_3 PA7 //D229

#define RailSwitchL_4 PC5 //D31
#define RailSwitchR_4 PC6 //D32

#define EndWaySensor PC2 //D35

#define TableSensor PL0 // D49

#define StartPointSencor PL1 //D48

#define KitchenSensor PL7 //42

#define SwitchTable_1 PG1 //D40
#define SwitchTable_2 PG2 //D39
#define SwitchTable_3 PD7 //D38
#define SwitchTable_4 PC0 //D37
#define SwitchTable_5 PC1 //D36

#define Gear_1_Pin PD3
#define Gear_2_Pin PD2
#define Gear_3_Pin PD1
#define Gear_4_Pin PD0

bool IsTrainMoving = false;
bool IsTableChosen = false;
bool IsTrainArrivedToTable = false;

int ChoisenTableNumber = 0; 

void AdjustWay(int ChosenTable)
{
	ChoisenTableNumber = ChosenTable;
	
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
		PORTA |= (1 << RailSwitchR_1);
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

		break;
		
		default:
	
		break;
	}
}


void TurnOnButtonLED(int ChosenTable)
{
	// ��������� ��� ����������
	PORTK &= ~((1 << TableLED_1) | (1 << TableLED_2) | (1 << TableLED_3) | (1 << TableLED_4) | (1 << TableLED_5)); // | (1 << TableLED_6));

	// �������� ��������� ��������� ���� ��� ������� ����� �������� �� ����, ���� ���� �� ��� ��������� ������ �����������
	if (ChosenTable != 0)
	{
			PORTK |= (1 << (ChosenTable - 1));
	}
	
}

void MakeTrainMove() 
{	
	int stepDelay = 1000; 
	PORTD |= (1 << Gear_1_Pin);
	_delay_ms(stepDelay);
	PORTD |= (1 << Gear_2_Pin);
	_delay_ms(stepDelay);
	PORTD |= (1 << Gear_3_Pin);
	_delay_ms(stepDelay);
	PORTD |= (1 << Gear_4_Pin);	
	
	IsTrainMoving = true;
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
	IsTrainMoving = false;
	PORTD &=~ ((1 << Gear_1_Pin) | (1 << Gear_2_Pin) | (1 << Gear_3_Pin) | (1 << Gear_4_Pin));
	PORTA &=~ (1 << ReversPin);
	SetLEDMove(0,0);
}

void MoveTrain(bool direction)
{	

	if (direction == 1 && PINA & (1 << ReversPin))
	{
		StopTrain();
		PORTA &=~ (1 << ReversPin);
	}

	if (direction == 0 && !(PINA & (1 << ReversPin)))
	{
		StopTrain();
		PORTA |= (1 << ReversPin);
	}


	MakeTrainMove();
}

void SlowMode () 
{
	int stepDelay = 50; 
	PORTD &= ~ (1 << Gear_1_Pin);
	_delay_ms(stepDelay);
	PORTD &= ~ (1 << Gear_2_Pin);
	_delay_ms(stepDelay);
	PORTD |= (1 << Gear_3_Pin);
	_delay_ms(stepDelay);
	PORTD |= (1 << Gear_4_Pin);	
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
	DDRC |= (1 << RailSwitchL_4) | (1 << RailSwitchR_4); // | (1 << EndWaySensor); //| (1 << RailSwitchR_5); // | (1 << RailSwitchL_6) | (1 << RailSwitchR_6);
	DDRD |= (1 << Gear_1_Pin) | (1 << Gear_2_Pin) | (1 << Gear_3_Pin) | (1 << Gear_4_Pin);
	
	// ���������� ����� ����������� ��������� �����������
	DDRC &= ~(1 << EndWaySensor);
	
	// ����������� ����� ������� �������� �� ����� ���
	
	DDRL &= ~ (1 << TableSensor);


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
		
		// ���� ���� ������ ������ �������� ������ ������, ����������� ������� ��� �� ������ ���� � �� ��������� �� ��������� ������ ����
		if (!(PINF & (1 << MoveForwardButton)) && IsTableChosen == true && IsTrainArrivedToTable == false)
		{
			SetLEDMove(1,0);
			MoveTrain(1);	
		}
		
		
		if (!(PINF & (1 << MoveBackwardButton)) && IsTableChosen == true)
		{		
			SetLEDMove(0,1);
			MoveTrain(0);
			IsTrainArrivedToTable = false;
		}

		// ���� ������ ������ ��������� 
		if (!(PINF & (1 << StopButton)))
		{
			SetLEDMove(0,0);
			StopTrain();
		}
		
		// ���� ��������� ������ �� ������ ��������� ���� �� �����, ����������� ������� �� ������� �� ������ �������� 
		if (!(PINC & (1 << EndWaySensor)) && !(PINA & (1 << ReversPin)))
		{
			StopTrain();
			IsTrainArrivedToTable = true;
		}
		// ���� ����� ���� ������ � �������� �� ������ ����������
		if (!(PINL & (1 << TableSensor)) && !(PINA & (1 << ReversPin)))
		{
			SlowMode();
		}
		
		// ���� ����� ������ �� ���������� ��� �������� � ����� 
		if (!(PINL & (1 << KitchenSensor)) && PINA & (1 << ReversPin))
		{
			SlowMode();
			TurnOnButtonLED(0);
		}
		
		// ���� ��������� ������ �� ������ ���������� ��� �����
		if (!(PINL & (1 << KitchenSensor)) && PINA & (1 << ReversPin))
		{
			SlowMode();
		}
		// ���� ��������� ������ �� ������ ��������� ������
		if (!(PINL & (1 << StartPointSencor)) && PINA & (1 << ReversPin))
		{
			StopTrain();
			TurnOnButtonLED(0);
		}		
				
		_delay_ms(50);		
	}

	return 0;
	
}
