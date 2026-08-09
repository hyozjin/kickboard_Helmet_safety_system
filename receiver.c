#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

#define BAUD 9600UL

// ====== 하드웨어 핀 매핑 ======
// LED (PA0)
#define LED_PORT   PORTA
#define LED_DDR    DDRA
#define LED_PIN_NO 0

// 부저 (PB4) - 패시브 부저 사용
#define BUZZ_PORT  PORTB
#define BUZZ_DDR   DDRB
#define BUZZ_PIN_NO 4

// 스위치 (PA1: 택트, PD4: 슬라이드)
#define TACT_PIN_REG PINA
#define TACT_DDR     DDRA
#define TACT_PIN_NO  1

#define SLIDE_PIN_REG PIND
#define SLIDE_DDR     DDRD
#define SLIDE_PIN_NO  4

// 모터 제어 L298N (PD0, PD1, PD5)
#define MTR_PORT   PORTD
#define MTR_DDR    DDRD
#define IN1_PIN    0
#define IN2_PIN    1
#define ENA_PIN    5

// ====== 동작 설정값 ======
#define BEEP_INTERVAL_MS 500UL
#define LINK_TIMEOUT_MS  2000UL
#define PRESENT_HOLD_MS  1200UL

// ====== 전역 변수 ======
volatile uint32_t millis_count = 0;
volatile char last_rx_data = '0';
volatile uint32_t last_rx_time = 0;
volatile uint32_t last_helmet_time = 0;
volatile uint8_t buzzer_toggle_flag = 0;

// 1ms 타이머 인터럽트
ISR(TIMER1_COMPA_vect){ 
    millis_count++; 
}

// Timer1 초기화 (1ms 주기)
static void timer1_init(void){
    TCCR1A = 0;
    TCCR1B = (1<<WGM12) | (1<<CS11) | (1<<CS10); // 64분주, CTC 모드
    OCR1A = (F_CPU / 64000UL) - 1;
    TIMSK |= (1<<OCIE1A);
}

// 밀리초 반환 함수
static inline uint32_t get_millis(void){ 
    return millis_count; 
}

// ====== UART 통신 초기화 및 인터럽트 ======
static void usart_init(void){
    // UART0 초기화
    UCSR0A = (1<<U2X0);
    uint16_t ubrr = (F_CPU / (8UL * BAUD)) - 1;
    UBRR0H = (ubrr>>8); 
    UBRR0L = (uint8_t)ubrr;
    UCSR0B = (1<<RXEN0) | (1<<RXCIE0);
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);

    // UART1 초기화
    UCSR1A = (1<<U2X1);
    UBRR1H = (ubrr>>8); 
    UBRR1L = (uint8_t)ubrr;
    UCSR1B = (1<<RXEN1) | (1<<RXCIE1);
    UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);
}

// 데이터 수신 처리 함수
static void process_rx_data(char c){
    if(c == '0' || c == '1'){
        last_rx_data = c;
        last_rx_time = millis_count;
        if(c == '1') {
            last_helmet_time = millis_count;
        }
    }
}

ISR(USART0_RX_vect){
    if(!(UCSR0A & ((1<<FE0)|(1<<DOR0)|(1<<UPE0)))) {
        process_rx_data(UDR0);
    }
}

ISR(USART1_RX_vect){
    if(!(UCSR1A & ((1<<FE1)|(1<<DOR1)|(1<<UPE1)))) {
        process_rx_data(UDR1);
    }
}

// ====== 패시브 부저 제어 (Timer2) ======
static void timer2_buzzer_init(void){
    TCCR2 = (1<<WGM21) | (1<<CS21); // CTC 모드, 8분주
    OCR2 = 249; // 약 4kHz 톤 발생
    TIMSK |= (1<<OCIE2);
}

