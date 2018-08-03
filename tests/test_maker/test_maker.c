#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

typedef uint8_t byte_t;

typedef struct
{
    uint64_t imm;
    uint32_t mode;
    uint8_t  reg;
    uint8_t  idx;
    uint8_t  scale;

} operand_t;

typedef struct
{
    operand_t operands[2];
    uint32_t  opcode;
    uint32_t  size;

} instruction_t;

int main(int argc, char* argv[])
{
    assert(argc == 2);

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
        printf("size: ");
        scanf("%d", &text[i].size);
        printf("\n");

        for (int e = 0; e < 2; e++)
        {
            printf("op%d: \n", e);  
            printf("  type: ");		scanf("%d",  	&text[i].operands[e].mode);
            printf("  imm: ");    	scanf("%ld", 	&text[i].operands[e].imm);
            printf("  reg: ");		scanf("%hhd", 	&text[i].operands[e].reg);
            printf("  idx: ");		scanf("%hhd", 	&text[i].operands[e].idx);
            printf("  scale: ");	scanf("%hhd", 	&text[i].operands[e].scale);
            printf("\n");
        }

        system("clear");
    }

    FILE* file = NULL;
    printf("opening %s\n", argv[1]);
    file = fopen(argv[1], "wb");

    assert(file);
    printf("%s opened\n", argv[1]);

    fwrite(&data_size, 8, 1, file);
    printf("data_size written\n");

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