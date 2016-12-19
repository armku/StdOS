#include "usmart_str.h"
#include "usmart.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�汾��V3.1
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2011-2021
//All rights reserved
//********************************************************************************
//����˵��
//V1.4
//�����˶Բ���Ϊstring���͵ĺ�����֧��.���÷�Χ������.
//�Ż����ڴ�ռ��,��̬�ڴ�ռ��Ϊ79���ֽ�@10������.��̬��Ӧ���ּ��ַ�������
//V2.0
//1,�޸���listָ��,��ӡ�������������ʽ.
//2,������idָ��,��ӡÿ����������ڵ�ַ.
//3,�޸��˲���ƥ��,֧�ֺ��������ĵ���(������ڵ�ַ).
//4,�����˺��������Ⱥ궨��.
//V2.1 20110707
//1,����dec,hex����ָ��,�������ò�����ʾ����,��ִ�н���ת��.
//ע:��dec,hex����������ʱ��,���趨��ʾ��������.�����������ʱ��,��ִ�н���ת��.
//��:"dec 0XFF" ��Ὣ0XFFתΪ255,�ɴ��ڷ���.
//��:"hex 100" 	��Ὣ100תΪ0X64,�ɴ��ڷ���
//2,����usmart_get_cmdname����,���ڻ�ȡָ������.
//V2.2 20110726
//1,������void���Ͳ����Ĳ���ͳ�ƴ���.
//2,�޸�������ʾ��ʽĬ��Ϊ16����.
//V2.3 20110815
//1,ȥ���˺�����������"("������.
//2,�������ַ��������в�����"("��bug.
//3,�޸��˺���Ĭ����ʾ������ʽ���޸ķ�ʽ.
//V2.4 20110905
//1,�޸���usmart_get_cmdname����,������������������.����������������ʱ����������.
//2,����USMART_ENTIM4_SCAN�궨��,���������Ƿ�ʹ��TIM2��ʱִ��scan����.
//V2.5 20110930
//1,�޸�usmart_init����Ϊvoid usmart_init(uint8_t sysclk),���Ը���ϵͳƵ���Զ��趨ɨ��ʱ��.(�̶�100ms)
//2,ȥ����usmart_init�����е�uart_init����,���ڳ�ʼ���������ⲿ��ʼ��,�����û����й���.
//V2.6 20111009
//1,������read_addr��write_addr��������.��������������������д�ڲ������ַ(��������Ч��ַ).���ӷ������.
//2,read_addr��write_addr������������ͨ������USMART_USE_WRFUNSΪ��ʹ�ܺ͹ر�.
//3,�޸���usmart_strcmp,ʹ��淶��.
//V2.7 20111024
//1,�����˷���ֵ16������ʾʱ�����е�bug.
//2,�����˺����Ƿ��з���ֵ���ж�,���û�з���ֵ,�򲻻���ʾ.�з���ֵʱ����ʾ�䷵��ֵ.
//V2.8 20111116
//1,������list�Ȳ���������ָ��ͺ���ܵ���������bug.
//V2.9 20120917
//1,�޸������磺void*xxx(void)���ͺ�������ʶ���bug��
//V3.0 20130425
//1,�������ַ���������ת�����֧�֡�
//V3.1 20131120
//1,����runtimeϵͳָ��,��������ͳ�ƺ���ִ��ʱ��.
//�÷�:
//����:runtime 1 ,��������ִ��ʱ��ͳ�ƹ���
//����:runtime 0 ,��رպ���ִ��ʱ��ͳ�ƹ���
///runtimeͳ�ƹ���,��������:USMART_ENTIMX_SCAN Ϊ1,�ſ���ʹ��!!
/////////////////////////////////////////////////////////////////////////////////////

