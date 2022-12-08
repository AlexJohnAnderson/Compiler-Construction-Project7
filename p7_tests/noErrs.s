.globl main
glb_main: .quad 0 
.align 8
main: pushq %rbp
movq %rsp, %rbp
addq $16, %rbp
subq $32, %rsp
#Fn body main
#            [a] := 2
movq  $2, %rax
movq %rax, -24(%rbp)
#            [b] := 2
movq  $2, %rax
movq %rax, -32(%rbp)
#            [tmp0] := [a] ADD64 5
movq -24(%rbp), %rax
movq  $5, %rbx
addq %rax, %rbx
movq %rbx, -16(%rbp)
#            [a] := [tmp0]
movq -16(%rbp), %rax
movq %rax, -24(%rbp)
#            REPORT [a]
movq -24(%rbp), %rdi
callq printInt
#Fn epilogue main
lbl_0: addq $32, %rsp
popq %rbp
retq
