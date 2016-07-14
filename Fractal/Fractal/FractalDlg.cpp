
// FractalDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "Fractal.h"
#include "FractalDlg.h"
#include "afxdialogex.h"
#include <math.h>
#include "time.h"
#define BMP_Header_Length 54
#define WindowWidth 800
#define WindowHeight 800
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFractalDlg �Ի���



CFractalDlg::CFractalDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFractalDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFractalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_slider1);
	DDX_Control(pDX, IDC_SLIDER2, m_slider2);
	DDX_Control(pDX, IDC_SLIDER3, m_slider3);
	//  DDX_Text(pDX, IDC_MAXH, m_maxh);
	DDX_Control(pDX, IDC_SLIDER4, m_R);
	DDX_Control(pDX, IDC_SLIDER5, m_G);
	DDX_Control(pDX, IDC_SLIDER6, m_B);
}

BEGIN_MESSAGE_MAP(CFractalDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEWHEEL()
ON_WM_MOUSEMOVE()
ON_WM_TIMER()
ON_BN_CLICKED(IDC_BUTTON1, &CFractalDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_CHECK1, &CFractalDlg::OnBnClickedCheck1)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CFractalDlg::OnDeltaposSpin1)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CFractalDlg::OnDeltaposSpin2)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CFractalDlg::OnDeltaposSpin3)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CFractalDlg::OnNMCustomdrawSlider1)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CFractalDlg::OnNMCustomdrawSlider2)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &CFractalDlg::OnNMCustomdrawSlider3)
ON_BN_CLICKED(IDC_CHECK2, &CFractalDlg::OnBnClickedCheck2)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER4, &CFractalDlg::OnNMCustomdrawSlider4)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER5, &CFractalDlg::OnNMCustomdrawSlider5)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER6, &CFractalDlg::OnNMCustomdrawSlider6)
END_MESSAGE_MAP()


// CFractalDlg ��Ϣ�������

