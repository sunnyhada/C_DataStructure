#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*사용자 선택*/

#define QUIT 	1
#define INSERT	2
#define DELETE	3
#define PRINT	4
#define SEARCH	5

/*자료구조 1. 단일 방향 연결 리스트 (Singly linked list)*/

///////////////////////////////////////////////////////////////////////////////
//LIST type definition

/*node*/

typedef struct node
{
	int data;
	struct node *link;
}NODE;

/*list*/

typedef struct list
{
	int 	cnt;
	NODE	*pos;
	NODE	*head;
	NODE	*rear;
}LIST;

////////////////////////////////////////////////////////////////////////////////
//Prototype declarations

/*
 Allocate dynamic memory for a list head node and returns its address to caller
 	return head node pointer
		NULL if overflow
*/

LIST *createList(void)
{
	LIST *list = (LIST*)malloc(sizeof(LIST));
	list -> cnt = 0;
	list -> head = NULL;

	if(!list) /*if overflow */
		return NULL;
	return list;
}

/*
 Deletes all data in list and recycles memory
 	 return NULL head pointer
 */

LIST *destroyList(LIST *pList)
{
	if(pList -> head == NULL)
		return pList;

	NODE *pNode = pList -> head;
	NODE *freeNode = pList -> head -> link;
	
	int i = 0;

	while ( i < pList -> cnt )
	{
		//pNode -> link 가 NULL이면 리스트의 끝이므로 pNode를 free하고 종료//
		if ( pNode -> link == NULL )
		{
			pNode -> data = 0;
			free(pNode);
			pList -> cnt = 0;
			return pList;
		}

		else
		{
			freeNode = pNode;
			pNode = pNode -> link;	
	
			free(freeNode);
		
			i++;
		}
	}
	
	free(pNode);
	pList -> cnt = 0;
	return pList;
}

/*
 Inserts data into list
 	return -1 (if overflow)
		0 (if successful)
		1 (if dupe key) --- 중복 허용 x
*/

int addNode(LIST *pList, int dataIn)
{
	NODE *nNode = (NODE*)malloc(sizeof(NODE));
	nNode -> data = dataIn;
	nNode -> link = NULL;

	NODE *pNode =	pList -> head;

	NODE *preNode = pList -> head;


	int i = 0;

	int dataOut;

	if (nNode == NULL)
		return -1;

	if(pList -> cnt == 0 && pList -> head == NULL)
	{
		pList -> head = nNode;
		pList -> pos = nNode;
		pList -> rear = nNode;
		pList->cnt++;
		return 0;
	}

	else
	{
		while(i < pList -> cnt)
		{
			//포인터가 가리키는 데이터보다 클 경우, 포인터의 위치를 한칸 오른쪽으로//
			if(pNode -> data < nNode -> data)
			{
				//포인터가 가리키는 값이 rear(끝값)인데 nNode 데이터가 큰 경우 -> 끝에 삽입 //
				if (pNode -> link == NULL)
				{
					pNode -> link = nNode;
					pList -> rear = nNode;
				
					pList->cnt += 1;
					return 0;
				}
				preNode = pNode;
				pNode = pNode -> link;
			}
			
			//포인터가 가리키는 데이터와 중복될 경우//
			if(pNode -> data == nNode -> data)
			{
				fprintf(stdout, "duplicated error");
				free(nNode);
				return 1;
			}
			
			//포인터가 가리키는 데이터보다 작을 경우, 삽입//
			if(pNode -> data > nNode -> data)
			{
				//포인터가 가리키는 데이터가 head일 경우//
				if(pNode == pList -> head)
				{
					nNode -> link = pNode;
					pList -> head = nNode;
					pList -> cnt ++;
					return 0;
				}
				nNode -> link = pNode;
				preNode -> link = nNode;

				pList->cnt++;
				return 0;
			}
			i++;
		}
		
	}


}


/* 
interface to search fuction
	Argu	key being sought
       dataOut	contains found data
	return 1 (successful)
 		0 (not found)
*/

