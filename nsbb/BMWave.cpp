//---------------------------------------------------------------------------
#include <vcl.h>
#include <dsgnintf.hpp>
#include <mmsystem.h>
#pragma hdrstop

#include "BMWave.h"

#define __USE_BMMEDIADIALOGS__

#ifdef __USE_BMMEDIADIALOGS__
  #include "BMMediaDialogs.h"
#endif

#pragma package(smart_init)


#ifdef __USE_BMMEDIADIALOGS__
 typedef TBMOpenMediaDialog TWOpenDialog;
 typedef TBMSaveMediaDialog TWSaveDialog;

#else
 typedef TOpenDialog TWOpenDialog;
 typedef TSaveDialog TWSaveDialog;

#endif
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TBMWave *)
{
    new TBMWave(NULL);
}
//---------------------------------------------------------------------------
namespace Bmwave
{

__fastcall TBMCustomSound::TBMCustomSound(TComponent* Owner)
    : inherited (Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TBMCustomSound::Play (void)
{
}
//---------------------------------------------------------------------------
bool __fastcall TBMCustomSound::Empty (void)
{
  return true;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TBMWave::TBMWave(TComponent* Owner)
    : inherited (Owner)
{
  FWave = new TBMWaveData;
}
//---------------------------------------------------------------------------
__fastcall TBMWave::~TBMWave()
{
  delete FWave;
}
//---------------------------------------------------------------------------
void __fastcall TBMWave::Play (void)
{
  FWave->Play ();
}
//---------------------------------------------------------------------------
bool __fastcall TBMWave::Empty (void)
{
  return FWave->Empty ();
}
//---------------------------------------------------------------------------
void __fastcall TBMWave::SetWave ( TBMWaveData* Value )
{
  FWave->Assign ( Value );
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TBMWaveData::TBMWaveData(void) :
  Classes::TPersistent(),
  FSize ( 0 ),
  FStoredInExe ( true )
{
}
//---------------------------------------------------------------------------
__fastcall TBMWaveData::~TBMWaveData ()
{
  Clear ();
}
//---------------------------------------------------------------------------
void __fastcall TBMWaveData::Clear ()
{
  if ( ! Empty () )
    {
    delete [] FData;
    }

  FSize = 0;
}
//---------------------------------------------------------------------------
void __fastcall TBMWaveData::Fill( long ASize, Byte *AData )
{
  Clear ();
  if ( FStoredInExe )
    if ( ASize > 0 )
      {
      FSize = ASize;
      FData = new Byte [ FSize ];
      memcpy ( FData, AData, FSize );
      }
}
//---------------------------------------------------------------------------
void __fastcall TBMWaveData::Assign( TPersistent *Source )
{
  if ( ! Source )
    Clear ();

  else
    {
    TBMWaveData *WaveData = dynamic_cast<TBMWaveData *> ( Source );
    if ( WaveData )
      {
      FileName = WaveData->FileName;
      FStoredInExe = WaveData->FStoredInExe;
      Fill ( WaveData->FSize, WaveData->FData );
      }

    else
      inherited::Assign(Source);
    }
}
//---------------------------------------------------------------------------
void __fastcall TBMWaveData::LoadFromFile( AnsiString Value )
{
  int iFileHandle;
  long  iFileLength;

  Clear ();
  if ( FileExists(Value) )
    {
    FileName = Value;
    iFileHandle = FileOpen( Value, fmOpenRead );
    iFileLength = FileSeek(iFileHandle,0,2);
    if ( iFileLength > 0 )
      {
      FSize = iFileLength;
      FData = new Byte [ FSize ];
      FileSeek(iFileHandle,0,0);
      FileRead( iFileHandle, FData, FSize );
      }

   FileClose(iFileHandle);
   }

}
//---------------------------------------------------------------------------
bool __fastcall TBMWaveData::Empty ()
{
  if ( FStoredInExe )
    return ( FSize == 0 );
    
  else
    return FileName.IsEmpty ();
}
//---------------------------------------------------------------------------
void __fastcall TBMWaveData::Play ()
{
  if ( ! Empty () )
    {
    if ( FStoredInExe )
      PlaySound ((char *)FData, Application->Handle, SND_ASYNC | SND_MEMORY | SND_NODEFAULT );
      
    else
      PlaySound ( FileName.c_str (), Application->Handle, SND_ASYNC | SND_FILENAME | SND_NODEFAULT );
    }
}
//---------------------------------------------------------------------------
bool __fastcall TBMWaveData::DoWrite ()
{
  return ! Empty ();
}
//---------------------------------------------------------------------------
void __fastcall TBMWaveData::WriteData( Classes::TStream *Stream )
{
  Stream->Write( &FStoredInExe, sizeof ( FStoredInExe ));
  int StrSize = FileName.Length ();
  Stream->Write( &StrSize, sizeof ( StrSize ));
  if ( StrSize )
    Stream->Write( FileName.c_str (), StrSize);
     
  Stream->Write( &FSize, sizeof(FSize));
  Stream->Write( FData,FSize );
}
//---------------------------------------------------------------------------
void __fastcall TBMWaveData::ReadData( Classes::TStream *Stream )
{
  Clear ();
  Stream->Read( &FStoredInExe, sizeof ( FStoredInExe ));

  int StrSize;
  Stream->Read( &StrSize,sizeof(StrSize));
  if ( StrSize )
    {
    char *Buffer = new char [ StrSize + 1];
    memset ( Buffer, '\0', StrSize + 1 ); 
    Stream->Read( Buffer, StrSize);
    FileName = Buffer;
    delete Buffer;
    }

  Stream->Read( &FSize,sizeof(FSize));
  if ( FSize>0 )
    {
    FData = new Byte [ FSize ];
    Stream->Read( FData, FSize );
    }
}
//---------------------------------------------------------------------------
void __fastcall TBMWaveData::DefineProperties(Classes::TFiler* Filer )
{
  inherited::DefineProperties(Filer);
  Filer->DefineBinaryProperty( "Data", ReadData, WriteData, DoWrite () );
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
Dsgnintf::TPropertyAttributes __fastcall TBMWaveDataProperty::GetAttributes(void)
{
  return inherited::GetAttributes () << paDialog;
}
//---------------------------------------------------------------------------
System::AnsiString __fastcall TBMWaveDataProperty::GetValue()
{
  if ( (( TBMWaveData *)GetOrdValue ())->Empty () )
    return "(Silence)";

  else
    return (( TBMWaveData *)GetOrdValue ())->FileName;
}
//---------------------------------------------------------------------------
void __fastcall TBMWaveDataProperty::SetValue(const System::AnsiString Value)
{
  if ( Value == "" )
    SetOrdValue ( NULL );
    
  inherited::SetValue( Value );
}
//---------------------------------------------------------------------------
void __fastcall TBMWaveDataProperty::Edit ()
{
  TWOpenDialog  *OpenDialog = new TWOpenDialog (Application);
  OpenDialog->Filter = "Wave files (*.wav)|*.wav|All files (*.*)|*.*";
  OpenDialog->Options = OpenDialog->Options << ofFileMustExist;
  TBMWaveData *TmpData = (TBMWaveData*)GetOrdValue ();
  if ( ! TmpData->FileName.IsEmpty () )
    {
    OpenDialog->InitialDir = ExtractFileDir ( TmpData->FileName );
    OpenDialog->FileName = ExtractFileName ( TmpData->FileName );
    }


  if ( OpenDialog->Execute () )
    {
    TmpData = new TBMWaveData;
    TmpData->LoadFromFile( OpenDialog->FileName );
    if ( Application->MessageBox( "Do you wand the sond data to be part of the executable ?", "Option", MB_YESNO ) == IDYES )
      TmpData->FStoredInExe = true;

    else
      TmpData->FStoredInExe = false;

    SetOrdValue( ( long ) TmpData );
    Modified ();
    delete TmpData;
    }

  delete OpenDialog;
}
//---------------------------------------------------------------------------
void __fastcall TBMWaveEditor::Edit(void)
{
  TWOpenDialog  *OpenDialog = new TWOpenDialog (Application);
  OpenDialog->Filter = "Wave files (*.wav)|*.wav|All files (*.*)|*.*";
  OpenDialog->Options = OpenDialog->Options << ofFileMustExist;
  if ( ! ((TBMWave *)Component)->Wave->FileName.IsEmpty () )
    {
    OpenDialog->InitialDir = ExtractFileDir ( ((TBMWave *)Component)->Wave->FileName );
    OpenDialog->FileName = ExtractFileName ( ((TBMWave *)Component)->Wave->FileName );
    }

  if ( OpenDialog->Execute () )
    {
    TBMWaveData *TmpData = new TBMWaveData;
    TmpData->LoadFromFile( OpenDialog->FileName );
    if ( Application->MessageBox( "Do you wand the sond data to be part of the executable ?", "Option", MB_YESNO ) == IDYES )
      TmpData->FStoredInExe = true;

    else
      TmpData->FStoredInExe = false;

    ((TBMWave *)Component)->Wave->Assign(TmpData);

    Designer->Modified();
    delete TmpData;
    }

  delete OpenDialog;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
    void __fastcall PACKAGE Register()
    {
      TComponentClass classes[1] = {__classid(TBMWave)};
      RegisterComponents("BMitov", classes, 0);
      RegisterPropertyEditor(__typeinfo(TBMWaveData), 0L, "", __classid(TBMWaveDataProperty));
      RegisterComponentEditor(classes[0],__classid(TBMWaveEditor));
    }
}
//---------------------------------------------------------------------------
 