BOOL CFractalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CWnd* wnd = GetDlgItem(IDC_RENDER);                                // ��ʼ��MFC�е�OpenGL����
    hrenderDC = ::GetDC(wnd->m_hWnd);
    if(SetWindowPixelFormat(hrenderDC) == FALSE) {                // ����hDC�����ظ�ʽ
        return 0;
    }
    if(CreateViewGLContext(hrenderDC) == FALSE) {                    // ��hDCת���õ�hRC
        return 0;
    }
    // openGL�ĳ�ʼ������
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glViewport(0, 0, 600, 550);
    glMatrixMode(GL_PROJECTION);
    //gluPerspective(45, 1, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // ���ü�ʱ��,10msˢ��һ��
    //SetTimer(1, 10, 0);
	//MessageBox("��ʼ��������", "attention", MB_OK);
	PI = 3.1415926535897;
    tAngInc = PI / 90;                      // ÿ�δ�������¼��任�ĽǶ�ֵ
    tFovy = 45.0;
    prePt.SetPoint(-1, -1);                 // ��ʼ��prePt & nowPt(��OnMouseMove��ʹ��)
    nowPt.SetPoint(-1, -1);
    tVerticalAng = 0.;
    tHorizonAng = PI / 2;
    tRadius = 400.0;
    tEyeX = tRadius * cos(tVerticalAng) * cos(tHorizonAng);
    tEyeY = tRadius * sin(tVerticalAng);
    tEyeZ = tRadius * cos(tVerticalAng) * sin(tHorizonAng);
    tCenterX = 0.;
    tCenterY = 0.;
    tCenterZ = 0.;
    tUpX = 0.;
    tUpY = 1.0;
    tUpZ = 0.;
	//glEnable(GL_TEXTURE_2D);//����ӳ�俪�أ�����ע�⣡����
	texGround = load_texture("ground.bmp");
	srand(time(NULL));

	hmin = -30; // �Ŷ��������Сֵ
	hmax = 75;
	minscale = -100;
	maxscale = 100;
	
	isSmooth = TRUE;
	isLines = FALSE;
	frac = 4;
	CString str;
    str.Format(_T("  %d "), frac);//��ʼ���ؼ��е���ֵ
	GetDlgItem(IDC_EDIT1)->SetWindowText(str);
	str.Format(_T("  %d "), hmax);
	GetDlgItem(IDC_MAXH)->SetWindowText(str);
	str.Format(_T("  %d "), hmin);
	GetDlgItem(IDC_MINH)->SetWindowText(str);
	m_slider1.SetRange(50,100);
	m_slider1.SetPos(75);
	m_slider2.SetRange(0,60);
	m_slider2.SetPos(30);
	m_slider3.SetRange(0,8);
	m_slider3.SetPos(4);
	m_R.SetRange(0,255);
	m_R.SetPos(255);
	m_G.SetRange(0,255);
	m_G.SetPos(127);
	m_B.SetRange(0,255);
	m_B.SetPos(0);
	OnBnClickedButton1();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CFractalDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFractalDlg::OnPaint()
{
	//RenderScene();
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ
HCURSOR CFractalDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CFractalDlg::SetWindowPixelFormat(HDC hDC) { 
    PIXELFORMATDESCRIPTOR pixelDesc;
    pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pixelDesc.nVersion = 1;
    pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA;
    pixelDesc.iPixelType = PFD_TYPE_RGBA;
    pixelDesc.cColorBits = 32;
    pixelDesc.cRedBits = 0;
    pixelDesc.cRedShift = 0;
    pixelDesc.cGreenBits = 0;
    pixelDesc.cGreenShift = 0;
    pixelDesc.cBlueBits = 0;
    pixelDesc.cBlueShift = 0;
    pixelDesc.cAlphaBits = 0;
    pixelDesc.cAlphaShift = 0;
    pixelDesc.cAccumBits = 0;
    pixelDesc.cAccumRedBits = 0;
    pixelDesc.cAccumGreenBits = 0;
    pixelDesc.cAccumBlueBits = 0;
    pixelDesc.cAccumAlphaBits = 0;
    pixelDesc.cDepthBits = 0;
    pixelDesc.cStencilBits = 1;
    pixelDesc.cAuxBuffers = 0;
    pixelDesc.iLayerType = PFD_MAIN_PLANE;
    pixelDesc.bReserved = 0;
    pixelDesc.dwLayerMask = 0;
    pixelDesc.dwVisibleMask = 0;
    pixelDesc.dwDamageMask = 0;

    PixelFormat = ChoosePixelFormat(hDC, &pixelDesc);
    if(PixelFormat == 0) {
        PixelFormat = 1;
        if(DescribePixelFormat(hDC, PixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc) == 0) {
            return FALSE;
        }
    }
    if(SetPixelFormat(hDC, PixelFormat, &pixelDesc) == FALSE) {
        return FALSE;
    }
    return TRUE;
}

BOOL CFractalDlg::CreateViewGLContext(HDC hDC) {
    hrenderRC = wglCreateContext(hDC);
    if(hrenderRC == NULL) {
        return FALSE;
    }
    if(wglMakeCurrent(hDC, hrenderRC) == FALSE) {
        return FALSE;
    }
    return TRUE;
}
//���ƿ��ƺ����������˸�����Ʋ��������ڲ������������Ļ��ƺ���
void CFractalDlg::RenderScene() {
	glMatrixMode(GL_PROJECTION);                            // ����modelview �� projection
    glLoadIdentity();                                       // �˴����䲻����glLoadIdentity()
    gluPerspective(tFovy, 1, 0.1, 1000.0);                  // ע��zNear,zFar��ȡֵ
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(tEyeX, tEyeY, tEyeZ, tCenterX, tCenterY, tCenterZ, tUpX, tUpY, tUpZ);

	{
    glClear(GL_COLOR_BUFFER_BIT);
	GLfloat lmodel_ambient[]={R,G,B,1.0f}; //���廷����
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient); //���û�����
	GLfloat light0_ambient[]={R,G,B,1.0}; //�����Դ������
	GLfloat light0_diffuse[]={1.0,0.5,0.0,1.0}; //�����Դɢ���
	GLfloat light0_specular[]={1.0,0.5,0.0,1.0}; //�����Դ�����
	GLfloat light0_position[]={160.0,160.0,50.0,1.0}; //�����Դλ��
	glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient); //���ù�Դ������
	glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse); //���ù�Դɢ���
	glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);//���ù�Դ�����
	glLightfv(GL_LIGHT0,GL_POSITION,light0_position); //���ù�Դλ��
	glEnable(GL_LIGHT0); //���ù�Դ
	glEnable(GL_LIGHTING); //���ù���Ч��
	}
	//{
	//GLfloat brass_ambient[]={0.5f,0.5f,0.0f,1.0f}; //������ʻ�����
	//GLfloat brass_diffuse[]={0.5f,0.5f,0.0f,1.0f}; //�������ɢ���
	//GLfloat brass_specular[]={0.5f,0.5f,0.0f,1.0};//������ʷ����
	//GLfloat brass_sinines[]={20.0f}; //������ʾ��淢��ǿ��
	//glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,brass_ambient); //���ò��ʻ�����
	//glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,brass_diffuse); //���ò���ɢ���
	//glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,brass_specular); //���ò��ʷ����
	//glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,brass_sinines); //���ò��ʾ��淴��ǿ��
	//
	//}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
 
	makeObject();//����������л���
    SwapBuffers(hrenderDC);//��������
}


