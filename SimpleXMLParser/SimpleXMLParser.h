#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ELEMENT_NULL	0
#define ELEMENT_TAG		1
#define ELEMENT_NAME	2
#define ELEMENT_VALUE	3
#define ELEMENT_ATTRIBN	4
#define ELEMENT_ATTRIBV	5

typedef struct __tag
{
	char level;
	char *name;
	char **attribname;
	char **attributes;
	char *text;
	//struct __tag *child;
} XMLTag;

typedef enum __BOOL
{
	TRUE=1,	FALSE=0
} BOOL;

#ifdef __cplusplus
extern "C"
{
#endif

void SetText(const char *text);

void SetPath(const char *text);

int ParseTree();

XMLTag *getRoot();

#ifdef __cplusplus
}
#endif