// Copyright Narrative Tools 2022. 

#include "DialogueEditorDetails.h"
#include "DetailLayoutBuilder.h"
#include "Dialogue.h"
#include "DialogueBlueprint.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "DialogueSM.h"
#include "Widgets/Input/SComboBox.h"
#include "IPropertyUtilities.h"

#define LOCTEXT_NAMESPACE "DialogueEditorDetails"

TSharedRef<IDetailCustomization> FDialogueEditorDetails::MakeInstance()
{
	return MakeShareable(new FDialogueEditorDetails);
}


FText FDialogueEditorDetails::GetSpeakerText() const
{
	//When argument changes auto-update the description
	if (LayoutBuilder)
	{
		TArray<TWeakObjectPtr<UObject>> EditedObjects;
		LayoutBuilder->GetObjectsBeingCustomized(EditedObjects);

		if (EditedObjects.IsValidIndex(0))
		{
			if (UDialogueNode_NPC* NPCNode = Cast<UDialogueNode_NPC>(EditedObjects[0].Get()))
			{
				return FText::FromName(NPCNode->SpeakerID);
			}
		}
	}

	return LOCTEXT("SpeakerText", "None");
}

TSharedRef<SWidget> FDialogueEditorDetails::MakeWidgetForOption(TSharedPtr<FText> InOption)
{
	return SNew(STextBlock).Text(*InOption);
}

void FDialogueEditorDetails::OnSelectionChanged(TSharedPtr<FText> NewSelection, ESelectInfo::Type SelectInfo)
{
	//When argument changes auto-update the description
	if (LayoutBuilder)
	{
		TArray<TWeakObjectPtr<UObject>> EditedObjects;
		LayoutBuilder->GetObjectsBeingCustomized(EditedObjects);

		if (EditedObjects.IsValidIndex(0))
		{
			if (UDialogueNode_NPC* NPCNode = Cast<UDialogueNode_NPC>(EditedObjects[0].Get()))
			{
				NPCNode->SpeakerID = FName(NewSelection->ToString());
			}
		}
	}
}

FReply FDialogueEditorDetails::SetTransformsFromActorSelection()
{
	//TArray<TWeakObjectPtr<UObject>> EditedObjects;
	//LayoutBuilder->GetObjectsBeingCustomized(EditedObjects);

	//if (EditedObjects.IsValidIndex(0))
	//{
	//	if (UDialogue* Dialogue = Cast<UDialogue>(EditedObjects[0].Get()))
	//	{
	//		

	//		LayoutBuilder->ForceRefreshDetails();
	//	}
	//}

	return FReply::Handled();
}

void FDialogueEditorDetails::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	LayoutBuilder = &DetailLayout;

	TArray<TWeakObjectPtr<UObject>> EditedObjects;
	DetailLayout.GetObjectsBeingCustomized(EditedObjects);

	IDetailCategoryBuilder& Category = DetailLayout.EditCategory("Details");

	FText GroupLabel(LOCTEXT("DetailsGroup", "Details"));

	if (EditedObjects.Num() > 0 && EditedObjects.IsValidIndex(0))
	{
		if (UDialogueNode_NPC* NPCNode = Cast<UDialogueNode_NPC>(EditedObjects[0].Get()))
		{
			if (!NPCNode->OwningDialogue)
			{
				return;
			}

			if (UDialogueBlueprint* DialogueBP = Cast<UDialogueBlueprint>(NPCNode->OwningDialogue->GetOuter()))
			{
				if (UDialogue* DialogueCDO = Cast<UDialogue>(DialogueBP->GeneratedClass->GetDefaultObject()))
				{

					for (const auto& Speaker : DialogueCDO->Speakers)
					{
						SpeakersList.Add(MakeShareable(new FText(FText::FromName(Speaker.SpeakerID))));

						if (Speaker.SpeakerID == NPCNode->SpeakerID)
						{
							SelectedItem = SpeakersList.Last();
						}
					}

					FText RowText = LOCTEXT("SpeakerIDLabel", "Speaker");

					//Add a button to make the quest designer more simplified 
					FDetailWidgetRow& Row = Category.AddCustomRow(GroupLabel)
						.NameContent()
						[
							SNew(STextBlock)
							.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
						.Text(RowText)
						]
					.ValueContent()
						[
							SNew(SComboBox<TSharedPtr<FText>>)
							.OptionsSource(&SpeakersList)
						.OnSelectionChanged(this, &FDialogueEditorDetails::OnSelectionChanged)
						.InitiallySelectedItem(SelectedItem)
						.OnGenerateWidget_Lambda([](TSharedPtr<FText> Option)
							{
								return SNew(STextBlock)
									.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
									.Text(*Option);
							})
						[
							SNew(STextBlock)
							.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
								.Text(this, &FDialogueEditorDetails::GetSpeakerText)
						]
						];
				}
			}
		}
		//else if (UDialogue* DialogueCDO = Cast<UDialogue>(EditedObjects[0]))
		//{
		//	//Add a button to make the quest designer more simplified 
		//	Category.AddCustomRow(GroupLabel)
		//		.ValueContent()
		//		[
		//			SNew(SButton)
		//			.ButtonStyle(FAppStyle::Get(), "RoundButton")
		//		.OnClicked(this, &FDialogueEditorDetails::SetTransformsFromActorSelection)
		//		[
		//			SNew(STextBlock)
		//			.Font(IDetailLayoutBuilder::GetDetailFontBold())
		//			.ToolTipText(LOCTEXT("SetSpeakerTransformsTooltip", "")
		//			LOCTEXT("SetSpeakerTransforms", "Set Speaker Transforms From Selection"))
		//		]
		//		];
		//}
	}
}


