#include "Player/AuraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"

class UEnhancedInputLocalPlayerSubsystem;

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	check(AuraContext);

	auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	
	Subsystem->AddMappingContext(AuraContext, 0);
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
}

void AAuraPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CursorTrace();	
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();
	
	if (LastActor == nullptr && ThisActor != nullptr)
	{
		ThisActor->HighlightActor();
	}
	if (LastActor != nullptr)
	{
		if (ThisActor == nullptr)
		{
			LastActor->UnHighlightActor();
		}
		else if (LastActor != ThisActor)
		{
			LastActor->UnHighlightActor();
			ThisActor->HighlightActor();
		}	
	}
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const auto InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	
	const auto ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const auto RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if(auto ControllerPawn = GetPawn<APawn>())
	{
		ControllerPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControllerPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
	
}