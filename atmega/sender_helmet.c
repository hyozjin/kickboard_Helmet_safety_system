#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

// HC-SR04 초음파 센서 핀 설정 (TRIG=PE6, ECHO=PE7)
#define TRIG PE6
#define ECHO PE7
#define BAUD 9600UL

// ---- UART0/1 공통 설정: U2X=1, 8N1 ----
static void usart0_init(uint32_t baud){
    UCSR0A = (1<<U2X0);
    uint16_t ubrr = (uint16_t)((F_CPU/(8UL*baud))-1UL);
    UBRR0H = (ubrr>>8); 
    UBRR0L = (uint8_t)ubrr;
    UCSR0B = (1<<TXEN0);
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
}

static void usart1_init(uint32_t baud){
    UCSR1A = (1<<U2X1);
    uint16_t ubrr = (uint16_t)((F_CPU/(8UL*baud))-1UL);
    UBRR1H = (ubrr>>8); 
    UBRR1L = (uint8_t)ubrr;
    UCSR1B = (1<<TXEN1);
    UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);
}

static inline void bt_send_char(char c){
    // UART0 TX 전송 완료 후 데이터 기록
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = c;
    
    // UART1 TX 전송 완료 후 데이터 기록
    while(!(UCSR1A & (1<<UDRE1)));
    UDR1 = c;
}

// ---- 초음파 거리 측정 (Timer1, Prescaler=8) ----
static bool within_5cm(void){
    // TRIG 신호 발생 (10us)
    PORTE &= ~(1<<TRIG); 
    _delay_us(2);
    PORTE |= (1<<TRIG); 
    _delay_us(10);
    PORTE &= ~(1<<TRIG);

    // Timer1 시작 (0.5us @ 16MHz / 8)
    TCCR1A = 0;
    TCCR1B = (1<<CS11);   
    TCNT1  = 0;

    // ECHO 상승 에지 대기 (30ms 타임아웃 처리)
    uint32_t to_ticks = (uint32_t)((F_CPU/8UL) * 30UL / 1000UL);
    while(!(PINE & (1<<ECHO))){
        if(TCNT1 > to_ticks){ 
            TCCR1B = 0; 
            return false; 
        }
    }
    
    // ECHO HIGH 구간(펄스 폭) 측정
    TCNT1 = 0;
    while(PINE & (1<<ECHO)){
        if(TCNT1 > to_ticks){ 
            TCCR1B = 0; 
            return false; 
        }
    }
    uint16_t ticks = TCNT1;
    TCCR1B = 0; // 타이머 정지

    // 임계값(5cm) 계산: echo_us ≈ 58 * cm => 5cm 기준 290us
    // ticks = echo_us * (F_CPU/8/1e6)
    uint32_t thr = (uint32_t)(290UL * (F_CPU/8UL) / 1000000UL);
    return (ticks <= thr);
}

int main(void){
    // 포트 E 방향 설정 (TRIG: 출력, ECHO: 입력)
    DDRE |= (1<<TRIG);
    DDRE &= ~(1<<ECHO);
    PORTE &= ~(1<<TRIG); // TRIG 초기화

    // 블루투스 및 시리얼 통신 초기화
    usart0_init(BAUD);
    usart1_init(BAUD);

    while(1){
        // 5cm 이내 감지 시 '1', 이탈 시 '0' 전송
        bt_send_char(within_5cm() ? '1' : '0');
        _delay_ms(100);
    }
}
