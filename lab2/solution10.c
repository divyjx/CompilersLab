#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char** table;

int isop(char c){
    return (c == '=' || c == '-' ||  c == '+' || c == '*' || c == '/');
}

typedef enum __NodeType{
    START,
    token,
    operator,
    TEMP,
    END
}NodeType;

// Node
typedef struct __Node
{
    int val; // ascii value of char (- ascii for negatives)
    NodeType type; 
    struct __Node* next;
    struct __Node* prev;
    int sign;
}node;

node* create_node(int val, node* next, node* prev){
    node* new_node = (node*) malloc (sizeof(node));
    new_node->next = next;
    new_node->prev = prev;
    new_node->sign = 0;
    if (isop((char)val)){
        new_node->type = operator;
        new_node->val = val;
    }
    else {
        new_node->type = token;
        new_node->val = val;
    }
    return new_node;
} 
node* create_temp (int val, node* next, node* prev){
    node* new_node = create_node(val, next, prev);
    new_node->type = TEMP;
    return new_node;
}

void get_label(node *_node, char* _str)
{
    if (_node->type == TEMP)
        sprintf(_str,"(%d)", _node->val);
    else
        sprintf(_str,"%c%c", ((_node->sign)?'-':' '),(char)abs(_node->val));
}


int main()
{
    printf("Enter the expression terminated by new line (without spaces)\n");
    char buff[1000];

    scanf("%[^\n]",buff); 
    int is_assignment = 0;


    int prev_op = 1;
    node* Head = create_node(-1, NULL, NULL);
    Head->type = START;
    node* Tail = create_node(1, NULL, Head);
    Head->type = END;
    Head->next = Tail;
    
    node* temp = Head;
    int ops = -1;
    int sign_count = 0;
    int sign = 0;
    for (int i = 0; i < strlen(buff); i++)
    {
        if (buff[i] == ' ') continue;
        if (!is_assignment && buff[i] == '=') {is_assignment = 1; }
        else if (is_assignment > 0 && buff[i]=='=') {
            printf("Invalid Operator / Number order\n");
            exit(0);
        }  
        else if (isop(buff[i]) == prev_op){
            if (sign_count == 1 && buff[i] == '-'){
                sign_count++;
                sign = 1;
                continue;
            }else {
                printf("Invalid Operator / Number order\n");
                exit(0);
            }
        }
        if (isop(buff[i])) sign_count++;
        else sign_count = 0;
        ops+=isop(buff[i]);
        prev_op = !prev_op;
        temp -> next -> prev = create_node((int) buff[i], temp->next, temp);
        temp -> next = temp -> next -> prev;
        temp = temp -> next;
        if (sign) {temp->sign = 1; sign = 0; ops++;}
    }
    if (prev_op == 1) {
        printf("Invalid Operator / Number order\n");
        exit(0);
    }
    
    // temp = Head->next;
    // int j = 0;
    // while (temp!=Tail)
    // {
    //     printf("%d : %c %d | ", j++,temp->val, temp->sign);
    //     temp = temp ->next;
    // }
    // printf("\n");
    // temp = Head;
    
    int table_i = 0;
    table = (char**) malloc((sizeof(char*)*(ops+1)));

    printf("\nTable ('_' denotes null) \n\n");
    // printf("ops : %d", ops);
    // exit(0);

    for (int i = 0; i < ops+1; i++){
        int found = 0; 
        temp = Head;
        
        // find negative sign
        if (!found) {
            temp = Head;
            while (temp!=Tail){
                if (temp->sign){
                    found = 1;
                    break;
                }
                temp = temp->next;
            }
        }
 

        // find mul/div first
        if (!found) {
            temp = Head;
            while (temp!=Tail){
                if (((temp->val == (int)'*') || (temp -> val == (int)'/')) && (temp->type == operator)){
                    found = 1;
                    break;
                }
                temp = temp->next;
            }
        }

        // find sub/add next         
        if (! found){
            temp = Head;
            while (temp!=Tail){
                if ((temp->val == (int)'-' || temp -> val == (int)'+') && temp->type == operator){
                    found = 1;
                    break;
                }
                temp = temp -> next;
            }
        }
        if (! found) break;

        // Node is found with sign
        if (temp->sign){
            temp->type = TEMP;
            table[table_i] = (char*) malloc (sizeof(char)* 50);

            sprintf(table[table_i], "%d %c _ -", table_i+1, (char)temp->val);
            printf("%s\n",table[table_i]);
            temp->val = table_i + 1;
            table_i++;
            temp->sign = 0;
            continue;
        }

        // Normal 3AC calculation
        node* temp_node = create_temp(table_i+1, temp -> next -> next, temp -> prev -> prev);
        node* prev_node = temp->prev;
        node* next_node = temp->next;
        node* curr_node = temp;
        table[table_i] = (char*) malloc (sizeof(char)* 50);

        char prev_str[10];
        get_label(prev_node, prev_str);
        char next_str[10];
        get_label(next_node, next_str);

        sprintf(table[table_i], "%d %s %s %c", table_i+1, prev_str, next_str, (char)temp->val);
        printf("%s\n",table[table_i]);
        
        table_i ++;
        temp_node->prev->next = temp_node;
        temp_node->next->prev = temp_node;

        free (next_node);
        free (prev_node);
        free (temp);
    }


    if (is_assignment){
        if (table_i == 0) 
            printf("%d  %c _ = \n", table_i+1,  (char)Head->next->next->next->val);
        else 
            printf("%d (%d) _ = \n", table_i+1, table_i-1);
    }
}


