#include "client.h"
#include "resource.h"
#include "mstc_project.h"
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"Kernel32.lib")
#pragma comment(lib,"Winmm.lib")

char file_name[50];
extern int file_permissions[9];
extern int file_creation_mode;

char* buffer_string;

extern hashqueue_t* p_hashqueue;
extern list_t* p_freelist;
extern i_list_t* i_p_freelist;
extern i_hashqueue_t* i_p_hashqueue;
extern dilb_inode_t* dilb[MAX_DILB_SIZE];
extern iit_inode_t* incore_inode_table[MAX_IIT_SIZE];
extern ufdt_t* ufdt[MAX_UFDT_SIZE];
extern ft_t* ft[MAX_FT_SIZE];
extern super_block_t* super_block;
extern char* hdd[1024];

int flag;

FILE* fp_log_file=nullptr;
FILE* fp_log_file_r=nullptr;

char* logs=nullptr;

int screen_width = GetSystemMetrics(SM_CXSCREEN);
int screen_height = GetSystemMetrics(SM_CYSCREEN);


int SPLASH_SCREEN_X_SHIFT_OUTER_CREDIT=0; 
int SPLASH_SCREEN_Y_SHIFT_OUTER_CREDIT=0;
int SPLASH_SCREEN_X_SHIFT_INNER_CREDIT=0; 
int SPLASH_SCREEN_Y_SHIFT_INNER_CREDIT=0;
int SPLASH_SCREEN_CIRCLE_RADIUS=80;

