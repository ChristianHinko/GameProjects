// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInventory.h"

#include "UnrealNetwork.h"
#include "SiegeCharacter.h"
#include "BaseWeapon.h"

#include "Kismet/KismetSystemLibrary.h"



void UWeaponInventory::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UWeaponInventory, WeaponInventory, COND_OwnerOnly);
	DOREPLIFETIME(UWeaponInventory, CurrentWeapon);
	DOREPLIFETIME_CONDITION(UWeaponInventory, PreviousWeapon, COND_OwnerOnly);
}


UWeaponInventory::UWeaponInventory()
{
	PrimaryComponentTick.bCanEverTick = false;

	SiegeCharacter = Cast<ASiegeCharacter>(GetOwner());
}

void UWeaponInventory::InitializeComponent()
{
	Super::InitializeComponent();


	SetIsReplicated(true);
}

void UWeaponInventory::SpawnDefaultInventory()
{
	if (SiegeCharacter->HasAuthority())
	{
		int32 DefaultInventoryClassesNum = DefaultInventoryClasses.Num();

		for (int32 i = 0; i < DefaultInventoryClassesNum; i++)
		{
			FActorSpawnParameters WeaponSpawnParameters;
			WeaponSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ABaseWeapon* NewWeapon = GetWorld()->SpawnActor<ABaseWeapon>(DefaultInventoryClasses[i], WeaponSpawnParameters);

			AddWeaponToInventory(NewWeapon, false);
		}

		if (DefaultInventoryClassesNum > 0)
		{
			if (DefaultInventoryClassesNum == 1)
			{
				CurrentWeapon = WeaponInventory[0];
				PreviousWeapon = WeaponInventory[0];
				ThirdLastWeapon = WeaponInventory[0];
			}
			else if (DefaultInventoryClassesNum == 2)
			{
				CurrentWeapon = WeaponInventory[0];
				PreviousWeapon = WeaponInventory[1];
				ThirdLastWeapon = WeaponInventory[1];
			}
			else
			{
				CurrentWeapon = WeaponInventory[0];
				PreviousWeapon = WeaponInventory[1];
				ThirdLastWeapon = WeaponInventory[2];
			}
			CurrentWeapon->OnEquip();
		}
	}
}

void UWeaponInventory::SetCurrentWeapon(ABaseWeapon* NewCurrentWeapon)
{
	if (!WeaponInventory.Contains(NewCurrentWeapon))
	{
		return;
		UKismetSystemLibrary::PrintString(this, "Not valid weapon");
	}
	if (NewCurrentWeapon == CurrentWeapon)
	{
		return;
	}


	if (!ThirdLastWeapon)
	{
		if (!PreviousWeapon)
		{
			if (!CurrentWeapon)
			{
				ThirdLastWeapon = NewCurrentWeapon;
				PreviousWeapon = NewCurrentWeapon;
				CurrentWeapon = NewCurrentWeapon;

				if(CurrentWeapon)
					CurrentWeapon->OnEquip();
			}
			else
			{
				//if(CurrentWeapon)
					CurrentWeapon->OnDequip();

				ThirdLastWeapon = CurrentWeapon;
				PreviousWeapon = CurrentWeapon;
				CurrentWeapon = NewCurrentWeapon;

				if(CurrentWeapon)
					CurrentWeapon->OnEquip();
			}
		}
		else
		{
			if (!CurrentWeapon)
			{
				ThirdLastWeapon = PreviousWeapon;
				//PreviousWeapon = PreviousWeapon;
				CurrentWeapon = NewCurrentWeapon;

				if(CurrentWeapon)
					CurrentWeapon->OnEquip();
			}
			else
			{
				//if(CurrentWeapon)
					CurrentWeapon->OnDequip();

				ThirdLastWeapon = PreviousWeapon;
				PreviousWeapon = CurrentWeapon;
				CurrentWeapon = NewCurrentWeapon;

				if(CurrentWeapon)
					CurrentWeapon->OnEquip();
			}
		}
	}
	else
	{
		if (!PreviousWeapon)
		{
			if (!CurrentWeapon)
			{
				//ThirdLastWeapon = ThirdLastWeapon;
				PreviousWeapon = ThirdLastWeapon;
				CurrentWeapon = NewCurrentWeapon;

				if(CurrentWeapon)
					CurrentWeapon->OnEquip();
			}
			else
			{
				//if(CurrentWeapon)
					CurrentWeapon->OnDequip();

				//ThirdLastWeapon = ThirdLastWeapon;
				PreviousWeapon = ThirdLastWeapon;
				CurrentWeapon = NewCurrentWeapon;

				if(CurrentWeapon)
					CurrentWeapon->OnEquip();
			}
		}
		else
		{
			if (!CurrentWeapon)
			{
				//ThirdLastWeapon = ThirdLastWeapon;
				//PreviousWeapon = PreviousWeapon;
				CurrentWeapon = NewCurrentWeapon;

				if(CurrentWeapon)
					CurrentWeapon->OnEquip();
			}
			else
			{
				//if(CurrentWeapon)
					CurrentWeapon->OnDequip();

				ThirdLastWeapon = PreviousWeapon;
				PreviousWeapon = CurrentWeapon;
				CurrentWeapon = NewCurrentWeapon;

				if(CurrentWeapon)
					CurrentWeapon->OnEquip();
			}
		}
	}
}
void UWeaponInventory::SetCurrentWeapon(int32 NewCurrentWeaponIndex)
{
	if (!WeaponInventory.IsValidIndex(NewCurrentWeaponIndex))
	{
		return;
		UKismetSystemLibrary::PrintString(this, "Not valid weapon");
	}
	if (WeaponInventory[NewCurrentWeaponIndex] == CurrentWeapon)
	{
		return;
	}


	if (!ThirdLastWeapon)
	{
		if (!PreviousWeapon)
		{
			if (!CurrentWeapon)
			{
				ThirdLastWeapon = WeaponInventory[NewCurrentWeaponIndex];
				PreviousWeapon = WeaponInventory[NewCurrentWeaponIndex];
				CurrentWeapon = WeaponInventory[NewCurrentWeaponIndex];

				if(CurrentWeapon)
					CurrentWeapon->OnEquip();
			}
			else
			{
				//if(CurrentWeapon)
					CurrentWeapon->OnDequip();

				ThirdLastWeapon = CurrentWeapon;
				PreviousWeapon = CurrentWeapon;
				CurrentWeapon = WeaponInventory[NewCurrentWeaponIndex];

				if(CurrentWeapon)
					CurrentWeapon->OnEquip();
			}
		}
		else
		{
			if (!CurrentWeapon)
			{
				ThirdLastWeapon = PreviousWeapon;
				//PreviousWeapon = PreviousWeapon;
				CurrentWeapon = WeaponInventory[NewCurrentWeaponIndex];

				if(CurrentWeapon)
					CurrentWeapon->OnEquip();
			}
			else
			{
				//if(CurrentWeapon)
					CurrentWeapon->OnDequip();

				ThirdLastWeapon = PreviousWeapon;
				PreviousWeapon = CurrentWeapon;
				CurrentWeapon = WeaponInventory[NewCurrentWeaponIndex];

				if(CurrentWeapon)
					CurrentWeapon->OnEquip();
			}
		}
	}
	else
	{
		if (!PreviousWeapon)
		{
			if (!CurrentWeapon)
			{
				//ThirdLastWeapon = ThirdLastWeapon;
				PreviousWeapon = ThirdLastWeapon;
				CurrentWeapon = WeaponInventory[NewCurrentWeaponIndex];

				if(CurrentWeapon)
					CurrentWeapon->OnEquip();
			}
			else
			{
				//if(CurrentWeapon)
					CurrentWeapon->OnDequip();

				//ThirdLastWeapon = ThirdLastWeapon;
				PreviousWeapon = ThirdLastWeapon;
				CurrentWeapon = WeaponInventory[NewCurrentWeaponIndex];

				if(CurrentWeapon)
					CurrentWeapon->OnEquip();
			}
		}
		else
		{
			if (!CurrentWeapon)
			{
				//ThirdLastWeapon = ThirdLastWeapon;
				//PreviousWeapon = PreviousWeapon;
				CurrentWeapon = WeaponInventory[NewCurrentWeaponIndex];

				if(CurrentWeapon)
					CurrentWeapon->OnEquip();
			}
			else
			{
				//if(CurrentWeapon)
					CurrentWeapon->OnDequip();

				ThirdLastWeapon = PreviousWeapon;
				PreviousWeapon = CurrentWeapon;
				CurrentWeapon = WeaponInventory[NewCurrentWeaponIndex];

				if(CurrentWeapon)
					CurrentWeapon->OnEquip();
			}
		}
	}
}