//����ÿ������Ҫ�������飨ˢ��/��ͼ��
void CFractalDlg::OnTimer(UINT nIDEvent) {
    RenderScene();
    CDialog::OnTimer(nIDEvent);
}


//ͨ�����ֲ��������ӽ�
afx_msg BOOL CFractalDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	//MessageBox("mouse move function triggered!", "attentino", MB_OK);
    int tWheelCount = zDelta / 120;
    if(tWheelCount > 0) {
        tFovy += 1.0;
    } else if(tWheelCount < 0) {
        tFovy -= 1.0;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();                                            // �˴����䲻����glLoadIdentity()
    gluPerspective(tFovy, 1, 0.1, 1000.0);                        // ע��zNear,zFar��ȡֵ
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    return TRUE;
}


// �ڽ�����ʾ��Ļ��ͨ���������任�����ӽ�
afx_msg void CFractalDlg::OnMouseMove(UINT nFlags, CPoint point) {

    if((nFlags & MK_LBUTTON) == TRUE) {

        //MessageBox("mouse move function triggered!", "attention", MB_OK);

        nowPt.x = point.x;
        nowPt.y = point.y;
        if(prePt.x!=-1 && prePt.y!=-1 && nowPt.x!=-1 && nowPt.y!=-1) {
            double tDx = nowPt.x - prePt.x;
            double tDy = nowPt.y - prePt.y;
            double tDis = sqrt(tDx*tDx+tDy*tDy);
            if(tDx > 0.) {
                tHorizonAng += tAngInc * tDx / tDis;
                if(tHorizonAng < 0.) { tHorizonAng += 2*PI; }
                if(tHorizonAng > 2*PI) { tHorizonAng -= 2*PI; }
            } else if(tDx < 0.) {
                tHorizonAng += tAngInc * tDx / tDis;
                if(tHorizonAng < 0.) { tHorizonAng += 2*PI; }
                if(tHorizonAng > 2*PI) { tHorizonAng -= 2*PI; }
            }
            if(tDy > 0.) {
                tVerticalAng = tVerticalAng + tAngInc * tDy / tDis;
                if(tVerticalAng > PI/2) { tVerticalAng = PI/2; }
            } else if(tDy < 0.) {
                tVerticalAng = tVerticalAng + tAngInc * tDy / tDis;
                if(tVerticalAng < -PI/2) { tVerticalAng = -PI/2; }
            }

            tEyeX = tRadius * cos(tVerticalAng) * cos(tHorizonAng);
            tEyeY = tRadius * sin(tVerticalAng);
            tEyeZ = tRadius * cos(tVerticalAng) * sin(tHorizonAng);
        }
        prePt.x = nowPt.x;
        prePt.y = nowPt.y;
    }
}