int SPLASH_SCREEN_X_SHIFT_OUTER=0; 
int SPLASH_SCREEN_Y_SHIFT_OUTER=0;
int SPLASH_SCREEN_X_SHIFT_INNER=0; 
int SPLASH_SCREEN_Y_SHIFT_INNER=0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    p_vector = create_vector(); 
	draw_rectangle(p_vector);  

    p_vector_circle = create_vector() ; 
    create_circle_points(p_vector_circle,SPLASH_SCREEN_CIRCLE_RADIUS);

    static TCHAR szMainName[] = TEXT("Main window");
    static TCHAR szSplashName[] = TEXT("Splash window");
    static TCHAR szWindowCaption[] = TEXT("splash screen");
    TCHAR szCreditsName[] = TEXT("credit");

	WNDCLASSEX MainWndEx; 
    WNDCLASSEX SplashWndEx;
    WNDCLASSEX CreditsWndEx;
	MSG msg;	

    ZeroMemory(&MainWndEx,sizeof(WNDCLASSEX));
    ZeroMemory(&SplashWndEx,sizeof(WNDCLASSEX));
    ZeroMemory(&msg,sizeof(MSG));

    HWND hRadioButton_creat = NULL;
    HWND hRadioButton_open = NULL;
    HWND hRadioButton_close = NULL;
    HWND hRadioButton_read = NULL;
    HWND hRadioButton_write = NULL;
    HWND hRadioButton_logs = NULL;
    HWND hRadioButton_end = NULL;

    initialize();

	MainWndEx.cbClsExtra = NULL;
	MainWndEx.cbSize = sizeof(WNDCLASSEX);
	MainWndEx.cbWndExtra = NULL;
	MainWndEx.hbrBackground = CreateSolidBrush(RGB(79,0,39)); 
	MainWndEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	MainWndEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	MainWndEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	MainWndEx.hInstance = hInstance;
	MainWndEx.lpfnWndProc = WndProc;
	MainWndEx.lpszClassName = szMainName;
	MainWndEx.lpszMenuName = NULL;
	MainWndEx.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&MainWndEx))
	{
		MessageBox(NULL, TEXT("Class Registration Failed"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return 0;
	}

	hMain = CreateWindowEx(WS_EX_CLIENTEDGE,szMainName, TEXT("Lady Ada Lovelace Group"), 
                        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
                        CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    if(hMain == NULL)
    {
        MessageBox((HWND)NULL,TEXT("Cannot get handle to Window"),TEXT("CreateWindowEx"),MB_ICONERROR);
        return EXIT_FAILURE;
    }

    SplashWndEx.cbSize = sizeof(WNDCLASSEX);
    SplashWndEx.style = 0;
    SplashWndEx.lpfnWndProc = SplashWndProc;
    SplashWndEx.cbClsExtra = 0;
    SplashWndEx.cbWndExtra = 0;
    SplashWndEx.hInstance = hInstance;
    SplashWndEx.hIcon = NULL;
    SplashWndEx.hIconSm = NULL;
    SplashWndEx.hCursor = LoadCursor(NULL,IDC_ARROW);
    SplashWndEx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    SplashWndEx.lpszMenuName = NULL;
    SplashWndEx.lpszClassName = szSplashName;

    if(!RegisterClassEx(&SplashWndEx))
    {
        MessageBox((HWND)NULL,TEXT("Cannot register Splash class"),TEXT("RegisterClassEx"),MB_ICONERROR);
        return EXIT_FAILURE;
    }

    CreditsWndEx.cbSize = sizeof(WNDCLASSEX);
	CreditsWndEx.style = CS_HREDRAW | CS_VREDRAW;
	CreditsWndEx.cbClsExtra = 0; 
	CreditsWndEx.cbWndExtra = 0; 	
    CreditsWndEx.lpfnWndProc = CreditsWndProc; 
	CreditsWndEx.hInstance = hInstance; 
	CreditsWndEx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	CreditsWndEx.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	CreditsWndEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	CreditsWndEx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	CreditsWndEx.lpszClassName = szCreditsName; 
	CreditsWndEx.lpszMenuName = NULL; 

    RegisterClassEx(&CreditsWndEx);

    hwnd_credits = CreateWindow(szCreditsName, 
        NULL,    
		WS_DLGFRAME,	   
		CW_USEDEFAULT,				
		CW_USEDEFAULT,				
		CW_USEDEFAULT ,				
		CW_USEDEFAULT,						
        (HWND)NULL,                 
        (HMENU)NULL,				
		hInstance,				 
		NULL);

    hSplash = CreateWindowEx(WS_EX_DLGMODALFRAME,
                            szSplashName, 
    				        NULL,    
						    WS_POPUP|WS_CHILD,
						    CW_USEDEFAULT,				
						    CW_USEDEFAULT,				
						    CW_USEDEFAULT ,				
						    CW_USEDEFAULT,				
						    (HWND)NULL,                 
        				    (HMENU)NULL,				
						    hInstance,					
						    NULL);

    hRadioButton_creat = CreateWindowEx(0,
                                   TEXT("BUTTON"),
                                   TEXT("Create File"),
                                   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
                                   125, 90, 130, 20,
                                   hMain,
                                   HMENU(RADIO_BUTTON_CREAT),
                                   hInstance,
                                   NULL
                                  ); 

    hRadioButton_open = CreateWindowEx(0,
                                   TEXT("BUTTON"),
                                    TEXT("Open File"),
                                   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
                                   125, 120, 130, 20,
                                   hMain,
                                   HMENU(RADIO_BUTTON_OPEN),
                                   hInstance,
                                   NULL
                                  );

    hRadioButton_close = CreateWindowEx(0,
                                   TEXT("BUTTON"),
                                   TEXT("Close File"),
                                   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
                                   125, 150, 130, 20,
                                   hMain,
                                   HMENU(RADIO_BUTTON_CLOSE),
                                   hInstance,
                                   NULL
                                  );

    hRadioButton_read = CreateWindowEx(0,
                                   TEXT("BUTTON"),
                                   TEXT("Read File"),
                                   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
                                   125, 180, 130, 20,
                                   hMain,
                                   HMENU(RADIO_BUTTON_READ),
                                   hInstance,
                                   NULL
                                  );

    hRadioButton_write = CreateWindowEx(0,
                                   TEXT("BUTTON"),
                                   TEXT("Write File"),
                                   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
                                   125, 210, 130, 20,
                                   hMain,
                                   HMENU(RADIO_BUTTON_WRITE),
                                   hInstance,
                                   NULL
                                  );

    hRadioButton_logs = CreateWindowEx(0,
                                   TEXT("BUTTON"),
                                   TEXT("Show Logs"),
                                   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
                                   125, 300, 130, 20,
                                   hMain,
                                   HMENU(RADIO_BUTTON_LOGS),
                                   hInstance,
                                   NULL
                                  ); 

    hRadioButton_end = CreateWindowEx(0,
                                   TEXT("BUTTON"),
                                   TEXT("End Demo"),
                                   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
                                   1300, 750, 100, 20,
                                   hMain,
                                   HMENU(RADIO_BUTTON_END),
                                   hInstance,
                                   NULL
                                  );                    

    /*dwStyle = GetWindowLong(hwnd, GWL_STYLE);
    if (dwStyle & WS_OVERLAPPEDWINDOW)
    {
        mi = { sizeof(MONITORINFO) };
        if (GetWindowPlacement(hwnd,&wpPrev) && GetMonitorInfo(MonitorFromWindow(hwnd, MONITORINFOF_PRIMARY), &mi))
        {
            SetWindowLong(hwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, (mi.rcMonitor.right - mi.rcMonitor.left),( mi.rcMonitor.bottom - mi.rcMonitor.top), SWP_NOZORDER | SWP_FRAMECHANGED);
        }
    }*/
	
    //ShowCursor(FALSE);

	//ShowWindow(hMain, SW_MAXIMIZE);
	//UpdateWindow(hMain);

    SetWindowLong(hSplash, GWL_STYLE, 0);
	SetWindowPos(hSplash, HWND_TOP, 0 , 0, screen_width, screen_height, SWP_FRAMECHANGED);

	AnimateWindow(hSplash, 2000, AW_CENTER);
	UpdateWindow(hSplash);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

    deinitialize();
    destroy_vector(&p_vector); 
    destroy_vector(&p_vector_circle);

	return (int)msg.wParam;
}

void initialize(void)
{
    //Initialize in memory hdd
	hdd_initialize();
	//Read file disk in memory
	hdd_disk_read(hdd);
	//Initialize buffer freelist and hashqueue
	p_freelist = create_list();
	initialize_freelist(p_freelist);

	p_hashqueue = create_hashqueue();

	//Initialize inode freelist and hashqueue
	i_p_freelist = i_create_list();
	i_initialize_freelist(i_p_freelist);

	i_p_hashqueue = i_create_hashqueue();

	//Create in memory dilb
	dilb_initialize();

	//Initialize in memory dilb with disk
	dilb_disk_read(dilb);

	//Initialize Superblock
	superblk_initialize();

	//Read disk contents of superblock
	superblk_disk_read(super_block);

    fp_log_file = fopen("Log.txt","w");
    fclose(fp_log_file);

    fp_log_file = fopen("Log.txt","a");
    if(fp_log_file == NULL)
    {
        MessageBox(NULL,TEXT("Cannot open log file"),TEXT("Error"),MB_OK);
        exit(EXIT_FAILURE);
    }

    buffer_string = (char*)calloc(10024,sizeof(char));
    if(buffer_string == NULL)
    {
        MessageBox(NULL,TEXT("buffer_string cannot be calloced"),TEXT("ERROR"),MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    logs = (char*)malloc(10000000);
    if(logs == NULL)
    {
        MessageBox(NULL,TEXT("Logs cannot be malloced"),TEXT("ERROR"),MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    PlaySound("C:\\Users\\91928\\Documents\\VisualCodeProjects\\CPA_MSTC_PROJECT\\MSTC PROJECT\\sound.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam)
{
    HINSTANCE hInstance =  NULL;
    HDC hdc = NULL;
    PAINTSTRUCT ps;

    TEXTMETRIC tm; 
    static float cxClient, cyClient, cxChar, cyChar;
    int i;

	switch (imsg)
	{
        case WM_COMMAND:
            if (LOWORD(wParam) == RADIO_BUTTON_CREAT)
            {
                flag = 1;
                displayDlg(hwnd);
				DialogBox((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
				          MAKEINTRESOURCE(CREAT), hwnd, (DLGPROC)DlgProc);

                for(i=0;i<9;i++)
                {
                    file_permissions[i]=0;
                }
				
			}
            else if(LOWORD(wParam) == RADIO_BUTTON_OPEN)
            {
                flag = 2;
                displayDlg(hwnd);
				DialogBox((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
				          MAKEINTRESOURCE(OPEN), hwnd, (DLGPROC)DlgProc);
            }
            else if(LOWORD(wParam) == RADIO_BUTTON_CLOSE)
            {
                flag = 3;
                displayDlg(hwnd);
				DialogBox((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
				          MAKEINTRESOURCE(CLOSE), hwnd, (DLGPROC)DlgProc);
            }
            else if(LOWORD(wParam) == RADIO_BUTTON_READ)
            {
                flag = 4;
                displayDlg(hwnd);
				DialogBox((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
				          MAKEINTRESOURCE(READ), hwnd, (DLGPROC)DlgProc);
            }
            else if(LOWORD(wParam) == RADIO_BUTTON_WRITE)
            {
                flag = 5;
                displayDlg(hwnd);
				DialogBox((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
				          MAKEINTRESOURCE(WRITE), hwnd, (DLGPROC)DlgProc);
            }
            else if(LOWORD(wParam) == RADIO_BUTTON_LOGS)
            {
                flag = 6;
                displayDlg(hwnd);
				DialogBox((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
				          MAKEINTRESOURCE(LOGS), hwnd, (DLGPROC)DlgProc);
            }
            else if(LOWORD(wParam) == RADIO_BUTTON_END)
            {
                DestroyWindow(hwnd);
                SetWindowLong(hwnd_credits,GWL_STYLE,0);
	            SetWindowPos(hwnd_credits, HWND_TOP , 0, 0,screen_width, screen_height, SWP_FRAMECHANGED);
                ShowWindow(hwnd_credits,SW_MAXIMIZE);
	            UpdateWindow(hwnd_credits); 
                SetTimer(hwnd_credits, 1,10, NULL); 
                PlaySound(NULL, NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
                PlaySound("C:\\Users\\91928\\Documents\\VisualCodeProjects\\CPA_MSTC_PROJECT\\MSTC PROJECT\\Whiplash.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
            }
            break;
         case WM_SIZE: 
				cxClient = LOWORD(lParam);
				cyClient = HIWORD(lParam);

				break;
        
        case WM_PAINT:
                hdc = BeginPaint(hwnd, &ps);

                display(hwnd,hdc,ps,cxClient,cyClient);
                EndPaint(hwnd,&ps);
                break;

	    case WM_DESTROY:
		    //PostQuitMessage(0);
		    break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}

void display(HWND hwnd,HDC hdc,PAINTSTRUCT ps,float cxClient,float cyClient)
{
    SetBkMode(hdc,TRANSPARENT);
    SetTextColor(hdc,RGB(255,255,255));
    
    TextOut(hdc,(cxClient)/2+50,(cyClient)/2+50,TEXT("IIT"),3);
    show_IIT(hdc,cxClient,cyClient);

    TextOut(hdc,cxClient/2-125,cyClient/2+50,TEXT("FT"),2);
    show_FT(hdc,cxClient,cyClient);

    TextOut(hdc,cxClient/2-310,cyClient/2+50,TEXT("UFDT"),4);
    show_UFDT(hdc,cxClient,cyClient);

    TextOut(hdc,2*cxClient/3,cyClient/10,TEXT("SUPERBLOCK:"),11);
    show_Superblock(hdc,cxClient,cyClient);

    HPEN hpen=NULL;
    //For border
    hpen = CreatePen(PS_SOLID, 8, RGB(255, 255, 255)) ;
    SelectObject(hdc, hpen);
    MoveToEx (hdc, cxClient, 2 , NULL);
    LineTo(hdc, 0, 2);

    MoveToEx (hdc,cxClient, cyClient-2 , NULL);
    LineTo(hdc, 0, cyClient-2);

    MoveToEx (hdc,2, 2 , NULL);
    LineTo(hdc, 2, cyClient-2);

    MoveToEx (hdc,cxClient-2, 2 , NULL);
    LineTo(hdc,cxClient-2, cyClient-2);

    DeleteObject(hpen); 
//--------------------------------------------------------------------------------------------

    hpen = CreatePen(PS_SOLID, 4, RGB(255, 255, 255)) ;
    SelectObject(hdc, hpen);

    MoveToEx (hdc,(cxClient/2)+200, 0 , NULL);
    LineTo(hdc, (cxClient/2)+200, (cyClient/2)+70);

    //MoveToEx (hdc,2, 2 , NULL);
    //LineTo(hdc, 2, cyClient-2);

    //MoveToEx (hdc,cxClient-2, 2 , NULL);
    //LineTo(hdc,cxClient-2, cyClient-2);

    DeleteObject(hpen); 

    hpen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255)) ;
    SelectObject(hdc, hpen);
   
    //First Table
    MoveToEx (hdc, cxClient/3, (cyClient)/10 , NULL);
    LineTo(hdc, cxClient/3  , (5.28*cyClient)/10);

    MoveToEx (hdc, cxClient/3.5, cyClient/10 , NULL);
    LineTo(hdc, cxClient/3.5, (5.28*cyClient)/10);

    for(int i = 0; i<16;i= i+1)
    {
        MoveToEx (hdc, cxClient/3, ((1+i/3.5)*cyClient/10) , NULL);
        LineTo(hdc, cxClient/3.5,((1+i/3.5)*cyClient/10));
    }

    // second table
    MoveToEx (hdc, cxClient/2.25, cyClient/10 , NULL);
    LineTo(hdc, cxClient/2.25, (5.28*cyClient)/10);

    MoveToEx (hdc, cxClient/2.5, cyClient/10 , NULL);
    LineTo(hdc, cxClient/2.5, (5.28*cyClient)/10);

    for(int i = 0; i<16;i= i+1)
    {
        MoveToEx (hdc, cxClient/2.25, ((1+i/3.5)*cyClient/10) , NULL);
        LineTo(hdc, cxClient/2.5,((1+i/3.5)*cyClient/10));
    }

    //third table
    MoveToEx (hdc, cxClient/1.95, cyClient/10 , NULL);
    LineTo(hdc, cxClient/1.95, (5.28*cyClient)/10);

    MoveToEx (hdc, cxClient/1.8, cyClient/10 , NULL);
    LineTo(hdc, cxClient/1.8, (5.28*cyClient)/10);

    for(int i = 0; i<16;i= i+1)
    {
        MoveToEx (hdc, cxClient/1.95, ((1+i/3.5)*cyClient/10) , NULL);
        LineTo(hdc, cxClient/1.8,((1+i/3.5)*cyClient/10));
    }

    DeleteObject(hpen);

    TextOut(hdc,(0.5)*cxClient/20,6*cyClient/10,TEXT("FREELIST OF BUFFERS: "),strlen("FREELIST OF BUFFERS:  "));
    show_freelist_buffer(hdc,cxClient,cyClient);

    TextOut(hdc,(0.5)*cxClient/20,7*cyClient/10,TEXT("FREELIST OF INODES: "),strlen("FREELIST OF INODES:  "));
    show_freelist_inode(hdc,cxClient,cyClient);

    TextOut(hdc,(0.5)*cxClient/20,8*cyClient/10,TEXT("HASHQUEUE OF BUFFERS: "),strlen("HASHQUEUE OF BUFFERS:  "));
    show_hashqueue_buffer(hdc,cxClient,cyClient);

    TextOut(hdc,(10)*cxClient/20,8*cyClient/10,TEXT("HASHQUEUE OF INODES: "),strlen("HASHQUEUE OF INODES:  "));
    show_hashqueue_inode(hdc,cxClient,cyClient);
}

void dlgCommand(HWND hwnd,WPARAM wParam,LPARAM lParam)
{
    int wmId, wmEvent;

    wmId = LOWORD(wParam);
    wmEvent = HIWORD(wParam); 

    char line[10000];

    if(flag == 6)
    {
        fp_log_file_r = fopen("Log.txt","r");
        if(fp_log_file_r == NULL)
        {
            MessageBox(NULL,TEXT("Cannot open log file for reading."),TEXT("ERROR"),MB_ICONERROR);
            exit(EXIT_FAILURE);
        }

        //fscanf(fp_log_file_r,"%s",logs); 
        memset(logs,'\0',strlen(logs));

        while((fgets(line,10000,fp_log_file_r)) != NULL )
        {
            strcat(logs,line);
        }
        
        SetDlgItemText(hwnd,ID_LOGS,logs);
        //EnableWindow(GetDlgItem(hwnd,ID_LOGS),false);
    }

    int no_of_bytes,status;

    if(wmId == ID_ETFILENAME)
    {
        if(GetDlgItemText(hwnd, ID_ETFILENAME, file_name, 50)!='\0')
        {
            EnableWindow(GetDlgItem(hwnd,RB_READ),true);
            EnableWindow(GetDlgItem(hwnd,RB_WRITE),true);

            EnableWindow(GetDlgItem(hwnd,ID_NO_OF_BYTES),true);

            no_of_bytes = GetDlgItemInt(hwnd, ID_NO_OF_BYTES,&status ,TRUE); 

            EnableWindow(GetDlgItem(hwnd,ID_DATA),true);
            
        }
        else
        {
            if (IsDlgButtonChecked(hwnd, RB_READ)) 
                CheckDlgButton(hwnd,RB_READ, BST_UNCHECKED); 
    
            if (IsDlgButtonChecked(hwnd, RB_WRITE)) 
                CheckDlgButton(hwnd,RB_WRITE, BST_UNCHECKED); 
    
            if (IsDlgButtonChecked(hwnd, RB_UR)) 
                CheckDlgButton(hwnd,RB_UR, BST_UNCHECKED); 
            
            if (IsDlgButtonChecked(hwnd, RB_UW)) 
                CheckDlgButton(hwnd,RB_UW, BST_UNCHECKED); 
            
            if (IsDlgButtonChecked(hwnd, RB_UX)) 
                CheckDlgButton(hwnd,RB_UX, BST_UNCHECKED); 

            if (IsDlgButtonChecked(hwnd, RB_GR)) 
                CheckDlgButton(hwnd,RB_GR, BST_UNCHECKED); 
            
            if (IsDlgButtonChecked(hwnd, RB_GW)) 
                CheckDlgButton(hwnd,RB_GW, BST_UNCHECKED); 
            
            if (IsDlgButtonChecked(hwnd, RB_GX)) 
                CheckDlgButton(hwnd,RB_GX, BST_UNCHECKED); 

            if (IsDlgButtonChecked(hwnd, RB_OR)) 
                CheckDlgButton(hwnd,RB_OR, BST_UNCHECKED); 
            
            if (IsDlgButtonChecked(hwnd, RB_OW)) 
                CheckDlgButton(hwnd,RB_OW, BST_UNCHECKED); 
            
            if (IsDlgButtonChecked(hwnd, RB_OX)) 
                CheckDlgButton(hwnd,RB_OX, BST_UNCHECKED); 
            
            EnableWindow(GetDlgItem(hwnd,RB_READ),false);
            EnableWindow(GetDlgItem(hwnd,RB_WRITE),false); 
            EnableWindow(GetDlgItem(hwnd,RB_UR),false);
            EnableWindow(GetDlgItem(hwnd,RB_UW),false);
            EnableWindow(GetDlgItem(hwnd,RB_UX),false);
            EnableWindow(GetDlgItem(hwnd,RB_GR),false);
            EnableWindow(GetDlgItem(hwnd,RB_GW),false);
            EnableWindow(GetDlgItem(hwnd,RB_GX),false);
            EnableWindow(GetDlgItem(hwnd,RB_OR),false);
            EnableWindow(GetDlgItem(hwnd,RB_OW),false);
            EnableWindow(GetDlgItem(hwnd,RB_OX),false); 
            EnableWindow(GetDlgItem(hwnd,ID_NO_OF_BYTES),false);
            EnableWindow(GetDlgItem(hwnd,ID_DATA),false); 
        }
    }

    if(wmId == RB_UR)
    {
        if (wmEvent == BN_CLICKED)
        {
            int chkState = (int)SendMessage((HWND)lParam, BM_GETCHECK, 0, 0);
            if (chkState == BST_CHECKED)
            {
                file_permissions[0] = 1 ;
                //MessageBoxA(hwnd, "Read permission is enabled for User!", "CheckBox", MB_OK);
            }
            else 
            {
                file_permissions[0] = 0 ;
                //MessageBoxA(hwnd, "Read permission is disabled for User!", "CheckBox", MB_OK);
            }
        }
    }

    if(wmId == RB_UW)
    {
        if (wmEvent == BN_CLICKED)
        {
            int chkState = (int)SendMessage((HWND)lParam, BM_GETCHECK, 0, 0);
            if (chkState == BST_CHECKED)
            {
                file_permissions[1] = 1 ;
                //MessageBoxA(hwnd, "Write permission is enabled for User!", "CheckBox", MB_OK);
            }
            else 
            {
                file_permissions[1] = 0 ;
                //MessageBoxA(hwnd, "Write permission is disabled for User!", "CheckBox", MB_OK);
            }
        }
    }

    if(wmId == RB_UX)
    {
        if (wmEvent == BN_CLICKED)
        {
            int chkState = (int)SendMessage((HWND)lParam, BM_GETCHECK, 0, 0);
            if (chkState == BST_CHECKED)
            {
                file_permissions[2] = 1 ;
                //MessageBoxA(hwnd, "Execute permission is enabled for User!", "CheckBox", MB_OK);
            }
            else 
            {
                file_permissions[2] = 0 ;
                //MessageBoxA(hwnd, "Execute permission is disabled for User!", "CheckBox", MB_OK);
            }
        }
    }

    if(wmId == RB_GR)
    {
        if (wmEvent == BN_CLICKED)
        {
            int chkState = (int)SendMessage((HWND)lParam, BM_GETCHECK, 0, 0);
            if (chkState == BST_CHECKED)
            {
                file_permissions[3] = 1 ;
                //MessageBoxA(hwnd, "Read permission is enabled for Group!", "CheckBox", MB_OK);
            }
            else 
            {
                file_permissions[3] = 0 ;
                //MessageBoxA(hwnd, "Read permission is disabled for Group!", "CheckBox", MB_OK);
            }
        }
    }

    if(wmId == RB_GW)
    {
        if (wmEvent == BN_CLICKED)
        {
            int chkState = (int)SendMessage((HWND)lParam, BM_GETCHECK, 0, 0);
            if (chkState == BST_CHECKED)
            {
                file_permissions[4] = 1 ;
                //MessageBoxA(hwnd, "Write permission is enabled for Group!", "CheckBox", MB_OK);
            }
            else 
            {
                file_permissions[4] = 0 ;
                //MessageBoxA(hwnd, "Write permission is disabled for Group!", "CheckBox", MB_OK);
            }
        }
    }
        
    if(wmId == RB_GX)
    {
        if (wmEvent == BN_CLICKED)
        {
            int chkState = (int)SendMessage((HWND)lParam, BM_GETCHECK, 0, 0);
            if (chkState == BST_CHECKED)
            {
                file_permissions[5] = 1 ;
                //MessageBoxA(hwnd, "Execute permission is enabled for Group!", "CheckBox", MB_OK);
            }
            else 
            {
                file_permissions[5] = 0 ;
                //MessageBoxA(hwnd, "Execute permission is disabled for Group!", "CheckBox", MB_OK);
            }
        }
    }

    if(wmId == RB_OR)
    {
        if (wmEvent == BN_CLICKED)
        {
            int chkState = (int)SendMessage((HWND)lParam, BM_GETCHECK, 0, 0);
            if (chkState == BST_CHECKED)
            {	
                file_permissions[6] = 1 ;
                //MessageBoxA(hwnd, "Read permission is enabled for Other!", "CheckBox", MB_OK);
            }
            else 
            {
                file_permissions[6] = 0 ;
                //MessageBoxA(hwnd, "Read permission is disabled for Other!", "CheckBox", MB_OK);
            }				
        }
    }

    if(wmId == RB_OW)
    {
        if (wmEvent == BN_CLICKED)
        {
            int chkState = (int)SendMessage((HWND)lParam, BM_GETCHECK, 0, 0);
            if (chkState == BST_CHECKED)
            {	
                file_permissions[7] = 1 ;
                //MessageBoxA(hwnd, "Write permission is enabled for Others!", "CheckBox", MB_OK);
            }
            else 
            {
                file_permissions[7] = 0 ;
                //MessageBoxA(hwnd, "Write permission is disabled for Others!", "CheckBox", MB_OK);
            }
        }
    }

    if(wmId == RB_OX)
    {
        if (wmEvent == BN_CLICKED)
        {
            int chkState = (int)SendMessage((HWND)lParam, BM_GETCHECK, 0, 0);
            if (chkState == BST_CHECKED)
            {	
                file_permissions[8] = 1 ;
                //MessageBoxA(hwnd, "Execute permission is enabled for Others!", "CheckBox", MB_OK);
            }
            else 
            {
                file_permissions[8] = 0 ;
                //MessageBoxA(hwnd, "Execute permission is disabled for Others!", "CheckBox", MB_OK);
            }
        }
    }

    if(wmId == RB_READ)
    {
        if(wmEvent == BN_CLICKED)
        {
            file_creation_mode = 0;

            EnableWindow(GetDlgItem(hwnd,RB_UR), true);
            EnableWindow(GetDlgItem(hwnd,RB_UW), true);
            EnableWindow(GetDlgItem(hwnd,RB_UX), true);
            EnableWindow(GetDlgItem(hwnd,RB_GR), true);
            EnableWindow(GetDlgItem(hwnd,RB_GW), true);
            EnableWindow(GetDlgItem(hwnd,RB_GX), true);
            EnableWindow(GetDlgItem(hwnd,RB_OR), true);
            EnableWindow(GetDlgItem(hwnd,RB_OW), true);
            EnableWindow(GetDlgItem(hwnd,RB_OX), true);
    
            //MessageBox(NULL, "READ Clicked", TEXT("debug"), MB_OK);
        }
    }

    if(wmId == RB_WRITE)
    {
        if(wmEvent == BN_CLICKED)
        {
            file_creation_mode = 1;

            EnableWindow(GetDlgItem(hwnd,RB_UR), true);
            EnableWindow(GetDlgItem(hwnd,RB_UW), true);
            EnableWindow(GetDlgItem(hwnd,RB_UX), true);
            EnableWindow(GetDlgItem(hwnd,RB_GR), true);
            EnableWindow(GetDlgItem(hwnd,RB_GW), true);
            EnableWindow(GetDlgItem(hwnd,RB_GX), true);
            EnableWindow(GetDlgItem(hwnd,RB_OR), true);
            EnableWindow(GetDlgItem(hwnd,RB_OW), true);
            EnableWindow(GetDlgItem(hwnd,RB_OX), true);
            //EnableWindow(GetDlgItem(hwnd,ID_ETTEXT),true);
            //n_displayDialog(hwnd);
            //DialogBox((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(NOTEPAD), hwnd, (DLGPROC)n_MyDlgProc);
            //MessageBox(NULL, "WRITE Clicked", TEXT("debug"), MB_OK);
        }
    }

    int fd = -2;
    char fd_string[1024];
    int no_of_bytes_rw;
    int status_read;

    switch (LOWORD(wParam))
    {
        case PB_OK:
            
            GetDlgItemText(hwnd,ID_ETFILENAME,file_name,50);
            
            if(flag == 1)
            {
                fd = creat(file_name,file_creation_mode);
                if(fd == PERMISSION_ERROR)
                {
                    MessageBox(NULL,TEXT("File does not have creat permission."),TEXT("ERROR"),MB_ICONERROR);
                    resetDlg(hwnd,wParam);
                }
                else
                {
                    sprintf(fd_string,"File created fd = %d",fd);
                    MessageBox(NULL, fd_string,TEXT("Hurray") , MB_OK);
                    EndDialog(hwnd,wParam);
                }
            }
            else if(flag == 2)
            {
                fd = open(file_name,file_creation_mode);
                if(fd == FILE_PERMISSIONS_NOT_VALID)
                {
                    sprintf(fd_string,"File %s does not have open permission.",file_name);
                    MessageBox(NULL,fd_string,TEXT("ERROR"),MB_ICONERROR);
                    resetDlg(hwnd,wParam);
                }
                else if(fd == FILE_DOES_NOT_EXISTS)
                {
                    sprintf(fd_string,"File %s does not exits.",file_name);
                    MessageBox(NULL,fd_string,TEXT("ERROR"),MB_ICONERROR);
                    resetDlg(hwnd,wParam);
                }
                else
                {
                    sprintf(fd_string,"File %s opened. Fd = %d",file_name,fd);
                    MessageBox(NULL, fd_string,TEXT("Hurray") , MB_OK);
                    EndDialog(hwnd,wParam);
                }
            }
            else if(flag == 3)
            {
                fd = getFdFromFileName(file_name);
                if(fd == -1)
                {
                    MessageBox(NULL,TEXT("File does not exits."),TEXT("ERROR"),MB_ICONERROR);
                    resetDlg(hwnd,wParam);
                }
                else
                {
                    close(fd);
                    sprintf(fd_string,"File %s closed successfully!!",file_name);
                    MessageBox(NULL,fd_string,TEXT("Hurray"),MB_OK);
                    EndDialog(hwnd,wParam);
                }
            }
            else if(flag == 4)
            { 
                //EnableWindow(GetDlgItem(hwnd,ID_DATA),false);
                
                fd = getFdFromFileName(file_name);
                if(fd == -1)
                {
                    sprintf(fd_string,"File %s is not opened. Please call open system call first.",file_name);
                    MessageBox(NULL,fd_string,TEXT("ERROR"),MB_ICONERROR);
                    resetDlg(hwnd,wParam);
                }
                else if(fd == -2)
                {
                    sprintf(fd_string,"File %s does not exits.Please try another file name.",file_name);
                    MessageBox(NULL,fd_string,TEXT("ERROR"),MB_ICONERROR);
                    resetDlg(hwnd,wParam);
                }
                else
                {
                    no_of_bytes_rw = GetDlgItemInt(hwnd,ID_NO_OF_BYTES,NULL,TRUE);
                    if(no_of_bytes_rw < 0)
                    {
                        MessageBox(NULL,TEXT("Invalid no of bytes specified."),TEXT("ERROR"),MB_ICONERROR);
                        resetDlg(hwnd,wParam);
                    }
                    else
                    {
                        status_read = Read(fd,&buffer_string,no_of_bytes_rw);
                        if(status_read == -2)
                        {
                            MessageBox(NULL,TEXT("Invalid file name specified."),TEXT("ERROR"),MB_ICONERROR);
                            resetDlg(hwnd,wParam);
                        }
                        else if(status_read == -3)
                        {
                            sprintf(fd_string,"File %s does not have read permissions.",file_name);
                            MessageBox(NULL,fd_string,TEXT("ERROR"),MB_ICONERROR);
                            resetDlg(hwnd,wParam);
                        }
                        else if(status_read == -4)
                        {
                            sprintf(fd_string,"File %s is not opened in read mode.",file_name);
                            MessageBox(NULL,fd_string,TEXT("ERROR"),MB_ICONERROR);
                            resetDlg(hwnd,wParam);
                        }
                        else
                        {
                            fprintf(fp_log_file,"buffer string = %s\n",buffer_string);
                            fflush(fp_log_file);
                            MessageBox(NULL,TEXT("Data read successfully to file."),TEXT("Hurray"),MB_OK);
                            status = SetDlgItemText(hwnd,ID_DATA,buffer_string);

                            fprintf(fp_log_file,"status = %d\n",status);
                            fflush(fp_log_file);

                            //EnableWindow(GetDlgItem(hwnd,ID_DATA),true);

                            fprintf(fp_log_file,"Completed Read \n");
                            fflush(fp_log_file);
                        }
                    }
                }
            }
            else if(flag == 5)
            {
                EnableWindow(GetDlgItem(hwnd,ID_DATA),false);
                fd = getFdFromFileName(file_name);
                if(fd == -1)
                {
                    sprintf(fd_string,"File %s is not opened. Please call open system call first.",file_name);
                    MessageBox(NULL,fd_string,TEXT("ERROR"),MB_ICONERROR);
                    resetDlg(hwnd,wParam);
                }
                else if(fd == -2)
                {
                    sprintf(fd_string,"File %s does not exits.Please try another file name.",file_name);
                    MessageBox(NULL,fd_string,TEXT("ERROR"),MB_ICONERROR);
                    resetDlg(hwnd,wParam);
                }
                else
                {
                    no_of_bytes_rw = GetDlgItemInt(hwnd,ID_NO_OF_BYTES,NULL,TRUE);
            
                    if(no_of_bytes_rw < 0)
                    {
                        MessageBox(NULL,TEXT("Invalid no of bytes specified."),TEXT("ERROR"),MB_ICONERROR);
                        resetDlg(hwnd,wParam);
                    }
                    else
                    {
                        GetDlgItemText(hwnd,ID_DATA,buffer_string,(no_of_bytes_rw+1));
                        no_of_bytes_rw = Write(fd,buffer_string,(no_of_bytes_rw+1));
                        if(no_of_bytes_rw == -2)
                        {
                            MessageBox(NULL,TEXT("Invalid file name specified."),TEXT("ERROR"),MB_ICONERROR);
                            resetDlg(hwnd,wParam);
                        }
                        else if(no_of_bytes_rw == -3)
                        {
                            sprintf(fd_string,"File %s does not have write permissions.",file_name);
                            MessageBox(NULL,fd_string,TEXT("ERROR"),MB_ICONERROR);
                            resetDlg(hwnd,wParam);
                        }
                        else if(no_of_bytes_rw == -4)
                        {
                            sprintf(fd_string,"File %s is not opened in write mode.",file_name);
                            MessageBox(NULL,fd_string,TEXT("ERROR"),MB_ICONERROR);
                            resetDlg(hwnd,wParam);
                        }
                        else
                        {
                            MessageBox(NULL,TEXT("Data written successfully to file."),TEXT("Hurray"),MB_OK);
                            SetDlgItemText(hwnd,ID_DATA,buffer_string);
                            //EnableWindow(GetDlgItem(hwnd,ID_DATA),true);
                        }
                    }
                }
            }
            
            InvalidateRect(NULL,NULL,TRUE);
            break;

        case PB_RESET :
            resetDlg(hwnd,wParam); 
            InvalidateRect(NULL,NULL,TRUE);
            break;

        case PB_CLOSE:
            //MessageBox(NULL, file_name, TEXT("Matched PB_CLOSE"), MB_OK);
            EndDialog(hwnd, wParam);
            InvalidateRect(NULL,NULL,TRUE);
            break;
    } 
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static HBRUSH hBrush;
    switch (iMsg)
	{
        case WM_INITDIALOG:
            EnableWindow(GetDlgItem(hwnd,RB_UR),false);
            EnableWindow(GetDlgItem(hwnd,RB_UW),false);
            EnableWindow(GetDlgItem(hwnd,RB_UX),false);
            EnableWindow(GetDlgItem(hwnd,RB_GR),false);
            EnableWindow(GetDlgItem(hwnd,RB_GW),false);
            EnableWindow(GetDlgItem(hwnd,RB_GX),false);
            EnableWindow(GetDlgItem(hwnd,RB_OR),false);
            EnableWindow(GetDlgItem(hwnd,RB_OW),false);
            EnableWindow(GetDlgItem(hwnd,RB_OX),false);
            EnableWindow(GetDlgItem(hwnd,RB_READ),false);
            EnableWindow(GetDlgItem(hwnd,RB_WRITE),false);
            break;

        case WM_CTLCOLORDLG:
		case WM_CTLCOLORBTN:
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLORMSGBOX:
			SetBkMode((HDC)wParam, RGB(255, 255, 255));
			SetTextColor((HDC)wParam, RGB(0, 0, 128));

			hBrush = CreateSolidBrush(RGB(200, 200, 200));
			return (INT_PTR)hBrush;
			break;

        case WM_COMMAND:
            dlgCommand(hwnd,wParam,lParam);
            break;


    }    

    return FALSE;
}

void displayDlg(HWND hWnd)
{
    //LPCWSTR szAppName = TEXT("FileSystem");
    CreateWindow(TEXT("FileSystem"), TEXT("FileSystem"),  
	             WS_VISIBLE|WS_POPUP|WS_THICKFRAME, 
				 CW_USEDEFAULT, CW_USEDEFAULT, 
				 CW_USEDEFAULT, CW_USEDEFAULT, 
				 NULL, NULL, 
				 GetModuleHandle(NULL), NULL);
}

void resetDlg(HWND hwnd,WPARAM wParam)
{
    int i;

    for(int i = 0; i < 9; i++)
    {
        file_permissions[i] = 0;
    }

    EnableWindow(GetDlgItem(hwnd,RB_UR),false);
    EnableWindow(GetDlgItem(hwnd,RB_UW),false);
    EnableWindow(GetDlgItem(hwnd,RB_UX),false);
    EnableWindow(GetDlgItem(hwnd,RB_GR),false);
    EnableWindow(GetDlgItem(hwnd,RB_GW),false);
    EnableWindow(GetDlgItem(hwnd,RB_GX),false);
    EnableWindow(GetDlgItem(hwnd,RB_OR),false);
    EnableWindow(GetDlgItem(hwnd,RB_OW),false);
    EnableWindow(GetDlgItem(hwnd,RB_OX),false);
    EnableWindow(GetDlgItem(hwnd,RB_READ),false);
    EnableWindow(GetDlgItem(hwnd,RB_WRITE),false);

    switch(HIWORD(wParam))
    {	 
        case BN_CLICKED:        
            SetDlgItemText(hwnd, ID_ETFILENAME,TEXT(""));
            SetDlgItemText(hwnd, ID_NO_OF_BYTES,TEXT(""));
            SetDlgItemText(hwnd, ID_DATA,TEXT(""));
            //SetDlgItemText(hwnd, ID_ETTEXT,(""));

            BOOL checked = IsDlgButtonChecked(hwnd, RB_UR);
            if (checked) 
            {
                CheckDlgButton(hwnd,RB_UR, BST_UNCHECKED); 
            }
        
            checked = IsDlgButtonChecked(hwnd, RB_UW);
            if(checked) 
            { 
                CheckDlgButton(hwnd,RB_UW, BST_UNCHECKED); 
            }

            checked = IsDlgButtonChecked(hwnd, RB_UX);
            if 	(checked) 
            {
                CheckDlgButton(hwnd,RB_UX, BST_UNCHECKED); 
            }

            checked = IsDlgButtonChecked(hwnd, RB_GR);
            if (checked) 
            {
                CheckDlgButton(hwnd,RB_GR, BST_UNCHECKED); 
            }

            checked = IsDlgButtonChecked(hwnd, RB_GW);
            if (checked) 
            {
                CheckDlgButton(hwnd,RB_GW, BST_UNCHECKED); 
            }

            checked = IsDlgButtonChecked(hwnd, RB_GX);
            if (checked) 
            {
                CheckDlgButton(hwnd,RB_GX, BST_UNCHECKED);
            }

            checked = IsDlgButtonChecked(hwnd, RB_OR);
            if (checked) 
            {
                CheckDlgButton(hwnd,RB_OR, BST_UNCHECKED); 
            }

            checked = IsDlgButtonChecked(hwnd, RB_OW);
            if (checked) 
            {
                CheckDlgButton(hwnd,RB_OW, BST_UNCHECKED); 
            }

            checked = IsDlgButtonChecked(hwnd, RB_OX);
            if (checked) 
            {
                CheckDlgButton(hwnd,RB_OX, BST_UNCHECKED); 
            }

            checked = IsDlgButtonChecked(hwnd, RB_READ);
            if (checked) 
            {
                file_creation_mode = -1;
                CheckDlgButton(hwnd,RB_READ, BST_UNCHECKED); 
            }

            checked = IsDlgButtonChecked(hwnd, RB_WRITE);
            if (checked) 
            {
                file_creation_mode = -1;
                CheckDlgButton(hwnd,RB_WRITE, BST_UNCHECKED); 
            }

        break;
    }
}

void show_IIT(HDC hdc,float cxClient,float cyClient)
{
    iit_inode_t* inode = NULL;
    int i;
    char inode_no_in_string[20];
    char str[65] = "\0";
    int count = 0;

    for(i=0; i < MAX_IIT_SIZE;i++)
    {
        if(incore_inode_table[i] != NULL  )
        {
            if(incore_inode_table[i]->reference_count != 0)
            {
                inode = i_search_inode_in_freelist(i_p_freelist,incore_inode_table[i]->inode_number);
                if(inode != NULL)
                    continue;

                sprintf(inode_no_in_string,"%d   ",incore_inode_table[i]->inode_number);
                TextOut(hdc,cxClient/1.88,((((float)count+3.0)*cyClient)/30),inode_no_in_string,strlen(inode_no_in_string));
                count++;
                //TextOut(hdc,cxClient/2,cyClient/2,TEXT("HELLO"),strlen("HELLO"));
            }
        }
    }

    //TextOut(hdc,cxClient/3,1.5*cyClient/10,str,strlen(str));
}

void show_FT(HDC hdc,float cxClient,float cyClient)
{
    int i;
    char inode_no_in_string[10]="\0";
    char str[65] = "\0";
    int count = 0;
    int index_in_ufdt;

    /*for(i=0; i < MAX_FT_SIZE;i++)
    {
        if(ft[i] != NULL && ft[i]->iit_pointer != NULL)
        {
            //printf("%d      ",ft[i]->iit_pointer->inode_number);
            //fprintf(fp_log_file,"I = %d ft if block = %d\n",i,ft[i]->iit_pointer->inode_number);
            //fflush(fp_log_file);

            index_in_ufdt = find_inode_in_ufdt(ft[i]->iit_pointer->inode_number);
            if(index_in_ufdt == -1 )
                continue;
            
            sprintf(inode_no_in_string,"%d   ",ft[i]->iit_pointer->inode_number);
            //strcat(str,inode_no_in_string);
            TextOut(hdc,cxClient/2.35,((((float)count+3.0)*cyClient)/30),inode_no_in_string,strlen(inode_no_in_string));
            count++;
        }
    }*/

    //fprintf(fp_log_file,"In Show_FT");
    //fflush(fp_log_file);

    for(i=0;i < MAX_UFDT_SIZE;i++)
    {
        if(ufdt[i] != NULL && ufdt[i]->ft_pointer!= NULL && ufdt[i]->ft_pointer->iit_pointer!=NULL) //&& ufdt[i]->ft_pointer!= NULL && ufdt[i]->ft_pointer->iit_pointer!=NULL
        {
            sprintf(inode_no_in_string,"%d   ",ufdt[i]->ft_pointer->iit_pointer->inode_number);
            TextOut(hdc,cxClient/2.35,((((float)count+3.0)*cyClient)/30),inode_no_in_string,strlen(inode_no_in_string));
            count++;
        }
    }

    //fprintf(fp_log_file,"Show_FT completed");
    //fflush(fp_log_file);
}

void show_UFDT(HDC hdc,float cxClient,float cyClient)
{
    int i;
    char inode_no_in_string[5]="\0";
    char str[65] = "\0";
    int count =0;

    //fprintf(fp_log_file,"In Show_UFDT");
    //fflush(fp_log_file);

    for(i=0;i < MAX_UFDT_SIZE;i++)
    {
        if(ufdt[i] != NULL && ufdt[i]->ft_pointer!= NULL && ufdt[i]->ft_pointer->iit_pointer!=NULL) //&& ufdt[i]->ft_pointer!= NULL && ufdt[i]->ft_pointer->iit_pointer!=NULL
        {
            sprintf(inode_no_in_string,"%d   ",ufdt[i]->ft_pointer->iit_pointer->inode_number);
            //strcat(str,inode_no_in_string);
            TextOut(hdc,cxClient/3.25,((((float)count+3.0)*cyClient)/30),inode_no_in_string,strlen(inode_no_in_string));
            count++;
        }
    }

    //fprintf(fp_log_file,"Show_UFDT completed");
    //fflush(fp_log_file);
}

void show_Superblock(HDC hdc,float cxClient,float cyClient)
{
     //fprintf(fp_log_file,"In Show_Superblock");
     //fflush(fp_log_file);

    char str_size_inode[30], str_index_inode[30],str_array_of_inodes[50]="\0";
    char temp_str[50]="\0";
    char str_remembered_inode[40];
    char str_remembered_db[40];

    sprintf(str_size_inode,"\nSize of inode list = %d\n",super_block->size_of_inode_list);
    TextOut(hdc,2*cxClient/3,1.75*cyClient/10,str_size_inode,strlen(str_size_inode));

    sprintf(str_index_inode,"\nIndex of next inode in list = %d\n",super_block->index_to_next_free_inode);
    TextOut(hdc,2*cxClient/3,2.25*cyClient/10,str_index_inode,strlen(str_index_inode));

    int i;
    for(i=0;i<SUPER_BLOCK_LIST_SIZE;i++)
    {
        if(super_block->list_of_free_inodes[i] == -4)
        {
            sprintf(temp_str,"X ");    
        }
        else
        {
            sprintf(temp_str,"%d  ",super_block->list_of_free_inodes[i]);
        }
        strcat(str_array_of_inodes,temp_str);
    }

    char str[70] = "Array of free Inodes: "; 
    strcat(str,str_array_of_inodes);

    TextOut(hdc,2*cxClient/3,2.75*cyClient/10,str,strlen(str));

    char str_size_db[30], str_index_db[30],str_array_of_dbs[50]="\0";
    char temp_str_db[50]="\0";

    sprintf(str_size_db,"\nSize of data block list = %d\n",super_block->size_of_data_block);
    TextOut(hdc,2*cxClient/3,3.25*cyClient/10,str_size_db,strlen(str_size_db));

    sprintf(str_index_db,"\nIndex of next data block in list = %d\n",super_block->index_to_next_free_datablock);
    TextOut(hdc,2*cxClient/3,3.75*cyClient/10,str_index_db,strlen(str_index_db));

    for(i=0;i<SUPER_BLOCK_LIST_SIZE;i++)
    {
        if(super_block->list_of_free_datablocks[i] == -3)
        {
            sprintf(temp_str_db,"X ");
        }
        else
        {
            sprintf(temp_str_db,"%d ",super_block->list_of_free_datablocks[i]);
        }

        strcat(str_array_of_dbs,temp_str_db);
    }

    char str_db[70] = "Array of free Datablocks: "; 
    strcat(str_db,str_array_of_dbs);

    TextOut(hdc,2*cxClient/3,4.25*cyClient/10,str_db,strlen(str_db));

    sprintf(str_remembered_inode,"\nRemembered Inode = %d\n",super_block->remembered_inode);
    TextOut(hdc,2*cxClient/3,4.75*cyClient/10,str_remembered_inode,strlen(str_remembered_inode));

    sprintf(str_remembered_db,"\nRemembered Datablock = %d\n",super_block->remembered_datablock);
    TextOut(hdc,2*cxClient/3,5.25*cyClient/10,str_remembered_db,strlen(str_remembered_db));

    //fprintf(fp_log_file,"In Show_Superblock completed");
    //fflush(fp_log_file);
}

void show_freelist_buffer(HDC hdc,float cxClient,float cyClient)
{
    //fprintf(fp_log_file,"In show_freelist_buffer");
    //fflush(fp_log_file);

    char* str=NULL;
    char* temp_str = NULL;

    str = (char*)calloc(400,sizeof(char));
    assert(str!=NULL);

    node_t *p_run;

    temp_str = (char*)calloc(400,sizeof(char));
    assert(temp_str!=NULL);

    //TextOut(hdc,(0.5)*cxClient/20,6.3*cyClient/10,TEXT("BEG <-> "),strlen("BEG <-> "));
    strcpy(str,"[BEG]<->");
    for(p_run = p_freelist->buffer_header.next_buffer_on_freelist; p_run != p_freelist; p_run = p_run->buffer_header.next_buffer_on_freelist)
    {
        if(p_run->buffer_header.block_number == -1)
        {
            sprintf(temp_str,"x <-> ");
        }
        else
        {
            sprintf(temp_str,"%d <-> ",p_run->buffer_header.block_number);
        }
        
        strcat(str,temp_str);
    }

    strcat(str,"[END]");

    TextOut(hdc,(0.5)*cxClient/20,6.4*cyClient/10,str,strlen(str));
    free(str);
    str=NULL;

    free(temp_str);
    temp_str=NULL;

   // fprintf(fp_log_file,"show_freelist_buffer completed");
   // fflush(fp_log_file);
}

void show_hashqueue_buffer(HDC hdc,float cxClient,float cyClient)
{
    node_t *p_run;
    int i;
    hashqueueHeaders_t hashqueue ;

    char* str=NULL;
    char* temp_str = NULL;

   // fprintf(fp_log_file,"In show_hashqueue_buffer");
    //fflush(fp_log_file);

    str = (char*)calloc(400,sizeof(char));
    assert(str!=NULL);
    
    temp_str = (char*)calloc(400,sizeof(char));
    assert(temp_str!=NULL);
    
   for(i=0;i<4;i++)
    {
        hashqueue[i] = (*(p_hashqueue))[i];
        strcpy(str,"[BEG]<->");

        for(p_run = hashqueue[i]->buffer_header.next_buffer_on_hashqueue; p_run != hashqueue[i]; p_run = p_run->buffer_header.next_buffer_on_hashqueue)
        {
            sprintf(temp_str,"%d <->",p_run->buffer_header.block_number);
            strcat(str,temp_str);
        }

        strcat(str,"[END]");
        TextOut(hdc,(0.5)*cxClient/20,(i*0.04+1)*(8.5*cyClient/10),str,strlen(str));
    }

    free(str);
    str=NULL;

    free(temp_str);
    temp_str=NULL;

   // fprintf(fp_log_file,"show_hashqueue_buffer completed");
   // fflush(fp_log_file);
}

void show_freelist_inode(HDC hdc,float cxClient,float cyClient)
{
    char* str=NULL;
    char* temp_str = NULL;

    //fprintf(fp_log_file,"show_freelist_inode completed");
    //fflush(fp_log_file);

    str = (char*)calloc(400,sizeof(char));
    assert(str!=NULL);

    i_node_t *p_run;

    temp_str = (char*)calloc(400,sizeof(char));
    assert(temp_str!=NULL);

    strcpy(str,"[BEG]<->");
    
    for(p_run = i_p_freelist->next_inode_on_freelist; p_run != i_p_freelist; p_run = p_run->next_inode_on_freelist)
    {
        if(p_run->inode_number == -4)
        {
            sprintf(temp_str,"x <-> ");    
        }
        else
        {
            sprintf(temp_str,"%d <-> ",p_run->inode_number);
        }
        strcat(str,temp_str);
    }

    strcat(str,"[END]");
    TextOut(hdc,(0.5)*cxClient/20,7.4*cyClient/10,str,strlen(str));

    free(str);
    str=NULL;

    free(temp_str);
    temp_str=NULL;

     //fprintf(fp_log_file,"show_freelist_inode completed");
    //fflush(fp_log_file);
}

void show_hashqueue_inode(HDC hdc,float cxClient,float cyClient)
{
    i_node_t *p_run;
    int i;
    i_hashqueueHeaders_t hashqueue ;
    
    char* str=NULL;
    char* temp_str = NULL;

    //fprintf(fp_log_file,"In show_hashqueue_inode ");
    //fflush(fp_log_file);

    str = (char*)calloc(400,sizeof(char));
    assert(str!=NULL);
    
    temp_str = (char*)calloc(400,sizeof(char));
    assert(temp_str!=NULL);
    
   for(i=0;i<4;i++)
    {
        hashqueue[i] = (*(i_p_hashqueue))[i];
        strcpy(str,"[BEG]<->");

        for(p_run = hashqueue[i]->next_inode_on_hashqueue; p_run != hashqueue[i]; p_run = p_run->next_inode_on_hashqueue)
        {
            sprintf(temp_str,"%d <->",p_run->inode_number);
            strcat(str,temp_str);
        }

        strcat(str,"[END]");
        TextOut(hdc,(10)*cxClient/20,(i*0.04+1)*(8.5*cyClient/10),str,strlen(str));
    }

    free(str);
    str=NULL;

    free(temp_str);
    temp_str=NULL;

    //fprintf(fp_log_file,"show_hashqueue_inode completed");
    //fflush(fp_log_file);
}

LRESULT CALLBACK SplashWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int cxClient, cyClient, cxChar, cyChar, cxMouse,cyMouse;
	static int SPLASH_SCREEN_BG_DRAW = 0;
    static int SPALSH_SCREEN_COMPLETED = 0;
	
	HDC hdc; 
	HDC hdcComp;
	HPEN HLinePen;
	HINSTANCE hInst;
	HPEN HOldPen;
	HBRUSH HNewBrush;
	HBRUSH HOldBrush;
	HFONT HSplashScreenFont;
	HFONT HOldFont;
	
	static HBITMAP hbmap;
	HGDIOBJ prevHGDIObj = NULL;
	BITMAP bmBuffer;
	RECT rc;
	
	TEXTMETRIC tm; 
    PAINTSTRUCT ps; 
	int pointIndex;
	double point_distance;
	
    switch (iMsg)
	{
		case WM_CREATE:
			hbmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(MY_BITMAP_LOVELACE));
			SetTimer(hwnd, 1, 10, NULL); 
            hdc = GetDC(hwnd); 
            GetTextMetrics(hdc, &tm);
            cxChar = tm.tmAveCharWidth; 
            cyChar = tm.tmHeight + tm.tmExternalLeading; 	
			ReleaseDC(hwnd, hdc); 
            hdc = NULL; 
            
    	break;

        case WM_SIZE: 
            cxClient = LOWORD(lParam); 
            cyClient = HIWORD(lParam); 
                       
			SPLASH_SCREEN_X_SHIFT_INNER = (cxClient/2);  //Images's rect's (max x ) right position 
			SPLASH_SCREEN_Y_SHIFT_INNER = (cyClient/4);  //Images's rect's (max Y)  bottom position 
			
			SPLASH_SCREEN_X_SHIFT_OUTER = (cxClient/2);  //Images's rect's (max x ) right position 
			SPLASH_SCREEN_Y_SHIFT_OUTER = (cyClient/2)-50;  //Images's rect's (max Y)  bottom position 
			SPLASH_SCREEN_BG_DRAW = 1;
            break; 


		case WM_CHAR:
        case WM_KILLFOCUS:
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
            DestroyWindow(hwnd);

			ShowWindow(hMain,SW_MAXIMIZE);
            UpdateWindow(hMain);
            SetForegroundWindow(hMain); 
        
			break;

		case WM_PAINT: 
			hdc = BeginPaint(hwnd, &ps);
        	hdcComp = CreateCompatibleDC(hdc);
			prevHGDIObj = SelectObject(hdcComp, hbmap);
		    GetObject(hbmap, sizeof(BITMAP), &bmBuffer); 
		    GetClientRect(hwnd, &rc);  
		    BitBlt(hdc, (rc.right - bmBuffer.bmWidth)/2, (rc.bottom - bmBuffer.bmHeight - 100)/2, (int)bmBuffer.bmWidth , (int)bmBuffer.bmHeight, hdcComp, 0, 0, SRCCOPY); 
		    SPLASH_SCREEN_BG_DRAW=0;
	        HLinePen=CreatePen(PS_SOLID, 5, RGB(255, 165, 0));
		    HOldPen=(HPEN)SelectObject(hdc,HLinePen);
		    for(pointIndex=0; pointIndex < point_drawn ; pointIndex++)  
		    {
            	if(pointIndex!=0)
				{
					MoveToEx(hdc,p_vector->pp_point[pointIndex-1]->x_position+SPLASH_SCREEN_X_SHIFT_OUTER,p_vector->pp_point[pointIndex-1]->y_position+SPLASH_SCREEN_Y_SHIFT_OUTER,NULL);
					LineTo(hdc,p_vector->pp_point[pointIndex]->x_position+SPLASH_SCREEN_X_SHIFT_OUTER,p_vector->pp_point[pointIndex]->y_position+SPLASH_SCREEN_Y_SHIFT_OUTER);
				}
					
		    }
		
		    if(point_drawn == p_vector->count ) 
		    {
				SetTextAlign(hdc, TA_CENTER | TA_TOP); 
				SetBkMode(hdc,TRANSPARENT);
				SetTextColor(hdc,RGB(255, 165, 0));
				HSplashScreenFont = CreateFont(50, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Cambria"));
				HOldFont = (HFONT)SelectObject(hdc, HSplashScreenFont);
				TextOut(hdc, SPLASH_SCREEN_X_SHIFT_OUTER, SPLASH_SCREEN_Y_SHIFT_OUTER + 120 + 20, "Lady Ada Lovelace", lstrlen("Lady Ada Lovelace"));
			}
		
			SelectObject(hdcComp, prevHGDIObj);
			DeleteDC(hdcComp);
			EndPaint(hwnd, &ps);
			break;

     case WM_TIMER:
		
		if(point_drawn != p_vector->count) 
		{
			point_drawn++;	
			InvalidateRect(hwnd, NULL, FALSE);
        }

		break;
		
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

vector_t* create_vector(void)
{
    vector_t* p_vector=NULL;
    p_vector=(vector_t*)malloc(sizeof(vector_t));
    p_vector->pp_point=NULL;
    p_vector->count=0;
    return p_vector;
}

void destroy_vector(vector_t** pp_vector)
{
    int i=0;
    for (i=0;i<(*pp_vector)->count;i++)
    {
        free((*pp_vector)->pp_point[i]);
		(*pp_vector)->pp_point[i]=NULL;
    }
    free((*pp_vector)->pp_point);
    (*pp_vector)->pp_point=NULL;
    (*pp_vector)->count=0;
    free(*pp_vector);
    *pp_vector=NULL;
}

int push(vector_t *p_vector,point_t *p_point)
{
    p_vector->count+=1;
    p_vector->pp_point=(point_t**)realloc(p_vector->pp_point,p_vector->count*sizeof(point_t*));
	
    p_vector->pp_point[p_vector->count-1] = p_point;
	
    return (EXIT_SUCCESS);
}

point_t *create_point(int xPos,int yPos)
{
    point_t* p_point=NULL;
	p_point=(point_t*)malloc(sizeof(point));
    p_point->x_position=xPos;
    p_point->y_position=yPos;
    return p_point;
}

void draw_rectangle(vector_t *p_vector)
{
    point_t *p_point;
	int i;

	for(i=-112;i<=112;i+=6)
    {
		p_point=create_point(i,-121);     
		push(p_vector,p_point);
    }

	for(i=-121;i<=121;i+=6)
    {
		p_point=create_point(110,i);
		push(p_vector,p_point);
    }

	for(i=110;i>=-112;i-=6)
    {
		p_point=create_point(i,120);
		push(p_vector,p_point);
    }

	for(i=120;i>=-121;i-=6)
    {
		p_point=create_point(-112,i);
		push(p_vector,p_point);
    }
}


void deinitialize(void)
{
    //MessageBox(NULL,TEXT("Deinitialize"),TEXT("NM"),MB_OK);

    fprintf(fp_log_file,"In deinitialize");
    fflush(fp_log_file);

    free(buffer_string);
    free(logs);

    superblk_disk_write(super_block);

    destroy_superblk(super_block);

	//Write dilb to disk
	dilb_disk_write(dilb);

	//Destroy buffer freelist and hashqueue
	destroy_freelist(p_freelist);
	destroy_hashqueue(p_hashqueue);
	
	//Destroy inode freelist and hashqueue
	i_destroy_freelist(&i_p_freelist);
	i_destroy_hashqueue(&i_p_hashqueue);

	//Destroy dilb
	destroy_dilb(dilb);

	//Write file disk in memory
	hdd_disk_write(hdd);

	//Destroy in memory hdd
	destroy_in_memory_hdd();

    //fp_log_file = fopen("Log.txt","w");

    fprintf(fp_log_file,"deinitialize completed");
    fflush(fp_log_file);

    fclose(fp_log_file);
    fp_log_file = NULL;

}

LRESULT CALLBACK CreditsWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{ 
    static int cxClient, cyClient, cxChar, cyChar, cxMouse,cyMouse;
	static int SPLASH_SCREEN_BG_DRAW=0;
    static int SPALSH_SCREEN_COMPLETED=0;
	
	HDC hdc; 
	HDC hdcComp;
	HPEN HLinePen;
	HINSTANCE hInst;
	HPEN HOldPen;
	HBRUSH HNewBrush;
	HBRUSH HOldBrush;
	HFONT HSplashScreenFont;
	HFONT HOldFont;
	
	static HBITMAP hbmap;
	HGDIOBJ prevHGDIObj = NULL;
	BITMAP bmBuffer;
	RECT rc;
	
	
    COLORREF color;
	
	TEXTMETRIC tm; 
    PAINTSTRUCT ps; 
	int pointIndex;
	//RECT Cliet_Rect;
	double point_distance;
	
    switch (iMsg)
	{
		// very 1st Msg
	case WM_CREATE:
			hbmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(MY_BITMAP));
            hdc = GetDC(hWnd); 
            GetTextMetrics(hdc, &tm);
            cxChar = tm.tmAveCharWidth; 
            cyChar = tm.tmHeight + tm.tmExternalLeading; 	
			ReleaseDC(hWnd, hdc); 
            hdc = NULL; 
    	break;

        case WM_SIZE: 
            cxClient = LOWORD(lParam); 
            cyClient = HIWORD(lParam); 
           
					
			SPLASH_SCREEN_X_SHIFT_INNER_CREDIT=(cxClient/2)-(564/2);  //Images's rect's (max x ) right position was 312
			SPLASH_SCREEN_Y_SHIFT_INNER_CREDIT=(cyClient/2)-(339/2) ;  //Images's rect's (max Y)  bottom position was 564
			
			SPLASH_SCREEN_X_SHIFT_OUTER_CREDIT=(cxClient/2);  //Images's rect's (max x ) right position was 312
			SPLASH_SCREEN_Y_SHIFT_OUTER_CREDIT=(cyClient/8);  //Images's rect's (max Y)  bottom position was 564
			SPLASH_SCREEN_BG_DRAW=1;
        break; 


        case WM_MOUSEMOVE:
            cxMouse = LOWORD(lParam);
            cyMouse = HIWORD(lParam);
			CLIENT_XMouse=cxMouse;
			CLIENT_YMouse=cyMouse;
        break;

		case WM_KEYDOWN:

			if(wParam == VK_ESCAPE )
			{
				PostQuitMessage(0);
			}
		break;
        case WM_PAINT: 
		
		hdc = BeginPaint(hWnd, &ps);
        hdcComp = CreateCompatibleDC(hdc);
		prevHGDIObj = SelectObject(hdcComp, hbmap);// performs bit block transfer from memory DC to hdc returned from BeginPaint
		GetObject(hbmap, sizeof(BITMAP), &bmBuffer); // retrieves dimension of a bitmap
		GetClientRect(hWnd, &rc);  // fill the x,y, width, height into rectangle
		BitBlt(hdc, (rc.right - bmBuffer.bmWidth)/2, ((rc.bottom - bmBuffer.bmHeight)/8)-50, (int)bmBuffer.bmWidth, (int)bmBuffer.bmHeight, hdcComp, 0, 0, SRCCOPY); 
		SPLASH_SCREEN_BG_DRAW=0;
	    HLinePen = CreatePen(PS_SOLID,4,orange);
		HOldPen=(HPEN)SelectObject(hdc,HLinePen);
		for(pointIndex=0; pointIndex < point_drawn_credit ; pointIndex++)  //Draws the circular bar near aound logo image
		{
            		if(pointIndex!=0)
					{
							MoveToEx(hdc,p_vector_circle->pp_point[pointIndex-1]->x_position+SPLASH_SCREEN_X_SHIFT_OUTER_CREDIT,p_vector_circle->pp_point[pointIndex-1]->y_position+SPLASH_SCREEN_Y_SHIFT_OUTER_CREDIT,NULL);
							LineTo(hdc,p_vector_circle->pp_point[pointIndex]->x_position+SPLASH_SCREEN_X_SHIFT_OUTER_CREDIT,p_vector_circle->pp_point[pointIndex]->y_position+SPLASH_SCREEN_Y_SHIFT_OUTER_CREDIT);
					}
					
		}
		if( point_drawn_credit == p_vector_circle->count ) //this means the circile drawing has completed in the splash screen 
		{
					SetTextAlign(hdc, TA_CENTER | TA_TOP); 
					SetBkMode(hdc,TRANSPARENT);
					SetTextColor(hdc,orange);
					
					HSplashScreenFont=CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Cambria"));
					HOldFont=(HFONT)SelectObject(hdc, HSplashScreenFont);
					TextOut(hdc, SPLASH_SCREEN_X_SHIFT_OUTER_CREDIT, SPLASH_SCREEN_Y_SHIFT_OUTER_CREDIT + SPLASH_SCREEN_CIRCLE_RADIUS + 20, "CoreCode Programming Academy", lstrlen("CoreCode  Programming Accedemy"));                    
        }
        
        if(flag_credits == ID_TIMER_CREDITS)
        {
                    SetTextAlign(hdc, TA_CENTER | TA_TOP); 
                    SetBkMode(hdc,TRANSPARENT);
                    SetTextColor(hdc,yellow);
                        
                    HSplashScreenFont=CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Cambria"));
                    HOldFont=(HFONT)SelectObject(hdc, HSplashScreenFont);
                    TextOut(hdc, SPLASH_SCREEN_X_SHIFT_OUTER_CREDIT, SPLASH_SCREEN_Y_SHIFT_OUTER_CREDIT + SPLASH_SCREEN_CIRCLE_RADIUS + 100, "CREDITS", lstrlen("CREDITS"));
        }

        if(flag_Tanvi == ID_TIMER_TANVI)
        {
                    SetTextAlign(hdc, TA_CENTER | TA_TOP); 
					SetBkMode(hdc,TRANSPARENT);
					SetTextColor(hdc,red);
					
					HSplashScreenFont=CreateFont(50, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Cambria"));
					HOldFont=(HFONT)SelectObject(hdc, HSplashScreenFont);
					TextOut(hdc, SPLASH_SCREEN_X_SHIFT_OUTER_CREDIT, SPLASH_SCREEN_Y_SHIFT_OUTER_CREDIT + SPLASH_SCREEN_CIRCLE_RADIUS + 140, "Tanvi Arawkar", lstrlen("Tanvi Arawkar"));  
        }

         if(flag_Apurva == ID_TIMER_APURVA )
         {
                // Apurva
                    SetTextAlign(hdc, TA_LEFT | TA_TOP); 
					SetBkMode(hdc,TRANSPARENT);
					SetTextColor(hdc,orange);
					
					HSplashScreenFont=CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Cambria"));
					HOldFont=(HFONT)SelectObject(hdc, HSplashScreenFont);
					TextOut(hdc, SPLASH_SCREEN_X_SHIFT_OUTER_CREDIT - 500, SPLASH_SCREEN_Y_SHIFT_OUTER_CREDIT + SPLASH_SCREEN_CIRCLE_RADIUS + 220, "Apurva Kukade", lstrlen("Apurva Kukade"));
         }
        
         if(flag_Manali == ID_TIMER_MANALI)
         {
                // //Manali Jain   
                    SetTextAlign(hdc, TA_LEFT | TA_TOP); 
					SetBkMode(hdc,TRANSPARENT);
					SetTextColor(hdc,orange);
					
					HSplashScreenFont=CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Cambria"));
					HOldFont=(HFONT)SelectObject(hdc, HSplashScreenFont);
					TextOut(hdc, SPLASH_SCREEN_X_SHIFT_OUTER_CREDIT + 300, SPLASH_SCREEN_Y_SHIFT_OUTER_CREDIT + SPLASH_SCREEN_CIRCLE_RADIUS + 220, "Manali Jain", lstrlen("Manali Jain"));  
         }

         if(flag_Monika == ID_TIMER_MONIKA) 
         {
                  // // Monika Patil
                   SetTextAlign(hdc, TA_LEFT | TA_TOP); 
					SetBkMode(hdc,TRANSPARENT);
					SetTextColor(hdc,orange);
					
					HSplashScreenFont=CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Cambria"));
					HOldFont=(HFONT)SelectObject(hdc, HSplashScreenFont);
					TextOut(hdc, SPLASH_SCREEN_X_SHIFT_OUTER_CREDIT - 500, SPLASH_SCREEN_Y_SHIFT_OUTER_CREDIT + SPLASH_SCREEN_CIRCLE_RADIUS + 300, "Monika Patil", lstrlen("Monika Patil"));


         }

         if(flag_Nikita == ID_TIMER_NIKITA)
          {
                 // Nikta Sonsale
                    SetTextAlign(hdc, TA_LEFT | TA_TOP); 
					SetBkMode(hdc,TRANSPARENT);
					SetTextColor(hdc,orange);
					
					HSplashScreenFont=CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Cambria"));
					HOldFont=(HFONT)SelectObject(hdc, HSplashScreenFont);
					TextOut(hdc, SPLASH_SCREEN_X_SHIFT_OUTER_CREDIT + 300, SPLASH_SCREEN_Y_SHIFT_OUTER_CREDIT + SPLASH_SCREEN_CIRCLE_RADIUS + 300, "Nikita Sonsale", lstrlen("Nikita Sonsale"));
          }
          if(flag_Ravina == ID_TIMER_RAVINA)
          {
                    // // Ravina Jain
                    SetTextAlign(hdc, TA_LEFT | TA_TOP); 
					SetBkMode(hdc,TRANSPARENT);
					SetTextColor(hdc,white);
					
					HSplashScreenFont=CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Cambria"));
					HOldFont=(HFONT)SelectObject(hdc, HSplashScreenFont);					
					TextOut(hdc, SPLASH_SCREEN_X_SHIFT_OUTER_CREDIT - 500, SPLASH_SCREEN_Y_SHIFT_OUTER_CREDIT + SPLASH_SCREEN_CIRCLE_RADIUS + 380, "Ravina Jain", lstrlen("Ravina Jain"));    
              

          }

          if(flag_Renuka == ID_TIMER_RENUKA)
          {
                   //Renuka Hajare
                    SetTextAlign(hdc, TA_LEFT | TA_TOP); 
					SetBkMode(hdc,TRANSPARENT);
					SetTextColor(hdc,white);
					
					HSplashScreenFont=CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Cambria"));
					HOldFont=(HFONT)SelectObject(hdc, HSplashScreenFont);					
					TextOut(hdc, SPLASH_SCREEN_X_SHIFT_OUTER_CREDIT + 300, SPLASH_SCREEN_Y_SHIFT_OUTER_CREDIT + SPLASH_SCREEN_CIRCLE_RADIUS + 380, "Renuka Hajare", lstrlen("Renuka Hajare"));
          }
          if(flag_Sadhana == ID_TIMER_SADHANA)
          {
                     // // Sadhana Kulkarni
                   SetTextAlign(hdc, TA_LEFT | TA_TOP); 
					SetBkMode(hdc,TRANSPARENT);
					SetTextColor(hdc,white);
					
					HSplashScreenFont=CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Cambria"));
					HOldFont=(HFONT)SelectObject(hdc, HSplashScreenFont);					
					TextOut(hdc, SPLASH_SCREEN_X_SHIFT_OUTER_CREDIT - 500, SPLASH_SCREEN_Y_SHIFT_OUTER_CREDIT + SPLASH_SCREEN_CIRCLE_RADIUS + 460, "Sadhana Kulkarni", lstrlen("Sadhana KulKarni"));
          }
          if(flag_samruddhi == ID_TIMER_SAMRUDDHI)
          {
                    // Samruddhi Dhadphale
                    SetTextAlign(hdc, TA_LEFT | TA_TOP); 
					SetBkMode(hdc,TRANSPARENT);
					SetTextColor(hdc,white);
					
					HSplashScreenFont=CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Cambria"));
					HOldFont=(HFONT)SelectObject(hdc, HSplashScreenFont);					
					TextOut(hdc, SPLASH_SCREEN_X_SHIFT_OUTER_CREDIT + 300, SPLASH_SCREEN_Y_SHIFT_OUTER_CREDIT + SPLASH_SCREEN_CIRCLE_RADIUS + 460, "Samruddhi Dhadphale", lstrlen("Samruddhi Dhadphale"));
          }
          if(flag_Shraddha == ID_TIMER_SHRADDHA)
          {
                     // // Shraddha Gaikar
                    SetTextAlign(hdc, TA_LEFT | TA_TOP); 
					SetBkMode(hdc,TRANSPARENT);
					SetTextColor(hdc,green);
					
					HSplashScreenFont=CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Cambria"));
					HOldFont=(HFONT)SelectObject(hdc, HSplashScreenFont);					
					TextOut(hdc, SPLASH_SCREEN_X_SHIFT_OUTER_CREDIT - 500, SPLASH_SCREEN_Y_SHIFT_OUTER_CREDIT + SPLASH_SCREEN_CIRCLE_RADIUS + 540, "Shraddha Gaikar", lstrlen("Shraddha Gaikar"));
          }

          if(flag_ShraddhaM == ID_TIMER_SHRADDHAM)
          {
                   // // Shraddha Magar
                    SetTextAlign(hdc, TA_LEFT | TA_TOP); 
					SetBkMode(hdc,TRANSPARENT);
					SetTextColor(hdc,green);
					
					HSplashScreenFont=CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Cambria"));
					HOldFont=(HFONT)SelectObject(hdc, HSplashScreenFont);					
					TextOut(hdc, SPLASH_SCREEN_X_SHIFT_OUTER_CREDIT -500, SPLASH_SCREEN_Y_SHIFT_OUTER_CREDIT + SPLASH_SCREEN_CIRCLE_RADIUS + 620, "Shraddha Magar", lstrlen("Shraddha Magar"));

          }

          if(flag_Vaishnavi == ID_TIMER_VAISHNAVI)
          {
                     // // Vaishnavi 
                    SetTextAlign(hdc, TA_LEFT | TA_TOP); 
					SetBkMode(hdc,TRANSPARENT);
					SetTextColor(hdc,green);
					
					HSplashScreenFont=CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Cambria"));
					HOldFont=(HFONT)SelectObject(hdc, HSplashScreenFont);
					TextOut(hdc, SPLASH_SCREEN_X_SHIFT_OUTER_CREDIT +300, SPLASH_SCREEN_Y_SHIFT_OUTER_CREDIT + SPLASH_SCREEN_CIRCLE_RADIUS + 540, "Vaishnavi Tamhankar", lstrlen("Vaishnavi Tamhankar"));
          }

          if(flag_Kshitija == ID_TIMER_KSHITIJA)
          {
                     // // Vaishnavi 
                    SetTextAlign(hdc, TA_LEFT | TA_TOP); 
					SetBkMode(hdc,TRANSPARENT);
					SetTextColor(hdc,green);
					
					HSplashScreenFont=CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Cambria"));
					HOldFont=(HFONT)SelectObject(hdc, HSplashScreenFont);
					TextOut(hdc, SPLASH_SCREEN_X_SHIFT_OUTER_CREDIT +300, SPLASH_SCREEN_Y_SHIFT_OUTER_CREDIT + SPLASH_SCREEN_CIRCLE_RADIUS + 620, "Kshitija Deshpande", lstrlen("Kshitija Deshpande"));
          }

		SelectObject(hdcComp, prevHGDIObj);
		DeleteDC(hdcComp);
		EndPaint(hWnd, &ps);
	break;

    case WM_TIMER:

		if( point_drawn_credit != p_vector_circle->count) 
		{
			point_drawn_credit ++;	
			InvalidateRect(hWnd, NULL, FALSE);
        }

        if(flag_credits != ID_TIMER_CREDITS)
        {
            flag_credits++;
            InvalidateRect(hWnd, NULL, FALSE);

        }
        if(flag_Tanvi != ID_TIMER_TANVI)
        {
            flag_Tanvi++;
            InvalidateRect(hWnd, NULL, FALSE);

        }

        if(flag_Apurva != ID_TIMER_APURVA)
        {
            flag_Apurva++;
            InvalidateRect(hWnd, NULL, FALSE);
        }

        if(flag_Manali != ID_TIMER_MANALI)
        {
            flag_Manali++;
            InvalidateRect(hWnd, NULL, FALSE);
        }

        if(flag_Monika != ID_TIMER_MONIKA)
        {
            flag_Monika++;
            InvalidateRect(hWnd, NULL, FALSE);
        }

        if(flag_Nikita != ID_TIMER_NIKITA)
        {
            flag_Nikita++;
             InvalidateRect(hWnd, NULL, FALSE);
        }

         if(flag_Ravina != ID_TIMER_RAVINA)
        {
            flag_Ravina++;
            InvalidateRect(hWnd, NULL, FALSE);
        }

        if(flag_Renuka != ID_TIMER_RENUKA)
        {
            flag_Renuka++;
            InvalidateRect(hWnd, NULL, FALSE);
        }
        if(flag_Sadhana != ID_TIMER_SADHANA)
        {
            flag_Sadhana++;
            InvalidateRect(hWnd, NULL, FALSE);
        }

        if(flag_samruddhi != ID_TIMER_SAMRUDDHI)
        {
            flag_samruddhi++;
             InvalidateRect(hWnd, NULL, FALSE);
        }

        if(flag_Shraddha != ID_TIMER_SHRADDHA)
        {
            flag_Shraddha++;
             InvalidateRect(hWnd, NULL, FALSE);
        }

         if(flag_ShraddhaM != ID_TIMER_SHRADDHAM)
        {
            flag_ShraddhaM++;
            // flag_Vaishnavi = flag_Vaishnavi +1;
             InvalidateRect(hWnd, NULL, FALSE);
        }

        if(flag_Vaishnavi != ID_TIMER_VAISHNAVI)
        {
            flag_Vaishnavi++;
            InvalidateRect(hWnd, NULL, FALSE);
        }

        if(flag_Kshitija != ID_TIMER_KSHITIJA)
        {
            flag_Kshitija++;
            InvalidateRect(hWnd, NULL, FALSE);
        }

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	// 12
	return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}

void create_circle_points(vector_t *p_vector_circle,int radius)
{
    point_t *p_point;
    int angle_deg;
    int point_x,point_y;
    float angle_rad;

    for(angle_deg=0;angle_deg>=-360;angle_deg-=1)
    {
        angle_rad=M_PI/180*angle_deg;
		point_x= cos(angle_rad) * (radius);
		point_y= sin(angle_rad) * (radius);

        p_point = create_point(point_x,point_y);
        push(p_vector_circle,p_point);   
    }
}