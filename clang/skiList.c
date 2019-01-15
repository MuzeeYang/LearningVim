#include "skiList.h"

void sortInsert(PTrainNode pHead, LISTSORT_FUNC cmpFunc)
{
	PTrainNode cursor;
	PTrainNode pNode;
	for(cursor = pHead->next; cursor != pHead;){
		for(pNode = cursor; pNode->prev != pHead; pNode = pNode->prev){
			if(cmpFunc(pNode->prev, cursor) < 0)
				break;
		}

		if(pNode == cursor)
			cursor = cursor->next;
		else{
			cursor = cursor->next;
			insertTrainNodeFront(pNode, deleteTrainNode(cursor->prev));
		}
	}
}

void sortMerge(PTrainNode pHead, LISTSORT_FUNC cmpFunc)
{
	if(pHead->next == pHead->prev)
		return;

	PTrainNode cursor;
	DECLARE_HEAD(newhead1);
	DECLARE_HEAD(newhead2);
	GET_MIDPTR_LIST(pHead, cursor);

	cutTrainNodes(&newhead1, pHead->next, cursor);
	cutTrainNodes(&newhead2, pHead->next, pHead->prev);
	mergeSort(&newhead1, cmpFunc);
	mergeSort(&newhead2, cmpFunc);

	//merge start
	DECLARE_HEAD(tmphead);
	while(1)
	{
		if(IS_EMPTY_LIST(&newhead1)){
			catTrainNodes(pHead, &newhead2);
			break;
		}else if(IS_EMPTY_LIST(&newhead2)){
			catTrainNodes(pHead, &newhead1);
			break;
		}

		cursor = newhead1.next;
		while(cmpFunc(cursor, newhead2.next) <= 0 && cursor != &newhead1)
			cursor = cursor->next;
		if(cursor != newhead1.next){
			cutTrainNodes(&tmphead, newhead1.next, cursor->prev);
			catTrainNodes(pHead, &tmphead);
		}else if(cursor == &newhead1){
			catTrainNodes(pHead, &newhead1);
			continue;
		}

		cursor = newhead2.next;
		while(cmpFunc(cursor, newhead1.next) < 0 && cursor != &newhead2)
			cursor = cursor->next;
		if(cursor != newhead2.next){
			cutTrainNodes(&tmphead, newhead2.next, cursor->prev);
			catTrainNodes(pHead, &tmphead);
		}else if(cursor == &newhead2){
			catTrainNodes(pHead, &newhead2);
			continue;
		}
	}
}

void pushList(PTrainNode pHead, PTrainNode pNode)
{
	insertTrainNodeFront(pHead, pNode);
}

PTrainNode popList(pHead)
{
	PTrainNode ret = NULL;
	if((ret = deleteTrainNode(pHead->prev)) == pHead)
		ret = NULL
	return ret;
}

PTrainNode getFirstList(PTrainNode pHead)
{
	PTrainNode ret = NULL;
	if((ret = deleteTrainNode(pHead->next)) == pHead)
		ret = NULL
	return ret;
}
