#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <GiDS.h>

void printIntData(void *param)
{
    AVLTreeNode *treeNode = (AVLTreeNode *)param;

    for (int i = 0; i < treeNode->height; i++)
        putchar('\t');
    printf("node %lu at height %lu has data %d\n", treeNode->key, treeNode->height, *(int*)(treeNode->pData));
}

int main(int argc, char *argv[])
{
    AVLTreeNode *avltTest = NULL;

    srand(time(NULL));
    //trying to add 10 nodes to a binary search tree
    //with pseudorandomly generated keys and data
    for (int i = 0; i < 10; i++) {
        unsigned long new_key = (unsigned long)rand()%100 + 1;
        int *data = malloc(sizeof data);

        *data = (int)rand();
        printf("Trying to insert new node with key %lu and data %d!\n", new_key, *data);

        if (insertNodeAVLTree(&avltTest, new_key, (void*)data)) {
            printf("Node was successfully inserted!\n");
            assert(isBST(avltTest));
            printf("TREE IS BST!!!!\n");
        } else {
            printf("Node insertion failed!\n");
            free(data);
        }
    }

    //testing traversals
    /*printf("\npre-order traversal:\n");
    traverseAVLTree(avltTest, PRE_ORDER, printIntData);

    printf("\nin-order traversal:\n");
    traverseAVLTree(avltTest, IN_ORDER, printIntData);

    printf("\npost-order traversal:\n");
    traverseAVLTree(avltTest, POST_ORDER, printIntData);

    printf("\nbreadth-first traversal:\n");
    traverseAVLTree(avltTest, BREADTH_FIRST, printIntData);

    printf("\neuler traversal:\n");
    traverseAVLTree(avltTest, EULER, printIntData);*/

    for (unsigned long i = 0; i < 50; i++) {
        int *data = (int*) deleteByKeyAVLTree(&avltTest, i);

        if (data) {
            printf("\nDeleted node %lu with data %d\n", i, *data);
            free(data);
            printf("in-order traversal (the node numbers should be in ascending order):\n");
            traverseAVLTree(avltTest, IN_ORDER, printIntData);
            assert(isBST(avltTest));
            printf("TREE IS BST!!!!\n");
        }
    }
    deleteAVLTree(&avltTest, free);
    return 0;
}