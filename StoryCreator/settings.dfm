object AppSettings: TAppSettings
  Left = 88
  Top = 93
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Settings'
  ClientHeight = 168
  ClientWidth = 409
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Position = poOwnerFormCenter
  OnShow = FormShow
  TextHeight = 13
  object Label2: TLabel
    Left = 8
    Top = 8
    Width = 62
    Height = 16
    Caption = 'Editor Font'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 264
    Top = 8
    Width = 24
    Height = 16
    Caption = 'Size'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object SaveSettings: TButton
    Left = 226
    Top = 128
    Width = 75
    Height = 25
    Caption = 'Save'
    TabOrder = 1
    OnClick = SaveSettingsClick
  end
  object CancelSettings: TButton
    Left = 315
    Top = 128
    Width = 80
    Height = 25
    Caption = 'Close'
    TabOrder = 0
    OnClick = CancelSettingsClick
  end
  object EnableSyntaxHighlight: TCheckBox
    Left = 8
    Top = 78
    Width = 193
    Height = 17
    Caption = 'Use Syntax Highlight in Scripts'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
  end
  object SelectFont: TButton
    Left = 320
    Top = 30
    Width = 75
    Height = 25
    Caption = 'Select Font'
    TabOrder = 3
    OnClick = SelectFontClick
  end
  object CurrentFont: TEdit
    Left = 8
    Top = 30
    Width = 241
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 4
  end
  object CurrentFontSize: TEdit
    Left = 264
    Top = 30
    Width = 41
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
  end
  object FontDialog: TFontDialog
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    MinFontSize = 8
    MaxFontSize = 20
    Options = [fdTrueTypeOnly, fdLimitSize]
    Left = 352
    Top = 64
  end
end
