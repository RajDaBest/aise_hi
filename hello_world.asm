section .data
    hello_msg db 'Hello, World!', 0xA  
    hello_len equ $ - hello_msg        

section .text
    global _start                      

_start:
    mov rax, 1     ; system call number for write (file_descriptor ,buffer, length)                    
    mov rdi, 1     ; file descriptor no for stdout 
    mov rsi, hello_msg                 
    mov rdx, hello_len                 
    syscall                            

    mov rax, 60  ;  system call number for exit (RETURN_VALUE)                    
    xor edi, edi                      
    syscall         
           
