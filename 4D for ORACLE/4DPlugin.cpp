/* --------------------------------------------------------------------------------
 #
 #	4DPlugin.c
 #	source generated by 4D Plugin Wizard
 #	Project : 4D for ORACLE
 #	author : miyako
 #	2012/12/31
 #
 # --------------------------------------------------------------------------------*/


#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "oracle_login.h"
#include "oracle_context_definition.h"
#include "oracle_using_context.h"
#include "oracle_low_level.h"
#include "oracle_low_level_extra.h"
#include "oracle_preferences.h"
#include "oracle_control.h"
#include "oracle_high_level.h"
#include "oracle_environment.h"

#define THIS_BUNDLE_ID @"com.4D.4DPlugin.miyako.4DforORACLE"
#define THIS_BUNDLE_NAME L"4D for ORACLE.4DX"

std::map<uint32_t, sessionInfo*> _sessions;

#pragma mark -
#pragma mark internal memory management
#pragma mark -

sessionInfo *_sessionCreate(unsigned int *index, 
							OCIEnv	*envhp, 
							OCIServer *srvhp, 
							OCISvcCtx *svchp, 
							OCISession *authp, 
							OCIError	*errhp,
							CUTF16String		&user,
							CUTF16String		&server)
{
	sessionInfo *obj = NULL;
	
	unsigned int i = 10000;
	
	while ((_sessions.find(i) != _sessions.end()) && (i < 100000))
	{
		i++;
	}
	
	if(i != 100000)
	{
		obj = new(sessionInfo);
		
		obj->authp = authp;
		obj->envhp = envhp;
		obj->srvhp = srvhp;
		obj->svchp = svchp;
		obj->errhp = errhp;
		obj->user = user;
		obj->server = server;
		
		_sessions.insert(std::map<uint32_t, sessionInfo*>::value_type(i, obj));
	
		*index = i; 
	
	}else{
		*index = 0;
		_errorInfoSet(0, 0, 0, 0, true, PA_GetCurrentProcessNumber(), PLUGIN_ERROR_NOT_ENOUGH_MEMORY);
	}	
		
	return obj;
}

void _sessionDelete(unsigned int i)
{	
	std::map<uint32_t, sessionInfo*>::iterator pos = _sessions.find(i);
	
	if(pos != _sessions.end()) 
	{
		
		sessionInfo *obj = pos->second;
		
		OCIHandleFree (obj->errhp, OCI_HTYPE_ERROR);
		OCIHandleFree (obj->srvhp, OCI_HTYPE_SERVER);		
		OCIHandleFree (obj->svchp, OCI_HTYPE_SVCCTX);
		OCIHandleFree (obj->authp, OCI_HTYPE_SESSION);		
		OCIHandleFree (obj->envhp, OCI_HTYPE_ENV);
		
		obj->errhp = 0;
		obj->srvhp = 0;
		obj->svchp = 0;		
		obj->authp = 0;	
		obj->envhp = 0;	
		
		delete obj;
		
		_sessions.erase(pos);
	}
}

sessionInfo *_sessionGet(unsigned int i)
{	
	sessionInfo *obj = NULL;
	
	std::map<uint32_t, sessionInfo*>::iterator pos = _sessions.find(i);
	
	if(pos != _sessions.end()) 
	{
		obj = pos->second;
	}else{
		_errorInfoSet(0, 0, i, 0, true, PA_GetCurrentProcessNumber(), PLUGIN_ERROR_LOGIN_INVALID);
	}
	
	return obj;
}

#pragma mark -

