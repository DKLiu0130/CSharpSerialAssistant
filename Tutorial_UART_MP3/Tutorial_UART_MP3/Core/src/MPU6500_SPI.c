/*
Program MPU6500_SPI;
Author 0x91f aka Lzy;
Description{
	SPI Driver for MPU6500;
	Set SPI name as MPU.
	MPU6500 SPIͨ��ԭ��
		SPI����д�����ȡ�����λ���֣�0Ϊд�룬1Ϊ��ȡ��
		�Ե���SPIͨ��Ϊ����
			I. һ��д����Ҫ���¹��̣�
				1.д���ַ���Ĵ���������ȡ����ֵ��
				2.д�����ݣ���ȡ����ֵ��
			II. һ�ζ�ȡ��Ҫ���¹��̣�
				1.д���ַ����ȡ����ֵ��
}
*/
#include "ti_msp_dl_config.h"
#include "MPU6500_Regs.h"
#include "MPU6500_SPI.h"
#include "UART.h"
#include "oled_spi_V0.2.h"
#include <math.h>

//Notes:
/*
delay_cycles(CPU_Frq * 100);  ==  delay_ms(100);
*/

//Global Variables:
#define Gopt 0
#define Aopt 0
#define RoomTempDegC 21
#define gravity 98 
static int16_t ACC_DATA[7];
static double xAoffset = 0,yAoffset = 0,zAoffset = 0;
static double xGoffset = 0,yGoffset = 0,zGoffset = 0;

//Inner Functions:
inline uint8_t ReadWriteByte(SPI_Regs *spi, uint8_t byte) {  
    uint8_t receivedByte = 0;  
    //����һ���ֽ�|Send a Byte;
		DL_SPI_transmitDataBlocking8(spi, byte);
    //����һ���ֽ�|Receive a Byte;
		receivedByte = DL_SPI_receiveDataBlocking8(spi);
    return receivedByte;
}


//SPI writes a byte;
uint8_t MPU6500_Write_Byte(uint8_t reg,uint8_t data) {
		//����ֵ��������|Return Value Reserved;
		ReadWriteByte(MPU_INST,reg); 	//��MPU6500д����ʱ���λΪ0���ҼĴ�����ַ���λΪ0;
		ReadWriteByte(MPU_INST,data);
		return 0;
}

//SPI reads a byte;
uint8_t MPU6500_Read_Byte(uint8_t reg){
		uint8_t tmp=0;
		ReadWriteByte(MPU_INST,reg|0x80);//r ���λΪ1
		tmp = ReadWriteByte(MPU_INST,0xff);
		return tmp;
}

int16_t AEncoder(int16_t Origin) {  
    if (Origin >= 0) {  
        return Origin;  
    }
    return ((-1 * ~(Origin)) + 1);  
}

int16_t ADecoder(int16_t complement) {  
    // �����������������ֱ�ӷ��أ������Ĳ������ԭ�룩  
    if (complement >= 0) {  
        return complement;  
    }
    // ���ڸ���������תԭ�룺�����1��λȡ��  
    // ע�⣺~�������ᰴλȡ������������λ  
    // Ȼ��������Ҫ�������1���ָ���ԭ�루��Ϊԭ���ǲ����1ǰ������  
    // ������������ͨ������λ���䰴λȡ��Ȼ���1�ķ�ʽ�õ�����ģ���������ֻ��Ҫ��λȡ������ȫ���ż���  
    return -~(complement - 1);  
}

