TITLE �����������α׷�				(main.asm)
;������Ŵ� ��ǻ���а� 2000160005 �ѱ���

INCLUDE Irvine32.inc

StudentInfo struct
	stuName byte 8 DUP(?)
	point	byte 0
StudentInfo ends

.data
inputText			byte	"������ �Է�", 0
printText			byte	"������ ���", 0
nameText			byte	"�̸� : ",0
pointText			byte	"���� : ",0
deviationText		byte	"ǥ������ : ",0
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
;������ �Է� ���ν���
InputData PROC USES eax ecx edi edx esi
	;�Է� �޼��� ���
	mov edx, offset inputText
	call writestring
	call crlf
	
	;������ ī����
	mov ecx, 100
	dec ecx	
	
	;�ʱ� ��
	mov edi, offset studentdata
	mov dataSize, 0
	
INPUT:	
	push ecx
	
	;�̸� �Է� �޼����� ���� ���� �о��
	mov edx, offset nameText
	call writestring
	mov edx, offset tempinputName
	mov ecx, (Sizeof tempinputName)
	call readstring

INPUTPOINT:
	;���� �Է� �޼����� ���� ���� �о��
	mov edx, offset pointText
	call writestring	
	call readint
	mov tempinputpoint, eax
	
	;������ -1 �̸� �Է� ��
	cmp eax, -1
	jne SAVE
	pop ecx
	ret

SAVE:
	;������ 100���� ũ�� �ٽ� �Է�
	cmp tempinputpoint, 100 		;tempinputpoint > 100 ?
	ja INPUTPOINT

	inc dataSize
	
	;�̸� ����
	mov esi, offset tempinputName
	invoke Str_copy, esi, edi
	
	;���� ����
	mov eax, tempinputpoint
	mov (StudentInfo ptr [edi]).point, al
	
	;�ؽ�Ʈ
	add edi, structsize

	;�ƴϸ� ��� ����
	pop ecx
	loop INPUT

	ret

InputData ENDP
;----------------------------------------------------------
	
;----------------------------------------------------------
;������ ��� ���ν���
PrintData PROC USES eax ecx edi edx
	;��� �޼��� ���
	mov edx, offset inputText
	call writestring
	call crlf
	
	;������ ī����
	mov ecx, dataSize
	;dec ecx	
	
	;�ʱ� ��
	mov edi, offset studentData
	
	;��� ����
Print:	
	push ecx
	
	;�̸� �޼����� ���
	mov edx, offset nameText
	call writestring
	mov edx, edi
	call writestring
	mov edx, offset spaceText
	call writestring
	
	;���� �޽����� ���
	mov edx, offset pointText
	call writestring
	mov al, [edi+pointpos]
	call writeint
	call crlf
	
	;�ؽ�Ʈ
	add edi, structsize
	
	pop ecx		
	Loop Print
	
	;ǥ������ �޽����� ���
	mov edx, offset deviationText
	call writestring	
	mov eax, deviationValue
	call writeint
	call crlf	
	
	ret
	
PrintData ENDP
;----------------------------------------------------------

;----------------------------------------------------------
;���� ��Ʈ ���ν���
BubbleSort PROC USES eax ecx edi

SortIntPart:
	;������ ī����
	mov ecx, dataSize
	;dec ecx		

	;���� ���� ����
StartSort:
	push ecx
	mov edi, offset studentData

Compare:
	;���� 2�� ����
	mov bl, [edi+pointpos]
	mov dl, byte ptr [edi+structsize+pointpos]
			
	;��
	cmp bl, dl
	jge noChange

	;������ȯ
	xchg bl, dl
	mov [edi+pointpos], bl
	mov byte ptr [edi+structsize+pointpos],dl
	
	;�̸���ȯ
	push eax
	INVOKE Str_copy, ADDR (Studentinfo ptr [edi]).stuName, ADDR tempinputname
	INVOKE Str_copy, ADDR (Studentinfo ptr [edi+structsize]).stuName, ADDR (Studentinfo ptr [edi]).stuName
	INVOKE Str_copy, ADDR tempinputname, ADDR (Studentinfo ptr [edi+structsize]).stuName
	pop eax
	
noChange:
	;�ؽ�Ʈ
	add edi, structsize			
	loop Compare
			
	pop ecx		
	Loop StartSort
	
	ret

BubbleSort ENDP
;----------------------------------------------------------

;----------------------------------------------------------
;�л� ��� ���ν���
Deviation PROC USES eax ecx edi edx
	;���� ����
	LOCAL SQUSUM:DWORD
	LOCAL SUM:DWORD
	LOCAL RES:DWORD
	LOCAL SQRT:DWORD
	mov SQRT, 0

	;���� ������ �� ���ϱ�
	;�ʱ� ��
	mov ecx, dataSize
	mov edi, offset studentData
	mov edx, 0

CALCULATION1:
	push ecx
		
	;��i=1,N xi2 ���
	mov al, (StudentInfo ptr [edi]).point
	mov bl, al
	mul bl
	add edx, eax
	
	pop ecx
	
	;�ؽ�Ʈ
	add edi, structsize
	loop CALCULATION1
	
	mov SQUSUM, edx
	
	;������ �� ���ϱ�
	;�ʱ� ��
	mov ecx, dataSize
	mov edi, offset studentData
	mov edx, 0

CALCULATION2:
	push ecx
		
	;��i=1,N xi ���
	mov al, (StudentInfo ptr [edi]).point
	add edx, eax
	
	pop ecx
	
	;�ؽ�Ʈ
	add edi, structsize
	loop CALCULATION2
	
	mov SUM, edx
	
CALCULATION3:
	;��= [{��i=1,N xi2 - (��i=1,N xi)2 /N} /(N-1) ] ���	
	;������ ���� ����
	mov eax, SUM
	mov ebx, eax
	mul ebx
	
	;������ ���� ������ ���
	mov ebx, dataSize
	div ebx
	
	;�߰�ȣ ���� ���
	sub SQUSUM, eax 
	
	;���ȣ ���� ���
	mov eax, SQUSUM
	mov ebx, dataSize
	sub ebx, 1
	div ebx
	
	mov RES, eax
	
	;������ ���
	;�ʱ� ��
	mov edi, 1			; n = 0
	mov edx, 0			; sigma

CALCULATION4:
	;��(2n-1)
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
	;ǥ������ ��
	mov deviationValue, edi
	
	ret
	
Deviation ENDP
;----------------------------------------------------------

;----------------------------------------------------------
;���� ���ν���
main PROC
	call Clrscr
	
	;������ �Է�
	call InputData
	
	;������ �Է� Ȯ��
	cmp dataSize, 0
	jne PROCESS
	exit

PROCESS:
	;������ ũ�� Ȯ��
	cmp dataSize, 1
	jbe PRINT		; datasize <= 1 ?
	
	;����
	call BubbleSort

	;�л� ���
	call Deviation

PRINT:	
	;������ ���
	call PrintData
	
	exit
main ENDP
;----------------------------------------------------------

END main