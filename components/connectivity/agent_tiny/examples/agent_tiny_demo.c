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

#include "agent_tiny_demo.h"
#ifdef CONFIG_FEATURE_FOTA
#include "fota_port.h"
#endif

#define DEFAULT_SERVER_IPV4 "139.159.140.34"/*Huawei */
//#define DEFAULT_SERVER_IPV4 "47.106.88.116"/*ALI */


#define LWM2M_LIFE_TIME     50000


//b7ed6cd86b78293f35116d8b18876b10
//char * g_endpoint_name = "power07";
//unsigned char g_psk_value[16] = {0xb7, 0xed, 0x6c, 0xd8, 0x6b, 0x78, 0x29, 0x3f, 0x35, 0x11, 0x6d, 0x8b, 0x18, 0x87, 0x6b, 0x10};


//b7ed6cd86b78293f35116d8b18876b10
//char * g_endpoint_name = "power08";
//unsigned char g_psk_value[16] = {0xb7, 0xed, 0x6c, 0xd8, 0x6b, 0x78, 0x29, 0x3f, 0x35, 0x11, 0x6d, 0x8b, 0x18, 0x87, 0x6b, 0x10};

//3179e2c3d0e22e39192a6a09a2fc4101
//char * g_endpoint_name = "power09";
//unsigned char g_psk_value[16] = {0x31, 0x79, 0xe2, 0xc3, 0xd0, 0xe2, 0x2e, 0x39, 0x19, 0x2a, 0x6a, 0x09, 0xa2, 0xfc, 0x41, 0x01};

//86ba2db165b1e1292be327feedfbfd1d
//char * g_endpoint_name = "power10";
//unsigned char g_psk_value[16] = {0x86, 0xba, 0x2d, 0xb1, 0x65, 0xb1, 0xe1, 0x29, 0x2b, 0xe3, 0x27, 0xfe, 0xed, 0xfb, 0xfd, 0x1d};

//ecdf1e8424a9f16b79c1f5376142cde4
//char * g_endpoint_name = "power11";
//unsigned char g_psk_value[16] = {0xec, 0xdf, 0x1e, 0x84, 0x24, 0xa9, 0xf1, 0x6b, 0x79, 0xc1, 0xf5, 0x37, 0x61, 0x42, 0xcd, 0xe4};

//11e352f9f35170ab71012e7f6433b36b
//char * g_endpoint_name = "power12";
//unsigned char g_psk_value[16] = {0x11, 0xe3, 0x52, 0xf9, 0xf3, 0x51, 0x70, 0xab, 0x71, 0x01, 0x2e, 0x7f, 0x64, 0x33, 0xb3, 0x6b};


//195deee586259e0f95e7719894a9f6b4
//char * g_endpoint_name = "power13";
//unsigned char g_psk_value[16] = {0x19, 0x5d, 0xee, 0xe5, 0x86, 0x25, 0x9e, 0x0f, 0x95, 0xe7, 0x71, 0x98, 0x94, 0xa9, 0xf6, 0xb4};

//195deee586259e0f95e7719894a9f6b4
//char * g_endpoint_name = "power14";
//unsigned char g_psk_value[16] = {0x18, 0x5d, 0xee, 0xe5, 0x86, 0x25, 0x9e, 0x0f, 0x95, 0xe7, 0x71, 0x98, 0x94, 0xa9, 0xf6, 0xb4};

//97930fe9bbcd4ad8d59a9934f89be8ea
//char * g_endpoint_name = "power15";
//unsigned char g_psk_value[16] = {0x97, 0x93, 0x0f, 0xe9, 0xbb, 0xcd, 0x4a, 0xd8, 0xd5, 0x9a, 0x99, 0x34, 0xf8, 0x9b, 0xe8, 0xea};

//43896da21587638f3650847b4766126a
//char * g_endpoint_name = "zqf03";
//unsigned char g_psk_value[]={ 0x43, 0x89, 0x6d, 0xa2, 0x15, 0x87, 0x63, 0x8f, 0x36, 0x50, 0x84, 0x7b, 0x47, 0x66, 0x12, 0x6a, };

//7fa2e11a0af95ededb0c9734a4d2de8f
//char * g_endpoint_name = "ledlwm2m01";
//unsigned char g_psk_value[]={ 0x43, 0x89, 0x6d, 0xa2, 0x15, 0x87, 0x63, 0x8f, 0x36, 0x50, 0x84, 0x7b, 0x47, 0x66, 0x12, 0x6a, };

//0779af28def6668f32b9ae30c6ea5f7f
//char * g_endpoint_name = "ledlwm2m02";
//unsigned char g_psk_value[]={ 0x07, 0x79, 0xaf, 0x28, 0xde, 0xf6, 0x66, 0x8f, 0x32, 0xb9, 0xae, 0x30, 0xc6, 0xea, 0x5f, 0x7f};