//APIs:
uint8_t MPU6500_Init(void){ 
		//д��Ĵ�������ֵ���������ֲᣬ��Ҫ��д��
		//����ܽ���ֲᣡ����
		//����ܽ���ֲᣡ����
		//����ܽ���ֲᣡ����
		xAoffset = yAoffset = zAoffset = xGoffset = yGoffset = zGoffset = 0;	//Initialize Offset;
		MPU6500_Write_Byte(PWR_MGMT_1,0X80);					//��λmpu6500;
		delay_cycles(CPU_Frq * 100); 									//Unit:ms; - delay 100ms;
		MPU6500_Write_Byte(PWR_MGMT_1,0X01);					//��ʼ��mpu6500;
		delay_cycles(CPU_Frq * 100); 									//Unit:ms; - delay 100ms;
		MPU6500_Write_Byte(PWR_MGMT_2,0X00);				  //ȡ������,���ٶ��������Ƕ�����;
		delay_cycles(CPU_Frq * 100);									//Unit:ms; - delay 100ms
		MPU6500_Write_Byte(SIGNAL_PATH_RESET,0X07);		//reset GYR+ACC+TEMP;
		delay_cycles(CPU_Frq * 100); 									//Unit:ms; - delay 100ms;
		//MPU6500_Write_Byte(USER_CTRL, 0x91);				  //SET spi mode+Reset all gyro digital signal path, accel digital signal path, and temp;
		MPU6500_Write_Byte(USER_CTRL, 0x11);				  //SET spi mode+Reset all gyro digital signal path, accel digital signal path, and temp;
		delay_cycles(CPU_Frq * 100); 									//Unit:ms; - delay 100ms;	
		uint16_t res = MPU6500_Read_Byte(WHO_AM_I);		//Recognize the device;
		if(res == 0x70){
				//����ID��ȷ
				MPU6500_Set_Gyro_Fsr(Gopt);									//�����Ǵ�����,��250dps
				//MPU6500_Set_Gyro_Fsr(3);									//�����Ǵ�����,��2000dps
				//MPU6500_Write_Byte(GYRO_CONFIG,0x19);			//�����Ǵ�����,��2000dps,and FCHOICE_B = 11;
				delay_cycles(CPU_Frq * 100);								//ÿдһ���Ĵ���ע����ʱ����Ȼ�������벻���Ĵ���lol --yulong
				MPU6500_Set_Accel_Fsr(Aopt);									//���ٶȴ�����,��2g
				//MPU6500_Set_Accel_Fsr(3);									//���ٶȴ�����,��16g
				delay_cycles(CPU_Frq * 100);
				MPU6500_Write_Byte(CONFIG,0X03);					//0x03:	gyr Fs=1khz,bandwidth=41hz
				delay_cycles(CPU_Frq * 100); 
				//MPU6500_Write_Byte(ACCEL_CONFIG_2,0X03);	//Acc Fs=1khz, bandtidth=41hz
				MPU6500_Write_Byte(ACCEL_CONFIG_2,0X08);	//Acc Fs=4khz, bandtidth=1.13khz
				delay_cycles(CPU_Frq * 100);
				//MPU6500_Write_Byte(MPU_INTBP_CFG_REG,0XA0);	//INT���ŵ͵�ƽ��Ч,�������
				//delay_cycles(CPU_Frq * 50);
				//MPU6500_Write_Byte(MPU_INT_EN_REG,0X01);	//raw data inter open
				//delay_cycles(CPU_Frq * 50);
				//MPU6500_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//����CLKSEL,PLL X��Ϊ�ο�
				//delay_cycles(CPU_Frq * 50);
				//MPU6500_Set_Rate(1000);											//���ò�����Ϊ1000Hz,��FCHOICE_B��Ϊȫ0ʱ�����ã�
				//delay_cycles(CPU_Frq * 100);
		}
		return 0;
}

