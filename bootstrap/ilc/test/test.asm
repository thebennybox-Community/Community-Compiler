global add
add:
push ebp
mov ebp, esp
sub esp, 16
mov Eax, [Ebp+8]
mov [Ebp-4], Eax
mov Eax, [Ebp+12]
mov [Ebp-8], Eax
mov Eax, [Ebp-4]
push Eax
mov Eax, [Ebp-8]
push Eax
pop Ecx
pop Eax
add Eax, Ecx
push Eax
leave
ret
global test
test:
push ebp
mov ebp, esp
sub esp, 16
mov eax, str1
push Eax
extern printf
mov edx, esp
call printf
sub esp, edx
leave
ret
section .data
str1 db 'hello world!',0
