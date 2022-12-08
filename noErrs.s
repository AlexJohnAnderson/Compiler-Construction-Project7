.data
.globl main
glb_a: .quad 0 
glb_main: .quad 0 
.align 8
main: pushq %rbp
movq %rsp, %rbp
addq $16, %rbp
subq $0, %rsp
#Fn body main
#Fn epilogue main
lbl_0: addq $0, %rsp
popq %rbp
retq
