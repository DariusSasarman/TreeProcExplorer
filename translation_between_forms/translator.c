#include "translator.h"

tree * translate(process * target)
{
    /// If null then there's nothing to translate
    if(target == NULL) return NULL;

    /// O(n) translation to intermediate representation
    intermediate_representation ** rep = translateOneToTwo(target);
    
    /// If the representation failed, then stop
    if(!rep)return NULL;

    /// O(n) translation to final representation
    node * virtual_root = translateTwoToThree(hashGet(0,rep));
    /// Intermediate representation isn't needed anymore
    deleteIntermediateRepresentation(rep);
    /// Regardless if successfull, or not 
    if(!virtual_root)
    {
        return NULL;
    }

    /// Building up the final tree
    tree * ret = (tree*)malloc(sizeof(tree));
    ret->virtual_root = virtual_root;
    
    /// Getting the height of it
    ret->height = 1;
    getHeight(virtual_root,1,&ret->height);

    /// Getting the widths for each row
    ret->widths = (int*)calloc(ret->height, sizeof(int));
    getWidth(ret->widths,0,virtual_root);

    /// Getting the screen coordinates
    setCoordinates(ret);
    return ret;
}

/// Used for deleting the "intermediate_representation" data structure
void deleteIntermediateRepresentation(intermediate_representation ** target)
{
    for(int i = 0; i < TABLE_SIZE; i++)
    {
        if(target[i] != NULL)
        {
            while(target[i]->children != NULL)
            {
                ///Pop does free the stack nodes
                target[i]->children = stack_pop(target[i]->children);
            }

            free(target[i]);
        }
    }
    free(target);
}

/// Used for translating the "parent array" representation
/// To the intermediate one, where each node holds references to all children
intermediate_representation ** translateOneToTwo(process * target)
{
    if(target == NULL) return NULL;

    intermediate_representation **rep = (intermediate_representation**)calloc(TABLE_SIZE, sizeof(intermediate_representation*));
    /// Adding virtual Root 0
    int key = hashKey(0,rep);

    /// This sets up the virtual root's string fields
    rep[key] = (intermediate_representation*)malloc(sizeof(intermediate_representation));
    rep[key]->id = 0;

    const char * name = "Virtual Root";
    strncpy(rep[key]->name,name,STRING_LEN);
    rep[key]->name[STRING_LEN - 1] = '\0';

    const char * path = "/proc/";
    strncpy(rep[key]->path,path,STRING_LEN);
    rep[key]->path[STRING_LEN - 1] = '\0';

    rep[key]->children = NULL;

    /// Putting all elements in the "rep" map
    int i = 0;
    while(target[i].pid != -1)
    {
        /// First we find the place in the map
        key = hashKey(target[i].pid,rep);

        if(key == -1)
        {
            deleteIntermediateRepresentation(rep);
            return NULL;
        }

        /// Field initializations
        rep[key] = (intermediate_representation*)malloc(sizeof(intermediate_representation));
        rep[key]->id = target[i].pid;

        strncpy(rep[key]->name,target[i].name,STRING_LEN);
        rep[key]->name[STRING_LEN - 1] = '\0';

        strncpy(rep[key]->path,target[i].path,STRING_LEN);
        rep[key]->path[STRING_LEN - 1] = '\0';

        rep[key]->children = NULL;

        i++;
    }
    /// Establishing parent-child relationships
    i=0;
    while(target[i].pid != -1)
    {
        intermediate_representation * parent = hashGet(target[i].ppid,rep);
        if(parent == NULL)
        {
            deleteIntermediateRepresentation(rep);
            return NULL;
        }
        /// Decided to use a stack because we don't need to access 
        /// Said nodes only on the build up of the final, third representation
        parent->children = stack_push(parent->children,hashGet(target[i].pid,rep));
        i++;
    }
    /// We now have all nodes stored in intermediate_representation
    /// And we return what we just built up.
    return rep;
}

/// Used for obtaining the final multi-way tree
node * translateTwoToThree(intermediate_representation * target)
{
    if(target == NULL)
    {
        return NULL;
    }
    node * ret = (node*)calloc(1,sizeof(node));
    
    ///Transfer Data from target to node 
    ret->id = target->id;
    strncpy(ret->name, target->name,STRING_LEN);
    ret->name[STRING_LEN - 1] = '\0';
    strncpy(ret->path, target->path,STRING_LEN);
    ret->path[STRING_LEN - 1] = '\0';
    /// Drawing elements
    ret->x = -1;
    ret->y = -1;
    ret->radius = -1;
    ///Initiate first child 
    ret->first_child = translateTwoToThree(stack_peek(target->children));
    if(ret->first_child != NULL)
    {
        ret->first_child->parent = ret;
        node * bro = ret->first_child;
        stack_node * iter = target->children;
        
        ///Go to next child 
        iter = iter->next;

        /// And keep adding siblings to said child
        while(iter != NULL && bro != NULL)
        {
            ///Initiate said sibling
            bro->next_sibling = translateTwoToThree(stack_peek(iter));
            ///Establish backwards connection to parent
            bro = bro->next_sibling;
            if(bro != NULL) bro->parent = ret;
            ///Keep going until there are no children left
            iter = iter->next;
        }
    }
    return ret;
}

/// Used in generation of hashkeys
int hash(int target, int attempt)
{
    return (target + attempt*2 + attempt*attempt*3)%TABLE_SIZE;
}

/// Returns the hashkey in the "rep" table 
int hashKey(int pid, intermediate_representation *rep[TABLE_SIZE])
{
    for(int attempt = 0; attempt < TABLE_SIZE; attempt++)
    {
        int key = hash(pid,attempt);
        if(rep[key]!= NULL) continue;
        return key;
    }
    return -1;
}

/// Returns the coressponding element from the "rep" table
intermediate_representation * hashGet(int pid, intermediate_representation *rep[TABLE_SIZE])
{
    for(int attempt = 0; attempt < TABLE_SIZE; attempt++)
    {
        int key = hash(pid,attempt);
        if(rep[key]==NULL) break;
        if(rep[key]->id != pid) continue;
        return rep[key];
    }
    return NULL;
}

/// Working functions for the stack implementation

/// works by target = stack_push(target, added);
stack_node * stack_push ( stack_node * target, intermediate_representation * added)
{
    stack_node * link = (stack_node*)malloc(sizeof(stack_node));
    link -> next = target;
    link -> rep  = added;
    return link;
}


/// works by target = stack_pop(target, added);
stack_node * stack_pop ( stack_node * target)
{
    if(target == NULL)
    {
        return NULL;
    }
    stack_node * aux = target;
    target = target->next;
    free(aux);
    return target;
}

/// this just returns the top element available
intermediate_representation * stack_peek ( stack_node * target)
{
    if(target == NULL)
    {
        return NULL;
    }
    return target->rep;
}