import pandas as pd
from concurrent.futures import ProcessPoolExecutor
from linear_regression import calculate_coefficients

def main(input_data):
    print('Code running')

    # 병렬 계산을 위한 프로세스 풀 생성
    with ProcessPoolExecutor() as executor:
        # 각 프로세스에서 계수 계산
        results = list(executor.map(calculate_coefficients, [input_data]))
    print('병렬진행 끝')
    # 결과를 1차원 배열로 펼치기
    coefficients = [coef for sublist in results for coef in sublist]

    # 결과 출력
    result_df = pd.DataFrame({'Coefficient': coefficients})
    print(result_df)

if __name__ == "__main__":
    df = pd.read_csv(r'/Users/jinhyun/Documents/GitHub/Cpp/Vaccum./data2-1.csv')

    main(df)
