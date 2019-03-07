/* This is a library module supporting lists.  A list has a current position
which is thought of as being between items, and can thus be anywhere from
before the first item to after the last item.  Every operation is constant time.

The items stored in a list are of type 'item'.  The typedef for this can be
changed for any particular application so that 'item' stands for any desired
type, e.g. a raw type such as int, char[20], struct... or pointer type such as
char *, struct...*.

The lists provided by this module are not thread safe.
*/
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

// The type of items stored in the list. Change this for different applications.

void fail(char *message){
  printf("%s \n", message);
  fail("error \n");
}

struct node {
  item x ;
  struct node *next;
  struct node *prev;
};


typedef struct node node;

struct list{
  node *first;
  node *current;
  node *last;
};

// Create a new empty list.
list *newList(){
  list *l = malloc(sizeof(list));
  node *n1 = malloc(sizeof(node));
  node *n2 = malloc(sizeof(node));
  n1->next = n2;
  n2->prev = n1;
  l->first = n1;
  l->current = l->first;
  l->last = n2;
  return l;
}

// Set the current position before the first item or after the last item, e.g.
// to begin a forward or backward traversal.
void start(list *l){
l->current = l->first;
}

void end(list *l){
l->current = l->last->prev;
}

// Check whether the current position is at the start or end, e.g. to test
// whether a traversal has finished.
bool atStart(list *l){
if (l->current == l->first)
  return true;
else
  return false;
}

bool atEnd(list *l){
  if (l->current == l->last->prev)
    return true;
  else
    return false;
}

// Move the current position one place forwards or backwards.  It is an error
// to call forward when at the end of the list, or backward when at the start.
void forward(list *l){
if (atEnd(l) == false)
l->current = l->current->next;
else
  fail("error\n");
}

void backward(list *l){
if (atStart(l) == false)
l->current = l->current->prev;
else
fail("error\n");

}
void insertEmpty(list *l, item x){
node *n = malloc(sizeof(node));
n->x = x;
n->next = l->last;
n->prev = l->first;
l->current = n;

}



// Insert an item before or after the current position (i.e. at the current
// position, but with the current position ending up after or before the new
// item).
void insertBefore(list *l, item x){
  node *n = malloc(sizeof(node));
  n->x = x;
  n->next = l->current->next;
  l->current->next->prev = n;
  n->prev = l->current;
  l->current->next = n;
  l->current = n;
}

void insertAfter(list *l, item x){
node *n = malloc(sizeof(node));
n->x = x;
n->next = l->current->next;
l->current->next->prev = n;
n->prev = l->current;
l->current->next = n;
}

// Get the item before or after the current position. It is an error to call
// getBefore when at the start, or getAfter when at the end.
item getBefore(list *l){
if (atStart(l) == true)
  fail("error\n");
return l->current->x;
}

item getAfter(list *l){
if (atEnd(l) == true)
  fail("error\n");
return l->current->next->x;
}

// Set the item before or after the current position. It is an error to call
// setBefore when at the start, or setAfter when at the end.
void setBefore(list *l, item x){
if (atStart(l) != true)
  l->current->x = x;
else fail("error\n");
}
void setAfter(list *l, item x){
if (atEnd(l) != true)
    l->current->next->x = x;
else fail ("error\n");
}

// Delete the item before or after the current position. It is an error to call
// deleteBefore when at the start, or deleteAfter when at the end.
void deleteBefore(list *l){
if (atStart(l))
fail("error/n");
l->current->prev->next= l->current->next;
l->current->next->prev = l->current->prev;
free(l->current);
l->current = l->current->prev;


}
void deleteAfter(list *l){
if (atEnd(l) == true)
fail("error\n");
l->current->prev->next = l->current->next;
l->current->next->prev = l->current->prev;
free(l->current);
}

//--------------Testing -------------------------------------------------------------

//Testing Start and End
void testStartAndEnd(){
  list *l = newList();
  assert(atStart(l));
  assert(atEnd(l));
  start(l);
  assert(atStart(l));
  end(l);
  assert(atEnd(l));
}
//Testing Forward and Backward
void testFwdandBwd(){
list *l = newList();
insertEmpty(l,50);
insertAfter(l,51);
forward(l);
assert(l->current->x == 51);
backward(l);
assert(l->current->x == 50);
}
//Testing Insert Before
void testInsertBefore(){
list *l = newList();
assert(atStart(l));
insertBefore(l,30);
assert(l->current->x == 30);
assert(!atStart(l));
}
//Testing Insert After
void testInsertAfter(){
list *l = newList();
assert(atEnd(l));
insertAfter(l,44);
assert(l->current->next->x == 44);
assert(!atEnd(l));
}
//Testing Get Before
void testGetbefore(){
list *l = newList();
insertEmpty(l,39);
insertBefore(l,20);
assert(atStart(l)== false);
assert(getBefore(l) == 20);
assert(atEnd(l));
}
//Testing Get After
void testGetAfter(){
list *l = newList();
insertAfter(l,25);
insertAfter(l,19);
assert(atEnd(l) == false);
assert(getAfter(l) == 19);
assert(atEnd(l) == false);
}
//Testing Set before
void testSetBefore(){
list *l = newList();
insertAfter(l,13);
insertBefore(l,22);
assert(atStart(l) == false);
setBefore(l,22);
assert(getBefore(l) == 22);
assert(l->current->x == 22);
assert(atStart(l) == false);
}
//Testing Set After
void testSetAfter(){
list *l = newList();
insertAfter(l,15);
insertAfter(l,32);
assert(atEnd(l) == false);
setAfter(l,32);
assert(getAfter(l) == 32);
assert(l->current->next->x == 32);
assert(atEnd(l) == false);
}
void testDeleteBefore(){
list *l = newList();
insertEmpty(l,33);
insertBefore(l,14);
assert(!atStart(l));
deleteBefore(l);
assert(l->current->x == 33);
assert(!atStart(l));
}

void testDeleteAfter(){
list *l = newList();
insertEmpty(l,23);
insertAfter(l,45);
assert(!atEnd(l));
deleteAfter(l);
assert(l->current->next->prev == l->current->prev);
}
//List Main
int listMain(){
testStartAndEnd();
testFwdandBwd();
testInsertBefore();
testInsertAfter();
testGetbefore();
testGetAfter();
testSetBefore();
testSetAfter();
testDeleteAfter();
testDeleteBefore();
printf("all tests passed\n");
}
