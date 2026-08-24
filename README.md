
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
- **센서 문제 원인 분석 및 초음파 센서 변경 제안
- **ATmega128 펌웨어 공동 개발(Fail-safe 안전 제어 로직 설계)

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







