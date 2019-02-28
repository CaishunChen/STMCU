#include "piclib.h"

_pic_info picinfo; //ͼƬ��Ϣ
_pic_phy pic_phy;  //ͼƬ��ʾ����ӿ�
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//��ͼ��ʼ��,�ڻ�ͼ֮ǰ,�����ȵ��ô˺���
//ָ������/����
void piclib_init(void)
{


	picinfo.ImgWidth = 0;  //��ʼ�����Ϊ0
	picinfo.ImgHeight = 0; //��ʼ���߶�Ϊ0
	picinfo.Div_Fac = 0;   //��ʼ������ϵ��Ϊ0
	picinfo.S_Height = 0;  //��ʼ���趨�ĸ߶�Ϊ0
	picinfo.S_Width = 0;   //��ʼ���趨�Ŀ��Ϊ0
	picinfo.S_XOFF = 0;	//��ʼ��x���ƫ����Ϊ0
	picinfo.S_YOFF = 0;	//��ʼ��y���ƫ����Ϊ0
	picinfo.staticx = 0;   //��ʼ����ǰ��ʾ����x����Ϊ0
	picinfo.staticy = 0;   //��ʼ����ǰ��ʾ����y����Ϊ0
}
//����ALPHA BLENDING�㷨.
//src:Դ��ɫ
//dst:Ŀ����ɫ
//alpha:͸���̶�(0~32)
//����ֵ:��Ϻ����ɫ.
uint16_t piclib_alpha_blend(uint16_t src, uint16_t dst, uint8_t alpha)
{
	uint32_t src2;
	uint32_t dst2;
	//Convert to 32bit |-----GGGGGG-----RRRRR------BBBBB|
	src2 = ((src << 16) | src) & 0x07E0F81F;
	dst2 = ((dst << 16) | dst) & 0x07E0F81F;
	//Perform blending R:G:B with alpha in range 0..32
	//Note that the reason that alpha may not exceed 32 is that there are only
	//5bits of space between each R:G:B value, any higher value will overflow
	//into the next component and deliver ugly result.
	dst2 = ((((dst2 - src2) * alpha) >> 5) + src2) & 0x07E0F81F;
	return (dst2 >> 16) | dst2;
}
//��ʼ�����ܻ���
//�ڲ�����
void ai_draw_init(void)
{
	float temp, temp1;
	temp = (float)picinfo.S_Width / picinfo.ImgWidth;
	temp1 = (float)picinfo.S_Height / picinfo.ImgHeight;
	if (temp < temp1)
		temp1 = temp; //ȡ��С���Ǹ�
	if (temp1 > 1)
		temp1 = 1;
	//ʹͼƬ��������������м�
	picinfo.S_XOFF += (picinfo.S_Width - temp1 * picinfo.ImgWidth) / 2;
	picinfo.S_YOFF += (picinfo.S_Height - temp1 * picinfo.ImgHeight) / 2;
	temp1 *= 8192; //����8192��
	picinfo.Div_Fac = temp1;
	picinfo.staticx = 0xffff;
	picinfo.staticy = 0xffff; //�ŵ�һ�������ܵ�ֵ����
}
//�ж���������Ƿ������ʾ
//(x,y) :����ԭʼ����
//chg   :���ܱ���.
//����ֵ:0,����Ҫ��ʾ.1,��Ҫ��ʾ
uint8_t is_element_ok(uint16_t x, uint16_t y, uint8_t chg)
{
	if (x != picinfo.staticx || y != picinfo.staticy)
	{
		if (chg == 1)
		{
			picinfo.staticx = x;
			picinfo.staticy = y;
		}
		return 1;
	}
	else
		return 0;
}
