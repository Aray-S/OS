#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"

/*define the data stucture and variables*/
struct map{
	unsigned long m_size;
	char *m_addr;
	struct map *prior,*next;
};

struct map *x_TableStart;		/* Pointer of the free space table */
unsigned long x_WholeMemSize=1024;	/* The size of available memory */
char *Start;					/* The beginning address of the free space */

void Initialize();
	void Order();
	void ErrorOrder();
	char *ApplyMem(unsigned long x_MemAmount);
	void ErrorApply();
	void MemFree(unsigned long x_FreeAddr,unsigned long x_MemAmount);
	void ErrorFree();
	void PrintTable();
	void EndProgram();

void Initialize()
{/*initialize the free space table*/
	char out[50]="The initialization work has been finished! \n";

	x_TableStart=(struct map *)malloc(sizeof(struct map));
	x_TableStart->m_addr=Start;
	x_TableStart->m_size=x_WholeMemSize;
	x_TableStart->next=x_TableStart;
	x_TableStart->prior=x_TableStart;

	puts(out);
	PrintTable();
}

void Order()
{
	char x_UserOrder[30];	/* to save user's order */
	int x_MallocIdt=0;
	int x_FreeIdt=0;
	int x_EndIdt=0;			/* above 3 to identify user's order */

	int i=0;				/* to record the times */

	char x_Malloc[8]="lmalloc ";
	char x_Free[6]="lfree ";
	char x_End[4]="end\0";

	for(;;)
	{
		/*To get the input of a user  a */
		printf("Command:\\>");
		gets(x_UserOrder);
		x_MallocIdt=0;
		x_FreeIdt=0;
		x_EndIdt=0;

		/*To identify the order*/

		for(i=0;i<=7;i++)
		{/* identify malloc a  */
			if(x_Malloc[i]==x_UserOrder[i])
			{
				x_MallocIdt++;
			}
		}

		for(i=0;i<=5;i++)
		{/* identify free  a */
			if(x_Free[i]==x_UserOrder[i])
			{
				x_FreeIdt++;
			}
		}

		for(i=0;i<=3;i++)
		{/* identify end  a */
			if(x_End[i]==x_UserOrder[i])
			{
				x_EndIdt++;
			}
		}

		if(x_MallocIdt==8)
		{/* The order "lmalloc" a */
			unsigned long x_MemAmount=0;
			i=8;

			for(;((x_UserOrder[i]>='0')&&(x_UserOrder[i]<='9'));)
			{/* To save the size of the space applied for) a */
				x_MemAmount=x_MemAmount*10+(x_UserOrder[i]-'0');
				i++;
			}
			if((x_UserOrder[i]!='\0')||(x_MemAmount==0))
			{/* invalid input  a */
				ErrorOrder();
			}
			else
			{
				ApplyMem(x_MemAmount);
			}
		}
		else if(x_FreeIdt==6)
		{/* The order "lfree"  a */
			unsigned long x_FreeAddr=0;
			i=6;

			for(;(x_UserOrder[i]>='0')&&(x_UserOrder[i]<='9');)
			{/* To save the physical relative address ) a */
				x_FreeAddr=x_FreeAddr*10+(x_UserOrder[i]-'0');
				i++;
			}
			if(x_UserOrder[i]!=' ')
			{/* invalid input  a */
				ErrorOrder();
			}
			else
			{/* address is OK  a */
				unsigned long x_MemAmount=0;

				for(i++;(x_UserOrder[i]>='0')&&(x_UserOrder[i]<='9');)
				{/* To save the size of the space want to free) a */
					x_MemAmount=x_MemAmount*10+(x_UserOrder[i]-'0');
					i++;
				}

				if((x_UserOrder[i]!='\0')||(x_MemAmount==0))
				{/* invalid input  a */
					ErrorOrder();
				}
				else
				{
					MemFree(x_FreeAddr,x_MemAmount);
				}
			}
		}
		else if(x_EndIdt==4)
		{/* The order "end"  a */
			EndProgram();
			break;
		}
		else
		{/* The others a */
			ErrorOrder();
		}
		PrintTable();
	}
}