//�Ա��ַ���str1��str2
//*str1:�ַ���1ָ��
//*str2:�ַ���2ָ��
//����ֵ:0�����;1�������;
uint8_t usmart_strcmp(uint8_t *str1, uint8_t *str2)
{
    while(1)
    {
        if(*str1 != *str2)return 1; //�����
        if(*str1 == '\0')break; //�Ա������.
        str1++;
        str2++;
    }
    return 0;//�����ַ������
}
//��str1������copy��str2
//*str1:�ַ���1ָ��
//*str2:�ַ���2ָ��
void usmart_strcopy(uint8_t *str1, uint8_t *str2)
{
    while(1)
    {
        *str2 = *str1;	//����
        if(*str1 == '\0')break; //���������.
        str1++;
        str2++;
    }
}
//�õ��ַ����ĳ���(�ֽ�)
//*str:�ַ���ָ��
//����ֵ:�ַ����ĳ���
uint8_t usmart_strlen(uint8_t *str)
{
    uint8_t len = 0;
    while(1)
    {
        if(*str == '\0')break; //���������.
        len++;
        str++;
    }
    return len;
}
//m^n����
//����ֵ:m^n�η�
uint32_t usmart_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while(n--)result *= m;
    return result;
}
//���ַ���תΪ����
//֧��16����ת��,����16������ĸ�����Ǵ�д��,�Ҹ�ʽΪ��0X��ͷ��.
//��֧�ָ���
//*str:�����ַ���ָ��
//*res:ת����Ľ����ŵ�ַ.
//����ֵ:0���ɹ�ת�����.����,�������.
//1,���ݸ�ʽ����.2,16����λ��Ϊ0.3,��ʼ��ʽ����.4,ʮ����λ��Ϊ0.
uint8_t usmart_str2num(uint8_t *str, uint32_t *res)
{
    uint32_t t;
    uint8_t bnum = 0;	//���ֵ�λ��
    uint8_t *p;
    uint8_t hexdec = 10; //Ĭ��Ϊʮ��������
    p = str;
    *res = 0; //����.
    while(1)
    {
        if((*p <= '9' && *p >= '0') || (*p <= 'F' && *p >= 'A') || (*p == 'X' && bnum == 1)) //�����Ϸ�
        {
            if(*p >= 'A')hexdec = 16;	//�ַ����д�����ĸ,Ϊ16���Ƹ�ʽ.
            bnum++;					//λ������.
        }
        else if(*p == '\0')break;	//����������,�˳�.
        else return 1;				//��ȫ��ʮ���ƻ���16��������.
        p++;
    }
    p = str;			  //���¶�λ���ַ�����ʼ�ĵ�ַ.
    if(hexdec == 16)		//16��������
    {
        if(bnum < 3)return 2;			//λ��С��3��ֱ���˳�.��Ϊ0X��ռ��2��,���0X���治������,������ݷǷ�.
        if(*p == '0' && (*(p + 1) == 'X')) //������'0X'��ͷ.
        {
            p += 2;	//ƫ�Ƶ�������ʼ��ַ.
            bnum -= 2; //��ȥƫ����
        }
        else return 3; //��ʼͷ�ĸ�ʽ����
    }
    else if(bnum == 0)return 4; //λ��Ϊ0��ֱ���˳�.
    while(1)
    {
        if(bnum)bnum--;
        if(*p <= '9' && *p >= '0')t = *p - '0';	//�õ����ֵ�ֵ
        else t = *p - 'A' + 10;				//�õ�A~F��Ӧ��ֵ
        *res += t * usmart_pow(hexdec, bnum);
        p++;
        if(*p == '\0')break; //���ݶ�������.
    }
    return 0;//�ɹ�ת��
}
//�õ�ָ����
//*str:Դ�ַ���
//*cmdname:ָ����
//*nlen:ָ��������
//maxlen:��󳤶�(������,ָ�����̫����)
//����ֵ:0,�ɹ�;����,ʧ��.
uint8_t usmart_get_cmdname(uint8_t *str, uint8_t *cmdname, uint8_t *nlen, uint8_t maxlen)
{
    *nlen = 0;
    while(*str != ' ' && *str != '\0') //�ҵ��ո���߽���������Ϊ������
    {
        *cmdname = *str;
        str++;
        cmdname++;
        (*nlen)++;//ͳ�������
        if(*nlen >= maxlen)return 1; //�����ָ��
    }
    *cmdname = '\0'; //���������
    return 0;//��������
}
//��ȡ��һ���ַ������м��кܶ�ո��ʱ�򣬴˺���ֱ�Ӻ��Կո��ҵ��ո�֮��ĵ�һ���ַ���
//str:�ַ���ָ��
//����ֵ:��һ���ַ�
uint8_t usmart_search_nextc(uint8_t *str)
{
    str++;
    while(*str == ' ' && str != '\0')str++;
    return *str;
}
//��str�еõ�������
//*str:Դ�ַ���ָ��
//*fname:��ȡ���ĺ�������ָ��
//*pnum:�����Ĳ�������
//*rval:�Ƿ���Ҫ��ʾ����ֵ(0,����Ҫ;1,��Ҫ)
//����ֵ:0,�ɹ�;����,�������.
uint8_t usmart_get_fname(uint8_t *str, uint8_t *fname, uint8_t *pnum, uint8_t *rval)
{
    uint8_t res;
    uint8_t fover = 0;	 //�������
    uint8_t *strtemp;
    uint8_t offset = 0;
    uint8_t parmnum = 0;
    uint8_t temp = 1;
    uint8_t fpname[6];//void+X+'/0'
    uint8_t fplcnt = 0; //��һ�������ĳ��ȼ�����
    uint8_t pcnt = 0;	 //����������
    uint8_t nchar;
    //�жϺ����Ƿ��з���ֵ
    strtemp = str;
    while(*strtemp != '\0') //û�н���
    {
        if(*strtemp != ' ' && (pcnt & 0X7F) < 5) //����¼5���ַ�
        {
            if(pcnt == 0)pcnt |= 0X80; //��λ���λ,��ǿ�ʼ���շ���ֵ����
            if(((pcnt & 0x7f) == 4) && (*strtemp != '*'))break; //���һ���ַ�,������*
            fpname[pcnt & 0x7f] = *strtemp; //��¼�����ķ���ֵ����
            pcnt++;
        }
        else if(pcnt == 0X85)break;
        strtemp++;
    }
    if(pcnt)//��������
    {
        fpname[pcnt & 0x7f] = '\0'; //���������
        if(usmart_strcmp(fpname, "void") == 0)*rval = 0; //����Ҫ����ֵ
        else *rval = 1;							 //��Ҫ����ֵ
        pcnt = 0;
    }
    res = 0;
    strtemp = str;
    while(*strtemp != '(' && *strtemp != '\0') //�˴����ҵ���������������ʼλ��
    {
        strtemp++;
        res++;
        if(*strtemp == ' ' || *strtemp == '*')
        {
            nchar = usmart_search_nextc(strtemp);		//��ȡ��һ���ַ�
            if(nchar != '(' && nchar != '*')offset = res;	//�����ո��*��
        }
    }
    strtemp = str;
    if(offset)strtemp += offset + 1; //������������ʼ�ĵط�
    res = 0;
    nchar = 0; //�Ƿ������ַ�������ı�־,0�������ַ���;1�����ַ���;
    while(1)
    {
        if(*strtemp == 0)
        {
            res = USMART_FUNCERR; //��������
            break;
        }
        else if(*strtemp == '(' && nchar == 0)fover++; //�����������һ��
        else if(*strtemp == ')' && nchar == 0)
        {
            if(fover)fover--;
            else res = USMART_FUNCERR; //�������,û�յ�'('
            if(fover == 0)break; //��ĩβ��,�˳�
        }
        else if(*strtemp == '"')nchar = !nchar;

        if(fover == 0) //��������û������
        {
            if(*strtemp != ' ') //�ո����ں�����
            {
                *fname = *strtemp; //�õ�������
                fname++;
            }
        }
        else  //�Ѿ��������˺�������.
        {
            if(*strtemp == ',')
            {
                temp = 1;		//ʹ������һ������
                pcnt++;
            }
            else if(*strtemp != ' ' && *strtemp != '(')
            {
                if(pcnt == 0 && fplcnt < 5)		//����һ��������ʱ,Ϊ�˱���ͳ��void���͵Ĳ���,�������ж�.
                {
                    fpname[fplcnt] = *strtemp; //��¼��������.
                    fplcnt++;
                }
                temp++;	//�õ���Ч����(�ǿո�)
            }
            if(fover == 1 && temp == 2)
            {
                temp++;		//��ֹ�ظ�����
                parmnum++; 	//��������һ��
            }
        }
        strtemp++;
    }
    if(parmnum == 1) //ֻ��1������.
    {
        fpname[fplcnt] = '\0'; //���������
        if(usmart_strcmp(fpname, "void") == 0)parmnum = 0; //����Ϊvoid,��ʾû�в���.
    }
    *pnum = parmnum;	//��¼��������
    *fname = '\0';	//���������
    return res;		//����ִ�н��
}


