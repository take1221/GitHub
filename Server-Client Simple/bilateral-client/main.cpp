#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <math.h>
#include <time.h>

#include <GL/glu.h>
#include <GL/gl.h>
#include <string.h>
#include <malloc.h>
#include <windows.h>
#include <conio.h>
#include "connect.h"
#include "socket.h"

#define SERVER_IP "123.123.123.123"
//#define SPAIN  //���̏ꍇ�̓R�����g�A�E�g����

//���COM�|�[�g��ύX����

unsigned char ch=0,key;
float t=0;
unsigned long start,end;

float Kp=20.0, Kv=15.0;

int netd, fd; /*�ʐM�̕ϐ�*/

float point_tx=0.0, point_ty=0.0; //����̃|�C���^���W
float point_x=0.0, point_y=0.0;   //�|�C���^�̍��W

float FX=0, FY=0;               // �͊o�p
int star = 0;	//�������L�[�h���h�p�̕ϐ�
///////////////////////

#define M_PI 3.14159265238979 //�~����

union data_valf{/*�ϊ��֐��L�����N�^�t���[�g*/
	float fdata;
	unsigned char cdata[4];
}data;

union data_val{
	signed short sdata;
	char cdata[2];
}data_s;


#define aL0 27.0
#define aL1 30.0
unsigned long count=0;
unsigned long Ccount=0;


void drafting_table()
{
	short edata[2]={0};
	short ddata[2]={0};
	unsigned char wbuf[4];

	float s1 = 0, s0 = 0;
	float enc1 = 0, enc2 = 0;
	float X1=0, Y1=0, X2=0, Y2=0, R=0, theta=0, cos_Alpha=0, Alpha=0;
	float tau1=0, tau2=0;
	float old_x=0, old_y=0, point_speed=0;

	float r1 = 0, r2 = 0;

	Open_Comm1();

	data_s.sdata=0;
	wbuf[0]=data_s.cdata[0];
	wbuf[1]=data_s.cdata[1];
	data_s.sdata=0;
	wbuf[2]=data_s.cdata[0];
	wbuf[3]=data_s.cdata[1];
	Write_Comm1(4,wbuf); /* �ŏ��Ɉ�x�f�[�^�𑗂�F�i�f�[�^�ʐM�J�n�j*/

	key = 0;
	start=GetTickCount();

	while(!ch){
		Read_Comm1(4,wbuf);

		data_s.cdata[0]=wbuf[0];
		data_s.cdata[1]=wbuf[1];
		edata[0]=data_s.sdata;
		data_s.cdata[0]=wbuf[2];
		data_s.cdata[1]=wbuf[3];
		edata[1]=data_s.sdata;

		if (star == 1) {//������
			r1 = edata[0];
			r2 = edata[1];
			star = 0;
		}

		//enc1 =  90.0 -(edata[0]-r1)/100.00;
		//enc2 =  (edata[1]-r2)/100.00;
#ifdef SPAIN
		enc1 =  90.0+(edata[0]-r1)/(8192.0/360.0)*2.0;
		enc2 =       -(edata[1]-r2)/(8192.0/360.0)*2.0;
#else
		enc1 =  90.0+(edata[0]-r1)/(8192.0/360.0);
		enc2 =       -(edata[1]-r2)/(8192.0/360.0);
#endif
		//���W�A���ɕϊ�
		s0 = (enc1)*M_PI/180.0;
		s1 = (enc2)*M_PI/180.0;
		X1 = -3.0-aL0*cos(s0);
		Y1 = -aL0*sin(s0);
		X2 = 3.0+aL0*cos(s1);
		Y2 = -aL0*sin(s1);

		R = sqrt(((X2-X1)*(X2-X1))+((Y2-Y1)*(Y2-Y1)));
		theta = atan((Y2-Y1)/(X2-X1));
		cos_Alpha = (R)/(2*aL1);
		Alpha = acos(cos_Alpha);

		point_x = (X1+aL1*cos(theta-Alpha));
		point_y = (Y1+aL1*sin(theta-Alpha));
		point_speed = sqrt(pow((old_x-point_x),2) + pow((old_y-point_y),2));

		FX=(point_tx-point_x)*Kp - (point_x-old_x)*Kv;
		FY=(point_ty-point_y)*Kp - (point_y-old_y)*Kv;

		//�͂��t�ɕt�������ׁA�΍�
		FX *= -1;
		FY *= -1;

		old_x = point_x;
		old_y = point_y;

		float dx1 = point_x + 3 + aL0*cos(s0);
		float dx2 = point_x - 3 - aL0*cos(s1);
		float dy1 = point_y + aL0*sin(s0);
		float dy2 = point_y + aL0*sin(s1);

		float ddd = dx1*dy2-dx2*dy1;
		float d11 = (point_x+3.0)*aL0*sin(s0)-point_y*aL0*cos(s0);
		float d12 = 0.0;
		float d21 = 0.0;
		float d22 = -(point_x+3.0)*aL0*sin(s1)-point_y*aL0*cos(s1);

		tau1 = (( dy2*d11 - dx2*d21)*FX + ( dy2*d12 - dx2*d22)*FY)/ddd;
		tau2 = ((-dy1*d11 + dx1+d21)*FX + (-dy1*d12 + dx1*d22)*FY)/ddd;

#ifdef SPAIN 
		if(tau1>20000)		tau1=20000;
		if(tau1<-20000)		tau1=-20000;

		if(tau2>20000)			tau2=20000;
		if(tau2<-20000)			tau2=-20000;
#else
		if(tau1>2000)		tau1=2000;
		if(tau1<-2000)		tau1=-2000;

		if(tau2>2000)			tau2=2000;
		if(tau2<-2000)			tau2=-2000;
#endif
		ddata[0] = (int)tau1;
		ddata[1] = -(int)tau2;

		data_s.sdata=ddata[0];
		wbuf[0]=data_s.cdata[0];
		wbuf[1]=data_s.cdata[1];
		data_s.sdata=ddata[1];
		wbuf[2]=data_s.cdata[0];
		wbuf[3]=data_s.cdata[1];

		Write_Comm1(4,wbuf);
		t++;
		count++;
	}
	Read_Comm1(4,wbuf);
	data_s.sdata=0;
	wbuf[0]=data_s.cdata[0];
	wbuf[1]=data_s.cdata[1];
	data_s.sdata=0;
	wbuf[2]=data_s.cdata[0];
	wbuf[3]=data_s.cdata[1];
	Write_Comm1(4,wbuf);
	Read_Comm1(4,wbuf);

	Close_Comm1();
	ch=0;
}

