#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <cstdio>
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"comdlg32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"user32.lib")

using namespace std;

// ===================== IDs =====================
#define ID_TAB              10
// Tab Ma hoa
#define ID_ENC_INPUT        101
#define ID_ENC_KEY          102
#define ID_ENC_RESULT       103
#define ID_ENC_FORMAT       104
#define ID_ENC_BTN_ENCRYPT  105
#define ID_ENC_BTN_CLEAR    106
#define ID_ENC_BTN_SAVERES  107
#define ID_ENC_BTN_LOADTXT  108
#define ID_ENC_BTN_SAVETXT  109
#define ID_ENC_BTN_GENKEY   110
#define ID_ENC_BTN_SAVEKEY  111
#define ID_ENC_BTN_LOADKEY  112
#define ID_ENC_LBL_KEYSTAT  113
// Tab Giai ma
#define ID_DEC_INPUT        201
#define ID_DEC_KEY          202
#define ID_DEC_RESULT       203
#define ID_DEC_FORMAT       204
#define ID_DEC_BTN_DECRYPT  205
#define ID_DEC_BTN_CLEAR    206
#define ID_DEC_BTN_SAVERES  207
#define ID_DEC_BTN_LOADTXT  208
#define ID_DEC_BTN_LOADKEY  209
#define ID_DEC_LBL_KEYSTAT  210