#define Calibration_Period 30
struct MPUInfo MPU6500_Calibration(){
		struct MPUInfo Integral,AvgOffset;
		Integral.xAcc = Integral.yAcc = Integral.zAcc = Integral.xGypo = Integral.yGypo = Integral.zGypo = 0;
		for (uint16_t t = 1;t <= Calibration_Period;++t){
				AvgOffset = MPU6500_getRawData();
				//Integral.xAcc += AvgOffset.xAcc;
				//Integral.yAcc += AvgOffset.yAcc;
				//Integral.zAcc += AvgOffset.zAcc;
				Integral.xGypo += AvgOffset.xGypo;
				Integral.yGypo += AvgOffset.yGypo;
				Integral.zGypo += AvgOffset.zGypo;
				
				delay_cycles(CPU_Frq * 50);
		}
		//xAoffset = AvgOffset.xAcc = Integral.xAcc / Calibration_Period;
		//yAoffset = AvgOffset.yAcc = Integral.yAcc / Calibration_Period;
		//zAoffset = AvgOffset.zAcc = Integral.zAcc / Calibration_Period;
		xGoffset = AvgOffset.xGypo = Integral.xGypo / Calibration_Period;
		yGoffset = AvgOffset.yGypo = Integral.yGypo / Calibration_Period;
		zGoffset = AvgOffset.zGypo = Integral.zGypo / Calibration_Period;
		/*
		xAoffset = AvgOffset.xAcc = Integral.xAcc / Calibration_Period /AUnit[Aopt];
		yAoffset = AvgOffset.yAcc = Integral.yAcc / Calibration_Period /AUnit[Aopt];
		zAoffset = AvgOffset.zAcc = Integral.zAcc / Calibration_Period /AUnit[Aopt];
		xGoffset = AvgOffset.xGypo = Integral.xGypo / Calibration_Period /GUnit[Gopt];
		yGoffset = AvgOffset.yGypo = Integral.yGypo / Calibration_Period /GUnit[Gopt];
		zGoffset = AvgOffset.zGypo = Integral.zGypo / Calibration_Period /GUnit[Gopt];
		*/
		//OLED_ShowSignedNum(0,0,(int32_t)floor(xAoffset),6,16);
		//OLED_ShowSignedNum(0,1,(int32_t)floor(yAoffset),6,16);
		//OLED_ShowSignedNum(0,2,(int32_t)floor(zAoffset),6,16);
		OLED_ShowSignedNum(64,0,(int32_t)floor(xGoffset),6,16);
		OLED_ShowSignedNum(64,1,(int32_t)floor(yGoffset),6,16);
		OLED_ShowSignedNum(64,2,(int32_t)floor(zGoffset),6,16);
		OLED_ShowString(0,3,"Saving Offset");
		delay_cycles(CPU_Frq * 2000);
		OLED_Clear();
		return AvgOffset;
}

uint8_t MPU6500_Set_Gyro_Fsr(uint8_t fsr){
//����MPU6050�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ��
	return MPU6500_Write_Byte(GYRO_CONFIG, fsr<<3);//���������������̷�Χ  
}

uint8_t MPU6500_Set_Accel_Fsr(uint8_t fsr){
//����MPU6500���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 	
	return MPU6500_Write_Byte(ACCEL_CONFIG, fsr<<3);//���ü��ٶȴ����������̷�Χ  
}

uint8_t MPU6500_Set_LPF(uint16_t lpf){
//����MPU6500�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
//For the DLPF being able to be used, FCHOICE[1:0] must be set to 2��b11,FCHOICE_B[1:0]; is 2��b00.
//FCHOICE_B[1:0] is GYRO_CONFIG[1:0];
	
	uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU6500_Write_Byte(CONFIG,data);//�������ֵ�ͨ�˲���	
}


uint8_t MPU6500_Set_Rate(uint16_t rate){
//����MPU6500�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ��
//For being able to be used, FCHOICE[1:0] must be set to 2��b11,FCHOICE_B[1:0]; is 2��b00.
//FCHOICE_B[1:0] is GYRO_CONFIG[1:0];
	uint8_t data;
	if(rate>1000)rate=1000; //���1khz������
	if(rate<4)rate=4;
	data=1000/rate-1; //���ݹ�ʽ�����
	data = MPU6500_Write_Byte(SMPLRT_DIV,data);	//���ò�����
 	//return MPU_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
	return 0;
}
 

