global main
main:
jmp start
global fntest
fntest:
push ebp
mov ebp, esp
sub esp, 16
push 41
push princhar
pop eax
call eax
add esp, 0
leave
ret
global princhar
princhar:
push ebp
mov ebp, esp
sub esp, 16
mov Eax, [Ebp+8]
push Eax
extern putchar
call putchar
add esp, 4
push Eax
leave
ret
global start
start:
push ebp
mov ebp, esp
sub esp, 16
mov Eax, [Ebp+12]
push Eax
lea eax, [ebp+8]
push Eax
extern glutInit
call glutInit
add esp, 8
push 0
extern glutInitDisplayMode
call glutInitDisplayMode
add esp, 4
push Eax
push 600
push 800
extern glutInitWindowSize
call glutInitWindowSize
add esp, 8
push Eax
mov eax, str1
push Eax
extern glutCreateWindow
call glutCreateWindow
add esp, 4
push Eax
push __float32__( 0.00000000 )
push __float32__( 0.00000000 )
push __float32__( 0.00000000 )
push __float32__( 0.00000000 )
extern glClearColor
call glClearColor
add esp, 0
push Eax
push display
extern glutDisplayFunc
call glutDisplayFunc
add esp, 0
push Eax
extern glutMainLoop
call glutMainLoop
add esp, 0
push Eax
push 0
leave
ret
global display
display:
push ebp
mov ebp, esp
sub esp, 16
push 16384
extern glClear
call glClear
add esp, 4
push Eax
push __float32__( 1.00000000 )
push __float32__( 1.00000000 )
push __float32__( 1.00000000 )
extern glColor3f
call glColor3f
add esp, 12
push Eax
push 4
extern glBegin
call glBegin
add esp, 0
push Eax
push __float32__( -0.50000000 )
push __float32__( -0.50000000 )
extern glVertex2f
call glVertex2f
add esp, 0
push Eax
push __float32__( -0.50000000 )
push __float32__( 0.50000000 )
extern glVertex2f
call glVertex2f
add esp, 0
push Eax
push __float32__( 0.50000000 )
push __float32__( 0.00000000 )
extern glVertex2f
call glVertex2f
add esp, 0
push Eax
extern glEnd
call glEnd
add esp, 0
push Eax
extern glFlush
call glFlush
add esp, 0
push Eax
leave
ret
section .data
str1 db 'Test',0
