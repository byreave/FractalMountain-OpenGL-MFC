
// FractalDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CFractalDlg 对话框



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


// CFractalDlg 消息处理程序

BOOL CFractalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CWnd* wnd = GetDlgItem(IDC_RENDER);                                // 初始化MFC中的OpenGL设置
    hrenderDC = ::GetDC(wnd->m_hWnd);
    if(SetWindowPixelFormat(hrenderDC) == FALSE) {                // 设置hDC的像素格式
        return 0;
    }
    if(CreateViewGLContext(hrenderDC) == FALSE) {                    // 由hDC转换得到hRC
        return 0;
    }
    // openGL的初始化设置
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glViewport(0, 0, 600, 550);
    glMatrixMode(GL_PROJECTION);
    //gluPerspective(45, 1, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // 设置计时器,10ms刷新一次
    //SetTimer(1, 10, 0);
	//MessageBox("初始化变量。", "attention", MB_OK);
	PI = 3.1415926535897;
    tAngInc = PI / 90;                      // 每次触发鼠标事件变换的角度值
    tFovy = 45.0;
    prePt.SetPoint(-1, -1);                 // 初始化prePt & nowPt(在OnMouseMove中使用)
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
	//glEnable(GL_TEXTURE_2D);//纹理映射开关！！！注意！！！
	texGround = load_texture("ground.bmp");
	srand(time(NULL));

	hmin = -30; // 扰动的最大最小值
	hmax = 75;
	minscale = -100;
	maxscale = 100;
	
	isSmooth = TRUE;
	isLines = FALSE;
	frac = 4;
	CString str;
    str.Format(_T("  %d "), frac);//初始化控件中的数值
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
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFractalDlg::OnPaint()
{
	//RenderScene();
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示
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
//绘制控制函数，设置了各项绘制参数后在内部调用了真正的绘制函数
void CFractalDlg::RenderScene() {
	glMatrixMode(GL_PROJECTION);                            // 设置modelview 和 projection
    glLoadIdentity();                                       // 此处尤其不能少glLoadIdentity()
    gluPerspective(tFovy, 1, 0.1, 1000.0);                  // 注意zNear,zFar的取值
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(tEyeX, tEyeY, tEyeZ, tCenterX, tCenterY, tCenterZ, tUpX, tUpY, tUpZ);

	{
    glClear(GL_COLOR_BUFFER_BIT);
	GLfloat lmodel_ambient[]={R,G,B,1.0f}; //定义环境光
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient); //设置环境光
	GLfloat light0_ambient[]={R,G,B,1.0}; //定义光源环境光
	GLfloat light0_diffuse[]={1.0,0.5,0.0,1.0}; //定义光源散射光
	GLfloat light0_specular[]={1.0,0.5,0.0,1.0}; //定义光源发射光
	GLfloat light0_position[]={160.0,160.0,50.0,1.0}; //定义光源位置
	glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient); //设置光源环境光
	glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse); //设置光源散射光
	glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);//设置光源反射光
	glLightfv(GL_LIGHT0,GL_POSITION,light0_position); //设置光源位置
	glEnable(GL_LIGHT0); //启用光源
	glEnable(GL_LIGHTING); //启用光照效果
	}
	//{
	//GLfloat brass_ambient[]={0.5f,0.5f,0.0f,1.0f}; //定义材质环境光
	//GLfloat brass_diffuse[]={0.5f,0.5f,0.0f,1.0f}; //定义材质散射光
	//GLfloat brass_specular[]={0.5f,0.5f,0.0f,1.0};//定义材质反射光
	//GLfloat brass_sinines[]={20.0f}; //定义材质镜面发射强度
	//glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,brass_ambient); //设置材质环境光
	//glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,brass_diffuse); //设置材质散射光
	//glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,brass_specular); //设置材质发射光
	//glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,brass_sinines); //设置材质镜面反射强度
	//
	//}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
 
	makeObject();//根据坐标进行绘制
    SwapBuffers(hrenderDC);//交换缓存
}


//设置每个周期要做的事情（刷新/绘图）
void CFractalDlg::OnTimer(UINT nIDEvent) {
    RenderScene();
    CDialog::OnTimer(nIDEvent);
}


//通过滚轮操作缩放视角
afx_msg BOOL CFractalDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	//MessageBox("mouse move function triggered!", "attentino", MB_OK);
    int tWheelCount = zDelta / 120;
    if(tWheelCount > 0) {
        tFovy += 1.0;
    } else if(tWheelCount < 0) {
        tFovy -= 1.0;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();                                            // 此处尤其不可少glLoadIdentity()
    gluPerspective(tFovy, 1, 0.1, 1000.0);                        // 注意zNear,zFar的取值
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    return TRUE;
}


