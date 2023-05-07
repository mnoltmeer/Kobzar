object EditScriptForm: TEditScriptForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Edit Script'
  ClientHeight = 372
  ClientWidth = 536
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Position = poOwnerFormCenter
  OnShow = FormShow
  TextHeight = 13
  object Label1: TLabel
    Left = 82
    Top = 8
    Width = 80
    Height = 16
    Caption = 'CardOfDialog:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 4
    Top = 8
    Width = 17
    Height = 16
    Caption = 'ID:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 4
    Top = 38
    Width = 71
    Height = 16
    Caption = 'Parameters:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 2
    Top = 346
    Width = 147
    Height = 16
    Caption = 'Save result in Parameter:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object CardOfDialog: TEdit
    Left = 166
    Top = 7
    Width = 39
    Height = 21
    TabOrder = 0
  end
  object Confirm: TBitBtn
    Left = 363
    Top = 343
    Width = 75
    Height = 25
    Caption = 'Confirm'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Kind = bkOK
    NumGlyphs = 2
    ParentFont = False
    TabOrder = 1
    OnClick = ConfirmClick
  end
  object Discard: TBitBtn
    Left = 455
    Top = 343
    Width = 75
    Height = 25
    Caption = 'Discard'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Kind = bkNo
    NumGlyphs = 2
    ParentFont = False
    TabOrder = 2
    OnClick = DiscardClick
  end
  object ID: TEdit
    Left = 24
    Top = 7
    Width = 41
    Height = 21
    TabOrder = 3
  end
  object Params: TEdit
    Left = 81
    Top = 37
    Width = 448
    Height = 21
    TabOrder = 4
  end
  object Text: TRichEdit
    Left = 2
    Top = 64
    Width = 527
    Height = 273
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssBoth
    TabOrder = 5
    WantTabs = True
    OnKeyPress = TextKeyPress
    OnKeyUp = TextKeyUp
  end
  object OpenInIDE: TButton
    Left = 448
    Top = 6
    Width = 82
    Height = 25
    Hint = 'Open script in ELI IDE if installed'
    Caption = 'Open in Editor'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 6
    OnClick = OpenInIDEClick
  end
  object ResultParam: TEdit
    Left = 155
    Top = 343
    Width = 190
    Height = 21
    TabOrder = 7
  end
end