void client_func(void)
{
	unsigned char buff[10];
	while(!ch){
		ReadSocket(netd,8,buff); /* �N���C�A���g��8�o�C�g��M */

		data.cdata[0]=buff[0];
		data.cdata[1]=buff[1];
		data.cdata[2]=buff[2];
		data.cdata[3]=buff[3];
		point_tx=data.fdata;
		data.cdata[0]=buff[4];
		data.cdata[1]=buff[5];
		data.cdata[2]=buff[6];
		data.cdata[3]=buff[7];
		point_ty=data.fdata;

		data.fdata=point_x;
		buff[0]=data.cdata[0];
		buff[1]=data.cdata[1];
		buff[2]=data.cdata[2];
		buff[3]=data.cdata[3];
		data.fdata=point_y;
		buff[4]=data.cdata[0];
		buff[5]=data.cdata[1];
		buff[6]=data.cdata[2];
		buff[7]=data.cdata[3];

		WriteSocket(netd,8,buff); /* �N���C�A���g��8�o�C�g���M*/

		Ccount++;
	}
}


void display(void)						/*��ʕ`�掞�ɌĂ΂��*/
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0.0, 0.0, 150.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);	//�^����(z��)

	glPushMatrix();							  //��
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(point_x, point_y, 10);
	glutSolidSphere(2, 15,5);
	glPopMatrix();

	glPushMatrix();							  //��
	glColor3f(1.0, 0.0, 1.0);
	glTranslatef(point_tx, point_ty, 10);
	glutSolidSphere(2, 15,5);
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

void init(void)
{

	glClearColor(0.0, 0.1, 0.4, 0.1);				/*��ʃN���A*/
	//glShadeModel(GL_FLAT);							/*�t���b�g�V�F�[�f�B���O*/
	glShadeModel(GL_SMOOTH);							/*�X���[�X�V�F�[�f�B���O*/

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);

	InitSocket();                  /* �\�P�b�g�̏����� */
	if(!OpenClient(&netd, 60001, SERVER_IP)){
		fprintf(stderr,"open error\n");
		exit(1);
	}
}

void reshape(int w, int h)							/*��ʃT�C�Y�ύX���ɌĂ΂��*/
{
	glViewport(0,0, (GLsizei) w, (GLsizei) h);		/*��ʂ̏o��͈�*/
	glMatrixMode(GL_PROJECTION);					/*���E�ϊ��錾*/
	glLoadIdentity();								/*�P�ʍs����*/
	glOrtho(-29.0, 29.0, -45.5, -13.0, -1.0, 10000.0);	/*���̐ς̐ݒ�*/
	//glFrustum(-14.2, 14.2, -7.95, 7.95, 75.7575, 200.0);
	//glFrustum(-29/2, 29/2, -45.5/2,-13/2, 75, 175);
	glMatrixMode(GL_MODELVIEW);						/*���f���ϊ��錾*/
	glLoadIdentity();								/*�P�ʍs����*/
}

void keyboard(unsigned char key, int x, int y)
{
	int tcount=0;

	switch(key) {

	case 's': //�|�C���^�̏�����
		star = 1;
		break;

	case 'q'://�I��
	case 'Q':
		ch = 1;
		end=GetTickCount();

		printf("connection closed by client %.2f[Hz]\n", (float)count*1000/(end-start));
		while(ch){
			Sleep(1);
			tcount++;
			if(tcount>1000)break;
		}
		exit(1);
		break;
	case 'c':
		/* �ڑ��I�� */
		CloseServer(fd);
		QuitServer();
		break;

	}
}

int main(int argc, char** argv)
{
	HANDLE hthread;
	DWORD lpThreadId;

	HANDLE tcp_hthread;
	DWORD tcp_lpThreadId;

	glutInit(&argc, argv);        /*GLUT������*/
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1360, 768); /*��ʂ̑傫���ݒ�*/
	glutInitWindowPosition(0, 0); /*��ʂ̈ʒu�ݒ�*/
	glutCreateWindow("drafter client"); /*�����̖��O�̃E�C���h�E�쐬*/

	init();
	//  glutFullScreen();             /*�t���X�N���[�����[�h*/

	hthread=CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)drafting_table, NULL,
		0, &lpThreadId);
	tcp_hthread=CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)client_func, NULL,
		0, &tcp_lpThreadId);

	glutIdleFunc(display);        /*�A�C�h�����ɌĂԊ֐�*/
	glutDisplayFunc(display);     /*�\�����ɌĂԊ֐�*/
	glutReshapeFunc(reshape);     /*��ʃT�C�Y�ύX���ɌĂԊ֐�*/
	glutKeyboardFunc(keyboard);   /*�L�[�{�[�h*/


	glutMainLoop();               /*�`��J�n*/

	return 0;
}

