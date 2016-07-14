#include "SimpleXMLParser.h"
#define MAX_NUM_OF_SIZE		32727
#define MAX_NUM_OF_ATTRIB	100
#define VarToStr(name) #name

// Methods
XMLTag *new_XMLTag(int size);
XMLTag *allocstrXMLTag(char arg,char *text,XMLTag *Tag);
int skipSpaces(int count);
int skipHeader(int count);

// Variables
char *xmlText=NULL;
int textSize=0,numElem=0,lvlIndex=NULL;
XMLTag *root=NULL;
XMLTag *xmlptr=NULL; 

/* void SetText(const char* text)
* text - XML character stream
* set text from loaded source
*/
void SetText(const char *text)
{
	textSize=strlen(text);

	if(textSize==0)
	{
		printf("Err: Text size is NULL.\n");
		return;
	}
	xmlText=(char *)calloc(textSize+1,sizeof(char));

	strcpy_s(xmlText,textSize+1,text);
}

/* void SetPath(const char* text)
* text - path of the XML file
* get text from a file
*/
void SetPath(const char *text)
{
}

/* int ParseTree()
* Builds the DOM tree using simple charcter parsing
*/
int ParseTree()
{
	int count=0,wordIndex=-1;
	char *ptr=NULL;
	char *element=NULL;
	char lastProcessedElement=NULL;
	BOOL tagOpened=FALSE;
	BOOL bufferFilledTagValue=FALSE,bufferFilledTagName=FALSE;

	if(xmlText==NULL)
	{
		return -1;
	}

	if (!root)
	{
		root=new_XMLTag(MAX_NUM_OF_SIZE);
	}
	else
	{
		return -1;
	}

	xmlptr=root;

	while (TRUE)
	{
		if(*(xmlText+count)=='\0')
			break;
		else if(*(xmlText+count)=='<')
			tagOpened=TRUE;
		else if(*(xmlText+count)<0)
			goto LOOP;

		if (tagOpened)
		{
			if (*(xmlText+count)=='<')
			{								
				wordIndex=0;
				count=skipSpaces(count+1);
				//printf("%d\t%d\t%s-OpenTag\n",lastProcessedElement,lvlIndex,element);
				xmlptr=allocstrXMLTag(lastProcessedElement,element,xmlptr);
				lastProcessedElement=ELEMENT_NAME;
				element=(char*)calloc(MAX_NUM_OF_SIZE,sizeof(char));
				if (*(xmlText+count+1)=='/')
				{
					free(element);
					element=NULL;
					lastProcessedElement=ELEMENT_NULL;
					lvlIndex--;
				}
				else if (*(xmlText+count+1)=='?')
				{
					while (*(xmlText+count++)!='>');
					//count++;
					free(element);
					element=NULL;
					goto LOOP;
				}
				else
				{
					lvlIndex++;
				}
				goto LOOP;
			}
			else if (*(xmlText+count)==' ')
			{
				wordIndex=0;
				count=skipSpaces(count+1);
				//printf("%d\t%d\t%s-Spaces\n",lastProcessedElement,lvlIndex,element);
				xmlptr=allocstrXMLTag(lastProcessedElement,element,xmlptr);
				lastProcessedElement=ELEMENT_ATTRIBN;
				element=(char*)calloc(MAX_NUM_OF_SIZE,sizeof(char));
			}
			else if (*(xmlText+count)=='=')
			{
				wordIndex=0;
				count=skipSpaces(count+1);
				//printf("%d\t%d\t%s-Equals\n",lastProcessedElement,lvlIndex,element);
				xmlptr=allocstrXMLTag(lastProcessedElement,element,xmlptr);
				lastProcessedElement=ELEMENT_ATTRIBV;
				element=(char*)calloc(MAX_NUM_OF_SIZE,sizeof(char));
			}
			else if (*(xmlText+count)=='/')
			{
				if (*(xmlText+count+1)=='>')
				{
					lvlIndex--;
				}
			}
			else if (*(xmlText+count)=='>')
			{
				tagOpened=FALSE;
				count=skipSpaces(count+1);
				//printf("%d\t%d\t%s-CloseTag\n",lastProcessedElement,lvlIndex,element);
				xmlptr=allocstrXMLTag(lastProcessedElement,element,xmlptr);
				lastProcessedElement=ELEMENT_VALUE;		
				free(element);
				element=NULL;	
				element=(char*)calloc(MAX_NUM_OF_SIZE,sizeof(char));
				wordIndex=0;
				goto LOOP;
			}
			else if (element!=NULL)
			{
				*(element+wordIndex++)=*(xmlText+count);
			}
		}
		else
		{
			if (element!=NULL)
				*(element+wordIndex++)=*(xmlText+count);
			else
			{
				count=skipSpaces(count+1);
				element=(char*)calloc(MAX_NUM_OF_SIZE,sizeof(char));
			}
		}
LOOP:
		count++;
	}

EXIT:
	return 0;
}

/* XMLTag newXMLTag()
* Allocates memory for a XMLTag pointer
*/
XMLTag *new_XMLTag(int size)
{
	XMLTag *new_tag=(XMLTag*)calloc(size,sizeof(XMLTag));
	if (new_tag==NULL)
	{
		return NULL;
	}
	new_tag->attribname=(char**)calloc(MAX_NUM_OF_ATTRIB,sizeof(char*));
	new_tag->attributes=(char**)calloc(MAX_NUM_OF_ATTRIB,sizeof(char*));
	return new_tag;
}


/* XMLTag newXMLTag()
* Allocates memory for a XMLTag pointer internals
* sizeName - size should be allocated for name
* sizeValue - size should be allocated for text
*/
XMLTag *allocstrXMLTag(char arg,char *text,XMLTag *Tag)
{
	char *ptr=NULL;
	int index=0,elemLen=0;
	XMLTag *temp=NULL;

	if (!text)
	{
		return Tag;
	}

	elemLen=strlen(text);

	ptr=(char *)calloc(elemLen+2,sizeof(char));

	if (!ptr)
	{
		return Tag;
	}

	strcpy_s(ptr,elemLen+1,text);

	switch (arg)
	{
	case ELEMENT_NAME:
		if (Tag->name)
		{
			Tag++;
			Tag->attribname=(char**)calloc(MAX_NUM_OF_ATTRIB,sizeof(char*));
			Tag->attributes=(char**)calloc(MAX_NUM_OF_ATTRIB,sizeof(char*));
		}
		Tag->name=ptr;
		Tag->level=lvlIndex;
		break;
	case ELEMENT_VALUE:
		Tag->text=ptr;
		break;
	case ELEMENT_ATTRIBN:
		if (*(Tag->attribname))
		{
			while (*(Tag->attribname+index++));
		}
		*(Tag->attribname+index++)=ptr;
		*(Tag->attribname+index)=NULL;
		break;
	case ELEMENT_ATTRIBV:
		if (*(Tag->attributes))
		{
			while (*(Tag->attributes+index++));
		}
		*(Tag->attributes+index++)=ptr;
		*(Tag->attributes+index)=NULL;
		break;
	default:
		printf("Err: Invalid element type.\n");
		break;
	}
	if (!ptr)
	{
		printf("Err: Memory allocation failed.\n");
		return NULL;
	}
	return Tag;
}


int skipSpaces(int count)
{
	int pos=count;
	while (*(xmlText+(pos))==' ')
	{
		pos++;
	}
	return pos-1;
}


XMLTag *getRoot()
{
	return root;
}