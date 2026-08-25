
# 🛴 전동 킥보드 헬멧 착용 인식 및 안전 제어 시스템
> **🏆 교내 캡스톤 디자인 경진대회 '대상' 수상작**


## 🛠 개발 기간 및 사용 기술 (Tech Stack)
- **Hardware** : ATmega128 (JMOD-128-1, 5V 기반), 초음파 센서(HC-SR04), 블루투스 모듈
- **Software** : C (Embedded)



## 💡 주요 기능 및 핵심 로직
- **하드웨어 타이머 제어** : Timer1을 활용한 정밀 측정 및 Timeout 예외 처리
- **Fail-safe (급제동 방지) 로직** : 주행 중 헬멧 이탈 감지 시 즉각적인 모터 정지가 아닌, 탑승자 보호를 위한 **경고 주행 모드** 진입

## 👤 담당 역할
- 4인 팀 프로젝트
- 센서 문제 원인 분석 및 초음파 센서 변경 제안
- ATmega128 펌웨어 공동 개발(Fail-safe 안전 제어 로직 설계)

## 🔄 시스템 순서도 (Flowchart)

**[송신부]**
<img width="400" height="500" alt="송신부 순서도" src="https://github.com/user-attachments/assets/d7890438-ec34-4ef9-9db2-e878349b7ad3" />

<br>

**[수신부]**
<img width="550" height="1000" alt="수신부 순서도" src="https://github.com/user-attachments/assets/c2e04aff-52f8-46e0-95c2-974063d6513a" />



## 시연영상

**헬멧 미착용**
https://github.com/user-attachments/assets/0de99bac-186e-4e9e-91e7-ea15a97d2041


**헬멧 착용**
https://github.com/user-attachments/assets/8b8afa14-92cc-4021-8c89-9e30f3b152db


**주행 허가 상태 이후 헬멧 미착용**
https://github.com/user-attachments/assets/965b8358-3441-4cd6-a13c-a6519cd525c8



## 🐍 Python: SW Test Log Analysis & Reporting
파이썬을 활용하여 킥보드와 통신하는 UART 시리얼 로그를 분석하고, 초음파 센서 기반의 헬멧 착용 인식 시스템이 정상적으로 동작하는지 검증합니다.[cite: 1]

### 1. 주요 기능 (Main Features)
* **시리얼 로그 데이터 분석:** 킥보드 주행 중 기록된 UART 통신 로그(CSV)를 불러와 시간(Time)에 따른 상태 변화를 추적합니다.[cite: 1]
* **예외 상황 및 오동작 검증:**[cite: 1]
  * **센서 인식 검증:** 초음파 센서를 통해 헬멧 미착용 상태(`State == 0`)가 감지되었을 때 경고 주행 로직이 제대로 작동하는지 확인합니다.[cite: 1]
  * **통신 단절(Timeout) 감지:** 데이터 수신 간격을 초 단위로 계산(`Time_Diff`)하여, 2초 이상 지연 시 블루투스 통신 단절 오류로 식별합니다.[cite: 1]
* **자동화 테스트 리포트 생성:** 발생한 예외 상황(헬멧 미착용, 통신 단절)의 로그만 필터링하여 엑셀(.xlsx) 파일 형태의 결과 보고서로 자동 저장합니다.[cite: 1]

### 2. 사용된 라이브러리 (Dependencies)
* `pandas`: 시계열 데이터 처리, 데이터 필터링 및 엑셀(Excel) 리포트 자동 출력[cite: 1]





