#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "comdlg32.lib")

using namespace std;

// ===================== ID CÃC CONTROL =====================
#define ID_BTN_ENCRYPT       101
#define ID_BTN_CLEAR_ENC     102
#define ID_BTN_SAVE_ENC      103
#define ID_BTN_LOAD_TEXT_ENC 104
#define ID_BTN_GEN_KEY_ENC   105
#define ID_BTN_LOAD_KEY_ENC  106
#define ID_BTN_SAVE_KEY_ENC  107

#define ID_BTN_DECRYPT       201
#define ID_BTN_CLEAR_DEC     202
#define ID_BTN_SAVE_DEC      203
#define ID_BTN_LOAD_TEXT_DEC 204
#define ID_BTN_LOAD_KEY_DEC  205
#define ID_BTN_SAVE_KEY_DEC  206

#define ID_EDIT_PLAIN        301
#define ID_EDIT_ENC_KEY      302
#define ID_EDIT_ENC_RESULT   303
#define ID_EDIT_CIPHER_INPUT 401
#define ID_EDIT_DEC_KEY      402
#define ID_EDIT_DEC_RESULT   403

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

// ===================== HAM DES =====================
string permute(const string& in, int* tbl, int n){
    string r(n,'0');
    for(int i=0;i<n;i++) r[i]=in[tbl[i]-1];
    return r;
}
string lshift(const string& k,int n){ return k.substr(n)+k.substr(0,n); }
string xorS(const string& a,const string& b){
    string r(a.size(),'0');
    for(size_t i=0;i<a.size();i++) r[i]=(a[i]==b[i])?'0':'1';
    return r;
}
string strToBin(const string& s){
    string r="";
    for(unsigned char c:s){ for(int i=7;i>=0;i--) r+=(char)('0'+((c>>i)&1)); }
    return r;
}
string hexToBin(const string& h){
    string r="";
    for(char c:h){
        int v=(c>='0'&&c<='9')?c-'0':(c>='a'&&c<='f')?c-'a'+10:(c>='A'&&c<='F')?c-'A'+10:0;
        for(int i=3;i>=0;i--) r+=(char)('0'+((v>>i)&1));
    }
    return r;
}
string binToHex(const string& b){
    string r="";
    for(size_t i=0;i<b.size();i+=4){
        int v=0;
        for(int j=0;j<4;j++) v=(v<<1)+(b[i+j]-'0');
        r+=(char)(v<10?'0'+v:'A'+v-10);
    }
    return r;
}
string binToBase64(const string& b){
    const string ch="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    string p=b; while(p.size()%6) p+='0';
    string r="";
    for(size_t i=0;i<p.size();i+=6){
        int v=0; for(int j=0;j<6;j++) v=(v<<1)+(p[i+j]-'0');
        r+=ch[v];
    }
    while(r.size()%4) r+='=';
    return r;
}
string base64ToBin(const string& s){
    const string ch="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    string r="";
    for(char c:s){
        if(c=='=') break;
        int idx=(int)ch.find(c);
        if(idx<0) continue;
        for(int i=5;i>=0;i--) r+=(char)('0'+((idx>>i)&1));
    }
    return r;
}
string binToStr(const string& b){
    string r="";
    for(size_t i=0;i+8<=b.size();i+=8){
        int v=0; for(int j=0;j<8;j++) v=(v<<1)+(b[i+j]-'0');
        r+=(char)v;
    }
    return r;
}
string funcF(const string& R,const string& K){
    string ex=permute(R,E_TBL,48);
    string xo=xorS(ex,K);
    string so="";
    for(int i=0;i<8;i++){
        string bl=xo.substr(i*6,6);
        int row=(bl[0]-'0')*2+(bl[5]-'0');
        int col=(bl[1]-'0')*8+(bl[2]-'0')*4+(bl[3]-'0')*2+(bl[4]-'0');
        int v=SBOX[i][row][col];
        for(int j=3;j>=0;j--) so+=(char)('0'+((v>>j)&1));
    }
    return permute(so,P_TBL,32);
}
vector<string> genSubKeys(const string& key64){
    vector<string> sk;
    string k56=permute(key64,PC1,56);
    string C=k56.substr(0,28),D=k56.substr(28,28);
    for(int i=0;i<16;i++){
        C=lshift(C,SHIFT[i]); D=lshift(D,SHIFT[i]);
        sk.push_back(permute(C+D,PC2,48));
    }
    return sk;
}
string processBlock(const string& b64,vector<string> sk){
    string a=permute(b64,IP,64);
    string L=a.substr(0,32),R=a.substr(32,32);
    for(int i=0;i<16;i++){ string nr=xorS(L,funcF(R,sk[i])); L=R; R=nr; }
    return permute(R+L,IP_INV,64);
}