// 在界面显示屏幕上通过鼠标操作变换绘制视角
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


//绘制函数
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
	 if(vert != NULL)//防止内存泄漏
		 delete []vert;

     return list;
 }


 //点击“生成”按钮的操作
 void CFractalDlg::OnBnClickedButton1()
 {
	 init(frac);
	 midpoint(frac,frac,0,0);
	 SetTimer(1, 30, 0);
	 CString str;
	 str.Format(_T("  %d "), getTriangleNum()/3);
	 GetDlgItem(IDC_TRINUM)->SetWindowText(str);
 }


 //核心函数，通过迭代以计算出每个点的高度值并保存在数组以便绘制
int CFractalDlg::midpoint(int i, int start, int topx, int topy) {
	 //MessageBox("进入midpoint函数!", "attention", MB_OK);
  if (i < 1) {//迭代终止条件
    return 0;
  }

  int len = (int)pow(2.0f, i);//计算本次迭代的长度

  int x1 = topx + len / 2;//顶部中点
  int y1 = topy;

  int x2 = topx + len;//右部中点
  int y2 = topy + len / 2;

  int x3 = x1;//底部中点
  int y3 = y1 + len;

  int x4 = x2 - len;//做部中点
  int y4 = y2;

  int xmid = x1;//中心中点
  int ymid = y2;

  //计算各个点的高度（z）值

		
  // 顶部
  data[x1][y1] =
      data[topx][topy] + (data[topx + len][topy] - data[topx][topy]) / 2;

  // 右部
  data[x2][y2] = data[topx + len][topy] +
                 (data[topx + len][topy + len] - data[topx + len][topy]) / 2;
  // 底部
  data[x3][y3] = data[topx][topy + len] +
                 (data[topx + len][topy + len] - data[topx][topy + len]) / 2;
  // 左部
  data[x4][y4] =
      data[topx][topy] + (data[topx][topy + len] - data[topx][topy]) / 2;

  // 中心
  data[xmid][ymid] = data[x1][y1] + (data[x3][y3] - data[x1][y1]) / 2;

  // 随着迭代次数增加，减少中点扰动的范围
  float min =
      hmin * (((float)start - ((float)start - (float)i)) / (float)start);
  float max =
      hmax * (((float)start - ((float)start - (float)i)) / (float)start);
  // 增加随机扰动值
  data[xmid][ymid] +=
      (int)(min + ((float)rand() / (float)RAND_MAX) * (max - min));
  //str.Format("data[xmid][ymid]=data[%d][%d]=%d,", xmid,ymid,data[xmid][ymid]);
  //MessageBox(str); 

  i--;//迭代终止条件自减

  // 左上迭代
  midpoint(i, start, topx, topy);

  // 右上迭代
  midpoint(i, start, x1, y1);

  // 左下迭代
  midpoint(i, start, x4, y4);

  // 右下迭代
  midpoint(i, start, xmid, ymid);
  return 0;
}


//初始化，开辟用于保存所有坐标的二维数组空间
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


//缩放函数，使得无论初始三角形的边长为多少都可完整显示
GLfloat CFractalDlg::scale(int data) {

  return (GLfloat)(minscale + (float)data/(float)(length-1) * (maxscale - minscale));

}


//取得绘制三角形的坐标数组
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


//获取三角形个数，以确定绘图时需要绘制多少次
int CFractalDlg::getTriangleNum()
{
    return (length-1) * (length-1) *6;
}


//控制绘图方式：平滑/非平滑阴影
void CFractalDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
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
	GLint	PixelDataLength;	// 计算像素数据的实际长度
	i = WindowWidth * 3;	// 得到每一行的像素数据长度
	while( i%4 != 0 )	// 补充数据，直到i是的倍数
		++i;
	PixelDataLength = i * WindowHeight;	// 分配内存和打开文件
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
		exit(0);	// 读取像素
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, WindowWidth, WindowHeight,GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);	// 把dummy.bmp的文件头复制为新文件的文件头
	fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);
	fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
	fseek(pWritingFile, 0x0012, SEEK_SET);
	i = WindowWidth;
	j = WindowHeight;
	fwrite(&i, sizeof(i), 1, pWritingFile);
	fwrite(&j, sizeof(j), 1, pWritingFile);	// 写入像素数据
	fseek(pWritingFile, 0, SEEK_END);
	fwrite(pPixelData, PixelDataLength, 1, pWritingFile);	// 释放内存和关闭文件
	fclose(pDummyFile);
	fclose(pWritingFile);
	free(pPixelData);
} 


