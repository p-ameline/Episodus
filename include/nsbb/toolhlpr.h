#if !defined(TOOL_TOOLHLPR_H)
#define TOOL_TOOLHLPR_H

#if !defined(WINSYS_COLOR_H)
# include <winsys/color.h>
#endif
#if !defined(WINSYS_GEOMETRY_H)
# include <winsys/geometry.h>
#endif

typedef DWORD HLSCOLOR;

#define HLS(h,l,s) ((HLSCOLOR)(((BYTE)(h)|((WORD)((BYTE)(l))<<8))|(((DWORD)(BYTE)(s))<<16)))

#define HLS_H(hls) ((BYTE)(hls))
#define HLS_L(hls) ((BYTE)(((WORD)(hls))>>8))
#define HLS_S(hls) ((BYTE)((hls)>>16))

// Graphic helper functions
//
TColor LightenColor(TColor color,  BYTE value);
TColor DarkenColor (TColor color,  BYTE value);
TColor MergeColors (TColor color1, TColor color2);

void FillSolidRect(HDC hdc, int x, int y, int cx, int cy, TColor color);
void FillSolidRect(HDC hdc, const TRect& rect, TColor color);

void Draw3dRect(HDC hdc, int x, int y, int cx, int cy, TColor topLeftColor,
                TColor bottomRightColor);
void Draw3dRect(HDC hdc, const TRect& rect, TColor topLeftColor,
                TColor bottomRightColor);

HLSCOLOR RGB2HLS(TColor color);
TColor   HLS2RGB(HLSCOLOR hls);

TColor HLS_TRANSFORM(TColor rgb, int percent_L, int percent_S);

static BYTE _ToRGB(float rm1, float rm2, float rh);

#endif  // TOOL_TOOLHLPR_H
