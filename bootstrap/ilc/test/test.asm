global main
main:
jmp start
global fntest
fntest:
push Ebp
mov Ebp, Esp
sub Esp, 16
push 41
push princhar
pop Eax
call Eax
add Esp, 0
leave
ret
global princhar
princhar:
push Ebp
mov Ebp, Esp
sub Esp, 16
mov Eax, [Ebp+8]
push Eax
extern putchar
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
extern glutInit
call glutInit
add Esp, 8
push 0
extern glutInitDisplayMode
call glutInitDisplayMode
add Esp, 4
push Eax
push 600
push 800
extern glutInitWindowSize
call glutInitWindowSize
add Esp, 8
push Eax
mov Eax, str1
push Eax
extern glutCreateWindow
call glutCreateWindow
add Esp, 4
push Eax
push __float32__( 0.00000000 )
push __float32__( 0.00000000 )
push __float32__( 0.00000000 )
push __float32__( 0.00000000 )
extern glClearColor
call glClearColor
add Esp, 0
push Eax
push display
extern glutDisplayFunc
call glutDisplayFunc
add Esp, 0
push Eax
extern glutMainLoop
call glutMainLoop
add Esp, 0
push Eax
push 0
leave
ret
global display
display:
push Ebp
mov Ebp, Esp
sub Esp, 16
push 16384
extern glClear
call glClear
add Esp, 4
push Eax
push __float32__( 1.00000000 )
push __float32__( 1.00000000 )
push __float32__( 1.00000000 )
extern glColor3f
call glColor3f
add Esp, 12
push Eax
push 4
extern glBegin
call glBegin
add Esp, 0
push Eax
push __float32__( -0.50000000 )
push __float32__( -0.50000000 )
extern glVertex2f
call glVertex2f
add Esp, 0
push Eax
push __float32__( -0.50000000 )
push __float32__( 0.50000000 )
extern glVertex2f
call glVertex2f
add Esp, 0
push Eax
push __float32__( 0.50000000 )
push __float32__( 0.00000000 )
extern glVertex2f
call glVertex2f
add Esp, 0
push Eax
extern glEnd
call glEnd
add Esp, 0
push Eax
extern glFlush
call glFlush
add Esp, 0
push Eax
leave
ret
section .data
str1 db 'push.str "Test',0
