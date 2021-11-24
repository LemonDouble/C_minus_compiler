# parser

해당 폴더는 Syntax Analayzer인 Parser의 구현체가 있는 폴더입니다.

Parser는 Scanner가 처리한 Stream of tokens를 Parse tree(Syntax tree)로 변환합니다.

### 1. 프로젝트 실행 방식은 다음과 같습니다.

```text
1. (option) make 명령어를 이용하여 소스코드를 build합니다.
2. linux 사용자의 경우 parser.out을, windows 사용자의 경우 parser.exe를 터미널을 통해 실행합니다.
3. ./parser.out inputfile outfile 과 같이 사용합니다.
만약 ./parser.out 1.c 1.out 과 같이 실행한 경우, 1.c 파일을 parser가 읽어 처리한 후, 1.out 파일에 해당 결과를 작성합니다.
```

### 2. 해당 폴더의 파일 목록은 다음과 같습니다.
```python
1. parser.cpp, parser.h : parser의 헤더 및 c++ 소스 코드 파일입니다. 
2. scanner.cpp, scanner.h : scanner의 헤더 및 c++ 소스 코드 파일입니다. parser는 scanner의 처리 결과물이 필요하므로, 같이 빌드하였습니다.
3. main.cpp : parser 및 scanner를 호출하여 소스 코드를 해석합니다. main 함수가 있습니다.
4. parser.out : UNIX ( 또는 Linux ) 사용자가 사용할 수 있는, 컴파일된 소스 코드입니다.
5. parser.exe : Windows 사용자가 사용할 수 있는, 컴파일된 소스 코드입니다.
6. 1.c, 2.c : 예시 input 파일입니다.
7. 1.out, 2.out : 예시 output 파일입니다.
8. C_minus_Parser.pdf : 해당 프로젝트 진행 시의 요구 조건 및 처리 지침에 대한 내용의 자료입니다.
9. Makefile : 프로젝트 빌드를 위한 Makefile입니다. (make 명령어 사용 위함)
10. parser.h.gch / scanner.h.gch : g++ 사용시 자동으로 생성되는 컴파일된 헤더입니다. 삭제해도 구동에 문제가 없습니다.
```

## 아래는 입출력 예시입니다. 1.c 의 input / output과 동일합니다.

* 프로젝트 전체에서는 source code를 읽고 parse tree를 만들지만, 아래 예시 입/출력은 Parser의 역할을 작성한 것입니다.

예시 입력 ( Stream of tokens )
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

예시 출력 (Parse tree)

-> 실제 데이터는 Tree지만, 출력을 위해 Tree의 부모-자식 관계를 들여쓰기를 통해 표현했습니다.
```c++
[STMT:DECLARATION_LIST]
	[STMT:FUN_DECLAR], attr -> INT,gcd
		[EXP:PARAMS]
			[EXP:PARAMS_LIST]
				[EXP:PARAM], attr -> INT,u,NOTARRAY
				[EXP:PARAM], attr -> INT,v,NOTARRAY
		[STMT:COMPOUND_STMT]
			[STMT:STMT_LIST]
				[STMT:SELECTION_STMT]
					[EXP:SIMPLE_EXPRESSION], attr -> EE
						[EXP:VAR], attr -> v
						[EXP:NUMBER], attr -> 0
					[STMT:RETURN_STMT]
						[EXP:VAR], attr -> u
					[STMT:RETURN_STMT]
						[EXP:CALL], attr -> gcd
							[EXP:ARG_LIST]
								[EXP:VAR], attr -> v
								[EXP:ADDITIVE_EXPRESSION]
									[EXP:VAR], attr -> u
									[EXP:ADDOP], attr -> MINUS
									[EXP:TERM]
										[EXP:VAR], attr -> u
										[EXP:MULOP], attr -> DIV
										[EXP:VAR], attr -> v
										[EXP:MULOP], attr -> MUL
										[EXP:VAR], attr -> v
	[STMT:FUN_DECLAR], attr -> VOID,main
		[EXP:PARAMS], attr -> VOID
		[STMT:COMPOUND_STMT]
			[STMT:LOCAL_DECLAR]
				[STMT:VAR_DECLAR], attr -> INT,x
				[STMT:VAR_DECLAR], attr -> INT,y
			[STMT:STMT_LIST]
				[EXP:EXPRESSION]
					[EXP:VAR], attr -> x
					[EXP:CALL], attr -> input
				[EXP:EXPRESSION]
					[EXP:VAR], attr -> y
					[EXP:CALL], attr -> input
				[EXP:CALL], attr -> output
					[EXP:CALL], attr -> gcd
						[EXP:ARG_LIST]
							[EXP:VAR], attr -> x
							[EXP:VAR], attr -> y
```
