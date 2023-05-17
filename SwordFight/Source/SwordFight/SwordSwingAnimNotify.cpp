#include "SwordSwingAnimNotify.h"
#include "Engine.h"
#include "SwordFightCharacter.h"
#include "SwordWeapon.h"

void USwordSwingAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = IsValid(MeshComp) ? MeshComp->GetOwner() : nullptr;
	if (IsValid(Owner))
	{
		ASwordFightCharacter* Player = Cast<ASwordFightCharacter>(Owner);
		const AController* PlayerController = IsValid(Player) ? Player->GetController() : nullptr;
		if (IsValid(PlayerController))
		{
			if (PlayerController->GetLocalRole() >= ROLE_AutonomousProxy && PlayerController->GetRemoteRole() != ROLE_AutonomousProxy)
			{
				ASwordWeapon* CurrentWeapon = Cast<ASwordWeapon>(Player->Weapon->GetChildActor());
				if (IsValid(CurrentWeapon))
				{
					CurrentWeapon->CheckAndDealDamage(Player);
				}
			}
		}
	}
}