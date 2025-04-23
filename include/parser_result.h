#ifndef PARSER_RESULT_H
#define PARSER_RESULT_H
#include "assembler_parser.h"
ParserResult *parse(const char *filename);
void free_parser_result(ParserResult *result) ;
#endif // PARSER_RESULT_H