void ErrorOrder()
{/* give out error message  a */
	char OrderError[70]="\nYour input is invalid, please input in standard way.\n";
	char x_StdWay[70]="\nThat is, lmalloc(lfree) size(address) (free size) or end.\n";
	puts(OrderError);
	puts(x_StdWay);
}

char *ApplyMem(unsigned long x_MemAmount)
{
	char *x_Return;					/* To return the pointer that point to the free space.*/
	struct map *x_TableBrws,*x_MapFree;		/* TableBrowser */

	x_TableBrws=x_TableStart;

	if (x_TableStart!=NULL)
	{/* The table is not empty a */
		x_Return=x_TableStart->m_addr;
		do
		{/* look for the proper space */
			if((x_TableBrws->m_size)>=x_MemAmount)
			{/* Then allocate the momery a */

				x_Return=x_TableBrws->m_addr;
				x_TableBrws->m_size-=x_MemAmount;
				x_TableBrws->m_addr=x_TableBrws->m_addr+x_MemAmount;

				if(x_TableBrws->m_size==0)
				{/* about to delete the node */

					x_MapFree=x_TableBrws;
					if(x_TableBrws->next==x_TableBrws->prior)
					{/* only one node left to be deleted a */
						x_TableStart=NULL;
						x_MapFree=x_TableBrws;
					}
					else
					{/* More than one node left to be deleted */
						x_TableBrws=x_TableBrws->prior;
						x_TableBrws->next=x_MapFree->next;
						x_TableBrws=x_TableBrws->next;
						x_TableBrws->prior=x_MapFree->prior;
						x_TableStart=x_TableBrws;
					}
					free(x_MapFree);	/* free the node a */
				}
				printf("\nAllocating Succeed! The Given Address is %u \n",(x_Return-Start));
				return x_Return;
			}
			x_TableBrws=x_TableBrws->next;	/* next node a */
		}while(x_TableBrws!=x_TableStart);			/* to tell if the iterator return to the start */
	}
	ErrorApply();
	return NULL;
}

void ErrorApply()
{/* to deal with the error taking place in allocating memory */
	char x_ApplyError[]="\nNot enough memory, please free or apply for less memory!\n";
	puts(x_ApplyError);
}