uint8_t MPU6500_Read_Len(uint8_t reg, uint8_t len,uint8_t *buf){ 
//
//ͬʱ������Ĵ���
//reg:��ʼ�Ĵ�����ַ
//len�����Ĵ������ܸ���
//*buf���洢�ڴ���ʼָ��
		uint8_t tmp=0;
		ReadWriteByte(MPU_INST, reg|0x80);//��ȡ�������λΪ1��
		while(len){
				*buf = ReadWriteByte(MPU_INST,0x00);
				--len;
				++buf;	
		}	
		return tmp;
}

uint8_t MPU6500_Set_xAOFFSET(int16_t xoffset){
	uint8_t tmpH,tmpL;
	tmpH = (xoffset >> 7) & 0xFF;							//��ȡ���ֽ�  
  tmpL = (xoffset & 0xFF) << 1;										//��ȡ���ֽ�  
	MPU6500_Write_Byte(XA_OFFSET_H,tmpH);
	MPU6500_Write_Byte(XA_OFFSET_L,tmpL);
	return 0;
}

uint8_t MPU6500_Set_yAOFFSET(int16_t yoffset){
	uint8_t tmpH,tmpL;
	tmpH = (yoffset >> 7) & 0xFF;							//��ȡ���ֽ�  
  tmpL = (yoffset & 0xFF) << 1;										//��ȡ���ֽ�  
	MPU6500_Write_Byte(YA_OFFSET_H,tmpH);
	MPU6500_Write_Byte(YA_OFFSET_L,tmpL);
	return 0;
}

uint8_t MPU6500_Set_zAOFFSET(int16_t zoffset){
	uint8_t tmpH,tmpL;
	tmpH = (zoffset >> 7) & 0xFF;							//��ȡ���ֽ�  
  tmpL = (zoffset & 0xFF) << 1;										//��ȡ���ֽ�  
	MPU6500_Write_Byte(ZA_OFFSET_H,tmpH);
	MPU6500_Write_Byte(ZA_OFFSET_L,tmpL);
	return 0;
}

uint8_t MPU6500_Set_xGOFFSET(int16_t xoffset){
	uint8_t tmpH,tmpL;
	tmpH = (xoffset >> 8) & 0xFF;							//��ȡ���ֽ�  
  tmpL = (xoffset & 0xFF);										//��ȡ���ֽ�  
	MPU6500_Write_Byte(XG_OFFSET_H,tmpH);
	MPU6500_Write_Byte(XG_OFFSET_L,tmpL);
	return 0;
}

uint8_t MPU6500_Set_yGOFFSET(int16_t yoffset){
	uint8_t tmpH,tmpL;
	tmpH = (yoffset >> 8) & 0xFF;							//��ȡ���ֽ�  
  tmpL = (yoffset & 0xFF);										//��ȡ���ֽ�  
	MPU6500_Write_Byte(YG_OFFSET_H,tmpH);
	MPU6500_Write_Byte(YG_OFFSET_L,tmpL);
	return 0;
}

uint8_t MPU6500_Set_zGOFFSET(int16_t zoffset){
	uint8_t tmpH,tmpL;
	tmpH = (zoffset >> 8) & 0xFF;							//��ȡ���ֽ�  
  tmpL = (zoffset & 0xFF);										//��ȡ���ֽ�  
	MPU6500_Write_Byte(ZG_OFFSET_H,tmpH);
	MPU6500_Write_Byte(ZG_OFFSET_L,tmpL);
	return 0;
}

