.globl main
glb_main: .quad 0 
.align 8
main: pushq %rbp
movq %rsp, %rbp
addq $16, %rbp
subq $48, %rsp
#Fn body main
#            [a] := 2
movq  $2, %rax
movq %rax, -32(%rbp)
#            [b] := 2
movq  $2, %rax
movq %rax, -40(%rbp)
#            [tmp0] := [a] ADD64 5
movq -32(%rbp), %rax
movq  $5, %rbx
addq %rbx, %rax
movq %rax, -16(%rbp)
#            [a] := [tmp0]
movq -16(%rbp), %rax
movq %rax, -32(%rbp)
#            [tmp1] := [b] SUB64 2
movq -40(%rbp), %rax
movq  $2, %rbx
subq %rbx, %rax
movq %rax, -24(%rbp)
#            [b] := [tmp1]
movq -24(%rbp), %rax
movq %rax, -40(%rbp)
#            REPORT [a]
movq -32(%rbp), %rdi
callq printInt
#            REPORT [b]
movq -40(%rbp), %rdi
callq printInt
#Fn epilogue main
lbl_0: addq $48, %rsp
popq %rbp
retq
