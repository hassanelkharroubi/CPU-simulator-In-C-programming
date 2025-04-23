


int search_and_replace(char **str, HashMap *values);
char *trim(char *str) ;
int resolve_constants(ParserResult *result) ;
void allocate_code_segment(CPU *cpu, Instruction **code_instructions, int code_count) ;
int handle_instruction(CPU *cpu, Instruction *instr, void *src, void *dest) ;
int execute_instruction(CPU *cpu, Instruction *instr) ;
Instruction *fetch_next_instruction(CPU *cpu);
int run_program(CPU *cpu) ;