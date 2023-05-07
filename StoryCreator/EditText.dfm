object EditTextForm: TEditTextForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Edit ScreenText'
  ClientHeight = 291
  ClientWidth = 431
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
    Left = 80
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
    Left = 2
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
  object Text: TMemo
    Left = 2
    Top = 34
    Width = 425
    Height = 223
    TabOrder = 0
  end
  object CardOfDialog: TEdit
    Left = 164
    Top = 7
    Width = 39
    Height = 21
    TabOrder = 1
  end
  object Confirm: TBitBtn
    Left = 260
    Top = 263
    Width = 75
    Height = 25
    Caption = 'Confirm'
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 2
    OnClick = ConfirmClick
  end
  object Discard: TBitBtn
    Left = 352
    Top = 263
    Width = 75
    Height = 25
    Caption = 'Discard'
    Kind = bkNo
    NumGlyphs = 2
    TabOrder = 3
    OnClick = DiscardClick
  end
  object ID: TEdit
    Left = 22
    Top = 7
    Width = 41
    Height = 21
    TabOrder = 4
  end
end
