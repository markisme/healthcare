TITLE MASM Template						(main.asm)

; Description:
; 
; Revision date:

INCLUDE Irvine32.inc
INCLUDE student.inc

StudentInfo struct
	name	byte "        "
	point	byte 0
StudentInfo ends

.data
inputmenu			byte	"데이터 입력", 0
inputname			byte	"이름 : ",0
inputPoint			byte	"점수 : ",0
tempinputname		byte	"        "
tempinputpoint		Dword	0

studentData StudentInfo 100 dup(<>)	;## 100개 구조체 선언

.code
main PROC
	call Clrscr
	
	;입력 메세지 출력
	mov edx, offset inputmenu
	call writestring

	mov  esi,2 ;루프 카운터 셋팅
	
inputdata:	
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
	mov edi, offset studentdata
	mov eax, tempinputpoint
	mov (StudentInfo ptr [edi]).point, al
	mov esi, offset tempinputName
	invoke Str_copy, esi, edi
	
	;이름이 널값이고 점수가 -1 이면 입력 끝
	
	;아니면 계속 루프
	loop inputdata
	
	;데이터 입력 확인
	
	;소팅
	
	;분산(표준편차) 계산

	exit
main ENDP

Sorting Proc, select:DWORD
	cmp select, 3
	mov eax, 17
	je SortIntPart
	cmp select, 4
	mov eax, 18
	je SortIntPart
	cmp select, 5
	mov eax, 19
	je SortIntPart
	cmp select, 6
	mov eax, 20
	je SortIntPart
	cmp select, 7
	mov eax, 21
	je SortIntPartT
	cmp select, 8
	mov eax, 25
	je SortIntPartT

	SortIntPart:		
		mov ecx, TotalNums
		dec ecx		
		BubbleInt:
			push ecx
			mov edi, offset studentData
			SortIntSub:
				push eax
				mov bl, [edi+eax]
				cmp bl, byte ptr [edi+structRowSize+eax]

				jb changeScore
				jmp NoChangeScore
				changeScore :
					push ecx
					mov ecx, StructRowsize
					mov eax, 0
					ChangeScoreSub:						
						mov bl, [edi+eax]
						mov dl, byte ptr [edi+structrowSize+eax]
						xchg bl, dl
						mov [edi+eax], bl
						mov byte ptr [edi+structRowSize+eax],dl
						inc eax
						loop ChangeScoreSub					
				pop ecx
				jmp NoChangeScore
	
				NoChangeScore:
					add edi, StructRowSize
					pop eax

				loop SortIntSub
			pop ecx
			Loop BubbleInt	
		ret

	SortIntPartT:		
		mov ecx, TotalNums
		dec ecx		
		BubbleIntT:
			push ecx
			mov edi, offset studentData
			SortIntSubT:
				push eax
				mov ebx, [edi+eax]
				cmp ebx, [edi+structRowSize+eax]

				jb changeScoreT
				jmp NoChangeScoreT
				changeScoreT :
					push ecx
					mov ecx, StructRowsize
					mov eax, 0
					ChangeScoreSubT:						
						mov bl, [edi+eax]
						mov dl, byte ptr [edi+structrowSize+eax]
						xchg bl, dl
						mov [edi+eax], bl
						mov byte ptr [edi+structRowSize+eax],dl
						inc eax
						loop ChangeScoreSubT					
				pop ecx
				jmp NoChangeScoreT
	
				NoChangeScoreT:
					add edi, StructRowSize
					pop eax

				loop SortIntSubT
			pop ecx
			Loop BubbleIntT		
		ret
Sorting Endp

END main