//���ƺ���
 GLuint CFractalDlg::makeObject()
 {
     GLfloat * vert = getTriangles();
     int length = getTriangleNum();
     GLuint list = glGenLists(1);
	 if(isLines)
	 {
		 for(int i = 0;i < getTriangleNum()/3;i++)
		 {
			glBegin(GL_LINE_LOOP);
			glVertex3f(vert[9 * i + 0],vert[9 * i + 1],vert[9 * i + 2]);
			glVertex3f(vert[9 * i + 3],vert[9 * i + 4],vert[9 * i + 5]);
			glVertex3f(vert[9 * i + 6],vert[9 * i + 7],vert[9 * i + 8]);
			glEnd();
		 
		 }
	 }
	 else{
		 for(int i = 0;i < getTriangleNum()/3;i++)
		 {
			glBindTexture(GL_TEXTURE_2D, texGround);
			glBegin(GL_TRIANGLES);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(vert[9 * i + 0],vert[9 * i + 1],vert[9 * i + 2]);
			glTexCoord2f(0.0f, 5.0f);
			glVertex3f(vert[9 * i + 3],vert[9 * i + 4],vert[9 * i + 5]);
			glTexCoord2f(5.0f, 0.0f);
			glVertex3f(vert[9 * i + 6],vert[9 * i + 7],vert[9 * i + 8]);
			glEnd();
		 
		 }
	 }
	 if(vert != NULL)//��ֹ�ڴ�й©
		 delete []vert;

     return list;
 }


 //��������ɡ���ť�Ĳ���
 void CFractalDlg::OnBnClickedButton1()
 {
	 init(frac);
	 midpoint(frac,frac,0,0);
	 SetTimer(1, 30, 0);
	 CString str;
	 str.Format(_T("  %d "), getTriangleNum()/3);
	 GetDlgItem(IDC_TRINUM)->SetWindowText(str);
 }


 //���ĺ�����ͨ�������Լ����ÿ����ĸ߶�ֵ�������������Ա����
int CFractalDlg::midpoint(int i, int start, int topx, int topy) {
	 //MessageBox("����midpoint����!", "attention", MB_OK);
  if (i < 1) {//������ֹ����
    return 0;
  }

  int len = (int)pow(2.0f, i);//���㱾�ε����ĳ���

  int x1 = topx + len / 2;//�����е�
  int y1 = topy;

  int x2 = topx + len;//�Ҳ��е�
  int y2 = topy + len / 2;

  int x3 = x1;//�ײ��е�
  int y3 = y1 + len;

  int x4 = x2 - len;//�����е�
  int y4 = y2;

  int xmid = x1;//�����е�
  int ymid = y2;

  //���������ĸ߶ȣ�z��ֵ

		
  // ����
  data[x1][y1] =
      data[topx][topy] + (data[topx + len][topy] - data[topx][topy]) / 2;

  // �Ҳ�
  data[x2][y2] = data[topx + len][topy] +
                 (data[topx + len][topy + len] - data[topx + len][topy]) / 2;
  // �ײ�
  data[x3][y3] = data[topx][topy + len] +
                 (data[topx + len][topy + len] - data[topx][topy + len]) / 2;
  // ��
  data[x4][y4] =
      data[topx][topy] + (data[topx][topy + len] - data[topx][topy]) / 2;

  // ����
  data[xmid][ymid] = data[x1][y1] + (data[x3][y3] - data[x1][y1]) / 2;

  // ���ŵ����������ӣ������е��Ŷ��ķ�Χ
  float min =
      hmin * (((float)start - ((float)start - (float)i)) / (float)start);
  float max =
      hmax * (((float)start - ((float)start - (float)i)) / (float)start);
  // ��������Ŷ�ֵ
  data[xmid][ymid] +=
      (int)(min + ((float)rand() / (float)RAND_MAX) * (max - min));
  //str.Format("data[xmid][ymid]=data[%d][%d]=%d,", xmid,ymid,data[xmid][ymid]);
  //MessageBox(str); 

  i--;//������ֹ�����Լ�

  // ���ϵ���
  midpoint(i, start, topx, topy);

  // ���ϵ���
  midpoint(i, start, x1, y1);

  // ���µ���
  midpoint(i, start, x4, y4);

  // ���µ���
  midpoint(i, start, xmid, ymid);
  return 0;
}