struct MPUInfo MPU6500_getRawData(){
		struct MPUInfo nowValue;
		uint8_t raw_datas[14]={0}; //acc*6+temp*2+gyr*6
		uint8_t res;
		res = MPU6500_Read_Byte(INT_STATUS); //Ĭ�϶��˼Ĵ����ܹ���˱�־λ.��ѭ����ѯ�˼Ĵ�������
		//printf("int status:%x\n", res);
		delay_cycles(CPU_Frq * 50);
		if(res == 0x01) {
				//����ready
				MPU6500_Read_Len(ACCEL_XOUT_H, 8, &raw_datas[0]);
				delay_cycles(CPU_Frq * 50);
				MPU6500_Read_Len(GYRO_XOUT_H, 6, &raw_datas[8]);
				delay_cycles(CPU_Frq * 50);
				
				ACC_DATA[0] = ADecoder(((int16_t)raw_datas[0]		<<8) | raw_datas[1]);//������ٶ�
				ACC_DATA[1] = ADecoder(((int16_t)raw_datas[2]		<<8) | raw_datas[3]);
				ACC_DATA[2] = ADecoder(((int16_t)raw_datas[4]		<<8) | raw_datas[5]);
				ACC_DATA[3] = (((int16_t)raw_datas[8]		<<8) | raw_datas[9]);//������ٶ�
				ACC_DATA[4] = (((int16_t)raw_datas[10]	<<8) | raw_datas[11]);
				ACC_DATA[5] = (((int16_t)raw_datas[12]	<<8) | raw_datas[13]);
				ACC_DATA[6] = (((int16_t)raw_datas[6]		<<8) | raw_datas[7]); //�¶�����
				nowValue.xAcc  = ((double)ACC_DATA[0])	*10000.0	;//������ٶ�
				nowValue.yAcc  = ((double)ACC_DATA[1])	*10000.0	;
				nowValue.zAcc  = ((double)ACC_DATA[2])	*10000.0	;
				nowValue.xGypo = ((double)ACC_DATA[3])	*100.0;//������ٶ�
				nowValue.yGypo = ((double)ACC_DATA[4])	*100.0;
				nowValue.zGypo = ((double)ACC_DATA[5])	*100.0;
				nowValue.temp  = ((double)ACC_DATA[6]-RoomTempDegC) / 333.87 + 21; //�¶�����
		
		
				/*
				OLED_ShowNum(0,0,ACC_DATA[0],6,16);
				OLED_ShowNum(0,1,ACC_DATA[1],6,16);
				OLED_ShowNum(0,2,ACC_DATA[2],6,16);
				OLED_ShowNum(64,0,ACC_DATA[3],6,16);
				OLED_ShowNum(64,1,ACC_DATA[4],6,16);
				OLED_ShowNum(64,2,ACC_DATA[5],6,16);
				OLED_ShowNum(0,3,ACC_DATA[6],6,16);
				OLED_ShowSignedNum(0,0,ACC_DATA[0],6,16);
				OLED_ShowSignedNum(0,1,ACC_DATA[1],6,16);
				OLED_ShowSignedNum(0,2,ACC_DATA[2],6,16);
				OLED_ShowSignedNum(64,0,ACC_DATA[3],6,16);
				OLED_ShowSignedNum(64,1,ACC_DATA[4],6,16);
				OLED_ShowSignedNum(64,2,ACC_DATA[5],6,16);
				OLED_ShowSignedNum(0,3,ACC_DATA[6],6,16);
				*/	
		}
		return nowValue;
}

