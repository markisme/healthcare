TITLE 성적관리프로그램				(main.asm)
;정보통신대 컴퓨터학과 2000160005 한규혁

INCLUDE Irvine32.inc

StudentInfo struct
	stuName byte 8 DUP(?)
	point	byte 0
StudentInfo ends

.data
inputText			byte	"데이터 입력", 0
printText			byte	"데이터 출력", 0
nameText			byte	"이름 : ",0
pointText			byte	"점수 : ",0
deviationText		byte	"표준편차 : ",0
spaceText			byte	" ",0
tempinputname		byte	8 DUP(?)
tempinputpoint		Dword	0
dataSize			Dword	0
deviationValue		Dword	0

structsize = 9
pointpos = 8

studentData StudentInfo 100 dup(<>)
dSize Dword 0

.code
;----------------------------------------------------------
;데이터 입력 프로시저
InputData PROC USES eax ecx edi edx esi
	;입력 메세지 출력
	mov edx, offset inputText
	call writestring
	call crlf
	
	;루프의 카운터
	mov ecx, 100
	dec ecx	
	
	;초기 값
	mov edi, offset studentdata
	mov dataSize, 0
	
INPUT:	
	push ecx
	
	;이름 입력 메세지를 띄우고 값을 읽어옴
	mov edx, offset nameText
	call writestring
	mov edx, offset tempinputName
	mov ecx, (Sizeof tempinputName)
	call readstring

INPUTPOINT:
	;점수 입력 메세지를 띄우고 값을 읽어옴
	mov edx, offset pointText
	call writestring	
	call readint
	mov tempinputpoint, eax
	
	;점수가 -1 이면 입력 끝
	cmp eax, -1
	jne SAVE
	pop ecx
	ret

SAVE:
	;점수가 100보다 크면 다시 입력
	cmp tempinputpoint, 100 		;tempinputpoint > 100 ?
	ja INPUTPOINT

	inc dataSize
	
	;이름 저장
	mov esi, offset tempinputName
	invoke Str_copy, esi, edi
	
	;점수 저장
	mov eax, tempinputpoint
	mov (StudentInfo ptr [edi]).point, al
	
	;넥스트
	add edi, structsize

	;아니면 계속 루프
	pop ecx
	loop INPUT

	ret

InputData ENDP
;----------------------------------------------------------
	
;----------------------------------------------------------
;데이터 출력 프로시저
PrintData PROC USES eax ecx edi edx
	;출력 메세지 출력
	mov edx, offset inputText
	call writestring
	call crlf
	
	;루프의 카운터
	mov ecx, dataSize
	;dec ecx	
	
	;초기 값
	mov edi, offset studentData
	
	;출력 시작
Print:	
	push ecx
	
	;이름 메세지와 출력
	mov edx, offset nameText
	call writestring
	mov edx, edi
	call writestring
	mov edx, offset spaceText
	call writestring
	
	;점수 메시지와 출력
	mov edx, offset pointText
	call writestring
	mov al, [edi+pointpos]
	call writeint
	call crlf
	
	;넥스트
	add edi, structsize
	
	pop ecx		
	Loop Print
	
	;표준편차 메시지와 출력
	mov edx, offset deviationText
	call writestring	
	mov eax, deviationValue
	call writeint
	call crlf	
	
	ret
	
PrintData ENDP
;----------------------------------------------------------

;----------------------------------------------------------
;버블 소트 프로시저
BubbleSort PROC USES eax ecx edi

SortIntPart:
	;루프의 카운터
	mov ecx, dataSize
	;dec ecx		

	;버플 소팅 시작
StartSort:
	push ecx
	mov edi, offset studentData

Compare:
	;원소 2개 잡음
	mov bl, [edi+pointpos]
	mov dl, byte ptr [edi+structsize+pointpos]
			
	;비교
	cmp bl, dl
	jge noChange

	;점수교환
	xchg bl, dl
	mov [edi+pointpos], bl
	mov byte ptr [edi+structsize+pointpos],dl
	
	;이름교환
	push eax
	INVOKE Str_copy, ADDR (Studentinfo ptr [edi]).stuName, ADDR tempinputname
	INVOKE Str_copy, ADDR (Studentinfo ptr [edi+structsize]).stuName, ADDR (Studentinfo ptr [edi]).stuName
	INVOKE Str_copy, ADDR tempinputname, ADDR (Studentinfo ptr [edi+structsize]).stuName
	pop eax
	
noChange:
	;넥스트
	add edi, structsize			
	loop Compare
			
	pop ecx		
	Loop StartSort
	
	ret

BubbleSort ENDP
;----------------------------------------------------------

;----------------------------------------------------------
;분산 계산 프로시저
Deviation PROC USES eax ecx edi edx
	;지역 변수
	LOCAL SQUSUM:DWORD
	LOCAL SUM:DWORD
	LOCAL RES:DWORD
	LOCAL SQRT:DWORD
	mov SQRT, 0

	;점수 제곱의 합 구하기
	;초기 값
	mov ecx, dataSize
	mov edi, offset studentData
	mov edx, 0

CALCULATION1:
	push ecx
		
	;Σi=1,N xi2 계산
	mov al, (StudentInfo ptr [edi]).point
	mov bl, al
	mul bl
	add edx, eax
	
	pop ecx
	
	;넥스트
	add edi, structsize
	loop CALCULATION1
	
	mov SQUSUM, edx
	
	;점수의 합 구하기
	;초기 값
	mov ecx, dataSize
	mov edi, offset studentData
	mov edx, 0

CALCULATION2:
	push ecx
		
	;Σi=1,N xi 계산
	mov al, (StudentInfo ptr [edi]).point
	add edx, eax
	
	pop ecx
	
	;넥스트
	add edi, structsize
	loop CALCULATION2
	
	mov SUM, edx
	
CALCULATION3:
	;σ= [{Σi=1,N xi2 - (Σi=1,N xi)2 /N} /(N-1) ] 계산	
	;점수의 합의 제곱
	mov eax, SUM
	mov ebx, eax
	mul ebx
	
	;점수의 합의 제곱에 평균
	mov ebx, dataSize
	div ebx
	
	;중괄호 내부 계산
	sub SQUSUM, eax 
	
	;대괄호 내부 계산
	mov eax, SQUSUM
	mov ebx, dataSize
	sub ebx, 1
	div ebx
	
	mov RES, eax
	
	;제곱근 계산
	;초기 값
	mov edi, 1			; n = 0
	mov edx, 0			; sigma

CALCULATION4:
	;Σ(2n-1)
	mov eax, 2
	mov ebx, edi
	mul ebx
	sub eax, 1
	add SQRT, eax
	
	;
	mov edx, SQRT
	cmp edx, RES 		;edx >= RES ?
	jae CALCULATION5
	
	;
	inc edi
	mov ecx, 2
	loop CALCULATION4

CALCULATION5:	
	;표준편차 값
	mov deviationValue, edi
	
	ret
	
Deviation ENDP
;----------------------------------------------------------

;----------------------------------------------------------
;메인 프로시저
main PROC
	call Clrscr
	
	;데이터 입력
	call InputData
	
	;데이터 입력 확인
	cmp dataSize, 0
	jne PROCESS
	exit

PROCESS:
	;데이터 크기 확인
	cmp dataSize, 1
	jbe PRINT		; datasize <= 1 ?
	
	;소팅
	call BubbleSort

	;분산 계산
	call Deviation

PRINT:	
	;데이터 출력
	call PrintData
	
	exit
main ENDP
;----------------------------------------------------------

END main