//91b4c34d597a9b0b44c42a7104ec1423
//char * g_endpoint_name = "ledlwm2m03";
//unsigned char g_psk_value[]={0x91, 0xb4, 0xc3, 0x4d, 0x59, 0x7a, 0x9b, 0x0b, 0x44, 0xc4, 0x2a, 0x71, 0x04, 0xec, 0x14, 0x23};

//91b4c34d597a9b0b44c42a7104ec1423
//char * g_endpoint_name = "zqf03";
//unsigned char g_psk_value[]={0x91, 0xb4, 0xc3, 0x4d, 0x59, 0x7a, 0x9b, 0x0b, 0x44, 0xc4, 0x2a, 0x71, 0x04, 0xec, 0x14, 0x23};

//6c74374a7124eed651950da81e5e2b96
//char * g_endpoint_name = "zqf04";
//unsigned char g_psk_value[32];
//const char *g_psk_string="6c74374a7124eed651950da81e5e2b96";

char * g_endpoint_name = "zqf05";
unsigned char g_psk_value[32];
const char *g_psk_string="6c74374a7124eed651950da81e5e2b96";

//use this function to transfer the string to the byte array
//return the array length transfered while  failed
static int pskstring_to_array(const char *str,unsigned char *buf,int len)
{
	int ret = 0;
	char tmp[8];
	unsigned int value;
	int i = 0;
	
	if((len *2)!= strlen(str))
	{
		return ret;
	}
	memset(tmp,0,8);
	tmp[0] = '0';
	tmp[1] = 'x';
	for(i = 0;i<strlen(str);i=i+2)
	{
		tmp[2] = str[i];
		tmp[3] = str[i+1];
		value = strtoul(tmp,NULL,0);
		buf[ret] = value&0xff;
		ret = ret+1;
	}
	return ret;
}



static void *g_phandle = NULL;
static atiny_device_info_t g_device_info;
static atiny_param_t g_atiny_params;

void ack_callback(atiny_report_type_e type, int cookie, data_send_status_e status)
{
    ATINY_LOG(LOG_DEBUG, "type:%d cookie:%d status:%d\n", type, cookie, status);
}

#pragma pack(1)
struct data_envrionstate
{
    uint8_t msgid;
    char    intensity[4];
    char    humidity[4];
    char    temprature[4];
};

struct data_ledstate
{
    uint8_t msgid;
    char    state[3];
};
#pragma pack()



/*lint -e550*/
void app_data_report(void)
{
    int ret = 0;
    int cnt = 0;
    int times = 0;
    struct data_ledstate      ledstate;
    struct data_envrionstate  environstate;
    data_report_t report_data;
    float v1;
    while(1)
    {
        
        switch(times)
        {
            case 0:   //send the environ state
                //simulate the environment data
                environstate.msgid = 0;
                memcpy(environstate.intensity,"10  ",4);
                v1 = 12.3;
                sprintf(environstate.humidity, "%.1f", v1);
                v1 = 45.6;
                sprintf(environstate.temprature,"%.1f", v1);         
                report_data.buf = (uint8_t *)&environstate;
                report_data.callback = ack_callback;
                report_data.cookie = cnt;
                report_data.len = sizeof(environstate);
                report_data.type = APP_DATA;
                ret = atiny_data_report(g_phandle, &report_data);
                break;
            
            case 1:   //send the led state 1  on 
                //simulate the led state
                ledstate.msgid = 1;
                memcpy(ledstate.state," ON",3);
                
                report_data.buf = (uint8_t *)&ledstate;
                report_data.callback = ack_callback;
                report_data.cookie = cnt;
                report_data.len = sizeof(ledstate);
                report_data.type = APP_DATA;
                ret = atiny_data_report(g_phandle, &report_data);
                break;            
            case 2:   //send the led state 2  off
                //simulate the led state
                ledstate.msgid = 1;
                memcpy(ledstate.state,"OFF",3);
                
                report_data.buf = (uint8_t *)&ledstate;
                report_data.callback = ack_callback;
                report_data.cookie = cnt;
                report_data.len = sizeof(ledstate);
                report_data.type = APP_DATA;
                ret = atiny_data_report(g_phandle, &report_data);
                break; 
                /*
            case 3:   //send the led state 3  dam
                //simulate the led state
                ledstate.msgid = 1;
                memcpy(ledstate.state,"DAM",3);
                report_data.buf = (uint8_t *)&ledstate;
                report_data.callback = ack_callback;
                report_data.cookie = cnt;
                report_data.len = sizeof(ledstate);
                report_data.type = APP_DATA;
                ret = atiny_data_report(g_phandle, &report_data);
                break;
            */
            default:
                //ret = atiny_data_change(g_phandle, DEVICE_MEMORY_FREE);
                break;
        }
        printf("report state:times:%d ret:%d\n\r",times, ret);
        if(0 == ret)
        {
            times = (times+1)% 7;
        }
        cnt++;
        (void)LOS_TaskDelay(10*1000);

    }
}
/*lint +e550*/

