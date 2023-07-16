// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Math/TransformNonVectorized.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Animation/AnimMontage.h"
#include "Component/SInteractionComponent.h"
#include "Component/SAttributeComponent.h"
#include "Component/SActionComponent.h"
#include "Component/SWeaponComponent.h"
#include "CollisionShape.h"
#include "CollisionQueryParams.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ��ӵ��ɱ����
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true; // �������Pawn��תͬ��
	// ���������
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
	// ��ӽ������
	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
	// ����������
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	// ����ж����
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
	// �������
	WeaponComp = CreateDefaultSubobject<USWeaponComponent>("WeaponComp");

	GetCharacterMovement()->bOrientRotationToMovement = true; // ��ɫ���˶�������ת
	bUseControllerRotationYaw = false; // ���ý�ɫ���Ƶ�������ת
}

// ע���¼��ص�����
void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// ע�������ײ�¼�������  
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::GetHealthChange);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// �������ӳ��
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//// Use the mouse scroll wheel to zoom in and out
	//float ZoomSpeed = 1.0f;
	//FVector newLocation = SpringArmComp->GetRelativeLocation() + SpringArmComp->GetForwardVector()* ZoomSpeed;
	//SpringArmComp->SetRelativeLocation(newLocation);


}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		// ��Ծ
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		// �ƶ�
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		// ��
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASCharacter::Look);
		// ��
		EnhancedInputComponent->BindAction(PrimaryParryAction, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryParry);
		// ��ͨ����
		EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryAttack);
		// ���
		EnhancedInputComponent->BindAction(PrimaryDashAction, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryDash);
		// ���ٿ�ʼ
		EnhancedInputComponent->BindAction(PrimarySprintDown, ETriggerEvent::Started, this, &ASCharacter::PrimarySprintStart);
		// ���ٽ���
		EnhancedInputComponent->BindAction(PrimarySprintRelease, ETriggerEvent::Triggered, this, &ASCharacter::PrimarySprintEnd);
		// ������ʼ
		EnhancedInputComponent->BindAction(AccumulateDown, ETriggerEvent::Started, this, &ASCharacter::AccumulateStart);
		// ��������
		EnhancedInputComponent->BindAction(AccumulateRelease, ETriggerEvent::Triggered, this, &ASCharacter::AccumulateEnd);
		// �ӵ������л�
		EnhancedInputComponent->BindAction(BulletTypeRightAction, ETriggerEvent::Triggered, this, &ASCharacter::BulletTypeRight);
		// �ӵ������л�
		EnhancedInputComponent->BindAction(BulletTypeLeftAction, ETriggerEvent::Triggered, this, &ASCharacter::BulletTypeLeft);
		// ��ɱ
		EnhancedInputComponent->BindAction(PrimaryMustKillAction, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryMustKill);
		// ��������
		EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryInteract);
	}
}


// �����ж�
void ASCharacter::GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// ȷ���������������
	if (NewHealth <= 0.0f && Delta <= 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);

		SetLifeSpan(5.0f);
	}
}

// ��������Ŀ���̨����
void ASCharacter::HealSelf(float Amount /* 100.0f */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

// ��ȡ���λ������ת
FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ɫ����

// ��ɫ�ƶ�����
void ASCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

// ��ɫ��꿴����
void ASCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// ��
void ASCharacter::PrimaryParry()
{
	ActionComp->StartActionByName(this, "Parry");
}

// ��ͨ����
void ASCharacter::PrimaryAttack()
{

	ActionComp->StartActionByName(this, "Fire");
}

// ���(Ŀǰ����߼����ͷų���ӵ����������ݺ���ͨ����һ���ˡ���)
void ASCharacter::PrimaryDash()
{
	//ActionComp->StartActionByName(this, "PrimaryDash");
	ActionComp->StartActionByName(this, "Dash");
	
}

// ���ٿ�ʼ
// �����ж������Sprint�ж�
void ASCharacter::PrimarySprintStart()
{
	UE_LOG(LogTemp, Log, TEXT("Sprint Start"));
	ActionComp->StartActionByName(this, "Sprint");
}

// ���ٽ���
// �����ж������Sprint�ж�
void ASCharacter::PrimarySprintEnd()
{
	UE_LOG(LogTemp, Log, TEXT("Sprint End"));
	ActionComp->StopActionByName(this, "Sprint");
}

// ������ʼ
void ASCharacter::AccumulateStart()
{
	UE_LOG(LogTemp, Log, TEXT("Accumulate Start"));
	ActionComp->StartActionByName(this, "Accumulate");
}
// ��������
void ASCharacter::AccumulateEnd()
{
	UE_LOG(LogTemp, Log, TEXT("Accumulate End"));
	ActionComp->StopActionByName(this, "Accumulate");
}

// �ӵ������л�
void ASCharacter::BulletTypeRight()
{
	UE_LOG(LogTemp, Log, TEXT("BulletTypeRight"));
	ActionComp->StartActionByName(this, "BulletTypeRight");
}
// �ӵ������л�
void ASCharacter::BulletTypeLeft()
{
	UE_LOG(LogTemp, Log, TEXT("BulletTypeLeft"));
	ActionComp->StartActionByName(this, "BulletTypeLeft");
}

// ��ɱ(Ŀǰ��ɱ�߼����ͷźڶ����������ݺ���ͨ����һ���ˡ���)
void ASCharacter::PrimaryMustKill()
{
	ActionComp->StartActionByName(this, "PrimaryMustKill");
}

// ��������
void ASCharacter::PrimaryInteract()
{
	// ���ý������
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
	
}
