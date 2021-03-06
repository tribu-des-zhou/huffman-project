//
// Created by thepa on 03/11/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include "HuffmanTree.h"
#include "../stack/Stack.h"
#include "../SLL/Element.h"

Tree* init_Tree()
{
    Tree* tree = (Tree*)malloc(sizeof(Tree));
    tree->data = '\0';
    tree->occurrence = 0;
    tree->left = NULL;
    tree->right = NULL;

    return tree;
}

Tree* create_tree(char data, int occurrence, Tree* left, Tree* right)
{
    Tree* tree = (Tree*)malloc(sizeof(Tree));
    tree->data = data;
    tree->occurrence = occurrence;
    tree->left = left;
    tree->right = right;

    return tree;
}

Element* BT_to_UT(Tree* occurrences)
{
    if(!occurrences){return NULL;}
    Stack* history = initialize_stack();
    Element** returned = (Element**)malloc(sizeof(Element*)), **buffer;
    *returned = NULL;
    buffer = returned;

    push_stack(history, occurrences);

    while(!is_stack_empty(history)){
        Tree* node = pull_stack(history);

        for(buffer = returned; *buffer != NULL; buffer = &((*buffer)->next))
        {
            if((*buffer)->node->occurrence >= node->occurrence)
                break;
        }
        *buffer = create_element(
                create_tree(node->data, node->occurrence, NULL, NULL),
                (*buffer));

        if(node->right){
            push_stack(history, node->right);
        }

        if(node->left){
            push_stack(history, node->left);
        }
    }

    free(history);
    Element* tmp = *returned;
    free(returned);
    return tmp;
}

void SLL_to_HT(Element** l){
    if(l == NULL){return;} //ça c'est pour l'erreur, à del pour toi si tu en veux pas

    if(*l != NULL && (*l)->next != NULL)
    {
        Element* add = init_element();
        add->node->left = (*l)->node;
        add->node->right = (*l)->next->node;
        add->node->occurrence = add->node->left->occurrence + add->node->right->occurrence; //ça c'est la node qui est la somme des deux les plus petites

        Element* buffer = (*l)->next->next, **scan; //on avance de deux
        free((*l)->next); //les free avant de les perdres
        free(*l);
        *l = buffer;
        scan = l;
        while (*scan != NULL && (*scan)->node->occurrence < add->node->occurrence) //on remet bien la nouvelle node
        {
            scan = &(*scan)->next;
        }
        add->next = *scan;
        *scan = add;

        SLL_to_HT(l);
    }
}

void add_occurrences(Tree **ht, char ch)
{
    if((*ht))
    {
        if((*ht)->data == ch)
        {
            (*ht)->occurrence++;
        }else if((*ht)->data > ch){
            add_occurrences(&((*ht)->right), ch);
        }else{
            add_occurrences(&((*ht)->left), ch);
        }
    }else{
        (*ht) = create_tree(ch, 1, NULL, NULL);
    }
}

void t_free(Tree* node)
{
    if(node != 0)
    {
        t_free(node->left);
        t_free(node->right);
        free(node);
    }
}

char *get_bin(char c, Tree *source, int size)
{
    char* tmp = NULL;

    if(source)
    {
        if(source->data == c)
        {
            tmp = (char*)malloc(sizeof(char)*(size+1));
            tmp[size] = '\0';
        }
        else
        {
            tmp = get_bin(c, source->left, size+1);
            if(tmp)
                tmp[size] = '0';
            else
            {
                tmp = get_bin(c, source->right, size+1);
                if(tmp)
                    tmp[size] = '1';
            }
        }
    }

    return tmp;
}

int hasSons(Tree* arbre)
{
    return (arbre->left && arbre->right);
}

void ecosia(FILE* output_file, Tree* tree)
{
    if(output_file == NULL){return;}

    if(tree != NULL)
    {
        if(tree->data == 0)
        {
            fputc(':',output_file);
            ecosia(output_file, tree->left);
            ecosia(output_file, tree->right);
        }
        else
        {
            if(tree->data == '.' || tree->data == ':' || tree->data == '#'){fputc('#', output_file);}
            fputc(tree->data, output_file);
            //if(tree->left != NULL || tree->right != NULL){t_free(tree->right);t_free(tree->left);}
        }
        //free(tree);
    }
    else
    {
        fputc('.', output_file);
    }
}

Tree* read_tree(FILE* tree_file)
{
    if(tree_file == NULL){return NULL;}
    char c = fgetc(tree_file);

    if(c == '.' || c == EOF){return NULL;}

    Tree* tree = init_Tree();
    if(c == ':')
    {
        tree->left = read_tree(tree_file);
        tree->right = read_tree(tree_file);
    }
    else
    {
        if(c == '#'){c = fgetc(tree_file);}
        tree->data = c;
    }
    return tree;
}
