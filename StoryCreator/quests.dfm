object QuestManager: TQuestManager
  Left = 0
  Top = 0
  Caption = 'Quest Manager'
  ClientHeight = 527
  ClientWidth = 824
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Position = poOwnerFormCenter
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  TextHeight = 13
  object QuestStates: TValueListEditor
    Left = 254
    Top = 22
    Width = 570
    Height = 505
    Align = alRight
    Color = clBtnFace
    PopupMenu = PopupStates
    TabOrder = 0
    OnSelectCell = QuestStatesSelectCell
    OnValidate = QuestStatesValidate
    ColWidths = (
      87
      477)
  end
  object QuestList: TValueListEditor
    Left = 0
    Top = 22
    Width = 273
    Height = 505
    Align = alLeft
    Color = clBtnFace
    PopupMenu = PopupQuests
    TabOrder = 1
    OnSelectCell = QuestListSelectCell
    OnValidate = QuestListValidate
    ColWidths = (
      104
      163)
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 824
    Height = 22
    Align = alTop
    Caption = 'Panel1'
    TabOrder = 2
    object QuestLibPath: TEdit
      Left = 1
      Top = 1
      Width = 801
      Height = 20
      Align = alLeft
      ReadOnly = True
      TabOrder = 0
      ExplicitHeight = 21
    end
    object SetQuestLibPath: TButton
      Left = 796
      Top = 1
      Width = 33
      Height = 20
      Align = alRight
      Caption = '...'
      TabOrder = 1
      OnClick = SetQuestLibPathClick
      ExplicitLeft = 790
    end
  end
  object PopupStates: TPopupMenu
    Left = 632
    Top = 152
    object PPAddState: TMenuItem
      Caption = 'Add State'
      OnClick = PPAddStateClick
    end
    object PPDelState: TMenuItem
      Caption = 'Delete State'
      OnClick = PPDelStateClick
    end
  end
  object PopupQuests: TPopupMenu
    Left = 632
    Top = 216
    object PPAddQuest: TMenuItem
      Caption = 'New Quest'
      OnClick = PPAddQuestClick
    end
    object PPDelQuest: TMenuItem
      Caption = 'Delete Quest'
      OnClick = PPDelQuestClick
    end
  end
  object OpenQuestLibFile: TOpenDialog
    DefaultExt = 'xml'
    Filter = 'quests files (.xml)|*.xml'
    Left = 704
    Top = 152
  end
end