void MemFree(unsigned long x_FreeAddr,unsigned long x_MemAmount)
{/* To free the memory asked */

	struct map *x_TableBrws,*x_BrwsHelp,*x_NodeNew,*x_NodeFree;
	char *x_FreeBegin,*x_FreeEnd;	/* to record the beginning and end of the space to be freed */
	unsigned long x_FreeSize=0;
	char *x_PrevAddr,*x_NextAddr;	/* save the Addr pointer maybe used */
	unsigned long x_PrevSize=0;		/* the free size of the previous node */
	unsigned long x_NextSize=0;		/* the free size of the latter node */
	int x_Error=0;					/* to tell whether there are any mistakes */

	/* To get the char * pointer pointing to the start and the end of the space to be freed */
	x_TableBrws=x_TableStart;
	x_BrwsHelp=x_TableStart;
	x_FreeBegin=Start+x_FreeAddr;
	x_FreeEnd=x_FreeBegin+x_MemAmount;

	if((x_FreeBegin<Start)||(x_FreeEnd>(Start+1024)))
	{/* The address to free the memory from out of the free memory range */
		x_Error=1;
	}
	else
	{/* The address is in the range */

		if(x_TableBrws==NULL)
		{/* The table is empty  a */
			x_TableStart=(struct map *)malloc(sizeof(struct map));
			x_TableStart->m_addr=x_FreeBegin;
			x_TableStart->m_size=x_MemAmount;
			x_TableStart->next=x_TableStart;
			x_TableStart->prior=x_TableStart;
			x_Error=-1;
		}
		else
		{/* Not empty */
			do
			{/* Check if the address is the same as the address already in the list */
				if(x_FreeBegin==(x_TableBrws->m_addr))
				{
					x_Error=1;
				}
			x_TableBrws=x_TableBrws->next;
			}while(x_TableBrws!=x_TableStart);

			if(x_Error!=1)
			{/* The address has no error by now  a */
				do
				{
					x_TableBrws=x_TableBrws->next;
					x_PrevAddr=x_BrwsHelp->m_addr;
					x_NextAddr=x_TableBrws->m_addr;

					if(x_PrevAddr>x_NextAddr)
					{/* x_BrwsHelp is pointing to the last free space node  a */
						x_PrevSize=x_BrwsHelp->m_size;
						x_NextSize=x_TableBrws->m_size;

						if((x_FreeBegin>(x_PrevAddr+x_PrevSize))||(x_FreeEnd<x_NextAddr))
						{/* To add a new node */
							x_NodeNew=(struct map *)malloc(sizeof(struct map));
							x_NodeNew->m_size=x_MemAmount;
							x_NodeNew->m_addr=x_FreeBegin;
							x_NodeNew->next=x_TableBrws;
							x_NodeNew->prior=x_BrwsHelp;
							x_BrwsHelp->next=x_NodeNew;
							x_TableBrws->prior=x_NodeNew;
							x_TableStart=x_NodeNew;
							x_BrwsHelp=x_TableStart;
							x_TableBrws=x_TableStart;
							x_Error=-1;					/* Work Succeed */
							break;
						}
						else if(x_FreeBegin==(x_PrevAddr+x_PrevSize))
						{/* To be combinated with the previous one */
							x_BrwsHelp->m_size=x_BrwsHelp->m_size+x_MemAmount;
							x_TableStart=x_BrwsHelp;
							x_TableBrws=x_TableStart;
							x_Error=-1;
							break;
						}
						else if(x_FreeEnd==x_NextAddr)
						{/* To be combinated with the latter one */
							x_TableBrws->m_addr=x_FreeBegin;
							x_TableBrws->m_size+=x_MemAmount;
							x_BrwsHelp=x_TableBrws;
							x_TableStart=x_TableBrws;
							x_Error=-1;
							break;
						}
					}
					else if(x_PrevAddr==x_NextAddr)
					{/* There are only one node in the free space node table  a */
						x_PrevSize=x_BrwsHelp->m_size;

						if(x_FreeBegin>(x_PrevAddr+x_PrevSize)||(x_FreeEnd<x_PrevAddr))
						{/* Add the new node as the first or the last node  a */
							x_NodeNew=(struct map *)malloc(sizeof(struct map));
							x_NodeNew->m_size=x_MemAmount;
							x_NodeNew->m_addr=x_FreeBegin;
							x_NodeNew->next=x_TableBrws;
							x_NodeNew->prior=x_TableBrws;
							x_TableBrws->next=x_NodeNew;
							x_TableBrws->prior=x_NodeNew;
							x_TableStart=x_NodeNew;
							x_BrwsHelp=x_TableStart;
							x_TableBrws=x_NodeNew;
							x_Error=-1;
							break;
						}
						else if(x_FreeBegin==(x_PrevAddr+x_PrevSize))
						{/* Combinate with the node and change the size */
							x_TableBrws->m_size+=x_MemAmount;
							x_Error=-1;
							break;
						}
						else if(x_FreeEnd==x_PrevAddr)
						{/* Combinate with the node and change both size and addr */
							x_TableBrws->m_addr=x_FreeBegin;
							x_TableBrws->m_size+=x_MemAmount;
							x_Error=-1;
							break;
						}
					}
					else
					{/* The general condition a */

						x_PrevSize=x_BrwsHelp->m_size;
						x_NextSize=x_TableBrws->m_size;

						if((x_FreeBegin==(x_PrevAddr+x_PrevSize))&&(x_FreeEnd==x_NextAddr))
						{/* the new unit can be combinated with both the former and the latter one  a */
							x_BrwsHelp->m_size+=(x_MemAmount+x_NextSize);
							x_NodeFree=x_TableBrws;
							x_TableBrws=x_TableBrws->next;
							x_BrwsHelp->next=x_TableBrws;
							x_TableBrws->prior=x_BrwsHelp;
							free(x_NodeFree);
							x_TableStart=x_BrwsHelp;
							x_TableBrws=x_TableStart;
							x_Error=-1;
							break;
						}
						else if((x_FreeBegin==(x_PrevAddr+x_PrevSize))&&(x_FreeEnd<x_NextAddr))
						{/* the new unit can be combinated with the former one  a */
							x_BrwsHelp->m_size+=x_MemAmount;
							x_TableStart=x_BrwsHelp;
							x_TableBrws=x_TableStart;
							x_Error=-1;
							break;
						}
						else if((x_FreeBegin>(x_PrevAddr+x_PrevSize))&&(x_FreeEnd==x_NextAddr))
						{/* the new unit can be combinated with the latter one  a */
							x_TableBrws->m_addr=x_FreeBegin;
							x_TableBrws->m_size+=x_MemAmount;
							x_TableStart=x_TableBrws;
							x_BrwsHelp=x_TableBrws;
							x_Error=-1;
							break;
						}
						else if((x_FreeBegin>(x_PrevAddr+x_PrevSize))&&(x_FreeEnd<x_NextAddr))
						{/* need a new node  a */
							x_NodeNew=(struct map *)malloc(sizeof(struct map));
							x_NodeNew->m_size=x_MemAmount;
							x_NodeNew->m_addr=x_FreeBegin;
							x_NodeNew->next=x_TableBrws;
							x_NodeNew->prior=x_BrwsHelp;
							x_BrwsHelp->next=x_NodeNew;
							x_TableBrws->prior=x_NodeNew;
							x_TableStart=x_NodeNew;
							x_BrwsHelp=x_TableStart;
							x_TableBrws=x_TableStart;
							x_Error=-1;
							break;
						}
					}
					x_BrwsHelp=x_TableBrws;
				}while(x_TableBrws!=x_TableStart);
			}
		}
	}
	if(x_Error!=-1)
	{/* if there is no work that succeed */
		ErrorFree();
	}
}


