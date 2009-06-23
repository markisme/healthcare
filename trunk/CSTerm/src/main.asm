TITLE MASM Template						(main.asm)

; Description:
; 
; Revision date:

INCLUDE Irvine32.inc

StudentInfo struct
	name	byte  10 DUP(?)
	point	Dword 0
StudentInfo ends

.data
inputmenu			byte	"데이터 입력", 0
inputname			byte	"이름 : ",0
inputPoint			byte	"점수 : ",0
tempinputname		byte	10 DUP(?)
tempinputpoint		Dword	0

studentData StudentInfo 100 dup(<>)
dSize Dword 0

.code
;----------------------------------------------------------
; 일단은 점수만 소팅 되도록
BubbleSort PROC USES eax ecx esi edi,
	pArray:PTR DWORD,		; pointer to array
	Count:DWORD				; array size

	mov ecx,Count
	dec ecx					; decrement count by 1
	
L1:	push ecx				; save outer loop count
	mov esi, pArray
	add esi, 10

L2:	mov eax, [esi]			; get array value
	cmp [esi+14],eax		; compare a pair of values
	jge L3					; if [esi] <= [edi], don't exch
	xchg eax,[esi+14]		; exchange the pair
	mov [esi],eax

L3:	add esi,10				; move both pointers forward
	loop L2					; inner loop

	pop ecx					; retrieve outer loop count
	loop L1					; else repeat outer loop

L4:	ret
BubbleSort ENDP
;----------------------------------------------------------

;----------------------------------------------------------
; 분산 계산 관련 프로시저
;----------------------------------------------------------

;----------------------------------------------------------
; 출력 관련 프로시저
;----------------------------------------------------------

;----------------------------------------------------------
main PROC
	call Clrscr
	
	;입력 메세지 출력
	mov edx, offset inputmenu
	call writestring

	mov ecx,2		;루프 카운터 셋팅
	mov dSize, 0	;사이즈 셋팅
	
inputdata:	push ecx
	;이름 입력 메세지를 띄우고 값을 읽어옴
	mov edx, offset inputName
	call writestring
	mov edx, offset tempinputName
	mov ecx, (Sizeof tempinputName)
	call readstring

	;점수 입력 메세지를 띄우고 값을 읽어옴
	mov edx, offset inputPoint
	call writestring	
	call readint
	mov tempinputpoint, eax
	
	;구조체에 저장
	mov eax,14
	mul dSize
	
	mov edi, offset studentdata + 14
	add edi, eax
	mov eax, tempinputpoint
	mov (StudentInfo ptr [edi]).point, eax
	;mov esi, offset tempinputName
	;invoke Str_copy, esi, edi
		
	inc dSize
	
	;이름이 널값이고 점수가 -1 이면 입력 끝
	
	;아니면 계속 루프
	pop ecx
	loop inputdata
	
	;데이터 입력 확인
	
	;소팅
	invoke BubbleSort, ADDR studentData, 100
	
	;데이터 입력 확인
	
	;분산(표준편차) 계산

	exit
main ENDP
;----------------------------------------------------------

END main