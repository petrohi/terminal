#define SCREEN_COLS 80
#define SCREEN_ROWS 20

extern void ClearScreen();
extern void ScrollUp();
extern void ScrollDown();
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
