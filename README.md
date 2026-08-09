
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


## 시연영상

**헬멧 미착용**
https://github.com/user-attachments/assets/0de99bac-186e-4e9e-91e7-ea15a97d2041


**헬멧 착용**
https://github.com/user-attachments/assets/8b8afa14-92cc-4021-8c89-9e30f3b152db


**주행 허가 상태 이후 헬멧 미착용**
https://github.com/user-attachments/assets/965b8358-3441-4cd6-a13c-a6519cd525c8



## 📊 테스트 자동화 및 시스템 검증 (Python)
실제 주행 중 발생하는 예외 상황을 검증하고 하드웨어 동작의 신뢰성을 높이기 위해, 파이썬을 활용한 로그 분석 스크립트를 직접 구현


- **분석 대상**: 킥보드 동작 중 기록된 시리얼 통신 로그 데이터 (CSV)
- **주요 로직**: `Pandas` 라이브러리를 활용해 수천 줄의 로그 데이터 간 시간 차이(Time difference)를 계산하여, 통신 지연 및 Timeout 등의 예외 상황(Edge Case) 검출
- **결과 자동화**: 분석된 오류 내역을 가독성 높은 엑셀(Excel) 파일로 변환하여 검증 시간 단축


**[스크립트 실행 방법]**
> ※ 본 저장소에 포함된 샘플 로그 파일(`uart_test_log.csv`)을 활용하면, 실물 킥보드 하드웨어가 없어도 분석 스크립트를 직접 실행하고 엑셀 결과물이 생성되는 과정을 확인할 수 있습니다.

1. 필요 라이브러리 설치
`pip install pandas`

2. 테스트 스크립트 실행
`python check_uart_log.py`




