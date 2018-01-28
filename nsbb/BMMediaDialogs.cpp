//---------------------------------------------------------------------------
#include <vcl.h>
#include <consts.hpp>
#pragma hdrstop

#include "BMMediaDialogs.h"
#pragma package(smart_init)
#pragma resource "BMMediaDialogs.res"
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
namespace Bmmediadialogs
{
static inline void ValidCtrCheck(TBMOpenMediaDialog *)
{
        new TBMOpenMediaDialog(NULL);
}
//---------------------------------------------------------------------------
AnsiString GetTmpFile ( AnsiString Extention )
{
  char Buffer [ MAX_PATH + 1 ] = "";
  GetTempPath ( MAX_PATH, Buffer );

  int i = 0;

  for (;;i++)
    {
    AnsiString FileName = Buffer;

    FileName += (AnsiString)"\\BMMD" + i + Extention;

    HANDLE Return = CreateFile(

      FileName.c_str (),	// pointer to name of the file
      GENERIC_WRITE,	// access (read-write) mode
      0,	// share mode
      SECURITY_ANONYMOUS,	// pointer to security attributes
      CREATE_NEW,	// how to create
      FILE_ATTRIBUTE_TEMPORARY,	// file attributes
      0 	// handle to file with attributes to copy
      );

    if ( Return != INVALID_HANDLE_VALUE )
      {
      CloseHandle( Return );
      return FileName;
      }
    }
}
//---------------------------------------------------------------------------
__fastcall TBMMediaDialogImpl::TBMMediaDialogImpl( TOpenDialog* AOwner)
        : inherited (AOwner),
        OwnerDialog ( (TBMOpenMediaDialog *)AOwner )
{
  FTmpFile = "";

  InPreview = false;
  
  FPicturePanel = new TPanel ( this );
  IsValidView = true;

  FPicturePanel->Name = "PicturePanel";
  FPicturePanel->Caption = "";
  FPicturePanel->SetBounds(204, 5, 169, 200);
  FPicturePanel->BevelOuter = bvNone;
  FPicturePanel->BorderWidth = 6;
  FPicturePanel->TabOrder = 1;
  FPictureLabel = new TLabel (this);

  FPictureLabel->Name = "PictureLabel";
  FPictureLabel->Caption = "";
  FPictureLabel->SetBounds(6, 6, 157, 23 + 10);
  FPictureLabel->Align = alTop;
  FPictureLabel->AutoSize = false;
  FPictureLabel->Parent = FPicturePanel;

  FCommandPanel = new TPanel ( this );
  FCommandPanel->Name = "CommandPanel";
  FCommandPanel->Caption = "";
  FCommandPanel->SetBounds( 0, 2, 150, 26);
  FCommandPanel->Align = alBottom;
#ifdef __BCB_40__
  FCommandPanel->AutoSize = false;
#endif
  FCommandPanel->Parent = FPicturePanel;

  FPreviewButton = new TSpeedButton (this);

  FPreviewButton->Name = "PreviewButton";
  FPreviewButton->SetBounds(77, 1, 23, 22);
  FPreviewButton->Enabled = false;
  FPreviewButton->Glyph->LoadFromResourceName((UINT)HInstance, "PREVIEWGLYPH");
  FPreviewButton->Hint = "Show Preview"; //Consts_SPictureDesc;
  FPreviewButton->ParentShowHint = false;
  FPreviewButton->ShowHint = true;
  FPreviewButton->OnClick = PreviewClick;
  FPreviewButton->Parent = FPicturePanel;

  FPaintPanel = new TPanel (this);

  FPaintPanel->Name = "PaintPanel";
  FPaintPanel->Caption = "";
  FPaintPanel->SetBounds(6, 29, 157, 145);
  FPaintPanel->Align = alClient;
  FPaintPanel->BevelInner = bvRaised;
  FPaintPanel->BevelOuter = bvLowered;
  FPaintPanel->TabOrder = 0;
  FPaintPanel->Parent = FPicturePanel;

  FSoundImage = new TImage ( this );
  FSoundImage->Name = "SoundImage";

  FSoundImage->Picture->Bitmap->LoadFromResourceName((UINT)HInstance, "SOUNDGLYPH");
  FSoundImage->Transparent = true;
  FSoundImage->Visible = false;
  FSoundImage->Parent = FPaintPanel;

  FPlayer = new TMediaPlayer ( this );
  FPlayer->Name = "MediaPlayer";
  FPlayer->TimeFormat = tfMilliseconds;
  FPlayer->Display = FPaintPanel;
  FPlayer->Visible = false;
  
  FTimer = new TTimer ( this );
  FTimer->OnTimer = MediaNotify;
  FTimer->Interval = 50;

  FUpdateTimer = new TTimer ( this );
  FUpdateTimer->OnTimer = UpdateNotify;
  FUpdateTimer->Enabled = false;
  FUpdateTimer->Interval = 1500;

  FLoopCheckBox = new TCheckBox (this);
  FLoopCheckBox->Name = "LoopCheckBox";
  FLoopCheckBox->SetBounds( 100, 5, 50, 15);
  FLoopCheckBox->Checked = true;
  FLoopCheckBox->Caption = "Loop";
  FLoopCheckBox->Parent = FCommandPanel;

  FShowCheckBox = new TCheckBox (this);
  FShowCheckBox->Name = "ShowCheckBox";
  FShowCheckBox->OnClick = OnShowCheck;
  FShowCheckBox->Checked = true;
  FShowCheckBox->SetBounds( 6, 5, 90, 15);
  FShowCheckBox->Caption = "Show preview";
  FShowCheckBox->Parent = FCommandPanel;
//  FPlayer->SetBounds(0, 0, 153, 141);
//  FPlayer->VisibleButtons = Mplayer::TButtonSet () << btPlay << btStop << btPause;
  FPlayer->VisibleButtons = FPlayer->VisibleButtons >> btEject >> btRecord;
}
//---------------------------------------------------------------------------
bool __fastcall TBMMediaDialogImpl::ValidFile ( AnsiString FileName )
{
    return GetFileAttributes(PChar(FileName.c_str ())) != 0xFFFFFFFF;
}
//---------------------------------------------------------------------------
void __fastcall TBMMediaDialogImpl::OnShowCheck (System::TObject* Sender)
{
  FLoopCheckBox->Enabled = FShowCheckBox->Checked;

  if ( FShowCheckBox->Checked && FPlayer->Capabilities.Contains ( mpCanPlay ))
    FPlayer->Play ();
}
//---------------------------------------------------------------------------
void __fastcall TBMMediaDialogImpl::DoFolderChange(void)
{
  if ( ! FTmpFile.IsEmpty ())
    {
    FPlayer->Wait = true;
    FPlayer->Close ();
    DeleteFile ( FTmpFile );
    }

  FTmpFile = "";
  FSoundImage->Visible = false;
  FPreviewButton->Enabled = false;
  FPictureLabel->Caption = "";
//  inherited::DoFolderChange();
}
//---------------------------------------------------------------------------
void __fastcall TBMMediaDialogImpl::DoSelectionChange(void)
{
  FPreviewButton->Enabled = false;
  FUpdateTimer->Enabled = false;
  IsValidView = false;
  FPictureLabel->Enabled = false;
  FUpdateTimer->Enabled = true;
  if ( FPlayer->Mode == mpPlaying )
    FPlayer->Pause ();
}
//---------------------------------------------------------------------------
void __fastcall TBMMediaDialogImpl::DoClose(void)
{
  FPlayer->Wait = true;
  FPlayer->Close ();
}
//---------------------------------------------------------------------------
void __fastcall TBMMediaDialogImpl::DoShow(void)
{
  TRect PreviewRect, StaticRect;

  // Set preview area to entire dialog
  GetClientRect ( OwnerDialog->Handle, (RECT*)&PreviewRect );
  StaticRect = OwnerDialog->GetStaticRect ();
  // Move preview area to right of static area
  PreviewRect.Left = StaticRect.Left + (StaticRect.Right - StaticRect.Left);
  PreviewRect.Top += 4;
//  PreviewRect.Bottom -= FPlayer->Height;
  FPicturePanel->BoundsRect = PreviewRect;
  FPreviewButton->Left = FPaintPanel->BoundsRect.Right - FPreviewButton->Width - 2;
//  FPicture->Assign(NULL);
  FPicturePanel->ParentWindow = OwnerDialog->Handle;

//  FPlayer->DisplayRect = PreviewRect;

/*
  PreviewRect.Top = PreviewRect.Bottom;
  PreviewRect.Bottom += FPlayer->Height;
  FPlayer->BoundsRect = PreviewRect;
*/
  FPlayer->ParentWindow = OwnerDialog->Handle;
}
//---------------------------------------------------------------------------
void __fastcall TBMMediaDialogImpl::ExecuteEnter(void)
{
  if ( NewStyleControls && ! ( OwnerDialog->Options.Contains ( ofOldStyleDialog ) ))
    OwnerDialog->Template = "DLGTEMPLATE";

  else
    OwnerDialog->Template = NULL;

}
//---------------------------------------------------------------------------
bool IsVideo ( MCIDEVICEID DeviceID )
{
    MCI_GETDEVCAPS_PARMS DevCapParm;

    DWORD FFlags = MCI_WAIT | MCI_GETDEVCAPS_ITEM;

    DevCapParm.dwCallback = NULL;
    DevCapParm.dwItem = MCI_GETDEVCAPS_HAS_VIDEO;
    mciSendCommand( DeviceID, MCI_GETDEVCAPS, FFlags,  (long int) &DevCapParm );
    return (bool) DevCapParm.dwReturn;
}
//---------------------------------------------------------------------------
void __fastcall TBMMediaDialogImpl::UpdateNotify (System::TObject* Sender)
{
  FUpdateTimer->Enabled = false;
  IsValidView = true;

  FPictureLabel->Enabled = true;

//  AnsiString FullName;
  bool ValidPicture;

  ValidPicture = FileExists(OwnerDialog->FileName) && ValidFile(OwnerDialog->FileName);

  if ( CurrentFileName == OwnerDialog->FileName )
    {
    if ( FPlayer->Mode == mpPaused )
      FPlayer->Resume ();

    FPreviewButton->Enabled = IsVideo ( FPlayer->DeviceID );
    return;
    }

  FSoundImage->Left = ( FPaintPanel->Width - 32 ) / 2;
  FSoundImage->Top = ( FPaintPanel->Height - 32 ) / 2;
  
  CurrentFileName = OwnerDialog->FileName;
  FPaintPanel->Invalidate ();
  if ( ValidPicture )
  try
    {
    if ( ! FTmpFile.IsEmpty ())
      {
      FPlayer->Wait = true;
      FPlayer->Close ();
      DeleteFile ( FTmpFile );
      }

    FTmpFile = GetTmpFile ( ExtractFileExt ( CurrentFileName ));
    if ( CopyFile ( CurrentFileName.c_str (), FTmpFile.c_str (), false ))
      FPlayer->FileName = FTmpFile;

    else
      FPlayer->FileName = CurrentFileName;

    FPlayer->Open ();
    OriginalDisplayRect = FPlayer->DisplayRect;
    TRect DisplayRect = ::Rect ( 6, 6, FPaintPanel->Width - 2 * 6, FPaintPanel->Height - 2 * 6 );
    int VDisplay = DisplayRect.Bottom - DisplayRect.Top;
    int VOld = OriginalDisplayRect.Bottom - OriginalDisplayRect.Top;
    int HDisplay = DisplayRect.Right - DisplayRect.Left;
    int HOld = OriginalDisplayRect.Right - OriginalDisplayRect.Left;

    CalculatedRect = DisplayRect;
//    if ( FPlayer->Capabilities.Contains ( mpUsesWindow ) )
    if ( IsVideo ( FPlayer->DeviceID ))
      {
      if ( VOld * HDisplay > HOld * VDisplay )
        {
        int Hor =  VDisplay * HOld / VOld;
        CalculatedRect.Right = CalculatedRect.Left + Hor;
        }

      else
        {
        int Vert = HDisplay * VOld / HOld;
        CalculatedRect.Bottom = CalculatedRect.Top + Vert;
        }

      FPlayer->DisplayRect = CalculatedRect; //::Rect ( 6, 6, FPaintPanel->Width - 2 * 6, FPaintPanel->Height - 2 * 6 );
      CurrentRect = CalculatedRect;
      }

//    int TheLength = FPlayer->Length;
//    AnsiString LenString = IntToStr(LOBYTE(LOWORD(TheLength)) & 0xF ) + ":" + IntToStr(LOBYTE(LOWORD(TheLength)) >> 4 ) + ":" + IntToStr(HIBYTE(LOWORD(TheLength)) & 0xF );
//    AnsiString LenString = (AnsiString)( TheLength / 1000 ) + "." + ( TheLength % 1000 ) + " Sec";
//    if ( FPlayer->Capabilities.Contains ( mpUsesWindow ) )
    if ( IsVideo ( FPlayer->DeviceID ))
      {
      FPictureLabel->Caption = (AnsiString)"Movie : " + OriginalDisplayRect.Right + " : " + OriginalDisplayRect.Bottom; // + "\nTime : " +  LenString;
      }

    else
      {
//      FPictureLabel->Caption = (AnsiString)"Sound : " +  LenString;
      FPictureLabel->Caption = (AnsiString)"Sound";
      }

  if ( FShowCheckBox->Checked )
    FPlayer->Play ();
/*
    FPicture->LoadFromFile(FullName);
    TVarRec Rec [ 2 ];
    Rec [ 0 ] = FPicture->Width;
    Rec [ 1 ] = FPicture->Height;

    FPictureLabel->Caption = Format(Consts_SPictureDesc, Rec, 2);
*/
    FPreviewButton->Enabled = IsVideo ( FPlayer->DeviceID );
/*
    if ( ! FPreviewButton->Enabled )
      FPlayer->Display = NULL;

    else
      FPlayer->Display = FPaintPanel;
*/

    FSoundImage->Visible = ! FPreviewButton->Enabled;
    }

  catch (...)
    {
    ValidPicture = false;
    }

  if ( ! ValidPicture )
    {
    FPictureLabel->Caption = ""; //Consts_SPictureLabel;
    FPreviewButton->Enabled = false;
    FSoundImage->Visible = false;
//    FPicture->Assign(NULL);
    }

//  FPaintBox->Invalidate ();
//  inherited::DoSelectionChange ();
}
//---------------------------------------------------------------------------
void __fastcall TBMMediaDialogImpl::ExecuteExit(void)
{
  if ( ! FTmpFile.IsEmpty ())
    DeleteFile ( FTmpFile );
}
//---------------------------------------------------------------------------
void __fastcall TBMMediaDialogImpl::MediaNotify (System::TObject* Sender)
{
  FPlayer->DisplayRect = CurrentRect; //::Rect ( 6, 6, FPaintPanel->Width - 2 * 6, FPaintPanel->Height - 2 * 6 );

  if ( !IsValidView )
    return;

  if ( InPreview )
    return;

  if ( FShowCheckBox->Checked )
    {
    if ( FLoopCheckBox->Checked )
      if ( FPlayer->Mode == mpStopped )
        FPlayer->Play ();

    }

  else
    if ( FPlayer->Mode == mpStopped )
      FPaintPanel->Invalidate ();

}
//---------------------------------------------------------------------------
void __fastcall TBMMediaDialogImpl::OnResize (System::TObject* Sender)
{
  CurrentRect.Right = PreviewForm->ClientWidth - 4;//= OriginalDisplayRect.Right - OriginalDisplayRect.Left + 4; //FPicture->Width + (PreviewForm->ClientWidth - Panel->ClientWidth)+ 10;
  CurrentRect.Bottom = PreviewForm->ClientHeight - 4;//= OriginalDisplayRect.Bottom - OriginalDisplayRect.Top + 4;//FPicture->Height + (PreviewForm->ClientHeight - Panel->ClientHeight) + 10;

//  FPreviewPlayer->DisplayRect = CurrentRect;
  FPlayer->DisplayRect = CurrentRect;
}
//---------------------------------------------------------------------------
void __fastcall TBMMediaDialogImpl::OnPreviewShow (System::TObject* Sender)
{
}
//---------------------------------------------------------------------------
void __fastcall TBMMediaDialogImpl::PreviewClick(System::TObject* Sender)
{
  TPanel *Panel;

  PreviewForm = new TForm (this);
//  with PreviewForm do
  try
    {
    PreviewForm->Name = "PreviewForm";
    PreviewForm->Caption = Consts_SPreviewLabel;
    PreviewForm->BorderStyle = bsSizeToolWin;
    PreviewForm->KeyPreview = true;
    PreviewForm->Position = poScreenCenter;
    PreviewForm->OnKeyPress = PreviewKeyPress;

/*
    FPreviewPlayer = new TMediaPlayer ( PreviewForm );
    FPreviewPlayer->Name = "PreviewPlayer";
    FPlayer->Wait = true;
    FPlayer->Close ();
    FPreviewPlayer->FileName = FPlayer->FileName;
    FPreviewPlayer->TimeFormat = tfMilliseconds;
    FPreviewPlayer->AutoRewind = true;
    FPreviewPlayer->VisibleButtons = FPreviewPlayer->VisibleButtons >> btEject >> btRecord;
    FPreviewPlayer->Align = alBottom;
    FPreviewPlayer->Parent = PreviewForm;
    FPreviewPlayer->Open ();
*/
    Panel = new TPanel ( PreviewForm );
    Panel->Name = "Panel";
    Panel->Caption = "";
    Panel->Align = alClient;
    Panel->BevelOuter = bvNone;
    Panel->BorderStyle = bsSingle;
    Panel->BorderWidth = 5;
    Panel->Color = clWindow;
    Panel->Parent = PreviewForm;
    Panel->OnResize = OnResize;

    PreviewForm->ClientWidth = OriginalDisplayRect.Right - OriginalDisplayRect.Left + 4; //FPicture->Width + (PreviewForm->ClientWidth - Panel->ClientWidth)+ 10;
    PreviewForm->ClientHeight = OriginalDisplayRect.Bottom - OriginalDisplayRect.Top + 4 + FPlayer->Height;//FPicture->Height + (PreviewForm->ClientHeight - Panel->ClientHeight) + 10;

    FPaintPanel->Invalidate ();
    InPreview = true;
//    FPreviewPlayer->Display = Panel;
    FPlayer->Display = Panel;
    FPlayer->ParentWindow = NULL;
    FPlayer->Parent = PreviewForm;
    FPlayer->Align = alBottom;
    FPlayer->Visible = true;
    FPlayer->Stop ();
    PreviewForm->OnShow = OnPreviewShow;
    CurrentRect = OriginalDisplayRect;
    FPlayer->Rewind ();
    PreviewForm->ShowModal ();
    InPreview = false;
    FPlayer->Parent = NULL;
    FPlayer->ParentWindow = OwnerDialog->Handle;
    FPlayer->Display = FPaintPanel;
//    FPreviewPlayer->Close ();
//    FPlayer->Open ();
    CurrentRect = CalculatedRect;
    }

  __finally
    {
    delete PreviewForm;
    }

}            
//---------------------------------------------------------------------------
void __fastcall TBMMediaDialogImpl::PreviewKeyPress(System::TObject* Sender, char &Key)
{
  if ( Key == 27 )
    (( TForm *)Sender)->Close ();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TBMOpenMediaDialog::TBMOpenMediaDialog(TComponent* Owner)
        : inherited (Owner)
{
  Options = Options << ofShareAware;

  FMediaDialogImpl = new TBMMediaDialogImpl ( this );

  Filter = "All media (*.wav;*.mid;*.avi)|*.wav;*.mid;*.avi|Wave File (*.wav)|*.wav|MIDI File (*.mid)|*.mid|Video (*.avi)|*.avi|All files (*.*)|*.*";
}
//---------------------------------------------------------------------------
__fastcall TBMOpenMediaDialog::~TBMOpenMediaDialog(void)
{
  delete FMediaDialogImpl;
}
//---------------------------------------------------------------------------
bool __fastcall TBMOpenMediaDialog::GetAutoShow ()
{
  return FMediaDialogImpl->FShowCheckBox->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TBMOpenMediaDialog::SetAutoShow ( bool Value )
{
  FMediaDialogImpl->FShowCheckBox->Checked = Value;
}
//---------------------------------------------------------------------------
bool __fastcall TBMOpenMediaDialog::GetLoop ()
{
  return FMediaDialogImpl->FLoopCheckBox->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TBMOpenMediaDialog::SetLoop ( bool Value )
{
  FMediaDialogImpl->FLoopCheckBox->Checked = Value;
}
//---------------------------------------------------------------------------
void __fastcall TBMOpenMediaDialog::DoFolderChange(void)
{
  FMediaDialogImpl->DoFolderChange();
  inherited::DoFolderChange();
}
//---------------------------------------------------------------------------
void __fastcall TBMOpenMediaDialog::DoSelectionChange(void)
{
  FMediaDialogImpl->DoSelectionChange();
  inherited::DoSelectionChange();
}
//---------------------------------------------------------------------------
void __fastcall TBMOpenMediaDialog::DoClose(void)
{
  FMediaDialogImpl->DoClose ();
  inherited::DoClose ();
  // Hide any hint windows left behind
  Application->HideHint ();
}
//---------------------------------------------------------------------------
void __fastcall TBMOpenMediaDialog::DoShow(void)
{
  FMediaDialogImpl->DoShow ();
  inherited::DoShow ();
}
//---------------------------------------------------------------------------
bool __fastcall TBMOpenMediaDialog::Execute(void)
{
  FMediaDialogImpl->ExecuteEnter ();
  int Result = inherited::Execute ();
  FMediaDialogImpl->ExecuteExit ();

  return Result;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TBMSaveMediaDialog::TBMSaveMediaDialog(TComponent* Owner)
        : inherited (Owner)
{
  Options = Options << ofShareAware;

  FMediaDialogImpl = new TBMMediaDialogImpl ( this );

  Filter = "All media (*.wav;*.mid;*.avi)|*.wav;*.mid;*.avi|Wave File (*.wav)|*.wav|MIDI File (*.mid)|*.mid|Video (*.avi)|*.avi|All files (*.*)|*.*";
}
//---------------------------------------------------------------------------
__fastcall TBMSaveMediaDialog::~TBMSaveMediaDialog(void)
{
  delete FMediaDialogImpl;
}
//---------------------------------------------------------------------------
bool __fastcall TBMSaveMediaDialog::GetAutoShow ()
{
  return FMediaDialogImpl->FShowCheckBox->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TBMSaveMediaDialog::SetAutoShow ( bool Value )
{
  FMediaDialogImpl->FShowCheckBox->Checked = Value;
}
//---------------------------------------------------------------------------
bool __fastcall TBMSaveMediaDialog::GetLoop ()
{
  return FMediaDialogImpl->FLoopCheckBox->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TBMSaveMediaDialog::SetLoop ( bool Value )
{
  FMediaDialogImpl->FLoopCheckBox->Checked = Value;
}
//---------------------------------------------------------------------------
void __fastcall TBMSaveMediaDialog::DoFolderChange(void)
{
  FMediaDialogImpl->DoFolderChange();
  inherited::DoFolderChange();
}
//---------------------------------------------------------------------------
void __fastcall TBMSaveMediaDialog::DoSelectionChange(void)
{
  FMediaDialogImpl->DoSelectionChange();
  inherited::DoSelectionChange();
}
//---------------------------------------------------------------------------
void __fastcall TBMSaveMediaDialog::DoClose(void)
{
  FMediaDialogImpl->DoClose ();
  inherited::DoClose ();
  // Hide any hint windows left behind
  Application->HideHint ();
}
//---------------------------------------------------------------------------
void __fastcall TBMSaveMediaDialog::DoShow(void)
{
  FMediaDialogImpl->DoShow ();
  inherited::DoShow ();
}
//---------------------------------------------------------------------------
bool __fastcall TBMSaveMediaDialog::Execute(void)
{
  FMediaDialogImpl->ExecuteEnter ();
  int Result = inherited::Execute ();
  FMediaDialogImpl->ExecuteExit ();

  return Result;
}
//---------------------------------------------------------------------------
  void __fastcall PACKAGE Register()
  {
    TComponentClass classes[2] = {__classid(TBMOpenMediaDialog), __classid(TBMSaveMediaDialog)};
    RegisterComponents("Dialogs", classes, 1);
  }
}
//---------------------------------------------------------------------------
