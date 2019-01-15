#include "skiTree.h"

static PTreeNode __insert_adjust(PTreeNode pRoot, PTreeNode pNode)
{
	PTreeNode parent, gparent, uncle;
	while(!IS_ROOT_TREE(pNode)){
		//parent is black or node is root
		if(GET_COLOR_TREE(GET_PARENT_TREE(pNode)))
			break;

		parent = GET_PARENT_TREE(pNode);
		gparent = GET_PARENT_TREE(parent);
		if(IS_LEFT_CHILD_TREE(parent)){
			uncle = gparent->rb_right;
			//case 1: uncle is red
			if(GET_COLOR_TREE(uncle) == RB_RED){
				SET_BLACK_COLOR_TREE(parent);
				SET_BLACK_COLOR_TREE(uncle);
				SET_RED_COLOR_TREE(gparent);
				pNode = gparent;
				continue;
			}

			//case 2: node is right child, uncle is black
			if(IS_RIGHT_CHILD_TREE(pNode)){
				rotate_rbnode_left(parent);
				pNode = parent;
				parent = GET_PARENT_TREE(pNode);
			}

			//case 3: node is left child, uncle is black
			if(IS_LEFT_CHILD_TREE(pNode)){
				SET_BLACK_COLOR_TREE(parent);
				SET_RED_COLOR_TREE(gparent);
				pNode = rotate_rbnode_right(gparent);
				break;
			}
		}else{
			uncle = gparent->rb_left;
			//case 1: uncle is red
			if(GET_COLOR_TREE(uncle) == RB_RED){
				SET_BLACK_COLOR_TREE(parent);
				SET_BLACK_COLOR_TREE(uncle);
				SET_RED_COLOR_TREE(gparent);
				pNode = gparent;
				continue;
			}

			//case 2: node is left child, uncle is black
			if(IS_LEFT_CHILD_TREE(pNode)){
				rotate_rbnode_right(parent);
				pNode = parent;
				parent = GET_PARENT_TREE(pNode);
			}

			//case 3: node is left child, uncle is black
			if(IS_RIGHT_CHILD_TREE(pNode)){
				SET_BLACK_COLOR_TREE(parent);
				SET_RED_COLOR_TREE(gparent);
				pNode = rotate_rbnode_left(gparent);
				break;
			}
		}
	}

	if(IS_ROOT_TREE(pNode)){
		SET_BLACK_COLOR_TREE(pNode);
		return pNode;
	}else
		return pRoot;
}

static PTreeNode __erase_adjust(PTreeNode pRoot, PTreeNode pNode)
{
	if(GET_COLOR_TREE(pNode) == RB_RED)
		return pRoot;

	PTreeNode parent, sibling, lnephew, rnephew, fRoot;

	fRoot = pRoot;
	while(!IS_ROOT_TREE(pNode)){
		parent = GET_PARENT_TREE(pNode);
		printf_debug("%d\n", IS_LEFT_CHILD_TREE(pNode));
		if(IS_LEFT_CHILD_TREE(pNode)){
			//case 1: pNode has red left child
			printf_debug("%d\n", GET_COLOR_TREE(pNode->rb_left));
			if(GET_COLOR_TREE(pNode->rb_left) == RB_RED){
				SET_BLACK_COLOR_TREE(pNode->rb_left);
				break;
			}

			//case 2: sibling is red
			sibling = parent->rb_right;
			printf_debug("%d\n", GET_COLOR_TREE(sibling));
			if(GET_COLOR_TREE(sibling) == RB_RED){
				SET_RED_COLOR_TREE(parent);
				SET_BLACK_COLOR_TREE(sibling);
				fRoot = rotate_rbnode_left(parent);
				sibling = parent->rb_right;
			}

			//case 3: sibling is black, nephews are black;
			lnephew = sibling->rb_left;
			rnephew = sibling->rb_right;
			printf_debug("%d, %d\n", GET_COLOR_TREE(lnephew), GET_COLOR_TREE(rnephew));
			if(GET_COLOR_TREE(lnephew) && GET_COLOR_TREE(rnephew)){
				SET_RED_COLOR_TREE(sibling);
				if(GET_COLOR_TREE(parent)){
					pNode = parent;
					continue;
				}else{
					SET_BLACK_COLOR_TREE(parent);
					break;
				}
			}

			//case 4: sibling is black, right nephews is red
			printf_debug("%d\n", GET_COLOR_TREE(rnephew));
			if(GET_COLOR_TREE(rnephew) == RB_RED){
				SET_COLOR_TREE(sibling, GET_COLOR_TREE(parent));
				SET_BLACK_COLOR_TREE(parent);
				SET_BLACK_COLOR_TREE(rnephew);
				fRoot = rotate_rbnode_left(parent);
				break;
			}

			//case 5: sibling is black, left nephews is red
			printf_debug("%d\n", GET_COLOR_TREE(lnephew));
			if(GET_COLOR_TREE(lnephew) == RB_RED){
				SET_RED_COLOR_TREE(sibling);
				SET_BLACK_COLOR_TREE(lnephew);
				fRoot = rotate_rbnode_right(sibling);
				continue;
			}

		}else{
			//case 1: pNode has red right child
			if(GET_COLOR_TREE(pNode->rb_right) == RB_RED){
				SET_BLACK_COLOR_TREE(pNode->rb_right);
				break;
			}

			//case 2: sibling is red
			sibling = parent->rb_left;
			if(GET_COLOR_TREE(sibling) == RB_RED){
				SET_RED_COLOR_TREE(parent);
				SET_BLACK_COLOR_TREE(sibling);
				fRoot = rotate_rbnode_right(parent);
				sibling = parent->rb_left;
			}

			//case 3: sibling is black, nephews are black;
			lnephew = sibling->rb_left;
			rnephew = sibling->rb_right;
			if(GET_COLOR_TREE(lnephew) && GET_COLOR_TREE(rnephew)){
				SET_RED_COLOR_TREE(sibling);
				if(GET_COLOR_TREE(parent)){
					pNode = parent;
					continue;
				}else{
					SET_BLACK_COLOR_TREE(parent);
					break;
				}
			}

			//case 4: sibling is black, left nephews is red
			if(GET_COLOR_TREE(sibling) && GET_COLOR_TREE(lnephew) == RB_RED){
				SET_COLOR_TREE(sibling, GET_COLOR_TREE(parent));
				SET_BLACK_COLOR_TREE(parent);
				SET_BLACK_COLOR_TREE(lnephew);
				fRoot = rotate_rbnode_right(parent);
				break;
			}

			//case 5: sibling is black, right nephews is red
			if(GET_COLOR_TREE(sibling) && GET_COLOR_TREE(rnephew) == RB_RED){
				SET_RED_COLOR_TREE(sibling);
				SET_BLACK_COLOR_TREE(rnephew);
				fRoot = rotate_rbnode_left(sibling);
				continue;
			}
		}
	}

	if(IS_ROOT_TREE(fRoot))
		return fRoot;
	else
		return pRoot;
}

