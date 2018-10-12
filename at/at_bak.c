/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/


#include <stdlib.h>
#include <string.h>
#include <at.h>
//at control block here
struct at_cmd
{
    void *nxt;     
    u32_t       flags;
    semp_t      semp;              //do the sync with the semp
    u8_t       *cmd;
    s32_t       cmdlen;
    const char *index;
    u8_t       *respbuf;           //which used to storage the responese
    s32_t       respbuflen;
    s32_t       respdatalen;
};

struct at_oob
{
    void       *nxt;
    fnoob       func;
    const char *index;
};

struct at_cb
{
    fnframe_read            read;   //the member function to read a frame from the at module
    fnframe_write           write;  //the member function to write a frame to the at module

    mutex_t                 mutex;  //used to protect the command list
    struct at_cmd          *cmdlst; 
    struct at_oob          *ooblst;
    u8_t                   *rcvbuf;
    s32_t                   rcvbuflen;
};
static struct at_cb g_at_cb;

static s32_t __cmd_send(u8_t *buf,s32_t buflen,u32_t timeout)
{
    s32_t ret = 0;
    if(NULL != g_at_cb.write)
    {
        ret = g_at_cb.write(buf,buflen,timeout);
        if(ret > 0)
        {
            printf("snd %d bytes:%s\n\r",ret,buf);
        }  
    }   
    return ret;
}

static s32_t __resp_rcv(u8_t *buf,s32_t buflen)
{
    s32_t ret = 0;
    if(NULL != g_at_cb.write)
    {
        ret = g_at_cb.read(buf,buflen,LOS_WAIT_FOREVER);
        if(ret > 0)
        {
            printf("rcv %d bytes:%s\n\r",ret,buf);
        }    
    }   
    return ret;
}

//create a command
static struct at_cmd *__cmd_create(u8_t *cmd,s32_t cmdlen,const char *index,u8_t *respbuf,s32_t respbuflen)
{
     struct at_cmd *ret = NULL;

     ret = malloc(sizeof(struct at_cmd));
     if(NULL != ret)
     {
         memset(ret,0,sizeof(struct at_cmd));
         if(semp_create(&ret->semp,1,0))
         {
             ret->cmd = cmd;
             ret->cmdlen = cmdlen;
             ret->index = index;
             ret->respbuf = respbuf;
             ret->respbuflen = respbuflen;
         }
         else
         {
             free(ret);
             ret = NULL;
         }
     }
     return ret;
}

static void __cmd_delete( struct at_cmd *cmd)
{
    semp_del(&cmd->semp);
    free(cmd);
}

static void __cmd_queue(struct at_cmd *cmd)
{
    if(mutex_lock(g_at_cb.mutex))
    {
        cmd->nxt = g_at_cb.cmdlst;
        g_at_cb.cmdlst = cmd;

        mutex_unlock(g_at_cb.mutex);
    }
}

static void __cmd_remove(struct at_cmd *cmd)
{
    struct at_cmd *tmp;
    if(mutex_lock(g_at_cb.mutex))
    {
        if(g_at_cb.cmdlst == cmd)
        {
            g_at_cb.cmdlst =cmd->nxt;
        }
        else
        {
            while(NULL != tmp)
            {
                if(tmp->nxt == cmd)
                {
                    tmp = cmd->nxt;
                    break;
                }
                else
                {
                    tmp = tmp->nxt;
                }
            }
        }
        mutex_unlock(g_at_cb.mutex);
    }
}

static bool_t  __cmd_match(u8_t *data,s32_t len)
{
    bool_t ret = false;
    s32_t  cpylen;
    struct at_cmd *cmd = NULL;
    if(mutex_lock(g_at_cb.mutex))
    {
        cmd = g_at_cb.cmdlst;
        while(NULL != cmd)
        {
            if(NULL  != strstr((const char *)data,cmd->index))
            {
                if(NULL != cmd->respbuf)
                {
                    cpylen = len > cmd->respbuflen?cmd->respbuflen:len;
                    memcpy(cmd->respbuf,data,cpylen);
                    cmd->respdatalen = cpylen;
                }
                semp_post(cmd->semp);
                ret = true;
                break;
            }
            else
            {
                cmd = cmd->nxt;
            }
        }
        mutex_unlock(g_at_cb.mutex);
    }
    return ret;
}

//use this function to send a command and wait for the response;
//which will return the response len here
s32_t  at_command(u8_t *cmd,s32_t cmdlen,const char *index,u8_t *respbuf,s32_t respbuflen,u32_t timeout)
{
    s32_t ret = 0;
    struct at_cmd *item;
    if((NULL != index)&&(NULL != cmd))//which means no response need
    {
        item = __cmd_create(cmd,cmdlen,index,respbuf,respbuflen);
        if(NULL != item)
        {
            __cmd_queue(item);
            __cmd_send((void *)cmd,cmdlen,timeout);
            if(semp_pend(item->semp,timeout))
            {
                ret = item->respdatalen;
            }
            __cmd_remove(item);
            __cmd_delete(item);
        }
    }
    return ret;
}
//just write the data to the at channel, never wait for the response
s32_t at_datawrite(u8_t *buf,s32_t len,u32_t timeout)
{
    return __cmd_send(buf,len,timeout);
}

