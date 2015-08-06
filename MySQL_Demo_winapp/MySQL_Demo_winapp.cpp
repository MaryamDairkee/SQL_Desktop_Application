// MySQL_Demo_winapp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MySQL_Demo_winapp.h"
#include <my_global.h>
#include <mysql.h>


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
mysqlconnection Sqlcon;
HWND hWnd= NULL;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Conn_Details(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void AppendText( const HWND &hwnd, TCHAR *newText );


//void finish_with_error(MYSQL *con)
//{
////fprintf(stderr, "%s\n", mysql_error(con));
//mysql_close(con);
//exit(1);        
//}
int APIENTRY _tWinMain(HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPTSTR    lpCmdLine,
                    int       nCmdShow)
{
UNREFERENCED_PARAMETER(hPrevInstance);
UNREFERENCED_PARAMETER(lpCmdLine);

// TODO: Place code here.
MSG msg;
HACCEL hAccelTable;

// Initialize global strings
LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
LoadString(hInstance, IDC_MYSQL_DEMO_WINAPP, szWindowClass, MAX_LOADSTRING);
MyRegisterClass(hInstance);

// Perform application initialization:
if (!InitInstance (hInstance, nCmdShow))
{
	return FALSE;
}

hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYSQL_DEMO_WINAPP));