static PTreeNode __insert_deal(PTreeNode pRoot, PTreeNode pNode, TREECMP_FUNC cmpFunc)
{
	if(pRoot == NULL){
		SET_ROOT_TREE(pNode);
		return pNode;
	}

	PTreeNode root = pRoot;
	while(1){
		if(cmpFunc(pNode, root) <= 0){
			if(root->rb_left){
				root = root->rb_left;
				continue;
			}
			add_rbnode_left(root, pNode);
			break;
		}else{
			if(root->rb_right){
				root = root->rb_right;
				continue;
			}
			add_rbnode_right(root, pNode);
			break;
		}
	}
	return pRoot;
}

static PTreeNode __erase_aim(PTreeNode pRoot, PTreeNode pNode)
{
	if(pNode->rb_left && pNode->rb_right){
		PTreeNode pCursor = pNode->rb_left;
		while(1){
			if(pCursor->rb_right){
				pCursor = pCursor->rb_right;
			}else{
				//swap_rbnode(pCursor, pNode)
				if(pCursor == pNode->rb_left){
					add_rbnode_left(pNode, pCursor->rb_left);
					add_rbnode_right(pCursor, pNode->rb_right);
					pNode->rb_right = NULL;
					swap_parent_rbnode(GET_PARENT_TREE(pNode), pNode, pCursor, pCursor);
				}else{
					add_rbnode_right(pCursor, pNode->rb_right);
					pNode->rb_right = pCursor->rb_left;
					add_rbnode_left(pCursor, pNode->rb_left);
					add_rbnode_left(pNode, pNode->rb_right);
					pNode->rb_right = NULL;
					swap_parent_rbnode(GET_PARENT_TREE(pNode), pNode, GET_PARENT_TREE(pCursor), pCursor);
				}

				//swap color
				if(GET_COLOR_TREE(pNode)){
					SET_COLOR_TREE(pNode, GET_COLOR_TREE(pCursor));
					SET_BLACK_COLOR_TREE(pCursor);
				}else{
					SET_COLOR_TREE(pNode, GET_COLOR_TREE(pCursor));
					SET_RED_COLOR_TREE(pCursor);
				}


				if(IS_ROOT_TREE(pCursor))
					pRoot = pCursor;

				break;
			}
		}
	}
	return pRoot;
}

static PTreeNode __erase_deal(PTreeNode pRoot, PTreeNode pNode)
{
	if(pRoot == pNode){
		if(pNode->rb_left)
			pRoot = pNode->rb_left;
		else if(pNode->rb_right)
			pRoot = pNode->rb_right;
		else
			pRoot = NULL;
	}

	delete_rbnode(pNode);
	return pRoot;
}

PTreeNode insertRBLeaf(PTreeNode pRoot, PTreeNode pNode, TREECMP_FUNC cmpFunc)
{
	pRoot = __insert_deal(pRoot, pNode, cmpFunc);
	pRoot = __insert_adjust(pRoot, pNode);
	return pRoot;
}

PTreeNode eraseRBLeaf(PTreeNode pRoot, PTreeNode pNode)
{
	pRoot = __erase_aim(pRoot, pNode);
	pRoot = __erase_adjust(pRoot, pNode);
	pRoot = __erase_deal(pRoot, pNode);
	return pRoot;
}

PTreeNode searchRBLeaf(PTreeNode pRoot, PTreeNode pNode, TREECMP_FUNC cmpFunc)
{
	while(1){
		if(cmpFunc(pRoot, pNode) == 0)
			break;

		if(cmpFunc(pRoot, pNode) < 0 && pRoot->rb_left)
			pRoot = pRoot->rb_left;
		else if(cmpFunc(pRoot, pNode) > 0 && pRoot->rb_right)
			pRoot = pRoot->rb_right;
		else
			break;
	}

	return pRoot;
}