bool_t at_oobregister(fnoob func,const char *index)
{
    bool_t ret = false;
    struct at_oob *oob;
    if(NULL != func)
    {
        oob = malloc(sizeof(struct at_oob));
        if(NULL != oob)
        {
            oob->func = func;
            oob->index = index;
            if(mutex_lock(g_at_cb.mutex))
            {
                oob->nxt = g_at_cb.ooblst;
                g_at_cb.ooblst = oob;
                ret = true;
                mutex_unlock(g_at_cb.mutex);
            }
            else
            {
                free(oob);
            }
        }
    }
    return ret;
}
static bool_t  __oob_match(u8_t *data,s32_t len)
{
    bool_t ret = false;
    struct at_oob *oob = NULL;
    if(mutex_lock(g_at_cb.mutex))
    {
        oob = g_at_cb.ooblst;
        while(NULL != oob)
        {
            if(0 == memcmp(oob->index,data,strlen(oob->index)))
            {
                break;
            }
            else
            {
                oob = oob->nxt;
            }
        }
        mutex_unlock(g_at_cb.mutex);
        if((NULL != oob)&&(NULL != oob->func))
        {
            oob->func(data,len);
            ret = true;
        }
    }
    return ret;
}

//this is the at frame receive task here
static u32_t __rcv_task_entry(void *args)
{
    u32_t  ret = 0;
    bool_t matchret;
    s32_t  rcvlen;
    struct at_cmd *item;
    struct at_oob *oob;
    while(1)
    {
        memset(g_at_cb.rcvbuf,0,g_at_cb.rcvbuflen);
        rcvlen = __resp_rcv(g_at_cb.rcvbuf,g_at_cb.rcvbuflen);
        if(rcvlen > 0)
        {
            matchret = __cmd_match(g_at_cb.rcvbuf,rcvlen);
            if(matchret == false)
            {
                __oob_match(g_at_cb.rcvbuf,rcvlen);
            }
        }
    }
}


bool_t at_install(fnframe_read func_read,fnframe_write func_write,s32_t rcvmax)
{
    bool_t ret = false;
    if((NULL ==func_read)||(NULL== func_write)||(rcvmax <=0))
    {
        printf("%s:parameters error\n\r",__FUNCTION__);
        goto EXIT_PARA;
    }
    
    memset(&g_at_cb,0,sizeof(g_at_cb));

    g_at_cb.rcvbuf = malloc(rcvmax);
    if(NULL == g_at_cb.rcvbuf)
    {
        printf("%s:rcvbuf malloc error\n\r",__FUNCTION__);
        goto EXIT_RCVBUF;
    }
    g_at_cb.rcvbuflen = rcvmax;

    if(false == mutex_create(&g_at_cb.mutex))
    {
        printf("%s:mutex create error\n\r",__FUNCTION__);
        goto EXIT_MUTEX;
    }
    if(-1 == task_create("at_rcv",__rcv_task_entry,0x800,NULL,NULL,10))
    {
        printf("%s:rcvtask create error\n\r",__FUNCTION__);
        goto EXIT_RCVTASK;
    }
    g_at_cb.read = func_read;
    g_at_cb.write = func_write;
    ret = true;
    return ret;


EXIT_RCVTASK:
    mutex_del(&g_at_cb.mutex);
EXIT_MUTEX:
    free(g_at_cb.rcvbuf);
    g_at_cb.rcvbuf = NULL;
    g_at_cb.rcvbuflen = 0;
EXIT_RCVBUF:
EXIT_PARA:
    return ret;
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

#include <shell.h>

static s32_t at_debug(s32_t argc, const char *argv[])
{
    #define CN_AT_SHELL_LEN 64
     
    u8_t respbuf[CN_AT_SHELL_LEN];
    u8_t cmdbuf[CN_AT_SHELL_LEN];
    s32_t ret = -1;

    if(argc != 3)
    {
        printf("paras error\n\r");
        return ret;
    }

    memset(respbuf,0,CN_AT_SHELL_LEN);
    snprintf((char *)cmdbuf,CN_AT_SHELL_LEN,"%s\r",argv[1]);

    ret = at_command(cmdbuf,strlen((const char *)cmdbuf),argv[2],respbuf,CN_AT_SHELL_LEN,LOSCFG_BASE_CORE_TICK_PER_SECOND); //one second
    if(ret > 0)
    {
        printf("atresponse:%d Bytes:%s\n\r",ret,respbuf);
    }
    else
    {
        printf("atresponse:timeout or error\n\r");
    }
    return ret;
}
OSSHELL_EXPORT_CMD(at_debug,"at","at atcommand atrespindex");




