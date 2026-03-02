.data
array1: .dword 0x0000000100000001,0
array2: .dword 0x0000000100000001,7

.text
_start:
    la x10, array1     # base 0x1000 (array1)
    la x20, array2      # base 0x2000 (array2)
    addi x21, x0, 4
    slli x21, x21, 20
    addi x4, x0, 4
    slli x4, x4, 12
    meta x21, x10, x0
lwu x1, 68(x21)
sparse_mul x4, x10, x20, x21
end: addi x0, x0, 0