//��ʼ�����������ڱ�����������Ķ�ά����ռ�
void CFractalDlg::init(int s) {
  if(data != NULL)
  {
	  for (int i = 0; i < length; i++)
		delete []data[i];
	  delete []data;
  }

  length = (int)pow(2.0f, s) + 1;
  data = new int *[length];
  for (int i = 0; i < length; i++)
    data[i] = new int[length];

  // Fill arrray with zeros
  for (int y = 0; y < length; y++) {
    for (int x = 0; x < length; x++) {
      data[x][y] = 0;
    }
  }
}


//���ź�����ʹ�����۳�ʼ�����εı߳�Ϊ���ٶ���������ʾ
GLfloat CFractalDlg::scale(int data) {

  return (GLfloat)(minscale + (float)data/(float)(length-1) * (maxscale - minscale));

}


//ȡ�û��������ε���������
GLfloat * CFractalDlg::getTriangles()
{
    GLfloat * triangles = new GLfloat[(length-1) * (length-1) *18];
    //GLfloat * triangles = new GLfloat[(length-1) * (length-1) *12];

	int i =0;
	for(int y=0;y<length-1;y++) {
			for(int x=0;x<length-1;x++) {

				triangles[i++] = scale(x);
				triangles[i++] = scale(y);
				triangles[i++] = data[x][y];

				triangles[i++] = scale(x+1);
				triangles[i++] = scale(y);
				triangles[i++] = data[x+1][y];

				triangles[i++] = scale(x);
				triangles[i++] = scale(y+1);
				triangles[i++] = data[x][y+1];

				triangles[i++] = scale(x+1);
				triangles[i++] = scale(y+1);
				triangles[i++] = data[x+1][y+1];

                triangles[i++] = scale(x);
				triangles[i++] = scale(y+1);
				triangles[i++] = data[x][y+1];

				triangles[i++] = scale(x+1);
				triangles[i++] = scale(y);
                triangles[i++] = data[x+1][y];

			}
	}
 return triangles;
}


//��ȡ�����θ�������ȷ����ͼʱ��Ҫ���ƶ��ٴ�
int CFractalDlg::getTriangleNum()
{
    return (length-1) * (length-1) *6;
}


//���ƻ�ͼ��ʽ��ƽ��/��ƽ����Ӱ
void CFractalDlg::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(isSmooth)
	{
		isSmooth = FALSE;
		glShadeModel(GL_FLAT);
	}
	else
	{
		isSmooth = TRUE;
		glShadeModel(GL_SMOOTH);
	}
}


