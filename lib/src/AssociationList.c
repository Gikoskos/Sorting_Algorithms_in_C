 /********************
 *  AssociationList.c
 *
 * This file is part of libvoids which is licensed under the 2-Clause BSD License
 *
 * Copyright (c) 2015-2018 George Koskeridis <georgekoskerid@outlook.com>
 * All rights reserved.
  ***********************************************************************************/


#include "HeapAllocation.h"
#include "AssociationList.h"


AListNode *AList_insert(AListNode **aListHead,
                        void *pData,
                        void *pKey,
                        vdsUserCompareFunc KeyCmp,
                        vdsErrCode *err)
{
    vdsErrCode tmp_err = VDS_SUCCESS;
    AListNode *new_node = NULL;

    if (aListHead && pKey && KeyCmp) {

        //insert the new node ONLY if a node with the same key doesn't exist already in the list
        if (!AList_find(*aListHead, pKey, KeyCmp, NULL)) {
            new_node = VdsMalloc(sizeof(AListNode));

            if (new_node) {

                new_node->item.pData = pData;
                new_node->item.pKey = pKey;
                new_node->nxt = *aListHead;

                *aListHead = new_node;

            } else
                tmp_err = VDS_MALLOC_FAIL;
        } else
            tmp_err = VDS_KEY_EXISTS;
    } else
        tmp_err = VDS_INVALID_ARGS;

    SAVE_ERR(err, tmp_err);

    return new_node;
}

AListNode *AList_append(AListNode **aListHead,
                        void *pData,
                        void *pKey,
                        vdsUserCompareFunc KeyCmp,
                        vdsErrCode *err)
{
    vdsErrCode tmp_err = VDS_SUCCESS;
    AListNode *new_node = NULL;

    if (aListHead && pKey && KeyCmp) {

        new_node = VdsMalloc(sizeof(AListNode));

        if (new_node) {

            new_node->item.pData = pData;
            new_node->item.pKey = pKey;
            new_node->nxt = NULL;

            if (!(*aListHead)) {
                *aListHead = new_node;
            } else {
                AListNode *curr;

                for (curr = *aListHead; curr->nxt; curr = curr->nxt)
                    if (!KeyCmp(curr->item.pKey, pKey)) { //if a node with the same key, already exists on the list
                        VdsFree(new_node);                  //we don't add it
                        new_node = curr;
                        tmp_err = VDS_KEY_EXISTS;
                        break;
                    }

                curr->nxt = new_node;
            }
        } else
            tmp_err = VDS_MALLOC_FAIL;
    } else
        tmp_err = VDS_INVALID_ARGS;

    SAVE_ERR(err, tmp_err);

    return new_node;
}

AListNode *AList_insertAfter(AListNode *dictListPrev,
                             void *pData,
                             void *pKey,
                             vdsErrCode *err)
{
    vdsErrCode tmp_err = VDS_SUCCESS;
    AListNode *new_node = NULL;

    if (dictListPrev && pKey) {
        new_node = VdsMalloc(sizeof(AListNode));

        if (new_node) {

            new_node->item.pData = pData;
            new_node->item.pKey = pKey;
            new_node->nxt = dictListPrev->nxt;

            dictListPrev->nxt = new_node;

        } else
            tmp_err = VDS_MALLOC_FAIL;
    } else
        tmp_err = VDS_INVALID_ARGS;

    SAVE_ERR(err, tmp_err);

    return new_node;
}

void *AList_delete(AListNode **aListHead,
                   void *pKey,
                   vdsUserCompareFunc KeyCmp,
                   vdsErrCode *err)
{
    vdsErrCode tmp_err = VDS_INVALID_ARGS;
    void *pDeleted = NULL;

    if (aListHead && KeyCmp && pKey) {
        AListNode *curr, *prev = NULL;

        for (curr = *aListHead; curr && KeyCmp(curr->item.pKey, pKey); curr = curr->nxt)
            prev = curr;

        if (curr) {
            pDeleted = curr->item.pData;

            if (prev) {
                prev->nxt = curr->nxt;
            } else {
                *aListHead = curr->nxt;
            }

            VdsFree(curr);
            tmp_err = VDS_SUCCESS;
        }
    } else
        tmp_err = VDS_INVALID_ARGS;

    SAVE_ERR(err, tmp_err);

    return pDeleted;
}

void *AList_find(AListNode *aListHead,
                 void *pKey,
                 vdsUserCompareFunc KeyCmp,
                 vdsErrCode *err)
{
    vdsErrCode tmp_err = VDS_SUCCESS;
    AListNode *curr = aListHead;

    if (aListHead && KeyCmp && pKey)
        for (; curr && KeyCmp(curr->item.pKey, pKey); curr = curr->nxt);
    else
        tmp_err = VDS_INVALID_ARGS;

    SAVE_ERR(err, tmp_err);

    return (curr) ? curr->item.pData : NULL;
}

void *AList_replace(AListNode *aListHead,
                    void *pNewData,
                    void *pKey,
                    vdsUserCompareFunc KeyCmp,
                    vdsErrCode *err)
{
    vdsErrCode tmp_err = VDS_SUCCESS;
    AListNode *curr = aListHead;
    void *pOldValue = NULL;

    if (aListHead && KeyCmp && pKey) {
        for (; curr && KeyCmp(curr->item.pKey, pKey); curr = curr->nxt);

        if (curr) {
            pOldValue = curr->item.pData;
            curr->item.pData = pNewData;
        }
    } else
        tmp_err = VDS_INVALID_ARGS;

    SAVE_ERR(err, tmp_err);

    return pOldValue;
}

void AList_traverse(AListNode *aListHead,
                    vdsTraverseFunc handleData,
                    vdsErrCode *err)
{
    vdsErrCode tmp_err = VDS_SUCCESS;

    if (aListHead && handleData) {
        for (AListNode *curr = aListHead; curr; curr = curr->nxt)
            if (!handleData((void *)&curr->item))
                break;
    } else
        tmp_err = VDS_INVALID_ARGS;

    SAVE_ERR(err, tmp_err);
}

void AList_destroy(AListNode **aListHead,
                   vdsUserDataFunc freeData,
                   vdsErrCode *err)
{
    vdsErrCode tmp_err = VDS_SUCCESS;

    if (aListHead) {
        AListNode *curr, *tmp;

        for (curr = *aListHead; curr;) {
            if (freeData)
                freeData((void *)&curr->item);

            tmp = curr;
            curr = curr->nxt;
            VdsFree(tmp);
        }

        *aListHead = NULL;
    } else
        tmp_err = VDS_INVALID_ARGS;

    SAVE_ERR(err, tmp_err);
}
