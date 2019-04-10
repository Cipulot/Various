//Luca Sevà
//Assortment of recursive functions for the list structure
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//types
typedef struct N{
    int data;
    struct N *next;
}node;

typedef node *ptrnode;

//Check empty list
int empty_list(ptrnode l){
    if(l == NULL) return 1;
    else return 0;
}

//Dimension
int dimension_rec(ptrnode l){
    if(empty_list(l)) return 0;
    else return(1+dimension_rec(l->next)); //The function is called until NULL is encountered for end list node.
}

//Print list
void print_list_rec(ptrnode l){
    //The data field is printed until the node points to NULL
    if(empty_list(l)) printf("-|End list\n");
    else{
        printf("%d ", l->data);
        print_list_rec(l->next);
    }
}

//REC-Find
int find_rec(ptrnode l, int el){
    if(empty_list(l)) return 0;
    if(l->data == el) return 1; //return 1 at the first occurrence, multiple occurrences are not reported
    find_rec(l->next, el);
}

//Add last
ptrnode add_last_rec(ptrnode l, int el){
    ptrnode ptr;
    if(empty_list(l)){ //if list is empty
        ptr = (ptrnode)malloc(sizeof(node)); //the new node is allocated as the head of the list (see ptr-> next = NULL)
        if(ptr){
            ptr->data = el;
            ptr->next = NULL;
        }
        else printf("Error\n"); //wrong allocation
        return ptr; //return head pointer
    }
    else{
        l->next = add_last_rec(l->next, el); //if the list was NOT empty then proceed to create the node with the function itself.
        //This time it will no longer be the head but its next one, due to the fact that l is returned (remember that l! = NULL, so enter ELSE)
        return l;
    }
}
