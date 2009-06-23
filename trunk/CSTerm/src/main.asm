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
inputmenu			byte	"������ �Է�", 0
inputname			byte	"�̸� : ",0
inputPoint			byte	"���� : ",0
tempinputname		byte	"        "
tempinputpoint		Dword	0

studentData StudentInfo 100 dup(<>)	;## 100�� ����ü ����

.code
main PROC
	call Clrscr
	
	;�Է� �޼��� ���
	mov edx, offset inputmenu
	call writestring

	mov  esi,2 ;���� ī���� ����
	
inputdata:	
	;�̸� �Է� �޼����� ���� ���� �о��
	mov edx, offset inputName
	call writestring
	mov edx, offset tempinputName
	mov ecx, (Sizeof tempinputName)
	call readstring

	;���� �Է� �޼����� ���� ���� �о��
	mov edx, offset inputPoint
	call writestring	
	call readint
	mov tempinputpoint, eax
	
	;����ü�� ����
	mov edi, offset studentdata
	mov eax, tempinputpoint
	mov (StudentInfo ptr [edi]).point, al
	mov esi, offset tempinputName
	invoke Str_copy, esi, edi
	
	;�̸��� �ΰ��̰� ������ -1 �̸� �Է� ��
	
	;�ƴϸ� ��� ����
	loop inputdata
	
	;������ �Է� Ȯ��
	
	;����
	
	;�л�(ǥ������) ���

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