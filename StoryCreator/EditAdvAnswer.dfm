object EditAdvAnswerForm: TEditAdvAnswerForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Edit Adv.Answer'
  ClientHeight = 246
  ClientWidth = 454
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
    Left = 153
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
  object Label2: TLabel
    Left = 296
    Top = 8
    Width = 105
    Height = 16
    Caption = 'NextCardOfDialog:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 3
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
  object Label4: TLabel
    Left = 153
    Top = 34
    Width = 53
    Height = 16
    Caption = 'LinkedID:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 296
    Top = 34
    Width = 83
    Height = 16
    Caption = 'LinkedFromID:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label6: TLabel
    Left = 216
    Top = 72
    Width = 99
    Height = 16
    Caption = 'NeedQuestValue:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label7: TLabel
    Left = 216
    Top = 97
    Width = 89
    Height = 16
    Caption = 'SetQuestValue:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label8: TLabel
    Left = 3
    Top = 70
    Width = 71
    Height = 16
    Caption = 'QuestName:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label9: TLabel
    Left = 24
    Top = 35
    Width = 60
    Height = 16
    Caption = 'End Dialog'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object CardOfDialog: TEdit
    Left = 239
    Top = 7
    Width = 41
    Height = 21
    TabOrder = 0
  end
  object Text: TMemo
    Left = 3
    Top = 128
    Width = 446
    Height = 81
    TabOrder = 1
  end
  object Confirm: TBitBtn
    Left = 284
    Top = 215
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
    TabOrder = 2
    OnClick = ConfirmClick
  end
  object Discard: TBitBtn
    Left = 374
    Top = 215
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
    TabOrder = 3
    OnClick = DiscardClick
  end
  object NextCardOfDialog: TEdit
    Left = 408
    Top = 7
    Width = 41
    Height = 21
    TabOrder = 4
  end
  object ID: TEdit
    Left = 23
    Top = 7
    Width = 41
    Height = 21
    TabOrder = 5
  end
  object LinkedID: TEdit
    Left = 239
    Top = 33
    Width = 41
    Height = 21
    TabOrder = 6
  end
  object LinkedFromID: TEdit
    Left = 408
    Top = 33
    Width = 41
    Height = 21
    TabOrder = 7
  end
  object NeedQuestValue: TEdit
    Left = 321
    Top = 69
    Width = 103
    Height = 21
    TabOrder = 8
  end
  object SetQuestValue: TEdit
    Left = 321
    Top = 96
    Width = 103
    Height = 21
    TabOrder = 9
  end
  object QuestName: TEdit
    Left = 3
    Top = 92
    Width = 173
    Height = 21
    TabOrder = 10
  end
  object BtSelectQuest: TButton
    Left = 176
    Top = 90
    Width = 25
    Height = 25
    Hint = 'Select quest from QuestLibrary'
    Caption = '...'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 11
    OnClick = BtSelectQuestClick
  end
  object EndDialog: TCheckBox
    Left = 3
    Top = 35
    Width = 17
    Height = 17
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 12
  end
  object BtNeedQuest: TButton
    Left = 424
    Top = 67
    Width = 25
    Height = 25
    Hint = 'Select value from QuestLibrary'
    Caption = '...'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 13
    OnClick = BtNeedQuestClick
  end
  object BtSetQuest: TButton
    Left = 424
    Top = 94
    Width = 25
    Height = 25
    Hint = 'Select value from QuestLibrary'
    Caption = '...'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 14
    OnClick = BtSetQuestClick
  end
end