void ErrorFree()
{
	char x_FreeError[]="\nInvalid Address or size, please retry!\n";
	puts(x_FreeError);
}

void PrintTable()
{/* To output the free space table */
	struct map *x_TableBrws;		/* iterator */
	int i=1;

	x_TableBrws=x_TableStart;

	if(x_TableStart!=NULL)
	{/* Not Empty */
		printf("No.    Address        Size\n");
		do
		{/* list the table one unit by one unit */
			printf("%d      %u       %u \n",i++,(x_TableBrws->m_addr-Start),x_TableBrws->m_size);
			x_TableBrws=x_TableBrws->next;
		}while(x_TableBrws!=x_TableStart);
	}
	else
	{/* Empty */
		printf("\nNo memory is available now!");
	}
	printf("\n");
}

void EndProgram()
{
	struct map *x_TableBrws;			/* iterator */

	if(x_TableStart!=NULL)
	{/* to free the nodes one by one */
		if((x_TableStart->next)==x_TableStart)
		{/* only one node */
			free(x_TableStart);
		}
		else
		{/* more than 1 node */
			x_TableBrws=x_TableStart;
			do
			{/* modify the prev pointer */
				x_TableBrws->prior=NULL;
			}while(x_TableBrws->next==x_TableStart);
			x_TableBrws->next=NULL;
			x_TableBrws->prior=NULL;
			x_TableBrws=x_TableStart;
			do
			{/* delete the nodes */
				x_TableStart=x_TableStart->next;
				free(x_TableBrws);
				x_TableBrws=x_TableStart;
			}while(x_TableStart!=NULL);
		}
	}
	free(Start);		/* free the space */
}


void main()
{
	/*declare the sub-function*/
	void Initialize();
	void Order();
	void ErrorOrder();
	char *ApplyMem(unsigned long x_MemAmount);
	void ErrorApply();
	void MemFree(unsigned long x_FreeAddr,unsigned long x_MemAmount);
	void ErrorFree();
	void PrintTable();
	void EndProgram();

	/*apply for the memory space*/
	Start=(char *)malloc(x_WholeMemSize);


	/*finish the task of the problem*/
	Initialize(Start,x_WholeMemSize,x_TableStart);
	Order(Start,x_TableStart);
}
