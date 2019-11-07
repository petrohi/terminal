#define LINE_T   	   1600
#define PIXEL_T        192

#define LINES          525
#define SYNC           2
#define FRONT_PORCH    (10 + 160)
#define BACK_PORCH     33

#define VIDEO_V_PIXELS (LINES - FRONT_PORCH - SYNC - BACK_PORCH)
#define VIDEO_H_PIXELS 640

#define CHAR_HEIGHT    16
#define CHAR_WIDTH     8

#define SCREEN_COLS    (VIDEO_H_PIXELS / CHAR_WIDTH)
#define SCREEN_ROWS    (VIDEO_V_PIXELS / CHAR_HEIGHT)

#define TAB_SIZE       8 

extern void ClearScreen();
extern void ScrollUp();
extern void ScrollDown();

extern int UnderlineChar;
extern int ReverseVideoChar;
extern int InvisibleChar;

extern void DrawChar(int row, int col, char c);

extern int CursorRow;
extern int CursorCol;
extern int CursorOff;

extern void ShowCursor(int cursor);
extern void MoveCursor(int row, int col);
extern void ClearEOL();
extern void ClearEOS();
extern void ClearBOL();
extern void ClearBOS();

extern int AutoLineWrap;

extern void PutChar(char c);
extern void PutChars(char* s);

extern void InitVga();

extern void SetPixel(int x, int y);
extern void DrawLine(int x1, int y1, int x2, int y2);
extern void DrawBox(int x1, int y1, int x2, int y2, int fill);
extern void DrawCircle(int x, int y, int radius, int fill, float aspect);