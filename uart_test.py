import pandas as pd

def check_uart_log(file_path):
    # 1. 킥보드에서 측정한 시리얼 로그 파일 불러오기
    df = pd.read_csv(file_path)
    df['Time'] = pd.to_datetime(df['Time'])
    
    # 통신이 끊겼는지 확인하기 위해 이전 데이터가 들어온 시간과의 차이를 초 단위로 계산
    df['Time_Diff(sec)'] = df['Time'].diff().dt.total_seconds()

    # 2. 에러 검증하기
    # 수신부에서 헬멧 벗었을 때(0) 경고주행 잘 되는지 확인
    helmet_fail_logs = df[df['State'] == 0]
    
    # 송수신이 끊겼을 때 수신부 코드(2초 지연 시 타임아웃) 제대로 동작하는지 확인
    # 0.1초마다 와야 할 데이터가 2초 이상 안 왔다면 통신이 끊긴 것으로 판단
    timeout_logs = df[df['Time_Diff(sec)'] >= 2.0]

    # 3. 확인된 에러 로그들만 모아서 엑셀로 저장 (보고서 첨부용)
    report_name = 'SW_Test_251110.xlsx'
    with pd.ExcelWriter(report_name) as writer:
        helmet_fail_logs.to_excel(writer, sheet_name='헬멧미착용_경고주행', index=False)
        timeout_logs.to_excel(writer, sheet_name='통신단절_오류', index=False)

    print(f"테스트 검증 완료, 엑셀 파일 : ({report_name})")

# 2025.11 통합 테스트 때 추출한 로그로 실행
check_uart_log('uart_test_log.csv')