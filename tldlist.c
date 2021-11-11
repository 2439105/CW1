/*2471065c
*sp Exercise 1a
*“This is my own work as defined in the Academic Ethics agreement I have signed.”
*/
#include <stdio.h>
//#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "date.h"
#include "tldlist.h"

struct tldnode
{
	char* name_;
	long count_;
	struct tldnode* lchild_;
	struct tldnode* rchild_;
	struct tldnode* parent_;
	int visited_;
};

struct tldlist
{
	long count_;
	Date* begin_;
	Date* end_;
	struct tldnode* nodes_;
};

struct tlditerator
{
	struct tldnode* node_;
};

int search_tag(TLDNode* root, char* key, TLDNode* def, TLDNode** rst) {
	if (!root) {
		*rst = def;
		return 0;
	}
	else if (strcmp(key, root->name_) == 0) {
		*rst = root;
		return 1;
	}
	else if (strcmp(key, root->name_) < 0) {
		return search_tag(root->lchild_, key, root, rst);
	}
	else {
		return search_tag(root->rchild_, key, root, rst);
	}
}

int insert_tag(TLDNode** root, char* key) {
	TLDNode* rst = NULL;
	if (!search_tag((*root), key, NULL, &rst))
	{
		TLDNode* node = (TLDNode*)malloc(sizeof(TLDNode));
		node->name_ = (char*)malloc(strlen(key) + 1);
		memcpy(node->name_, key, strlen(key) + 1);
		node->lchild_ = node->rchild_ = node->parent_ = NULL;
		node->count_ = 1;
		node->visited_ = 0;
		if (!rst) {
			*root = node;
		}
		else if (strcmp(key, rst->name_) < 0)
		{
			rst->lchild_ = node;
			node->parent_ = rst;
		}
		else
		{
			rst->rchild_ = node;
			node->parent_ = rst;
		}
		return 1;
	}
	else
	{
		rst->count_ += 1;
		return 1;
	}
	return 0;
}

void delete_node(TLDNode** root)
{
	TLDNode* q, * s;
	if (!(*root)->lchild_ && !(*root)->rchild_)
	{
		//free(*root);
		*root = NULL;
	}
	else if (!(*root)->lchild_)
	{
		q = *root;
		*root = (*root)->rchild_;
		(*root)->parent_ = NULL;
		free(q);
	}
	else if (!(*root)->rchild_)
	{
		q = *root;
		*root = (*root)->lchild_;
		(*root)->parent_ = NULL;
		free(q);
	}
	else
	{
		q = *root;
		s = (*root)->lchild_;
		while (s->rchild_)
		{
			q = s;
			s = s->rchild_;
		}

		free((*root)->name_);
		(*root)->name_ = (char*)malloc(strlen(s->name_) + 1);
		memcpy((*root)->name_, s->name_, strlen(s->name_) + 1);
		free(s->name_);

		if (q != *root)
		{
			q->rchild_ = s->lchild_;
			s->parent_ = q;
		}
		else
		{
			q->lchild_ = s->lchild_;
			s->lchild_ = q;
		}
		free(s);
	}
}

int delete_tag(TLDNode** root, char* key)
{
	if (!(*root))
	{
		return 0;
	}
	else
	{
		if (strcmp(key, (*root)->name_) == 0)
		{
			delete_node(root);
			return 1;
		}
		else if (strcmp(key, (*root)->name_) < 0)
		{
			return delete_tag(&(*root)->lchild_, key);
		}
		else
		{
			return delete_tag(&(*root)->rchild_, key);
		}
	}
}

/*
 * tldlist_create generates a list structure for storing counts against
 * top level domains (TLDs)
 *
 * creates a TLDList that is constrained to the `begin' and `end' Date's
 * returns a pointer to the list if successful, NULL if not
 */
TLDList* tldlist_create(Date* begin, Date* end)
{
	TLDList* ldlist = (TLDList*)malloc(sizeof(TLDList));
	if (!ldlist)
	{
		fprintf(stderr, "1 - malloc error");
		return NULL;
	}

	ldlist->count_ = 0;
	ldlist->begin_ = begin;
	ldlist->end_ = end;
	ldlist->nodes_ = NULL;

	return ldlist;
}

/*
 * tldlist_destroy destroys the list structure in `tld'
 *
 * all heap allocated storage associated with the list is returned to the heap
 */
void tldlist_destroy(TLDList* tld)
{
	if (!tld)
		return;
	while (tld->nodes_) 
	{
		delete_node(&tld->nodes_);
	}
}

/*
 * tldlist_add adds the TLD contained in `hostname' to the tldlist if
 * `d' falls in the begin and end dates associated with the list;
 * returns 1 if the entry was counted, 0 if not
 */
int tldlist_add(TLDList* tld, char* hostname, Date* d)
{
	if (!tld || !hostname || !d)
	{
		return 0;
	}
	char* dot = strchr(hostname, '.');
	while (dot != NULL)
	{
		hostname = dot + 1;
		if (*hostname == '\0')
			return 0;
		dot = strchr(hostname, '.');
	}

	if (date_compare(d, tld->begin_) < 0 || date_compare(d, tld->end_) > 0)
	{
		return 0;
	}

	if (insert_tag(&(tld->nodes_), hostname))
	{
		tld->count_ += 1;
		return 1;
	}

	return 0;
}

/*
 * tldlist_count returns the number of successful tldlist_add() calls since
 * the creation of the TLDList
 */
long tldlist_count(TLDList* tld)
{
	return (!tld) ? 0 : tld->count_;
}

/*
 * tldlist_iter_create creates an iterator over the TLDList; returns a pointer
 * to the iterator if successful, NULL if not
 */
TLDIterator* tldlist_iter_create(TLDList* tld)
{
	TLDIterator * itor = (TLDIterator*)malloc(sizeof(TLDIterator));
	if (!itor)
	{
		return NULL;
	}
	itor->node_ = tld->nodes_;
	return itor;
}

/*
 * tldlist_iter_next returns the next element in the list; returns a pointer
 * to the TLDNode if successful, NULL if no more elements to return
 */
TLDNode* tldlist_iter_next(TLDIterator* iter)
{
	TLDNode* curr = iter->node_;
	if (curr)
	{
		curr->visited_ = 1;

		if (curr->lchild_ && !curr->lchild_->visited_)
			iter->node_ = curr->lchild_;
		else if (curr->rchild_ && !curr->rchild_->visited_)
			iter->node_ = curr->rchild_;
		else
		{
			while (iter->node_->parent_ && (!iter->node_->parent_->rchild_ || iter->node_->parent_->rchild_->visited_))
			{
				iter->node_ = iter->node_->parent_;
			}
			if (iter->node_->parent_ && iter->node_->parent_->rchild_ && !iter->node_->parent_->rchild_->visited_)
			{
				iter->node_ = iter->node_->parent_->rchild_;
			}
			else
			{
				if (!iter->node_->parent_)
					iter->node_ = NULL;
			}
		}
	}
	else
		iter->node_ = NULL;

	return curr;
}

/*
 * tldlist_iter_destroy destroys the iterator specified by `iter'
 */
void tldlist_iter_destroy(TLDIterator* iter)
{
	if (iter)
	{
		iter->node_ = NULL;
		free(iter);
	}
}

/*
 * tldnode_tldname returns the tld associated with the TLDNode
 */
char* tldnode_tldname(TLDNode* node)
{
	return (!node) ? 0 : node->name_;
}

/*
 * tldnode_count returns the number of times that a log entry for the
 * corresponding tld was added to the list
 */
long tldnode_count(TLDNode* node)
{
	return (!node) ? 0 : node->count_;
}
