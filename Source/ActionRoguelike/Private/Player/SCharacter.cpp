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

	// 添加弹簧臂组件
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true; // 摄像机与Pawn旋转同步
	// 添加相机组件
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
	// 添加交互组件
	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
	// 添加属性组件
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	// 添加行动组件
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
	// 武器组件
	WeaponComp = CreateDefaultSubobject<USWeaponComponent>("WeaponComp");

	GetCharacterMovement()->bOrientRotationToMovement = true; // 角色朝运动方向旋转
	bUseControllerRotationYaw = false; // 禁用角色控制的左右旋转
}

// 注册事件回调函数
void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 注册组件碰撞事件处理函数  
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::GetHealthChange);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// 添加输入映射
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

		// 跳跃
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		// 移动
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		// 看
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASCharacter::Look);
		// 格挡
		EnhancedInputComponent->BindAction(PrimaryParryAction, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryParry);
		// 普通攻击
		EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryAttack);
		// 冲刺
		EnhancedInputComponent->BindAction(PrimaryDashAction, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryDash);
		// 加速开始
		EnhancedInputComponent->BindAction(PrimarySprintDown, ETriggerEvent::Started, this, &ASCharacter::PrimarySprintStart);
		// 加速结束
		EnhancedInputComponent->BindAction(PrimarySprintRelease, ETriggerEvent::Triggered, this, &ASCharacter::PrimarySprintEnd);
		// 蓄力开始
		EnhancedInputComponent->BindAction(AccumulateDown, ETriggerEvent::Started, this, &ASCharacter::AccumulateStart);
		// 蓄力结束
		EnhancedInputComponent->BindAction(AccumulateRelease, ETriggerEvent::Triggered, this, &ASCharacter::AccumulateEnd);
		// 子弹向右切换
		EnhancedInputComponent->BindAction(BulletTypeRightAction, ETriggerEvent::Triggered, this, &ASCharacter::BulletTypeRight);
		// 子弹向左切换
		EnhancedInputComponent->BindAction(BulletTypeLeftAction, ETriggerEvent::Triggered, this, &ASCharacter::BulletTypeLeft);
		// 必杀
		EnhancedInputComponent->BindAction(PrimaryMustKillAction, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryMustKill);
		// 交互物体
		EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryInteract);
	}
}


// 死亡判断
void ASCharacter::GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// 确认死亡后禁用输入
	if (NewHealth <= 0.0f && Delta <= 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);

		SetLifeSpan(5.0f);
	}
}

// 治疗自身的控制台命令
void ASCharacter::HealSelf(float Amount /* 100.0f */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

// 获取相机位置与旋转
FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 角色输入

// 角色移动函数
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

// 角色鼠标看函数
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

// 格挡
void ASCharacter::PrimaryParry()
{
	ActionComp->StartActionByName(this, "Parry");
}

// 普通攻击
void ASCharacter::PrimaryAttack()
{

	ActionComp->StartActionByName(this, "Fire");
}

// 冲刺(目前冲刺逻辑是释放冲刺子弹，所以内容和普通攻击一样了。。)
void ASCharacter::PrimaryDash()
{
	//ActionComp->StartActionByName(this, "PrimaryDash");
	ActionComp->StartActionByName(this, "Dash");
	
}

// 加速开始
// 开启行动组件中Sprint行动
void ASCharacter::PrimarySprintStart()
{
	UE_LOG(LogTemp, Log, TEXT("Sprint Start"));
	ActionComp->StartActionByName(this, "Sprint");
}

// 加速结束
// 结束行动组件中Sprint行动
void ASCharacter::PrimarySprintEnd()
{
	UE_LOG(LogTemp, Log, TEXT("Sprint End"));
	ActionComp->StopActionByName(this, "Sprint");
}

// 蓄力开始
void ASCharacter::AccumulateStart()
{
	UE_LOG(LogTemp, Log, TEXT("Accumulate Start"));
	ActionComp->StartActionByName(this, "Accumulate");
}
// 蓄力结束
void ASCharacter::AccumulateEnd()
{
	UE_LOG(LogTemp, Log, TEXT("Accumulate End"));
	ActionComp->StopActionByName(this, "Accumulate");
}

// 子弹向右切换
void ASCharacter::BulletTypeRight()
{
	UE_LOG(LogTemp, Log, TEXT("BulletTypeRight"));
	ActionComp->StartActionByName(this, "BulletTypeRight");
}
// 子弹向左切换
void ASCharacter::BulletTypeLeft()
{
	UE_LOG(LogTemp, Log, TEXT("BulletTypeLeft"));
	ActionComp->StartActionByName(this, "BulletTypeLeft");
}

// 必杀(目前必杀逻辑是释放黑洞，所以内容和普通攻击一样了。。)
void ASCharacter::PrimaryMustKill()
{
	ActionComp->StartActionByName(this, "PrimaryMustKill");
}

// 交互物体
void ASCharacter::PrimaryInteract()
{
	// 调用交互组件
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
	
}
