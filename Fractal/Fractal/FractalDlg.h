
// FractalDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CFractalDlg 对话框
class CFractalDlg : public CDialogEx
{
// 构造
public:
	CFractalDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FRACTAL_DIALOG };
	BOOL SetWindowPixelFormat(HDC hDC);                    // 设定像素格式
    BOOL CreateViewGLContext(HDC hDC);                    // view GL Context
    void RenderScene();                                                    // 绘制场景
    HDC hrenderDC;                                                            // DC
    HGLRC hrenderRC;                                                        // RC
    int PixelFormat;                                                        // 像素格式

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	double PI;
    double tFovy;                   // 在gluPerspective中使用(在OnMouseWheel中使用)
    CPoint prePt, nowPt;                                        // 在OnMouseMove中使用
    double tEyeX, tEyeY, tEyeZ;                                    // 在gluLookAt中使用的3个向量
    double tCenterX, tCenterY, tCenterZ;
    double tUpX, tUpY, tUpZ;
    double tVerticalAng, tHorizonAng, tRadius, tAngInc; // 水平方向、垂直方向的角、半径

	int hmin; // Max and min size added to midpoint
	int hmax;
	int **data;
	int minscale;
	int maxscale;
	int length;
	GLuint makeObject();//绘制函数
	int getTriangleNum();
	GLfloat * getTriangles();
	void init(int s);
	int midpoint(int i, int start, int topx, int topy);//迭代完成坐标数组计算填充的函数
	GLfloat scale(int data);
	/* 纹理对象的编号*/
	GLuint texGround;
	GLuint load_texture(const char* file_name);
	int power_of_two(int n);
	void grab(void);

	//控制
	bool isSmooth;
	bool isLines;
	int frac;
	GLfloat R,G,B;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);                // 重载OnTimer函数
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);   // 重载OnMouseWheel函数
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);        // 重载OnMouseMove函数
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_slider1;
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_slider2;
	CSliderCtrl m_slider3;
	afx_msg void OnBnClickedCheck2();
	CSliderCtrl m_R;
	CSliderCtrl m_G;
	CSliderCtrl m_B;
	afx_msg void OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult);
};