// Main message loop:
while (GetMessage(&msg, NULL, 0, 0))
{
	if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
WNDCLASSEX wcex;

wcex.cbSize = sizeof(WNDCLASSEX);

wcex.style			= CS_HREDRAW | CS_VREDRAW;
wcex.lpfnWndProc	= WndProc;
wcex.cbClsExtra		= 0;
wcex.cbWndExtra		= 0;
wcex.hInstance		= hInstance;
wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYSQL_DEMO_WINAPP));
wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MYSQL_DEMO_WINAPP);
wcex.lpszClassName	= szWindowClass;
wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{


hInst = hInstance; // Store instance handle in our global variable

hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

if (!hWnd)
{
    return FALSE;
}

ShowWindow(hWnd, nCmdShow);
UpdateWindow(hWnd);

return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
int wmId, wmEvent;
PAINTSTRUCT ps;
HDC hdc;

switch (message)
{
case WM_CREATE:
	/*CreateWindowEx(NULL, TEXT("BUTTON"),TEXT("New Connection"),
    WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON|WS_TABSTOP,
    50,350, 150, 25, hWnd, (HMENU)IDC_NEWCON, GetModuleHandle(NULL), NULL);*/

		

	//CreateWindowEx(NULL, TEXT(""),TEXT("Connect"),
    // WS_TABSTOP|WS_VISIBLE|WS_CHILD|WS_TABSTOP,
    // 115, 45, 100, 23, hWnd, (HMENU)IDC_CONBUTTON, GetModuleHandle(NULL), NULL);

		


case WM_COMMAND:
	wmId    = LOWORD(wParam);
	wmEvent = HIWORD(wParam);
	// Parse the menu selections:
	switch (wmId)
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;

	case IDC_NEWCON:
		{
		int ret=DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CON), hWnd, Conn_Details);
		if(ret == -1)
			MessageBox(hWnd, TEXT("Dialog failed!"), TEXT("Error"),MB_OK | MB_ICONINFORMATION);
		break;
		}
	case IDC_STOP:
		{
		HWND hExec=GetDlgItem(hWnd,IDC_EXEC);
		EnableWindow(hExec,false);

		HWND hDiscon=GetDlgItem(hWnd,IDC_DISCON);
		EnableWindow(hDiscon,false);

		Sqlcon.CloseConnection();

		if(Sqlcon.RestoreConnection()){
		MessageBox(hWnd, TEXT("Successfully Restored"), TEXT(""),MB_OK | MB_ICONINFORMATION);
		HWND hStop=GetDlgItem(hWnd,IDC_STOP);
		EnableWindow(hStop,false);
		break;
		}

		else
			MessageBox(hWnd, TEXT("Restore Unsuccessful"), TEXT("Error"),MB_OK | MB_ICONINFORMATION);
		}	
			
	case IDC_DISCON:{
			HWND hExec=GetDlgItem(hWnd,IDC_EXEC);
			HWND hStop=GetDlgItem(hWnd,IDC_STOP);
			HWND hQuery=GetDlgItem(hWnd,IDC_QUERY);
			HWND hResult=GetDlgItem(hWnd,IDC_RESULT);
			HWND hDiscon=GetDlgItem(hWnd,IDC_DISCON);

			DestroyWindow(hExec);
			DestroyWindow(hStop);
			DestroyWindow(hQuery);
			DestroyWindow(hResult);
			DestroyWindow(hDiscon);
			Sqlcon.CloseConnection();
			break;
					}
	case IDC_EXEC:
		{
			HWND hStop=GetDlgItem(hWnd,IDC_STOP);
			EnableWindow(hStop,true);
			//-------------Clears the result Window-----------------//
			SetDlgItemText(hWnd,IDC_RESULT,L"");
			MYSQL *con=Sqlcon.GetConnection();
			
//---------------------------------------Query Extraction----------------------------------------//
		HWND hQuery=GetDlgItem(hWnd,IDC_QUERY);
		int n=GetWindowTextLength(hQuery)+1;
			
		wchar_t* query= new wchar_t[n+1];
		GetDlgItemText(hWnd,IDC_QUERY,query,n);

		int len = WideCharToMultiByte(CP_UTF8,0,query,n,NULL,0,NULL, 0);
		char* sql_query=new char[len+1];
		WideCharToMultiByte(CP_UTF8,0,query,-1,sql_query,len+1,NULL,NULL);

//-----------------------------------------End of Query Extraction---------------------------------------//	
			
		if (mysql_query(con,sql_query)) 
			{
				MessageBox(hWnd,L"Error in query",L"Query error",MB_OK | MB_ICONINFORMATION);
				//exit(1);
			}
		else 
		{
			int status,count=0;
			MYSQL_RES *result;

			do{

			count++;
			result = mysql_store_result(con);
			if(result){
			
			
			wchar_t *c = new wchar_t[50];
			swprintf(c, 10, L"%d", count);
			
			char *text=new char[50];
			strcpy(text,"Result of Query ");

			int tit_len = MultiByteToWideChar (CP_UTF8,0,text,strlen(text)+1, NULL, 0);
			wchar_t *title= new wchar_t[tit_len+50];
			MultiByteToWideChar (CP_UTF8, 0,text, -1,title, tit_len);
			title[tit_len]=L'\0';
			wcscat(title,c);
			wcscat(title,L"\r\n");
			
		    //title=wcscat(title,c);
			AppendText(hWnd,title);
			


			MYSQL_ROW row;
			MYSQL_FIELD* field;
			wchar_t* header;
			int norow=mysql_num_rows(result);
			int num_fields=mysql_num_fields(result);
			header=(wchar_t*)GlobalAlloc(GPTR, sizeof(wchar_t)*num_fields*norow*500);
			wcscpy(header,L"");
			while ((row = mysql_fetch_row(result))) 
			{ 
						  
		    for(int i = 0; i <num_fields; i++) 
			{ 
				if (i == 0) 
			{              
			while(field = mysql_fetch_field(result) )
			{
				wcscat(header,L"\t\t");
				int length;


				length = MultiByteToWideChar (CP_UTF8,0,field->name,strlen(field->name)+1, NULL, 0);

				wchar_t * name = new wchar_t [length+1];


				MultiByteToWideChar (CP_UTF8, 0,field->name, -1, name, length);
				name[length]=L'\0';
				wcscat(header,name);
				delete[]name;

			}

			wcscat(header,L"\r\n");           
		}
		wcscat(header,L"\t\t");
		int length;

		char * details=new char[100];
		strcpy(details,"");
		strcat(details,(row[i] ? row[i] : "NULL"));
		length = MultiByteToWideChar (CP_UTF8, 0,details, strlen(details)+1, NULL, 0);

		wchar_t * name = new wchar_t [length+1];


		MultiByteToWideChar (CP_UTF8, 0, details, -1, name, length);
		name[length]=L'\0';
		wcscat(header,name);
		delete[] name;
		delete[] details;

		} 
		wcscat(header,L"\r\n");
			
		}
  
		mysql_free_result(result);
		AppendText(hWnd,header);
		//SetDlgItemText(hWnd,IDC_RESULT,header);

		//SendMessage(hWnd,EM_REPLACESEL,TRUE,header);
			
		}
			 
			else{
  
		if(mysql_errno(con))
		{
			fprintf(stderr, "%s\n", mysql_error(con));
		}

		else{
				printf("Working !!");
			}
			}

		//Append Code
		status=mysql_next_result(con);
		if(status>0)
		{
		MessageBox(hWnd, TEXT("Error in next query"), TEXT("Error"),MB_OK | MB_ICONINFORMATION);
		}

		} while(status==0);
		
		}
		EnableWindow(hStop,false);
		break;
		
		}


	case IDM_EXIT:
		Sqlcon.CloseConnection();
		Sqlcon.ClearDetails();
		MessageBox(hWnd, TEXT("Window Exited!"), TEXT("Exit"),MB_OK | MB_ICONINFORMATION);
		DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	break;
	}