void CFractalDlg::grab(void)
{
	FILE*
		pDummyFile;
	FILE*
		pWritingFile;
	GLubyte* pPixelData;
	GLubyte	BMP_Header[BMP_Header_Length];
	GLint	i, j;
	GLint	PixelDataLength;	// �����������ݵ�ʵ�ʳ���
	i = WindowWidth * 3;	// �õ�ÿһ�е��������ݳ���
	while( i%4 != 0 )	// �������ݣ�ֱ��i�ǵı���
		++i;
	PixelDataLength = i * WindowHeight;	// �����ڴ�ʹ��ļ�
	pPixelData = (GLubyte*)malloc(PixelDataLength);
	if( pPixelData == 0 )
	{
		exit(0);
	}
	pDummyFile = fopen("dummy.bmp", "rb");
	if( pDummyFile == 0 )
		exit(0);
	pWritingFile = fopen("grab.bmp", "wb");
	if( pWritingFile == 0 )
		exit(0);	// ��ȡ����
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, WindowWidth, WindowHeight,GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);	// ��dummy.bmp���ļ�ͷ����Ϊ���ļ����ļ�ͷ
	fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);
	fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
	fseek(pWritingFile, 0x0012, SEEK_SET);
	i = WindowWidth;
	j = WindowHeight;
	fwrite(&i, sizeof(i), 1, pWritingFile);
	fwrite(&j, sizeof(j), 1, pWritingFile);	// д����������
	fseek(pWritingFile, 0, SEEK_END);
	fwrite(pPixelData, PixelDataLength, 1, pWritingFile);	// �ͷ��ڴ�͹ر��ļ�
	fclose(pDummyFile);
	fclose(pWritingFile);
	free(pPixelData);
} 


/* ����power_of_two* ���һ�������Ƿ�Ϊ2�������η�������ǣ�����1�����򷵻�0
* ʵ����ֻҪ�鿴�������λ���ж��ٸ������������1��������1�����򷵻�0
* �ڡ��鿴�������λ���ж��ٸ���ʱʹ����һ��С����
* ʹ��n &= (n-1)����ʹ��n�еļ���һ��������ԭ���ҿ����Լ�˼����*/
int CFractalDlg::power_of_two(int n)
{
	if( n <= 0 )
		return 0;
	return (n & (n-1)) == 0;
}	


//����load_texture* ��ȡһ��BMP�ļ���Ϊ����* ���ʧ�ܣ�����0������ɹ�������������
GLuint CFractalDlg::load_texture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint last_texture_ID, texture_ID = 0;	// ���ļ������ʧ�ܣ�����
	FILE* pFile = fopen(file_name, "rb");
	if( pFile == 0 )
	{
		return 0;
	}
	// ��ȡ�ļ���ͼ��Ŀ�Ⱥ͸߶�
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);	// ����ÿ��������ռ�ֽ����������ݴ����ݼ����������ֽ���
	{
		GLint line_bytes = width * 3;
		while( line_bytes % 4 != 0 )
			++line_bytes;
		total_bytes = line_bytes * height;
	}	// �����������ֽ��������ڴ�
	pixels = (GLubyte*)malloc(total_bytes);
	if( pixels == 0 )
	{
		fclose(pFile);
		return 0;
	}	// ��ȡ��������
	if( fread(pixels, total_bytes, 1, pFile) <= 0 )
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// �ھɰ汾��OpenGL��
	// ���ͼ��Ŀ�Ⱥ͸߶Ȳ��ǵ������η�������Ҫ��������
	// ���ﲢû�м��OpenGL�汾�����ڶ԰汾�����ԵĿ��ǣ����ɰ汾����
	// ���⣬�����Ǿɰ汾�����°汾��
	// ��ͼ��Ŀ�Ⱥ͸߶ȳ�����ǰOpenGLʵ����֧�ֵ����ֵʱ��ҲҪ��������
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if( !power_of_two(width)|| !power_of_two(height)|| width > max|| height > max )
		{
			const GLint new_width = 256;
			const GLint new_height = 256; // �涨���ź��µĴ�СΪ�߳���������
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;	// ����ÿ����Ҫ���ֽ��������ֽ���
			new_line_bytes = new_width * 3;
			while( new_line_bytes % 4 != 0 )
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;	// �����ڴ�
			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if( new_pixels == 0 )
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}

			// ������������
			gluScaleImage(GL_RGB, width, height, GL_UNSIGNED_BYTE, pixels, new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);	 // �ͷ�ԭ�����������ݣ���pixelsָ���µ��������ݣ�����������width��height
			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}	// ����һ���µ�������
	glGenTextures(1, &texture_ID);
	if( texture_ID == 0 )
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}	// ���µ���������������������������ڰ�ǰ���Ȼ��ԭ���󶨵������ţ��Ա��������лָ�
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (int *)&last_texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, last_texture_ID);	// ֮ǰΪpixels������ڴ����ʹ��glTexImage2D�Ժ��ͷ�
	// ��Ϊ��ʱ���������Ѿ���OpenGL���б�����һ�ݣ����ܱ����浽ר�ŵ�ͼ��Ӳ���У�
	free(pixels);
	return texture_ID;
} 