void OD_SetOracleLibraryDirectory()
{
#if VERSIONMAC	
	NSBundle *thisBundle = [NSBundle bundleWithIdentifier:THIS_BUNDLE_ID];
	if(thisBundle)
	{
		NSString *executablePath = [thisBundle executablePath];	
		
		if(executablePath) 
		{			
			std::string DYLD_LIBRARY_PATH((const char *)[[executablePath stringByDeletingLastPathComponent]UTF8String]);
			chdir(DYLD_LIBRARY_PATH.c_str());			
		}
	}
#else
	/*
	wchar_t	libPath[ _MAX_PATH ] = {0};
	wchar_t	fDrive[ _MAX_DRIVE ], fDir[ _MAX_DIR ], fName[ _MAX_FNAME ], fExt[ _MAX_EXT ];
	
	HMODULE libmecabModule = GetModuleHandleW(THIS_BUNDLE_NAME);
	GetModuleFileNameW(libmecabModule, libPath, _MAX_PATH);
	
	_wsplitpath_s( libPath, fDrive, fDir, fName, fExt );

	std::wstring DYLD_LIBRARY_PATH = fDrive;
	DYLD_LIBRARY_PATH += fDir;
	
	std::wstring oldValue = std::wstring(_wgetenv(L"PATH"));
	oldValue += L";";
	DYLD_LIBRARY_PATH = oldValue + DYLD_LIBRARY_PATH;

	_wputenv_s(L"PATH", DYLD_LIBRARY_PATH.c_str());
	*/
#endif
}

void PluginMain(int32_t selector, PA_PluginParameters params)
{
	try
	{
		int32_t pProcNum = selector;
		sLONG_PTR *pResult = (sLONG_PTR *)params->fResult;
		PackagePtr pParams = (PackagePtr)params->fParameters;
		
		CommandDispatcher(pProcNum, pResult, pParams); 
	}
	catch(...)
	{
		
	}
}

#pragma mark -

void CommandDispatcher (int32_t pProcNum, sLONG_PTR *pResult, PackagePtr pParams)
{
	switch(pProcNum)
	{
			
			// --- OD: Login
			
		case 1 :
			OD_Login(pResult, pParams);
			break;
			
		case 2 :
			OD_LOGOUT(pResult, pParams);
			break;
			
		case 3 :
			OD_Login_state(pResult, pParams);
			break;
			
			// --- OD: Low Level
			
		case 4 :
			OD_Create_cursor(pResult, pParams);
			break;
			
		case 5 :
			OD_DROP_CURSOR(pResult, pParams);
			break;
			
		case 6 :
			OD_Cursor_state(pResult, pParams);
			break;
			
		case 7 :
			OD_Set_SQL_in_cursor(pResult, pParams);
			break;
			
		case 8 :
			OD_BIND_TOWARDS_SQL(pResult, pParams);
			break;
			
		case 9 :
			OD_BIND_TOWARDS_4D(pResult, pParams);
			break;
			
		case 10 :
			OD_EXECUTE_CURSOR(pResult, pParams);
			break;
			
		case 11 :
			OD_Load_rows_cursor(pResult, pParams);
			break;
			
		case 12 :
			OD_Number_rows_processed(pResult, pParams);
			break;
			
		case 13 :
			OD_GET_COLUMN_ATTRIBUTES(pResult, pParams);
			break;
			
		case 14 :
			OD_Get_column_title(pResult, pParams);
			break;
			
		case 15 :
			OD_Number_of_columns(pResult, pParams);
			break;
			
			// --- OD: Preferences
			
		case 16 :
			OD_SET_NULL_VALUE(pResult, pParams);
			break;
			
			// --- OD: Control
			
		case 17 :
			OD_CANCEL_LOADING(pResult, pParams);
			break;
			
		case 18 :
			OD_Last_error(pResult, pParams);
			break;
			
			// --- OD: High Level
			
		case 19 :
			OD_COMMIT(pResult, pParams);
			break;
			
		case 20 :
			OD_ROLLBACK(pResult, pParams);
			break;
			
			// --- OD: Environment
			
		case 21 :
			OD_SET_ENVIRONMENT_VARIABLE(pResult, pParams);
			break;
			
		case 22 :
			OD_Get_environment_variable(pResult, pParams);
			break;
			
			// --- OD: Debug
			
		case 23 :
			OD_INTROSPECT_CURSOR(pResult, pParams);
			break;
			
		case 24 :
			OD_INTROSPECT_NULL_VALUE_MAP(pResult, pParams);
			break;
			
	}
}