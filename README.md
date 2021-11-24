# C_minus_compiler
C minus 컴파일러를 직접 구현해 보는 리포지토리입니다.
현재는 Scanner과 Parser까지만 구현되어 있습니다.

### 1. 프로젝트 구조는 다음과 같습니다.

```python
1. document 폴더 : C-- language의 문법 규칙이 있는 PDF 파일이 있습니다. Scanner나 Parser 코드를 볼 때, 해당 파일과 함꼐 보시는 것을 권장합니다.
2. Scanner 폴더 : 컴파일러의 첫 단계인 Lexical Analysis를 하는 Scanner 프로그램이 들어가 있습니다.
3. Parser 폴더 : Scanner의 결과물인 stream of token을 이용하여 Syntax Analysis 수행 후 parse tree를 만듭니다.
```

## 아래는 개략적인 각 프로그램의 입출력 예시입니다. 더 자세한 설명은 해당 폴더 안에 있습니다.


### 1. Scanner

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

### 2. parser

예시 입력 ( Stream of tokens )
```c++
바로 위 Scanner의 출력과 동일합니다.
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
