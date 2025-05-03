#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap* new = (TreeMap*) malloc(sizeof(TreeMap));
    if (new == NULL) {
        return NULL; 
    }
    new->root = NULL;
    new->lower_than = lower_than;
    new->current = NULL;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode* new_node = createTreeNode(key, value);
    if (new_node == NULL){
        return;
    }
    if (tree->root == NULL) {
        tree->root = new_node;
        tree->current = new_node;
        return;
    }
    TreeNode* current = tree->root;
    TreeNode* parent = NULL;
    while (current != NULL) {
        parent = current;
        if (tree->lower_than(key, current->pair->key)) {
            current = current->left; 
        } 
        else if (tree->lower_than(current->pair->key, key)) {
            current = current->right; 
        } 
        else {
            return;
        }
    }
    new_node -> parent = parent;
    if (tree->lower_than(key, parent->pair->key)){
        parent->left = new_node;
    } 
    else{
        parent->right = new_node;
    }
    tree->current = new_node;
    return;
}

TreeNode * minimum(TreeNode * x){
    if (x == NULL) return NULL;
    TreeNode *current = x;
    while (current->left != NULL) {
        current = current->left;   
    }
    return current;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (tree == NULL || node == NULL) return;
    TreeNode* parent = node->parent;
    if (node->left == NULL && node->right == NULL) {
        if (parent == NULL) {
            tree->root = NULL;
        } else if (parent->left == node) {
            parent->left = NULL;
        } else {
            parent->right = NULL; 
        }
        free(node->pair);
        free(node);
    }
    else if (node->left == NULL || node->right == NULL){
        TreeNode* hijo;
        if (node->left != NULL) {
            hijo = node->left;  
        } 
        else{
            hijo = node->right; 
        }
        if (parent == NULL) {
            tree->root = hijo;  
        } 
        else{
            if (parent->left == node) {
                parent->left = hijo;  
            } else {    
                parent->right = hijo;
            }
        }
        hijo->parent = parent;
        free(node->pair);
        free(node);
    }
    else {
        TreeNode* minimo = minimum(node->right);
        free(node->pair);
        node->pair = minimo->pair;
        removeNode(tree, minimo);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* current = tree->root; 
    while (current != NULL) {
        if (tree->lower_than(key, current->pair->key)) {
            current = current->left; 
        } 
        else if (tree->lower_than(current->pair->key, key)) {
            current = current->right; 
        } 
        else {
            tree->current = current;
            return current->pair;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode* current = tree->root;
    TreeNode* ub_node = NULL;
    while (current != NULL) { 
        if (tree->lower_than(key, current->pair->key)) {
            if (ub_node == NULL){
                ub_node = current;
            }
            current = current->left;
        }
        else if(ub_node != NULL && is_equal(tree,current->pair->key,key)){
            ub_node = current;
            return ub_node->pair;
        }
        else{
            current = current->right;
        }
    }
    return ub_node->pair;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode* value = minimum(tree->root);
    return value->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) return NULL;
    TreeNode* current = tree->current;
    TreeNode* siguiente = NULL;
    if (current->right != NULL) {
        siguiente = current->right;
        while (siguiente->left != NULL) {
            siguiente = siguiente->left;
        }
    }
    else{
        siguiente = current->parent;
        while (siguiente != NULL && current == siguiente->right) {
            current = siguiente;
            siguiente = siguiente->parent;
        }
    }
    tree->current = siguiente;
    if(siguiente == NULL) return NULL;
    else{
        return siguiente->pair;
    }
}
