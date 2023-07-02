# 项目文件作用简介
* SCharacter 玩家角色类
  * SInteractionComponent 玩家交互组件 S--SGmeplayInterface 游戏接口类
  * SAttributeComponent 玩家属性组件
  * SActionComponent 玩家行动组件
* SAction 玩家行动基类
* SHealthWidget 玩家生命UI
* SWorldUserWidget 敌人血条UI（后期仿玩家生命UI用C++完全实现）
* SHealthWidget 伤害数字UI（目前里面没东西，没搞明白C++的UCanvasPanel）
* SProjectileBase 基础抛射物
  * -SMagicProjectile 魔法攻击抛射物(已废弃)
  * -SDashProjectile 传送弹
  * -SBlackHoleProjectile 黑洞弹
* SItemChest 可交互宝箱物体 G--SGmeplayInterface 游戏接口类
* SPowerupActor 回血物品基类 G--SGmeplayInterface 游戏接口类
  * -SPowerup_HealthPotion 血瓶
* SExplosiveBarrel 炸药桶
* STargetDummy 击中后闪烁的物体
* SAICharacter AI角色类
* SAIController AI控制器
* SBTService_CheckAttackRange AI判断是否处于攻击范围内
* SBTService_CheckHealth AI判断是否血量过低
* SBTTaskRangeAttack AI远程攻击TASK
* SBTTask_HealSelf AI回血TASK
* SGameModeBase 游戏模式，自动生成敌人AI
* SGameplayFunctionLibrary 函数库