//�����ǿؼ��Ŀ��ƺ���
void CFractalDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	 if(pNMUpDown->iDelta == -1)  // �����ֵΪ-1 , ˵�������Spin�����ϼ�ͷ
    {
		if(frac < 8)
		{
			frac++;
			m_slider3.SetPos(frac);
		}
    }
    else if(pNMUpDown->iDelta == 1)  // �����ֵΪ1, ˵�������Spin�����¼�ͷ
    {
		if(frac > 0)
		{
			frac--;
			m_slider3.SetPos(frac);
		}
    }

    CString str;

    str.Format(_T("  %d "), frac);

    GetDlgItem(IDC_EDIT1)->SetWindowText(str);
	*pResult = 0;
}


void CFractalDlg::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	 if(pNMUpDown->iDelta == -1)  // �����ֵΪ-1 , ˵�������Spin�����ϼ�ͷ
    {
		if(hmax < 100)
		{
			hmax ++;
			m_slider1.SetPos(hmax);
		}
    }
    else if(pNMUpDown->iDelta == 1)  // �����ֵΪ1, ˵�������Spin�����¼�ͷ
    {
		if(hmax > 50)
		{
			hmax --;
			m_slider1.SetPos(hmax);
		}
    }

    CString str;

    str.Format(_T("  %d "), hmax);

    GetDlgItem(IDC_MAXH)->SetWindowText(str);
	*pResult = 0;
}


void CFractalDlg::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	 if(pNMUpDown->iDelta == -1)  // �����ֵΪ-1 , ˵�������Spin�����ϼ�ͷ
    {
		if(hmin < 0)
		{
			hmin ++;
			m_slider2.SetPos(hmin + 60);
		}
    }
    else if(pNMUpDown->iDelta == 1)  // �����ֵΪ1, ˵�������Spin�����¼�ͷ
    {
		if(hmin > -60)
		{
			hmin --;
			m_slider2.SetPos(hmin + 60);
		}
    }

    CString str;

    str.Format(_T("  %d "), hmin);

    GetDlgItem(IDC_MINH)->SetWindowText(str);
	*pResult = 0;
}


void CFractalDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	hmax = m_slider1.GetPos();
	CString str;
    str.Format(_T("  %d "), hmax);
	GetDlgItem(IDC_MAXH)->SetWindowText(str);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CFractalDlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	hmin = m_slider2.GetPos()-60;
	CString str;
    str.Format(_T("  %d "), hmin);
	GetDlgItem(IDC_MINH)->SetWindowText(str);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CFractalDlg::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	frac = m_slider3.GetPos();
	CString str;
    str.Format(_T("  %d "), frac);
	GetDlgItem(IDC_EDIT1)->SetWindowText(str);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CFractalDlg::OnBnClickedCheck2()//���û�ͼģʽ��������/�߶�
{
	if(isLines)
	{
		isLines = FALSE;
	}
	else
	{
		isLines = TRUE;
	}
}

//ɫ�ʿ���
void CFractalDlg::OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	R = m_R.GetPos()/255.0;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CFractalDlg::OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	G = m_G.GetPos()/255.0;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CFractalDlg::OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	B = m_B.GetPos()/255.0;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
