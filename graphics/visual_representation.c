#include "visual_representation.h"
#include "raylib_files/raylib.h"
#include <stdio.h>
#include <stdlib.h>

int isDifferent(node * root1, node * root2)
{
    if(root1 == NULL)
    {
        if(root2!= NULL)
        {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        if(root2 == NULL)
        {
            return 1;
        }
        else {
            if(root1->id != root2->id || strcmp(root1->name, root2->name)!=0 || strcmp(root1->path, root2->path) != 0)
            {
                return 1;
            }
            else {
                return 0 || 
                isDifferent(root1->first_child,root2->first_child) ||
                isDifferent(root1->next_sibling, root2->next_sibling);
            }
        }
    }
}

void deleteNode(node * n)
{
    if(n)
    {
        deleteNode(n->first_child);
        deleteNode(n->next_sibling);
        free(n);
    }
}

void deleteTree(tree * tree)
{
    deleteNode(tree->virtual_root);
    free(tree);
}

void assignCoordinates(node *n, int level, int *level_counters, int *widths, int cell_height) {
    if (!n) return;

    if (level > 0) {
        int node_spacing = 160; // Horizontal space per node
        
        // Calculate the total width occupied by all nodes at this specific level
        int total_level_width = widths[level] * node_spacing;
        
        // Calculate the starting X offset to center the row
        int x_offset = (WINDOW_WIDTH - total_level_width) / 2;

        // Position the node
        n->x = x_offset + (level_counters[level] * node_spacing) + (node_spacing / 2);
        n->y = level * cell_height + (cell_height / 2);
        
        // Radius logic
        n->radius = 50; 

        level_counters[level]++;
    }

    // Recurse
    assignCoordinates(n->first_child, level + 1, level_counters, widths, cell_height);
    assignCoordinates(n->next_sibling, level, level_counters, widths, cell_height);
}

void pullChildrenToParent(node *n) {
    if (!n || !n->first_child) return;

    // 1. Find the current center of the children group
    float first_x = n->first_child->x;
    float last_x = n->first_child->x;
    
    node *curr = n->first_child;
    while (curr->next_sibling) {
        curr = curr->next_sibling;
        last_x = curr->x;
    }
    
    float children_center_x = (first_x + last_x) / 2.0f;

    // 2. Calculate how much we need to shift them to align with parent
    float shift_x = n->x - children_center_x;

    // 3. Apply the shift to all immediate children
    // (Note: This propagates down because we recurse)
    curr = n->first_child;
    while (curr) {
        curr->x += shift_x;
        curr = curr->next_sibling;
    }

    // 4. Recurse down the tree to pull the next generation
    curr = n->first_child;
    while (curr) {
        pullChildrenToParent(curr);
        curr = curr->next_sibling;
    }
}

void setCoordinates(tree *tree_to_be_represented) {
    if (!tree_to_be_represented || !tree_to_be_represented->virtual_root) return;

    // exclude virtual root
    int visual_level_count = tree_to_be_represented->height - 1; 
    // track position in each level
    int *level_counters = calloc(tree_to_be_represented->height, sizeof(int)); 
    // number of nodes at each level
    int level_height = WINDOW_HEIGHT / visual_level_count;

    assignCoordinates(tree_to_be_represented->virtual_root, 0, level_counters, tree_to_be_represented->widths,level_height);

    pullChildrenToParent(tree_to_be_represented->virtual_root->first_child);
    free(level_counters);
}

void handleClick(int x, int y, node *root) {
    if (!root) return;

    int dx = x - root->x;
    int dy = y - root->y;
    if (dx*dx + dy*dy <= root->radius * root->radius) {
        // Click is inside this node
        // Open root->path
        if (root->path[0] != '\0') {
            char cmd[512];
            snprintf(cmd, sizeof(cmd), "xdg-open \"%s\"", root->path);
            if(system(cmd) == -1)
            {
                int val = system("zenity --error --text=\"Couldn't open the file!\"");
                printf("%d", val);
            }
        }
        return;
    }

    // Recurse children and siblings
    handleClick(x, y, root->first_child);
    handleClick(x, y, root->next_sibling);
}

void draw_nodes(node * node_to_be_drawn)
{
    if(node_to_be_drawn)
    {
        if(node_to_be_drawn->radius != -1 && node_to_be_drawn->x != -1 && node_to_be_drawn->y != -1)
        {
            DrawCircle(node_to_be_drawn->x, node_to_be_drawn->y, node_to_be_drawn->radius, BLUE);
            // Draw text centered
            Vector2 textSize = MeasureTextEx(GetFontDefault(), node_to_be_drawn->name, 12, 1);
            DrawText(node_to_be_drawn->name, node_to_be_drawn->x - textSize.x / 2, node_to_be_drawn->y - textSize.y / 2, 12, BLACK);
        }
        draw_nodes(node_to_be_drawn->first_child);
        draw_nodes(node_to_be_drawn->next_sibling);
    }
}

void draw_lines(node * node_to_be_drawn)
{
    if(node_to_be_drawn)
    {
        node * parent = node_to_be_drawn->parent;
        if((node_to_be_drawn->radius != -1 && node_to_be_drawn->x != -1 && node_to_be_drawn->y != -1) &&
        (parent != NULL) && (parent->radius != -1 && parent->x != -1 && parent->y!= -1))
        {
            DrawLine(node_to_be_drawn->x, node_to_be_drawn->y, parent->x, parent->y, BLACK);
        }
        draw_lines(node_to_be_drawn->first_child);
        draw_lines(node_to_be_drawn->next_sibling);
    }
}

void represent( tree * tree_to_be_represented)
{
    ClearBackground(RAYWHITE);
    draw_lines(tree_to_be_represented->virtual_root->first_child);
    draw_nodes(tree_to_be_represented->virtual_root->first_child);
}

void getHeight(node * root,int height, int * max_height)
{
    if(root==NULL) return;
    if(height > (*max_height))
    {
        (*max_height) = height;
    }
    getHeight(root->first_child,height+1,max_height);
    getHeight(root->next_sibling,height,max_height);
}

void getWidth(int * widths, int height, node * root)
{
    if(root==NULL) return;
    widths[height]++;
    getWidth(widths,height,root->next_sibling);
    getWidth(widths,height+1, root->first_child);
}
