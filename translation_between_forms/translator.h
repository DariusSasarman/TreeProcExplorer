#ifndef TRANSLATOR

#define TRANSLATOR
#define TABLE_SIZE 1031
#include "../process_monitor/list_processes.h"
#include "../graphics/visual_representation.h"


typedef struct intermediate_representation {
    int id;
    char name[STRING_LEN];
    char path[STRING_LEN];
    
    struct stack_node * children;

}intermediate_representation;

typedef struct stack_node{
    intermediate_representation * rep;
    struct stack_node * next;
}stack_node;

/*
#
#   The purpose of this library
#   is to use the translate function
#   to get from the 
#   parent array representation 
#   to the 
#   n-way tree representation
#
#   |   |   |   |   |   |   |
#   V   V   V   V   V   V   V
*/

tree * translate(process * target);

/*
#   A   A   A   A   A   A   A
#   |   |   |   |   |   |   |
*/


/// Helper functions 

intermediate_representation ** translateOneToTwo(process * target);
node * translateTwoToThree(intermediate_representation * target);

int hash(int target, int attempt);
int hashKey(int pid, intermediate_representation *rep[TABLE_SIZE]);
intermediate_representation * hashGet(int pid, intermediate_representation *rep[TABLE_SIZE]);

stack_node * stack_push ( stack_node * target, intermediate_representation * added);
stack_node * stack_pop ( stack_node * target);
intermediate_representation * stack_peek ( stack_node * target);

void deleteIntermediateRepresentation(intermediate_representation ** target);

#endif /* translator */