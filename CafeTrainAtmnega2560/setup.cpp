#include <avr/io.h>
#include "setup.h"

void setup() {
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
}