// ===================== BANG HOAN VI DES =====================
int IP[64]={58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7};
int IP_INV[64]={40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,34,2,42,10,50,18,58,26,33,1,41,9,49,17,57,25};
int E_TBL[48]={32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,12,13,14,15,16,17,16,17,18,19,20,21,20,21,22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1};
int P_TBL[32]={16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25};
int PC1[56]={57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,44,36,63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4};
int PC2[48]={14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32};
int SHIFT[16]={1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
int SBOX[8][4][16]={
{{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
{{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},
{{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},
{{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},
{{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
{{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},
{{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},
{{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}
};

// ===================== DES CORE =====================
string permute(const string& in,int* t,int n){string r(n,'0');for(int i=0;i<n;i++)r[i]=in[t[i]-1];return r;}
string lsh(const string& k,int n){return k.substr(n)+k.substr(0,n);}
string xorS(const string& a,const string& b){string r(a.size(),'0');for(size_t i=0;i<a.size();i++)r[i]=(a[i]==b[i])?'0':'1';return r;}
string s2b(const string& s){string r="";for(unsigned char c:s){for(int i=7;i>=0;i--)r+=(char)('0'+((c>>i)&1));}return r;}
string b2s(const string& b){string r="";for(size_t i=0;i+8<=b.size();i+=8){int v=0;for(int j=0;j<8;j++)v=(v<<1)+(b[i+j]-'0');r+=(char)v;}return r;}
string b2b64(const string& b){
    const string C="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    string p=b;while(p.size()%6)p+='0';string r="";
    for(size_t i=0;i<p.size();i+=6){int v=0;for(int j=0;j<6;j++)v=(v<<1)+(p[i+j]-'0');r+=C[v];}
    while(r.size()%4)r+='=';return r;
}
string b2hex(const string& b){
    string r="";
    for(size_t i=0;i<b.size();i+=8){int v=0;for(int j=0;j<8;j++)v=(v<<1)+(b[i+j]-'0');char buf[3];sprintf(buf,"%02X",v);r+=buf;}
    return r;
}
string b64_2b(const string& s){
    const string C="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    string r="";for(char c:s){if(c=='=')break;int idx=(int)C.find(c);if(idx<0)continue;for(int i=5;i>=0;i--)r+=(char)('0'+((idx>>i)&1));}return r;
}
string hex2b(const string& h){
    string r="";
    for(size_t i=0;i+1<h.size();i+=2){
        int hi=(h[i]>='0'&&h[i]<='9')?h[i]-'0':(h[i]>='A'&&h[i]<='F')?h[i]-'A'+10:(h[i]>='a'&&h[i]<='f')?h[i]-'a'+10:0;
        int lo=(h[i+1]>='0'&&h[i+1]<='9')?h[i+1]-'0':(h[i+1]>='A'&&h[i+1]<='F')?h[i+1]-'A'+10:(h[i+1]>='a'&&h[i+1]<='f')?h[i+1]-'a'+10:0;
        int v=(hi<<4)|lo;
        for(int j=7;j>=0;j--)r+=(char)('0'+((v>>j)&1));
    }
    return r;
}
string funcF(const string& R,const string& K){
    string ex=permute(R,E_TBL,48);string xo=xorS(ex,K);string so="";
    for(int i=0;i<8;i++){string bl=xo.substr(i*6,6);int row=(bl[0]-'0')*2+(bl[5]-'0');int col=(bl[1]-'0')*8+(bl[2]-'0')*4+(bl[3]-'0')*2+(bl[4]-'0');int v=SBOX[i][row][col];for(int j=3;j>=0;j--)so+=(char)('0'+((v>>j)&1));}
    return permute(so,P_TBL,32);
}
vector<string> genSK(const string& k64){
    vector<string> sk;string k56=permute(k64,PC1,56);string C=k56.substr(0,28),D=k56.substr(28,28);
    for(int i=0;i<16;i++){C=lsh(C,SHIFT[i]);D=lsh(D,SHIFT[i]);sk.push_back(permute(C+D,PC2,48));}
    return sk;
}
string processBlk(const string& b,vector<string> sk){
    string a=permute(b,IP,64);string L=a.substr(0,32),R=a.substr(32,32);
    for(int i=0;i<16;i++){string nr=xorS(L,funcF(R,sk[i]));L=R;R=nr;}
    return permute(R+L,IP_INV,64);
}

// Key la 8 ky tu bat ky
string encryptDES(const string& plain,const string& key8,bool useBase64){
    string k=key8.substr(0,8);while(k.size()<8)k+='\0';
    string kb=s2b(k);
    vector<string> sk=genSK(kb);
    string padded=plain;int pl=8-(padded.size()%8);for(int i=0;i<pl;i++)padded+=(char)pl;
    string cb="";for(size_t i=0;i<padded.size();i+=8)cb+=processBlk(s2b(padded.substr(i,8)),sk);
    return useBase64?b2b64(cb):b2hex(cb);
}
string decryptDES(const string& cipher,const string& key8,bool useBase64){
    string k=key8.substr(0,8);while(k.size()<8)k+='\0';
    string kb=s2b(k);
    vector<string> sk=genSK(kb);
    reverse(sk.begin(),sk.end());
    string cb=useBase64?b64_2b(cipher):hex2b(cipher);
    while(cb.size()%64&&!cb.empty())cb.pop_back();
    string pb="";for(size_t i=0;i<cb.size();i+=64)pb+=processBlk(cb.substr(i,64),sk);
    string pt=b2s(pb);
    if(!pt.empty()){int pl=(unsigned char)pt.back();if(pl>0&&pl<=8){bool ok=true;for(int i=0;i<pl;i++)if((unsigned char)pt[pt.size()-1-i]!=(unsigned char)pl){ok=false;break;}if(ok)pt=pt.substr(0,pt.size()-pl);}}
    return pt;
}
string genKey8(){
    srand((unsigned)time(0));
    const char* C="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*";
    int n=70;string k="";for(int i=0;i<8;i++)k+=C[rand()%n];return k;
}
// Kiem tra khoa: dung 8 ky tu bat ky
string checkKey(const string& key){
    int len=(int)key.size();
    if(len==0)return "Chua nhap khoa!";
    if(len<8){char buf[100];sprintf(buf,"Khoa dang THIEU %d ky tu! (can dung 8 ky tu)",8-len);return string(buf);}
    if(len>8){char buf[100];sprintf(buf,"Khoa dang THUA %d ky tu! (can dung 8 ky tu)",len-8);return string(buf);}
    return "OK";
}

// ===================== HELPERS =====================
string GetET(HWND hw){int n=GetWindowTextLength(hw)+1;vector<char>b(n);GetWindowTextA(hw,b.data(),n);return string(b.data());}
void SetET(HWND hw,const string& s){SetWindowTextA(hw,s.c_str());}

// [NÂNG C?P LÕI] Hàm ghi file siêu m?nh, dùng API Unicode ch?ng l?i ti?ng Vi?t
bool WriteF(const wstring& p, const string& c) {
    FILE* f = _wfopen(p.c_str(), L"wb");
    if (f) {
        fwrite(c.data(), 1, c.size(), f);
        fclose(f);
        return true; 
    }
    return false;
}

// [NÂNG C?P LÕI] Hàm d?c file h? tr? chu?n Unicode
string ReadF(const wstring& p) {
    FILE* f = _wfopen(p.c_str(), L"rb");
    if (!f) return "";
    
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    if(sz <= 0) { fclose(f); return ""; }
    
    string c(sz, '\0');
    fread(&c[0], 1, sz, f);
    fclose(f);
    return c;
}

// [NÂNG C?P LÕI] Dialog m?/luu dùng chu?n W (Wide Char)
bool SaveDlg(HWND p, wstring& out, const wchar_t* title=L"Luu file"){
    wchar_t sz[MAX_PATH]={0};
    OPENFILENAMEW o={0};
    o.lStructSize=sizeof(o);
    o.hwndOwner=p;
    o.lpstrFile=sz;
    o.nMaxFile=MAX_PATH;
    o.lpstrFilter=L"Text Files\0*.txt\0All Files\0*.*\0";
    o.lpstrTitle=title;
    o.lpstrDefExt=L"txt";
    o.Flags=OFN_OVERWRITEPROMPT;
    if(GetSaveFileNameW(&o)){out=sz;return true;}
    return false;
}

bool OpenDlg(HWND p, wstring& out, const wchar_t* title=L"Mo file"){
    wchar_t sz[MAX_PATH]={0};
    OPENFILENAMEW o={0};
    o.lStructSize=sizeof(o);
    o.hwndOwner=p;
    o.lpstrFile=sz;
    o.nMaxFile=MAX_PATH;
    o.lpstrFilter=L"Text Files\0*.txt\0All Files\0*.*\0";
    o.lpstrTitle=title;
    o.Flags=OFN_FILEMUSTEXIST;
    if(GetOpenFileNameW(&o)){out=sz;return true;}
    return false;
}

// ===================== FONTS & COLORS =====================
HFONT hFTitle,hFBold,hFNormal,hFMono;
#define CLR_ENC_BG  RGB(234,242,248)   // Xanh pastel
#define CLR_DEC_BG  RGB(253,237,236)   // Hong cam pastel
#define CLR_WHITE   RGB(255,255,255)
#define CLR_PRIMARY RGB(74,144,226)    
#define CLR_SUCCESS RGB(46,204,113)    
#define CLR_DANGER  RGB(231,76,60)     
#define CLR_GRAY    RGB(149,165,166)   
#define CLR_DARK    RGB(52,73,94)      

HBRUSH hBrEBg,hBrDBg,hBrWhite;
HFONT MkFont(int sz,bool bold,const char* face="Segoe UI"){
    return CreateFontA(-sz,0,0,0,bold?FW_BOLD:FW_NORMAL,0,0,0,ANSI_CHARSET,0,0,DEFAULT_QUALITY,DEFAULT_PITCH,face);
}

// ===================== GLOBAL HANDLES =====================
HWND hWnd,hTab;
HWND hEncPanel,hDecPanel;
HWND hEncInput,hEncKey,hEncResult,hEncFormat,hEncKeyStatus;
HWND hDecInput,hDecKey,hDecResult,hDecFormat,hDecKeyStatus;

struct BtnInfo { HWND hwnd; COLORREF color; bool hover; };
vector<BtnInfo> gBtns;
BtnInfo* FindBtn(HWND hw){for(auto&b:gBtns)if(b.hwnd==hw)return&b;return nullptr;}

HWND MkBtn(HWND parent,int id,const char* text,int x,int y,int w,int ht,COLORREF clr){
    HWND hw=CreateWindowA("BUTTON",text,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,x,y,w,ht,parent,(HMENU)(size_t)id,GetModuleHandle(NULL),NULL);
    SendMessage(hw,WM_SETFONT,(WPARAM)hFBold,TRUE);
    gBtns.push_back({hw,clr,false});
    return hw;
}
HWND MkEdit(HWND p,int id,int x,int y,int w,int ht,bool ml=false){
    DWORD s=WS_CHILD|WS_VISIBLE|WS_BORDER|(ml?(ES_MULTILINE|ES_AUTOVSCROLL|WS_VSCROLL|ES_WANTRETURN):ES_AUTOHSCROLL);
    HWND hw=CreateWindowA("EDIT","",s,x,y,w,ht,p,(HMENU)(size_t)id,GetModuleHandle(NULL),NULL);
    SendMessage(hw,WM_SETFONT,(WPARAM)(ml?hFMono:hFNormal),TRUE);return hw;
}
HWND MkLabel(HWND p,const char* t,int x,int y,int w,int ht,bool bold=true){
    HWND hw=CreateWindowA("STATIC",t,WS_CHILD|WS_VISIBLE|SS_LEFT,x,y,w,ht,p,NULL,GetModuleHandle(NULL),NULL);
    SendMessage(hw,WM_SETFONT,(WPARAM)(bold?hFBold:hFNormal),TRUE);return hw;
}
HWND MkCombo(HWND p,int id,int x,int y,int w,int ht){
    HWND hw=CreateWindowA("COMBOBOX","",WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_VSCROLL,x,y,w,ht,p,(HMENU)(size_t)id,GetModuleHandle(NULL),NULL);
    SendMessage(hw,WM_SETFONT,(WPARAM)hFNormal,TRUE);
    SendMessageA(hw,CB_ADDSTRING,0,(LPARAM)"Base64");
    SendMessageA(hw,CB_ADDSTRING,0,(LPARAM)"Hex");
    SendMessage(hw,CB_SETCURSEL,0,0);
    return hw;
}

void PaintPanel(HWND hwnd,COLORREF bg,const char* title,COLORREF titleClr){
    PAINTSTRUCT ps;HDC hdc=BeginPaint(hwnd,&ps);
    RECT rc;GetClientRect(hwnd,&rc);
    HBRUSH br=CreateSolidBrush(bg);FillRect(hdc,&rc,br);DeleteObject(br);
    SetBkMode(hdc,TRANSPARENT);
    SetTextColor(hdc,titleClr);
    SelectObject(hdc,hFTitle);
    RECT tr={0,15,rc.right,55};
    DrawTextA(hdc,title,-1,&tr,DT_CENTER|DT_VCENTER);
    EndPaint(hwnd,&ps);
}

void DrawRoundBtn(DRAWITEMSTRUCT* dis){
    BtnInfo* bi=FindBtn(dis->hwndItem);
    if(!bi)return;
    HDC hdc=dis->hDC;
    RECT rc=dis->rcItem;
    COLORREF clr=(dis->itemState&ODS_SELECTED)?RGB(
        max(0,(int)(GetRValue(bi->color)-30)),
        max(0,(int)(GetGValue(bi->color)-30)),
        max(0,(int)(GetBValue(bi->color)-30))
    ):bi->color;
    HBRUSH br=CreateSolidBrush(clr);
    SelectObject(hdc,br);
    SelectObject(hdc,GetStockObject(NULL_PEN));
    RoundRect(hdc,rc.left,rc.top,rc.right,rc.bottom,12,12);
    DeleteObject(br);
    char txt[128]={0};GetWindowTextA(dis->hwndItem,txt,128);
    SetBkMode(hdc,TRANSPARENT);
    SetTextColor(hdc,RGB(255,255,255));
    SelectObject(hdc,hFBold);
    DrawTextA(hdc,txt,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void UpdateKeyStatus(HWND hKeyEdit,HWND hStatus,COLORREF bg){
    string key=GetET(hKeyEdit);
    string st=checkKey(key);
    if(st=="OK"){
        SetET(hStatus,"  Khoa hop le - dung 8 ky tu");
    } else {
        SetET(hStatus,"  "+st);
    }
    InvalidateRect(hStatus,NULL,TRUE);
    UpdateWindow(hStatus);
}

// ===================== PANEL MA HOA =====================
LRESULT CALLBACK EncProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
    switch(msg){
    case WM_PAINT: PaintPanel(hwnd,CLR_ENC_BG,"CONG CU MA HOA DU LIEU",CLR_PRIMARY); return 0;
    case WM_ERASEBKGND:{RECT r={0,0,9999,9999};FillRect((HDC)wp,&r,hBrEBg);return 1;}
    case WM_CTLCOLORSTATIC:{
        HDC hdc=(HDC)wp;HWND hc=(HWND)lp;
        SetBkMode(hdc,TRANSPARENT);
        if(hc==hEncKeyStatus){
            string key=GetET(hEncKey);
            string st=checkKey(key);
            SetTextColor(hdc,st=="OK"?RGB(0,150,0):RGB(200,0,0));
            return(LRESULT)hBrEBg;
        }
        SetTextColor(hdc,CLR_DARK);
        return(LRESULT)hBrEBg;
    }
    case WM_CTLCOLOREDIT:{HDC hdc=(HDC)wp;SetBkColor(hdc,CLR_WHITE);SetTextColor(hdc,RGB(0,0,0));return(LRESULT)hBrWhite;}
    case WM_CTLCOLORLISTBOX:{HDC hdc=(HDC)wp;SetBkColor(hdc,CLR_WHITE);return(LRESULT)hBrWhite;}
    case WM_DRAWITEM: DrawRoundBtn((DRAWITEMSTRUCT*)lp); return TRUE;
    case WM_COMMAND:{
        int id=LOWORD(wp);int notif=HIWORD(wp);
        if(id==ID_ENC_KEY&&notif==EN_CHANGE) UpdateKeyStatus(hEncKey,hEncKeyStatus,CLR_ENC_BG);
        else if(id==ID_ENC_BTN_GENKEY){
            string k=genKey8();SetET(hEncKey,k);
            MessageBoxA(hwnd,("Da sinh khoa ngau nhien:\n\n   "+k+"\n\n(8 ky tu bat ky)").c_str(),"Sinh khoa thanh cong",MB_ICONINFORMATION);
        }
        else if(id==ID_ENC_BTN_ENCRYPT){
            string plain=GetET(hEncInput);string key=GetET(hEncKey);
            if(plain.empty()){MessageBoxA(hwnd,"Vui long nhap van ban can ma hoa!","Canh bao",MB_ICONWARNING);return 0;}
            string st=checkKey(key);
            if(st!="OK"){MessageBoxA(hwnd,st.c_str(),"Khoa khong hop le",MB_ICONERROR);return 0;}
            bool b64=(SendMessage(hEncFormat,CB_GETCURSEL,0,0)==0);
            string result=encryptDES(plain,key,b64);
            SetET(hEncResult,result);
            MessageBoxA(hwnd,"Ma hoa thanh cong!\n\nKet qua hien thi o o 'Ket qua ma hoa'.","Thong bao",MB_ICONINFORMATION);
        }
        else if(id==ID_ENC_BTN_CLEAR){SetET(hEncInput,"");SetET(hEncKey,"");SetET(hEncResult,"");}
        else if(id==ID_ENC_BTN_SAVERES){
            string c=GetET(hEncResult);
            if(c.empty()){MessageBoxA(hwnd,"Chua co ket qua ma hoa de luu!","Canh bao",MB_ICONWARNING);return 0;}
            wstring p;
            if(SaveDlg(hwnd,p,L"Luu ket qua ma hoa")){
                if(WriteF(p,c)) MessageBoxA(hwnd,"Luu thanh cong!","OK",MB_ICONINFORMATION);
                else MessageBoxA(hwnd,"Loi: Khong the luu file!\nKiem tra lai thu muc hoac quyen ghi!","Loi",MB_ICONERROR);
            }
        }
        else if(id==ID_ENC_BTN_SAVETXT){
            string c=GetET(hEncInput);
            if(c.empty()){MessageBoxA(hwnd,"Chua co van ban de luu!","Canh bao",MB_ICONWARNING);return 0;}
            wstring p;
            if(SaveDlg(hwnd,p,L"Luu van ban goc")){
                if(WriteF(p,c)) MessageBoxA(hwnd,"Luu thanh cong!","OK",MB_ICONINFORMATION);
                else MessageBoxA(hwnd,"Loi: Khong the luu file!","Loi",MB_ICONERROR);
            }
        }
        else if(id==ID_ENC_BTN_LOADTXT){
            wstring p;
            if(OpenDlg(hwnd,p,L"Tai tep van ban")){
                string content = ReadF(p);
                SetET(hEncInput, content);
            }
        }
        else if(id==ID_ENC_BTN_SAVEKEY){
            string key=GetET(hEncKey);string st=checkKey(key);
            if(st!="OK"){MessageBoxA(hwnd,st.c_str(),"Khoa khong hop le",MB_ICONERROR);return 0;}
            wstring p;
            if(SaveDlg(hwnd,p,L"Luu khoa DES")){
                if(WriteF(p,key)) MessageBoxA(hwnd,"Luu khoa thanh cong!","OK",MB_ICONINFORMATION);
                else MessageBoxA(hwnd,"Loi: Khong the luu file khoa!","Loi",MB_ICONERROR);
            }
        }
        else if(id==ID_ENC_BTN_LOADKEY){
            wstring p;
            if(OpenDlg(hwnd,p,L"Tai khoa DES")){
                string loaded = ReadF(p);
                if(loaded.size()>8) loaded = loaded.substr(0,8);
                SetET(hEncKey, loaded);
            }
        }
        break;
    }
    }
    return DefWindowProc(hwnd,msg,wp,lp);
}

// ===================== PANEL GIAI MA =====================
LRESULT CALLBACK DecProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
    switch(msg){
    case WM_PAINT: PaintPanel(hwnd,CLR_DEC_BG,"CONG CU GIAI MA DU LIEU",CLR_DANGER); return 0;
    case WM_ERASEBKGND:{RECT r={0,0,9999,9999};FillRect((HDC)wp,&r,hBrDBg);return 1;}
    case WM_CTLCOLORSTATIC:{
        HDC hdc=(HDC)wp;HWND hc=(HWND)lp;
        SetBkMode(hdc,TRANSPARENT);
        if(hc==hDecKeyStatus){
            string key=GetET(hDecKey);string st=checkKey(key);
            SetTextColor(hdc,st=="OK"?RGB(0,150,0):RGB(200,0,0));
            return(LRESULT)hBrDBg;
        }
        SetTextColor(hdc,CLR_DARK);
        return(LRESULT)hBrDBg;
    }
    case WM_CTLCOLOREDIT:{HDC hdc=(HDC)wp;SetBkColor(hdc,CLR_WHITE);SetTextColor(hdc,RGB(0,0,0));return(LRESULT)hBrWhite;}
    case WM_CTLCOLORLISTBOX:{HDC hdc=(HDC)wp;SetBkColor(hdc,CLR_WHITE);return(LRESULT)hBrWhite;}
    case WM_DRAWITEM: DrawRoundBtn((DRAWITEMSTRUCT*)lp); return TRUE;
    case WM_COMMAND:{
        int id=LOWORD(wp);int notif=HIWORD(wp);
        if(id==ID_DEC_KEY&&notif==EN_CHANGE) UpdateKeyStatus(hDecKey,hDecKeyStatus,CLR_DEC_BG);
        else if(id==ID_DEC_BTN_DECRYPT){
            string cipher=GetET(hDecInput);string key=GetET(hDecKey);
            if(cipher.empty()){MessageBoxA(hwnd,"Vui long nhap ban ma can giai ma!","Canh bao",MB_ICONWARNING);return 0;}
            string st=checkKey(key);
            if(st!="OK"){MessageBoxA(hwnd,st.c_str(),"Khoa khong hop le",MB_ICONERROR);return 0;}
            bool b64=(SendMessage(hDecFormat,CB_GETCURSEL,0,0)==0);
            string result=decryptDES(cipher,key,b64);
            if(result.empty()){MessageBoxA(hwnd,"Giai ma that bai!\nKhoa sai hoac dinh dang ban ma khong dung.","Loi Giai Ma",MB_ICONERROR);return 0;}
            SetET(hDecResult,result);
            MessageBoxA(hwnd,"Giai ma thanh cong!\n\nKet qua hien thi o o 'Ket qua giai ma'.","Thong bao",MB_ICONINFORMATION);
        }
        else if(id==ID_DEC_BTN_CLEAR){SetET(hDecInput,"");SetET(hDecKey,"");SetET(hDecResult,"");}
        else if(id==ID_DEC_BTN_SAVERES){
            string c=GetET(hDecResult);
            if(c.empty()){MessageBoxA(hwnd,"Chua co ket qua de luu!","Canh bao",MB_ICONWARNING);return 0;}
            wstring p;
            if(SaveDlg(hwnd,p,L"Luu ket qua giai ma")){
                if(WriteF(p,c)) MessageBoxA(hwnd,"Luu thanh cong!","OK",MB_ICONINFORMATION);
                else MessageBoxA(hwnd,"Loi: Khong the luu file!","Loi",MB_ICONERROR);
            }
        }
        else if(id==ID_DEC_BTN_LOADTXT){
            wstring p;
            if(OpenDlg(hwnd,p,L"Tai tep ma hoa")){
                SetET(hDecInput,ReadF(p));
                MessageBoxA(hwnd,"Da tai ban ma thanh cong!","Thong bao",MB_ICONINFORMATION);
            }
        }
        else if(id==ID_DEC_BTN_LOADKEY){
            wstring p;
            if(OpenDlg(hwnd,p,L"Tai khoa DES")){
                string loaded = ReadF(p);
                if(loaded.size()>8) loaded = loaded.substr(0,8);
                SetET(hDecKey, loaded);
            }
        }
        break;
    }
    }
    return DefWindowProc(hwnd,msg,wp,lp);
}

// ===================== MAIN WINDOW =====================
LRESULT CALLBACK MainProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
    switch(msg){
    case WM_SIZE:{
        int W=LOWORD(lp),H=HIWORD(lp);
        MoveWindow(hTab,0,0,W,H,TRUE);
        RECT tr={0,0,W,H};
        SendMessage(hTab, TCM_ADJUSTRECT, FALSE, (LPARAM)&tr);
        MoveWindow(hEncPanel,tr.left,tr.top,tr.right-tr.left,tr.bottom-tr.top,TRUE);
        MoveWindow(hDecPanel,tr.left,tr.top,tr.right-tr.left,tr.bottom-tr.top,TRUE);
        break;
    }
    case WM_NOTIFY:{
        NMHDR* nm=(NMHDR*)lp;
        if(nm->idFrom==ID_TAB&&nm->code==TCN_SELCHANGE){
            int sel=TabCtrl_GetCurSel(hTab);
            ShowWindow(hEncPanel,sel==0?SW_SHOW:SW_HIDE);
            ShowWindow(hDecPanel,sel==1?SW_SHOW:SW_HIDE);
        }
        break;
    }
    case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProc(hwnd,msg,wp,lp);
}

// Xay dung controls ben trong panel
void BuildEncPanel(HWND p,int W,int H){
    int lx=20,ew=W-lx*2-10;
    int y=60;
    MkLabel(p,"Van ban goc can ma hoa:",lx,y,250,22);
    MkBtn(p,ID_ENC_BTN_LOADTXT,"Tai tep (.txt)",W-290,y-2,120,26,CLR_PRIMARY);
    MkBtn(p,ID_ENC_BTN_SAVETXT,"Luu van ban",W-160,y-2,115,26,CLR_GRAY);
    hEncInput=MkEdit(p,ID_ENC_INPUT,lx,y+26,ew,110,true);
    y+=145;
    MkLabel(p,"Nhap khoa bi mat (Dung 8 ky tu):",lx,y,280,22);
    hEncKey=MkEdit(p,ID_ENC_KEY,lx,y+26,200,28);
    MkBtn(p,ID_ENC_BTN_GENKEY,"Sinh khoa",lx+210,y+24,100,28,CLR_PRIMARY);
    MkBtn(p,ID_ENC_BTN_SAVEKEY,"Luu khoa",lx+320,y+24,90,28,CLR_GRAY);
    MkBtn(p,ID_ENC_BTN_LOADKEY,"Tai khoa",lx+420,y+24,90,28,CLR_GRAY);
    hEncKeyStatus=MkLabel(p,"  Chua nhap khoa",lx,y+58,ew,22,false);
    y+=85;
    MkLabel(p,"Dinh dang dau ra:",lx,y+5,150,22);
    hEncFormat=MkCombo(p,ID_ENC_FORMAT,lx+160,y,120,120);
    MkBtn(p,ID_ENC_BTN_ENCRYPT,"  MA HOA DU LIEU  ",lx+310,y-2,180,32,CLR_SUCCESS);
    MkBtn(p,ID_ENC_BTN_CLEAR,"Xoa trang",lx+505,y,100,30,CLR_GRAY);
    y+=48;
    MkLabel(p,"Ket qua ma hoa:",lx,y,180,22);
    MkBtn(p,ID_ENC_BTN_SAVERES,"Luu ket qua",W-145,y-2,120,26,CLR_PRIMARY);
    hEncResult=MkEdit(p,ID_ENC_RESULT,lx,y+26,ew,H-y-50,true);
    SendMessage(hEncResult,EM_SETREADONLY,TRUE,0);
}

void BuildDecPanel(HWND p,int W,int H){
    int lx=20,ew=W-lx*2-10;
    int y=60;
    MkLabel(p,"Van ban da ma hoa:",lx,y,250,22);
    MkBtn(p,ID_DEC_BTN_LOADTXT,"Tai tep (.txt)",W-170,y-2,130,26,CLR_PRIMARY);
    hDecInput=MkEdit(p,ID_DEC_INPUT,lx,y+26,ew,110,true);
    y+=145;
    MkLabel(p,"Nhap khoa giai ma (Dung 8 ky tu):",lx,y,290,22);
    hDecKey=MkEdit(p,ID_DEC_KEY,lx,y+26,200,28);
    MkBtn(p,ID_DEC_BTN_LOADKEY,"Tai khoa",lx+210,y+24,90,28,CLR_GRAY);
    hDecKeyStatus=MkLabel(p,"  Chua nhap khoa",lx,y+58,ew,22,false);
    y+=85;
    MkLabel(p,"Dinh dang dau vao:",lx,y+5,160,22);
    hDecFormat=MkCombo(p,ID_DEC_FORMAT,lx+170,y,120,120);
    MkBtn(p,ID_DEC_BTN_DECRYPT,"  GIAI MA DU LIEU  ",lx+320,y-2,180,32,CLR_DANGER);
    MkBtn(p,ID_DEC_BTN_CLEAR,"Xoa trang",lx+515,y,100,30,CLR_GRAY);
    y+=48;
    MkLabel(p,"Ket qua giai ma (Van ban goc):",lx,y,260,22);
    MkBtn(p,ID_DEC_BTN_SAVERES,"Luu ket qua",W-145,y-2,120,26,CLR_PRIMARY);
    hDecResult=MkEdit(p,ID_DEC_RESULT,lx,y+26,ew,H-y-50,true);
    SendMessage(hDecResult,EM_SETREADONLY,TRUE,0);
}

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int nShow){
    InitCommonControls();
    hFTitle=MkFont(20,true);
    hFBold=MkFont(13,true);
    hFNormal=MkFont(13,false);
    hFMono=MkFont(13,false,"Consolas");
    
    hBrEBg=CreateSolidBrush(CLR_ENC_BG);
    hBrDBg=CreateSolidBrush(CLR_DEC_BG);
    hBrWhite=CreateSolidBrush(CLR_WHITE);

    auto Reg=[&](const char* nm,WNDPROC proc,HBRUSH bg){
        WNDCLASSEXA wc={sizeof(wc)};wc.lpfnWndProc=proc;wc.hInstance=hInst;
        wc.lpszClassName=nm;wc.hCursor=LoadCursor(NULL,IDC_ARROW);wc.hbrBackground=bg;
        RegisterClassExA(&wc);
    };
    Reg("MainCls",MainProc,CreateSolidBrush(RGB(240,240,240)));
    Reg("EncCls",EncProc,hBrEBg);
    Reg("DecCls",DecProc,hBrDBg);

    int W=1000,H=750;
    hWnd=CreateWindowExA(0,"MainCls","Chuong trinh Ma hoa & Giai ma DES",
        WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,W,H,NULL,NULL,hInst,NULL);

    hTab=CreateWindowExA(0,WC_TABCONTROLA,"",WS_CHILD|WS_VISIBLE|TCS_FLATBUTTONS,
        0,0,W,H,hWnd,(HMENU)ID_TAB,hInst,NULL);
    SendMessage(hTab,WM_SETFONT,(WPARAM)hFBold,TRUE);

    TCITEMA ti={0};ti.mask=TCIF_TEXT;
    ti.pszText=(char*)"  Ma Hoa DES  ";TabCtrl_InsertItem(hTab,0,&ti);
    ti.pszText=(char*)"  Giai Ma DES  ";TabCtrl_InsertItem(hTab,1,&ti);

    RECT cr={0,0,W,H};
    SendMessage(hTab, TCM_ADJUSTRECT, FALSE, (LPARAM)&cr);
    int cw=cr.right-cr.left,ch=cr.bottom-cr.top;

    hEncPanel=CreateWindowExA(0,"EncCls","",WS_CHILD|WS_VISIBLE,cr.left,cr.top,cw,ch,hWnd,NULL,hInst,NULL);
    hDecPanel=CreateWindowExA(0,"DecCls","",WS_CHILD,cr.left,cr.top,cw,ch,hWnd,NULL,hInst,NULL);

    BuildEncPanel(hEncPanel,cw,ch);
    BuildDecPanel(hDecPanel,cw,ch);

    ShowWindow(hWnd,nShow);
    UpdateWindow(hWnd);

    MSG msg;
    while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
