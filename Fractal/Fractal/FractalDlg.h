
// FractalDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CFractalDlg �Ի���
class CFractalDlg : public CDialogEx
{
// ����
public:
	CFractalDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FRACTAL_DIALOG };
	BOOL SetWindowPixelFormat(HDC hDC);                    // �趨���ظ�ʽ
    BOOL CreateViewGLContext(HDC hDC);                    // view GL Context
    void RenderScene();                                                    // ���Ƴ���
    HDC hrenderDC;                                                            // DC
    HGLRC hrenderRC;                                                        // RC
    int PixelFormat;                                                        // ���ظ�ʽ

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	double PI;
    double tFovy;                   // ��gluPerspective��ʹ��(��OnMouseWheel��ʹ��)
    CPoint prePt, nowPt;                                        // ��OnMouseMove��ʹ��
    double tEyeX, tEyeY, tEyeZ;                                    // ��gluLookAt��ʹ�õ�3������
    double tCenterX, tCenterY, tCenterZ;
    double tUpX, tUpY, tUpZ;
    double tVerticalAng, tHorizonAng, tRadius, tAngInc; // ˮƽ���򡢴�ֱ����Ľǡ��뾶

	int hmin; // Max and min size added to midpoint
	int hmax;
	int **data;
	int minscale;
	int maxscale;
	int length;
	GLuint makeObject();//���ƺ���
	int getTriangleNum();
	GLfloat * getTriangles();
	void init(int s);
	int midpoint(int i, int start, int topx, int topy);//���������������������ĺ���
	GLfloat scale(int data);
	/* �������ı��*/
	GLuint texGround;
	GLuint load_texture(const char* file_name);
	int power_of_two(int n);
	void grab(void);

	//����
	bool isSmooth;
	bool isLines;
	int frac;
	GLfloat R,G,B;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);                // ����OnTimer����
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);   // ����OnMouseWheel����
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);        // ����OnMouseMove����
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
