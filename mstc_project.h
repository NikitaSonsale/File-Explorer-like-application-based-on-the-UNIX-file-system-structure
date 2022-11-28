#include<windows.h>

#define RADIO_BUTTON_CREAT 1
#define RADIO_BUTTON_OPEN 2
#define RADIO_BUTTON_CLOSE 3
#define RADIO_BUTTON_READ 4
#define RADIO_BUTTON_WRITE 5
#define RADIO_BUTTON_LOGS 6
#define RADIO_BUTTON_END 7

typedef struct point
{
    int x_position;
    int y_position; 
}point_t;

typedef struct vector
{
    point_t** pp_point;
	int count;
}vector_t;

int CLIENT_XArea,CLIENT_YArea;
int CLIENT_XMouse,CLIENT_YMouse;

HWND hSplash;
HWND hMain;
HWND hwnd_credits;

COLORREF red=RGB(255,0,0); 
COLORREF green=RGB(0,255,0); 
COLORREF blue=RGB(0,0,255); 
COLORREF orange=RGB(255,165,0); 
COLORREF light_orange=RGB(240,173,33);
COLORREF cyan=RGB(0,255,255); 
COLORREF magenta=RGB(255,0,255); 
COLORREF yellow=RGB(255,215,0); 
COLORREF skyblue=RGB(0,191,255); 
COLORREF darkgrey=RGB(105,105,105);
COLORREF grey    =RGB(128,128,128);
COLORREF white = RGB(255,255,255);

int flag_credits = 0;
int flag_Tanvi = 0;
int flag_Nikita = 0;
int flag_Shraddha = 0;
int flag_Ravina = 0;
int flag_Renuka = 0;
int flag_ShraddhaM = 0;
int flag_Apurva = 0;
int flag_Manali = 0;
int flag_Monika = 0;
int flag_Vaishnavi = 0;
int flag_samruddhi =0;
int flag_Sadhana = 0;
int flag_Kshitija = 0;

int ID_TIMER_CREDITS = 420;
int ID_TIMER_TANVI = ID_TIMER_CREDITS + 70;
int ID_TIMER_APURVA = ID_TIMER_TANVI +120;
int ID_TIMER_MANALI = ID_TIMER_APURVA + 100;
int ID_TIMER_MONIKA = ID_TIMER_MANALI + 70;
int ID_TIMER_NIKITA = ID_TIMER_MONIKA + 70;
int ID_TIMER_RAVINA = ID_TIMER_NIKITA +70;
int ID_TIMER_RENUKA  = ID_TIMER_RAVINA + 70;
int ID_TIMER_SADHANA  = ID_TIMER_RENUKA+70;
int ID_TIMER_SAMRUDDHI =ID_TIMER_SADHANA +70;
int ID_TIMER_SHRADDHA = ID_TIMER_SAMRUDDHI +70;
int ID_TIMER_VAISHNAVI = ID_TIMER_SHRADDHA +70;
int ID_TIMER_SHRADDHAM = ID_TIMER_VAISHNAVI +70; 
int ID_TIMER_KSHITIJA = ID_TIMER_SHRADDHAM +70; 

vector_t *p_vector = NULL;
vector_t *p_vector_circle = NULL;
int point_drawn;
int point_drawn_credit;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SplashWndProc(HWND hWnd,UINT uMsg, WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK CreditsWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK Parent_WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
void registerDlgClass(HINSTANCE hInstance);
void displayDlg(HWND hDlg);

void resetDlg(HWND hwnd,WPARAM wParam);

void initialize(void);
void deinitialize(void);
void display(HWND hwnd,HDC hdc,PAINTSTRUCT ps,float cxClient,float cyClient);

void show_IIT(HDC hdc,float cxClient,float cyClient);
void show_FT(HDC hdc,float cxClient,float cyClient);
void show_UFDT(HDC hdc,float cxClient,float cyClient);

void show_Superblock(HDC hdc,float cxClient,float cyClient);
void show_freelist_buffer(HDC hdc,float cxClient,float cyClient);
void show_hashqueue_buffer(HDC hdc,float cxClient,float cyClient);
void show_freelist_inode(HDC hdc,float cxClient,float cyClient);
void show_hashqueue_inode(HDC hdc,float cxClient,float cyClient);


/* VECTOR */
vector_t* create_vector(void);
int push(vector_t* p_vector,point_t* p_point);
void destroy_vector(vector_t **pp_vector);

void draw_rectangle(vector_t *p_vector);
point_t* create_point(int xPos,int yPos);

void create_circle_points(vector_t *p_vector_circle,int radius);
