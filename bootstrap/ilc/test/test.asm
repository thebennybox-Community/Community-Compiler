global main
main:
extern glFlush
extern glEnd
extern glVertex2f
extern glVertex2f
extern glVertex2f
extern glBegin
extern glColor3f
extern glClear
extern glutMainLoop
extern glutDisplayFunc
extern glClearColor
extern glutCreateWindow
extern glutInitWindowSize
extern glutInitDisplayMode
extern glutInit
extern putchar
jmp start
global fntest
fntest:
push Ebp
mov Ebp, Esp
sub Esp, 16
push 41
call princhar
leave
ret
global princhar
princhar:
push Ebp
mov Ebp, Esp
sub Esp, 16
mov Eax, [Ebp+8]
push Eax
call putchar
add Esp, 4
push Eax
leave
ret
global start
start:
push Ebp
mov Ebp, Esp
sub Esp, 16
mov Eax, [Ebp+12]
push Eax
lea Eax, [Ebp+8]
push Eax
call glutInit
add Esp, 8
push 0
call glutInitDisplayMode
add Esp, 4
push Eax
push 600
push 800
call glutInitWindowSize
add Esp, 8
push Eax
mov Eax, str1
push Eax
call glutCreateWindow
add Esp, 4
push Eax
push __float32__( 0.00000000 )
push __float32__( 0.00000000 )
push __float32__( 0.00000000 )
push __float32__( 0.00000000 )
call glClearColor
push Eax
push display
call glutDisplayFunc
push Eax
call glutMainLoop
push Eax
leave
ret
global display
display:
push Ebp
mov Ebp, Esp
sub Esp, 16
push 16384
call glClear
add Esp, 4
push Eax
push __float32__( 1.00000000 )
push __float32__( 1.00000000 )
push __float32__( 1.00000000 )
call glColor3f
add Esp, 12
push Eax
push 4
call glBegin
push Eax
push __float32__( -0.50000000 )
push __float32__( -0.50000000 )
call glVertex2f
push Eax
push __float32__( -0.50000000 )
push __float32__( 0.50000000 )
call glVertex2f
push Eax
push __float32__( 0.50000000 )
push __float32__( 0.00000000 )
call glVertex2f
push Eax
call glEnd
push Eax
call glFlush
push Eax
leave
ret
section .data
str1 db 'Test',0
