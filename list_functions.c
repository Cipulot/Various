#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//types
typedef struct N{
    int data;
    struct N *next;
}node;

typedef node *ptrnode;

void main(){
    ptrnode head; //creation of the head
    head = NULL; //the head of an empty list must be initialized to null to prevent errors
}

//Check empty list
int empty_list(ptrnode l){
    if(l == NULL) return 1;
    else return 0;
}

//Print sorted list
void print_list(ptrnode l){
    ptrnode ptr;
    ptr = l;
    printf("Head--> ");
    while(ptr != NULL){ //count until end of list
        printf("%d\n", ptr->data);
        printf("--");
        ptr = ptr->next; //goes to the next node of the list
    }
}

//list length
int lenth(ptrnode l){
    int len = 0;
    ptrnode ptr; //this pointer is used instead of the original to prevent any mess
    ptr = l;
    while(ptr != NULL){ //count until end of list
        len += 1;
        ptr = ptr->next; //replace the pointer with the pointer of the next node
    }
    return(len);
}

//Add node @ top
ptrnode add_top(ptrnode head, int data){
    ptrnode temp = NULL; //initialized to null so that in case of errors in the malloc this will return null
    temp = (ptrnode)malloc(sizeof(node));
    temp->next = head; //ok, even if NULL //list's consolidation. The newly created node will have the next parameter corresponding to the old head of the list.
    temp->data = data;
    return temp; //return of the new had pointer
}

//Add note @ end of list
ptrnode add_last(ptrnode l, int data){
    ptrnode ptr, cursor;
    ptr = (ptrnode)malloc(sizeof(node)); 
    if(ptr){
        ptr->next = NULL; //to indicate that is the end of the list
        ptr->data = data; //memorize the data given to the function
        cursor = l; //memorize the pointer of the head
        while(cursor->next != NULL){ //count until the last node of the list
            cursor = cursor->next; //go on with the loop
        }
        cursor->next = ptr; //the next field of the previous end of list is changed with the allocated one
        
    }
    else printf("Allocation error\n");
}

//Add data array
ptrnode add_array(ptrnode head, int * data, int arraylen){
    int i;
    ptrnode temp = NULL;
    for(i=0; i<arraylen; i++){
        temp = (ptrnode)malloc(sizeof(node));
        temp->next = head;
        temp->data = *data;

        //Add local head;
        head = temp;
        //go on with the array
        data++;
    }
    return temp;
}

//Delete a specified element of the list with the specified data
ptrnode delete_node(ptrnode l, int el){
    ptrnode ptr, ptr_prev;
    ptr = l;
    ptr_prev = l;
    if(l!=NULL){ //check if list is empty
        if(l->data == el){
            ptr = ptr->next;
            free(l);
            return ptr;
        }
        else{
            while((ptr->data != el) && (ptr != NULL)){ //consolidating the list if the deleted node was between two nodes
                //go on
                ptr_prev = ptr;
                ptr = ptr->next;
            }
            if(ptr->data == el){
                ptr_prev->next = ptr->next;
                free(ptr);
                return l;
            }
        }
    }
}


//Extract and add to list
//This function could be useful for parsing an open file from which a string has been extracted
ptrnode extract_and_add(ptrnode head, char *s){
    char separator[] = ","; //this is the separator char for parsing
    //get the first part
    char * here = strtok(s, separator);
    if(here != NULL) return head;
    int count = 0;
    int n = 0;

    //go on
    while(here != NULL){
        printf(" %s\n", here);
        here = strtok(NULL, separator);
        if(count == 0){
            //first number/element
            n = atoi(here);
        }
        count++;
    }
    if(count>0) head = add_top(head, n); //if correct data counting is >0 will be added to the list
    return head;
}

//Read from file(csv) or similar(separator)
ptrnode read_from_file(ptrnode head, char * name){
    FILE * fp = fopen(name, "rt");
    if(fp == NULL) return head;
    
    char buffer[1024]; //this value is totally arbitrary
    char *s = buffer;
    while(s != NULL){
        s = fgets(buffer, 1024, fp);
        if(s != NULL){
            printf("%s", s);
            head = extract_and_add(head, s);
        }
    }
    fclose(fp);
    return head;
}