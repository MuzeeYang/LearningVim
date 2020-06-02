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
			catTrainNodes(pHead->prev, &newhead2);
			break;
		}else if(IS_EMPTY_LIST(&newhead2)){
			catTrainNodes(pHead->prev, &newhead1);
			break;
		}

		cursor = newhead1.next;
		while(cmpFunc(cursor, newhead2.next) <= 0 && cursor != &newhead1)
			cursor = cursor->next;
		if(cursor != newhead1.next){
			cutTrainNodes(&tmphead, newhead1.next, cursor->prev);
			catTrainNodes(pHead->prev, &tmphead);
		}else if(cursor == &newhead1){
			catTrainNodes(pHead->prev, &newhead1);
			continue;
		}

		cursor = newhead2.next;
		while(cmpFunc(cursor, newhead1.next) < 0 && cursor != &newhead2)
			cursor = cursor->next;
		if(cursor != newhead2.next){
			cutTrainNodes(&tmphead, newhead2.next, cursor->prev);
			catTrainNodes(pHead->prev, &tmphead);
		}else if(cursor == &newhead2){
			catTrainNodes(pHead->prev, &newhead2);
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

void unshiftList(PTrainNode pHead, PTrainNode pNode)
{
	insertTrainNodeBack(pHead, pNode);
}

PTrainNode shiftList(PTrainNode pHead)
{
	PTrainNode ret = 0;
	if((ret = deleteTrainNode(pHead->next)) == pHead)
		ret = 0;
	return ret;
}

PTrainNode jumpList(PTrainNode pHead, PTrainNode pNode, int length)
{
	while(length > 0 && pNode->next != pHead){
		pNode = pNode->next;
		length--;
	}

	while(length < 0 && pNode->prev != pHead){
		pNode = pNode->prev;
		length++;
	}

	return pNode;
}

int countList(PTrainNode pHead, PTrainNode start, PTrainNode end)
{
	int currentIdx = startIdx = endIdx = 0;
	PTrainNode cursor = 0;
	
	if(start == 0)start = pHead->next;
	if(end == 0)end = pHead->prev;
	FOR_EACH(pHead, cursor){
		currentIdx++;
		if(cursor == start)startIdx = currentIdx;
		else if(cursor == end)endIdx = currentIdx;

		if(endIdx && startIdx)return endIdx - startIdx;
	}

	return 0;
}

void spliceList(PTrainNode start, PTrainNode end, PTrainNode outHead, PTrainNode inHead)
{
	if(outHead)cutTrainNodes(outHead, start->next, end);
	if(inHead)catTrainNodes(start, inHead);
}

PTrainNode duplicateList(PTrainNode pHead, PTrainNode outHead, LISTDUP_FUNC dupFunc)
{
	PTrainNode cursor = 0;
	if(outHead)initTrainNode(outHead);
	else return outHead;

	FOR_EACH(pHead, cursor){
		insertTrainNodeBack(outHead, dupFunc(cursor));
	}
	return outHead;
}

PTrainNode reverseList(PTrainNode pHead, PTrainNode outHead, LISTDUP_FUNC dupFunc)
{
	PTrainNode cursor = 0;
	if(outHead)initTrainNode(outHead);
	else return outHead;

	FOR_EACH(pHead, cursor){
		insertTrainNodeFront(outHead, dupFunc(cursor));
	}
	return outHead;
}

PTrainNode filterList(PTrainNode pHead, PTrainNode outHead, LISTDUP_FUNC dupFunc)
{
	PTrainNode cursor = 0;
	if(outHead)initTrainNode(outHead);
	else return outHead;

	FOR_EACH(pHead, cursor){
		if(filterFunc(PTrainNode))insertTrainNodeBack(outHead, cursor);
	}
	return outHead;
}

PTrainNode reduceList(PTrainNode pHead, PTrainNode outNode, LISTRDC_FUNC reduceFunc)
{
	PTrainNode cursor = 0;
	if(outNode)initTrainNode(outNode);
	else return outNode;

	FOR_EACH(pHead, cursor){
		if(!reduceFunc(outNode, cursor))break;
	}

	return outNode;
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
	PTrainNode start = 0, end = 0;

	start = pHead->next;
	end = pHead->prev;
	do{
		if(cmpFunc(start, pNode) == 0)return start;
		if(cmpFunc(end, pNode) == 0)return end;
		start = start->next;
		end = end->prev;
	}while(start != end && start->next != end)

	return 0;
}

void destroyList(PTrainNode pHead, LISTDSTR_FUNC destroyFunc)
{
	PTrainNode cursor = 0;

	FOR_EACH(pHead, cursor){
		destroyFunc(cursor);
	}
}