case WM_PAINT:
	hdc = BeginPaint(hWnd, &ps);
	// TODO: Add any drawing code here...
	EndPaint(hWnd, &ps);
	break;
case WM_DESTROY:
	Sqlcon.CloseConnection();
	PostQuitMessage(0);
	break;
default:
	return DefWindowProc(hWnd, message, wParam, lParam);
}
return 0;
}

// Message handler for Connection Dialogue box.
INT_PTR CALLBACK Conn_Details(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
//HWND hwndCon;
//hwndCon = GetDlgItem(hDlg, IDD_CON);
UNREFERENCED_PARAMETER(lParam);
switch (message)
{
case WM_INITDIALOG:
	SetDlgItemText(hDlg,IDC_USER,L"root");
	SetDlgItemText(hDlg,IDC_PASS,L"root");
	SetDlgItemText(hDlg,IDC_HOST,L"localhost");
	SetDlgItemText(hDlg,IDC_PORT,L"3306");
	SetDlgItemText(hDlg,IDC_DB,L"sql_demo_db");
	return (INT_PTR)TRUE;

		

case WM_COMMAND:
	if (LOWORD(wParam) == IDC_CONBUTTON)
	{
		HWND hUser=GetDlgItem(hDlg,IDC_USER);
		HWND hPass=GetDlgItem(hDlg,IDC_PASS);
		HWND hDB=GetDlgItem(hDlg,IDC_DB);
		HWND hHost=GetDlgItem(hDlg,IDC_HOST);
		
/////////////////-------------------------Usename---------------------------------///////////////////			
		int n=GetWindowTextLength(hUser)+1;
		wchar_t* username= new wchar_t[n+1];
		GetDlgItemText(hDlg,IDC_USER,username,n);
		//MessageBox(hDlg,(LPCWSTR)username,L"Connection Details",MB_OK | MB_ICONINFORMATION);

		int len = WideCharToMultiByte(CP_UTF8, 0,username,n+1,NULL,0,NULL, 0);
		char* sql_username=new char[len+1];
		WideCharToMultiByte(CP_UTF8,0,username,-1,sql_username,len+1,NULL,NULL);
/////////////////-------------------------Password---------------------------------///////////////////

			n=GetWindowTextLength(hPass)+1;
			wchar_t* pass= new wchar_t[n+1];
		GetDlgItemText(hDlg,IDC_PASS,pass,n);
		//MessageBox(hDlg,(LPCWSTR)pass,L"Connection Details",MB_OK | MB_ICONINFORMATION);

		len = WideCharToMultiByte(CP_UTF8, 0,pass,n+1,NULL,0,NULL, 0);
		char* sql_pass=new char[len+1];
		WideCharToMultiByte(CP_UTF8,0,pass,-1,sql_pass,len+1,NULL,NULL);

/////////////////-------------------------DB---------------------------------///////////////////
			
		n=GetWindowTextLength(hDB)+1;
			wchar_t *db= new wchar_t[n+1]; 
			GetDlgItemText(hDlg,IDC_DB,db,n);
			//MessageBox(hDlg,(LPCWSTR)db,L"Connection Details",MB_OK | MB_ICONINFORMATION);

			len = WideCharToMultiByte(CP_UTF8, 0,db,n+1,NULL,0,NULL, 0);
		char* sql_db=new char[len+1];
		WideCharToMultiByte(CP_UTF8,0,db,-1,sql_db,len+1,NULL,NULL);

			
/////////////////-------------------------Host---------------------------------///////////////////

			n=GetWindowTextLength(hHost)+1;
		wchar_t* host=new wchar_t[n+1];
		GetDlgItemText(hDlg,IDC_HOST,host,n);
		//MessageBox(hDlg,(LPCWSTR)host,L"Connection Details",MB_OK | MB_ICONINFORMATION);

		len = WideCharToMultiByte(CP_UTF8, 0,host,n+1,NULL,0,NULL, 0);
		char* sql_host=new char[len+1];
		WideCharToMultiByte(CP_UTF8,0,host,-1,sql_host,len+1,NULL,NULL);

///////------------------------PORT----------------------------------/////////////////////

		BOOL success=true;
		int portno=GetDlgItemInt(hDlg,IDC_PORT,&success,true);
		if(Sqlcon.SetConnection(sql_username,sql_pass,sql_db,sql_host,portno)) { 
////////////---------------------------------------------------------/////////////////////
		
		EndDialog(hDlg,true);
			  
		CreateWindowEx(NULL, TEXT("EDIT"),TEXT("Write Query"),
			WS_BORDER|WS_HSCROLL|WS_VSCROLL|ES_AUTOVSCROLL|ES_WANTRETURN|WS_COPY|WS_PASTE|WS_VISIBLE|WS_CHILD|WS_TABSTOP,0,0,1275,350, hWnd, (HMENU)IDC_QUERY,GetModuleHandle(NULL), NULL);
			

		CreateWindowEx(NULL, TEXT("EDIT"),TEXT("Query Result"),
    WS_BORDER|WS_HSCROLL|WS_VSCROLL|WS_VISIBLE|WS_CHILD|WS_TABSTOP|ES_MULTILINE|ES_READONLY,0,400,1275,525, hWnd, (HMENU)IDC_RESULT,GetModuleHandle(NULL), NULL);
			 
			 
		CreateWindowEx(NULL, TEXT("BUTTON"),TEXT("Execute Query"),
    WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON|WS_TABSTOP,
	700,360,150, 30, hWnd, (HMENU)IDC_EXEC, GetModuleHandle(NULL), NULL);
			 

		CreateWindowEx(NULL, TEXT("BUTTON"),TEXT("Stop Query"),
    WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON|WS_TABSTOP,
    500,360,150, 30, hWnd, (HMENU)IDC_STOP, GetModuleHandle(NULL), NULL);	
		HWND hStop=GetDlgItem(hWnd,IDC_STOP);
		EnableWindow(hStop,false);

		CreateWindowEx(NULL, TEXT("BUTTON"),TEXT("Disconnect"),
    WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON|WS_TABSTOP,
	300,360,150, 30, hWnd, (HMENU)IDC_DISCON, GetModuleHandle(NULL), NULL);
	}
		else{
		MessageBox(hDlg,L"Connection Unsuccessful",L"Failure",MB_OK | MB_ICONINFORMATION);
		}
	}
	return (INT_PTR)true;	

	case IDCANCEL:
                EndDialog(hDlg, true);
            break;
		
	
default:
	return (INT_PTR)false;
	
}
return (INT_PTR)true;
}
BOOL CALLBACK About(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
switch(Message)
{
    case WM_INITDIALOG:
			return TRUE;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
            case IDOK:
                EndDialog(hwnd, IDOK);
            break;
            case IDCANCEL:
                EndDialog(hwnd, IDCANCEL);
            break;
        }
    break;
    default:
        return FALSE;
}
return TRUE;
}

