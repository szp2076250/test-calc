#include "stdafx.h"
#include <iostream>
#include "UIAManager.h"



int _tmain(int argc, _TCHAR* argv[])
{
	system("START calc");
	Sleep(500);
	UIA->Invoke(UIA->GetElementByAID("135"));
	Sleep(500);
	UIA->Invoke(UIA->GetElementByAID("132"));
	Sleep(500);
	UIA->Invoke(UIA->GetElementByAID("130"));
	Sleep(500);
	UIA->Invoke(UIA->GetElementByAID("131"));
	Sleep(500);
	UIA->Invoke(UIA->GetElementByAID("133"));
	Sleep(500);
	UIA->Invoke(UIA->GetElementByAID("131"));
	Sleep(500);
	UIA->Invoke(UIA->GetElementByAID("134"));
	std::cout << "Love You !"<<std::endl;
	system("pause");
	return 0;
}

