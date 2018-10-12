
#include <osport.h>
//this function is used to format the char string to the argc mode
//this function will changed the original string, used it carefully
//return how many arguments has been 
int string2arg(int *argc, const char *argv[],char *string)
{
	int argvlen = 0;
	int paramnum = 0;
	char *tmp = NULL;
	char bak;
	int len;

	argvlen = *argc;
	*argc = paramnum;
	if(NULL == string)
	{
		return paramnum;
	}

	//use the '\0' to replace the ' '
	len = strlen(string);
	tmp = string;
	while(tmp < (string + len))
	{
		if(*tmp == ' ')
		{
			*tmp = '\0';
		}
		tmp++;
	}
	bak = '\0';
	tmp = string;
	while(tmp < (string + len))
	{
		if((*tmp != '\0')&&(bak =='\0'))
		{
			if(paramnum < argvlen)
			{
				argv[paramnum] = tmp;
				paramnum++;
			}
		}
		bak = *tmp;
		tmp++;
	}
	*argc = paramnum;

	return paramnum;
}


//we use this for the at command
//usage:we use this function to deal the at result as the args format
s32_t  string_split(char *text,char *seperate,char *argv[],s32_t argc)
{
	s32_t result;
	char *s;
	s32_t len,i;
	s = seperate;
	len = strlen(text);
	while(*s != '\0') //make all the charactor in text matching the seperate to 0
	{
		for(i =0;i<len;i++)
		{
			if(text[i]==*s)
			{
				text[i]='\0';
			}
		}
		s++;
	}
	//ok now check the para start
	result = 0;
	s = text;
	while(result <argc)
	{
		//jump the NULL
		while(*s == '\0')
		{
			s++;
		}
		//the s is the start
		if(s < (text + len))
		{
			argv[result] = s;
			result++;
			s = s+strlen(s);
		}
		else
		{
			break;
		}
	}
	return result;
}