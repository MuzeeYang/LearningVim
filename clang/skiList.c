#include "skiList.h"

void sortInsert(PTrainNode pHead, LISTCMP_FUNC cmpFunc)
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

void sortMerge(PTrainNode pHead, LISTCMP_FUNC cmpFunc)
{
	if(pHead->next == pHead->prev)
		return;

	PTrainNode cursor;
	DECLARE_HEAD_LIST(newhead1);
	DECLARE_HEAD_LIST(newhead2);
	GET_MIDPTR_LIST(pHead, cursor);

	cutTrainNodes(&newhead1, pHead->next, cursor);
	cutTrainNodes(&newhead2, pHead->next, pHead->prev);
	sortMerge(&newhead1, cmpFunc);
	sortMerge(&newhead2, cmpFunc);

	//merge start
	DECLARE_HEAD_LIST(tmphead);
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

PTrainNode popList(PTrainNode pHead)
{
	PTrainNode ret = 0;
	if((ret = deleteTrainNode(pHead->prev)) == pHead)
		ret = 0;
	return ret;
}

PTrainNode shiftList(PTrainNode pHead)
{
	PTrainNode ret = 0;
	if((ret = deleteTrainNode(pHead->next)) == pHead)
		ret = 0;
	return ret;
}

void pushSortList(PTrainNode pHead, PTrainNode pNode, LISTCMP_FUNC cmpFunc)
{
	PTrainNode cursor = 0;
	FOR_EACH(pHead, cursor){
		if(cmpFunc(cursor, pNode) > 0)break;
	}
	insertTrainNodeFront(cursor, pNode);
}

PTrainNode searchList(PTrainNode pHead, PTrainNode pNode, LISTCMP_FUNC cmpFunc)
{
	PTrainNode start = NULL, end = NULL;

	start = pHead->next;
	end = pHead->prev;
	do{
		if(cmpFunc(start, pNode) == 0)return start;
		if(cmpFunc(end, pNode) == 0)return end;
		start = start->next;
		end = end->prev;
	}while(start != end && start->next != end)

	return NULL;
}
