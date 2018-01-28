#include "nsbb\toolhlpr.h"

// Graphic helper functions
//
TColor LightenColor(TColor color, BYTE value)
{
  BYTE red   = GetRValue(color);
  BYTE green = GetGValue(color);
  BYTE blue  = GetBValue(color);

  red   = (BYTE)min(red   + value, 255);
  green = (BYTE)min(green + value, 255);
  blue  = (BYTE)min(blue  + value, 255);

  return RGB(red, green, blue);
}

TColor DarkenColor(TColor color, BYTE value)
{
  BYTE red   = GetRValue(color);
  BYTE green = GetGValue(color);
  BYTE blue  = GetBValue(color);

  red   = (BYTE)(red   >= value ? red   - value : 0);
  green = (BYTE)(green >= value ? green - value : 0);
  blue  = (BYTE)(blue  >= value ? blue  - value : 0);

  return RGB(red, green, blue);
}

TColor MergeColors(TColor color1, TColor color2)
{
  BYTE red   = (BYTE)((GetRValue(color1) + GetRValue(color2)) / 2);
  BYTE green = (BYTE)((GetGValue(color1) + GetGValue(color2)) / 2);
  BYTE blue  = (BYTE)((GetBValue(color1) + GetBValue(color2)) / 2);

  return RGB(red, green, blue);
}

void FillSolidRect(HDC hdc, int x, int y, int cx, int cy, TColor color)
{
  if (!hdc)
    return;

  ::SetBkColor(hdc, color);

  TRect rect(x, y, x + cx, y + cy);
  ::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, 0, 0, 0);
}

void FillSolidRect(HDC hdc, const TRect& rect, TColor color)
{
  if (!hdc)
    return;

  ::SetBkColor(hdc, color);
  ::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, 0, 0, 0);
}

void Draw3dRect(HDC hdc, int x, int y, int cx, int cy,
                TColor topLeftColor, TColor bottomRightColor)
{
  if (!hdc)
    return;

  FillSolidRect(hdc, x,      y,      cx - 1,  1,      topLeftColor);
  FillSolidRect(hdc, x,      y,      1,       cy - 1, topLeftColor);
  FillSolidRect(hdc, x + cx, y,     -1,       cy,     bottomRightColor);
  FillSolidRect(hdc, x,      y + cy, cx,     -1,      bottomRightColor);
}

void Draw3dRect(HDC hdc, const TRect& rect,
                TColor topLeftColor, TColor bottomRightColor)
{
  if (!hdc)
    return;

  Draw3dRect(hdc, rect.left, rect.top, rect.Width(), rect.Height(),
             topLeftColor, bottomRightColor);
}

HLSCOLOR RGB2HLS(TColor color)
{
  BYTE red   = GetRValue(color);
  BYTE green = GetGValue(color);
  BYTE blue  = GetBValue(color);

  unsigned char minval = min(red, min(green, blue));
  unsigned char maxval = max(red, max(green, blue));

  float dif = maxval - minval;
  float sum = maxval + minval;

  float luminance  = sum / 510.0;
  float saturation = 0.0;
  float hue        = 0.0;

  if (maxval != minval) {
    float rnorm = (maxval - red)   / dif;
    float gnorm = (maxval - green) / dif;
    float bnorm = (maxval - blue)  / dif;

    saturation = (luminance <= 0.5) ? dif / sum : dif / (510.0 - sum);

    if (red == maxval)
      hue = 60.0 * (6.0 + bnorm - gnorm);

    if (green == maxval)
      hue = 60.0 * (2.0 + rnorm - bnorm);

    if (blue == maxval)
      hue = 60.0 * (4.0 + gnorm - rnorm);

    if (hue > 360.0)
      hue -= 360.0;
  }

  return HLS((hue * 255) / 360, luminance * 255, saturation * 255);
}

TColor HLS2RGB(HLSCOLOR hls)
{
  BYTE h = HLS_H(hls);
  BYTE l = HLS_L(hls);
  BYTE s = HLS_S(hls);

  float hue        = h * 360 / 255.0;
  float luminance  = l       / 255.0;
  float saturation = s       / 255.0;

  if (saturation == 0.0)
    return RGB(l, l, l);

  float rm1, rm2;

  if (luminance <= 0.5)
    rm2 =              luminance + luminance * saturation;
  else
    rm2 = saturation + luminance - luminance * saturation;

  rm1 = 2.0 * luminance - rm2;

  BYTE red   = _ToRGB(rm1, rm2, hue + 120.0);
  BYTE green = _ToRGB(rm1, rm2, hue);
  BYTE blue  = _ToRGB(rm1, rm2, hue - 120.0);

  return RGB(red, green, blue);
}

TColor HLS_TRANSFORM(TColor rgb, int percent_L, int percent_S)
{
  HLSCOLOR hls = RGB2HLS(rgb);

  BYTE h = HLS_H(hls);
  BYTE l = HLS_L(hls);
  BYTE s = HLS_S(hls);

  if (percent_L > 0)
    l = BYTE(l + ((255 - l) * percent_L) / 100);
  else if (percent_L < 0)
    l = BYTE((l * (100 + percent_L)) / 100);

  if (percent_S > 0)
    s = BYTE(s + ((255 - s) * percent_S) / 100);
  else if (percent_S < 0)
    s = BYTE((s * (100 + percent_S)) / 100);

  return HLS2RGB(HLS(h, l, s));
}

static BYTE _ToRGB(float rm1, float rm2, float rh)
{
  if (rh > 360.0)
    rh -= 360.0;
  else if (rh < 0.0)
    rh += 360.0;

  if (rh < 60.0)
    rm1 += rh * (rm2 - rm1) / 60.0;
  else if (rh < 180.0)
    rm1 = rm2;
  else if (rh < 240.0)
    rm1 += (240.0 - rh) * (rm2 - rm1) / 60.0;

  return (BYTE)(rm1 * 255);
}

