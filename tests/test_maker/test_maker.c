#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

typedef uint8_t byte_t;

typedef struct
{
    // register encoding:
    // 16 bit scale, 16 bit offset, 16 bit idx, 16 bit reg

    uint32_t    opcode;
    uint32_t    op_modes;
    uint64_t    operands[2];

} instruction_t;

typedef enum
{
    AM_NONE,
    AM_REG,
    AM_REG_DEREF,
    AM_IMM,
    AM_IMM_DEREF

} addr_mode_t;

int main(int argc, char* argv[])
{
    assert(argc == 2);

    size_t uninit_data_size;
    printf("sizeof(.uninit_data): ");
    scanf("%ld", &uninit_data_size);

    size_t data_size;
    printf("sizeof(.data): ");
    scanf("%ld", &data_size);

    byte_t* data = malloc(data_size);

    printf(".data: \n");
    for (size_t i = 0; i < data_size; i++)
    {
        printf("%lx: ", i);
        scanf("%hhd", &data[i]);
    }
    printf("\n");

    size_t text_size;
    printf("number of instructions: ");
    scanf("%ld", &text_size);

    instruction_t* text = malloc(text_size * sizeof(instruction_t));

    printf(".text: \n");
    for (size_t i = 0; i < text_size; i++)
    {
        printf("opcode: ");
        scanf("%d", &text[i].opcode);
        printf("\n");

        uint32_t type[2] = {0, 0};

        for (int e = 0; e < 2; e++)
        {
            printf("op%d: \n", e);  
            
            printf("  opmode: ");
            scanf("%d", &type[e]);

            if (type[e] == AM_IMM || type[e] == AM_IMM_DEREF)
            {
                printf("  imm: ");
                scanf("%lu", &text[i].operands[e]);
            }
            else if (type[e] == AM_REG)
            {
                printf("  reg: ");
                scanf("%lu", &text[i].operands[e]);
            }
            else if (type[e] == AM_REG_DEREF)
            {
                printf("  reg: ");
                uint64_t reg = 0;
                scanf("%lu", &reg);

                printf("  idx: ");
                uint64_t idx = 0;
                scanf("%lu", &idx);

                printf("  scale: ");
                uint64_t scale = 0;
                scanf("%lu", &scale);

                printf("  off:  ");
                uint64_t off = 0;
                scanf("%lu", &off);

                uint64_t operand = reg 
                                 | (idx << 16)
                                 | (scale << 32)
                                 | (off << 48);

                text[i].operands[e] = operand;
            }
            else if (type[e] == AM_NONE)
                text[i].operands[e] = (uint64_t)0;

            printf("\n");
        }
        text[i].op_modes = (type[1] << 16) | type[0];

        system("clear");
    }

    FILE* file = NULL;
    printf("opening %s\n", argv[1]);
    file = fopen(argv[1], "wb");

    assert(file);
    printf("%s opened\n", argv[1]);

    fwrite(&uninit_data_size, 8, 1, file);
    fwrite(&data_size, 8, 1, file);
    fwrite(&text_size, 8, 1, file);
    printf("size info written\n");

    fwrite(data, data_size, 1, file);
    printf("data written\n");

    fwrite(text, sizeof(instruction_t), text_size, file);
    printf("text written\n");

    fclose(file);
    printf("%s closed\n", argv[1]);

    free(text);
    free(data);

    return 0;
}