int searchList(LIST *pList, int Argu, int *dataOut)
{
	NODE *pNode = pList -> head;
	int i = 0;
	while( i < pList -> cnt )
	{
		if(pNode -> data == Argu)
		{
			*dataOut = pNode -> data;
			return 1;
		}

		else
			pNode = pNode -> link;
			i++;
	}

	return 0;
}


/*
 Remove data from list
	return 0 (not found)
	       1 (deleted, successful)
*/


int removeNode(LIST *pList, int Key, int *dataOut)
{
	NODE *pNode = pList -> head;
	NODE *preNode = pNode;

	int i = 0;

	if( !searchList(pList, Key, dataOut) )
	{
		fprintf(stdout, "there is no %d \n", Key);
		return 0;
	}

	while ( i < pList -> cnt )
	{
		if(pNode -> data == Key)
		{
			//head일 경우//
			if(pNode == pList -> head)
			{
				//뒤에 값이 있을 경우//
				if (pNode -> link != NULL)
				{
					pList -> head = pNode -> link;
					free(pNode);
					pList -> cnt --;
					return 1;
				}
				pList -> head = NULL;
				free(pNode);
				pList -> cnt --;
				return 1;
			}
				
			*dataOut = pNode -> data;
			if(pNode -> link == NULL)
			{
				preNode -> link = NULL;
				free(pNode);
				pList -> cnt --;
				return 1;
			}
			preNode -> link = pNode -> link;
			free(pNode);
			pList -> cnt --;
			return 1;
		}

		preNode = pNode;
		pNode = pNode -> link;

	}
	
}
/*
 return number of nodes in list
*/

int listCount(LIST *pList)
{
	return pList -> cnt;
}

/*
 return 1 (empty)
 	0 (list has data)
*/

int emptyList(LIST *pList)
{
	if (pList -> cnt == 0 && pList -> head == NULL)
		return 1;
	else
		return 0;
}

/*
 printf data from list
 */

void printList(LIST *pList)
{
	int i = 0;
	NODE *pNode = pList -> head;
	
	if (emptyList(pList))
		fprintf(stdout, "\n");

	while ( i < pList -> cnt)
	{
		fprintf(stdout, "%d ", pNode -> data);
		pNode = pNode -> link;
		i++;
	}

	fprintf(stdout, "\n");
}


/*
 get user's input
 */

int get_action()
{
	char ch;

	scanf("%c", &ch);
	ch = toupper(ch);
	
	switch(ch)
	{
		case 'Q' :
			return QUIT;
		case 'P' :
			return PRINT;
		case 'I' :
			return INSERT;
		case 'D' :
			return DELETE;
		case 'S' :
			return SEARCH;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////

int main(void)
{
	int num;
	LIST *list;
	int data;

	/*create a null list*/
	list = createList();
	if(!list)
	{
		printf("Cannot create list\n");
		return 100;
	}

	fprintf(stdout, "Select Q)uit, P)rint, I)nsert, D)elete, S)earch : ");

	while(1)
	{
		int action = get_action();

		switch(action)
		{
			case QUIT :
				destroyList(list);
				free(list);
				return 0;
			case PRINT :
				printList(list);
				break;
			case INSERT :
				fprintf(stdout, "Enter a number to insert : ");
				fscanf(stdin, "%d", &num);
				
				addNode(list, num);
				printList(list);
				break;
			case DELETE :
				fprintf(stdout, "Enter a number to delete : ");
				fscanf(stdin, "%d", &num);

				removeNode(list, num, &data);
				printList(list);
				break;
			case SEARCH :
				fprintf(stdout, "Enter a number to retrieve : ");
				fscanf(stdin, "%d", &num);

				int found;
				found = searchList(list, num, &data);
				if(found) fprintf(stdout, "Found : %d \n", data);
				else fprintf(stdout, "Not founde %d \n", num);
				
				break;
		}
		if(action)
			fprintf(stdout, "Select Q)uit, P)rint, I)nsert, D)elete, S)earch : ");
	}

	return 0;
}
