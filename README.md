# 🛴 전동 킥보드 헬멧 착용 인식 및 안전 제어 시스템
> **🏆 교내 캡스톤 디자인 경진대회 '대상' 수상작**

## 🛠 개발 기간 및 사용 기술 (Tech Stack)
- **Hardware** : ATmega128 (JMOD-128-1, 5V 기반), ToF 센서, 블루투스 모듈
- **Software** : C (Embedded), Python (Pandas)

## 💡 주요 기능 및 핵심 로직
- **하드웨어 타이머 제어** : Timer1을 활용한 0.5µs 단위 정밀 측정 및 Timeout 예외 처리
- **Fail-safe (급제동 방지) 로직** : 주행 중 헬멧 이탈 감지 시 즉각적인 모터 정지가 아닌, 탑승자 보호를 위한 **경고 주행 모드** 진입
- **테스트 자동화** : 파이썬 `diff()` 함수를 활용한 시리얼 로그 분석 및 예외 상황(Edge Case) 검증

## 🔄 시스템 순서도 (Flowchart)

**[송신부]**
<img width="400" height="600" alt="송신부 순서도" src="https://github.com/user-attachments/assets/d7890438-ec34-4ef9-9db2-e878349b7ad3" />

<br>

**[수신부]**
<img width="500" height="900" alt="수신부 순서도" src="https://github.com/user-attachments/assets/b041c24e-dc71-40c8-81d2-568a5e61b9a5" />





