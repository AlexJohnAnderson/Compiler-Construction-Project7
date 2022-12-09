.globl main 
.data 
str_0:	.asciz "hello world";
glb_a: .quad 0 
glb_b: .quad 0 
glb_c: .quad 0 
glb_d: .quad 0 
glb_e: .quad 0 
glb_f: .quad 0 
glb_g: .quad 0 
glb_i: .quad 0 
glb_fun: .quad 0 
glb_s: .quad 0 
glb_v: .quad 0 
glb_main: .quad 0 
.align 8
.text
fun_fun: pushq %rbp
movq %rsp, %rbp
addq $16, %rbp
subq $16, %rsp
#Fn body fun
#            getarg 1 [a]
#            getarg 2 [k]
#            [a] := 5
movq  $5, %rax
movq %rax, -24
#            [k] := 1
movq  $1, %rax
movq %rax, -32(%rbp)
#            IFZ [k] GOTO lbl_1
movq -32(%rbp), %rax
cmpq $0, %rax
je lbl_1
#            [a] := [a] ADD64 1
movq -24, %rax
movq  $1, %rbx
addq %rbx, %rax
movq %rax, -24
#            goto lbl_2
jmp lbl_2
lbl_1: #lbl_1:      nop
nop
#            [a] := [a] SUB64 1
movq -24, %rax
movq  $1, %rbx
subq %rbx, %rax
movq %rax, -24
lbl_2: #lbl_2:      nop
nop
#            setret [a]
movq -24, %rax
#            goto lbl_0
jmp lbl_0
#Fn epilogue fun
lbl_0: addq $16, %rsp
popq %rbp
retq

fun_s: pushq %rbp
movq %rsp, %rbp
addq $16, %rbp
subq $0, %rsp
#Fn body s
#Fn epilogue s
lbl_3: addq $0, %rsp
popq %rbp
retq

fun_v: pushq %rbp
movq %rsp, %rbp
addq $16, %rbp
subq $64, %rsp
#Fn body v
#            getarg 1 [a]
#            getarg 2 [b]
#            getarg 3 [c]
#            getarg 4 [d]
#            getarg 5 [e]
#            getarg 6 [f]
#            getarg 7 [g]
#            getarg 8 [i]
#            setret 99
movq  $99, %rax
#            goto lbl_4
jmp lbl_4
#Fn epilogue v
lbl_4: addq $64, %rsp
popq %rbp
retq

main: pushq %rbp
movq %rsp, %rbp
addq $16, %rbp
subq $0, %rsp
#Fn body main
#            REPORT str_0
movq  $str_0, %rdi
callq printString
callq printInt
#Fn epilogue main
lbl_5: addq $0, %rsp
popq %rbp
retq

