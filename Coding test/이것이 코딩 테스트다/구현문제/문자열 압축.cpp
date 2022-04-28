#include <string>

using namespace std;// using 키워드로 std 라이브러리에 포함된 내용들을 사용할 것이라고 선언합니다. 
// https://jjeongil.tistory.com/227


int solution(string s){
    int answer = s.size();
    for(int i=1; i<=s.size()/2; i++){
        int cnt =1;
        string temp = "", a="";
        a = s.substr(0,i);
        // basic_string substr(size_type pos = 0, size_type count = npos) const;
        // 문자열의 일부를 리턴한다.
        // 문자열의 pos 번째 문자 부터 count 길이 만큼의 문자열을 리턴한다. 
        // 만약에, 인자로 전달된 부분 문자열의 길이가 문자열 보다 길다면, 그 이상을 반환하지 않고 문자열의 끝 까지만 리턴한다.
        // 또한, count 로 npos 를 전달한다면, 자동으로 pos 부터 원래 문자열의 끝 까지 리턴한다.
        for(int j=1; j<s.size(); j+=i){
            if(a==s.substr(j,i)) cnt++;
            else{
                if(cnt>1) temp+= to_string(cnt);
                temp += a;
                a =s.substr(j,i);
                cnt = 1;
            }
        }
        if(cnt>1) temp+=to_string(cnt);
        temp +=a;
        if(answer>temp.size()) answer = temp.size();
    }
    return answer;
}
    