// Encrypt: key la hex 16 ky tu
string encryptDES(const string& plain, const string& hexKey){
    string keyBin=hexToBin(hexKey);
    vector<string> sk=genSubKeys(keyBin);
    string padded=plain;
    int padLen=8-(padded.size()%8);
    for(int i=0;i<padLen;i++) padded+=(char)padLen;
    string cb="";
    for(size_t i=0;i<padded.size();i+=8)
        cb+=processBlock(strToBin(padded.substr(i,8)),sk);
    return binToBase64(cb);
}

string decryptDES(const string& b64cipher, const string& hexKey){
    string keyBin=hexToBin(hexKey);
    vector<string> sk=genSubKeys(keyBin);
    reverse(sk.begin(),sk.end());
    string cb=base64ToBin(b64cipher);
    while(cb.size()%64&&!cb.empty()) cb.pop_back();
    string pb="";
    for(size_t i=0;i<cb.size();i+=64)
        pb+=processBlock(cb.substr(i,64),sk);
    string pt=binToStr(pb);
    if(!pt.empty()){
        int pl=(unsigned char)pt.back();
        if(pl>0&&pl<=8){
            bool ok=true;
            for(int i=0;i<pl;i++) if((unsigned char)pt[pt.size()-1-i]!=(unsigned char)pl){ok=false;break;}
            if(ok) pt=pt.substr(0,pt.size()-pl);
        }
    }
    return pt;
}

string genKey(){
    srand((unsigned)time(0));
    const char* ch="0123456789ABCDEF";
    string k=""; for(int i=0;i<16;i++) k+=ch[rand()%16];
    return k;
}

// ===================== WinAPI HELPERS =====================
// Lay text tu Edit control
string GetEditText(HWND hw){
    int len=GetWindowTextLength(hw)+1;
    vector<char> buf(len);
    GetWindowTextA(hw,buf.data(),len);
    return string(buf.data());
}
void SetEditText(HWND hw,const string& s){ SetWindowTextA(hw,s.c_str()); }

bool SaveFileDialog(HWND parent, string& outPath){
    char szFile[MAX_PATH]={0};
    OPENFILENAMEA ofn={0};
    ofn.lStructSize=sizeof(ofn);
    ofn.hwndOwner=parent;
    ofn.lpstrFile=szFile;
    ofn.nMaxFile=MAX_PATH;
    ofn.lpstrFilter="Text Files\0*.txt\0All Files\0*.*\0";
    ofn.Flags=OFN_OVERWRITEPROMPT;
    if(GetSaveFileNameA(&ofn)){ outPath=szFile; return true; }
    return false;
}
bool OpenFileDialog(HWND parent, string& outPath){
    char szFile[MAX_PATH]={0};
    OPENFILENAMEA ofn={0};
    ofn.lStructSize=sizeof(ofn);
    ofn.hwndOwner=parent;
    ofn.lpstrFile=szFile;
    ofn.nMaxFile=MAX_PATH;
    ofn.lpstrFilter="Text Files\0*.txt\0All Files\0*.*\0";
    ofn.Flags=OFN_FILEMUSTEXIST;
    if(GetOpenFileNameA(&ofn)){ outPath=szFile; return true; }
    return false;
}
void WriteFile2(const string& path, const string& content){
    ofstream f(path);
    if(f.is_open()){ f<<content; f.close(); }
}
string ReadFile2(const string& path){
    ifstream f(path);
    if(!f.is_open()) return "";
    return string((istreambuf_iterator<char>(f)),istreambuf_iterator<char>());
}

// ===================== GLOBAL HANDLES =====================
HWND hMain;
HWND hEditPlain, hEditEncKey, hEditEncResult;
HWND hEditCipherInput, hEditDecKey, hEditDecResult;
HWND hBtnEncrypt, hBtnClearEnc, hBtnSaveEnc;
HWND hBtnLoadTextEnc, hBtnGenKey, hBtnLoadKeyEnc, hBtnSaveKeyEnc;
HWND hBtnDecrypt, hBtnClearDec, hBtnSaveDec;
HWND hBtnLoadTextDec, hBtnLoadKeyDec, hBtnSaveKeyDec;
HWND hLabelTitle;