void UWeaponInventory::SwitchWeapon()
{
	SetCurrentWeapon(PreviousWeapon);
}

void UWeaponInventory::AddWeaponToInventory(ABaseWeapon* WeaponToAdd, bool equipWeapon)
{
	if (SiegeCharacter->HasAuthority())
	{
		if (WeaponToAdd)
		{
			WeaponInventory.Add(WeaponToAdd);
			WeaponToAdd->OnAddedToInventory(SiegeCharacter);

			if (equipWeapon == true)
			{
				SetCurrentWeapon(WeaponToAdd);
			}
		}
	}
}

void UWeaponInventory::RemoveWeaponFromInventory(ABaseWeapon* WeaponToRemove, bool removeSpecificInstance)
{
	if (WeaponToRemove)
	{
		int32 weaponRemoveIndex;
		if (removeSpecificInstance == true)
		{
			if (!WeaponInventory.Find(WeaponToRemove, weaponRemoveIndex))
			{
				return;
			}
		}
		else
		{
			if (!WeaponInventory.FindLast(WeaponToRemove, weaponRemoveIndex))
			{
				return;
			}
		}

		if (WeaponInventory.IsValidIndex(weaponRemoveIndex))
		{
			if (CurrentWeapon == WeaponInventory[weaponRemoveIndex])
			{
				CurrentWeapon->OnDequip();
				CurrentWeapon = PreviousWeapon;
				CurrentWeapon->OnEquip();

				PreviousWeapon = ThirdLastWeapon;
			}
			else if (PreviousWeapon == WeaponInventory[weaponRemoveIndex])
			{
				PreviousWeapon = ThirdLastWeapon;
			}
			WeaponInventory.RemoveAt(weaponRemoveIndex);

			WeaponInventory[weaponRemoveIndex]->OnRemovedFromInventory();
		}
	}
}


void UWeaponInventory::OnRepWeaponInventory(TArray<ABaseWeapon*> PreRepInventory)
{
	int greatestNumberOfEitherArray = WeaponInventory.Num();
	if (PreRepInventory.Num() > greatestNumberOfEitherArray)
	{
		greatestNumberOfEitherArray = PreRepInventory.Num(); // if weapons were generally removed from inventory
	}

	for (int i = 0; i < greatestNumberOfEitherArray; i++)
	{
		if (WeaponInventory[i] != PreRepInventory[i])
		{
			if(PreRepInventory[i])
				PreRepInventory[i]->OnRemovedFromInventory();
			if(WeaponInventory[i])
				WeaponInventory[i]->OnAddedToInventory(SiegeCharacter);
		}
	}

	if(CurrentWeapon)
		CurrentWeapon->OnEquip();
}
