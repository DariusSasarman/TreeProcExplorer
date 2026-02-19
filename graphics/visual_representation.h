#ifndef VISUAL_REPRESENTATION

#define VISUAL_REPRESENTATION

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib_files/raylib.h"

#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 800

typedef struct node{
    /// Holded data
    int id;
    char name[32];
    char path[32];

    /// Traversal pointers
    struct node * first_child;
    struct node * next_sibling;
    struct node * parent;

    /// Drawing coordinates
    int x;
    int y;
    float radius;
}node;

typedef struct tree{
    struct node * virtual_root;
    int height;
    int *widths;
}tree;


void getHeight(node * root,int height, int * max_height);
void getWidth(int * widths, int height, node * root);
void setCoordinates(tree * tree_to_be_represented);
int isDifferent(node * root1, node * root2);
void deleteTree(tree * tree);


void handleClick(int x, int y, node * root);

void represent( tree * tree_to_be_represented);

#endif /* Visual Representation */