// Font
HFONT hFontNormal, hFontBold, hFontTitle, hFontLabel;

HFONT CreateF(int size, bool bold, const char* face="Arial"){
    return CreateFontA(-size,0,0,0,bold?FW_BOLD:FW_NORMAL,0,0,0,
        ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,DEFAULT_PITCH,face);
}

HWND MakeLabel(HWND parent, const char* text, int x,int y,int w,int ht){
    HWND hw=CreateWindowA("STATIC",text,WS_CHILD|WS_VISIBLE|SS_LEFT,
        x,y,w,ht,parent,NULL,GetModuleHandle(NULL),NULL);
    SendMessage(hw,WM_SETFONT,(WPARAM)hFontLabel,TRUE);
    return hw;
}
HWND MakeEdit(HWND parent, int id,int x,int y,int w,int ht,bool multiline=false){
    DWORD style=WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL;
    if(multiline) style=WS_CHILD|WS_VISIBLE|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL|WS_VSCROLL|ES_WANTRETURN;
    HWND hw=CreateWindowA("EDIT","",style,x,y,w,ht,parent,(HMENU)(size_t)id,GetModuleHandle(NULL),NULL);
    SendMessage(hw,WM_SETFONT,(WPARAM)hFontNormal,TRUE);
    return hw;
}
HWND MakeButton(HWND parent,int id,const char* text,int x,int y,int w,int ht,COLORREF bg=RGB(220,220,220)){
    HWND hw=CreateWindowA("BUTTON",text,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
        x,y,w,ht,parent,(HMENU)(size_t)id,GetModuleHandle(NULL),NULL);
    SendMessage(hw,WM_SETFONT,(WPARAM)hFontBold,TRUE);
    return hw;
}

// Custom draw background
HBRUSH hBgBrush, hPanelBrush, hBtnBrush;

