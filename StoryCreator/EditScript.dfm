object EditScriptForm: TEditScriptForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Edit Script'
  ClientHeight = 345
  ClientWidth = 594
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Position = poOwnerFormCenter
  OnClose = FormClose
  OnShow = FormShow
  TextHeight = 13
  object Label1: TLabel
    Left = 82
    Top = 8
    Width = 40
    Height = 16
    Caption = 'Dialog:'
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
    Left = 187
    Top = 8
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
  object Dialog: TEdit
    Left = 128
    Top = 8
    Width = 39
    Height = 21
    TabOrder = 0
    OnChange = DialogChange
    OnExit = EditExit
    OnKeyUp = EditKeyUp
  end
  object ID: TEdit
    Left = 27
    Top = 8
    Width = 41
    Height = 21
    TabOrder = 1
    OnChange = IDChange
    OnExit = EditExit
    OnKeyUp = EditKeyUp
  end
  object Params: TEdit
    Left = 264
    Top = 8
    Width = 322
    Height = 21
    TabOrder = 2
    OnChange = ParamsChange
  end
  object Text: TRichEdit
    Left = 3
    Top = 35
    Width = 587
    Height = 302
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssBoth
    TabOrder = 3
    WantTabs = True
    OnChange = TextChange
    OnKeyPress = TextKeyPress
    OnKeyUp = TextKeyUp
  end
end
