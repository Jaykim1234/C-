import sys
import heapq
from itertools import count
# python 2-2.py input_file0.txt input_file1.txt input_file2.txt output_file.txt
def merge_sorted_files(file_paths):
    heap = []
    result = []

    # 파일 인덱스와 파일 핸들을 함께 저장하기 위한 우선순위 큐
    file_handles = [(i, open(file, 'r')) for i, file in enumerate(file_paths)]
    print(file_handles)
    # 각 파일에서 첫 번째 라인을 읽어와 heap에 저장
    for i, handle in file_handles:
        print(i, handle)
        line = handle.readline()
        if line:
            timestamp, price = map(int, line.strip().split(','))
            heapq.heappush(heap, (timestamp, price, i, handle))

    # 카운터를 사용하여 timestamp가 같을 때 파일 인덱스 순서대로 정렬
    # file_index_counter = count()

    while heap:
        timestamp, price, file_index, handle = heapq.heappop(heap)
        print(timestamp, price, file_index)
        #result.append((timestamp, next(file_index_counter), price))
        result.append((timestamp, file_index, price))

        # 다음 라인을 heap에 추가
        line = handle.readline()
        if line:
            next_timestamp, next_price = map(int, line.strip().split(','))
            heapq.heappush(heap, (next_timestamp, next_price, file_index, handle))

    # 파일 핸들 닫기
    for _, handle in file_handles:
        handle.close()

    return result

def write_output_file(result, output_file_path):
    with open(output_file_path, 'w') as output_file:
        for timestamp, file_index, price in result:
            output_file.write(f"{timestamp},{file_index},{price}\n")

if __name__ == "__main__":
    # Command Line Argument로 주어진 파일 경로들
    input_file_paths = sys.argv[1:-1]
    output_file_path = sys.argv[-1]

    # 정렬 및 병합
    merged_result = merge_sorted_files(input_file_paths)
    print(input_file_paths)
    print(output_file_path)
    # 결과 파일 작성
    write_output_file(merged_result, output_file_path)
    print('Done')