// ===================== WINDOW PROCEDURE =====================
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
    switch(msg){
    case WM_CTLCOLORSTATIC:{
        HDC hdc=(HDC)wp;
        HWND hCtrl=(HWND)lp;
        SetBkMode(hdc,TRANSPARENT);
        SetTextColor(hdc,RGB(30,30,80));
        return (LRESULT)hBgBrush;
    }
    case WM_CTLCOLOREDIT:{
        HDC hdc=(HDC)wp;
        SetBkColor(hdc,RGB(255,255,255));
        SetTextColor(hdc,RGB(0,0,0));
        return (LRESULT)GetStockObject(WHITE_BRUSH);
    }
    case WM_COMMAND:{
        int id=LOWORD(wp);
        // === MA HOA ===
        if(id==ID_BTN_ENCRYPT){
            string plain=GetEditText(hEditPlain);
            string key=GetEditText(hEditEncKey);
            if(plain.empty()||key.empty()){
                MessageBoxA(hwnd,"Vui long nhap van ban va khoa!","Loi",MB_ICONERROR);
                return 0;
            }
            if(key.size()!=16){
                MessageBoxA(hwnd,"Khoa phai co dung 16 ky tu Hex (8 bytes)!","Loi",MB_ICONERROR);
                return 0;
            }
            // Validate hex
            for(char c:key) if(!isxdigit(c)){
                MessageBoxA(hwnd,"Khoa chi duoc chua ky tu Hex (0-9, A-F)!","Loi",MB_ICONERROR);
                return 0;
            }
            string result=encryptDES(plain,key);
            SetEditText(hEditEncResult,result);
            MessageBoxA(hwnd,"Ma hoa thanh cong!","Thong bao",MB_ICONINFORMATION);
        }
        // === GIAI MA ===
        else if(id==ID_BTN_DECRYPT){
            string cipher=GetEditText(hEditCipherInput);
            string key=GetEditText(hEditDecKey);
            if(cipher.empty()||key.empty()){
                MessageBoxA(hwnd,"Vui long nhap ban ma va khoa!","Loi",MB_ICONERROR);
                return 0;
            }
            if(key.size()!=16){
                MessageBoxA(hwnd,"Khoa phai co dung 16 ky tu Hex!","Loi",MB_ICONERROR);
                return 0;
            }
            for(char c:key) if(!isxdigit(c)){
                MessageBoxA(hwnd,"Khoa chi duoc chua ky tu Hex!","Loi",MB_ICONERROR);
                return 0;
            }
            string result=decryptDES(cipher,key);
            SetEditText(hEditDecResult,result);
            MessageBoxA(hwnd,"Giai ma thanh cong!","Thong bao",MB_ICONINFORMATION);
        }
        // === SINH KHOA ===
        else if(id==ID_BTN_GEN_KEY_ENC){
            string k=genKey();
            SetEditText(hEditEncKey,k);
            SetEditText(hEditDecKey,k);
            string msg="Da tao khoa ngau nhien: "+k;
            MessageBoxA(hwnd,msg.c_str(),"Thong bao",MB_ICONINFORMATION);
        }
        // === XOA ENC ===
        else if(id==ID_BTN_CLEAR_ENC){
            SetEditText(hEditPlain,"");
            SetEditText(hEditEncResult,"");
        }
        // === XOA DEC ===
        else if(id==ID_BTN_CLEAR_DEC){
            SetEditText(hEditCipherInput,"");
            SetEditText(hEditDecResult,"");
        }
        // === LUU KET QUA MA HOA ===
        else if(id==ID_BTN_SAVE_ENC){
            string content=GetEditText(hEditEncResult);
            if(content.empty()){MessageBoxA(hwnd,"Khong co du lieu de luu!","Canh bao",MB_ICONWARNING);return 0;}
            string path;
            if(SaveFileDialog(hwnd,path)){
                WriteFile2(path,content);
                MessageBoxA(hwnd,("Da luu thanh cong:\n"+path).c_str(),"Thong bao",MB_ICONINFORMATION);
            }
        }
        // === LUU KET QUA GIAI MA ===
        else if(id==ID_BTN_SAVE_DEC){
            string content=GetEditText(hEditDecResult);
            if(content.empty()){MessageBoxA(hwnd,"Khong co du lieu de luu!","Canh bao",MB_ICONWARNING);return 0;}
            string path;
            if(SaveFileDialog(hwnd,path)){
                WriteFile2(path,content);
                MessageBoxA(hwnd,("Da luu thanh cong:\n"+path).c_str(),"Thong bao",MB_ICONINFORMATION);
            }
        }
        // === TAI VAN BAN GOC ===
        else if(id==ID_BTN_LOAD_TEXT_ENC){
            string path;
            if(OpenFileDialog(hwnd,path)){
                string content=ReadFile2(path);
                SetEditText(hEditPlain,content);
            }
        }
        // === TAI FILE BAN MA (sang giai ma) ===
        else if(id==ID_BTN_LOAD_TEXT_DEC){
            string path;
            if(OpenFileDialog(hwnd,path)){
                string content=ReadFile2(path);
                SetEditText(hEditCipherInput,content);
                MessageBoxA(hwnd,"Da tai van ban ma hoa thanh cong!","Thong bao",MB_ICONINFORMATION);
            }
        }
        // === LUU KHOA ENC ===
        else if(id==ID_BTN_SAVE_KEY_ENC){
            string key=GetEditText(hEditEncKey);
            if(key.empty()){MessageBoxA(hwnd,"Chua co khoa!","Canh bao",MB_ICONWARNING);return 0;}
            string path;
            if(SaveFileDialog(hwnd,path)){
                WriteFile2(path,key);
                MessageBoxA(hwnd,"Da luu khoa!","Thong bao",MB_ICONINFORMATION);
            }
        }
        // === TAI KHOA ENC ===
        else if(id==ID_BTN_LOAD_KEY_ENC){
            string path;
            if(OpenFileDialog(hwnd,path)){
                SetEditText(hEditEncKey,ReadFile2(path));
            }
        }
        // === LUU KHOA DEC ===
        else if(id==ID_BTN_SAVE_KEY_DEC){
            string key=GetEditText(hEditDecKey);
            if(key.empty()){MessageBoxA(hwnd,"Chua co khoa!","Canh bao",MB_ICONWARNING);return 0;}
            string path;
            if(SaveFileDialog(hwnd,path)){
                WriteFile2(path,key);
                MessageBoxA(hwnd,"Da luu khoa!","Thong bao",MB_ICONINFORMATION);
            }
        }
        // === TAI KHOA DEC ===
        else if(id==ID_BTN_LOAD_KEY_DEC){
            string path;
            if(OpenFileDialog(hwnd,path)){
                SetEditText(hEditDecKey,ReadFile2(path));
            }
        }
        break;
    }
    case WM_PAINT:{
        PAINTSTRUCT ps;
        HDC hdc=BeginPaint(hwnd,&ps);
        RECT rc; GetClientRect(hwnd,&rc);
        // Ve nen
        FillRect(hdc,&rc,hBgBrush);
        // Ve 2 panel
        int pw=(rc.right-20)/2;
        RECT rLeft={10,30,10+pw,rc.bottom-10};
        RECT rRight={20+pw,30,rc.right-10,rc.bottom-10};
        HBRUSH panelBr=CreateSolidBrush(RGB(190,225,255));
        FillRect(hdc,&rLeft,panelBr);
        FillRect(hdc,&rRight,panelBr);
        // Vien panel
        HPEN pen=CreatePen(PS_SOLID,2,RGB(100,150,255));
        HPEN old=(HPEN)SelectObject(hdc,pen);
        SelectObject(hdc,GetStockObject(NULL_BRUSH));
        Rectangle(hdc,rLeft.left,rLeft.top,rLeft.right,rLeft.bottom);
        Rectangle(hdc,rRight.left,rRight.top,rRight.right,rRight.bottom);
        SelectObject(hdc,old);
        DeleteObject(pen);
        DeleteObject(panelBr);
        // Tieu de panel
        SetBkMode(hdc,TRANSPARENT);
        SetTextColor(hdc,RGB(100,150,255));
        SelectObject(hdc,hFontTitle);
        RECT tLeft={rLeft.left,rLeft.top-5,rLeft.right,rLeft.top+20};
        RECT tRight={rRight.left,rRight.top-5,rRight.right,rRight.top+20};
        DrawTextA(hdc,"  Ma hoa  ",-1,&tLeft,DT_LEFT|DT_VCENTER);
        DrawTextA(hdc,"  Giai ma  ",-1,&tRight,DT_LEFT|DT_VCENTER);
        // Tieu de chinh
        SetTextColor(hdc,RGB(30,30,100));
        SelectObject(hdc,hFontBold);
        RECT tMain={0,5,rc.right,28};
        DrawTextA(hdc,"CHUONG TRINH MA HOA VA GIAI MA DES",-1,&tMain,DT_CENTER|DT_VCENTER);
        EndPaint(hwnd,&ps);
        return 0;
    }
    case WM_ERASEBKGND:{
        HDC hdc=(HDC)wp;
        RECT rc; GetClientRect(hwnd,&rc);
        FillRect(hdc,&rc,hBgBrush);
        return 1;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd,msg,wp,lp);
}

