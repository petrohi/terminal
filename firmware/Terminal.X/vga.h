#define VIDEO_LINES          525
#define VIDEO_V_SYNC         2
#define VIDEO_V_FRONT_PORCH  10
#define VIDEO_V_BACK_PORCH   33

#define VIDEO_PIXELS         800
#define VIDEO_H_SYNC         96
#define VIDEO_H_FRONT_PORCH  16
#define VIDEO_H_BACK_PORCH   48

#define VIDEO_LINE_T   	     (VIDEO_PIXELS * 2)
#define VIDEO_H_SYNC_T       (VIDEO_H_SYNC * 2)

#define VIDEO_V_PIXELS (VIDEO_LINES - VIDEO_V_FRONT_PORCH - VIDEO_V_SYNC - VIDEO_V_BACK_PORCH)
#define VIDEO_H_PIXELS (VIDEO_PIXELS - VIDEO_H_FRONT_PORCH - VIDEO_H_SYNC - VIDEO_H_BACK_PORCH)

#define CHAR_HEIGHT    16
#define CHAR_WIDTH     8

#define SCREEN_COLS    (VIDEO_H_PIXELS / CHAR_WIDTH)

#define TAB_SIZE       8 

extern int UnderlineChar;
extern int ReverseVideoChar;
extern int InvisibleChar;

extern int CursorRow;
extern int CursorCol;
extern int CursorOff;

extern int AutoLineWrap;

extern int ScreenRows();
extern void ShowCursor(int cursor);
extern void MoveCursor(int row, int col);
extern void ClearEOL();
extern void ClearEOS();
extern void ClearBOL();
extern void ClearBOS();
extern void ClearScreen();
extern void ScrollUp();
extern void ScrollDown();

extern void PutChar(char c);
extern void PutChars(char* s);

extern void InitVga(int marginLines);

extern void SetPixel(int x, int y);
extern void DrawLine(int x1, int y1, int x2, int y2);
extern void DrawBox(int x1, int y1, int x2, int y2, int fill);
extern void DrawCircle(int x, int y, int radius, int fill, float aspect);