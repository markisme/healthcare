TITLE MASM Template						(main.asm)

; Description:
; 
; Revision date:

INCLUDE Irvine32.inc
INCLUDE student.inc

StudentInfo struct
	id		byte "000000000"
	name	byte "      "
	point	byte 0
StudentInfo ends

.data
studentData StudentInfo 100 dup(<>)	;## 100개 구조체 선언

.code
main PROC
	call Clrscr

	mov	 edx,OFFSET myMessage
	call WriteString

	exit
main ENDP

END main