// ===================== WINMAIN =====================
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow){
    // Fonts
    hFontNormal=CreateF(13,false);
    hFontBold=CreateF(13,true);
    hFontTitle=CreateF(15,true);
    hFontLabel=CreateF(13,false);
    // Brushes
    hBgBrush=CreateSolidBrush(RGB(230,240,255));

    // Dang ky window class
    WNDCLASSEXA wc={sizeof(wc)};
    wc.lpfnWndProc=WndProc;
    wc.hInstance=hInst;
    wc.lpszClassName="DESToolClass";
    wc.hCursor=LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground=hBgBrush;
    RegisterClassExA(&wc);

    // Tao cua so chinh
    int W=1000,H=580;
    hMain=CreateWindowExA(0,"DESToolClass","DES String Tool",
        WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,
        CW_USEDEFAULT,CW_USEDEFAULT,W,H,NULL,NULL,hInst,NULL);

    // --- TINH KICH THUOC ---
    int pw=(W-30)/2; // chieu rong moi panel
    int lx=10, rx=lx+pw+10; // x cua panel trai / phai
    int py=35; // y bat dau panel
    int lw=160; // chieu rong label
    int ew=pw-lw-30; // chieu rong edit
    int ex_=lx+lw+5; // x edit panel trai
    int ex2=rx+lw+5; // x edit panel phai
    int row=50; // khoang cach giua cac dong

    // === PANEL TRAI: MA HOA ===
    // Hang 1: Van ban goc
    MakeLabel(hMain,"Van ban goc:",lx+8,py+10,lw,20);
    hEditPlain=MakeEdit(hMain,ID_EDIT_PLAIN,ex_,py+8,ew,80,true);

    // Nut tai tep van ban
    hBtnLoadTextEnc=MakeButton(hMain,ID_BTN_LOAD_TEXT_ENC,"Tai tep van ban",ex_+ew-130,py+95,130,22);

    // Hang 2: Nhap khoa
    MakeLabel(hMain,"Nhap khoa:",lx+8,py+125,lw,20);
    hEditEncKey=MakeEdit(hMain,ID_EDIT_ENC_KEY,ex_,py+123,ew,22);
    SetEditText(hEditEncKey,"22F1E653B02CFFB4");

    // Hang 3: Nut khoa
    int bx=ex_; int bw2=85;
    hBtnGenKey=MakeButton(hMain,ID_BTN_GEN_KEY_ENC,"Sinh khoa",bx,py+152,bw2,24);
    hBtnLoadKeyEnc=MakeButton(hMain,ID_BTN_LOAD_KEY_ENC,"Tai khoa",bx+bw2+5,py+152,bw2,24);
    hBtnSaveKeyEnc=MakeButton(hMain,ID_BTN_SAVE_KEY_ENC,"Luu khoa",bx+bw2*2+10,py+152,bw2,24);

    // Hang 4: Ket qua ma hoa
    MakeLabel(hMain,"Ket qua ma hoa:",lx+8,py+185,lw,20);
    hEditEncResult=MakeEdit(hMain,ID_EDIT_ENC_RESULT,ex_,py+183,ew,80,true);

    // Hang 5: Nut chinh
    int abx=lx+pw/2-145;
    hBtnEncrypt=MakeButton(hMain,ID_BTN_ENCRYPT,"Ma hoa",abx,py+272,90,28);
    hBtnClearEnc=MakeButton(hMain,ID_BTN_CLEAR_ENC,"Xoa",abx+100,py+272,70,28);
    hBtnSaveEnc=MakeButton(hMain,ID_BTN_SAVE_ENC,"Luu ket qua",abx+180,py+272,110,28);

    // === PANEL PHAI: GIAI MA ===
    // Hang 1: Van ban ma hoa
    MakeLabel(hMain,"Van ban ma hoa:",rx+8,py+10,lw,20);
    hEditCipherInput=MakeEdit(hMain,ID_EDIT_CIPHER_INPUT,ex2,py+8,ew,80,true);

    // Nut tai tep ma hoa
    hBtnLoadTextDec=MakeButton(hMain,ID_BTN_LOAD_TEXT_DEC,"Tai tep ma hoa",ex2+ew-130,py+95,130,22);

    // Hang 2: Nhap khoa
    MakeLabel(hMain,"Nhap khoa:",rx+8,py+125,lw,20);
    hEditDecKey=MakeEdit(hMain,ID_EDIT_DEC_KEY,ex2,py+123,ew,22);
    SetEditText(hEditDecKey,"22F1E653B02CFFB4");

    // Hang 3: Nut khoa
    hBtnLoadKeyDec=MakeButton(hMain,ID_BTN_LOAD_KEY_DEC,"Tai khoa",ex2,py+152,bw2,24);
    hBtnSaveKeyDec=MakeButton(hMain,ID_BTN_SAVE_KEY_DEC,"Luu khoa",ex2+bw2+5,py+152,bw2,24);

    // Hang 4: Ket qua giai ma
    MakeLabel(hMain,"Ket qua giai ma:",rx+8,py+185,lw,20);
    hEditDecResult=MakeEdit(hMain,ID_EDIT_DEC_RESULT,ex2,py+183,ew,80,true);

    // Hang 5: Nut chinh
    int abx2=rx+pw/2-120;
    hBtnDecrypt=MakeButton(hMain,ID_BTN_DECRYPT,"Giai ma",abx2,py+272,90,28);
    hBtnClearDec=MakeButton(hMain,ID_BTN_CLEAR_DEC,"Xoa",abx2+100,py+272,70,28);
    hBtnSaveDec=MakeButton(hMain,ID_BTN_SAVE_DEC,"Luu ket qua",abx2+180,py+272,110,28);

    ShowWindow(hMain,nCmdShow);
    UpdateWindow(hMain);

    MSG msg;
    while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DeleteObject(hFontNormal); DeleteObject(hFontBold);
    DeleteObject(hFontTitle); DeleteObject(hFontLabel);
    DeleteObject(hBgBrush);
    return (int)msg.wParam;
}