//��str�еõ�һ�������Ĳ���
//*str:Դ�ַ���ָ��
//*fparm:�����ַ���ָ��
//*ptype:�������� 0������;1���ַ���;0XFF����������
//����ֵ:0,�Ѿ��޲�����;����,��һ��������ƫ����.
uint8_t usmart_get_aparm(uint8_t *str, uint8_t *fparm, uint8_t *ptype)
{
    uint8_t i = 0;
    uint8_t enout = 0;
    uint8_t type = 0; //Ĭ��������
    uint8_t string = 0; //���str�Ƿ����ڶ�
    while(1)
    {
        if(*str == ',' && string == 0)enout = 1;			//�ݻ������˳�,Ŀ����Ѱ����һ����������ʼ��ַ
        if((*str == ')' || *str == '\0') && string == 0)break; //�����˳���ʶ��
        if(type == 0) //Ĭ�������ֵ�
        {
            if((*str >= '0' && *str <= '9') || (*str >= 'a' && *str <= 'f') || (*str >= 'A' && *str <= 'F') || *str == 'X' || *str == 'x') //���ִ����
            {
                if(enout)break;					//�ҵ�����һ������,ֱ���˳�.
                if(*str >= 'a')*fparm = *str - 0X20;	//Сдת��Ϊ��д
                else *fparm = *str;		   		//Сд�������ֱ��ֲ���
                fparm++;
            }
            else if(*str == '"') //�ҵ��ַ����Ŀ�ʼ��־
            {
                if(enout)break;//�ҵ�,����ҵ�",��Ϊ������.
                type = 1;
                string = 1; //�Ǽ�STRING ���ڶ���
            }
            else if(*str != ' ' && *str != ',') //���ַǷ��ַ�,��������
            {
                type = 0XFF;
                break;
            }
        }
        else //string��
        {
            if(*str == '"')string = 0;
            if(enout)break;			//�ҵ�����һ������,ֱ���˳�.
            if(string)				//�ַ������ڶ�
            {
                if(*str == '\\')		//����ת���(������ת���)
                {
                    str++;			//ƫ�Ƶ�ת���������ַ�,����ʲô�ַ�,ֱ��COPY
                    i++;
                }
                *fparm = *str;		//Сд�������ֱ��ֲ���
                fparm++;
            }
        }
        i++;//ƫ��������
        str++;
    }
    *fparm = '\0';	//���������
    *ptype = type;	//���ز�������
    return i;		//���ز�������
}
//�õ�ָ����������ʼ��ַ
//num:��num������,��Χ0~9.
//����ֵ:�ò�������ʼ��ַ
uint8_t usmart_get_parmpos(uint8_t num)
{
    uint8_t temp = 0;
    uint8_t i;
    for(i = 0; i < num; i++)temp += usmart_dev.plentbl[i];
    return temp;
}
//��str�еõ���������
//str:Դ�ַ���;
//parn:�����Ķ���.0��ʾ�޲��� void����
//����ֵ:0,�ɹ�;����,�������.
uint8_t usmart_get_fparam(uint8_t *str, uint8_t *parn)
{
    uint8_t i, type;
    uint32_t res;
    uint8_t n = 0;
    uint8_t len;
    uint8_t tstr[PARM_LEN + 1]; //�ֽڳ��ȵĻ���,�����Դ��PARM_LEN���ַ����ַ���
    for(i = 0; i < MAX_PARM; i++)usmart_dev.plentbl[i] = 0; //��ղ������ȱ�
    while(*str != '(') //ƫ�Ƶ�������ʼ�ĵط�
    {
        str++;
        if(*str == '\0')return USMART_FUNCERR; //������������
    }
    str++;//ƫ�Ƶ�"("֮��ĵ�һ���ֽ�
    while(1)
    {
        i = usmart_get_aparm(str, tstr, &type);	//�õ���һ������
        str += i;								//ƫ��
        switch(type)
        {
        case 0:	//����
            if(tstr[0] != '\0')				//���յ��Ĳ�����Ч
            {
                i = usmart_str2num(tstr, &res);	//��¼�ò���
                if(i)return USMART_PARMERR;		//��������.
                *(uint32_t *)(usmart_dev.parm + usmart_get_parmpos(n)) = res; //��¼ת���ɹ��Ľ��.
                usmart_dev.parmtype &= ~(1 << n);	//�������
                usmart_dev.plentbl[n] = 4;		//�ò����ĳ���Ϊ4
                n++;							//��������
                if(n > MAX_PARM)return USMART_PARMOVER; //����̫��
            }
            break;
        case 1://�ַ���
            len = usmart_strlen(tstr) + 1;	//�����˽�����'\0'
            usmart_strcopy(tstr, &usmart_dev.parm[usmart_get_parmpos(n)]); //����tstr���ݵ�usmart_dev.parm[n]
            usmart_dev.parmtype |= 1 << n;	//����ַ���
            usmart_dev.plentbl[n] = len;	//�ò����ĳ���Ϊlen
            n++;
            if(n > MAX_PARM)return USMART_PARMOVER; //����̫��
            break;
        case 0XFF://����
            return USMART_PARMERR;//��������
        }
        if(*str == ')' || *str == '\0')break; //�鵽������־��.
    }
    *parn = n;	//��¼�����ĸ���
    return USMART_OK;//��ȷ�õ��˲���
}
