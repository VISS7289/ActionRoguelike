// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Math/TransformNonVectorized.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "SInteractionComponent.h"
#include "Animation/AnimMontage.h"
#include "SAttributeComponent.h"
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
		// 普通攻击
		EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryAttack);
		// 冲刺
		EnhancedInputComponent->BindAction(PrimaryDashAction, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryDash);
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
	}
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

// 普通攻击
void ASCharacter::PrimaryAttack()
{
	if (ensureAlways(AttackAnim))
	{
		PlayAnimMontage(AttackAnim); // 播放蒙太奇

		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f); // 延时触发攻击
		// GetWorldTimerManager.ClearTimer(TimerHandle_PrimaryAttack);
	}
}
// 普通攻击延时
void ASCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileBaseClass);
}

// 冲刺(目前冲刺逻辑是释放冲刺子弹，所以内容和普通攻击一样了。。)
void ASCharacter::PrimaryDash()
{
	if (ensureAlways(AttackAnim))
	{
		PlayAnimMontage(AttackAnim); // 播放蒙太奇

		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryDash_TimeElapsed, 0.2f); // 延时触发攻击
		// GetWorldTimerManager.ClearTimer(TimerHandle_PrimaryAttack);
	}
}
// 冲刺延时
void ASCharacter::PrimaryDash_TimeElapsed()
{
	SpawnProjectile(ProjectileDashClass);
}

// 必杀(目前必杀逻辑是释放黑洞，所以内容和普通攻击一样了。。)
void ASCharacter::PrimaryMustKill()
{
	if (ensureAlways(AttackAnim))
	{
		PlayAnimMontage(AttackAnim); // 播放蒙太奇

		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryMustKill_TimeElapsed, 0.2f); // 延时触发攻击
		// GetWorldTimerManager.ClearTimer(TimerHandle_PrimaryAttack);
	}
}
// 必杀延时
void ASCharacter::PrimaryMustKill_TimeElapsed()
{
	SpawnProjectile(ProjectileMustKillClass);
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 工具函数

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		// 左手位置
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_02");
		
		// 胶囊检测体设置
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);
		// 设置碰撞
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		// 设置检测物
		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);
		// 设置起点，相机
		FVector TraceStart = CameraComp->GetComponentLocation();
		// 设置终点，相机到屏幕中间第一个触碰到的物体
		FVector TraceEnd = CameraComp->GetComponentLocation() + (CameraComp->GetComponentRotation().Vector() * 5000);
		// 胶囊检测
		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}
		// 根据检测结果计算旋转方向
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();


		// 角色前向位置
		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		// 设置生成物规则
		FActorSpawnParameters SpawnParames;
		SpawnParames.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParames.Instigator = this;
		// 生成抛射物
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParames);
	}
}