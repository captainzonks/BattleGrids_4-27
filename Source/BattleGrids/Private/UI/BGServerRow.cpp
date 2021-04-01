// © 2021 Matthew Barham. All Rights Reserved.


#include "UI/BGServerRow.h"
#include "UI/BGMainMenu.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UBGServerRow::Setup(UBGMainMenu* InParent, uint32 const InIndex)
{
	Parent = InParent;
	Index = InIndex;
	if (!ensure(RowButton)) return;
	RowButton->OnClicked.AddDynamic(this, &UBGServerRow::OnRowClicked);
}

void UBGServerRow::SetServerName(FString const& InString) const
{
	ServerName->SetText(FText::FromString(InString));
}

void UBGServerRow::SetHostUser(FString const& InString) const
{
	HostUser->SetText(FText::FromString(InString));
}

void UBGServerRow::SetConnectionFraction(FString const& InString) const
{
	ConnectionFraction->SetText(FText::FromString(InString));
}

void UBGServerRow::SetRowIsSelected(bool const InSelected)
{
	bSelected = InSelected;
}

void UBGServerRow::OnRowClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Clicked Server Row"))
	Parent->SelectIndex(Index);
}
