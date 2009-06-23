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
inputmenu			byte	"������ �Է�", 0
inputname			byte	"�̸� : ",0
inputPoint			byte	"���� : ",0
tempinputname		byte	10 DUP(?)
tempinputpoint		Dword	0

studentData StudentInfo 100 dup(<>)
dSize Dword 0

.code
;----------------------------------------------------------
; �ϴ��� ������ ���� �ǵ���
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
; �л� ��� ���� ���ν���
;----------------------------------------------------------

;----------------------------------------------------------
; ��� ���� ���ν���
;----------------------------------------------------------

;----------------------------------------------------------
main PROC
	call Clrscr
	
	;�Է� �޼��� ���
	mov edx, offset inputmenu
	call writestring

	mov ecx,2		;���� ī���� ����
	mov dSize, 0	;������ ����
	
inputdata:	push ecx
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
	mov eax,14
	mul dSize
	
	mov edi, offset studentdata + 14
	add edi, eax
	mov eax, tempinputpoint
	mov (StudentInfo ptr [edi]).point, eax
	;mov esi, offset tempinputName
	;invoke Str_copy, esi, edi
		
	inc dSize
	
	;�̸��� �ΰ��̰� ������ -1 �̸� �Է� ��
	
	;�ƴϸ� ��� ����
	pop ecx
	loop inputdata
	
	;������ �Է� Ȯ��
	
	;����
	invoke BubbleSort, ADDR studentData, 100
	
	;������ �Է� Ȯ��
	
	;�л�(ǥ������) ���

	exit
main ENDP
;----------------------------------------------------------

END main