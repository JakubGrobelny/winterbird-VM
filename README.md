# WBVM - Winterbird Virtual Machine

##What is it?
wbvm is an interpreter for IR bytecode produced by Winterbird language compiler. It can (will in the future) also assemble, link and run programs written in wbASM (Winterbird assembler).

##What is Winterbird?
Winterbird is a programming language, which is still in development.

##Technical data:
- 128 64-bit registers
- flexible stack size (16 MB by default)
- float/integer/unsigned/pointer data types

##Operand modes:
- register: $reg
- register dereference : (\$reg + $idx * scale + offset)
- 64-bit value: value
- 64-bit address dereference: *value

##Instruction set:
(some instructions have many variants that take operand sizes into consideration [eg. mov64 and mov32]. All of them operate on signed integers unless stated otherwise).

    name operands - description

    mov      X Y   - copies the contents of Y to X. Has size variants mem64, 
                        mem32, mem16, mem8.
    alloc    X Y   - dynamically allocates memory of size Y and stores 
                        the pointer in X.
    free     X     - frees dynamically allocated memory 
                        from pointer in X 
    swap     X Y   - swaps contents of X and Y
    laddr    X Y   - moves the address of label Y to X
    fopen    X Y   - opens file with name pointed by Y and stores file 
                        pointer in X. Sets test flag if failed.
    fclose   X     - closes file pointed by X. Sets test 
                     flag if failed.
    freadb   X Y   - reads byte from file pointed by Y and stores
                        it in X. Sets test flag if failed.
    fwriteb  X Y   - writes byte from file Y into X. Sets 
                        test flag if failed.
    memprep  X Y   - prepares pointer in X for memset/memcpy of Y bytes.
    memcpy   X     - copies Z bytes from X to A, where Z and A come from memprep A Z.
    memset   X     - sets Z bytes from A to X, where A and Z come from memprep A Z.
    push     X     - pushes data from X to the stack. Has size variants push64,
                        push32, push16, push8.
    pop      X     - pops data from the stack to X. Has size variants pop64,
                        pop32, pop16, pop8
    ftoi     X Y   - casts float from Y to integer and stores it in X.
    itof     X Y   - casts integer from Y to float and stores it in X.
    add      X Y   - sets X to X + Y.
    sub      X Y   - sets X to X - Y.
    neg      X Y   - sets X to -Y.
    mul      X Y   - sets X to X * Y. (signed)
    div      X Y   - sets X to X / Y. (signed)
    mod      X Y   - sets X to X mod Y. (signed)
    ror      X Y   - rotates X by Y bits to the right.
    rol      X Y   - rotates X by Y bits to the left.
    shr      X Y   - arithmetic shift of X to the right by Y bits.
    shl      X Y   - arithmetic shift of X to the left by Y bits.
    umul     X Y   - sets X to X * Y. (unsigned)
    udiv     X Y   - sets X to X / Y. (unsigned)
    umod     X Y   - sets X to X mod Y. (unsigned)
    ushr     X Y   - logical shift of X to the right by Y bits.
    ushl     X Y   - logical shift of X to the left by Y bits.
    fadd     X Y   - sets X to X + Y. (float64)
    fsub     X Y   - sets X to X - Y. (float64)
    fneg     X Y   - sets X to -Y. (float64)
    fmul     X Y   - sets X to X * Y. (float64)
    fdiv     X Y   - sets X to X / Y. (float64)
    f32add   X Y   - sets X to X + Y. (float32)
    f32sub   X Y   - sets X to X - Y. (floa32t)
    f32neg   X Y   - sets X to -Y. (float32)
    f32mul   X Y   - sets X to X * Y. (float32)
    f32div   X Y   - sets X to X / Y. (float32)
    and      X Y   - sets X to bitwise X and Y.
    or       X Y   - sets X to bitwise X or Y. 
    xor      X Y   - sets X to bitwise X xor Y.
    not      X Y   - sets X to bitwise not Y.
    bset     X Y   - sets the Y-th bit of X.
    bclr     X Y   - clears the Y-th bit of X.
    dec      X     - decrements X by one.
    inc      X     - increments X by one.
    ntest          - sets test flag to logical negation of itself.
    stest    X     - sets test flag to X.
    gtest    X     - sets X to test flag value.
    cmpz     X     - sets test flag if X is zero.
    cmpg     X Y   - sets test flag if X > Y
    cmpl     X Y   - sets test flag if X < Y
    cmpeq    X Y   - sets test flag if X and Y are equal.
    cmpfz    X     - sets test flag if X is zero. (float)
    cmpfg    X Y   - sets test flag if X > Y. (float)
    cmpfl    X Y   - sets test flag if X < Y. (float)
    cmpfi    X     - sets test flag if X is infinity.
    cmpfn    X     - sets test flag if X is NaN.
    cmpf32z    X   - sets test flag if X is zero. (float32)
    cmpf32g    X Y - sets test flag if X > Y. (float32)
    cmpf32l    X Y - sets test flag if X < Y. (float32)
    cmpf32i    X   - sets test flag if X is infinity. (float32)
    cmpf32n    X   - sets test flag if X is NaN. (float32)

    cmpul    X Y   - sets test flag if X < Y. (unsigned)
    cmpug    X Y   - sets test flag if X > Y. (unsigned)
    jmp      X     - sets program counter to X.
    jif      X     - sets program counter to X if test flag is set.
    call     X     - pushes current program counter to the stack and
                        sets program counter to X.
    ret            - pops a 64 bitvalue from the stack and sets 
                        program counter to the received value.
    halt     X     - exits the program with code X.
    debug    X     - prints stack/heap/register/.data/.text to stdout
                        based on the value of X.
                            registers if X & 1 != 0
                            stack     if X & 2 != 0
                            data      if X & 4 != 0
                            text      if X & 8 != 0
                            heap      if X & 16 != 0
    stacks   X     - sets stack pointer to X
    stackg   X     - sets X to the stack pointer
    getch    X     - sets X to value read from stdin
    putch    X Y   - prints X to
                        stdout if Y == 0
                        stderr if Y == 1

No size: