/* Bouldev 2022 */
#include <libbouldev.h>
#include <string>
#ifdef _WIN32
#include "stdafx.h"
#include <iostream>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")
#endif

/* WINDOWS */

#ifdef _WIN32
//credits: https://github.com/LordNoteworthy/al-khaser/blob/master/al-khaser/Shared/Utils.cpp#L740
static
bool InitWMI(IWbemServices **pSvc, IWbemLocator **pLoc, const TCHAR* szNetworkResource)
{
	HRESULT hres;
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	hres = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	hres = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(pLoc));
	BSTR strNetworkResource = SysAllocString(szNetworkResource);
	if (strNetworkResource) 
	{
		hres = (*pLoc)->ConnectServer(strNetworkResource, NULL, NULL, NULL, WBEM_FLAG_CONNECT_USE_MAX_WAIT, 0, 0, pSvc);
		SysFreeString(strNetworkResource);
	}
	hres = CoSetProxyBlanket(*pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	return 1;
}

//credits: https://github.com/LordNoteworthy/al-khaser/blob/master/al-khaser/Shared/Utils.cpp#L795
static
bool ExecWMIQuery(IWbemServices **pSvc, IWbemLocator **pLoc, IEnumWbemClassObject **pEnumerator, const TCHAR* szQuery)
{
	BSTR strQueryLanguage = SysAllocString(OLESTR("WQL"));
	BSTR strQuery = SysAllocString(szQuery);
	BOOL bQueryResult = TRUE;
	if (strQueryLanguage && strQuery) HRESULT hres = (*pSvc)->ExecQuery(strQueryLanguage, strQuery, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, pEnumerator);
	if (strQueryLanguage) SysFreeString(strQueryLanguage);
	if (strQuery) SysFreeString(strQuery);
	return bQueryResult;
}

//credits: https://github.com/LordNoteworthy/al-khaser/blob/master/al-khaser/AntiVM/Generic.cpp#L1525
static
int wmi_query_count(const _TCHAR* query)
{
    IWbemServices *pSvc = NULL;
    IWbemLocator *pLoc = NULL;
    IEnumWbemClassObject* pEnumerator = NULL;
    BOOL bStatus = FALSE;
    HRESULT hRes;
    int count = 0;
    bStatus = InitWMI(&pSvc, &pLoc, _T("ROOT\\CIMV2"));
    if (bStatus)
    {
        bStatus = ExecWMIQuery(&pSvc, &pLoc, &pEnumerator, query);
        if (bStatus)
        {
            IWbemClassObject *pclsObj = NULL;
            ULONG uReturn = 0;
            while (pEnumerator)
            {
                hRes = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
                if (0 == uReturn) break;
                count++;
                pclsObj->Release();
            }
	}
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();        
    }
    else return -1;
    return count;
}
#endif

#ifdef linux
static
bool under_qemu()
{
	char *cmdline = NULL;
	char *out = NULL;
	int ret = 0;
	strcpy(cmdline, "cat /proc/cpuinfo | grep QEMU &> /dev/null");
	boul_cmd buf = { cmdline, ret, out };
	run_cmd(buf);

	if (ret == 0) return true;
	return false;
}
#endif

static
bool under_sandbox()
{
	if (path_exists("/.dockerenv")) return true;
#ifdef linux
	char *cmdline = NULL;
	char *out = NULL;
	int ret = 0;
	strcpy(cmdline, "grep -sq 'docker|lxc' /proc/1/cgroup &> /dev/null");
	boul_cmd buf = { cmdline, ret, out };
	run_cmd(buf);

	if (ret == 0) return true;
#endif
	return false;
}

bool os_is_vm()
{
#ifdef _WIN32
	return wmi_query_count(_T("SELECT * FROM Win32_PortConnector")) == 0 ? true : false;
#elif defined(linux)
	return (under_qemu() || under_sandbox());
#elif defined(__APPLE__)
    // Not doing detections on Darwin systems for now
    return false;
#endif
}
