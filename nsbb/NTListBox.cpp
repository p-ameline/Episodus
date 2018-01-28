#include "nsbb\NTListBox.h"

DEFINE_RESPONSE_TABLE1(GListBox, TListBox)
   EV_WM_KEYDOWN,
END_RESPONSE_TABLE;



void GListBox::PrintArray(char *input[])
{
  int size =  sizeof(input);
  for (register int i =0; i < size; i++)
    AddString(input[i]);
}


void GListBox::EvKeyDown(uint key, uint /* repeatCount */, uint /* flags */)
{
  switch(key)
  {
    case VK_DELETE :
      break;
  }
}


