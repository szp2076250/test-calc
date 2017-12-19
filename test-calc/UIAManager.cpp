#include "StdAfx.h"
#include "UIAManager.h"

void log_info(std::string info)
{
#ifdef _DEBUG 
	std::cout<<info<<std::endl;
#endif
}

UIAManager * UIAManager::pInstance = NULL;

UIAManager * UIAManager::getInstance()
{
	if(NULL == pInstance)
	{
		pInstance = new UIAManager();
	}
	return pInstance;
}

bool UIAManager::init()
{
	m_pIUAutomation = NULL;
	m_pRoot = NULL;
	CoInitialize(NULL);
	HRESULT hr = CoCreateInstance(CLSID_CUIAutomation, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pIUAutomation));
	if (FAILED(hr))
	{
		CoUninitialize();
		return false;
	}

	hr = m_pIUAutomation->GetRootElement(&m_pRoot);
	if (FAILED(hr))
	{
		log_info(std::string("获得Root元素失败"));
		CoUninitialize();
		return false;
	}

	return true;
}

UIAE * UIAManager::GetElementByAIDEx(std::string strAid)
{
	UIAC * pCondition = NULL;
	VARIANT vt;
	vt.vt = VT_BSTR;
	vt.bstrVal = _com_util::ConvertStringToBSTR(strAid.c_str()); 
	CALL_RESULT(m_pIUAutomation->CreatePropertyCondition(UIA_AutomationIdPropertyId, vt, &pCondition));
	UIAE * pFound = NULL;
	CALL_RESULT(GetRoot()->FindFirst(TreeScope_Subtree, pCondition, &pFound));
	if (pFound == NULL)
	{
		log_info("Found null");
		return NULL;
	}
	return pFound;

}

UIAIP * UIAManager::GetElementByAID(std::string strAid)
{
	UIAC * pCondition = NULL;
	VARIANT vt;
	vt.vt = VT_BSTR;
	vt.bstrVal = _com_util::ConvertStringToBSTR(strAid.c_str());
	CALL_RESULT(m_pIUAutomation->CreatePropertyCondition(UIA_AutomationIdPropertyId, vt, &pCondition));
	return GetElementByCondition(pCondition);
}





UIAIP * UIAManager::GetElementByCondition(UIAC * uiac)
{
	
	UIAE * pFound=NULL;
	CALL_RESULT(GetRoot()->FindFirst(TreeScope_Subtree, uiac, &pFound));
	if (pFound == NULL)
	{
		log_info("Found null");
		return NULL;
	}
	UIAIP *  pPattern = ConvertoPattern(pFound);
	return pPattern;
}

UIAIP * UIAManager::ConvertoPattern(UIAE * pFound)
{
	UIAIP * pPattern = NULL;
	CALL_RESULT(pFound->GetCurrentPatternAs(UIA_InvokePatternId, IID_PPV_ARGS(&pPattern)));
	return pPattern;
}

UIAE * UIAManager::GetElementByHwnd(HWND hwnd)
{
	UIAE * pe=NULL;
	UIAC * pCondition = NULL;
	m_pIUAutomation->ElementFromHandle(hwnd,&pe);
	return pe;
}

bool UIAManager::Invoke(UIAIP * pattern)
{
	if (pattern == NULL) return false;
	pattern->Invoke();
	return true;
}

bool UIAManager::SetForce(UIAE * pae)
{
	UIA_HWND hWnd;
	pae->get_CurrentNativeWindowHandle(&hWnd);
	if (hWnd)
	{
		::BringWindowToTop((HWND)hWnd);
		::SetWindowPos((HWND)hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		::SetFocus((HWND)hWnd);
		return true;
	}
	return false;
}

bool UIAManager::ClickElement(UIAE * pae)
{
	UIA_HWND hd;
	pae->get_CurrentNativeWindowHandle(&hd);
	POINT point;
	BOOL  bClickable;
	SetForce(pae);
	if (!bClickable)
	{
		log_info("不是可以点击的！");
	}
	pae->GetClickablePoint(&point, &bClickable);
	if (hd)
	{
		LPARAM lp = MAKELPARAM(point.x,point.y);
	
		PostMessage((HWND)hd, WM_LBUTTONDOWN, VK_LBUTTON, lp);
		Sleep(50);
		PostMessage((HWND)hd, WM_LBUTTONUP, 0, lp);
		return true;
	}
	log_info("click error");
	return false;
}

UIAManager::UIAManager(void)
{
	if (!init()) exit(0);
}


UIAManager::~UIAManager(void)
{
	
	CoUninitialize();
}