struct MPUInfo MPU6500_getData(){
		struct MPUInfo nowValue;
		uint8_t raw_datas[14]={0}; //acc*6+temp*2+gyr*6
		uint8_t res;
		res = MPU6500_Read_Byte(INT_STATUS); //Ĭ�϶��˼Ĵ����ܹ���˱�־λ.��ѭ����ѯ�˼Ĵ�������
		//printf("int status:%x\n", res);
		delay_cycles(CPU_Frq * 50);
		if(res == 0x01) {
				//����ready
				MPU6500_Read_Len(ACCEL_XOUT_H, 8, &raw_datas[0]);
				delay_cycles(CPU_Frq * 50);
				MPU6500_Read_Len(GYRO_XOUT_H, 6, &raw_datas[8]);
				delay_cycles(CPU_Frq * 50);
				
				ACC_DATA[0] = ADecoder(((int16_t)raw_datas[0]		<<8) | raw_datas[1]);//������ٶ�
				ACC_DATA[1] = ADecoder(((int16_t)raw_datas[2]		<<8) | raw_datas[3]);
				ACC_DATA[2] = ADecoder(((int16_t)raw_datas[4]		<<8) | raw_datas[5]);
				ACC_DATA[3] = (((int16_t)raw_datas[8]		<<8) | raw_datas[9]);//������ٶ�
				ACC_DATA[4] = (((int16_t)raw_datas[10]	<<8) | raw_datas[11]);
				ACC_DATA[5] = (((int16_t)raw_datas[12]	<<8) | raw_datas[13]);
				ACC_DATA[6] = (((int16_t)raw_datas[6]		<<8) | raw_datas[7]); //�¶�����
				nowValue.xAcc  = (((double)ACC_DATA[0])	*10000.0	 - xAoffset)		/AUnit[Aopt];//������ٶ�
				nowValue.yAcc  = (((double)ACC_DATA[0])	*10000.0	 - yAoffset)		/AUnit[Aopt];
				nowValue.zAcc  = (((double)ACC_DATA[0])	*10000.0	 - zAoffset)		/AUnit[Aopt];
				nowValue.xGypo = ((double)ACC_DATA[3]		*100.0		 - xGoffset)		/GUnit[Gopt];//������ٶ�
				nowValue.yGypo = ((double)ACC_DATA[4]		*100.0		 - yGoffset)		/GUnit[Gopt];
				nowValue.zGypo = ((double)ACC_DATA[5]		*100.0		 - zGoffset)		/GUnit[Gopt];
				nowValue.temp  = ((double)ACC_DATA[6]-RoomTempDegC) / 333.87 + 21; //�¶�����
		
				/*
				OLED_ShowNum(0,0,ACC_DATA[0],6,16);
				OLED_ShowNum(0,1,ACC_DATA[1],6,16);
				OLED_ShowNum(0,2,ACC_DATA[2],6,16);
				OLED_ShowNum(64,0,ACC_DATA[3],6,16);
				OLED_ShowNum(64,1,ACC_DATA[4],6,16);
				OLED_ShowNum(64,2,ACC_DATA[5],6,16);
				OLED_ShowNum(0,3,ACC_DATA[6],6,16);
				OLED_ShowSignedNum(0,0,ACC_DATA[0],6,16);
				OLED_ShowSignedNum(0,1,ACC_DATA[1],6,16);
				OLED_ShowSignedNum(0,2,ACC_DATA[2],6,16);
				OLED_ShowSignedNum(64,0,ACC_DATA[3],6,16);
				OLED_ShowSignedNum(64,1,ACC_DATA[4],6,16);
				OLED_ShowSignedNum(64,2,ACC_DATA[5],6,16);
				OLED_ShowSignedNum(0,3,ACC_DATA[6],6,16);
				*/	
		}
		return nowValue;
}


void MPU6500_UARTDebugger(){
		struct MPUInfo nowValue = MPU6500_getData();
		/*
		MCUTransData16(UART_0_INST,&nowValue.xAcc,1);
		MCUTransData16(UART_0_INST,&nowValue.yAcc,1);
		MCUTransData16(UART_0_INST,&nowValue.zAcc,1);
		*/
		OLED_ShowSignedNum(0,1,(int32_t)floor(nowValue.xAcc),8,12);
		OLED_ShowSignedNum(0,2,(int32_t)floor(nowValue.yAcc),8,12);
		OLED_ShowSignedNum(0,3,(int32_t)floor(nowValue.zAcc),8,12);
		OLED_ShowSignedNum(64,1,(int32_t)floor(nowValue.xGypo),8,12);
		OLED_ShowSignedNum(64,2,(int32_t)floor(nowValue.yGypo),8,12);
		OLED_ShowSignedNum(64,3,(int32_t)floor(nowValue.zGypo),8,12);
		OLED_ShowSignedNum(64,0,nowValue.temp,6,16);
}