//TSharedRef<IPropertyTypeCustomization> FSpeakerSelectorCustomization::MakeInstance()
//{
//	return MakeShareable(new FSpeakerSelectorCustomization());
//}
//
//void FSpeakerSelectorCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
//{
//	HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()];
//
//	TArray<UObject*> OuterObjects;
//
//	PropertyHandle->GetOuterObjects(OuterObjects);
//
//	UDialogueNode* DialogueNode = nullptr;
//
//	for (auto& Obj : OuterObjects)
//	{
//		if (UDialogueNode* NodeObj = Cast<UDialogueNode>(Obj))
//		{
//			DialogueNode = NodeObj;
//			break;
//		}
//	}
//
//	if (UDialogueBlueprint* DialogueBP = Cast<UDialogueBlueprint>(DialogueNode->OwningDialogue->GetOuter()))
//	{
//		Dialogue = Cast<UDialogue>(DialogueBP->GeneratedClass->GetDefaultObject());
//	}
//
//	if(Dialogue)
//	{
//		for (const auto& Speaker : Dialogue->Speakers)
//		{
//			SpeakersList.Add(MakeShareable(new FName(Speaker.SpeakerID)));
//
//			//if (Speaker.SpeakerID == NPCNode->SpeakerID)
//			//{
//			//	SelectedItem = SpeakersList.Last();
//			//}
//		}
//
//		//Add a button to make the quest designer more simplified 
//		ComboBox = SNew(SComboBox<TSharedPtr<FName>>)
//			.OptionsSource(&SpeakersList)
//			.OnSelectionChanged(this, &FSpeakerSelectorCustomization::OnSelectionChanged)
//			.InitiallySelectedItem(SelectedItem)
//			.OnGenerateWidget_Lambda([](TSharedPtr<FText> Option)
//				{
//					return SNew(STextBlock)
//						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
//						.Text(*Option);
//				})
//			[
//				SNew(STextBlock)
//				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
//					.Text(this, &FDialogueEditorDetails::GetSpeakerText)
//			];
//
//
//	}
//}
//
//void FSpeakerSelectorCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
//{
//}
//
////FText FSpeakerSelectorCustomization::GetSpeakerText() const
////{
////
////}
////
////TSharedRef<SWidget> FSpeakerSelectorCustomization::MakeWidgetForOption(TSharedPtr<FText> InOption)
////{
////
////}
//
//void FSpeakerSelectorCustomization::OnSelectionChanged(TSharedPtr<FText> NewSelection, ESelectInfo::Type SelectInfo)
//{
//
//}
//
//void FSpeakerSelectorCustomization::UpdateProperty()
//{
//
//}

#undef LOCTEXT_NAMESPACE