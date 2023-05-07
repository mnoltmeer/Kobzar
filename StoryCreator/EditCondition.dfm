object EditConditionForm: TEditConditionForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Edit Condition'
  ClientHeight = 230
  ClientWidth = 426
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
    Left = 127
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
    Left = 270
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
    Left = 127
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
    Left = 270
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
  object Label7: TLabel
    Left = 8
    Top = 72
    Width = 58
    Height = 16
    Caption = 'Condition:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object CardOfDialog: TEdit
    Left = 213
    Top = 7
    Width = 41
    Height = 21
    TabOrder = 0
  end
  object Confirm: TBitBtn
    Left = 259
    Top = 198
    Width = 75
    Height = 25
    Caption = 'Confirm'
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 1
    OnClick = ConfirmClick
  end
  object Discard: TBitBtn
    Left = 349
    Top = 198
    Width = 75
    Height = 25
    Caption = 'Discard'
    Kind = bkNo
    NumGlyphs = 2
    TabOrder = 2
    OnClick = DiscardClick
  end
  object NextCardOfDialog: TEdit
    Left = 382
    Top = 7
    Width = 41
    Height = 21
    TabOrder = 3
  end
  object ID: TEdit
    Left = 23
    Top = 7
    Width = 41
    Height = 21
    TabOrder = 4
  end
  object LinkedID: TEdit
    Left = 213
    Top = 33
    Width = 41
    Height = 21
    TabOrder = 5
  end
  object LinkedFromID: TEdit
    Left = 382
    Top = 33
    Width = 41
    Height = 21
    TabOrder = 6
  end
  object Condition: TEdit
    Left = 72
    Top = 71
    Width = 329
    Height = 21
    TabOrder = 7
  end
  object Text: TMemo
    Left = 8
    Top = 112
    Width = 415
    Height = 71
    TabOrder = 8
  end
end
