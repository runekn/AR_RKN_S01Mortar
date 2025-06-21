[BaseContainerProps()]
class RKN_AnyPlayerInstigatorDamageCondition : RKN_DamageCondition
{
	override bool Init(IEntity entity, notnull BaseDamageContext damageContext)
	{
		return damageContext.instigator.GetInstigatorPlayerID() != 0;
	}
}