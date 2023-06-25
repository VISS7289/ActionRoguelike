# 项目文件作用简介
* SCharacter 玩家角色类
  * SInteractionComponent 玩家交互组件 S--SGmeplayInterface 游戏接口类
  * SAttributeComponent 玩家属性组件
* SHealthWidget 玩家生命UI
* SHealthWidget 伤害数字UI（目前里面没东西，没搞明白C++的UCanvasPanel）
* SProjectileBase 基础抛射物
  * -SMagicProjectile 魔法攻击抛射物(已废弃)
  * -SDashProjectile 传送弹
  * -SBlackHoleProjectile 黑洞弹
* SItemChest 可交互宝箱物体 G--SGmeplayInterface 游戏接口类
* SExplosiveBarrel 炸药桶
* STargetDummy 击中后闪烁的物体
* SAICharacter AI角色类
* SAIController AI控制器
* SBTService_CheckAttackRange AI判断是否处于攻击范围内