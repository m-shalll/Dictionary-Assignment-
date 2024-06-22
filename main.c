#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    char*item;
    struct node *left,*right;
} node;
node* newnode (char*x)
{
    node*n = malloc(sizeof (node));
    n->left = n->right = NULL;
    n->item =malloc(strlen(x)+1);
    strcpy(n->item, x);
    return n;
}
node* insert (node *root, char*x)
{
    if (root == NULL)
    {
        node *n = newnode(x);
        root = n;
    }
    else if (strcasecmp(x,root->item)<0)
    {
        root -> left = insert (root->left,x);
    }
    else if(strcasecmp(x,root->item))
        root ->right = insert(root->right,x);

    return root;
}
node*generatetree()
{
    node*root=NULL;
    FILE *f = fopen("Dictionary.txt","r");
    if(!f)
    {
        printf("File is not found");
        exit(-1);
    }
    while(!feof(f))
    {
        char w[30];
        fscanf(f,"%[^\n]%*c",w);
        root=insert(root,w);
    }
    fclose(f);
    return root;
}
int max(int a,int b)
{
    return(a>b?a:b);
}
int height(node*root)
{
        if(root==NULL)
            return -1;
        else
            return(1+max(height(root->left),height(root->right)));
}
int size(node*root)
{
    if (root==NULL)
        return 0;
    else
        return(1+size(root->left)+size(root->right));
}
node* search(node* root,char* data)
{
    if(root == NULL) return -1;
    else if(strcasecmp(data, root->item) == 0)
        return NULL;
    else if(strcasecmp(data, root->item) < 0)
    {
        if(root->left == 0) return root;
        return search(root->left, data);
    }
    else
    {
        if(root->right == 0) return root;
        return search(root->right, data);
    }
}
node* getSuccessor(node* root, node* curr)
{
    if(!curr) return NULL;
    if(curr->right)
    {
        node* temp = curr->right;
        while(temp->left) temp = temp->left;
        return temp;
    }
    else
    {
        node* successor = NULL;
        node* ancestor = root;
        while(ancestor != curr)
        {
            if(strcasecmp(curr->item, ancestor->item) < 0)
            {
                successor = ancestor;
                ancestor = ancestor->left;
            }
            else
                ancestor = ancestor->right;
        }
        return successor;
    }
}
node* getPredecessor(node* root, node* curr)
{
    if(!curr) return NULL;
    if(curr->left)
    {
        node* temp = curr->left;
        while(temp->right) temp = temp->right;
        return temp;
    }
    else
    {
        node* predecessor = NULL;
        node* ancestor = root;
        while(ancestor != curr)
        {
            if(strcasecmp(curr->item, ancestor->item) < 0)
                ancestor = ancestor->left;
            else
            {
                predecessor = ancestor;
                ancestor = ancestor->right;
            }
        }
        return predecessor;
    }
}
int main()
{
 node*root=generatetree();
 char str[200];
    if(root!=NULL)
    {
        printf("Dictionary loaded Successfully...!");
        printf("\n........................\n");
        printf("Size:%d",size(root));
        printf("\n........................\n");
        printf("Height:%d",height(root));
        printf("\n........................\n");
        printf("Enter a sentence: \n");
        fgets(str, 60, stdin);
        char* token = strtok(str, "\n");
        token = strtok(str, " ");
        while(token)
        {
            node* n = search(root, token);
            if(n == 0)
                printf("%s - CORRECT\n", token);
            else if(n == -1)
            {
                printf("Tree Error");
                exit(1);
            }
            else
            {
                node* pred = getPredecessor(root, n);
                node* succ = getSuccessor(root, n);
                printf("%s - Incorrect, Suggestions: %s %s %s\n",token, n->item, succ->item, pred->item);
            }
            token = strtok(NULL, " ");
        }
    }
    return 0;
}