ISR(TIMER2_COMP_vect){
    if(buzzer_toggle_flag){
        BUZZ_PORT ^= (1<<BUZZ_PIN_NO);
    } else {
        BUZZ_PORT &= ~(1<<BUZZ_PIN_NO);
    }
}

// ====== 모터 제어 함수 ======
static void motor_run(void){
    MTR_PORT |= (1<<ENA_PIN) | (1<<IN1_PIN);
    MTR_PORT &= ~(1<<IN2_PIN);
}

static void motor_stop(void){
    MTR_PORT &= ~((1<<ENA_PIN) | (1<<IN1_PIN) | (1<<IN2_PIN));
}

int main(void){
    // 입출력 핀 방향 설정
    LED_DDR |= (1<<LED_PIN_NO);
    BUZZ_DDR |= (1<<BUZZ_PIN_NO);
    MTR_DDR |= (1<<IN1_PIN) | (1<<IN2_PIN) | (1<<ENA_PIN);
    
    // 스위치 입력 설정 (택트 스위치는 풀업 저항 활성화)
    TACT_DDR &= ~(1<<TACT_PIN_NO);
    PORTA |= (1<<TACT_PIN_NO); 
    SLIDE_DDR &= ~(1<<SLIDE_PIN_NO);

    // 초기화
    LED_PORT &= ~(1<<LED_PIN_NO);
    motor_stop();
    
    timer1_init();
    usart_init();
    timer2_buzzer_init();
    sei(); // 전체 인터럽트 활성화

    // 상태 변수
    bool is_led_on = false;
    bool is_buzzer_on = false;
    bool beep_state = false;
    uint32_t last_beep_time = 0;

    while(1){
        uint32_t current_time = get_millis();

        // 스위치 상태 읽기
        bool is_slide_on = (SLIDE_PIN_REG & (1<<SLIDE_PIN_NO));
        bool is_tact_pressed = !(TACT_PIN_REG & (1<<TACT_PIN_NO)); // Active Low

        // 통신 연결 및 헬멧 착용 상태 판정
        bool is_connected = (last_rx_time != 0) && ((current_time - last_rx_time) <= LINK_TIMEOUT_MS);
        bool is_helmet_now = (is_connected && (last_rx_data == '1'));
        bool is_helmet_recently = (last_helmet_time != 0) && ((current_time - last_helmet_time) <= PRESENT_HOLD_MS);

        // 1. 전원(슬라이드 스위치) OFF 상태
        if(!is_slide_on){
            is_led_on = false;
            LED_PORT &= ~(1<<LED_PIN_NO);
            is_buzzer_on = false;
            motor_stop();
        }
        // 2. 전원 ON 상태 메인 로직
        else {
            // LED(킥보드 전원)가 꺼져있을 때
            if(!is_led_on){
                if(is_helmet_now){
                    is_led_on = true; // 헬멧 쓰면 켜짐
                    LED_PORT |= (1<<LED_PIN_NO);
                    is_buzzer_on = false;
                } else {
                    is_buzzer_on = true; // 헬멧 안 쓰면 경고음 대기
                }
            }
            // LED가 켜져있을 때 (주행 가능 상태)
            else {
                LED_PORT |= (1<<LED_PIN_NO);
                is_buzzer_on = !is_helmet_recently; // 최근에 헬멧을 벗었으면 경고음 발생
            }

            // 모터 동작: LED가 켜져 있고(조건 충족) 엑셀(택트)을 누를 때만 전진
            if(is_led_on && is_tact_pressed){
                motor_run();
            } else {
                motor_stop();
            }
        }

        // 경고음(부저) 깜빡임 제어 (0.5초 간격)
        if(is_buzzer_on){
            if(current_time - last_beep_time >= BEEP_INTERVAL_MS){
                last_beep_time = current_time;
                beep_state = !beep_state;
            }
        } else {
            beep_state = false;
        }
        
        buzzer_toggle_flag = (is_buzzer_on && beep_state);

        _delay_ms(10); // 루프 딜레이
    }
}