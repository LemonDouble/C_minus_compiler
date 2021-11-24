# Scanner

해당 폴더는 Lexical Analayzer인 Scanner의 구현체가 있는 폴더입니다.
Scanner는 Source code를 읽고 해당 Source code를 Stream of token으로 분류합니다.

### 1. 프로젝트 실행 방식은 다음과 같습니다.

```text
1. (option) make 명령어를 이용하여 소스코드를 build합니다.
2. linux 사용자의 경우 scanner.out을, windows 사용자의 경우 scanner.exe를 터미널을 통해 실행합니다.
3. ./scanner.out inputfile outfile 과 같이 사용합니다.
만약 ./scanner.out 1.c 1.out 과 같이 실행한 경우, 1.c 파일을 scanner가 읽어 처리한 후, 1.out 파일에 해당 결과를 작성합니다.
```

### 2. 해당 폴더의 파일 목록은 다음과 같습니다.
```python
1. scanner.cpp : scanner의 c++ 소스 코드 파일입니다. 
2. scanner.out : UNIX ( 또는 Linux ) 사용자가 사용할 수 있는, 컴파일된 소스 코드입니다.
3. scanner.exe : Windows 사용자가 사용할 수 있는, 컴파일된 소스 코드입니다.
4. 1.c, 2.c : 예시 input 파일입니다.
5. 1.out : 예시 output 파일입니다.
6. C_minus_Scanner.pdf : 해당 프로젝트 진행 시의 요구 조건 및 처리 지침에 대한 내용의 자료입니다.
7. Makefile : 프로젝트 빌드를 위한 Makefile입니다. (make 명령어 사용 위함)
```

## 아래는 입출력 예시입니다. 1.c 의 input / output과 동일합니다.

예시 입력 (Source Code)

```c++
/* A program to perform Euclid's 
Algorithm to compute gcd */

int gcd (int u, int v)
{   if (v==0) return u;
    else return gcd(v, u-u/v*v);
    /* u-u/v*v == u mod v */
}

void main(void)
{   int x; int y;
    x=input(); y=input();
    output(gcd(x,y));
}
```

예시 출력 (Stream of tokens)
```c++
1: /* A program to perform Euclid's 
2: Algorithm to compute gcd */
3: 
4: int gcd (int u, int v)
	4: ID, name = int
	4: ID, name = gcd
	4: (
	4: ID, name = int
	4: ID, name = u
	4: ,
	4: ID, name = int
	4: ID, name = v
	4: )
5: {   if (v==0) return u;
	5: {
	5: reserved word: if
	5: (
	5: ID, name = v
	5: ==
	5: NUM, val = 0
	5: )
	5: reserved word: return
	5: ID, name = u
	5: ;
6:     else return gcd(v, u-u/v*v);
	6: reserved word: else
	6: reserved word: return
	6: ID, name = gcd
	6: (
	6: ID, name = v
	6: ,
	6: ID, name = u
	6: -
	6: ID, name = u
	6: /
	6: ID, name = v
	6: *
	6: ID, name = v
	6: )
	6: ;
7:     /* u-u/v*v == u mod v */
8: }
	8: }
9: 
10: void main(void)
	10: reserved word: void
	10: ID, name = main
	10: (
	10: reserved word: void
	10: )
11: {   int x; int y;
	11: {
	11: ID, name = int
	11: ID, name = x
	11: ;
	11: ID, name = int
	11: ID, name = y
	11: ;
12:     x=input(); y=input();
	12: ID, name = x
	12: =
	12: ID, name = input
	12: (
	12: )
	12: ;
	12: ID, name = y
	12: =
	12: ID, name = input
	12: (
	12: )
	12: ;
13:     output(gcd(x,y));
	13: ID, name = output
	13: (
	13: ID, name = gcd
	13: (
	13: ID, name = x
	13: ,
	13: ID, name = y
	13: )
	13: )
	13: ;
14: }
	14: }
15: 
```
