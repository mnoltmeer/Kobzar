//---------------------------------------------------------------------------

#ifndef SplashH
#define SplashH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>
//---------------------------------------------------------------------------
class TSplashForm : public TForm
{
__published:	// IDE-managed Components
	TImage *SplashImage;
	TTimer *SplashCloseTimer;
	TLabel *LbVersion;
	TLabel *LbTitle;
	void __fastcall SplashCloseTimerTimer(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TSplashForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSplashForm *SplashForm;
//---------------------------------------------------------------------------
#endif