void AppendText( const HWND &hwnd, TCHAR *newText )
{
// get edit control from dialog
HWND hwndOutput = GetDlgItem( hwnd, IDC_RESULT);
SetFocus (hwndOutput);
// get the current selection
DWORD StartPos, EndPos;
SendMessage( hwndOutput, EM_GETSEL, reinterpret_cast<WPARAM>(&StartPos), reinterpret_cast<LPARAM>(&EndPos) );

// move the caret to the end of the text
int outLength = GetWindowTextLength(hwndOutput);
//SendMessage( hwndOutput, (UINT)EM_SETSEL, outLength, outLength );
SendMessage( hwndOutput, (UINT)EM_SETSEL, (WPARAM)outLength,(LPARAM)outLength);

// insert the text at the new caret position
//SendMessage(hwndOutput, (UINT)EM_REPLACESEL, (WPARAM) FALSE,(LPARAM)"\r\n");
SendMessage(hwndOutput, (UINT)EM_REPLACESEL, (WPARAM) FALSE,(LPARAM)newText);

// restore the previous selection
SendMessage( hwndOutput, EM_SETSEL, StartPos, EndPos );


}

//void AppendText( const HWND &hDlg, TCHAR *newText ){
// HWND hEdit = GetDlgItem (hDlg, IDC_RESULT);
//   int ndx = GetWindowTextLength (hEdit);
//   SetFocus (hEdit);
//   #ifdef WIN32
//      SendMessage (hEdit, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
//   #else
//      SendMessage (hEdit, EM_SETSEL, 0, MAKELONG (ndx, ndx));
//   #endif
//	   #ifdef WIN32
//      SendMessage (hEdit, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
//   #else
//      SendMessage (hEdit, EM_SETSEL, 0, MAKELONG (ndx, ndx));
//   #endif
//      SendMessage (hEdit, EM_REPLACESEL, 0, (LPARAM)newText);
//}
//