/* 函数power_of_two* 检查一个整数是否为2的整数次方，如果是，返回1，否则返回0
* 实际上只要查看其二进制位中有多少个，如果正好有1个，返回1，否则返回0
* 在“查看其二进制位中有多少个”时使用了一个小技巧
* 使用n &= (n-1)可以使得n中的减少一个（具体原理大家可以自己思考）*/
int CFractalDlg::power_of_two(int n)
{
	if( n <= 0 )
		return 0;
	return (n & (n-1)) == 0;
}	


//函数load_texture* 读取一个BMP文件作为纹理* 如果失败，返回0，如果成功，返回纹理编号
GLuint CFractalDlg::load_texture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint last_texture_ID, texture_ID = 0;	// 打开文件，如果失败，返回
	FILE* pFile = fopen(file_name, "rb");
	if( pFile == 0 )
	{
		return 0;
	}
	// 读取文件中图象的宽度和高度
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);	// 计算每行像素所占字节数，并根据此数据计算总像素字节数
	{
		GLint line_bytes = width * 3;
		while( line_bytes % 4 != 0 )
			++line_bytes;
		total_bytes = line_bytes * height;
	}	// 根据总像素字节数分配内存
	pixels = (GLubyte*)malloc(total_bytes);
	if( pixels == 0 )
	{
		fclose(pFile);
		return 0;
	}	// 读取像素数据
	if( fread(pixels, total_bytes, 1, pFile) <= 0 )
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// 在旧版本的OpenGL中
	// 如果图象的宽度和高度不是的整数次方，则需要进行缩放
	// 这里并没有检查OpenGL版本，出于对版本兼容性的考虑，按旧版本处理
	// 另外，无论是旧版本还是新版本，
	// 当图象的宽度和高度超过当前OpenGL实现所支持的最大值时，也要进行缩放
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if( !power_of_two(width)|| !power_of_two(height)|| width > max|| height > max )
		{
			const GLint new_width = 256;
			const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;	// 计算每行需要的字节数和总字节数
			new_line_bytes = new_width * 3;
			while( new_line_bytes % 4 != 0 )
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;	// 分配内存
			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if( new_pixels == 0 )
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}

			// 进行像素缩放
			gluScaleImage(GL_RGB, width, height, GL_UNSIGNED_BYTE, pixels, new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);	 // 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height
			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}	// 分配一个新的纹理编号
	glGenTextures(1, &texture_ID);
	if( texture_ID == 0 )
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}	// 绑定新的纹理，载入纹理并设置纹理参数，在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (int *)&last_texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, last_texture_ID);	// 之前为pixels分配的内存可在使用glTexImage2D以后释放
	// 因为此时像素数据已经被OpenGL另行保存了一份（可能被保存到专门的图形硬件中）
	free(pixels);
	return texture_ID;
} 


//以下是控件的控制函数
void CFractalDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	 if(pNMUpDown->iDelta == -1)  // 如果此值为-1 , 说明点击了Spin的往上箭头
    {
		if(frac < 8)
		{
			frac++;
			m_slider3.SetPos(frac);
		}
    }
    else if(pNMUpDown->iDelta == 1)  // 如果此值为1, 说明点击了Spin的往下箭头
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
	// TODO: 在此添加控件通知处理程序代码
	 if(pNMUpDown->iDelta == -1)  // 如果此值为-1 , 说明点击了Spin的往上箭头
    {
		if(hmax < 100)
		{
			hmax ++;
			m_slider1.SetPos(hmax);
		}
    }
    else if(pNMUpDown->iDelta == 1)  // 如果此值为1, 说明点击了Spin的往下箭头
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
	// TODO: 在此添加控件通知处理程序代码
	 if(pNMUpDown->iDelta == -1)  // 如果此值为-1 , 说明点击了Spin的往上箭头
    {
		if(hmin < 0)
		{
			hmin ++;
			m_slider2.SetPos(hmin + 60);
		}
    }
    else if(pNMUpDown->iDelta == 1)  // 如果此值为1, 说明点击了Spin的往下箭头
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
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CFractalDlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	hmin = m_slider2.GetPos()-60;
	CString str;
    str.Format(_T("  %d "), hmin);
	GetDlgItem(IDC_MINH)->SetWindowText(str);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CFractalDlg::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	frac = m_slider3.GetPos();
	CString str;
    str.Format(_T("  %d "), frac);
	GetDlgItem(IDC_EDIT1)->SetWindowText(str);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CFractalDlg::OnBnClickedCheck2()//设置绘图模式：三角形/线段
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

//色彩控制
void CFractalDlg::OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	R = m_R.GetPos()/255.0;
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CFractalDlg::OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	G = m_G.GetPos()/255.0;
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CFractalDlg::OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	B = m_B.GetPos()/255.0;
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
