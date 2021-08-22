;file ps.as
;sample source code

.extern wNumber
MAIN: add $3,$5,$9
LOOP: ori $9,-5,$2
la val1
jmp Next
Next: move $20,$4
bgt $4,$2,END
sw $0,4,$10
bne $31,$9,LOOP
call val1
jmp $4
la wNumber
.extern val1
END: stop