UINT32 creat_report_task()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;
    UINT32 TskHandle;

    task_init_param.usTaskPrio = 1;
    task_init_param.pcName = "app_data_report";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)app_data_report;
    task_init_param.uwStackSize = 0x800;

    uwRet = LOS_TaskCreate(&TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;

}

#ifdef CONFIG_FEATURE_FOTA
void agent_tiny_fota_init(void)
{
    atiny_fota_storage_device_s *storage_device = NULL ;
    fota_hardware_s *hardware = NULL;
    fota_pack_device_info_s device_info;
    const char *rsa_N = "C94BECB7BCBFF459B9A71F12C3CC0603B11F0D3A366A226FD3E73D453F96EFBBCD4DFED6D9F77FD78C3AB1805E1BD3858131ACB5303F61AF524F43971B4D429CB847905E68935C1748D0096C1A09DD539CE74857F9FDF0B0EA61574C5D76BD9A67681AC6A9DB1BB22F17120B1DBF3E32633DCE34F5446F52DD7335671AC3A1F21DC557FA4CE9A4E0E3E99FED33A0BAA1C6F6EE53EDD742284D6582B51E4BF019787B8C33C2F2A095BEED11D6FE68611BD00825AF97DB985C62C3AE0DC69BD7D0118E6D620B52AFD514AD5BFA8BAB998332213D7DBF5C98DC86CB8D4F98A416802B892B8D6BEE5D55B7E688334B281E4BEDDB11BD7B374355C5919BA5A9A1C91F";
    const char *rsa_E = "10001";

    (void)hal_init_fota();

    (void)hal_get_fota_device(&storage_device, &hardware);

    device_info.hardware = hardware;
    device_info.storage_device = storage_device;
    device_info.head_info_notify = NULL;
    device_info.key.rsa_N = rsa_N;
    device_info.key.rsa_E = rsa_E;
    (void)fota_set_pack_device(fota_get_pack_device(), &device_info);
}
#endif

void agent_tiny_entry(void)
{
	int psklen = 0;
    UINT32 uwRet = LOS_OK;
    atiny_param_t *atiny_params;
    atiny_security_param_t  *iot_security_param = NULL;
    atiny_security_param_t  *bs_security_param = NULL;

    atiny_device_info_t *device_info = &g_device_info;

#ifdef CONFIG_FEATURE_FOTA
    agent_tiny_fota_init();
#endif

#ifdef WITH_DTLS
    device_info->endpoint_name = g_endpoint_name;
#else
    device_info->endpoint_name = g_endpoint_name;
#endif
#ifdef CONFIG_FEATURE_FOTA
    device_info->manufacturer = "Lwm2mFota";
    device_info->dev_type = "Lwm2mFota";
#else
    device_info->manufacturer = "Agent_Tiny";
#endif
    atiny_params = &g_atiny_params;
    atiny_params->server_params.binding = "UQ";
    //atiny_params->server_params.life_time = LWM2M_LIFE_TIME;
    atiny_params->server_params.life_time = 20;
    atiny_params->server_params.storing_cnt = 0;

    atiny_params->bootstrap_mode = BOOTSTRAP_FACTORY;

    //pay attention: index 0 for iot server, index 1 for bootstrap server.
    iot_security_param = &(atiny_params->security_params[0]);
    bs_security_param = &(atiny_params->security_params[1]);


    iot_security_param->server_ip = DEFAULT_SERVER_IPV4;
    bs_security_param->server_ip = DEFAULT_SERVER_IPV4;

#ifdef WITH_DTLS

	psklen = pskstring_to_array(g_psk_string,g_psk_value,16);
	if(psklen <=0)   //check if the psk transfered right
	{
		return ;
	}
    iot_security_param->server_port = "5684";
    bs_security_param->server_port = "5684";

    iot_security_param->psk_Id = g_endpoint_name;
    iot_security_param->psk = (char *)g_psk_value;
    iot_security_param->psk_len = psklen;

    //bs_security_param->psk_Id = g_endpoint_name_bs;
    //bs_security_param->psk = (char *)g_psk_bs_value;
    //bs_security_param->psk_len = sizeof(g_psk_bs_value);
#else
    iot_security_param->server_port = "5683";
    bs_security_param->server_port = "5683";

    iot_security_param->psk_Id = NULL;
    iot_security_param->psk = NULL;
    iot_security_param->psk_len = 0;

    bs_security_param->psk_Id = NULL;
    bs_security_param->psk = NULL;
    bs_security_param->psk_len = 0;
#endif

    if(ATINY_OK != atiny_init(atiny_params, &g_phandle))
    {
        return;
    }

    uwRet = creat_report_task();
    if(LOS_OK != uwRet)
    {
        return;
    }

    (void)atiny_bind(device_info, g_phandle);
}


