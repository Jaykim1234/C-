import java.util.*;



class Solution {
    // public은 제한없이 어디에서나 사용이 가능하다는 뜻이 된다. 
    public int solution(int[] food_times, long k) { // int[] 크기선언
        long food_sum = 0;  // 모든 음식 먹는데 걸리는 총 시간
        for (int i = 0; i < food_times.length; i++) { // i 가 전체 food times의 길이가 되기 전
            //까지 계속 food_sum에 더함
            food_sum += food_times[i];
        }

        if (food_sum <= k) return -1; //k는 전체 음식을 먹는 시간을 의미
        // 음식의 전체 크기/음식을 다 먹는데 걸리는 시간이 k보다 작음

        // (음식 시간, 순서) 쌍으로 우선순위 큐에 저장
        PriorityQueue<Food> pq = new PriorityQueue<>();
        // import java.util.PriorityQueue; //import
        // int형 priorityQueue 선언 (우선순위가 낮은 숫자 순)

PriorityQueue<Integer> priorityQueue = new PriorityQueue<>();
        for (int i = 0; i < food_times.length; i++) {
            pq.offer(new Food(food_times[i], i + 1));
        }

        long total = 0;   // 먹기 위해 사용한 시간
        long previous = 0;  // 직전에 다 먹은 음식 시간
        long length = food_times.length;    // 남은 음식 개수

        while (total + ((pq.peek().time - previous) * length) <= k) {
            int now = pq.poll().time;queue.poll();  // queue에 첫번째 값을 반환하고 제거 비어있다면 null
            total += (now - previous) * length;
            length -= 1;
            previous = now;
        }

        ArrayList<Food> result = new ArrayList<>();
        while (!pq.isEmpty()) {
            result.add(pq.poll());
        }

        Collections.sort(result, new Comparator<Food>() {
            @Override
            public int compare(Food a, Food b) {
                return Integer.compare(a.idx, b.idx);
            }
        });

        return result.get((int) ((k - total) % length)).idx;
    }
}

